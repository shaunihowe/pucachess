// Puca Chess - By Shaun Howe
// https://github.com/shaunihowe/pucachess
// GNU General Public License v3.0 - see file 'LICENSE' for more information

void start_thread()           // start a thread thinking
{
    byte rc,tmp;
    tmp=0;
    rc = pthread_create(&thread_id, NULL, thinkmove, (void *)tmp);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        fflush(stdout);
    }
    return;
}

void stop_thread()            // stop the thread
{
    void *exitstatus;
    thread_signal = SIGNAL_TERMINATE;
    // Wait for the thread to terminate.
    pthread_join(thread_id,&exitstatus);
    return;
}

void stop_thread_move()			// stop the thread and move
{
    void *exitstatus;
    thread_signal = SIGNAL_TERM_MOVE;
    // Wait for the thread to terminate.
    pthread_join(thread_id,&exitstatus);
    return;
}

void thinkmove()
{
	unsigned int pvi;
	s_int lastscore;
	float lastcurtime;
	thread_state=THREAD_THINKING;
	thread_signal=SIGNAL_READY;
	setjmp(checkpoint);
	if (thread_signal!=SIGNAL_READY)
	{
		while (depth)
		{
			undomove(&tmpboard);
			--depth;
		}
		stopthinking();
	}
	starttime = clock();
	memset(pvl, 0, sizeof(pvl));
	memset(pv, 0, sizeof(pv));
	memset(history, 0, sizeof(history));
	memset(follows, 0, sizeof(follows));
	//hash_flush();
	nodes=0;
	for (curdepth=0;curdepth<TT_SIZE;curdepth++)
	{
   	    if (transtable[curdepth].numpieces>tmpboard.numpieces)
   	        transtable[curdepth].depth=0;
   	    if (transtable[curdepth].depth!=0){nodes++;}
	}
	//printf("# %i/%i\n",nodes,TT_SIZE);
	fflush(stdout);
    nodes=0;score=0;depth=0;lastscore=0;
	movesavail[depth]=0;generatecaptures();generatemoves();score=0;drawscore=10;
	for (curdepth=1;((curdepth<=maxsearchdepth)&&(score<9000)&&(score>-9000));curdepth++)
	{
		depthreached=0;qsdepthreached=0;onpv=true;onextension=false;hashhit=0;hashlook=0;
   		score = alphabeta(curdepth,-100000,100000);
		if ((curdepth<3)&&(score>-9000)&&(score<9000)&&(score!=0)){drawscore=score;}
		curtime = ((clock() - starttime) / (CLOCKS_PER_SEC / 100));
		if ((showthinking==true))//&&((nodes>1000)||(curdepth>5)))
		{
			// ply score time nodes pv
    		printf("%i %i %i %i ",curdepth,score,(int)curtime,nodes);
			//printf("%i %i %i %i (%i/%i) ",curdepth,score,(int)curtime,nodes,depthreached,qsdepthreached);
			for (pvi=0;pvi<pvl[0];++pvi)
			{
				printmove(BLANK,pv[0][pvi].source,pv[0][pvi].destination,pv[0][pvi].promotion);
			}
			printf("\n");
			fflush(stdout);
			
		} // */
		/*if ((lastscore!=score)&&((curtime-lastcurtime)>100))
		{
		    if (curdepth>2)
		    {
		        curdepth=curdepth-1;
		        //if ((curtime-lastcurtime)<10){curdepth=curdepth-1;}
		    }
		} //*/
    	lastscore=score;
    	lastcurtime=curtime;
	}
	// ply score time nodes pv
	printf("%i %i %i %i (%i/%i) ",curdepth,score,(int)curtime,nodes,depthreached,qsdepthreached);
	for (pvi=0;pvi<pvl[0];++pvi)
	{
		printmove(BLANK,pv[0][pvi].source,pv[0][pvi].destination,pv[0][pvi].promotion);
	}
	printf("\n");
	fflush(stdout);
	thread_signal=SIGNAL_TERMINATE;
	if (mode==MODE_GO){thread_signal=SIGNAL_TERM_MOVE;}
	fflush(stdout);
	stopthinking();
	return;
}

void stopthinking()
{
	byte pvi, isdraw;
    fflush(stdout);
	if ((mode==MODE_GO)&&(enginecolour==rootboard.turn)){thread_signal=SIGNAL_TERM_MOVE;}
    if (thread_signal==SIGNAL_TERM_MOVE)
    {
    	isdraw = draw(&rootboard);
		printf("move ");
    	printmove(BLANK, pv[0][0].source, pv[0][0].destination, pv[0][0].promotion);
	    printf("\n");
		tmpboard=rootboard;
		domove(&rootboard, pv[0]);
    	if (isdraw==false)
    	{
			//if (enginecolour==BLACKARMY){score=-score;}
			if (score==9999){printf("1-0 {Checkmate!}\n");}
			else {isdraw = draw(&rootboard);}
			if (isdraw==false)
			{
				if (score<=-9998){printf("resign\n");}	// resign if loss in 1
				else if ((score<=-9900)&&(drawscore>0)){printf("offer draw\n");}	// its possible the opponent hasn't seen the mate, if we are otherwise in a better position maybe they would accept a draw
				else if ((score<=-1600)&&(drawscore<-950)){printf("resign\n");}		// if the evaluation is bad and theres no possiblity of opponent errors then resign
				else if (score==0)
				{
					// do we want the draw?
					if (drawscore<1)
					{
						// yes, get the draw
						printf("offer draw\n");
					}
					else
					{
						// we dont want the draw!, dont mention anything and make either the opponent or xboard declare it.
						// however if the opponent requests a draw, then do the decent thing and accept (within reason)
						if (drawoffer==true){printf("offer draw\n");}
					}
				}
				else if (drawoffer==true)
				{
					// opponent has offered a draw, do we want it?
					if (score<-120){printf("offer draw\n");}
				}
			}
		}
        drawoffer=false;
        fflush(stdout);
    }
    thread_signal=SIGNAL_READY;
    thread_state=THREAD_IDLE;
    pthread_exit(NULL);
	return;
}

s_int alphabeta(byte d, s_int a, s_int b)
{
	byte mov,pvi,legalmove,ext,reps;
	s_int s,ts;
    unsigned int curttentry;
	if (depth>depthreached){depthreached=depth;}
    reps = repeattest(&tmpboard);
    if (reps>1){return 0;}

   	curttentry = (tmpboard.hash % TT_SIZE);
   	
   	if ((transtable[curttentry].hash==tmpboard.hash)&&(depth))
   	{
   	    s = transtable[curttentry].eval;
        if (s!=0)
   	    {
   	        if (transtable[curttentry].depth>=d)
       	    {
   	            if ((transtable[curttentry].evaltype>0)&&(s>=b)){return b;}
   	            else if ((transtable[curttentry].evaltype==0)&&(s<a)){return a;}
   	            //else if ((transtable[curttentry].evaltype==1)&&(s>a)&&(!reps)){return s;}
   	        }
   	        //else if ((transtable[curttentry].depth>=2)&&(transtable[curttentry].evaltype>0)&&(s>b)){return b;}
       	}
   	} //*/
   	
	if (checktest(&tmpboard, tmpboard.turn))
	{
	    if ((tmpboard.turn==enginecolour)&&(d<4))
	    {
	        if (qsearch(a,a+1)==a){return a;}
	    }
	    d++;
	}
   	
    if (d==0){return qsearch(a,b);}

    if (depth && reps)
    {
        if ((0<=a)||(0>=b)){return 0;}
        if (onpv){d++;}
    }
    
	nodes++;
	
	if (((nodes & 1023) == 0)||(thread_signal!=SIGNAL_READY))
	{
		curtime = ((clock() - starttime) / (CLOCKS_PER_SEC / 100));
		if ((curtime>=stoptime)||(thread_signal!=SIGNAL_READY))
		{
			if (thread_signal==SIGNAL_READY){thread_signal=SIGNAL_TERMINATE;}
			longjmp(checkpoint, 0);
		}
	}
	
   	pvl[depth] = depth;

	if (depth==99){return evaluateboard(&tmpboard);}
	
	if (depth>0){movesavail[depth]=0;generatecaptures();generatemoves();}
	if (onpv){sortpv();}
            
	if (transtable[curttentry].hash==tmpboard.hash){sorttt();}
	legalmove=false;

	for (mov=1;mov<=movesavail[depth];mov++)
	{
		if (depth==0){curmove=mov;}
		sortmoves(mov);
		if (!validmove(&tmpboard,&movelist[depth][mov])){continue;}

		ext=false;
		if ((tmpboard.square[movelist[depth][mov].source]==WHITEPAWN)&&(movelist[depth][mov].destination>SQR_H6)){ext=true;}
		else if ((tmpboard.square[movelist[depth][mov].source]==BLACKPAWN)&&(movelist[depth][mov].destination<SQR_A3)){ext=true;}
			
		domove(&tmpboard,&movelist[depth][mov]);
		if (!checktest(&tmpboard,30-tmpboard.turn)){legalmove=true;}
		else{undomove(&tmpboard);continue;}
		depth++;
		if (!ext)
		{
			s = -alphabeta(d-1,-b,-a);
		}
		else
			s = -alphabeta(d,-b,-a);
		--depth;
		undomove(&tmpboard);
		ext=false;
		if (s>a){ext=true;}
		else if ((transtable[curttentry].hash == tmpboard.hash)&&(transtable[curttentry].source == movelist[depth][mov].source)&&(transtable[curttentry].destination == movelist[depth][mov].destination)){ext=true;}
		if (ext)
		{
		    if ((d>=transtable[curttentry].depth))
		    {
                transtable[curttentry].hash = tmpboard.hash;
                transtable[curttentry].source = movelist[depth][mov].source;
                transtable[curttentry].destination = movelist[depth][mov].destination;
                transtable[curttentry].depth = d;
                transtable[curttentry].numpieces = tmpboard.numpieces;
                transtable[curttentry].eval = s;
                transtable[curttentry].evaltype = 1;
                if (s==0){transtable[curttentry].eval = 0;}
                else if (s>=b){transtable[curttentry].eval = b;transtable[curttentry].evaltype = 2;}
                else if (s<=a){transtable[curttentry].eval = a;transtable[curttentry].evaltype = 0;}
            }
        }
		if (s>a)
		{
			history[movelist[depth][mov].source][movelist[depth][mov].destination]+=d;
			if (depth>0)
			{
				follows[movelist[depth][mov].source][movelist[depth][mov].destination].source=tmpboard.movehistory[tmpboard.nummoves].source;
				follows[movelist[depth][mov].source][movelist[depth][mov].destination].destination=tmpboard.movehistory[tmpboard.nummoves].destination;
				follows[movelist[depth][mov].source][movelist[depth][mov].destination].eval=mov;
			}
			if (!depth){movelist[depth][mov].eval=10000+(curdepth*100)+mov;}
			if (s>=b){return b;}
			a=s;
			// deal with pv
			pv[depth][depth] = movelist[depth][mov];
			for (pvi = depth + 1; pvi < pvl[depth + 1]; ++pvi)
				pv[depth][pvi] = pv[depth + 1][pvi];
			pvl[depth] = pvl[depth + 1];
			if (!depth)
			{
				score=a;
				/*if ((showthinking==true)&&((score>9000)||(nodes>500)||(curdepth>2)))
				{			// ply score time nodes pv
  					printf("%i %i %i %i (%i/%i) ",curdepth,score,(int)curtime,nodes,depthreached,qsdepthreached);
					//printmove(BLANK,movelist[0][mov].source,movelist[0][mov].destination,movelist[0][mov].promotion);
					for (pvi=0;((pvi<pvl[0])||(pvi==0));++pvi)

					{
						printmove(BLANK,pv[0][pvi].source,pv[0][pvi].destination,pv[0][pvi].promotion);
					}
					printf("\n");
				} //*/
			}
		}
	}
	if (transtable[curttentry].hash == tmpboard.hash)
	{
	    if (transtable[curttentry].eval > 9000){transtable[curttentry].depth=99;}
	}
    /*else
    {
        // position didn't increase alpha
	    if ((d>=transtable[curttentry].depth))
	    {
            transtable[curttentry].hash = tmpboard.hash;
            transtable[curttentry].depth = d;
            transtable[curttentry].numpieces = tmpboard.numpieces;
            transtable[curttentry].eval = a;
            transtable[curttentry].evaltype = 0;
            if (s==0){transtable[curttentry].eval = 0;}
        }
    } //*/
	if (!legalmove)
	{
		if (checktest(&tmpboard,tmpboard.turn))
			return LOSE_CHECKMATE + depth;
		else
			return 0;
	}
	if (tmpboard.fifty >= 100){return 0;}
	return a;
}

s_int qsearch(s_int a, s_int b)
{
	byte mov,pvi;
	s_int s;
	nodes++;
	if (depth>qsdepthreached){qsdepthreached=depth;}
	pvl[depth] = depth;

	if (depth==99){return evaluateboard(&tmpboard);}
	
	s = evaluateboard(&tmpboard);
	if (s >= b){return b;}
	if (s > a){a = s;}
	
    movesavail[depth]=0;generatecaptures();
	if (onpv){sortpv();}
	for (mov=1;mov<=movesavail[depth];mov++)
	{
		sortmoves(mov);
		domove(&tmpboard,&movelist[depth][mov]);
		depth++;
		s = -qsearch(-b,-a);
		--depth;
		undomove(&tmpboard);

    	if (s>a)
		{
			if (s>=b){return b;}
			a=s;
			// deal with pv
			pv[depth][depth] = movelist[depth][mov];
			for (pvi = depth + 1; pvi < pvl[depth + 1]; ++pvi)
				pv[depth][pvi] = pv[depth + 1][pvi];
			pvl[depth] = pvl[depth + 1];
		}
    }

    return a;
}

