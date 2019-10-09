# Core-8
Core-8 is a [Chip-8](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) like interpreted programming language and virtual machine.
~~It features a compiler and linker which create files that can be executed by the interpreter.~~
At the moment it can just read the contents of a file with assembly like syntax described below.

## Syntax
The Core-8 interpreter executes from above to bottom meaning there is no main function (atm).
Every line can hold one instruction.
> From here (x) means optional.

An instruction is structured like this.
```
STMT (ARG1)(, ARG2)
```
An argument can be either one of the 16 8-bit registers, V0 - VF, the 16 bit register I, the delay timer DT or K which is only
used when getting a key. <br>
When referencing a register, [REG] is treated like an address; e.g. when register V3 holds address 0xA832,
`mov V3, 3` would set V3 to 3, while `mov [V3], 3` would set 0xA832 to 3. Also `mov V2, V3` would set V2 to 0xA832,
while `mov V2, [V3]` would take the value at 0xA832 (in this case 3) and put it into V2.
> The last part is yet WIP

I listed all instructions with their valid arguments below. Instructions with multiple possible arguments are listed repeatedly.
The list is incomplete atm. For a more complete, less good looking list view `src/cpu/cpu.h`

| Instruction |  Arguments      |   Description                      |
|-------------|:----------------|:-----------------------------------|
|cls          |None             |Clears the screen                   |
|ret          |None             |Returns from a subroutine           |
|jp           |address `a`      |Sets the PC<sup>1</sup> to `a`      |
|jp           |register `x`     |Sets the PC to the address in V`x`  |
|call         |addr `a`         |Calls a subroutine                  |
|se           |byte `b`, reg `x`|Skips next instruction if `b`==V`x` |
|se           |reg `x`, reg `y` |Skips next instruction if V`x`==V`y`|
|sne          |byte `b`, reg `x`|Skips next instruction if `b`!=V`x` |
|sne          |reg `x`, reg `y` |Skips next instruction if V`x`!=V`y`|
|mov          |reg `x`, byte `b`|Sets V`x` to `b`                    |
|mov          |reg `x`, reg `y` |Sets V`x` to the value in V`y`      |
|mov          |reg I, reg `x`   |Sets I to the value in V`x`         |
|mov          |reg `x`, DT      |Sets V`x` to the value of DT        |
|mov          |DT, rev `x`      |Sets DT to the value in V`x`        |
|mov          |[I], reg `x`     |Stores V`x` at the address in I     |
|mov          |reg `x`, [I]     |Loads V`x` from the address in I    |
|mov          |reg `x`, K       |Stores the next key press in V`x`   |

<sup>1</sup> The program counter or PC is the pointer to the current executed instruction.

