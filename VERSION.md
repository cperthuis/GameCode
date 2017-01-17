## Version 0.1.7
Fixed missing pointer patching for strings in parent classes
Added option to read a list of input files. the file paths can be absolute or relative to a root folder. syntax: -i listFile [-f rootFolder]


## Version 0.1.6
Arrays can now be filled with list of values in brackets in addition to the += syntax: a = [1,2,3] is equivalent to a+=1;a+=2;a+=3.
Added support for hexadecimal floats and ints. An array can be set with hex values without repeating the 0x by using the syntax arrayMember = 0x[3F800000,FF,ABCDEF]

## Version 0.1.5
Added support for 8,16 and 64 integer types: uint8,int8,uint16,int16,uint64 and int64.

## Version 0.1.4
Fixed potential memory trash in gcInitializeVptrTable().
Added an assert macro in gc.h that the user can define to verify that the memory patch offsets stay within the boundary of the object.
Replaced #ifndef/#define FILE_H by #pragma once to prevent issues with including multiple gcAll.h from different locations.
Fixed one instance of a class prefix hardcoded to "gc" in generated code.

## Version 0.1.3
Fixed setting of booleans.

## Version 0.1.2
Support for arrays of strings. Arrays of strings were not properly supported, they were filled with random data in the binary.

## Version 0.1.1
Added a note in the README.md that binary distributions don't require any mention or attribution.

## Version 0.1.0
Initial public release.
