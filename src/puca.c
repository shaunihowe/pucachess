// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

#include "version.h"
#include "puca.h"
#include "xboard.h"
#include "hash.h"
#include "search.h"
#include "moves.h"
#include "evaluate.h"
#include "generate.h"
#include "xboard.c"
#include "hash.c"
#include "search.c"
#include "moves.c"
#include "evaluate.c"
#include "generate.c"

int main (int argc, char *argv[])
{
    byte a;
    showthinking=false;
    programclose=false;
    thread_signal=SIGNAL_READY;
    thread_state=THREAD_IDLE;
    mode=MODE_FORCE;
   	memset(rootboard.square, 0, sizeof(rootboard.square));
   	memset(rootboard.movehistory, 0, sizeof(rootboard.movehistory));
   	memset(movelist, 0, sizeof(movelist));
    hash_init();
    printf("##########################################################\n");
    printf("#                       PUCA CHESS                       #\n");
    printf("#     Programmed By Shaun Howe (Twitter: @shaunhowe)     #\n");
    printf("##########################################################\n");
    // configure difficulty
    if ( argc != 2 ) 
    {
        /* We print argv[0] assuming it is the program name */
        printf("# usage: %s difficulty\n", argv[0]);
        printf("#      Difficulty: beginner|amateur|expert|master|grandmaster\n");
        argv[1] = "grandmaster";
    }

    if (!strcmp(argv[1], "beginner"))
        {knowledge=220+(rand()%280);maxsearchdepth=2+(rand()%2);}
    else if (!strcmp(argv[1], "amateur"))
        {knowledge=180+(rand()%100);maxsearchdepth=4+(rand()%3);}
    else if (!strcmp(argv[1], "expert"))
        {knowledge=62+(rand()%60);maxsearchdepth=7+(rand()%3);}
    else if (!strcmp(argv[1], "master"))
        {knowledge=16+(rand()%30);maxsearchdepth=11+(rand()%3);}
    else
        {knowledge=0;maxsearchdepth=99;argv[1] = "grandmaster";}
    printf("# Difficulty: '%s'\n",argv[1]);
    new_command();
    show_command();
    xboardcommandloop();
    return 0;
}

void resetboard(struct t_board *board)
{
    byte x,y;
    board->turn = WHITEARMY;
    board->nummoves = 0;
    board->wk = true;
    board->wq = true;
    board->wc = false;
    board->bk = true;
    board->bq = true;
    board->bc = false;
    board->ep = false;
    board->numpieces = 32;
    board->wkpos = SQR_E1;
    board->bkpos = SQR_E8;
    board->fifty = 0;

    // OutofBound Squares
    for (x=0;x<=119;x++)
        board->square[x] = OUTOFBOUNDS;

    // Empty Squares
    for (y=1;y<=8;y++)
        for (x=1;x<=8;x++)
            board->square[squareinmemory(x, y)] = BLANK;

// Set White Pieces
//  define piece type; specify piece location
    board->square[SQR_A2] = WHITEPAWN;
    board->square[SQR_B2] = WHITEPAWN;
    board->square[SQR_C2] = WHITEPAWN;
    board->square[SQR_D2] = WHITEPAWN;
    board->square[SQR_E2] = WHITEPAWN;
    board->square[SQR_F2] = WHITEPAWN;
    board->square[SQR_G2] = WHITEPAWN;
    board->square[SQR_H2] = WHITEPAWN;
    board->square[SQR_A1] = WHITEROOK;
    board->square[SQR_B1] = WHITEKNIGHT;
    board->square[SQR_C1] = WHITEBISHOP;
    board->square[SQR_D1] = WHITEQUEEN;
    board->square[SQR_E1] = WHITEKING;
    board->square[SQR_F1] = WHITEBISHOP;
    board->square[SQR_G1] = WHITEKNIGHT;
    board->square[SQR_H1] = WHITEROOK;

// Set Black Pieces
    board->square[SQR_A7] = BLACKPAWN;
    board->square[SQR_B7] = BLACKPAWN;
    board->square[SQR_C7] = BLACKPAWN;
    board->square[SQR_D7] = BLACKPAWN;
    board->square[SQR_E7] = BLACKPAWN;
    board->square[SQR_F7] = BLACKPAWN;
    board->square[SQR_G7] = BLACKPAWN;
    board->square[SQR_H7] = BLACKPAWN;
    board->square[SQR_A8] = BLACKROOK;
    board->square[SQR_B8] = BLACKKNIGHT;
    board->square[SQR_C8] = BLACKBISHOP;
    board->square[SQR_D8] = BLACKQUEEN;
    board->square[SQR_E8] = BLACKKING;
    board->square[SQR_F8] = BLACKBISHOP;
    board->square[SQR_G8] = BLACKKNIGHT;
    board->square[SQR_H8] = BLACKROOK;
    return;
}

void setboard(struct t_board *board)                   // Sets board from 'char fenstring'
{
    byte x,y,tmp,section,tmp2,whitespace;
    board->turn = WHITEARMY;
    board->nummoves = 0;
    board->wk = false;
    board->wq = false;
    board->bk = false;
    board->bq = false;
    board->ep = false;
    board->numpieces = 0; // increment as we find them
    board->wkpos = 0;    // same as above
    board->bkpos = 0;    // same as above

    // OutofBound Squares
    for (x=0;x<=119;x++)
        board->square[x] = OUTOFBOUNDS;

    // Empty Squares
    for (y=1;y<=8;y++)
        for (x=1;x<=8;x++)
            board->square[squareinmemory(x, y)] = BLANK;
    //setboard rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    section=1;
    x = 1;
    y = 8;
    for (tmp=0;tmp<strlen(fenstring);tmp++)
    {
        if (section == 1)
        { // in board layout section of string
            if (fenstring[tmp] == ' ')
                section=2; // goto turn section
            else if (fenstring[tmp] == '/')
            {
                y=y-1;
                x=1;
            }
            else if (fenstring[tmp] == 'p')
            {
                board->square[squareinmemory(x,y)] = BLACKPAWN;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'b')
            {
                board->square[squareinmemory(x,y)] = BLACKBISHOP;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'n')
            {
                board->square[squareinmemory(x,y)] = BLACKKNIGHT;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'r')
            {
                board->square[squareinmemory(x,y)] = BLACKROOK;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'q')
            {
                board->square[squareinmemory(x,y)] = BLACKQUEEN;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'k')
            {
                board->square[squareinmemory(x,y)] = BLACKKING;
                board->numpieces++;
                board->bkpos=squareinmemory(x,y);
                x++;
            }
            else if (fenstring[tmp] == 'P')
            {
                board->square[squareinmemory(x,y)] = WHITEPAWN;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'B')
            {
                board->square[squareinmemory(x,y)] = WHITEBISHOP;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'N')
            {
                board->square[squareinmemory(x,y)] = WHITEKNIGHT;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'R')
            {
                board->square[squareinmemory(x,y)] = WHITEROOK;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'Q')
            {
                board->square[squareinmemory(x,y)] = WHITEQUEEN;
                board->numpieces++;
                x++;
            }
            else if (fenstring[tmp] == 'K')
            {
                board->square[squareinmemory(x,y)] = WHITEKING;
                board->numpieces++;
                board->wkpos=squareinmemory(x,y);
                x++;
            }
            else if ((fenstring[tmp] > '0')&&(fenstring[tmp] < '9'))
            {
                whitespace = fenstring[tmp] - 48;
                for (tmp2=1;tmp2<=whitespace;tmp2++)
                    x++;
            }
        }
        else if (section == 2)
        {   // turn section of string
            if (fenstring[tmp] == ' ')
                section=3; // castling section of string
            else if (fenstring[tmp] == 'w')
                board->turn = WHITEARMY;
            else if (fenstring[tmp] == 'b')
                board->turn = BLACKARMY;
        }
        else if (section == 3)
        {   // castling section of string
            if (fenstring[tmp] == ' ')
                section=4; // en-passant section of string
            else if (fenstring[tmp] == 'K')
                board->wk = true;
            else if (fenstring[tmp] == 'Q')
                board->wq = true;
            else if (fenstring[tmp] == 'k')
                board->bk = true;
            else if (fenstring[tmp] == 'q')
                board->bq = true;
        }
        else if (section == 4)
        {  // en-passant section of string
            if (fenstring[tmp] == 'a')
                board->ep = 1;
            else if (fenstring[tmp] == 'b')
                board->ep = 2;
            else if (fenstring[tmp] == 'c')
                board->ep = 3;
            else if (fenstring[tmp] == 'd')
                board->ep = 4;
            else if (fenstring[tmp] == 'e')
                board->ep = 5;
            else if (fenstring[tmp] == 'f')
                board->ep = 6;
            else if (fenstring[tmp] == 'g')
                board->ep = 7;
            else if (fenstring[tmp] == 'h')
                board->ep = 8;
        }
    }
    return;
}

//int piece(int value);                                                  // Returns what piece is in square
byte piece(s_int value)
{
    if (value > BLACKARMY)
        return (value - BLACKARMY);
    else if (value > WHITEARMY)
        return (value - WHITEARMY);
    else
        return BLANK;
}

//int army(int value);                                                   // Returns WHITEARMY || BLACKARMY
byte army(s_int value)
{
    if (value > BLACKARMY)
        return BLACKARMY;
    else if (value > WHITEARMY)
        return WHITEARMY;
    else
        return BLANK;
}

//int squareinmemory(int x, int y);                                      // Returns 0-143 from X,Y Coords
byte squareinmemory(byte x, byte y)
{
    signed int st;
    st=0;
    if (y == 1)
        st = 20;
    else if (y == 2)
        st = 30;
    else if (y == 3)
        st = 40;
    else if (y == 4)
        st = 50;
    else if (y == 5)
        st = 60;
    else if (y == 6)
        st = 70;
    else if (y == 7)
        st = 80;
    else if (y == 8)
        st = 90;
    return (st + x);
}

void printboard(struct t_board *board)
{
    byte cursqr,cursqrarmy,cursqrpiece,x,y;
    printf("# Current Position\n");
    for (y=8;y>=1;y--)
    {
        printf("# ");
        for (x=1;x<=8;x++)
        {
            cursqr = board->square[squareinmemory(x,y)];
            cursqrarmy = army(cursqr);
            cursqrpiece = piece(cursqr);
            if (cursqrarmy == WHITEARMY)
                printf("W");
            else if (cursqrarmy == BLACKARMY)
                printf("B");
            else
                printf(".");
            if (cursqrpiece == PAWN)
                printf("P");
            else if (cursqrpiece == BISHOP)
                printf("B");
            else if (cursqrpiece == KNIGHT)
                printf("N");
            else if (cursqrpiece == ROOK)
                printf("R");
            else if (cursqrpiece == QUEEN)
                printf("Q");
            else if (cursqrpiece == KING)
                printf("K");
            else
                printf(".");
            printf(" ");
        }
        printf("\n");
    }
    if (board->turn == WHITEARMY)
        printf("# Whites Turn        %i pieces on board\n",board->numpieces);
    else
        printf("# Blacks Turn        %i pieces on board\n",board->numpieces);
    fflush(stdout);
    return;
}

void printmove(byte mpiece, byte msrc, byte mdes, byte mpromote)
{
    byte x,y,rowstart;
    x=0;y=0;rowstart=0;

    if (mpiece > WHITEARMY){mpiece=piece(mpiece);}
    if (mpiece == PAWN)
        printf("P");
    else if (mpiece == BISHOP)
        printf("B");
    else if (mpiece == KNIGHT)
        printf("N");
    else if (mpiece == ROOK)
        printf("R");
    else if (mpiece == QUEEN)
        printf("Q");
    else if (mpiece == KING)
        printf("K");


    if ((msrc<100)&&(msrc>90))
    {y=8;rowstart=90;}
    else if ((msrc<90)&&(msrc>80))
    {y=7;rowstart=80;}
    else if ((msrc<80)&&(msrc>70))
    {y=6;rowstart=70;}
    else if ((msrc<70)&&(msrc>60))
    {y=5;rowstart=60;}
    else if ((msrc<60)&&(msrc>50))
    {y=4;rowstart=50;}
    else if ((msrc<50)&&(msrc>40))
    {y=3;rowstart=40;}
    else if ((msrc<40)&&(msrc>30))
    {y=2;rowstart=30;}
    else if ((msrc<30)&&(msrc>20))
    {y=1;rowstart=20;}
    x = msrc - rowstart;
    if (x==1){printf("a");}
    if (x==2){printf("b");}
    if (x==3){printf("c");}
    if (x==4){printf("d");}
    if (x==5){printf("e");}
    if (x==6){printf("f");}
    if (x==7){printf("g");}
    if (x==8){printf("h");}
    if (y==1){printf("1");}
    if (y==2){printf("2");}
    if (y==3){printf("3");}
    if (y==4){printf("4");}
    if (y==5){printf("5");}
    if (y==6){printf("6");}
    if (y==7){printf("7");}
    if (y==8){printf("8");}


    if ((mdes<100)&&(mdes>90))
    {y=8;rowstart=90;}
    else if ((mdes<90)&&(mdes>80))
    {y=7;rowstart=80;}
    else if ((mdes<80)&&(mdes>70))
    {y=6;rowstart=70;}
    else if ((mdes<70)&&(mdes>60))
    {y=5;rowstart=60;}
    else if ((mdes<60)&&(mdes>50))
    {y=4;rowstart=50;}
    else if ((mdes<50)&&(mdes>40))
    {y=3;rowstart=40;}
    else if ((mdes<40)&&(mdes>30))
    {y=2;rowstart=30;}
    else if ((mdes<30)&&(mdes>20))
    {y=1;rowstart=20;}
    x = mdes - rowstart;
    if (x==1){printf("a");}
    if (x==2){printf("b");}
    if (x==3){printf("c");}
    if (x==4){printf("d");}
    if (x==5){printf("e");}
    if (x==6){printf("f");}
    if (x==7){printf("g");}
    if (x==8){printf("h");}
    if (y==1){printf("1");}
    if (y==2){printf("2");}
    if (y==3){printf("3");}
    if (y==4){printf("4");}
    if (y==5){printf("5");}
    if (y==6){printf("6");}
    if (y==7){printf("7");}
    if (y==8){printf("8");}

    if (piece(mpromote) == QUEEN){printf("Q");}
    else if (piece(mpromote) == ROOK){printf("R");}
    else if (piece(mpromote) == KNIGHT){printf("N");}
    else if (piece(mpromote) == BISHOP){printf("B");}
    printf(" ");
    fflush(stdout);
    return;
}

