// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

//void xboardcommandloop;                     // main engine loop
void xboardcommandloop()
{
    do
    {
        fflush(stdout);
        if (!fgets(param, 256, stdin)){return;}
		if (param[0] == '\n')
			continue;
        sscanf(param, "%s", command);
        if (!strcmp(command, "xboard"))
            xboard_command();
        else if (!strcmp(command, "new"))
            new_command();
        else if (!strcmp(command, "setboard"))
            setboard_command(&rootboard);
        else if (!strcmp(command, "post"))
            post_command();
        else if (!strcmp(command, "nopost"))
            nopost_command();
        else if (!strcmp(command, "level"))
            level_command();
        else if (!strcmp(command, "time"))
            time_command();
        else if (!strcmp(command, "otim"))
            otim_command();
        else if (!strcmp(command, "force"))
            force_command();
        else if (!strcmp(command, "easy"))
            easy_command();
        else if (!strcmp(command, "hard"))
            hard_command();
        else if (!strcmp(command, "draw"))
        	draw_command();
        else if (!strcmp(command, "result"))
        	result_command();
        else if (!strcmp(command, "go"))
            go_command();
        else if (!strcmp(command, "analyze"))
            analyze_command();
        else if (!strcmp(command, "."))
            dot_command();
        else if (!strcmp(command, "exit"))
            exit_command();
        else if (!strcmp(command, "playother"))
            playother_command();
        else if (!strcmp(command, "undo"))
            undo_command();
        else if (!strcmp(command, "remove"))
            remove_command();
        else if (!strcmp(command, "show"))
            show_command();
        else if (!strcmp(command, "quit"))
        {
            quit_command();
        }
        else if (!strcmp(command, "usermove"))
        {
            sscanf(param, "usermove %s", &param);
            usermove_command(param);
        }
        else if (!(param[0] < 'a' || param[0] > 'h' || param[1] < '0' || param[1] > '8' || param[2] < 'a' || param[2] > 'h' || param[3] < '0' || param[3] > '8'))
            usermove_command(param);
        modecheck();
    } while (!programclose);
    return;
}

//void xboard_command;                        // 'xboard' tells puca that GUI is xboard compatible
void xboard_command()
{
    printf("feature done=0\n");             // tell xboard to wait for all feature tags
    fflush(stdout);
    printf("feature myname=\"Puca v%i.%i.%i\"\n",VERSION_MAJOR,VERSION_MINOR,VERSION_REVISION);
    printf("feature playother=1\n");
    //printf("feature usermove=1\n");
    printf("feature time=1\n");
    printf("feature draw=1\n");
    printf("feature colors=0\n");
    printf("feature setboard=1\n");
    printf("feature analyze=1\n");
    printf("feature sigint=0\n");
    printf("feature sigterm=0\n");
    printf("feature reuse=1\n");
    printf("feature done=1\n");             // tell xboard all feature tags are done
    fflush(stdout);
    mode=MODE_FORCE;
    showthinking=false;                     // set output display to off as default, let xboard ask for it
    new_command();
    return;
}

//void new_command;                           // set engine to basic values
void new_command()
{
    if (thread_state!=THREAD_IDLE){stop_thread();}
    resetboard(&rootboard);           // reset the board
    rootboard.hash = hash_set(&rootboard);
    hash_flush();
    // reset clocks
    clock_time = 30000;
    clock_otim = 30000;
    enginecolour = BLACKARMY;   // set engine to play as black
    ponder=false;
	mode=MODE_GO;
	drawoffer=false;
    return;
}

//void setboard_command;                      // set board from fenstring
void setboard_command()
{
    char turn[2],castle[8],ep[2];
    if (thread_state!=THREAD_IDLE){stop_thread();}
    mode=MODE_FORCE;
    //setboard "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    //setboard "3r1bk1/ppqr1npp/4np2/2pRpN2/P3P3/BP4P1/2NR1P1P/3Q2K1 w - - 0 1"
    sscanf(param, "setboard %s %s %s %s", &fenstring,&turn,&castle,&ep);
    strcat(fenstring," ");
    strcat(fenstring,turn);
    strcat(fenstring," ");
    strcat(fenstring,castle);
    strcat(fenstring," ");
    strcat(fenstring,ep);
   	memset(rootboard.square, 0, sizeof(rootboard.square));
   	memset(rootboard.movehistory, 0, sizeof(rootboard.movehistory));
   	memset(movelist, 0, sizeof(movelist));
    setboard(&rootboard);
    rootboard.hash = hash_set(&rootboard);
    hash_flush();
    show_command();
    return;
}

//void post_command();                        // show thinking output
void post_command()
{
    showthinking=true;
    return;
}

//void nopost_command();                      // hide thinking output
void nopost_command()
{
    showthinking=false;
    return;
}

//void level_command();						// set game time controls
void level_command()						// set game time controls
{
	int m,t,i;
	sscanf(param, "level %i %i %i", &m,&t,&i);
	//printf("Level understood %i %i %i\n",m,t,i);
	clock_moves = m;
	clock_gametime = t * 60;
	clock_inc = i;
	return;
}

//void time_command();                        // set engine's clock
void time_command()
{
    s_int tmp;
    tmp=0;
    sscanf(param, "time %d", &tmp);
    clock_time=tmp;
    return;
}

//void otim_command;                          // set opponent's clock
void otim_command()
{
    s_int tmp;
    tmp=0;
    sscanf(param, "otim %d", &tmp);
    clock_otim=tmp;
    return;
}

// force_command();                             // set to passive mode
void force_command()
{
    mode = MODE_FORCE;
    enginecolour = 0;                       // engine doesn't play as either colour
    if (thread_state!=THREAD_IDLE){stop_thread();}
    return;
}

//void easy_command();						// set ponder mode off
void easy_command()
{
	ponder=false;
	return;
}

//void hard_command();						// set ponder mode on
void hard_command()
{
	ponder=true;		// disabled for the time being
	return;
}

//void draw_command();						// engine has been offered a draw
void draw_command()
{
	drawoffer=true;
	return;
}

//void result_command();						// the game has ended
void result_command()
{
	force_command();
	return;
}

// go_command();                             // set to active mode
void go_command()
{
    if (thread_state!=THREAD_IDLE){stop_thread();}
    mode=MODE_GO;
    enginecolour = rootboard.turn;                       // set engine to play as current turn
    drawoffer=false;
    return;
}

// analyze_command();                             // analyze position
void analyze_command()
{
	mode = MODE_FORCE;
    if (thread_state!=THREAD_IDLE){stop_thread();}
    mode = MODE_ANALYZE;
	showthinking=true;
    return;
}

void dot_command()
{
	byte pvi;
    // show thinking output
	if ((thread_state==THREAD_THINKING)&&(showthinking))
	{
		curtime = ((clock() - starttime) / (CLOCKS_PER_SEC / 100));
		printf("stat01: %i %i %i %i %i\n",(int)curtime,nodes,curdepth,movesavail[0]-curmove,movesavail[0]);
		fflush(stdout);
		//stat01: time nodes ply mvleft mvtot mvname
	}
    return;
}

void exit_command()
{
    mode = MODE_FORCE;
    enginecolour = 0;
    if (thread_state!=THREAD_IDLE){stop_thread();}
    return;
}

// playother_command();                             // set engine to play colour not in turn
void playother_command()
{
    mode = MODE_GO;
    enginecolour = 30 - rootboard.turn;
    if (thread_state!=THREAD_IDLE){stop_thread();}
    return;
}

//void undo_command();                        // enter force mode and undo last move
void undo_command()
{
    if (thread_state!=THREAD_IDLE){stop_thread();}
	undomove(&rootboard);
    return;
}

//void remove_command();                      // go back two moves
void remove_command()
{
//    undomove();
//    undomove();
    return;
}

void show_command()
{
	s_int eval;
	int a;
    printboard(&rootboard);
	if (checktest(&rootboard,tmpboard.turn)){printf("Check!\n");}
    fflush(stdout);
    return;
}

void quit_command()
{
    fflush(stdout);
    programclose = true;
    if (thread_state!=THREAD_IDLE){stop_thread();}
    return;
}

//void usermove_command();                      // parse move
void usermove_command(char *mov)
{
    byte x1,x2,y1,y2,tmp,cur,pro,cap,pvi;
    if (mode==MODE_GO)
	{
		if (rootboard.turn==enginecolour)
		{
			printf("Illegal move (Not your Turn!): ");
			printmove(BLANK,usermove.source,usermove.destination,usermove.promotion);
			printf("\n");
			fflush(stdout);
			return;
		}
	}
    if (thread_state!=THREAD_IDLE){stop_thread();}
    pro=BLANK;
    cap=BLANK;
    x1=0;y1=0;x2=0;y2=0;
    if (mov[4] == 'q'){pro=QUEEN;}
    if (mov[4] == 'r'){pro=ROOK;}
    if (mov[4] == 'n'){pro=KNIGHT;}
    if (mov[4] == 'b'){pro=BISHOP;}
    if (pro != BLANK){pro = pro + rootboard.turn;}
    for (cur=0;cur<=3;cur++)
    {
        tmp = (byte)mov[cur];
        if ((cur == 0)||(cur == 2))
        {
            if ((tmp < 97)||(tmp > 104))
            {
                cur--;
            }
            else
            {
                if (cur == 0){x1=tmp-96;}
                else{x2=tmp-96;}
            }
        }
        else
        {
            if ((tmp < 49)||(tmp > 56))
            {
                cur--;
            }
            else
            {
                if (cur == 1){y1=tmp-48;}
                else{y2=tmp-48;}
            }
        }
    }
    usermove.source=squareinmemory(x1,y1);
    usermove.destination=squareinmemory(x2,y2);
    usermove.promotion=pro;
    usermove.capture=cap;
    domove(&rootboard,&usermove);
    return;
}


//void modecheck();                           // check if its engine's turn to move
void modecheck()
{
	if (thread_state==THREAD_IDLE)
	{
		if (mode == MODE_GO)
		{
			if (rootboard.turn == enginecolour)
			{
				// engine's turn, lets work out how long to think
				if (clock_moves)
				{   // game is on a simgle time control
					stoptime = clock_gametime * 95;
					stoptime = stoptime / clock_moves;
				}
				else
				{
				    if (clock_inc)
    					stoptime = ((clock_time-(clock_inc*100))/26) + (clock_inc*95);
    			    else
        			    stoptime = (clock_time/32);
				}
				if (stoptime<10){stoptime=10;}            // make sure we think for at least a second
				tmpboard = rootboard;
				start_thread();
			}
			/*else if (ponder == true)
			{
			    // ponder mode
			    stoptime = 10000000;
			    tmpboard = rootboard;
			    start_thread();
			} //*/
		}
		else if (mode == MODE_ANALYZE)
		{
			// analyze this position
			stoptime = 10000000;
			tmpboard = rootboard;
			start_thread();
		}
    }
    return;
}

