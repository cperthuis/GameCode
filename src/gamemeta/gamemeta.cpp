#include <cstdlib>
#include <iostream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shellapi.h>
#include <Shlwapi.h>
#include <Shlobj.h>

#include "metagrammar.h"
#include "gamemetalexer.h"

#include "gamemeta.h"
#include <gamemeta/gamemetascanner.yy.h>
using namespace std;

#include "meta.h"
#include "metagenerator.h"

#define ParseAlloc MetaParseAlloc
#define Parse MetaParse
#define ParseFree MetaParseFree
#define ParseTrace MetaParseTrace

void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, GmlParserToken type, GmlParserExtra* valid);
void ParseFree(void* parser, void(*freeProc)(void*));

#ifndef NDEBUG
void ParseTrace(FILE *TraceFILE, char *zTracePrompt);
#endif

static int parseMeta(Meta *meta, const char *content, const char *outputPath) {
	
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	
	// Set up the scanner
	yyscan_t scanner;
	yymetalex_init(&scanner);

	MetaYYExtraType metaYYLexerExtra;
	memset(&metaYYLexerExtra, 0, sizeof(metaYYLexerExtra));
	metaYYLexerExtra.col = 1;
	metaYYLexerExtra.line = 1;
	yymetaset_extra(&metaYYLexerExtra, scanner);

	YY_BUFFER_STATE bufferState = yymeta_scan_string(content, scanner);

	// Set up the parser
	void* shellParser = ParseAlloc(malloc);

	//bool trace = false;
#ifndef NDEBUG
	bool trace = true;
	bool showTrace = false;
	
	FILE *fp = NULL;
	if (trace)
		fp = fopen("tmp.txt", "wb");
	if (fp)
		ParseTrace(fp, "");
#else
	bool trace = false;
	bool showTrace = false;
#endif

	
	GmlParserToken t0;
	memset(&t0, 0, sizeof(t0));

	GmlParserExtra extra;
	extra.root = NULL;
	int lexCode;
	do {
		
		YYSTYPE lexerLVal;
		memset(&lexerLVal, 0, sizeof(lexerLVal));
		lexCode = yymetalex(&lexerLVal, scanner);

		t0.iValue = lexerLVal.iValue;
		t0.fValue = lexerLVal.fValue;
		memcpy(t0.strValue, lexerLVal.strValue, sizeof(t0.strValue));
		
		extra.col = metaYYLexerExtra.col;
		extra.line = metaYYLexerExtra.line;
		extra.valid = true;
		Parse(shellParser, lexCode, t0, &extra);
	} while (lexCode > 0 && extra.valid);

	if (-1 == lexCode) {
		cerr << "The scanner encountered an error.\n";
	}

	if (!extra.valid) {
		cerr << "The parser encountered an error.\n";
	}

	int enumCount = 0;
	int objectCount = 0;
	GmlItemList *l = extra.root;
	while (l)
	{
		if (l->enumItem)
			enumCount++;
		if (l->objectItem)
			objectCount++;
		l = l->next;
	}
	
#ifndef NDEBUG
	ParseTrace(NULL, "");
	if (fp)
	{
		fclose(fp);
		if (showTrace || !extra.valid)
		{
			FILE* fp = fopen("tmp.txt", "rb");
			fseek(fp, 0, SEEK_END);
			int size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			char *buffer = new char[size + 1];
			fread(buffer, size, 1, fp);
			fclose(fp);
			buffer[size] = 0;
			printf("%s\n", buffer);
			delete[] buffer;
		}
	}
#endif

	// Cleanup the scanner and parser
	yymeta_delete_buffer(bufferState, scanner);
	yymetalex_destroy(scanner);
	ParseFree(shellParser, free);

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	double interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart*1000000.0; //micro seconds

	int metaResult = createMeta(meta, extra.root);
	if (metaResult != 0)
	{
		return -1;
	}
	if (outputPath)
		generateOutput(meta, outputPath, false);

	//gmlFreeItemList(extra.root); don't free while the meta still references the parser objects
	extra.root = NULL;

	LARGE_INTEGER end2;
	QueryPerformanceCounter(&end2);
	double interval2 = static_cast<double>(end2.QuadPart - end.QuadPart) / frequency.QuadPart*1000000.0; //micro seconds

	/*
	printf("enumCount: %i, objectCount: %i\n", enumCount, objectCount);
	printf("parsing time: %.01f ms\n", interval / 1000.0f);
	printf("generation time: %.01f ms\n", interval2 / 1000.0f);
	size_t allocationCount = getAllocationCount();
	printf("allocations left: %I64u\n", allocationCount);
	*/

	return 0;
}

static void deleteFolder(const char *path)
{
	char outPath[MAX_PATH];
	memset(outPath, 0, sizeof(outPath));
	PathCanonicalize(outPath, path);

	SHFILEOPSTRUCTA fileop;
	fileop.hwnd = NULL;    // no status display
	fileop.wFunc = FO_DELETE;  // delete operation
	fileop.pFrom = outPath;  // source file name as double null terminated string
	fileop.pTo = NULL;    // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user

	bool noRecycleBin = false;
	if (!noRecycleBin)
		fileop.fFlags |= FOF_ALLOWUNDO;

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle = NULL;
	fileop.hNameMappings = NULL;

	int ret = SHFileOperationA(&fileop); //SHFileOperation returns zero if successful; otherwise nonzero 
	//delete[] pszFrom;
	//return (0 == ret);
}

static void createFolder(const char *path, bool deleteExisting)
{
	if (deleteExisting)
		deleteFolder(path);
	SHCreateDirectoryEx(NULL, path, NULL);
}



int compileMeta(Meta *meta, bool generateMetaCode, const char *metaFilepath, const char *metaCodeOutputPath)
{
	char gExePath[MAX_PATH];
	strncpy_s(gExePath, __argv[0],sizeof(gExePath));
	GetFullPathNameA(__argv[0], sizeof(gExePath), gExePath, NULL);

	gExePath[sizeof(gExePath) - 1] = 0;
	char *p = strrchr(gExePath, '\\');
	if (p && *p)
	{
		*p = 0;
		//SetCurrentDirectory(gExePath);
	}
	
	char tmp[MAX_PATH];
	char gametypesFullpath[MAX_PATH];

	if (metaFilepath == NULL || metaFilepath[0] == 0)
	{
		sprintf_s(tmp, sizeof(tmp), "%s\\..\\test\\gametypes.txt", gExePath);
		//sprintf_s(tmp, sizeof(tmp), "%s\\..\\test\\gametypesBig.txt", gExePath);
		tmp[sizeof(tmp) - 1] = 0;
		PathCanonicalize(gametypesFullpath, tmp);
	}
	else
	{
		strncpy(gametypesFullpath, metaFilepath, sizeof(gametypesFullpath));
		gametypesFullpath[sizeof(gametypesFullpath) - 1] = 0;
	}

	char outputFullpath[MAX_PATH];
	outputFullpath[0];
	if (generateMetaCode)
	{
		if (metaCodeOutputPath == NULL || metaCodeOutputPath[0] == 0)
		{
			sprintf_s(tmp, sizeof(tmp), "%s\\..\\test\\out", gExePath);
			tmp[sizeof(tmp) - 1] = 0;
			PathCanonicalize(outputFullpath, tmp);
		}
		else
		{
			strncpy(outputFullpath, metaCodeOutputPath, sizeof(outputFullpath));
			outputFullpath[sizeof(outputFullpath) - 1] = 0;
		}
		createFolder(outputFullpath, false);
	}

	FILE *fp = NULL;
	fopen_s(&fp, gametypesFullpath, "rb");
	if (!fp)
	{
		printf("error couldn't open %s\n", gametypesFullpath);
		return 255;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *content = NULL;
	if (size)
	{
		content = new char[size+1];
		fread(content, size, 1, fp);
		content[size] = 0;
	}
	fclose(fp);

	if (content)
	{
		int metaResult = parseMeta(meta, content, generateMetaCode ? outputFullpath : NULL);
		if (metaResult != 0)
			return metaResult;
	}

	return 0;
}

#ifdef GAMEMETA_EXE
int main()
{
	initializeAllocator(100 * 1024 * 1024);

	Meta meta;
	int ret = compileMeta(&meta, true);

	resetAllocator();
	destroyAllocator();

	return ret;
}
#endif
