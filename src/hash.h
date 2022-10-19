// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

unsigned int hash_key[9][9][3][7];
unsigned int hash_turn;
unsigned int hash_ep[9];

// move entry
struct t_hash{
	unsigned int hash;
	s_int ascore,bscore;
	byte depth;
};

//struct t_hash hashlock[10][99][99];
void hash_init();		// initialise the hash keys
unsigned int hash_rand();		// generate a random key
unsigned int hash_set(struct t_board *board);	// generate a key from board position
void hash_flush();

