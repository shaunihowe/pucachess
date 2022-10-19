// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

// header file includes
#include <pthread.h>
#define NUM_THREADS     1
#include <time.h>                       // for time stuff
#include <stdio.h>                      // standard i/o library
#include <string.h>                     // output formatting
#include <setjmp.h>

// Army values
#define WHITEARMY           10          // White army value
#define BLACKARMY           20          // Black army value

// Piece values
#define PAWN                1           // Pawn value
#define KNIGHT              2           // Knight value
#define BISHOP              3           // Bishop value
#define ROOK                4           // Rook value
#define QUEEN               5           // Queen value
#define KING                6           // King value

// Square content values
#define BLANK               0          // a blank square
#define OUTOFBOUNDS         1          // square is out of bounds
#define WHITEPAWN           11         // contains a white pawn (WHITEARMY+PAWN OR 10+1)
#define BLACKPAWN           21         // black pawn
#define WHITEKNIGHT         12         // white knight
#define BLACKKNIGHT         22         // black knight
#define WHITEBISHOP         13         // white bishop
#define BLACKBISHOP         23         // black bishop
#define WHITEROOK           14         // white rook
#define BLACKROOK           24         // black rook
#define WHITEQUEEN          15         // white queen
#define BLACKQUEEN          25         // black queen
#define WHITEKING           16         // white king
#define BLACKKING           26         // black king

// Square numbers
#define SQR_A1                21
#define SQR_B1                22
#define SQR_C1                23
#define SQR_D1                24
#define SQR_E1                25
#define SQR_F1                26
#define SQR_G1                27
#define SQR_H1                28
#define SQR_A2                31
#define SQR_B2                32
#define SQR_C2                33
#define SQR_D2                34
#define SQR_E2                35
#define SQR_F2                36
#define SQR_G2                37
#define SQR_H2                38
#define SQR_A3                41
#define SQR_B3                42
#define SQR_C3                43
#define SQR_D3                44
#define SQR_E3                45
#define SQR_F3                46
#define SQR_G3                47
#define SQR_H3                48
#define SQR_A4                51
#define SQR_B4                52
#define SQR_C4                53
#define SQR_D4                54
#define SQR_E4                55
#define SQR_F4                56
#define SQR_G4                57
#define SQR_H4                58
#define SQR_A5                61
#define SQR_B5                62
#define SQR_C5                63
#define SQR_D5                64
#define SQR_E5                65
#define SQR_F5                66
#define SQR_G5                67
#define SQR_H5                68
#define SQR_A6                71
#define SQR_B6                72
#define SQR_C6                73
#define SQR_D6                74
#define SQR_E6                75
#define SQR_F6                76
#define SQR_G6                77
#define SQR_H6                78
#define SQR_A7                81
#define SQR_B7                82
#define SQR_C7                83
#define SQR_D7                84
#define SQR_E7                85
#define SQR_F7                86
#define SQR_G7                87
#define SQR_H7                88
#define SQR_A8                91
#define SQR_B8                92
#define SQR_C8                93
#define SQR_D8                94
#define SQR_E8                95
#define SQR_F8                96
#define SQR_G8                97
#define SQR_H8                98

#define CASTLING_DISABLE      1

// Other stuff
#define false               0           // false value
#define true                1           // true value
#define PLAYER_HUMAN        0           // human player
#define PLAYER_CPU          1           // computer player
#ifndef byte
    typedef unsigned int byte;         // byte value
    //typedef unsigned short byte;
#endif
#ifndef s_int
    typedef signed long s_int;           // value for all evaulations
#endif

byte pos_file[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

byte pos_rank[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 0,
    0, 4, 4, 4, 4, 4, 4, 4, 4, 0,
    0, 5, 5, 5, 5, 5, 5, 5, 5, 0,
    0, 6, 6, 6, 6, 6, 6, 6, 6, 0,
    0, 7, 7, 7, 7, 7, 7, 7, 7, 0,
    0, 8, 8, 8, 8, 8, 8, 8, 8, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

byte flip[120] = {
    110,111,112,113,114,115,116,117,118,119,
    100,101,102,103,104,105,106,107,108,109,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9
};

// gamestate values
byte showthinking;                      // show analysys output
byte programclose;                      // quit command has been issued
char fenstring[90];                     // fen string to set board to

byte enginecolour;                      // which colour is the engine playing

// move entry
struct t_move{
    byte source;                        // move starting square
    byte destination;                   // move ending square
    byte promotion;                     // piece to promote to if any
    byte capture;                       // piece captured if any
	s_int eval;
};
struct t_undomove{
    byte source;                        // move starting square
    byte destination;                   // move ending square
    byte promotion;                     // piece to promote to if any
    byte capture;                       // piece captured if any
    byte ep;                            // was move an en passant move?
    byte wq;                            // could white castle to queens side?
    byte wk;                            // could white castle to kings side?
    byte wc;							// has white castled?
    byte bq;                            // could black castle to queens side?
    byte bk;                            // could black castle to kings side?
    byte bc;							// has black castled?
    byte fifty;
    unsigned int hash;							// hash key before move
};
// The board in memory
struct t_board{
    byte turn;                          // whose turn is it (10 or 20)
    byte nummoves;                      // num of moves played
    struct t_undomove movehistory[1000];    // move history and undo information
    byte square[120];                   // square contents (10x12 including outofbounds)
    byte wk;                            // can white castle to kings side?
    byte wq;                            // can white castle to queens side?
    byte wc;							// has white castled?
    byte bk;                            // can black castle to kings side?
    byte bq;                            // can black castle to queens side?
    byte bc;							// has black castled?
    byte ep;                            // which column can ep happen?, if any
    byte numpieces;                     // number of pieces
    byte wkpos;                         // where is the white king
    byte bkpos;                         // where is the black king
    byte fifty;							// count moves for fifty move rule
    unsigned int hash;					// hash key for current position
};

// Trans table entry
struct t_ttable{
    unsigned int hash;
    byte source;
    byte destination;
    byte depth;
    byte numpieces;
    s_int eval;
    byte evaltype;
};

#define TT_SIZE         4468998         // number of entries in trans table

struct t_board rootboard;               // the game board in memory

void resetboard(struct t_board *board);                      // resets the board position to the starting position
void setboard(struct t_board *board);	                    // Sets board from 'char fenstring'
byte piece(s_int value);                // returns what piece is in square
byte army(s_int value);                 // returns WHITEARMY || BLACKARMY
byte squareinmemory(byte x, byte y);    // returns 0-144 from X,Y coords
void printboard(struct t_board *board);                     // display board to screen
void printmove(byte mpiece, byte msrc, byte mdes, byte mpromote);    // display move

