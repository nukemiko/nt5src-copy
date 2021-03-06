// charmap.c (FRA vesrion)
// Angshuman Guha, aguha
// Jan 12, 1999
// Modifed Jan 2000 (mrevow)
//  - moved accent marks to front of list
//  - Cleaned up missing characters, added the the Br pound and Euro
//
// Nov 2000
//   - Synch with usa with respect to continuation chars
// Sept 2001
//   - Synch with LM spec dated 9/8/2001 to use the supported chars

#include "common.h"
#include "charmap.h"

const unsigned char g_charMapDesc[] = "French Charmap Last updated 19 Sept 2001 - matches LM spec of 9/8/2001";

#define COUTPUT 200

const BYTE rgCharToOutputNode[2*256] = {
	// the first number is the beginning-activation node
	// the second number is the continuing-activation node
	// the value 255 means there is no output node
	// space ' ( ) , - . / < > [ \ ] ^ _ ` { | }  dont have continuation nodes
	// and accents  ` (0x60) ~ (0x7e) ? (0x88) ? (0xa8) ? (0xb4)  dont have continuation nodes

	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	  0, 255, //   0x20 ( 32)
	  5,   6, // ! 0x21 ( 33)
	  7,   8, // " 0x22 ( 34)
	 32,  33, // # 0x23 ( 35)
	 24,  25, // $ 0x24 ( 36)
	 34,  35, // % 0x25 ( 37)
	 36,  37, // & 0x26 ( 38)
	  9, 255, // ' 0x27 ( 39)
	 10, 255, // ( 0x28 ( 40)
	 11, 255, // ) 0x29 ( 41)
	 38,  39, // * 0x2a ( 42)
	 40,  41, // + 0x2b ( 43)
	 12, 255, // , 0x2c ( 44)
	 13, 255, // - 0x2d ( 45)
	 14, 255, // . 0x2e ( 46)
	 15, 255, // / 0x2f ( 47)
	 56,  57, // 0 0x30 ( 48)
	 58,  59, // 1 0x31 ( 49)
	 60,  61, // 2 0x32 ( 50)
	 62,  63, // 3 0x33 ( 51)
	 64,  65, // 4 0x34 ( 52)
	 66,  67, // 5 0x35 ( 53)
	 68,  69, // 6 0x36 ( 54)
	 70,  71, // 7 0x37 ( 55)
	 72,  73, // 8 0x38 ( 56)
	 74,  75, // 9 0x39 ( 57)
	 16,  17, // : 0x3a ( 58)
	 18,  19, // ; 0x3b ( 59)
	 42, 255, // < 0x3c ( 60)
	 43,  44, // = 0x3d ( 61)
	 45, 255, // > 0x3e ( 62)
	 20,  21, // ? 0x3f ( 63)
	 46,  47, // @ 0x40 ( 64)
	128, 129, // A 0x41 ( 65)
	130, 131, // B 0x42 ( 66)
	132, 133, // C 0x43 ( 67)
	134, 135, // D 0x44 ( 68)
	136, 137, // E 0x45 ( 69)
	138, 139, // F 0x46 ( 70)
	140, 141, // G 0x47 ( 71)
	142, 143, // H 0x48 ( 72)
	144, 145, // I 0x49 ( 73)
	146, 147, // J 0x4a ( 74)
	148, 149, // K 0x4b ( 75)
	150, 151, // L 0x4c ( 76)
	152, 153, // M 0x4d ( 77)
	154, 155, // N 0x4e ( 78)
	156, 157, // O 0x4f ( 79)
	158, 159, // P 0x50 ( 80)
	160, 161, // Q 0x51 ( 81)
	162, 163, // R 0x52 ( 82)
	164, 165, // S 0x53 ( 83)
	166, 167, // T 0x54 ( 84)
	168, 169, // U 0x55 ( 85)
	170, 171, // V 0x56 ( 86)
	172, 173, // W 0x57 ( 87)
	174, 175, // X 0x58 ( 88)
	176, 177, // Y 0x59 ( 89)
	178, 179, // Z 0x5a ( 90)
	 48, 255, // [ 0x5b ( 91)
	 22, 255, // \ 0x5c ( 92)
	 49, 255, // ] 0x5d ( 93)
	 50, 255, // ^ 0x5e ( 94)
	 23, 255, // _ 0x5f ( 95)
	  1, 255, // ` 0x60 ( 96)
	 76,  77, // a 0x61 ( 97)
	 78,  79, // b 0x62 ( 98)
	 80,  81, // c 0x63 ( 99)
	 82,  83, // d 0x64 (100)
	 84,  85, // e 0x65 (101)
	 86,  87, // f 0x66 (102)
	 88,  89, // g 0x67 (103)
	 90,  91, // h 0x68 (104)
	 92,  93, // i 0x69 (105)
	 94,  95, // j 0x6a (106)
	 96,  97, // k 0x6b (107)
	 98,  99, // l 0x6c (108)
	100, 101, // m 0x6d (109)
	102, 103, // n 0x6e (110)
	104, 105, // o 0x6f (111)
	106, 107, // p 0x70 (112)
	108, 109, // q 0x71 (113)
	110, 111, // r 0x72 (114)
	112, 113, // s 0x73 (115)
	114, 115, // t 0x74 (116)
	116, 117, // u 0x75 (117)
	118, 119, // v 0x76 (118)
	120, 121, // w 0x77 (119)
	122, 123, // x 0x78 (120)
	124, 125, // y 0x79 (121)
	126, 127, // z 0x7a (122)
	 51, 255, // { 0x7b (123)
	 52, 255, // | 0x7c (124)
	 53, 255, // } 0x7d (125)
	 54,  55, // ~ 0x7e (126)
	255, 255,
	 28,  29, // ? 0x80 (128)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	  2, 255, // ? 0x88 (136)
	255, 255,
	255, 255,
	255, 255,
	180, 181, // ? 0x8c (140)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	182, 183, // ? 0x9c (156)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	 26,  27, // ? 0xa3 (163)
	255, 255,
	 30,  31, // ? 0xa5 (165)
	255, 255,
	184, 185, // ? 0xa7 (167)
	  3, 255, // ? 0xa8 (168)
	255, 255,
	255, 255,
	186, 187, // ? 0xab (171)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	188, 189, // ? 0xb0 (176)
	255, 255,
	255, 255,
	255, 255,
	  4, 255, // ? 0xb4 (180)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	190, 191, // ? 0xbb (187)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	192, 193, // ? 0xc6 (198)
	194, 195, // ? 0xc7 (199)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	196, 197, // ? 0xe6 (230)
	198, 199, // ? 0xe7 (231)
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255,
	255, 255
};

const BYTE rgOutputNodeToChar[2*COUTPUT] = {
	// the first number is 0 for char-continuation node, 1 for char-beginning node
	// the second number is the character
// A) Space Node	
	1, ' ', // 0 0

// B) Accent characters
	1, '`', // 1 accent mark: grave `
	1, '?', // 2 accent mark: circumflex ^
	1, '?', // 3 accent mark: diaeresis (German umlaut)
	1, '?', // 4 accent mark: accute grave ?
	
// C) common punc
	1, '!', // 5
	0, '!', // 6
	1, '"', // 7
	0, '"', // 8
	1, '\'', // 9
	1, '(', // 10
	1, ')', // 11
	1, ',', // 12
	1, '-', // 13
	1, '.', // 14
	1, '/', // 15
	1, ':', // 16
	0, ':', // 17
	1, ';', // 18
	0, ';', // 19
	1, '?', // 20
	0, '?', // 21
	1, '\\', // 22
	1, '_', // 23

// D) Currency
	1, '$', // 24
	0, '$', // 25
	1, '?', // 26
	0, '?', // 27
	1, '?', // 28 euro
	0, '?', // 29 euro
	1, '?', // 30
	0, '?', // 31

// D) rare punc
	1, '#', // 32
	0, '#', // 33
	1, '%', // 34
	0, '%', // 35
	1, '&', // 36
	0, '&', // 37
	1, '*', // 38
	0, '*', // 39
	1, '+', // 40
	0, '+', // 41
	1, '<', // 42
	1, '=', // 43
	0, '=', // 44
	1, '>', // 45
	1, '@', // 46
	0, '@', // 47
	1, '[', // 48
	1, ']', // 49
	1, '^', // 50
	1, '{', // 51
	1, '|', // 52
	1, '}', // 53
	1, '~', // 54
	0, '~', // 55

// E) digits
	1, '0', // 56
	0, '0', // 57
	1, '1', // 58
	0, '1', // 59
	1, '2', // 60
	0, '2', // 61
	1, '3', // 62
	0, '3', // 63
	1, '4', // 64
	0, '4', // 65
	1, '5', // 66
	0, '5', // 67
	1, '6', // 68
	0, '6', // 69
	1, '7', // 70
	0, '7', // 71
	1, '8', // 72
	0, '8', // 73
	1, '9', // 74
	0, '9', // 75

// F) lowercase alphabets
	1, 'a', // 76
	0, 'a', // 77
	1, 'b', // 78
	0, 'b', // 79
	1, 'c', // 80
	0, 'c', // 81
	1, 'd', // 82
	0, 'd', // 83
	1, 'e', // 84
	0, 'e', // 85
	1, 'f', // 86
	0, 'f', // 87
	1, 'g', // 88
	0, 'g', // 89
	1, 'h', // 90
	0, 'h', // 91
	1, 'i', // 92
	0, 'i', // 93
	1, 'j', // 94
	0, 'j', // 95
	1, 'k', // 96
	0, 'k', // 97
	1, 'l', // 98
	0, 'l', // 99
	1, 'm', // 100
	0, 'm', // 101
	1, 'n', // 102
	0, 'n', // 103
	1, 'o', // 104
	0, 'o', // 105
	1, 'p', // 106
	0, 'p', // 107
	1, 'q', // 108
	0, 'q', // 109
	1, 'r', // 110
	0, 'r', // 111
	1, 's', // 112
	0, 's', // 113
	1, 't', // 114
	0, 't', // 115
	1, 'u', // 116
	0, 'u', // 117
	1, 'v', // 118
	0, 'v', // 119
	1, 'w', // 120
	0, 'w', // 121
	1, 'x', // 122
	0, 'x', // 123
	1, 'y', // 124
	0, 'y', // 125
	1, 'z', // 126
	0, 'z', // 127

//  G) uppercase alphabets
	1, 'A', // 128
	0, 'A', // 129
	1, 'B', // 130
	0, 'B', // 131
	1, 'C', // 132
	0, 'C', // 133
	1, 'D', // 134
	0, 'D', // 135
	1, 'E', // 136
	0, 'E', // 137
	1, 'F', // 138
	0, 'F', // 139
	1, 'G', // 140
	0, 'G', // 141
	1, 'H', // 142
	0, 'H', // 143
	1, 'I', // 144
	0, 'I', // 145
	1, 'J', // 146
	0, 'J', // 147
	1, 'K', // 148
	0, 'K', // 149
	1, 'L', // 150
	0, 'L', // 151
	1, 'M', // 152
	0, 'M', // 153
	1, 'N', // 154
	0, 'N', // 155
	1, 'O', // 156
	0, 'O', // 157
	1, 'P', // 158
	0, 'P', // 159
	1, 'Q', // 160
	0, 'Q', // 161
	1, 'R', // 162
	0, 'R', // 163
	1, 'S', // 164
	0, 'S', // 165
	1, 'T', // 166
	0, 'T', // 167
	1, 'U', // 168
	0, 'U', // 169
	1, 'V', // 170
	0, 'V', // 171
	1, 'W', // 172
	0, 'W', // 173
	1, 'X', // 174
	0, 'X', // 175
	1, 'Y', // 176
	0, 'Y', // 177
	1, 'Z', // 178
	0, 'Z', // 179

//	H) Euro chars
	1, '?', // 180
	0, '?', // 181
	1, '?', // 182
	0, '?', // 183
	1, '?', // 184
	0, '?', // 185
	1, '?', // 186
	0, '?', // 187
	1, '?', // 188
	0, '?', // 189
	1, '?', // 190
	0, '?', // 191
	1, '?', // 192
	0, '?', // 193
	1, '?', // 194
	0, '?', // 195
	1, '?', // 196
	0, '?', // 197
	1, '?', // 198
	0, '?', // 199
};

/*
BYTE BeginChar2Out(char ch)
{
	BYTE x;

	x = rgCharToOutputNode[2*(ch)];
	assert(x < COUTPUT);
	return x;
}

BYTE ContinueChar2Out(char ch)
{
	BYTE x;

	x = rgCharToOutputNode[2*(ch)+1];
	assert(x < COUTPUT);
	return x;
}
*/

const BYTE rgVirtualChar[2*256] = 
{
	// these are European accented characters which we currently represent as the
	// simultaneous occurrence of a base character and an accent character
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00, // ? (0x9f) == Y ? 
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x41, 0x60, // ? (0xc0) == A ` 
	0x00, 0x00, // ? (0xc1) == A ? 
	0x41, 0x88, // ? (0xc2) == A ? 
	0x00, 0x00, // ? (0xc3) == A ~ 
	0x41, 0xa8, // ? (0xc4) == A ? 
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x45, 0x60, // ? (0xc8) == E ` 
	0x45, 0xb4, // ? (0xc9) == E ? 
	0x45, 0x88, // ? (0xca) == E ? 
	0x45, 0xa8, // ? (0xcb) == E ? 
	0x00, 0x00, // ? (0xcc) == I ` 
	0x00, 0x00, // ? (0xcd) == I ? 
	0x49, 0x88, // ? (0xce) == I ? 
	0x49, 0xa8, // ? (0xcf) == I ? 
	0x00, 0x00,
	0x00, 0x00, // ? (0xd1) == N ~ 
	0x00, 0x00, // ? (0xd2) == O ` 
	0x00, 0x00, // ? (0xd3) == O ? 
	0x4f, 0x88, // ? (0xd4) == O ? 
	0x00, 0x00, // ? (0xd5) == O ~ 
	0x00, 0x00, // ? (0xd6) == O ? 
	0x00, 0x00,
	0x00, 0x00,
	0x55, 0x60, // ? (0xd9) == U ` 
	0x00, 0x00, // ? (0xda) == U ? 
	0x55, 0x88, // ? (0xdb) == U ? 
	0x55, 0xa8, // ? (0xdc) == U ? 
	0x00, 0x00, // ? (0xdd) == Y ? 
	0x00, 0x00,
	0x00, 0x00,
	0x61, 0x60, // ? (0xe0) == a ` 
	0x00, 0x00, // ? (0xe1) == a ? 
	0x61, 0x88, // ? (0xe2) == a ? 
	0x00, 0x00, // ? (0xe3) == a ~ 
	0x61, 0xa8, // ? (0xe4) == a ? 
	0x00, 0x00,
	0x00, 0x00,
	0x00, 0x00,
	0x65, 0x60, // ? (0xe8) == e ` 
	0x65, 0xb4, // ? (0xe9) == e ? 
	0x65, 0x88, // ? (0xea) == e ? 
	0x65, 0xa8, // ? (0xeb) == e ? 
	0x00, 0x00, // ? (0xec) == i ` 
	0x00, 0x00, // ? (0xed) == i ? 
	0x69, 0x88, // ? (0xee) == i ? 
	0x69, 0xa8, // ? (0xef) == i ? 
	0x00, 0x00,
	0x00, 0x00, // ? (0xf1) == n ~ 
	0x00, 0x00, // ? (0xf2) == o ` 
	0x00, 0x00, // ? (0xf3) == o ? 
	0x6f, 0x88, // ? (0xf4) == o ? 
	0x00, 0x00, // ? (0xf5) == o ~ 
	0x00, 0x00, // ? (0xf6) == o ? 
	0x00, 0x00,
	0x00, 0x00,
	0x75, 0x60, // ? (0xf9) == u ` 
	0x00, 0x00, // ? (0xfa) == u ? 
	0x75, 0x88, // ? (0xfb) == u ? 
	0x75, 0xa8, // ? (0xfc) == u ? 
	0x00, 0x00, // ? (0xfd) == y ? 
	0x00, 0x00,
	0x00, 0x00  // ? (0xff) == y ? 
};
