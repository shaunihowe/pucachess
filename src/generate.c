// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

void generatecaptures()
{
	byte src,des,srcpiece;
	for (src=SQR_A1;src<=SQR_H8;src++)
	{
		srcpiece=tmpboard.square[src];
		if (srcpiece!=OUTOFBOUNDS)
		{
			if (srcpiece!=BLANK)
			{
				if (army(srcpiece)==tmpboard.turn)
				{
					if (srcpiece==WHITEPAWN)
					{
						if (army(tmpboard.square[src+11])==BLACKARMY){addmove(depth,src,src+11,0);}
						if (army(tmpboard.square[src+9])==BLACKARMY){addmove(depth,src,src+9,0);}
						if (src>SQR_H6){if (tmpboard.square[src+10]==BLANK){addmove(depth,src,src+10,0);}}
						if ((pos_rank[src]==5)&&(tmpboard.ep!=0))
						{
							if (pos_file[src]+1==tmpboard.ep){addmove(depth,src,squareinmemory(tmpboard.ep,6),0);}
							else if (pos_file[src]-1==tmpboard.ep){addmove(depth,src,squareinmemory(tmpboard.ep,6),0);}
						}
					}
					else if (srcpiece==BLACKPAWN)
					{						
						if (army(tmpboard.square[src-11])==WHITEARMY){addmove(depth,src,src-11,0);}
						if (army(tmpboard.square[src-9])==WHITEARMY){addmove(depth,src,src-9,0);}
						if (src<SQR_A3){if (tmpboard.square[src-10]==BLANK){addmove(depth,src,src-10,0);}}
						if ((pos_rank[src]==4)&&(tmpboard.ep!=0))
						{
							if (pos_file[src]+1==tmpboard.ep){addmove(depth,src,squareinmemory(tmpboard.ep,3),0);}
							else if (pos_file[src]-1==tmpboard.ep){addmove(depth,src,squareinmemory(tmpboard.ep,3),0);}
						}
					}
					else if (srcpiece==WHITEKNIGHT)
					{
						if (army(tmpboard.square[src+8])==BLACKARMY){addmove(depth,src,src+8,0);}
						if (army(tmpboard.square[src+19])==BLACKARMY){addmove(depth,src,src+19,0);}
						if (army(tmpboard.square[src+21])==BLACKARMY){addmove(depth,src,src+21,0);}
						if (army(tmpboard.square[src+12])==BLACKARMY){addmove(depth,src,src+12,0);}
						if (army(tmpboard.square[src-8])==BLACKARMY){addmove(depth,src,src-8,0);}
						if (army(tmpboard.square[src-19])==BLACKARMY){addmove(depth,src,src-19,0);}
						if (army(tmpboard.square[src-21])==BLACKARMY){addmove(depth,src,src-21,0);}
						if (army(tmpboard.square[src-12])==BLACKARMY){addmove(depth,src,src-12,0);}
					}
					else if (srcpiece==BLACKKNIGHT)
					{
						if (army(tmpboard.square[src+8])==WHITEARMY){addmove(depth,src,src+8,0);}
						if (army(tmpboard.square[src+19])==WHITEARMY){addmove(depth,src,src+19,0);}
						if (army(tmpboard.square[src+21])==WHITEARMY){addmove(depth,src,src+21,0);}
						if (army(tmpboard.square[src+12])==WHITEARMY){addmove(depth,src,src+12,0);}
						if (army(tmpboard.square[src-8])==WHITEARMY){addmove(depth,src,src-8,0);}
						if (army(tmpboard.square[src-19])==WHITEARMY){addmove(depth,src,src-19,0);}
						if (army(tmpboard.square[src-21])==WHITEARMY){addmove(depth,src,src-21,0);}
						if (army(tmpboard.square[src-12])==WHITEARMY){addmove(depth,src,src-12,0);}
					}
					else if (srcpiece==WHITEBISHOP)
					{
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==BLACKBISHOP)
					{
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==WHITEROOK)
					{
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==BLACKROOK)
					{
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==WHITEQUEEN)
					{
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){}if (army(tmpboard.square[des])==BLACKARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==BLACKQUEEN)
					{
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){}if (army(tmpboard.square[des])==WHITEARMY){addmove(depth,src,des,0);}
					}
					else if (srcpiece==WHITEKING)
					{
						if (army(tmpboard.square[src+9])==BLACKARMY){addmove(depth,src,src+9,0);}
						if (army(tmpboard.square[src+10])==BLACKARMY){addmove(depth,src,src+10,0);}
						if (army(tmpboard.square[src+11])==BLACKARMY){addmove(depth,src,src+11,0);}
						if (army(tmpboard.square[src-1])==BLACKARMY){addmove(depth,src,src-1,0);}
						if (army(tmpboard.square[src+1])==BLACKARMY){addmove(depth,src,src+1,0);}
						if (army(tmpboard.square[src-11])==BLACKARMY){addmove(depth,src,src-11,0);}
						if (army(tmpboard.square[src-10])==BLACKARMY){addmove(depth,src,src-10,0);}
						if (army(tmpboard.square[src-9])==BLACKARMY){addmove(depth,src,src-9,0);}
					}
					else if (srcpiece==BLACKKING)
					{
						if (army(tmpboard.square[src+9])==WHITEARMY){addmove(depth,src,src+9,0);}
						if (army(tmpboard.square[src+10])==WHITEARMY){addmove(depth,src,src+10,0);}
						if (army(tmpboard.square[src+11])==WHITEARMY){addmove(depth,src,src+11,0);}
						if (army(tmpboard.square[src-1])==WHITEARMY){addmove(depth,src,src-1,0);}
						if (army(tmpboard.square[src+1])==WHITEARMY){addmove(depth,src,src+1,0);}
						if (army(tmpboard.square[src-11])==WHITEARMY){addmove(depth,src,src-11,0);}
						if (army(tmpboard.square[src-10])==WHITEARMY){addmove(depth,src,src-10,0);}
						if (army(tmpboard.square[src-9])==WHITEARMY){addmove(depth,src,src-9,0);}
					}
				}
			}
		}
		else
		{
			src++;
		}
	}
	return;
}

void generatemoves()
{
	byte src,des,srcpiece;
	byte tkpos;
	for (src=SQR_A1;src<=SQR_H8;src++)
	{
		srcpiece=tmpboard.square[src];
		if (srcpiece!=OUTOFBOUNDS)
		{
			if (srcpiece!=BLANK)
			{
				if (army(srcpiece)==tmpboard.turn)
				{
					if (srcpiece==WHITEPAWN)
					{
						if (tmpboard.square[src+10]==BLANK)
						{
							if (src<SQR_A7){addmove(depth,src,src+10,BLANK);}
							if (src<SQR_A3)
							{
								if (tmpboard.square[src+20]==BLANK){addmove(depth,src,src+20,BLANK);}
							}
						}
					}
					else if (srcpiece==BLACKPAWN)
					{
						if (tmpboard.square[src-10]==BLANK)
						{
							if (src>SQR_H2){addmove(depth,src,src-10,BLANK);}
							if (src>SQR_H6)
							{
								if (tmpboard.square[src-20]==BLANK){addmove(depth,src,src-20,BLANK);}
							}
						}
					}
					else if (srcpiece==WHITEKNIGHT)
					{
						if (tmpboard.square[src+8]==BLANK){addmove(depth,src,src+8,0);}
						if (tmpboard.square[src+19]==BLANK){addmove(depth,src,src+19,0);}
						if (tmpboard.square[src+21]==BLANK){addmove(depth,src,src+21,0);}
						if (tmpboard.square[src+12]==BLANK){addmove(depth,src,src+12,0);}
						if (tmpboard.square[src-8]==BLANK){addmove(depth,src,src-8,0);}
						if (tmpboard.square[src-19]==BLANK){addmove(depth,src,src-19,0);}
						if (tmpboard.square[src-21]==BLANK){addmove(depth,src,src-21,0);}
						if (tmpboard.square[src-12]==BLANK){addmove(depth,src,src-12,0);}
					}
					else if (srcpiece==BLACKKNIGHT)
					{
						if (tmpboard.square[src+8]==BLANK){addmove(depth,src,src+8,0);}
						if (tmpboard.square[src+19]==BLANK){addmove(depth,src,src+19,0);}
						if (tmpboard.square[src+21]==BLANK){addmove(depth,src,src+21,0);}
						if (tmpboard.square[src+12]==BLANK){addmove(depth,src,src+12,0);}
						if (tmpboard.square[src-8]==BLANK){addmove(depth,src,src-8,0);}
						if (tmpboard.square[src-19]==BLANK){addmove(depth,src,src-19,0);}
						if (tmpboard.square[src-21]==BLANK){addmove(depth,src,src-21,0);}
						if (tmpboard.square[src-12]==BLANK){addmove(depth,src,src-12,0);}
					}
					else if (srcpiece==WHITEBISHOP)
					{
						// up+right
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){addmove(depth,src,des,0);} // add sliding moves
						// down+right
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){addmove(depth,src,des,0);} // add sliding moves
						// down+left
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){addmove(depth,src,des,0);} // add sliding moves
						// up+left
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==BLACKBISHOP)
					{
						// up+right
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){addmove(depth,src,des,0);} // add sliding moves
						// down+right
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){addmove(depth,src,des,0);} // add sliding moves
						// down+left
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){addmove(depth,src,des,0);} // add sliding moves
						// up+left
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==WHITEROOK)
					{
						// up
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){addmove(depth,src,des,0);} // add sliding moves
						// right
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){addmove(depth,src,des,0);} // add sliding moves
						// down
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){addmove(depth,src,des,0);} // add sliding moves
						// left
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==BLACKROOK)
					{
						// up
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){addmove(depth,src,des,0);} // add sliding moves
						// right
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){addmove(depth,src,des,0);} // add sliding moves
						// down
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){addmove(depth,src,des,0);} // add sliding moves
						// left
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==WHITEQUEEN)
					{
						// up+right
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){addmove(depth,src,des,0);} // add sliding moves
						// down+right
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){addmove(depth,src,des,0);} // add sliding moves
						// down+left
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){addmove(depth,src,des,0);} // add sliding moves
						// up+left
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){addmove(depth,src,des,0);} // add sliding moves
						// up
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){addmove(depth,src,des,0);} // add sliding moves
						// right
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){addmove(depth,src,des,0);} // add sliding moves
						// down
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){addmove(depth,src,des,0);} // add sliding moves
						// left
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==BLACKQUEEN)
					{
						// up+right
						for (des=src+11;tmpboard.square[des]==BLANK;des=des+11){addmove(depth,src,des,0);} // add sliding moves
						// down+right
						for (des=src-9;tmpboard.square[des]==BLANK;des=des-9){addmove(depth,src,des,0);} // add sliding moves
						// down+left
						for (des=src-11;tmpboard.square[des]==BLANK;des=des-11){addmove(depth,src,des,0);} // add sliding moves
						// up+left
						for (des=src+9;tmpboard.square[des]==BLANK;des=des+9){addmove(depth,src,des,0);} // add sliding moves
						// up
						for (des=src+10;tmpboard.square[des]==BLANK;des=des+10){addmove(depth,src,des,0);} // add sliding moves
						// right
						for (des=src+1;tmpboard.square[des]==BLANK;des=des+1){addmove(depth,src,des,0);} // add sliding moves
						// down
						for (des=src-10;tmpboard.square[des]==BLANK;des=des-10){addmove(depth,src,des,0);} // add sliding moves
						// left
						for (des=src-1;tmpboard.square[des]==BLANK;des=des-1){addmove(depth,src,des,0);} // add sliding moves
					}
					else if (srcpiece==WHITEKING)
					{
						if (tmpboard.square[src+9]==BLANK){addmove(depth,src,src+9,0);}
						if (tmpboard.square[src+10]==BLANK){addmove(depth,src,src+10,0);}
						if (tmpboard.square[src+11]==BLANK){addmove(depth,src,src+11,0);}
						if (tmpboard.square[src-1]==BLANK){addmove(depth,src,src-1,0);}
						if (tmpboard.square[src+1]==BLANK){addmove(depth,src,src+1,0);}
						if (tmpboard.square[src-11]==BLANK){addmove(depth,src,src-11,0);}
						if (tmpboard.square[src-10]==BLANK){addmove(depth,src,src-10,0);}
						if (tmpboard.square[src-9]==BLANK){addmove(depth,src,src-9,0);}
						if (src==SQR_E1)
						{
							if (tmpboard.square[SQR_F1]==BLANK)
								if (tmpboard.square[SQR_G1]==BLANK)
									if (tmpboard.square[SQR_H1]==WHITEROOK)
										addmove(depth,SQR_E1,SQR_G1,0);
							if (tmpboard.square[SQR_D1]==BLANK)
								if (tmpboard.square[SQR_C1]==BLANK)
									if (tmpboard.square[SQR_B1]==BLANK)
										if (tmpboard.square[SQR_A1]==WHITEROOK)
											addmove(depth,SQR_E1,SQR_C1,0);
						}
					}
					else if (srcpiece==BLACKKING)
					{
						if (tmpboard.square[src+9]==BLANK){addmove(depth,src,src+9,0);}
						if (tmpboard.square[src+10]==BLANK){addmove(depth,src,src+10,0);}
						if (tmpboard.square[src+11]==BLANK){addmove(depth,src,src+11,0);}
						if (tmpboard.square[src-1]==BLANK){addmove(depth,src,src-1,0);}
						if (tmpboard.square[src+1]==BLANK){addmove(depth,src,src+1,0);}
						if (tmpboard.square[src-11]==BLANK){addmove(depth,src,src-11,0);}
						if (tmpboard.square[src-10]==BLANK){addmove(depth,src,src-10,0);}
						if (tmpboard.square[src-9]==BLANK){addmove(depth,src,src-9,0);}
						if (src==SQR_E8)
						{
							if (tmpboard.square[SQR_F8]==BLANK)
								if (tmpboard.square[SQR_G8]==BLANK)
									if (tmpboard.square[SQR_H8]==BLACKROOK)
										addmove(depth,SQR_E8,SQR_G8,0);
							if (tmpboard.square[SQR_D8]==BLANK)
								if (tmpboard.square[SQR_C8]==BLANK)
									if (tmpboard.square[SQR_B8]==BLANK)
										if (tmpboard.square[SQR_A8]==BLACKROOK)
											addmove(depth,SQR_E8,SQR_C8,0);
						}
					}
				}
			}
		}
		else
		{
			src++;
		}
	}
	return;
}
/*
	 +19     +21
  +8  +9 +10 +11 +12
      -1      +1
 -12 -11 -10  -9  -8
     -21     -19
*/

