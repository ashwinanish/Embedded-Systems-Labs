/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Put your names here or look silly
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put your EID in the next line
const char EID1[] = "AA224496"; //  ;replace abc123 with your EID
// Hint implement Traffic_Out before creating the struct, make struct match your Traffic_Out
struct STATES {
  uint32_t out;
  uint32_t time;
  uint32_t Next[8];};

typedef const struct STATES trafficLight_t;
#define SouthOn 0
#define SouthYellow 1
#define SouthRed 2
#define WestOn 3
#define WestYellow 4
#define WestRed 5
#define WalkOn 6
#define WalkRedOne 7
#define WalkRedOffOne 8
#define WalkRedTwo 9
#define WalkRedOffTwo 10
#define WalkRedThree 11

trafficLight_t FSM[12] = {
  {0x04000121, 5000, {SouthOn, SouthYellow, SouthOn, SouthYellow, SouthYellow, SouthYellow, SouthYellow, SouthYellow}},
  {0x04000122, 1000, {SouthRed ,SouthRed ,SouthRed ,SouthRed ,SouthRed ,SouthRed ,SouthRed ,SouthRed}},
  {0x04000124, 1000, {SouthOn, WestOn, SouthOn, WestOn, WalkOn, WalkOn, WalkOn, WalkOn}},
  {0x04000064, 5000, {WestYellow, WestOn, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow}},
  {0x040000A4, 1000, {WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed}},
  {0x04000124, 1000, {SouthOn, WestOn, SouthOn, SouthOn, WalkOn, WalkOn, SouthOn, SouthOn}},
  {0x0C400124, 5000, {WalkRedOne, WalkRedOne, WalkRedOne, WalkRedOne, WalkOn, WalkRedOne, WalkRedOne, WalkRedOne}},
  {0x04000124, 500 , {WalkRedOffOne, WalkRedOffOne, WalkRedOffOne, WalkRedOffOne, WalkRedOffOne, WalkRedOffOne, WalkRedOffOne, WalkRedOffOne}},
  {0x00000124, 500 , {WalkRedTwo, WalkRedTwo, WalkRedTwo, WalkRedTwo, WalkRedTwo, WalkRedTwo, WalkRedTwo, WalkRedTwo}},
  {0x04000124, 500 , {WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo, WalkRedOffTwo}},
  {0x00000124, 500 , {WalkRedThree, WalkRedThree, WalkRedThree, WalkRedThree, WalkRedThree, WalkRedThree, WalkRedThree, WalkRedThree}},
  {0x04000124, 500 , {SouthOn, WestOn, SouthOn, WestOn, WalkOn, WestOn, SouthOn, WestOn}}};




// initialize all 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
 // South Initialization
 IOMUX->SECCFG.PINCM[PB2INDEX] = (1<<7) | (0x01); //Red
 IOMUX->SECCFG.PINCM[PB1INDEX] = (1<<7) | (0x01); //Yellow
 IOMUX->SECCFG.PINCM[PB0INDEX] = (1<<7) | (0x01); //West
 
 //West Initialization
 IOMUX->SECCFG.PINCM[PB8INDEX] = (1<<7) | (0x01); //Red
 IOMUX->SECCFG.PINCM[PB7INDEX] = (1<<7) | (0x01); //Red
 IOMUX->SECCFG.PINCM[PB6INDEX] = (1<<7) | (0x01); //Red
 GPIOB->DOE31_0 = (1<<8) | (1<<7) | (1<<6) | (1<<2) | (1<<1) | (1<<0) | (1<<5) 
 | (1 << 22) | (1 << 26) | (1 << 27);

 //Input Initialization
 IOMUX->SECCFG.PINCM[PB17INDEX] = (1<<18) | (1<<7) | (0x01); //Walk
 IOMUX->SECCFG.PINCM[PB16INDEX] = (1<<18) | (1<<7) | (0x01); //Yellow
 IOMUX->SECCFG.PINCM[PB15INDEX] = (1<<18) | (1<<7) | (0x01); //Green

}
/* Activate LEDs
* Inputs: west is 3-bit value to three east/west LEDs
*         south is 3-bit value to three north/south LEDs
*         walk is 3-bit value to 3-color positive logic LED on PB22,PB26,PB27
* Output: none
* - west =1 sets west green
* - west =2 sets west yellow
* - west =4 sets west red
* - south =1 sets south green
* - south =2 sets south yellow
* - south =4 sets south red
* - walk=0 to turn off LED
* - walk bit 22 sets blue color
* - walk bit 26 sets red color
* - walk bit 27 sets green color
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t west, uint32_t south, uint32_t walk){
  // South: PB0, PB1, PB2
  // West : PB6, PB7, PB8
  // Walk : PB22, PB26, PB27
  GPIOB->DOUT31_0 &= 0;

  if(south & 0x01) GPIOB->DOUT31_0 |= (1<<0); // south green
  if(south & 0x02) GPIOB->DOUT31_0 |= (1<<1); // south yellow
  if(south & 0x04) GPIOB->DOUT31_0 |= (1<<2); // south red

  if(west & 0x01) GPIOB->DOUT31_0 |= (1<<6); // west green
  if(west & 0x02) GPIOB->DOUT31_0 |= (1<<7); // west yellow
  if(west & 0x04) GPIOB->DOUT31_0 |= (1<<8); // west red

  GPIOB->DOUT31_0 |= (walk & ((1<<22)|(1<<26)|(1<<27)));
}
/* Read sensors
 * Input: none
 * Output: sensor values
 * - bit 0 is west car sensor
 * - bit 1 is south car sensor
 * - bit 2 is walk people sensor
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){
  // PB15 = west car sensor   -> bit 0
  // PB16 = south car sensor  -> bit 1
  // PB17 = walk sensor       -> bit 2
  uint32_t in = GPIOB->DIN31_0;
  uint32_t out = 0;

  if(in & (1<<15)) out |= 0x01; // west
  if(in & (1<<16)) out |= 0x02; // south
  if(in & (1<<17)) out |= 0x04; // walk

  return out;
}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  SysTick_Init();
  Traffic_Init();
  Lab4Grader(1); // print assignment, no grading
  uint32_t cs = SouthOn;
  uint32_t input;
  uint32_t inputMask;
  while(1){
    GPIOB->DOUT31_0 = FSM[cs].out;
    SysTick_Wait10ms(FSM[cs].time/10);
    inputMask = 0x00038000;
    input = (GPIOB->DIN31_0 & inputMask)>>15;
    cs = FSM[cs].Next[input];
  }
}
// use main2 to debug LED outputs
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_Out  function
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  UART_Init();
  UART_OutString("Lab 4, Spring 2026, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  while(1){
      Traffic_Out(0, 1, 0);          // South Green
      Debug_Dump(0x01);        

      Traffic_Out(0, 2, 0);          // South Yellow
      Debug_Dump(0x02);

      Traffic_Out(0, 4, 0);          // South Red
      Debug_Dump(0x04);

      Traffic_Out(1, 0, 0);          // West Green
      Debug_Dump(0x08);

      Traffic_Out(2, 0, 0);          // West Yellow
      Debug_Dump(0x10);

      Traffic_Out(4, 0, 0);          // West Red
      Debug_Dump(0x20);

      Traffic_Out(0, 0, (1<<26));    // Walk Red
      Debug_Dump(0x4000000);

      Traffic_Out(0, 0, (1<<26|1<<27|1<<22));    // Walk White
      Debug_Dump(0xC400000);

    }
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  
}
// use main3 to debug the three input switches
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_In  function
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2026, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){ // main4
  uint32_t input;
  uint32_t cs = SouthOn;   // set initial state (pick whatever your lab expects)
  uint32_t west, south, walk;
  uint32_t packed;

  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization

  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2026, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");

  SysTick_Init(); // Initialize SysTick for software waits

  while(1){
    west = 0;
    if(FSM[cs].out & (1<<6)) west |= 0x01;   // west green
    if(FSM[cs].out & (1<<7)) west |= 0x02;   // west yellow
    if(FSM[cs].out & (1<<8)) west |= 0x04;   // west red

    south = 0;
    if(FSM[cs].out & (1<<0)) south |= 0x01;  // south green
    if(FSM[cs].out & (1<<1)) south |= 0x02;  // south yellow
    if(FSM[cs].out & (1<<2)) south |= 0x04;  // south red

    walk = 0;
    if(FSM[cs].out & (1<<22)) walk |= 0x01;  // blue
    if(FSM[cs].out & (1<<26)) walk |= 0x02;  // red
    if(FSM[cs].out & (1<<27)) walk |= 0x04;  // green
    Traffic_Out(west, south,
                ((walk&0x01)?(1<<22):0) |
                ((walk&0x02)?(1<<26):0) |
                ((walk&0x04)?(1<<27):0));
                
    packed = ((cs   & 0xFF) << 24) |
             ((west & 0xFF) << 16) |
             ((south& 0xFF) <<  8) |
             ((walk & 0xFF) <<  0);
    Debug_Dump(packed);
    SysTick_Wait10ms(FSM[cs].time/10);
    input = 7;
    cs = FSM[cs].Next[input];
  }
}
// use main5 to grade
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
// initialize your FSM
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize your FSM
  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){
      // 1) output depending on state using Traffic_Out
      // call your Debug_Dump logging your state number and output
      // 2) wait depending on state
      // 3) input from switches
      // 4) next depends on state and input
  }
}

