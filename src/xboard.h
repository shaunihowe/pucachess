// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

#define MODE_FORCE          0           // xboard passive mode
#define MODE_GO             1           // xboard think about move mode
#define MODE_PONDER			2			//  xboard think in opponents time
#define MODE_ANALYZE        3           // xboard passive thinking mode

byte mode;                              // what mode are we in

char param[256],command[256];               // input string data
struct t_move usermove;                 // move xboard has sent
void xboardcommandloop();                   // main engine loop
void xboard_command();                      // 'xboard' tells puca that GUI is xboard compatible
void new_command();                         // set engine to basic values
void setboard_command();                    // set board from char fenstri
void post_command();                        // show thinking output
void nopost_command();                      // hide thinking output
void level_command();						// set game time controls
void time_command();                        // set engine's clock
void otim_command();                        // set opponent's clock
void force_command();                       // set passive mode
void easy_command();						// set ponder mode off
void hard_command();						// set ponder mode on
void draw_command();						// the engine has been offered a draw
void result_command();						// the game has ended
void go_command();                          // begin game
void analyze_command();                     // analyze position
void dot_command();                         // show analysys output
void exit_command();                        // exit from analyze mode
void playother_command();                   // set engine to play colour not in turn
void undo_command();                        // enter force mode and undo last move
void remove_command();                      // go back two moves
void quit_command();                        // close the program
void usermove_command(char *mov);           // xboard sends a move
void show_command();                        // show game state
void modecheck();                           // check if it the engines turn to move
