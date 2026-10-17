# A Mini Compiler for Straight-Line C

Phase 1: Lexer + Parser, built with Flex and Bison under MSYS2 UCRT64.

## Repository
https://github.com/sHArmi25011991/CD_Group_MiniCompilerStraightLineC

## Contributors
- Sharmistha Chowdhury (@sHArmi25011991) — Lexer, README
- Joyeta Mallick (@joyetamallick160-source) — Parser, Makefile
- Armany Inkied (@armanyinkied-cmd) — Main, Tests

## Build
pacman -S --needed mingw-w64-ucrt-x86_64-gcc flex bison make
make

## Run
./compiler tests/correct02.c -tokens

## Options
- `-tokens` : print token stream

## Test files
- tests/correct01.c, tests/correct02.c — accepted
- tests/error01.c, tests/error02.c — rejected
