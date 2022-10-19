// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

s_int evaluateboard(struct t_board *board)                      // return board evaluation for current player from current players view
{
	byte cursqr,cursqrpiece;
	byte wpawns,bpawns;
	byte wbishops,bbishops;
	byte wknights,bknights;
	byte wrooks,brooks;
	byte wqueens,bqueens;
	byte wmatemat,bmatemat;
	byte numpiecesw,numpiecesb;
	byte wfile[10],bfile[10],passed,isolated;
	s_int wmaterial,bmaterial;
	s_int wposition,bposition;
	s_int wtotal,btotal;
	s_int total;
	s_int *wpawn_psq,*bpawn_psq;			// piece square pointers, depending on castling state
	s_int *wqueen_psq,*bqueen_psq;			// piece square pointers, depending on castling state
	s_int disx,disy;
	
	if (!board->wkpos)
	{
		if (board->turn==WHITEARMY){return KINGOFFBOARD+depth;}		// todo: change to use kingoffboard + depth
		else if (board->turn==BLACKARMY){return WIN_CHECKMATE;}
	}
	else if (!board->bkpos)
	{
		if (board->turn==BLACKARMY){return KINGOFFBOARD+depth;}		// todo: change to use kingoffboard + depth
		else if (board->turn==WHITEARMY){return WIN_CHECKMATE;}
	}
	
	wpawns=0;bpawns=0;
	wbishops=0;bbishops=0;
	wknights=0;bknights=0;
	wrooks=0;brooks=0;
	wqueens=0;bqueens=0;
	wmatemat=0;bmatemat=0;
	wmaterial=0;bmaterial=0;
	wposition=0;bposition=0;
	numpiecesw=0;numpiecesb=0;
	disx=0;disy=0;
   	memset(wfile, 0, sizeof(wfile));
   	memset(bfile, 0, sizeof(bfile));
	// find out which piece square to use for the pawns depending on castle state
	wpawn_psq=pawn;
	bpawn_psq=pawn;
	wqueen_psq=queen_opening;
	bqueen_psq=queen_opening;
	if (!board->wc){wposition+=NO_CASTLE;bqueen_psq=queen_opening;}
	else if (board->wc==1){wpawn_psq=pawn_kingside;bqueen_psq=queen_kingside;}
	else if (board->wc==2){wpawn_psq=pawn_queenside;bqueen_psq=queen_queenside;}
	if (!board->bc){bposition+=NO_CASTLE;wqueen_psq=queen_opening;}
	else if (board->bc==1){bpawn_psq=pawn_kingside;wqueen_psq=queen_kingside;}
	else if (board->bc==2){bpawn_psq=pawn_queenside;wqueen_psq=queen_queenside;}
	
	for (cursqr=SQR_A1;cursqr<=SQR_H8;cursqr++)
	{
		cursqrpiece = board->square[cursqr];
		if (cursqrpiece<2){continue;}
		if (army(cursqrpiece)==WHITEARMY){wmaterial+=piecevalues[cursqrpiece];numpiecesw++;}
		else if (army(cursqrpiece)==BLACKARMY){bmaterial+=piecevalues[cursqrpiece];numpiecesb++;}
		if (cursqrpiece==WHITEPAWN)
		{
			wpawns++;
			wmatemat++;
			wfile[pos_file[cursqr]]++;
		}
		else if (cursqrpiece==BLACKPAWN)
		{
			bpawns++;
			bmatemat++;
			bfile[pos_file[cursqr]]++;
		}
		else if (cursqrpiece==WHITEBISHOP)
		{
			wbishops++;
			wposition+=bishop[cursqr];
		}
		else if (cursqrpiece==BLACKBISHOP)
		{
			bbishops++;
			bposition+=bishop[flip[cursqr]];
		}
		else if (cursqrpiece==WHITEKNIGHT)
		{
			wknights++;
			wposition+=knight[cursqr];
		}
		else if (cursqrpiece==BLACKKNIGHT)
		{
			bknights++;
			bposition+=knight[flip[cursqr]];
		}
		else if (cursqrpiece==WHITEROOK)
		{
			wrooks++;
			wmatemat++;
		}
		else if (cursqrpiece==BLACKROOK)
		{
			brooks++;
			bmatemat++;
		}
		else if (cursqrpiece==WHITEQUEEN)
		{
			wqueens++;
			wmatemat++;
			wposition+=wqueen_psq[cursqr];
		}
		else if (cursqrpiece==BLACKQUEEN)
		{
			bqueens++;
			bmatemat++;
			bposition+=bqueen_psq[flip[cursqr]];
		}
	}

	// evaluate pawns
	for (cursqr=SQR_A1;cursqr<=SQR_H8;cursqr++)
	{
		cursqrpiece = board->square[cursqr];
		if (cursqrpiece<2){continue;}
		if (cursqrpiece==WHITEPAWN)
		{	
			wposition+=wpawn_psq[cursqr];
			if ((board->square[cursqr+9]==WHITEPAWN)&&(board->square[cursqr+11]==WHITEPAWN)){wposition+=PAWN_BACKWARD;}
			if (board->square[cursqr+10]!=BLANK){wposition+=PAWN_BLOCKED;}
			if (wfile[pos_file[cursqr]]>1){wposition+=PAWN_DOUBLED;}
			passed=true;isolated=true;
			if ((pos_file[cursqr]>1)&&(wfile[pos_file[cursqr]-1])){isolated=false;}
			else if ((pos_file[cursqr]<8)&&(wfile[pos_file[cursqr]+1])){isolated=false;}
			if (bfile[pos_file[cursqr]]){passed=false;}
			else if ((pos_file[cursqr]>1)&&(bfile[pos_file[cursqr]-1]))
			{
				// is it in front?
				for (disx=cursqr+9;board->square[disx]!=OUTOFBOUNDS;disx+=10)
				{
					if (board->square[disx]==BLACKPAWN){passed=false;break;}
				}
			}
			else if ((pos_file[cursqr]<8)&&(bfile[pos_file[cursqr]+1]))
			{
				// is it in front?
				for (disx=cursqr+11;board->square[disx]!=OUTOFBOUNDS;disx+=10)
				{
					if (board->square[disx]==BLACKPAWN){passed=false;break;}
				}
			}
			if (passed)
			{
				wposition+=pawn_passed[cursqr];wposition+=pawn_passed_kingdistance[squareinmemory(pos_file[cursqr],pos_file[board->bkpos])];
				for (disx=cursqr-10;board->square[disx]==BLANK;disx-=10){}
				if (board->square[disx]==WHITEROOK){wposition+=PAWN_PASSEDWITHROOK;}
				else if (board->square[disx]==BLACKROOK){wposition-=PAWN_PASSEDWITHROOK;}
			}
			else if (isolated){wposition+=PAWN_ISOLATED;}
		}
		else if (cursqrpiece==BLACKPAWN)
		{
			bposition+=bpawn_psq[flip[cursqr]];
			if ((board->square[cursqr-9]==BLACKPAWN)&&(board->square[cursqr-11]==BLACKPAWN)){bposition+=PAWN_BACKWARD;}
			if (board->square[cursqr-10]!=BLANK){bposition+=PAWN_BLOCKED;}
			if (bfile[pos_file[cursqr]]>1){bposition+=PAWN_DOUBLED;}
			passed=true;isolated=true;
			if ((pos_file[cursqr]>1)&&(bfile[pos_file[cursqr]-1])){isolated=false;}
			else if ((pos_file[cursqr]<8)&&(bfile[pos_file[cursqr]+1])){isolated=false;}
			if (wfile[pos_file[cursqr]]){passed=false;}
			else if ((pos_file[cursqr]>1)&&(wfile[pos_file[cursqr]-1]))
			{
				// is it in front?
				for (disx=cursqr+9;board->square[disx]!=OUTOFBOUNDS;disx+=10)
				{
					if (board->square[disx]==WHITEPAWN){passed=false;break;}
				}
			}
			else if ((pos_file[cursqr]<8)&&(wfile[pos_file[cursqr]+1]))
			{
				// is it in front?
				for (disx=cursqr+11;board->square[disx]!=OUTOFBOUNDS;disx+=10)
				{
					if (board->square[disx]==WHITEPAWN){passed=false;break;}
				}
			}
			if (passed)
			{
				bposition+=pawn_passed[flip[cursqr]];bposition+=pawn_passed_kingdistance[squareinmemory(pos_file[cursqr],pos_file[board->wkpos])];
				for (disx=cursqr+10;board->square[disx]==BLANK;disx+=10){}
				if (board->square[disx]==BLACKROOK){bposition+=PAWN_PASSEDWITHROOK;}
				else if (board->square[disx]==WHITEROOK){bposition-=PAWN_PASSEDWITHROOK;}
			}
			else if (isolated){bposition+=PAWN_ISOLATED;}
		}
		else if (cursqrpiece==WHITEROOK)
		{
			if (pos_rank[cursqr]>6){wposition+=ROOK_ADVANCED;}
			if (!wfile[pos_file[cursqr]])
			{
				if (!bfile[pos_file[cursqr]]){wposition+=ROOK_OPENFILE;}
				else {wposition+=ROOK_SEMIOPENFILE;}
			}
		}
		else if (cursqrpiece==BLACKROOK)
		{
			if (pos_rank[cursqr]<3){bposition+=ROOK_ADVANCED;}
			if (!bfile[pos_file[cursqr]])
			{
				if (!wfile[pos_file[cursqr]]){bposition+=ROOK_OPENFILE;}
				else {bposition+=ROOK_SEMIOPENFILE;}
			}
		}
	}
	// other material factors
	if (wbishops>1){wmatemat++;}
	if (bbishops>1){bmatemat++;}
	if ((!wmatemat)&&(!bmatemat)){return 0;}
	if (!wmatemat){wmaterial+=NOMATE_MATERIAL;}
	if (!bmatemat){bmaterial+=NOMATE_MATERIAL;}
	if (bmaterial<=1300)
	{
		wposition+=king_endgame[board->wkpos];
		if (numpiecesb==1)
		{
			bposition+=king_lone[board->bkpos];
			disx = pos_file[board->wkpos] - pos_file[board->bkpos];
			disy = pos_rank[board->wkpos] - pos_rank[board->bkpos];
			if (disx<0){disx=0-disx;}
			if (disy<0){disy=0-disy;}
			disx = 10 - disx;
			disy = 10 - disy;
			wposition+=(disx*disy);
		}
		else
		{
			disx=0;
			if (board->square[board->wkpos+9]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos+10]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos+11]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos-1]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos+1]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos-11]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos-10]==WHITEPAWN){disx++;}
			if (board->square[board->wkpos-9]==WHITEPAWN){disx++;}
			if (disx>3){disx=1;} // anymore than three is bad, it means doubled pawns and stuff
			wposition+=king_pawns[disx];
		}
	}
	else
	{
		wposition+=king[board->wkpos];
		if (army(board->square[board->wkpos+9])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos+10])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos+11])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos-1])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos+1])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos-11])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos-10])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->wkpos-9])==BLACKARMY){wposition+=KING_ENEMYNEXT;}
		disx=0;
		if (board->square[board->wkpos+9]==WHITEPAWN){disx++;}
		if (board->square[board->wkpos+10]==WHITEPAWN){disx++;}
		if (board->square[board->wkpos+11]==WHITEPAWN){disx++;}
		if (board->square[board->wkpos-1]==WHITEPAWN){disx++;}
		if (board->square[board->wkpos+1]==WHITEPAWN){disx++;}
		if (disx>3){disx=1;} // anymore than three is bad, it means doubled pawns and stuff
		wposition+=king_pawns[disx];
		for (disx=SQR_A1;disx<=SQR_H1;disx++)
		{
			if (board->square[disx]==WHITEBISHOP){wposition+=MINOR_UNDEVELOPED;}
			else if (board->square[disx]==WHITEKNIGHT){wposition+=MINOR_UNDEVELOPED;}
		}
	}
	if (wmaterial<=1300)
	{
		bposition+=king_endgame[board->bkpos];
		if (numpiecesw==1)
		{
			wposition+=king_lone[board->wkpos];
			disx = pos_file[board->wkpos] - pos_file[board->bkpos];
			disy = pos_rank[board->wkpos] - pos_rank[board->bkpos];
			if (disx<0){disx=0-disx;}
			if (disy<0){disy=0-disy;}
			disx = 10 - disx;
			disy = 10 - disy;
			bposition+=(disx*disy);
		}
		else
		{
			disx=0;
			if (board->square[board->bkpos+9]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos+10]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos+11]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos-1]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos+1]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos-11]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos-10]==BLACKPAWN){disx++;}
			if (board->square[board->bkpos-9]==BLACKPAWN){disx++;}
			if (disx>3){disx=1;} // anymore than three is bad, it means doubled pawns and stuff
			bposition+=king_pawns[disx];
		}
	}
	else
	{
		bposition+=king[flip[board->bkpos]];
		if (army(board->square[board->bkpos+9])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos+10])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos+11])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos-1])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos+1])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos-11])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos-10])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		if (army(board->square[board->bkpos-9])==WHITEARMY){bposition+=KING_ENEMYNEXT;}
		disx=0;
		if (board->square[board->bkpos-1]==BLACKPAWN){disx++;}
		if (board->square[board->bkpos+1]==BLACKPAWN){disx++;}
		if (board->square[board->bkpos-11]==BLACKPAWN){disx++;}
		if (board->square[board->bkpos-10]==BLACKPAWN){disx++;}
		if (board->square[board->bkpos-9]==BLACKPAWN){disx++;}
		if (disx>3){disx=1;} // anymore than three is bad, it means doubled pawns and stuff
		bposition+=king_pawns[disx];
		for (disx=SQR_A8;disx<=SQR_H8;disx++)
		{
			if (board->square[disx]==BLACKBISHOP){bposition+=MINOR_UNDEVELOPED;}
			else if (board->square[disx]==BLACKKNIGHT){bposition+=MINOR_UNDEVELOPED;}
		}
	}

	if (wbishops>1){wmaterial+=BISHOP_BOTHBONUS;}
	if (bbishops>1){bmaterial+=BISHOP_BOTHBONUS;}
	wmaterial+=(wknights*wpawns)*KNIGHT_EACHPAWN;
	bmaterial+=(bknights*bpawns)*KNIGHT_EACHPAWN;
	wmaterial+=(wrooks*wpawns)*ROOK_EACHPAWN;
	bmaterial+=(brooks*bpawns)*ROOK_EACHPAWN;
	wmaterial+=(wqueens*bpawns)*QUEEN_EACHOPPPAWN;
	bmaterial+=(bqueens*wpawns)*QUEEN_EACHOPPPAWN;

	wtotal=wmaterial+wposition;
	btotal=bmaterial+bposition;

	total=wtotal-btotal;
	if (knowledge){total+=(board->hash % (knowledge*2)) - knowledge;}

	if (board->turn==BLACKARMY)
	{
		total=-total;
	}
	if (board->fifty>=70){total=total/4;}
	else if (board->fifty>=80){total=total/5;}
	else if (board->fifty>=90){total=total/10;}
	if (total==0){total=1;}
    return total;
}
