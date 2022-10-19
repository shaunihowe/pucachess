// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

void hash_init()
{
	byte x,y;
   	memset(hash_key, 0, sizeof(hash_key));
   	memset(hash_ep, 0, sizeof(hash_ep));
   	hash_flush();
    srand(time(0));
	for (x=1;x<=8;x++)
	{
		for (y=1;y<=8;y++)
		{
			hash_key[x][y][1][PAWN] = hash_rand();
			hash_key[x][y][2][PAWN] = hash_rand();
			hash_key[x][y][1][BISHOP] = hash_rand();
			hash_key[x][y][2][BISHOP] = hash_rand();
			hash_key[x][y][1][KNIGHT] = hash_rand();
			hash_key[x][y][2][KNIGHT] = hash_rand();
			hash_key[x][y][1][ROOK] = hash_rand();
			hash_key[x][y][2][ROOK] = hash_rand();
			hash_key[x][y][1][QUEEN] = hash_rand();
			hash_key[x][y][2][QUEEN] = hash_rand();
			hash_key[x][y][1][KING] = hash_rand();
			hash_key[x][y][2][KING] = hash_rand();
		}
	}
	hash_turn = hash_rand();
	for (x=0;x<=8;x++){hash_ep[x] = hash_rand();}
	return;
}

unsigned int hash_rand()
{
	unsigned int i;
	unsigned int r = 0;
	for (i = 0; i < 32; ++i)
		r ^= rand() << i;
	return r;
}

unsigned int hash_set(struct t_board *board)
{
	byte x,y,c,p,sim;
	unsigned int h;
	h = 0;
	for (x=1;x<=8;x++)
	{
		for (y=1;y<=8;y++)
		{
			sim = squareinmemory(x,y);
			if (board->square[sim]!=BLANK)
			{
				c = army(board->square[sim]);
				p = piece(board->square[sim]);
				h ^= hash_key[x][y][c][p];
			}
		}
	}
	if (board->turn == BLACKARMY){h ^= hash_turn;}
	h ^= board->ep;
	
	return h;
}

//void hash_flush();
void hash_flush()
{
	memset(transtable, 0, sizeof(transtable));
	return;
}

