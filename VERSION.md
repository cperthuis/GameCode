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
