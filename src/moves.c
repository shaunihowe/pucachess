// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

void domove(struct t_board *board, struct t_move *move)                                  // do a move
{
    byte srcpiece,src;
    byte cap,des;
    byte pro;
    byte ep;

    src = move->source;
    srcpiece = board->square[src];
    des = move->destination;
    cap = board->square[des];
    pro = move->promotion;

    board->nummoves++;               // increment move number
    board->movehistory[board->nummoves].source=src;               // set undo move information
    board->movehistory[board->nummoves].destination=des;
    board->movehistory[board->nummoves].promotion=pro;
    board->movehistory[board->nummoves].capture=cap;
    board->movehistory[board->nummoves].ep=board->ep;
    board->movehistory[board->nummoves].wq=board->wq;
    board->movehistory[board->nummoves].wk=board->wk;
    board->movehistory[board->nummoves].wc=board->wc;
    board->movehistory[board->nummoves].bq=board->wq;
    board->movehistory[board->nummoves].bk=board->bk;
    board->movehistory[board->nummoves].bc=board->bc;
    board->movehistory[board->nummoves].fifty=board->fifty;
    board->movehistory[board->nummoves].hash=board->hash;

    board->square[src]=BLANK;        // move the piece
    board->square[des]=srcpiece;
    
    if (cap==WHITEKING){board->wkpos=0;}
    else if (cap==BLACKKING){board->bkpos=0;}
    
    board->fifty++;    
    if (cap!=BLANK){--board->numpieces;board->fifty=0;}
    
    if (pro!=BLANK){board->square[des]=pro;}     // if it was a promotion change the pawn
	if (srcpiece==WHITEPAWN)
	{
		board->fifty=0;
		if (pos_rank[src]==5)
			if (pos_file[des]==board->ep)
			{
				// move was ep, take the pawn off the board
				board->square[squareinmemory(board->ep,5)]=BLANK;
			}
		board->ep=0;								// reset ep
		if (pos_rank[src]==2)
			if (pos_rank[des]==4)
				board->ep=pos_file[src];
	}
	else if (srcpiece==BLACKPAWN)
	{
		board->fifty=0;
		if (pos_rank[src]==4)
			if (pos_file[des]==board->ep)
			{
				// move was ep, take the pawn off the board
				board->square[squareinmemory(board->ep,4)]=BLANK;
			}
		board->ep=0;								// reset ep
		if (pos_rank[src]==7)
			if (pos_rank[des]==5)
				board->ep=pos_file[src];
	}
	else
	{
		board->ep=0;
	}
	if (srcpiece==WHITEKING)
    {
        board->wkpos=des;                // update king position
        if (src==SQR_E1)
        {
            if (des==SQR_G1)
            {
             	// move was a castling move 0-0
                board->square[SQR_H1]=BLANK;
                board->square[SQR_F1]=WHITEROOK;
                board->wc=1;
            }
            else if (des==SQR_C1)
            {
             	// move was a castling move 0-0-0
                board->square[SQR_A1]=BLANK;
                board->square[SQR_D1]=WHITEROOK;
                board->wc=2;
            }
        }
    }
    else if (srcpiece==BLACKKING)
    {
        board->bkpos=des;                // update king position
        if (src==SQR_E8)
        {
            if (des==SQR_G8)
            {
             	// move was a castling move 0-0
                board->square[SQR_H8]=BLANK;
                board->square[SQR_F8]=BLACKROOK;
                board->bc=1;
            }
            else if (des==SQR_C8)
            {
             	// move was a castling move 0-0-0
                board->square[SQR_A8]=BLANK;
                board->square[SQR_D8]=BLACKROOK;
                board->bc=2;
            }
        }
    }
    // check if move prevents future castling
    if (src==SQR_E1){board->wq=false;board->wk=false;}
    else if (des==SQR_E1){board->wq=false;board->wk=false;}
    else if (src==SQR_A1){board->wq=false;}
    else if (des==SQR_A1){board->wq=false;}
    else if (src==SQR_H1){board->wk=false;}
    else if (des==SQR_H1){board->wk=false;}
    else if (src==SQR_E8){board->bq=false;board->bk=false;}
    else if (des==SQR_E8){board->bq=false;board->bk=false;}
    else if (src==SQR_A8){board->bq=false;}
    else if (des==SQR_A8){board->bq=false;}
    else if (src==SQR_H8){board->bk=false;}
    else if (des==SQR_H8){board->bk=false;}
    
    if (cap==WHITEKING){board->wkpos=0;}
    if (cap==BLACKKING){board->bkpos=0;}
    
    board->turn=30-board->turn;
    board->hash = hash_set(board);
    return;
}

void undomove(struct t_board *board)                                  // undo a move
{
    byte srcpiece,src;
    byte cap,des;
    byte pro;
    if (board->nummoves<1){return;}
    src = board->movehistory[board->nummoves].source;
    des = board->movehistory[board->nummoves].destination;
    cap = board->movehistory[board->nummoves].capture;
    srcpiece = board->square[des];
    pro = board->movehistory[board->nummoves].promotion;
    
    if (cap!=BLANK){board->numpieces++;}
    
    if (cap==WHITEKING){board->wkpos=des;}
    else if (cap==BLACKKING){board->bkpos=des;}
        
    board->ep = board->movehistory[board->nummoves].ep;
    board->wq = board->movehistory[board->nummoves].wq;
    board->wk = board->movehistory[board->nummoves].wk;
    board->wc = board->movehistory[board->nummoves].wc;
    board->bq = board->movehistory[board->nummoves].bq;
    board->bk = board->movehistory[board->nummoves].bk;
    board->bc = board->movehistory[board->nummoves].bc;
    board->fifty = board->movehistory[board->nummoves].fifty;
    board->hash = board->movehistory[board->nummoves].hash;
    board->square[src]=srcpiece;
    board->square[des]=cap;
    
    if (pro!=BLANK)
    {
    	if (board->turn==BLACKARMY)				// turn is reversed (last to move not next to move)
    	{
	    	board->square[src]=WHITEPAWN;
	    }
	    else
	    {
	    	board->square[src]=BLACKPAWN;
	    }
    	
    }
   	if (srcpiece==WHITEPAWN)
	{
		if (pos_rank[src]==5)
			if (pos_file[des]==board->ep)
			{
				// move was ep, put the pawn back on the board
				board->square[squareinmemory(board->ep,5)]=BLACKPAWN;
			}
	}
	else if (srcpiece==BLACKPAWN)
	{
		if (pos_rank[src]==4)
			if (pos_file[des]==board->ep)
			{
				// move was ep, put the pawn back on the board
				board->square[squareinmemory(board->ep,4)]=WHITEPAWN;
			}
	}
    else if (srcpiece==WHITEKING)
    {
    	board->wkpos = src;
    	if (src==SQR_E1)
    	{
    		if (des==SQR_G1)
    		{
    			// move was a white kingside castle, move rook back to h1
    			board->square[SQR_F1]=BLANK;
    			board->square[SQR_H1]=WHITEROOK;
    		}
    		else if (des==SQR_C1)
    		{
    			// move was a white queenside castle, move rook back to a1
    			board->square[SQR_D1]=BLANK;
    			board->square[SQR_A1]=WHITEROOK;
    			
    		}
    	}
    }
    else if (srcpiece==BLACKKING)
    {
    	board->bkpos = src;
    	if (src==SQR_E8)
    	{
    		if (des==SQR_G8)
    		{
    			// move was a black kingside castle, move rook back to h8
    			board->square[SQR_F8]=BLANK;
    			board->square[SQR_H8]=BLACKROOK;
    		}
    		else if (des==SQR_C8)
    		{
    			// move was a black queenside castle, move rook back to a8
    			board->square[SQR_D8]=BLANK;
    			board->square[SQR_A8]=BLACKROOK;
    		}
    	}
    }

	board->turn=30-board->turn;
	--board->nummoves;

    return;
}

void addmove(byte depth, byte src, byte des, byte pro)
{
	byte hashentry;
	movesavail[depth]++;
	movelist[depth][movesavail[depth]].source = src;
	movelist[depth][movesavail[depth]].destination = des;
	movelist[depth][movesavail[depth]].promotion = pro;
	movelist[depth][movesavail[depth]].eval = history[src][des];
	if (follows[src][des].source==tmpboard.movehistory[tmpboard.nummoves].source)
		if (follows[src][des].destination==tmpboard.movehistory[tmpboard.nummoves].destination)
			movelist[depth][movesavail[depth]].eval=9999000+follows[src][des].eval;
	if (tmpboard.square[des] != BLANK)
	{
		movelist[depth][movesavail[depth]].eval = 1000000+(piece(tmpboard.square[des])*10) - piece(tmpboard.square[src]);
	}
	if (tmpboard.square[src] == WHITEPAWN)
	{
		if (pos_rank[src] == 7)
		{
			movelist[depth][movesavail[depth]].promotion = WHITEQUEEN;
			movelist[depth][movesavail[depth]].eval += 1000800;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = WHITEKNIGHT;
			movelist[depth][movesavail[depth]].eval += 1000200;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = WHITEROOK;
			movelist[depth][movesavail[depth]].eval += 1000101;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = WHITEBISHOP;
			movelist[depth][movesavail[depth]].eval += 1000100;
		}
		else if (pos_rank[src] == 5)
			if (pos_file[des]==tmpboard.ep)
			{
				// move was en-passant
				movelist[depth][movesavail[depth]].eval = 1000202;
			}
	}
	else if (tmpboard.square[src] == BLACKPAWN)
	{
		if (pos_rank[src] == 2)
		{
			movelist[depth][movesavail[depth]].promotion = BLACKQUEEN;
			movelist[depth][movesavail[depth]].eval += 1000800;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = BLACKKNIGHT;
			movelist[depth][movesavail[depth]].eval += 1000200;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = BLACKROOK;
			movelist[depth][movesavail[depth]].eval += 1000101;
			movesavail[depth]++;
			movelist[depth][movesavail[depth]].source = src;
			movelist[depth][movesavail[depth]].destination = des;
			movelist[depth][movesavail[depth]].promotion = BLACKBISHOP;
			movelist[depth][movesavail[depth]].eval += 1000100;
		}
		else if (pos_rank[src] == 4)
			if (pos_file[des]==tmpboard.ep)
			{
				// move was en-passant
				movelist[depth][movesavail[depth]].eval = 1000202;
			}
	}
	return;
}

void sortpv()
{
	byte i;

	onpv = false;
	for(i=1;i<=movesavail[depth];i++)
	{
		if (pv[0][depth].source==movelist[depth][i].source)
		{
			if (pv[0][depth].destination==movelist[depth][i].destination)
			{
				if (pv[0][depth].promotion==movelist[depth][i].promotion)
				{
					onpv=true;
					movelist[depth][i].eval=10000001;
					return;
				}
			}
		}
	}
	return;
}

void sorttt()
{
	byte i;
    unsigned int curttentry;
    
    curttentry = tmpboard.hash % TT_SIZE;

	for(i=1;i<=movesavail[depth];i++)
	{
		if (transtable[curttentry].source==movelist[depth][i].source)
		{
			if (transtable[curttentry].destination==movelist[depth][i].destination)
			{
   				movelist[depth][i].eval=10000000;
  				return;
			}
		}
	}
	return;
}

void sortmoves(byte nextmove)
{
	byte i,curbest,pvfound;
	s_int curbestscore;
	struct t_move swapmove;
	curbest = nextmove;
	curbestscore = -10001;
	for (i=nextmove;i<=movesavail[depth];i++)
	{ 
		if (movelist[depth][i].eval>curbestscore)
		{
			// found new best move
			curbest = i;
			curbestscore = movelist[depth][i].eval;
		}
	}
	swapmove = movelist[depth][nextmove];
	movelist[depth][nextmove] = movelist[depth][curbest];
	movelist[depth][curbest] = swapmove;
	return;
}


byte checktest(struct t_board *board, byte turn)
{
	byte cursqr;
	byte kingpos;
	if (turn==WHITEARMY)
	{
		kingpos = board->wkpos;
		// check if there are pawns attacking the king
		if (board->square[kingpos+9]==BLACKPAWN){return true;}
		if (board->square[kingpos+11]==BLACKPAWN){return true;}
		// check if there are knights attacking the king
		if (board->square[kingpos+21]==BLACKKNIGHT){return true;}
		if (board->square[kingpos+12]==BLACKKNIGHT){return true;}
		if (board->square[kingpos-8]==BLACKKNIGHT){return true;}
		if (board->square[kingpos-19]==BLACKKNIGHT){return true;}
		if (board->square[kingpos-21]==BLACKKNIGHT){return true;}
		if (board->square[kingpos-12]==BLACKKNIGHT){return true;}
		if (board->square[kingpos+8]==BLACKKNIGHT){return true;}
		if (board->square[kingpos+19]==BLACKKNIGHT){return true;}
		// check if there is a king next to the king (used for post-move legality checking)
		if (board->square[kingpos+10]==BLACKKING){return true;}
		if (board->square[kingpos+11]==BLACKKING){return true;}
		if (board->square[kingpos+1]==BLACKKING){return true;}
		if (board->square[kingpos-9]==BLACKKING){return true;}
		if (board->square[kingpos-10]==BLACKKING){return true;}
		if (board->square[kingpos-11]==BLACKKING){return true;}
		if (board->square[kingpos-1]==BLACKKING){return true;}
		if (board->square[kingpos+9]==BLACKKING){return true;}
		// check diagonals for queens/bishops
		for (cursqr=kingpos+11;board->square[cursqr]==BLANK;cursqr=cursqr+11){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKBISHOP){return true;}
		for (cursqr=kingpos-9;board->square[cursqr]==BLANK;cursqr=cursqr-9){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKBISHOP){return true;}
		for (cursqr=kingpos-11;board->square[cursqr]==BLANK;cursqr=cursqr-11){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKBISHOP){return true;}
		for (cursqr=kingpos+9;board->square[cursqr]==BLANK;cursqr=cursqr+9){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKBISHOP){return true;}
		// check files/ranks for queens/rooks
		for (cursqr=kingpos+10;board->square[cursqr]==BLANK;cursqr=cursqr+10){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKROOK){return true;}
		for (cursqr=kingpos+1;board->square[cursqr]==BLANK;cursqr=cursqr+1){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKROOK){return true;}
		for (cursqr=kingpos-10;board->square[cursqr]==BLANK;cursqr=cursqr-10){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKROOK){return true;}
		for (cursqr=kingpos-1;board->square[cursqr]==BLANK;cursqr=cursqr-1){}if (board->square[cursqr]==BLACKQUEEN){return true;}if (board->square[cursqr]==BLACKROOK){return true;}
	}
	else if (turn==BLACKARMY)
	{
		kingpos = board->bkpos;
		// check if there are pawns attacking the king
		if (board->square[kingpos-9]==WHITEPAWN){return true;}
		if (board->square[kingpos-11]==WHITEPAWN){return true;}
		// check if there are knights attacking the king
		if (board->square[kingpos+21]==WHITEKNIGHT){return true;}
		if (board->square[kingpos+12]==WHITEKNIGHT){return true;}
		if (board->square[kingpos-8]==WHITEKNIGHT){return true;}
		if (board->square[kingpos-19]==WHITEKNIGHT){return true;}
		if (board->square[kingpos-21]==WHITEKNIGHT){return true;}
		if (board->square[kingpos-12]==WHITEKNIGHT){return true;}
		if (board->square[kingpos+8]==WHITEKNIGHT){return true;}
		if (board->square[kingpos+19]==WHITEKNIGHT){return true;}
		// check if there is a king next to the king (used for post-move legality checking)
		if (board->square[kingpos+10]==WHITEKING){return true;}
		if (board->square[kingpos+11]==WHITEKING){return true;}
		if (board->square[kingpos+1]==WHITEKING){return true;}
		if (board->square[kingpos-9]==WHITEKING){return true;}
		if (board->square[kingpos-10]==WHITEKING){return true;}
		if (board->square[kingpos-11]==WHITEKING){return true;}
		if (board->square[kingpos-1]==WHITEKING){return true;}
		if (board->square[kingpos+9]==WHITEKING){return true;}
		// check diagonals for queens/bishops
		for (cursqr=kingpos+11;board->square[cursqr]==BLANK;cursqr=cursqr+11){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEBISHOP){return true;}
		for (cursqr=kingpos-9;board->square[cursqr]==BLANK;cursqr=cursqr-9){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEBISHOP){return true;}
		for (cursqr=kingpos-11;board->square[cursqr]==BLANK;cursqr=cursqr-11){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEBISHOP){return true;}
		for (cursqr=kingpos+9;board->square[cursqr]==BLANK;cursqr=cursqr+9){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEBISHOP){return true;}
		// check files/ranks for queens/rooks
		for (cursqr=kingpos+10;board->square[cursqr]==BLANK;cursqr=cursqr+10){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEROOK){return true;}
		for (cursqr=kingpos+1;board->square[cursqr]==BLANK;cursqr=cursqr+1){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEROOK){return true;}
		for (cursqr=kingpos-10;board->square[cursqr]==BLANK;cursqr=cursqr-10){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEROOK){return true;}
		for (cursqr=kingpos-1;board->square[cursqr]==BLANK;cursqr=cursqr-1){}if (board->square[cursqr]==WHITEQUEEN){return true;}if (board->square[cursqr]==WHITEROOK){return true;}
	}
	return false;
}

byte validmove(struct t_board *board, struct t_move *move)
{
	byte src,des,srcpiece;
	src = move->source;
	des = move->destination;
	srcpiece = board->square[src];
	if (srcpiece==WHITEKING)
	{
		if (src==SQR_E1)
		{	
			if (des==SQR_G1)
			{
				if (!board->wk){return false;}
				if (checktest(board,WHITEARMY)){return false;}
				board->wkpos = SQR_F1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_G1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_H1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_E1;
			}
			else if (des==SQR_C1)
			{
				if (!board->wq){return false;}
				if (checktest(board,WHITEARMY)){return false;}
				board->wkpos = SQR_D1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_C1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_A1;if (checktest(board,WHITEARMY)){board->wkpos=SQR_E1;return false;}
				board->wkpos = SQR_E1;
			}
		}
	}
	else if (srcpiece==BLACKKING)
	{
		if (src==SQR_E8)
		{	
			if (des==SQR_G8)
			{
				if (!board->bk){return false;}
				if (checktest(board,BLACKARMY)){return false;}
				board->bkpos = SQR_F8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_G8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_H8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_E8;
			}
			else if (des==SQR_C8)
			{
				if (!board->bq){return false;}
				if (checktest(board,BLACKARMY)){return false;}
				board->bkpos = SQR_D8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_C8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_A8;if (checktest(board,BLACKARMY)){board->bkpos=SQR_E8;return false;}
				board->bkpos = SQR_E8;
			}
		}
	}
	
	return true;
}

byte repeattest(struct t_board *board)
{
	byte repeats,mov;
	// check for three time repeat
	repeats = 0;
	for (mov=board->nummoves-board->fifty;mov<board->nummoves;++mov)
	{
		if (board->movehistory[mov].hash==board->hash)
		{
			repeats++;
		}
	}
	return repeats;
}

byte draw(struct t_board *board)
{
	if (board->fifty>=100){printf("1/2-1/2 {Draw by 50 Move Rule}\n");return true;}
	else if (repeattest(board)>1){printf("1/2-1/2 {Draw by repetition}\n");return true;}
	return false;
}

