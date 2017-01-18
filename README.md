# GameCode

GameCode is a simple game data compiler. 
The user defines object types in a meta file and objects in content files. The meta is compiled into C++ code and the content files are compiled into binary files. A program can then include the generated C++ code to load the binary files in place.

## FAQ
* Do I have to include the license in a binary distribution?
No.
* Should I use the gamecode.exe available in the Release package or should I recompile it myself?
I am doing my best to make sure that it doesn't contain any threat but I recommend to recompile it yourself, this is provided as is. It's compiled for x64 with VS2015 Update 3.
