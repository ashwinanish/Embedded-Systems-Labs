// ****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Ashwin Anish
// Last Modified: 1/26/2026
// Spring 2026
        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "AA224496" // replace ZZZ123 with your EID here

        .global Phase
        .align 2
Phase:  .long 10
// Phase= 0 will display your objective and some of the test cases, 
// Phase= 1 to 5 will run one test case (the ones you have been given)
// Phase= 6 to 7 will run one test case (the inputs you have not been given)
// Phase=10 will run the grader (all cases 1 to 7)
        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
      LDR R4, =EID
      MOVS R5, #0
      MOVS R7, #0
looparray:
      LDR R6, [R0]
      cmp R6, #0
      BEQ done
      BL strcomp
      cmp R7, #1
      BEQ returnIndex
      ADDS R5, #1
      ADDS R0, #8
      B looparray
returnIndex:
      ADDS R0, R5, #0
      POP  {R4-R7,PC} // return
done:
      SUBS R0, R7, #1
      POP  {R4-R7,PC} // return


        .align 2
        .global myClass
myClass: .long pAB123  // pointer to EID
         .long 95      // Score
         .long pXYZ1   // pointer to EID
         .long 96      // Score
         .long pAB5549 // pointer to EID
         .long 94      // Score
         .long 0       // null pointer means end of list
         .long 0
pAB123:  .string "AB123"
pXYZ1:   .string "XYZ1"
pAB5549: .string "AB5549"

        .align 2
        .global strcomp

// r0 has string loc, r4 has eid loc
strcomp:
        PUSH {R0,R4-R6,LR}
        LDR R0, [R0]
        MOVS R7, #0
loopstr:
        LDRB R5, [R4, R7]
        LDRB R6, [R0, R7]
        CMP R5, R6
        BNE notEqual
        CMP R5, #0
        BEQ equal
        ADDS R7, #1
        B loopstr
equal:
        MOVS R7, #1
        POP  {R0,R4-R6,PC}
notEqual:
        MOVS R7, #0
        POP  {R0,R4-R6,PC}
        .end
