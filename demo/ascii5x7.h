//Header file containing the Character ROM codes and settings

#ifndef _ASCII_5x7_ 
#define _ASCII_5x7_

#include <stdint.h>

#define CHAR_COLS   5
#define CHAR_ROWS   7

#define CHAR_ROM_SIZE   256

//THE ROM CODES (emulating HD66702 LCD Driver)
#define ROM_CODE_A00    0x0A00
#define ROM_CODE_A01    0x0A01
#define ROM_CODE_A02    0x0A02

//******* SET YOUR DESIRED ROM CODE HERE *******
#define ROM_CODE    ROM_CODE_A01

//set this to 1 if you want to use the Japanese Hiragana character set
//in the normally unused LCD emulated ROM space. Otherwise set to 0.
#define USE_HIRAGANA_ROM    1

//select here if you want to use ascii '\' character instead of YEN (default)
#define USE_ASCII_BACKSLASH     0

//select here if you want to to use the ascii tilde '~' instead of right arrow
//which is the default in ROM codes A00 and A01
#define USE_ASCII_TILDE 0

//the offset before standard ASCII characters
#define ASCII_CHAR_OFFSET   32

//address for the cursor character
#define CHAR_CURSOR     0xFF

//if we have a ROM with japanese Katakana, 
//include the header file with the codes
#if (ROM_CODE == ROM_CODE_A00) || (ROM_CODE == ROM_CODE_A01) 
#include "lcd_char_jp.h"
#endif

//defines for the extra Japanese characters in ROM code A01
#if ROM_CODE == ROM_CODE_A01

#define CHAR_JP_TSUKI   0xE0    //月
#define CHAR_JP_NICHI   0xE1    //日 
#define CHAR_JP_BUN     0xE2    //分
#define CHAR_JP_LARGE_EN    0xE3    //円
#define CHAR_JP_LARGE_MARU  CHAR_JP_LARGE_EN
#define CHAR_JP_NAKA    0xE4    //中
#define CHAR_JP_YING_YANG   0xE5 

#define CHAR_JP_GA      0xE6    //ガ
#define CHAR_JP_GI      0xE7    //ギ
#define CHAR_JP_GU      0xE8    //グ 
#define CHAR_JP_GE      0xE9    //ゲ
#define CHAR_JP_GO      0xEA    //ゴ
#define CHAR_JP_ZA      0xEB    //ザ
#define CHAR_JP_JI      0xEC    //ジ
#define CHAR_JP_ZU      0xED    //ズ
#define CHAR_JP_ZE      0xEE    //ゼ
#define CHAR_JP_ZO      0xEF    //ゾ

#define CHAR_JP_DA      0xF0    //ダ
#define CHAR_JP_DZI     0xF1    //ヂ
#define CHAR_JP_DI      CHAR_JP_DZI
#define CHAR_JP_DZU     0xF2    //ヅ
#define CHAR_JP_DE      0xF3    //デ
#define CHAR_JP_DO      0xF4    //ド
#define CHAR_JP_PA      0xF5    //パ
#define CHAR_JP_PI      0xF6    //ピ
#define CHAR_JP_PU      0xF7    //プ
#define CHAR_JP_PE      0xF8    //ペ
#define CHAR_JP_PO      0xF9    //ポ
#define CHAR_JP_BA      0xFA    //バ
#define CHAR_JP_BI      0xFB    //ビ
#define CHAR_JP_BU      0xFC    //ブ
#define CHAR_JP_BE      0xFD    //ベ    
#define CHAR_JP_BO      0xFE    //ボ


#endif


#if USE_HIRAGANA_ROM == 1

//These codes are for a custom Japanese Hiragana Character set
//put in the unused ROM space of the standard LCD ROM space

//First empty space, in LCDs this is part of CGRAM
#define HIRAGANA_A      0x00    //あ    
#define HIRAGANA_I      0x01    //い
#define HIRAGANA_U      0x02    //う
#define HIRAGANA_E      0x03    //え
#define HIRAGANA_O      0x04    //お
#define HIRAGANA_KA     0x05    //か
#define HIRAGANA_KI     0x06    //き
#define HIRAGANA_KU     0x07    //く
#define HIRAGANA_KE     0x08    //け
#define HIRAGANA_KO     0x09    //こ
#define HIRAGANA_SA     0x0A    //さ
#define HIRAGANA_SHI    0x0B    //し
#define HIRAGANA_SU     0x0C    //す
#define HIRAGANA_SE     0x0D    //せ
#define HIRAGANA_SO     0x0E    //そ
#define HIRAGANA_TA     0x0F    //た
#define HIRAGANA_CHI    0x10    //ち
#define HIRAGANA_TSU    0x11    //つ
#define HIRAGANA_TE     0x12    //て
#define HIRAGANA_TO     0x13    //と
#define HIRAGANA_NA     0x14    //な
#define HIRAGANA_NI     0x15    //に
#define HIRAGANA_NU     0x16    //ぬ
#define HIRAGANA_NE     0x17    //ね
#define HIRAGANA_NO     0x18    //の
#define HIRAGANA_HA     0x19    //は
#define HIRAGANA_HI     0x1A    //ひ
#define HIRAGANA_HU     0x1B    //ふ
#define HIRAGANA_FU     HIRAGANA_HU
#define HIRAGANA_HE     0x1C    //へ
#define HIRAGANA_HO     0x1D    //ほ
//extras with dakuten, common particles が and だ
#define HIRAGANA_GA     0x1E    //が
#define HIRAGANA_DA     0x1F    //だ

//the second open part of the ROM space
#define HIRAGANA_MA     0x80    //ま
#define HIRAGANA_MI     0x81    //み
#define HIRAGANA_MU     0x82    //む
#define HIRAGANA_ME     0x83    //め
#define HIRAGANA_MO     0x84    //も
#define HIRAGANA_YA     0x85    //や
#define HIRAGANA_YU     0x86    //ゆ
#define HIRAGANA_YO     0x87    //よ
#define HIRAGANA_RA     0x88    //ら
#define HIRAGANA_RI     0x89    //り
#define HIRAGANA_RU     0x8A    //る
#define HIRAGANA_RE     0x8B    //れ
#define HIRAGANA_RO     0x8C    //ろ
#define HIRAGANA_WA     0x8D    //わ
#define HIRAGANA_WO     0x8E    //を
#define HIRAGANA_N      0x8F    //ん
#define HIRAGANA_XTSU   0x90    //っ
#define HIRAGANA_LITTLE_TSU     HIRAGANA_XTSU
#define HIRAGANA_XYA    0x91    //ゃ
#define HIRAGANA_LITTLE_YA      HIRAGANA_XYA
#define HIRAGANA_XYU    0x92    //ゅ
#define HIRAGANA_LITTLE_YU      HIRAGANA_XYU
#define HIRAGANA_XYO    0x93    //ょ
#define HIRAGANA_LITTLE_YO      HIRAGANA_XYO
//more extras with dakuten. Tried to include most common ones
//for conguation and particles in Japanese. 
#define HIRAGANA_GI     0x94    //ぎ
#define HIRAGANA_GU     0x95    //ぐ
#define HIRAGANA_GE     0x96    //げ
#define HIRAGANA_GO     0x97    //ご
#define HIRAGANA_JI     0x98    //じ
#define HIRAGANA_DZU    0x99    //づ
#define HIRAGANA_DU     HIRAGANA_DZU
#define HIRAGANA_DE     0x9A    //で
#define HIRAGANA_DO     0x9B    //ど
#define HIRAGANA_BA     0x9C    //ば
#define HIRAGANA_BI     0x9D    //び
#define HIRAGANA_BU     0x9E    //ぶ
#define HIRAGANA_BE     0x9F    //べ

//NOTE: Other hiragana characters can be made by 
//following the character with the dakuten or handakuten characters.
//defined as CHAR_JP_DAKUTEN and CHAR_JP_HANDAKUTEN in the Katakana set.
//example to make Hiragana 'zo'/'ぞ' use　so/'そ' followed by dakuten/'゛' 
//as shown below
// { HIRAGANA_SO, CHAR_JP_DAKUTEN }

#endif

//make our look up table available to the program
extern const uint8_t ascii_matrices[CHAR_ROM_SIZE][CHAR_COLS];

#endif
