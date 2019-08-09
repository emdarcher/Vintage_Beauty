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

//The port for column control (connects to transistors to drive the columns)
#define COL_PORT            P2
//sets the column port to all zeros to blank the columns
#define COLS_OFF            0x00;

//the pin for SPI serial out
#define DATA_OUT_PIN        P1_6
//the pin for SPI Clock output
#define DATA_CLK_PIN        P1_7

//pin used in the demo to switch between the English and Japanese demo messages
#define MESSAGE_SWITCH_PIN  P0_3

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

//use first bit addressable byte address as a buff
volatile __data __at (0x20) uint8_t out_bits_buff;
//bit variables to access the bits of the byte buff at data 0x20
__bit __at (0x00) out_bit0;
__bit __at (0x01) out_bit1;
__bit __at (0x02) out_bit2;
__bit __at (0x03) out_bit3;
__bit __at (0x04) out_bit4;
__bit __at (0x05) out_bit5;
__bit __at (0x06) out_bit6;
__bit __at (0x07) out_bit7;

//the standard test string to output
const uint8_t test_str[] = "Vintage Beauty.";

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
inline void shift_out_7bits(uint8_t in_byte);
void shift_out_buff(void);
inline void shift_out_column(uint8_t col_num);
uint8_t get_char_col(uint8_t ch, uint8_t col);
void load_col_buff(uint8_t col);
void load_out_str_buff(uint8_t * in_str);

void init_timer0_interrupt(void);
inline void disable_timer0_interrupt(void);



void load_out_str_buff(uint8_t * in_str){
    //loads the out string buffer with an input string

    //copy in the blank space string
    memcpy(out_str_buff, disp_blank_str, CHAR_COUNT + 1);
    //tmp variable to store the input string length
    uint8_t in_str_len = strlen(in_str);
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
        out_buff[i] = get_char_col(out_str_buff[i], col);
   
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

inline void shift_out_column(uint8_t col_num){
    //shifts out to a column on the display 

    //clear all columns before writing to the display
    COL_PORT = COLS_OFF;
    //shift out the contents of the buffer with row data
    shift_out_buff();
    //turn on the column we want
    COL_PORT = col_bits[col_num];
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

inline void shift_out_7bits(uint8_t in_byte){
    //shifts out 7 bits from the in_byte, LSB first in an SPI-like fashion
    //accesses each bit at a time utilizing the bit addressable variables
    //to optimize for speed

    out_bits_buff = in_byte;
    //set clock high to start
    DATA_CLK_PIN = 1;
    //put out the first bit of data
    DATA_OUT_PIN = out_bit0;
    //set clock down to shift in the bit.
    DATA_CLK_PIN = 0;
    //a NOP to give tiny downtime between clock pulses
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

inline void disable_timer0_interrupt(void){
    //disable the timer 0 interrupt enable bit
    ET0 = 0;
}


void main(void){
    //make the message switch pin an input
    MESSAGE_SWITCH_PIN = 1;

    //load test string into the buffer
    if(MESSAGE_SWITCH_PIN == 1){
        //if the pin is pulled HIGH then output the test_str
        load_out_str_buff(test_str);
    } else {
        //if the pin is pulled low, then output the jp_test_str
        load_out_str_buff(jp_test_str);
    }

    //variable to increment the current column
    uint8_t col_inc = 0;

    //init the timer
    init_timer0_interrupt();

    //loop
    while(1){
        //wait for the flag
        while(disp_col_update_flag == 0);
        //once flagged
        //load column data into the buffer
        load_col_buff(col_inc);
        //shift out the column data
        shift_out_column(col_inc);
        //increment the column
        col_inc++;
        //if increment has reached max, reset to column zero
        if(col_inc == CHAR_COLS){
            col_inc = 0;
        }
        //reset the flag, so we can wait for the next interrupt
        disp_col_update_flag = 0;
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

    //set the bits again     
    TR0 = 1;
}
