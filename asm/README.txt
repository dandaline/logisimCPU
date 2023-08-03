ASSAMBLER LANGUAGE

- MNEMONICS: First word in each line declares operation
- r1, r2, r3: REPLACE as PC, RA, RB, RC, RD, RE, RF, RG
- constant: REPLACE IN HEX and MAX 8bit!, i.e. 40 for 0d64
- Operations are seperated by new line (\n)
- end program with new empty line (\n)!!!! like this:
- comments are separated by white space after a code line i.e.:
    ldi 7f RA my comment
- No real error handling, so pay attention, to what you write!
- implemented as agreed on 22th of June 2022 (instructionlist) for all mandatory operations
    CUSTOM OPERATIONS MAY DIFFER! (in my case I think only the relative jumps)
- create file named what you like for your program (simple text file, no extensions needed)
- run asm and enter full path to your program
- machine code will appear in "rom.txt"
- ommit [], these just tell you, it's the address for RAM / ROM
- ALL relative jump-constants will be added to CURRENT PC
- jump constants are given in hexadecimal
    if you want to jump to jump like this:

    ld RA RF <- you want to land back here
    inc RF RF
    tst RB RF
    jzr -3 <- using this jump

- if you want to use strings, modify the string in asm.cpp at line 103
    the string will be written as ASCII chars to ROM addr 0xc0 through out 0xff

SYNTAX OF ALL BASIC OPERATIONS ([] means address to RAM/ROM, register should contain the correct address)

nop //do nothing

st [r2] r1 //write to ram
ld r3 [r2] //load from ram
ldRomL r3 [r1] //load from ROM lower two bytes
ldRomH r3 [r1] //load from ROM upper two bytes

asciiIOL [r1] r2 //load char from rom lower two bytes
asciiIOH [r1] r2

ldi constant r3 //load constant to register
mov r1 r3 // copy register content to another, from R3 to R1

jz constant //jump zero
jnz constant //jump not zero
jc constant //jump carry
jnc constant //jump no carry
jnr constant //jump negative ralative to PC
jnnr constant //jump not negative relative to PC
jzr constant //jump zero relative to PC
jnzr constant //jump not zero relative to PC

//r1 is always the destination, r2 and r3 are source

add r1 r2 r3 //r1=r2+r3
sub r1 r2 r3 //r1=r2-r3
inc r2 r3 //not shure anymore, what goes where
dec r2 r3 //not shure anymore waht goes where
mult r1 r2 r3 //MULTIPLY
tst r1 r2 //r1-r2=ALU FLAG, I think, register r1 will be written to, but not shure anymore
& r1 r2 r3 //AND
| r1 r2 r3 //OR
! r2 r3 //NOT
~ r2 r3 //NEGATE
^ r1 r2 r3 //XOR
<< r1 r2 r3 //logical shift left
>> r1 r2 r3 //logical shift right

EXAMPLE:
ldi f RA //load 15 into register A
ldi 31 RB //laod 46 into register B
add RA RB RC // add A and B and store result in C i.e. C=49
ldi 2 RA //load 2 into A for upcomming store operation
st RA RC //stores 49 from register C into RAM at adress 0x2
ldi 2 RA //store 2 in register A
ldi 2 RB //store 2 in register B
tst RA RB //perform test RA - RB = 0 --> ZERO FLAG true
jz 20 //sets PC to decimal 32 and performs jump since zero flag is true
