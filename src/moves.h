// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

void domove(struct t_board *board, struct t_move *move);                                  // do a move
void undomove(struct t_board *board);                                                   // undo last move

void addmove(byte depth, byte src, byte des, byte pro);  // add moves to legal move list
void sortpv();									// find the pv move if any
void sorttt();									// find the tt entry move if any
void sortmoves(byte nextmove);					// find next best move
byte checktest(struct t_board *board, byte turn);						// is king in check
byte validmove(struct t_board *board, struct t_move *move);				// check legality of the move
byte repeattest(struct t_board *board);							// check for 3 time repitition
byte draw(struct t_board *board);										// end game by draw

