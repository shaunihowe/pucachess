// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

// clock settings
int clock_time;                         // time on computers clock
int clock_otim;                         // time on opponents clock
float clock_moves;						// number of moves per time control
float clock_movesleft;					// number of moves remaining in this control
float clock_gametime;						// minutes per time control
float clock_inc;                          // seconds to add for each move
clock_t starttime;                         // time as of starting to think
float curtime;                           // current length of thinking
clock_t stoptime;                          // length of time to think about move
byte maxsearchdepth;                        // depth to search to on iterative deepening
jmp_buf checkpoint;
byte ponder;							// is ponder enabled?
byte drawoffer;							// has the engine's opponent offered a draw?
s_int drawscore;

struct t_board tmpboard;               // the scratch board for this thread
byte pvl[101];						// pv length
struct t_move pv[101][101];			// the line it thinks best so far
byte onpv;							// are we on the pv line?
byte onextension;					// how many extentions have we used
int history[99][99];				// history heurustic
struct t_move follows[99][99];		// create move lists for combination spotting
struct t_ttable transtable[TT_SIZE]; // trans table
byte movesavail[101];				// number of moves availible
struct t_move movelist[101][128];	// the legal move list for each depth
byte curdepth;							// depth of current iteration
byte curmove;							// which move are we thinking about
byte depth;								// depth of current position
byte depthreached;						// depth searched to
byte qsdepthreached;					// depth qsearch reached
unsigned int hashhit;                   // number of hits
unsigned int hashlook;                  // number of lookups
unsigned int nodes;                // number of positions searched
s_int score;                        // the score so far

// thread settings
#define THREAD_IDLE         0               // passive mode, wait for another command
#define THREAD_THINKING     1               // continue to think about next move

#define SIGNAL_READY        0               // thread ready for instruction
#define SIGNAL_TERMINATE    1               // tell thread to stop thinking and die
#define SIGNAL_TERM_MOVE    2               // tell thread to stop thinking, move then die

pthread_t thread_id;                       // posix thread library id
byte thread_state;                         // thread state
byte thread_signal;                        // place for thread to receive terminate/starting signals

void start_thread();           // start a thread thinking
void stop_thread();            // stop the thread
void stop_thread_move();	   // stop the thread and move

void thinkmove();            // thinkmove thread function
void stopthinking();		  // code to stop the thread and move
s_int alphabeta(byte d, s_int a, s_int b);
s_int qsearch(s_int a, s_int b);

