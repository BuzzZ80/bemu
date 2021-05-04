# bemu

"Emulator" for the W-4096/0 CPU

Written by Buzz Pendarvis

# Compilation 

Create a directory and execute `cmake ..` and then `make` to compile. 

An executable called `bemu` will be created 

Programmed for linux, untested on windows.

# Use

Execute `bemu -h` for help. 

```
Usage: emulator [Options] [File]
Emulates the execution of the contents of FILE (8KiB little endian binary file) as a W-4096/0

-r                Displays register contents after a halt
-a                Displays accumulator value after a halt
-o                Displays the contents of (0xFFE) when (0xFFF) is 1

-R                Displays register contents before every instruction execution
-A                Displays accumulator value before every instruction execution
-h                Displays the help file.
```
