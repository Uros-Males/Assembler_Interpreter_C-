# AssemblerInterpreterC++
Program in C++ that serves as a sort of interpreter of assembler-like code. 
Program is written in object-oriented paradigm and user needs to provide a file with his assembler-like code. 
For every type of instruction we have a class (for example, instruction LOOP has class Instruction_LOOP which implements its functionalities).
All those instructions inherit a base class Instruction which has pure virtual function executeInstruction() used in every 
inherited instruction.
We also have a main class Machine and another class Operand where we store our variables (for example, A,B,C,X,Y,Z...)
