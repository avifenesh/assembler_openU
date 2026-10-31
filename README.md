# assembler_openU

Two-pass assembler in ANSI C. Final project of the Open University Systems Programming Lab course.

It reads assembly source files (`.as`) and writes the machine code (`.ob`), the external references (`.ext`) and the entry points (`.ent`).

## Build

```sh
cd FinalProj
make
```

The makefile compiles with `gcc -Wall -ansi -pedantic` and produces `./assembler`.

## Usage

Pass the file names without the `.as` extension. Each name is assembled on its own.

```sh
./assembler prog1 prog2
```

For `prog1.as` it writes `prog1.ob` next to the source, plus `prog1.ext` and `prog1.ent` when the file uses externals or entries. Nothing is written for a file with errors; each error is printed with the file name and line number.

## Language

- 16 instructions: `mov`, `cmp`, `add`, `sub`, `lea`, `clr`, `not`, `inc`, `dec`, `jmp`, `bne`, `jsr`, `red`, `prn`, `rts`, `stop`.
- Directives: `.data`, `.string`, `.entry`, `.extern`.
- Addressing modes: immediate (`#5`), direct (`LABEL`), relative (`%LABEL`) and register (`r0` to `r7`).
- Labels start with a letter and hold letters and digits. Lines are read in 80-character chunks. `;` starts a comment line.
- Code is loaded from address 100.

## Tests

`FinalProj/test` holds sample sources, and `FinalProj/output files` holds the output the assembler produced for each.
