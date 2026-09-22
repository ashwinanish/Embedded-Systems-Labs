// StringConversion.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String
     .data
     .align 2
// no globals allowed for Lab 6
    .global OutChar    // virtual output device
    .global OutDec     // your Lab 6 function
    .global Test_udivby10

    .text
    .align 2
// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

// ****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
   PUSH {R4-R7, LR}

   MOVS R4, #0
   MOVS R5, R0

loop:
   CMP R5, #10
   BLO done
   SUBS R5, R5, #10
   ADDS R4, R4, #1
   B loop

done:
   MOVS R0, R4
   MOVS R1, R5

   POP  {R4-R7, PC}

  
//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// OutChar does not do actual output, OutChar does virtual output used by the grader
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
.equ ones, 0
.equ tens, 4
.equ hundreds, 8
.equ thousands, 12
.equ tenThousands, 16
OutDec:
   PUSH {R4-R7, LR}
   
   SUB SP, SP, #20
   MOVS R4, #0
   CMP R0, #0
   BNE digitLoop
   MOVS R5, #0
   STR R5, [SP, #ones]
   ADDS R4, R4, #1
   B printSetup

digitLoop:
   BL udivby10
   
   CMP R4, #0
   BEQ storeOnes
   CMP R4, #1
   BEQ storeTens
   CMP R4, #2
   BEQ storeHundreds
   CMP R4, #3
   BEQ storeThousands
   B storeTenThousands

storeOnes:
   STR R1, [SP, #ones]
   B stored 

storeTens:
   STR R1, [SP, #tens]
   B stored 

storeHundreds:
   STR R1, [SP, #hundreds]
   B stored

storeThousands:
   STR R1, [SP, #thousands]
   B stored

storeTenThousands:
   STR R1, [SP, #tenThousands]
   B stored

stored:
   ADDS R4, R4, #1
   CMP R0, #0
   BNE digitLoop

printSetup:
   SUBS R4, R4, #1

print:
   CMP R4, #0
   BEQ printOnes
   CMP R4, #1
   BEQ printTens
   CMP R4, #2
   BEQ printHundreds
   CMP R4, #3
   BEQ printThousands
   B printTenThousands

printOnes:
   LDR R0, [SP, #ones]
   B printChar

printTens:
   LDR R0, [SP, #tens]
   B printChar

printHundreds:
   LDR  R0, [SP, #hundreds]
   B    printChar

printThousands:
   LDR  R0, [SP, #thousands]
   B    printChar

printTenThousands:
   LDR  R0, [SP, #tenThousands]
   B printChar

printChar:
   ADDS R0, R0, #48
   BL OutChar

   CMP R4, #0
   BEQ done1
   SUBS R4, R4, #1
   B print

done1:
   ADD SP, SP, #20
   POP  {R4-R7, PC}
// * * * * * * * * End of OutDec * * * * * * * *

// ECE319H recursive version
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11

OutDec2:
   PUSH {LR}

   POP  {PC}



     .end
