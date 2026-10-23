# A Mini Compiler for Straight-Line C

A small, working mini-compiler built with Flex and Bison under MSYS2 UCRT64.

**Repository:** https://github.com/sHArmi25011991/CD_Group_MiniCompilerStraightLineC

**Contributors:**
- Sharmistha Chowdhury (@sHArmi25011991) — Leader (Lexer, main.c, codegen, README)
- Joyeta Mallick (@joyetamallick160-source) — Parser, IR, Makefile
- Armany Inkied (@armanyinkied-cmd) — Parser logic, Semantic checks, Tests

---

**Project Phases:**

**Phase 1: Front End (Lexer + Parser)**
- Generated a lexical analyzer using Flex.
- Generated a syntax analyzer (parser) using Bison.
- Implemented a grammar for straight-line assignments.
- Added support for the `-tokens` option to print the token stream.
- Reports lexical and syntax errors with exact line numbers.

**Phase 2: Middle End & Back End (Semantic & CodeGen)**
- **Symbol Table:** Tracks variables and their types.
- **Semantic Checks:** Detects undeclared variables and division by zero.
- **3AC Generation:** Translates expressions into Three-Address Code using temporaries (t1, t2, etc.).
- **Assembly Generation:** Translates 3AC into a simple pseudo-assembly using 4 registers (R0–R3).
- Added support for `-symtab`, `-3ac`, and `-asm` flags.

---

**Prerequisites:**
To build and run the compiler, open the MSYS2 UCRT64 terminal. First, install the required tools (only once) by running:
```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc flex bison make
Build:
To build the compiler, run:

bash
make
Run:
To run the compiler on a test file and see all stages, run:

bash
./compiler tests/correct03.c -tokens -symtab -3ac -asm
Available Options:

-tokens : print the token stream

-symtab : print the symbol table

-3ac : print the Three-Address Code

-asm : print the R0-R3 assembly output

Test Files:

tests/correct01.c ... correct05.c — Accepted

tests/error01.c ... error03.c — Rejected
