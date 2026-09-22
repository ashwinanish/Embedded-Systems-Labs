// ****************** ECE319K_Lab2.s ***************
// Your solution to Lab 2 in assembly code
// Author: Ashwin Anish
// Last Modified: Your date
// ECE319K Spring 2026 (ECE319H students do Lab2H)
// I/O port addresses
    .include "../inc/msp.s"

        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive


        .text
        .thumb
        .align 2
        .global EID
EID:    .string "AA224496" // replace ZZZ123 with your EID here
        .align 2

// this allow your Lab2 programs to the Lab2 grader
        .global Lab2Grader
// this allow the Lab2 grader to call your Lab2 program
        .global Lab2
// these two allow your Lab2 programs to all your Lab3 solutions
        .global Debug_Init
        .global Dump

// Switch input: PB2 PB1 or PB0, depending on EID
// LED output:   PB18 PB17 or PB16, depending on EID
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20
Lab2:
// Initially the main program will
//   set bus clock at 80 MHz,
//   reset and power enable both Port A and Port B
// Lab2Grader will
//   configure interrupts  on TIMERG0 for grader or TIMERG7 for TExaS
//   initialize ADC0 PB20 for scope,
//   initialize UART0 for grader or TExaS
     MOVS R0,#10
// 0 for info,
// 1 debug with logic analyzer,
// 2 debug with scope,
// 3 debug without scope or logic analyzer
// 10 for grade
     BL   Lab2Grader
     BL   Debug_Init // your Lab3 (ignore this line while doing Lab 2)
     BL   Lab2Init
     LDR R1, =GPIOB_DOUT31_0
     LDR R2, [R1] 
     LDR R4, =GPIOB_DIN31_0
     LDR R5, [R4]
     LDR R3, =0x08
     ANDS R3, R3, R5
     CMP R3, 0x00
     BEQ ZerDC
     BL FirDC
Turnon:
     LDR R3, =0x40000
     ORRS R2, R2, R3
     STR R2, [R1]
     BX LR
Turnoff:
     LDR R3, =0x40000
     BICS R2, R2, R3
     STR R2, [R1]
     BX LR
Delay: SUBS R0, R0, #2
dloop: SUBS R0, R0, #4
        NOP
        BHS dloop
        BX LR
ZerDC:
     wroop:
        BL compare
        CMP R3, 0x08
        BEQ FirDC
        B wroop

FirDC:
     LDR R6, =400000
     LDR R7, =1200000
     roop:
        BL compare
        CMP R3, 0x08
        BEQ SecDC
        BL switch
        B roop
SecDC:
     LDR R6, =720000
     LDR R7, =880000
     moop:
        BL compare
        CMP R3, 0x08
        BEQ ThirDC 
        BL switch
        B moop
ThirDC:
     LDR R6, =960000
     LDR R7, =640000
     koop:
        BL compare
        CMP R3, 0x08
        BEQ FourDC
        BL switch
        B koop
FourDC:
     LDR R6, =1360000
     LDR R7, =240000
     doop:
        BL compare
        CMP R3, 0x08
        BEQ FirDC
        BL switch
        B doop
switch: 
     PUSH {LR}   
     MOVS R0, R6
     BL Turnon
     PUSH {R0-R3}
     BL Dump
     POP {R0-R3}
     BL Delay
     MOVS R0, R7
     BL Turnoff
     PUSH {R0-R3}
     BL Dump
     POP {R0-R3}
     BL Delay
     POP {PC}
compare:
     LDR R5, [R4]
     LDR R3, =0x08
     ANDS R3, R3, R5
     BX LR
// make switch an input, LED an output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
// ***do not reset/power Port A or Port B, already done****
        LDR R2, =0x00040081
        LDR R1, =IOMUXPB3
        STR R2, [R1]
        MOVS R2, #0x81
        LDR R1, =IOMUXPB18
        STR R2, [R1]
        LDR R1, =GPIOB_DOE31_0
        LDR R2, [R1]
        LDR R3, =0x40000
        ORRS R2, R2, R3
        STR R2, [R1]
        
   BX   LR

   .end