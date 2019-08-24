//Display a message on HP HDSP-2000 Alphanumeric LED displays

#include <8051.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ascii5x7.h"

#define CPU_FREQ        11059200UL //Hz
#define PRESCALER       12
#define PERIOD          1 

//how many ticks we want per second (for 500Hz update rate)
#define TICKS_PER_SEC   500
//calculates the timer clock setup
#define TIMER_CLOCKS    ((((CPU_FREQ)/(PRESCALER))*(PERIOD))/(TICKS_PER_SEC))
#define TIMER_RES       65536 //16 bits

//how many bits in one column
#define COL_BITS            7

#define CHAR_PER_DISPLAY    4
#define DISP_COUNT          4
#define CHAR_COUNT          ((CHAR_PER_DISPLAY)*(DISP_COUNT))

//4 spaces, for blanking a single display unit
#define SPACE_4             "    "

//blank spaces for display counts
#if DISP_COUNT == 1
#define DISP_BLANK_SPACES   SPACE_4
#elif DISP_COUNT == 2
#define DISP_BLANK_SPACES   SPACE_4 SPACE_4
#elif DISP_COUNT == 3
#define DISP_BLANK_SPACES   SPACE_4 SPACE_4 SPACE_4
#elif DISP_COUNT == 4
#define DISP_BLANK_SPACES   SPACE_4 SPACE_4 SPACE_4 SPACE_4
#endif

//LCD display blink rate is 320ms 
#define BLINK_RATE_MS   320
//caclulates how many timer ticks to generate the proper blink rate in MS
#define TICKS_PER_BLINK     ((BLINK_RATE_MS)/(1000/(TICKS_PER_SEC)))

//The port for column control (connects to transistors to drive the columns)
#define COL_PORT            P2
//sets the column pin to all zeros to blank the columns
#define COLS_OFF_MASK       0b11100000;

//the pin for SPI serial out
#define DATA_OUT_PIN        P1_6
//the pin for SPI Clock output
#define DATA_CLK_PIN        P1_7

//LCD control commands, the base bit mask for each command.
#define CMD_CLEAR_DISPLAY   0x01
#define CMD_RETURN_HOME     0x02
#define CMD_ENTRY_MODE_SET  0x04
#define CMD_DISP_ON_OFF     0x08
#define CMD_DISP_SHIFT      0x10
#define CMD_FUNC_SET        0x20
#define CMD_SET_CGRAM_ADDR  0x40
#define CMD_SET_DDRAM_ADDR  0x80

//masks for read/write etc. bottom two bits are values of RS and RW
#define READ_BUSY_FLAG      0x01
#define WRITE_DATA          0x02
#define READ_DATA           0x03

//bit numbers for flags in various commands
#define ENTRY_MODE_I_D_BIT      1
#define ENTRY_MODE_S_BIT        0
#define DISPLAY_ON_BIT          2
#define DISPLAY_CURSOR_BIT      1
#define DISPLAY_BLINK_BIT       0
#define DISPLAY_SHIFT_BIT       3
#define DISPLAY_SHITT_DIR_BIT   2
#define FUNC_SET_DATA_LEN_BIT   4
#define FUNC_SET_N_LINES_BIT    3
#define FUNC_SET_FONT_BIT       2


//the selection and control pins
#define RS_PIN      P1_1
#define RW_PIN      P1_0
//the enable pin, attached to external interrupt 1 (INT0) on P3.2
//make sure to invert the E signal before input using an inverter
//as the interrupt is triggered on LOW, but LCD spec is triggered on HIGH
#define E_PIN       P3_2
//the port for data bus input/output
#define DB_PORT     P0
//the busy state output pin (this is done on DB7)
#define BUSY_PIN    P0_7


//bits for outputting the columns on the I/O port
const uint8_t col_bits[CHAR_COLS] = {
    (1<<0),
    (1<<1),
    (1<<2),
    (1<<3),
    (1<<4),
};


//output buffer to hold column data to shift out
volatile uint8_t out_buff[CHAR_COUNT];

//variable to store the IR (instruction register)
volatile uint8_t ins_reg;

//variable to store the DR (data register)
volatile uint8_t data_reg;

//variable to store the AC (address counter)
volatile uint8_t addr_counter;

//the internal busy flag, used for outputting the busy state when requested.
volatile __bit busy_flag = 0;

//sets the blinking  state
volatile __bit blink_state = 0;

//triggers the blinking tick function
volatile __bit blink_flag = 0;

//sets if blinking block is currently on
__bit block_on = 0;

//variable to count ticks
volatile uint8_t tick_cnt = 0;

//set when enabled
volatile __bit enable_flag = 0;
//sets the enabled state
__bit enable_state = 0;


/*
 * Memory Map example for one byte of the 8051's bit memory
 *
 * Byte Addr    |MSB           Bit Addresses         LSB|
 * 0x20         |0x07|0x06|0x05|0x04|0x03|0x02|0x01|0x00|
 *
 */

//use the first bit addressable byte address in the 8051 as a buffer
volatile __data __at (0x20) uint8_t out_bits_buff;
//bit variables to access the individual bits of the byte buff at data 0x20
__bit __at (0x00) out_bit0; //address for bit 0 of the byte buff at data 0x20
__bit __at (0x01) out_bit1; //bit 1
__bit __at (0x02) out_bit2; //bit 2
__bit __at (0x03) out_bit3; //bit 3
__bit __at (0x04) out_bit4; //bit 4
__bit __at (0x05) out_bit5; //bit 5
__bit __at (0x06) out_bit6; //bit 6
__bit __at (0x07) out_bit7; //bit 7

//REMOVE FOR SPACE LATER
//the standard test string to output
const uint8_t test_str[] = "Vintage Beauty.";

//REMOVE FOR SPACE LATER
//the japanese test string to output (using ROM_CODE_A01)
//message: ビンテージのうつくしさ。
const uint8_t jp_test_str[] = {
CHAR_JP_BI, CHAR_JP_N, CHAR_JP_TE, CHAR_JP_LONG, CHAR_JP_JI, 
HIRAGANA_NO,
HIRAGANA_U, HIRAGANA_TSU, HIRAGANA_KU, HIRAGANA_SHI, HIRAGANA_SA,
CHAR_JP_FULL_STOP,
};

//buffer for the output string
uint8_t out_str_buff[CHAR_COUNT + 1];

//blank display string
const uint8_t disp_blank_str[CHAR_COUNT + 1] = DISP_BLANK_SPACES;

//flag to be set in the timer interrupt to trigger a column update
volatile __bit disp_col_update_flag;

//function prototypes
void shift_out_7bits(uint8_t in_byte);
void shift_out_buff(void);
void shift_out_column(uint8_t col_num);
uint8_t get_char_col(uint8_t ch, uint8_t col);
void load_col_buff(uint8_t col);
void load_out_str_buff(uint8_t * in_str, bool is_str);
//inerrupt setup functions
void init_timer0_interrupt(void);
void disable_timer0_interrupt(void);
void init_int0_interrupt(void);

//tick functions
void blinking_tick(void);
void column_update_tick(void);

void init_int0_interrupt(void){
    //set the pin value to 1 
    P3_2 = 1;
    //enable all interrupts
    EA = 1;
    //enable external interrupt 0
    EX0 = 1;
    //setup for edge-triggering
    IT0 = 1;
}


void column_update_tick(void){
    //variable to increment the current column
    static uint8_t col_inc;
    
    if(disp_col_update_flag != 0){
        //wait for the flag
        //once flagged
        //load column data into the buffer
        load_col_buff(col_inc);
        //shift out the column data
        shift_out_column(col_inc);
        //increment the column
        col_inc++;
        //if increment has reached max, reset to column zero
        if(col_inc >= CHAR_COLS){
            col_inc = 0;
        }
        //reset the flag, so we can wait for the next interrupt
        disp_col_update_flag = 0;
    }
}


void blinking_tick(void){
    if(blink_flag == 1){
        //toggle the blink
        block_on == !block_on;
        //reset the flag
        blink_flag = 0;
    }
}

void load_out_str_buff(uint8_t * in_str, bool is_str){
    //loads the out string buffer with an input string
    //the is_str parameter is to tell if the input is a 
    //NULL terminated C string, or if it's just a raw character array

    //copy in the blank space string
    memcpy(out_str_buff, disp_blank_str, CHAR_COUNT + 1);
    //tmp variable to store the input string length
    uint8_t in_str_len = (is_str) ? strlen(in_str) : sizeof(in_str);
    //copy in the in_str to the buffer, making sure not to overflow
    if(in_str_len <= CHAR_COUNT){
        memcpy(out_str_buff, in_str, in_str_len);
    } else {
        memcpy(out_str_buff, in_str, CHAR_COUNT);
    }
}

void load_col_buff(uint8_t col){
    //loads the out_buff with the column data for characters
    //in the out_str_buff

    uint8_t i = CHAR_COUNT - 1;
    while(1){
        //load the row data for the char
        if((blink_state == 1) && (i == addr_counter) && (block_on == 1)){
            //if blinking, blink the block when active
            out_buff[i] =  0xFF; //all 1's to blink display
        } else {
            //load the character columns from the ROM
            out_buff[i] = get_char_col(out_str_buff[i], col);
        }
   
        //break when we reach zero 
        if(i == 0){
            break;
        }
        //decrement the index
        i--;
    }
}

uint8_t get_char_col(uint8_t ch, uint8_t col){
    //gets the column data for a char
    return ascii_matrices[ch][col];
}

void shift_out_column(uint8_t col_num){
    //shifts out to a column on the display 

    //clear all columns before writing to the display
    COL_PORT &= COLS_OFF_MASK;
    //shift out the contents of the buffer with row data
    shift_out_buff();
    //turn on the column we want
    COL_PORT |= col_bits[col_num];
}

void shift_out_buff(void){
    //shifts out the contents of of the out_buff
    
    uint8_t i = CHAR_COUNT - 1;
    while(1){
        //shift out a set of bits from the buffer
        shift_out_7bits(out_buff[i]);
        //when we reach index zero, leave the loop.
        if(i == 0){
            break;
        }
        //decrement the index
        i--;
    }
}

void shift_out_7bits(uint8_t in_byte){
    //shifts out 7 bits from the in_byte, LSB first in a SPI-like fashion
    //accesses each bit at a time utilizing the bit addressable variables
    //to optimize for speed

    //copy our in_byte into our bit-addressable byte buffer
    out_bits_buff = in_byte;
    //set clock high to start
    DATA_CLK_PIN = 1;
    //put out the first bit of data
    DATA_OUT_PIN = out_bit0;
    //set clock down to shift in the bit.
    DATA_CLK_PIN = 0;
    //a NOP to give a tiny downtime between clock pulses
    __asm
        nop  
    __endasm;
    //repeat for rest of the bits
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit1; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit2; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit3; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit4; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit5; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    DATA_CLK_PIN = 1; DATA_OUT_PIN = out_bit6; DATA_CLK_PIN = 0;
    __asm    nop    __endasm;
    //end by returning the clock to high
    DATA_CLK_PIN = 1;
}


//function to initialize our timer 0 interrupt
void init_timer0_interrupt(void){
    //set the timer bits and enable the interrupt
    
    //high bits
    TH0 = (TIMER_RES - TIMER_CLOCKS) >> 8;
    //low bits
    TL0 = (TIMER_RES - TIMER_CLOCKS) & 0x00ff;
    TMOD = 0x01; //set mode
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void disable_timer0_interrupt(void){
    //disable the timer 0 interrupt enable bit
    ET0 = 0;
}


void main(void){
    //init the timer
    init_timer0_interrupt();
    //init the external interrupt
    init_int0_interrupt();

    //loop
    while(1){
        column_update_tick();
        blinking_tick();
    }
}

//the Timer 0 interrupt service routine
void Timer0_ISR(void) __interrupt 1 {
    //set the clock bits again 
    //high bits
    TH0 = (TIMER_RES - TIMER_CLOCKS) >> 8;
    //low bits
    TL0 = (TIMER_RES - TIMER_CLOCKS) & 0x00ff;

    //set our flag
    disp_col_update_flag = 1;

    //if we are blinking
    if(blink_state == 1){
        //increment count
        tick_cnt++;
        //flag blink when count is reached
        if(tick_cnt >= TICKS_PER_BLINK){
            //trigger flag
            blink_flag = 1;
            //reset count
            tick_cnt = 0;
        }
    }

    //set the bits again     
    TR0 = 1;
}

//the External Interrupt 0 (INT0) service routine
void INT0_ISR(void) __interrupt 0 {
    
    //set the busy flag
    busy_flag = 1;

}
