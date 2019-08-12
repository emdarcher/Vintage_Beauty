
#ifndef LCD_CHAR_JP_H
#define LCD_CHAR_JP_H


//definitions for the Japanese characters
//found in HD44780/HD66702 Character LCD Controller ROMs

//symbols, punctuiation, and Small Kana
//
#define CHAR_JP_SPACE   0xA0
// 。
#define CHAR_JP_FULL_STOP    0xA1
#define CHAR_JP_PERIOD  CHAR_JP_FULL_STOP
// 「
#define CHAR_JP_L_BRACKET   0xA2
// 」
#define CHAR_JP_R_BRACKET   0xA3
// 、
#define CHAR_JP_PAUSE   0xA4
// ・
#define CHAR_JP_DOT     0xA5
// ヲ
#define CHAR_JP_WO      0xA6
// ァ
#define CHAR_JP_Sm_A    0xA7
// ィ
#define CHAR_JP_Sm_I    0xA8
// ゥ
#define CHAR_JP_Sm_U    0xA9
// ェ
#define CHAR_JP_Sm_E    0xAA
// ォ
#define CHAR_JP_Sm_O    0xAB
// ャ
#define CHAR_JP_Sm_YA   0xAC
// ュ
#define CHAR_JP_Sm_YU   0xAD
// ョ
#define CHAR_JP_Sm_YO   0xAE
// ッ
#define CHAR_JP_Sm_TSU  0xAF

//ー
#define CHAR_JP_PROLONG 0xB0
#define CHAR_JP_LONG    CHAR_JP_PROLONG
#define CHAR_JP_DASH CHAR_JP_PROLONG

//Full Size Katakana カタカナ、片仮名

//ア、イ、ウ、エ、オ
#define CHAR_JP_A       0xB1
#define CHAR_JP_I       0xB2
#define CHAR_JP_U       0xB3
#define CHAR_JP_E       0xB4
#define CHAR_JP_O       0xB5

//カ、キ、ク、ケ、コ
#define CHAR_JP_KA      0xB6
#define CHAR_JP_KI      0xB7
#define CHAR_JP_KU      0xB8
#define CHAR_JP_KE      0xB9
#define CHAR_JP_KO      0xBA

//サ、シ、ス、セ、ソ
#define CHAR_JP_SA      0xBB
#define CHAR_JP_SHI     0xBC
#define CHAR_JP_SU      0xBD
#define CHAR_JP_SE      0xBE
#define CHAR_JP_SO      0xBF

//タ、チ、ツ、テ、ト
#define CHAR_JP_TA      0xC0
#define CHAR_JP_CHI     0xC1
#define CHAR_JP_TSU     0xC2
#define CHAR_JP_TE      0xC3
#define CHAR_JP_TO      0xC4

//ナ、ニ、ヌ、ネ、ノ
#define CHAR_JP_NA      0xC5
#define CHAR_JP_NI      0xC6
#define CHAR_JP_NU      0xC7
#define CHAR_JP_NE      0xC8
#define CHAR_JP_NO      0xC9

//ハ、ヒ、フ、ヘ、ホ
#define CHAR_JP_HA      0xCA
#define CHAR_JP_HI      0xCB
#define CHAR_JP_HU      0xCC
#define CHAR_JP_FU      CHAR_JP_HU
#define CHAR_JP_HE      0xCD
#define CHAR_JP_HO      0xCE

//マ、ミ、ム、メ、モ
#define CHAR_JP_MA      0xCF
#define CHAR_JP_MI      0xD0
#define CHAR_JP_MU      0xD1
#define CHAR_JP_ME      0xD2
#define CHAR_JP_MO      0xD3

//ヤ、ユ、ヨ
#define CHAR_JP_YA      0xD4
#define CHAR_JP_YU      0xD5
#define CHAR_JP_YO      0xD6

//ラ、リ、ル、レ、ロ
#define CHAR_JP_RA      0xD7
#define CHAR_JP_RI      0xD8
#define CHAR_JP_RU      0xD9
#define CHAR_JP_RE      0xDA
#define CHAR_JP_RO      0xDB

//ワ
#define CHAR_JP_WA      0xDC

//ン
#define CHAR_JP_N       0xDD

//the Dakuten 濁点と半濁点
// ゛
#define CHAR_JP_DAKUTEN 0xDE
#define CHAR_JP_NIGORI  CHAR_JP_DAKUTEN
// ゜
#define CHAR_JP_HANDAKUTEN  0xDF
#define CHAR_JP_MARU    CHAR_JP_HANDAKUTEN

//mini kanji 小さな漢字
//mini 千
#define CHAR_JP_SEN     0xFA
//mini 万
#define CHAR_JP_MAN     0xFB
//mini en (yen) 円
#define CHAR_JP_EN      0xFC

#endif

