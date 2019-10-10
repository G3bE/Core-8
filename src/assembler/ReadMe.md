# Assembler
Gratz', you made it to the assembler part of this project. Actually this isn't a real assembler
as neither is what it translates from assembly nor is what it translates to machine code. 
It's more a translator or parser. However I will quickly describe the parts of it and my thought process behind it.

## Assembler .cpp/.h
This basically consists of one function and a half. For once we have the hash function. 
It is used to switch/case strings which is not a native feature of C++.
Instead we translate all the strings into integers switch/case can handle.
Then we have the process_file function which is the core part of the whole file.
Its purpose is it to scan through the file and translate all the lines into bytecode which can be interpreted by Core-8.
We do this by first splitting the file into to its lines and then the lines into several tokens.
Its a rather trivial task, but I will cover its internal processes in the Instructions part.
After getting all the tokens for a line, we check if it matches one of the instructions we can understand.
If it does we pass the whole line to a function which does the rest of the work or, for very simple instructions set the bytecode right away.
Also if the line is blank or some gibberish we don't know we set the instruction to 0x0 which is effectively pass.
