#include <cstdlib>
#include <iostream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shellapi.h>
#include <Shlwapi.h>
#include <Shlobj.h>

#include "codegrammar.h"
#include "codelexer.h"

#include "gamecode.h"
#include <gamecode/gamecodescanner.yy.h>
using namespace std;

#include "codegenerator.h"

#include <gamemeta/meta.h>

void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, GclParserToken type, GclParserExtra* valid);
void ParseFree(void* parser, void(*freeProc)(void*));

#ifndef NDEBUG
void ParseTrace(FILE *TraceFILE, char *zTracePrompt);
#endif

int parseData(const char *content, const char *filename, const Meta* meta, const char *outputPath) {
	
	printf("%s\n", filename);
	
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	
	// Set up the scanner
	yyscan_t scanner;
	yylex_init(&scanner);

	CodeYYExtraType codeyyLexerExtra;
	memset(&codeyyLexerExtra, 0, sizeof(codeyyLexerExtra));
	codeyyLexerExtra.col = 0;
	codeyyLexerExtra.line = 0;
	codeyyLexerExtra.meta = meta;
	yyset_extra(&codeyyLexerExtra, scanner);

	
	YY_BUFFER_STATE bufferState = yy_scan_string(content, scanner);
	bufferState->yy_bs_lineno = 0;
	bufferState->yy_bs_column = 0;

	// Set up the parser
	void* shellParser = ParseAlloc(malloc);

	//bool trace = false;
#ifndef NDEBUG
	//bool trace = true;
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

	
	GclParserToken t0;
	memset(&t0, 0, sizeof(t0));

	GclParserExtra extra;
	extra.filename = filename;
	extra.root = NULL;
	int lexCode;
	do {
		
		YYSTYPE lexerLVal;
		memset(&lexerLVal, 0, sizeof(lexerLVal));
		lexCode = yylex(&lexerLVal,scanner);

		t0.iValue = lexerLVal.iValue;
		t0.fValue = lexerLVal.fValue;
		memcpy(t0.strValue, lexerLVal.strValue, sizeof(t0.strValue));
		t0.metaObject = lexerLVal.metaObject;
		
		extra.col = codeyyLexerExtra.col;
		extra.line = codeyyLexerExtra.line;

		extra.first_column = codeyyLexerExtra.first_column;
		extra.last_column = codeyyLexerExtra.last_column;
		extra.first_line = codeyyLexerExtra.first_line;
		extra.last_line = codeyyLexerExtra.last_line;

		extra.valid = true;
		Parse(shellParser, lexCode, t0, &extra);
		
	} while (lexCode > 0 && extra.valid);

	if (-1 == lexCode) {
		cerr << "The scanner encountered an error.\n";
		return -1;
	}

	if (!extra.valid) {
		cerr << "The parser encountered an error.\n";
		return -1;
	}

	int objectCount = 0;
	GclObjectList *l = extra.root;
	while (l)
	{
		if (l->object)
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
	yy_delete_buffer(bufferState, scanner);
	yylex_destroy(scanner);
	ParseFree(shellParser, free);

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	double interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart*1000000.0; //micro seconds

	int ret = generateOutput(extra.root, meta, filename, outputPath, false);

	if (extra.root) gclFreeObjectList(extra.root);
	extra.root = NULL;

	LARGE_INTEGER end2;
	QueryPerformanceCounter(&end2);
	double interval2 = static_cast<double>(end2.QuadPart - end.QuadPart) / frequency.QuadPart*1000000.0; //micro seconds

	if (ret == 0)
	{
		/*
		printf("objectCount: %i\n", objectCount);
		printf("parsing time: %.01f ms\n", interval / 1000.0f);
		printf("generation time: %.01f ms\n", interval2 / 1000.0f);
		size_t allocationCount = getAllocationCount();
		printf("allocations left: %I64u\n", allocationCount);
		*/
	}

	return ret;
}

void deleteFolder(const char *path)
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

void createFolder(const char *path, bool deleteExisting)
{
	if (deleteExisting)
		deleteFolder(path);
	SHCreateDirectoryEx(NULL, path, NULL);
}

int compileFile(const char *inputPath, const Meta *meta, const char *outputPath)
{
	int ret = 0;

	FILE *fp = NULL;
	fopen_s(&fp, inputPath, "rb");
	if (!fp)
	{
		printf("error couldn't open %s\n", inputPath);
		ret = 255;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char *content = NULL;
		if (size)
		{
			content = new char[size + 1];
			fread(content, size, 1, fp);
			content[size] = 0;
		}
		fclose(fp);

		if (content)
		{
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			char outputFolderPath[MAX_PATH];

			_splitpath(outputPath, drive, dir, fname, ext);
			sprintf(outputFolderPath, "%s%s", drive, dir);

			//createFolder(outputFullpath,true);
			createFolder(outputFolderPath, false);
			
			ret = parseData(content, inputPath, meta, outputPath);
		}
	}
	return ret;
}


void readFolder(std::vector<std::string> &files, std::string &sourceFolder, std::string &relativeFolder, bool compileFolderRecursive)
{
	//first pass iterate through files at this level
	WIN32_FIND_DATA FindFileData;
	size_t len = sourceFolder.length();
	size_t relativeFolderLen = relativeFolder.size();
	
	if (len)
		sourceFolder += "\\*.gct";
	else
		sourceFolder += "*.gct";
	HANDLE hFind = FindFirstFile(sourceFolder.c_str(), &FindFileData);
	sourceFolder.resize(len); //sourceFolder
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			relativeFolder += FindFileData.cFileName;
			files.push_back(relativeFolder);
			relativeFolder.resize(relativeFolderLen); //relativeFolder
		}
		if (!FindNextFile(hFind, &FindFileData))
		{
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
		}
	}
	

	if (compileFolderRecursive)
	{
		if (len)
			sourceFolder += "\\*"; //sourceFolder + "\\*"
		else
			sourceFolder += "*"; //sourceFolder + "\\*"
		HANDLE hFind = FindFirstFile(sourceFolder.c_str(), &FindFileData);
		sourceFolder.resize(len); //sourceFolder + "\\"

		while (hFind != INVALID_HANDLE_VALUE)
		{
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) == 0)
				&& ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0)
				&& ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) == 0)
				)
			{
				if (!(FindFileData.cFileName[0] == '.' && (FindFileData.cFileName[1] == 0 || (FindFileData.cFileName[1] == '.' && FindFileData.cFileName[2] == 0))))
				{
					if (len)
						sourceFolder += "\\";
					sourceFolder += FindFileData.cFileName;
					relativeFolder += FindFileData.cFileName;
					relativeFolder += "\\";
					readFolder(files, sourceFolder, relativeFolder, compileFolderRecursive);
					relativeFolder.resize(relativeFolderLen);
					sourceFolder.resize(len); //sourceFolder + "\\"
				}
			}
			if (!FindNextFile(hFind, &FindFileData))
			{
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}
		}
	}
	sourceFolder.resize(len); //sourceFolder
}

char gExePath[MAX_PATH];

int main(int argc, char *argv[])
{
	strncpy_s(gExePath, __argv[0],sizeof(gExePath));
	GetFullPathNameA(__argv[0], sizeof(gExePath), gExePath, NULL);

	gExePath[sizeof(gExePath) - 1] = 0;
	char *p = strrchr(gExePath, '\\');
	if (p && *p)
	{
		*p = 0;
		//SetCurrentDirectory(gExePath);
	}
	
	#define RELATIVE_PATH "..\\test\\" //from bin dir when cmake project has been regenerated from VS build step
	//#define RELATIVE_PATH "..\\test\\" //from cmake compilation output dir, when cmake proj comes from batch script

	char tmp[MAX_PATH];
	sprintf_s(tmp, sizeof(tmp), "%s\\" RELATIVE_PATH "sample.txt", gExePath);
	tmp[sizeof(tmp) - 1] = 0;
	char inputFullpath[MAX_PATH];
	PathCanonicalize(inputFullpath, tmp);

	sprintf_s(tmp, sizeof(tmp), "%s\\" RELATIVE_PATH "dataout", gExePath);
	tmp[sizeof(tmp) - 1] = 0;
	char outputFolderFullpath[MAX_PATH];
	PathCanonicalize(outputFolderFullpath, tmp);

	sprintf_s(tmp, sizeof(tmp), "%s\\" RELATIVE_PATH "codeout", gExePath);
	tmp[sizeof(tmp) - 1] = 0;
	char codeOutputFolderFullpath[MAX_PATH];
	PathCanonicalize(codeOutputFolderFullpath, tmp);

	sprintf_s(tmp, sizeof(tmp), "%s\\" RELATIVE_PATH "scene", gExePath);
	tmp[sizeof(tmp) - 1] = 0;
	char sourceFolderPath[MAX_PATH];
	PathCanonicalize(sourceFolderPath, tmp);

	sprintf_s(tmp, sizeof(tmp), "%s\\" RELATIVE_PATH "gametypes.txt", gExePath);
	tmp[sizeof(tmp) - 1] = 0;
	char metaFullpath[MAX_PATH];
	PathCanonicalize(metaFullpath, tmp);

	char outputFullpath[MAX_PATH];
	outputFullpath[0] = 0;
	

	bool dataCompilationMode = (argc == 1);
	bool forceGenerateCode = (argc == 1);
	bool compileFolderRecursive = false;
	bool compileFolder = false;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-m") == 0)
		{
			i++;
			if (i < argc)
			{
				GetFullPathNameA(argv[i], sizeof(metaFullpath), metaFullpath, NULL);
			}
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			i++;
			if (i < argc)
			{
				GetFullPathNameA(argv[i], sizeof(outputFolderFullpath), outputFolderFullpath, NULL);
			}
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			i++;
			if (i < argc)
			{
				GetFullPathNameA(argv[i], sizeof(outputFullpath), outputFullpath, NULL);
			}
		}
		else if (strcmp(argv[i], "-c") == 0)
		{
			forceGenerateCode = true;
			i++;
			if (i < argc)
			{
				GetFullPathNameA(argv[i], sizeof(codeOutputFolderFullpath), codeOutputFolderFullpath, NULL);
			}
		}
		else if (strcmp(argv[i], "-f") == 0)
		{
			forceGenerateCode = true;
			i++;
			if (i < argc)
			{
				PathCanonicalize(sourceFolderPath, argv[i]);
			}
			else
			{
				sourceFolderPath[0] = 0;
			}
			compileFolder = true;
			dataCompilationMode = true;
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			compileFolderRecursive = true;
		}
		else
		{
			dataCompilationMode = true;
			//GetFullPathNameA(argv[i], sizeof(inputFullpath), inputFullpath, NULL);
			PathCanonicalize(inputFullpath, argv[i]);
		}
	}
	
	initializeAllocator(100 * 1024 * 1024);
	Meta meta;
	
	int ret = 0;
	if (!dataCompilationMode || forceGenerateCode)
	{
		ret = compileMeta(&meta, true, metaFullpath, codeOutputFolderFullpath);
	}
	else
		ret = compileMeta(&meta, false, metaFullpath);

	if (ret == 0 && dataCompilationMode)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		if (!compileFolder)
		{
			if (outputFullpath[0] == 0)
			{
				_splitpath(inputFullpath, drive, dir, fname, ext);
				sprintf(outputFullpath, "%s\\%s.gcb", outputFolderFullpath, fname);
			}
			compileFile(inputFullpath, &meta, outputFullpath);
		}
		else
		{
			std::vector<std::string> files;
			std::string sourceFolder = sourceFolderPath;
			std::string relativeFolder;
			readFolder(files, sourceFolder, relativeFolder, compileFolderRecursive);

			size_t count = files.size();
			for (size_t i = 0; i < count; i++)
			{
				const char *filepath = files[i].c_str();
				_splitpath(filepath, drive, dir, fname, ext);
				if (dir[0] == 0)
					sprintf(outputFullpath, "%s\\%s.gcb", outputFolderFullpath, fname);
				else
					sprintf(outputFullpath, "%s\\%s\\%s.gcb", outputFolderFullpath, dir, fname);
				compileFile(filepath, &meta, outputFullpath);
			}
		}
	}

	resetAllocator();
	destroyAllocator();
	
	return ret;
}
