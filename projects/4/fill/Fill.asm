// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.

(RESTART)
    // set position
    @SCREEN
    D=A
    @addr
    M=D
    // is pressed?
    @KBD
    D=M
    @WHITE
    D;JEQ

(BLACK)
    @color
    M=-1
    @PAINTIT
    0;JMP

(WHITE)
    @color
    M=0
    @PAINTIT
    0;JMP

(PAINTIT)
    @addr
    D=M
    @KBD
    D=A-D
    @RESTART
    D;JEQ
    @color
    D=M
    @addr
    A=M
    M=D
    @addr
    M=M+1
    @PAINTIT
    0;JMP

(END)
    @END
    0;JMP