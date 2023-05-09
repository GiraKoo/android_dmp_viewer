# android dmp viewer

Show stack of android crash dump files with llvm tools.

Author: [GiraKoo]  
LISENCE: MIT

## Structures

- Use IMGUI as UI framework. Make Windows a top priority.
- Use [minidump_stackwalk] to parse stack.
- Use [llvm-symbolizer](https://llvm.org/docs/CommandGuide/llvm-symbolizer.html) to convert addresses to function names in stack.

## Features

- [ ] Specify **dump file path**.
- [ ] Specify **symbol file path**.
- [ ] Output stack. May show content in application.
- [ ] Convert addresses to function names.
- [ ] Save **symbol file path** in ini config file.
- [ ] Load **symbol avaliable files** from web server.
- [ ] Download crash files from Android Device via ADB.
