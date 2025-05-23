// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

    @pos
    M=0

(LOOP)
    @KBD
    D=M

    @WHITE
    D;JEQ

    @BLACK
    D;JGT

(WHITE)
    @pos
    D=M

    @LOOP
    D;JLT

    @SCREEN
    A=A+D
    M=0

    @pos
    M=M-1

    @LOOP
    0;JMP

(BLACK)
    @pos
    D=M
    @8191
    D=D-A

    @LOOP
    D;JGE

    @pos
    D=M

    @SCREEN
    A=A+D
    M=-1

    @pos
    M=M+1
    
    @LOOP
    0;JMP

(END)
    @END
    0;JMP