#include<fstream.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#include<graphics.h>
#include"PLAYER.H"
#include"WORMS.H"
#include"COVER.H"
#include"SOUND.H"
#include"EXTRA.H"
#include"ANIMATIO.H"

int solidarr[2][96],softarr[2][100],bombarr[2][5],wormsarr[2][10],wormx[10],wormy[10],level;
int flagmap=0,count=0,noofworms=10,flag[10],doorx,doory,flagdoor=0,maintime=300,bonusleveltime=60;
int set=1,pflag[4]={0,0,0,0},powerx[4]={-1,-1,-1,-1},powery[4]={-1,-1,-1,-1},shield=0,pass=0,lives=3,maxbombs=1,brange=1,bonusflag=0;
long score=0;
clock_t stop,end;
char strmaintime[]="300",strbonusleveltime[]="60",strscore[15]="0",strlives[2]="3",strbrange[]="1";
char strmaxbombs[]="1",strup[]="                 ",NAME[50];

//pflag=power up flag which is =1-when powerup is visible, 0-when powerup is hidden, -1-when powerup is used of destroyed

void playermovement();
void controller(int a);

class scores            	//FOR HIGH SCORES
{
	public:
		long SCORE;
		char name[50];
		void display(int y);
	}obj,xyz;

void scores::display(int y)
{
	int i=0;
	char ar[10];
	long count;
	outtextxy(30,y,name);
	outtextxy(250,y,"-");
	count=SCORE;

	while(count)
	{
		i++;
		count/=10;
		}
	ar[i]='\0';
	i--;
	while(SCORE)
	{
		ar[i]=SCORE%10+48;
		SCORE/=10;
		i--;
		}
	setcolor(RED);
	outtextxy(400,y,ar);
	}

void HIGH()			//TO SAVE HIGH SCORE WHEN GAME OVER
{
	ifstream fin("HIGHSCOR.TXT");
	ofstream fout("TEMPORARY.TXT");
	int A=1;
	for(int i=0;i<5;i++)
	{
		fin.read((char *) &obj,sizeof(obj));
		if(score>obj.SCORE && A==1)
		{
			xyz.SCORE=score;
			strcpy(xyz.name,NAME);
			fout.write((char *) &xyz,sizeof(xyz));
			A=0;
			i--;
			}
		else
			fout.write((char *) &obj,sizeof(obj));
		if(score==obj.SCORE && A==1)
		{
			if(strcmp(NAME,obj.name)>0)
			{
				A=0;
				xyz.SCORE=score;
				strcpy(xyz.name,NAME);
				fout.write((char *) &xyz,sizeof(xyz));
				}
			else
				fout.write((char *) &obj,sizeof(obj));
			i++;
			}
		}
	fout.close();
	fin.close();
	remove("HIGHSCOR.TXT");
	rename("TEMPORARY.TXT","HIGHSCOR.TXT");
	}

class continu		//FOR CONTINUE
{
	public:
		char name[50];
		long score;
		int set,lives,brange,maxbombs,level;

	}OBJ;
void bonus()		//to create bonus level
{
	flagmap=1;
	flagdoor=0;
	doorx=doory=-1;
	setbkcolor(LIGHTGRAY);
	setcolor(WHITE);
	rectangle(10,50,635,475);         //SIZE OF BLOCK 25,25
	setfillstyle(1,GREEN);
	floodfill(50,150,WHITE);

	//REMOVING DATA OF PREVIOUS SOFT BLOCKS

	for(int i=0;i<100;i++)
	{	softarr[0][i]=-1;
		softarr[1][i]=-1;
		}

	//SOLID BLOCK

	int j=0,xcoordinate=1,ycoordinate=1;
	for(int y=75;y<475;y+=50,ycoordinate+=2)
	{
		xcoordinate=1;
		for(int x=35;x<635;x+=50,xcoordinate+=2)
		{
			setcolor(WHITE);
			rectangle(x,y,x+25,y+25);
			setfillstyle(1,LIGHTGRAY);
			floodfill(x+1,y+1,WHITE);
			solidarr[0][j]=xcoordinate;
			solidarr[1][j]=ycoordinate;
			j++;
			}
		}

	settextstyle(2,0,5);
	//TO WRITE TIME

	bonusleveltime=60;
	strbonusleveltime[0]='6';
	strbonusleveltime[1]='0';
	setcolor(DARKGRAY);
	outtextxy(20,20,"TIMER:");
	outtextxy(80,20,"60");

	//TO WRITE NO. OF LIVES
	outtextxy(120,20,"LIVES:");
	outtextxy(170,20,strlives);

	//TO WRITE THE BOMB RANGE
	outtextxy(200,20,"RANGE:");
	outtextxy(250,20,strbrange);

	//TO WRITE MAX BOMBS
	outtextxy(280,20,"BOMBS:");
	outtextxy(330,20,strmaxbombs);
	//TO WRITE SCORE

	outtextxy(550,20,"SCORE:");
	outtextxy(600,20,strscore);
	//TO WRITE POWER UP

	outtextxy(350,20,"POWERUP:");
	outtextxy(420,20,strup);

	playermovement();
	}

void timebonus()		//TO MANAGE TIMER OF BONUS LEVEL
{
	int x;
	end=clock();
	x=(end-stop)/CLK_TCK;
	if(x==1)
	{
		stop=clock();
		setcolor(LIGHTGRAY);
		outtextxy(80,20,strbonusleveltime);

		//strmaintime is used as outtextxy works for strings only

		if(strbonusleveltime[1]=='0')
		{
			strbonusleveltime[1]='9';
			strbonusleveltime[0]--;
			}
		else
			strbonusleveltime[1]--;
		bonusleveltime--;
		setcolor(DARKGRAY);
		if(bonusleveltime<=10)
			setcolor(RED);
		outtextxy(80,20,strbonusleveltime);
		}

	}
void timer()    	//to control main timer
{
	int x;
	end=clock();
	x=(end-stop)/CLK_TCK;
	if(x==1)
	{
		stop=clock();
		setcolor(LIGHTGRAY);
		outtextxy(80,20,strmaintime);

		//strmaintime is used as outtextxy works for strings only

		if(strmaintime[1]=='0'&&strmaintime[2]=='0')
		{
			strmaintime[0]--;
			strmaintime[1]='9';
			strmaintime[2]='9';
			}
		else if(strmaintime[2]=='0')
		{
			strmaintime[2]='9';
			strmaintime[1]--;
			}
		else
			strmaintime[2]--;
		maintime--;
		setcolor(DARKGRAY);
		if(shield==1)
			setcolor(CYAN);
		if(maintime<=10)
			setcolor(RED);
		outtextxy(80,20,strmaintime);
		}

	}

void map()     	//TO CREATE LEVELS
{
	flagmap=0;
	setbkcolor(LIGHTGRAY);
	setcolor(WHITE);
	rectangle(10,50,635,475);         //SIZE OF BLOCK 25,25
	setfillstyle(1,GREEN);
	floodfill(50,150,WHITE);

	//SOLID BLOCK

	int j=0,xcoordinate=1,ycoordinate=1;
	for(int y=75;y<475;y+=50,ycoordinate+=2)
	{
		xcoordinate=1;
		for(int x=35;x<635;x+=50,xcoordinate+=2)
		{
			setcolor(WHITE);
			rectangle(x,y,x+25,y+25);
			setfillstyle(1,LIGHTGRAY);
			floodfill(x+1,y+1,WHITE);
			solidarr[0][j]=xcoordinate;
			solidarr[1][j]=ycoordinate;
			j++;
			}
		}

	//SOFT BLOCK

	int xrandom,yrandom,A,B;
	randomize();
	for(int i=0;i<100;i++)
	{
		A=random(25);           //coordinate positions
		B=random(17);

		for(int j=0;j<i;j++)
			if(A==softarr[0][j]&&B==softarr[1][j])
				break;
		if((A==0&&B==0)||(A==1&&B==0)||(A==0&&B==1)||(A%2!=0&&B%2!=0)||j<i)
					//checking block (0,0); block (0,1); block (1,0)
		{                       //as solid block is when both A and B are odd
			i--;            //checking that A and B aren't odd so solid isn't replaced by soft block
			continue;
			}
		xrandom=(25*A)+10;       //pixel position
		yrandom=(25*B)+50;
		makesoftbox(xrandom,yrandom);
		softarr[0][i]=A;
		softarr[1][i]=B;
		}

	//TO CREATE POWER UPS

	int pownum[10];
	for(i=0;i<4;i++)
	{	pownum[i]=random(100);
		for(int k=0;k<i;k++)
			if(pownum[i]==pownum[k])
				break;
		if(k!=i)
		{	--i;
			continue;
			}
		pflag[i]=0;
		powerx[i]=softarr[0][pownum[i]];
		powery[i]=softarr[1][pownum[i]];
		}

	//TO CREATE DOOR

	flagdoor=0;
	A=random(100);
	doorx=softarr[0][A];
	doory=softarr[1][A];
	settextstyle(2,0,5);


	//TO WRITE TIME
	maintime=300;
	strmaintime[0]='3';
	strmaintime[1]='0';
	strmaintime[2]='0';
	setcolor(DARKGRAY);
	outtextxy(20,20,"TIMER:");
	outtextxy(80,20,"300");

	//TO WRITE NO. OF LIVES
	outtextxy(120,20,"LIVES:");
	outtextxy(170,20,strlives);

	//TO WRITE THE BOMB RANGE
	outtextxy(200,20,"RANGE:");
	outtextxy(250,20,strbrange);

	//TO WRITE MAX BOMBS
	outtextxy(280,20,"BOMBS:");
	outtextxy(330,20,strmaxbombs);

	//TO WRITE SCORE

	outtextxy(550,20,"SCORE:");
	outtextxy(600,20,strscore);

	//TO WRITE POWER UP

	outtextxy(350,20,"POWER UP:");
	outtextxy(420,20,strup);
	playermovement();
	}

void updatelives(int &lives,int &posx,int &posy,int &x,int &y,int &brange,int &maxbombs)
{
	setcolor(LIGHTGRAY);                         //to update the lives and
	outtextxy(170,20,strlives);                  //the notification
	--lives;
	live();
	strlives[0]=lives+48;
	setcolor(DARKGRAY);
	outtextxy(170,20,strlives);
	replaceplayer(x,y);
	posx=0;posy=0;
	originalplayer(20,60,0);
	x=20;y=60;
	if(pass==1)
		pass=0;
	if(shield==1)        //if shield is active
	{
		shield=0;    //if we die we loss this powerup
		setcolor(DARKGRAY);        //then converting everything back to dark gray
		if(maintime<=10)
			setcolor(RED);
		outtextxy(20,20,"TIMER:");
		outtextxy(80,20,strmaintime);

		setcolor(DARKGRAY);
		outtextxy(120,20,"LIVES:");
		outtextxy(170,20,strlives);

		outtextxy(200,20,"RANGE:");
		outtextxy(250,20,strbrange);

		outtextxy(280,20,"BOMBS:");
		outtextxy(330,20,strmaxbombs);

		outtextxy(550,20,"SCORE:");
		outtextxy(600,20,strscore);

		outtextxy(350,20,"POWER UP:");
		outtextxy(420,20,strup);
		setcolor(LIGHTGRAY);
		rectangle(535,18,548,37);
		setfillstyle(1,LIGHTGRAY);
		floodfill(540,25,LIGHTGRAY);
		}
	//if brange,maxbombs is greater than 1 then after we die we lose 1 from each, drawback of death
	if(brange>1)
	{
		setcolor(LIGHTGRAY);
		outtextxy(250,20,strbrange);
		--brange;
		strbrange[0]--;
		setcolor(DARKGRAY);
		outtextxy(200,20,"RANGE:");
		outtextxy(250,20,strbrange);
		}
	if(maxbombs>1)
	{
		setcolor(LIGHTGRAY);
		outtextxy(330,20,strmaxbombs);
		--maxbombs;
		strmaxbombs[0]--;
		setcolor(DARKGRAY);
		outtextxy(280,20,"BOMBS:");
		outtextxy(330,20,strmaxbombs);
		}
	if(lives==0)
	{
		setcolor(RED);
		cleardevice();
		levelend();
		setbkcolor(WHITE);
		settextstyle(1,0,4);
		outtextxy(230,215,"GAME OVER");
		HIGH();

		//REMOVING DATA OF THE PERSON FROM CONTINUE IF HE DIES
		ifstream fin2("CONT.TXT");
		ofstream fout2("TEMPO.TXT");
		while(!fin2.eof())
		{
			fin2.read((char *) &OBJ,sizeof(OBJ));
			if(strcmp(OBJ.name,NAME)!=0)
				fout2.write((char *) &OBJ,sizeof(OBJ));
			}
		fin2.close();
		fout2.close();
		remove("CONT.TXT");
		rename("TEMPO.TXT","CONT.TXT");

		delay(1000);
		set=1;
		score=0;
		brange=1;
		maxbombs=1;
		strlives[0]=51;
		strbrange[0]=strmaxbombs[0]=49;
		lives=3;
		getch();
		controller(1);
		}
	}

void updatescore(int type)       //for updating the score when you kill a worm
{       long tempscr;            //get a power or finish the level
	if(type==1)			 //slow worm
		score+=50;
	else if(type==2)                 //medium worm
		score+=75;
	else if(type==3)                 //fast worm
		score+=100;
	else if(type==4)                 //pass through worm
		score+=150;
	else if(type==5)                 //power up
		score+=250;
	else if(type==6)                 //time left after completing level
		score+=maintime;
	setcolor(LIGHTGRAY);
	outtextxy(600,20,strscore);
	tempscr=score;
	for(int cnt=1;tempscr>=10;cnt*=10)
		tempscr/=10;
	tempscr=score;
	int i=0;
	while(cnt)
	{	strscore[i]=tempscr/cnt+48;
		tempscr%=cnt;
		cnt/=10;
		i++;
		}
	strscore[i]='\0';
	setcolor(DARKGRAY);
	if(shield==1)
		setcolor(CYAN);
	outtextxy(600,20,strscore);
	}
void wormupdate(int &p)
{
	if(set==1)
	{	replaceslowworm(wormx[p],wormy[p]);
		updatescore(1);
		}
	else if(set==2)
	{
		if(p<5)
		{	replaceslowworm(wormx[p],wormy[p]);
			updatescore(1);
			}
		else
		{	replacemediumworm(wormx[p],wormy[p]);
			updatescore(2);
			}
		}
	else if(set==3)
	{
		if(p<4)
		{	replaceslowworm(wormx[p],wormy[p]);
			updatescore(1);
			}
		else if(p<7)
		{	replacemediumworm(wormx[p],wormy[p]);
			updatescore(2);
			}
		else
		{	replacefastworm(wormx[p],wormy[p]);
			updatescore(3);
			}
		}
	else if(set==4)
	{
		if(p<3)
		{	replaceslowworm(wormx[p],wormy[p]);
			updatescore(1);
			}
		else if(p<6)
		{	replacemediumworm(wormx[p],wormy[p]);
			updatescore(2);
			}
		else if(p<8)
		{	replacefastworm(wormx[p],wormy[p]);
			updatescore(3);
			}
		else
		{	replacepassthroughworm(wormx[p],wormy[p]);
			updatescore(4);
			}
		}
	wormsarr[0][p]=-1;
	wormsarr[1][p]=-1;
	noofworms--;
	}

void bombblast(time_t start[15],time_t end[15],int &BOMB,int &x,int &y,int &posx,int &posy,int &lives,int &brange,int wormx[15],int wormy[15],int &maxbombs)
{
	int bombx,bomby,flagt=0,flagb=0,flagl=0,flagr=0,ft=0,fb=0,fl=0,fr=0;
	int M=0,N=0,O=0,P=0,Q=0,R=0,S=0,T=0,DOORr=0,DOORt=0,DOORb=0,DOORl=0;
	for(int i=0;i<count;i++)		//to check whether to blast the bomb or not
	{					//after 3 seconds the bomb blasts
		end[i]=time(NULL);
		if(difftime(end[i],start[i])>=3)
		{
			flag[i]=0;
			bombx=bombarr[0][i]*25+20;
			bomby=bombarr[1][i]*25+60;
			replacebomb(bombx,bomby);
			bexplode(bombx,bomby);
			bomb();

			//checking whether around the bomb player is their

			if(bombarr[0][i]==posx && bombarr[1][i]==posy)
			{       if(flagmap==0)
					updatelives(lives,posx,posy,x,y,brange,maxbombs);
				else
					bonusflag=1;
				bexplode(bombx,bomby);
				}
			for(int m=1;m<=brange;m++)
			{
				//checking above the bomb
				for(int k=0;k<96;k++)
					if(bombarr[1][i]-m==solidarr[1][k] && bombarr[0][i]==solidarr[0][k] || bombarr[1][i]-m<0)
						flagt=1;
				for(int j=0;j<100;j++)
				{	if(bombarr[1][i]-m==softarr[1][j] && bombarr[0][i]==softarr[0][j] && flagt==0 &&Q==0)
					{
						score+=5;
						updatescore(0);
						if(softarr[0][j]==doorx&&softarr[1][j]==doory)
							ft=1;
						M=1;
						softarr[0][j]=-1;
						softarr[1][j]=-1;
						}
					}

				if(flagdoor==1&&bombarr[1][i]-m==doory&&bombarr[0][i]==doorx)
					DOORt=1;
				if(bombarr[1][i]-m==posy && bombarr[0][i]==posx && flagt==0&&Q==0)
				{	if(flagmap==0)
						updatelives(lives,posx,posy,x,y,brange,maxbombs);
					else
						bonusflag=1;
					}
				for(int p=0;p<10;p++)
					if(bombarr[1][i]-m==wormsarr[1][p]&&bombarr[0][i]==wormsarr[0][p]&&flagt==0&&Q==0)
						wormupdate(p);

				if(flagt==0&&Q==0&&DOORt==0)
					bexplode(bombx,bomby-(25*m));

				//checking below the bomb

				for(k=0;k<96;k++)
					if(bombarr[1][i]+m==solidarr[1][k] && bombarr[0][i]==solidarr[0][k] || bombarr[1][i]+m>16)
						flagb=1;
				for( j=0;j<100;j++)
				{	if(bombarr[1][i]+m==softarr[1][j] && bombarr[0][i]==softarr[0][j] && flagb==0 && R==0)
					{
						score+=5;
						updatescore(0);
						if(softarr[0][j]==doorx&&softarr[1][j]==doory)
							fb=1;
						softarr[0][j]=-1;
						softarr[1][j]=-1;
						N=1;
						}
					}
				if(flagdoor==1&&bombarr[1][i]+m==doory&&bombarr[0][i]==doorx)
					DOORb=1;
				if(bombarr[1][i]+m==posy && bombarr[0][i]==posx && flagb==0&&R==0)
				{	if(flagmap==0)
						updatelives(lives,posx,posy,x,y,brange,maxbombs);
					else
						bonusflag=1;
					}
				for( p=0;p<10;p++)
					if(bombarr[1][i]+m==wormsarr[1][p]&&bombarr[0][i]==wormsarr[0][p]&&flagb==0&&R==0)
						wormupdate(p);

				if(flagb==0&&R==0&&DOORb==0)
					bexplode(bombx,bomby+(25*m));

				//checking left of bomb
				for( k=0;k<96;k++)
					if(bombarr[0][i]-m==solidarr[0][k] && bombarr[1][i]==solidarr[1][k] || bombarr[0][i]-m<0)
						flagl=1;
				for( j=0;j<100;j++)
				{
					if(bombarr[0][i]-m==softarr[0][j] && bombarr[1][i]==softarr[1][j] && flagl==0&&S==0)
					{
						score+=5;
						updatescore(0);
						if(softarr[0][j]==doorx&&softarr[1][j]==doory)
							fl=1;
						softarr[0][j]=-1;
						softarr[1][j]=-1;
						O=1;
						}
					}
				if(flagdoor==1&&bombarr[0][i]-m==doorx&&bombarr[1][i]==doory)
					DOORl=1;
				if(bombarr[0][i]-m==posx && bombarr[1][i]==posy &&flagl==0&&S==0)
				{	if(flagmap==0)
						updatelives(lives,posx,posy,x,y,brange,maxbombs);
					else
						bonusflag=1;
					}
				for( p=0;p<10;p++)
					if(bombarr[0][i]-m==wormsarr[0][p]&&bombarr[1][i]==wormsarr[1][p]&&flagl==0&&S==0)
						wormupdate(p);

				if(flagl==0&&S==0&&DOORl==0)
					bexplode(bombx-(25*m),bomby);

				//checking right of bomb
				for( k=0;k<96;k++)
					if(bombarr[0][i]+m==solidarr[0][k] && bombarr[1][i]==solidarr[1][k] || bombarr[0][i]+m>24)
						flagr=1;
				for( j=0;j<100;j++)
				{	if(bombarr[0][i]+m==softarr[0][j] && bombarr[1][i]==softarr[1][j] && flagr==0 && T==0)
					{
						score+=5;
						updatescore(0);
						if(softarr[0][j]==doorx&&softarr[1][j]==doory)
							fr=1;
						softarr[0][j]=-1;
						softarr[1][j]=-1;
						P=1;
						}
					}
				if(bombarr[0][i]+m==posx && bombarr[1][i]==posy&&flagr==0&&T==0)
				{	if(flagmap==0)
						updatelives(lives,posx,posy,x,y,brange,maxbombs);
					else
						bonusflag=1;
					}
				if(flagdoor==1&&bombarr[0][i]+m==doorx&&bombarr[1][i]==doory)
					DOORr=1;
				for( p=0;p<10;p++)
					if(bombarr[0][i]+m==wormsarr[0][p]&&bombarr[1][i]==wormsarr[1][p]&&flagr==0&&T==0)
						wormupdate(p);

				if(flagr==0&&T==0&&DOORr==0)
					bexplode(bombx+(25*m),bomby);

				//replacing visual of explode

				delay(50);
				if(flagr==0&&T==0&&DOORr==0)
				{
					repexplode(bombx+(25*m),bomby);
					if(fr==1)
					{
						flagdoor=1;
						door(bombx+(25*m)-10,bomby-10);
						fr=0;
						}
					for(int h=0;h<count;h++)
					{
						if(bombarr[0][i]+m==bombarr[0][h]&&bombarr[1][i]==bombarr[1][h])
							bomb(bombx+(25*m),bomby);
						}
					}
				if(flagl==0&&S==0&&DOORl==0)
				{
					repexplode(bombx-(25*m),bomby);
					if(fl==1)
					{
						flagdoor=1;
						door(bombx-(25*m)-10,bomby-10);
						fl=0;
						}
					for(int h=0;h<count;h++)
					{
						if(bombarr[0][i]-m==bombarr[0][h]&&bombarr[1][i]==bombarr[1][h])
							bomb(bombx-(25*m),bomby);
						}
					}
				if(flagb==0&&R==0&&DOORb==0)
				{
					repexplode(bombx,bomby+(25*m));
					if(fb==1)
					{
						flagdoor=1;
						door(bombx-10,bomby+(25*m)-10);
						fb=0;
						}
					for(int h=0;h<count;h++)
					{
						if(bombarr[1][i]+m==bombarr[1][h]&&bombarr[0][i]==bombarr[0][h])
							bomb(bombx,bomby+(25*m));
						}
					}
				if(flagt==0&&Q==0&&DOORt==0)
				{
					repexplode(bombx,bomby-(25*m));
					if(ft==1)
					{
						flagdoor=1;
						door(bombx-10,bomby-(25*m)-10);
						ft=0;
						}
					for(int h=0;h<count;h++)
					{
						if(bombarr[1][i]-m==bombarr[1][h]&&bombarr[0][i]==bombarr[0][h])
							bomb(bombx,bomby-(25*m));
						}
				}
				//TO CHECK WHETHER AROUND BOMB POWERUP IS THEIR

				for(int d=0;d<4;d++)
				{	if(bombarr[1][i]-m==powery[d]&&bombarr[0][i]==powerx[d]&&flagt==0&&pflag[d]!=-1&&Q==0)
					{	drawpowerup(powerx[d]*25+14,powery[d]*25+54);
						pflag[d]=1;
						}

					else if(bombarr[1][i]+m==powery[d]&&bombarr[0][i]==powerx[d]&&flagb==0&&pflag[d]!=-1&&R==0)
					{	drawpowerup(powerx[d]*25+14,powery[d]*25+54);
						pflag[d]=1;
						}

					else if(bombarr[0][i]-m==powerx[d]&&bombarr[1][i]==powery[d]&&flagl==0&&pflag[d]!=-1&&S==0)
					{	drawpowerup(powerx[d]*25+14,powery[d]*25+54);
						pflag[d]=1;
						}

					else if(bombarr[0][i]+m==powerx[d]&&bombarr[1][i]==powery[d]&&flagr==0&&pflag[d]!=-1&&T==0)
					{	drawpowerup(powerx[d]*25+14,powery[d]*25+54);
						pflag[d]=1;
						}
					}
				if(M==1)
					Q=1;
				if(N==1)
					R=1;
				if(O==1)
					S=1;
				if(P==1)
					T=1;
				}

			repexplode(bombx,bomby);

			//to check whether around the bomb in square(bombx-m,bomby-m,bombx+m,bomby+m,
			//soft/solid block is their, if then make it again so that its boundary is not disappeared

			for(int j=bombarr[0][i]-m;j<=bombarr[0][i]+m;j++)
			{
				for(int k=bombarr[1][i]-m;k<=bombarr[1][i]+m;k++)
				{
					//whether around it soft block is their
					for(int J=0;J<100;J++)
					{
						if(softarr[0][J]==j&&softarr[1][J]==k&&softarr[0][J]!=-1)
						{
							setcolor(WHITE);
							makesoftbox(softarr[0][J]*25+10,softarr[1][J]*25+50);
							}
						}
					//whether around it solid block is their
					for(int K=0;K<96;K++)
					{
						if(solidarr[0][K]==j&&solidarr[1][K]==k)
						{
							setcolor(WHITE);
							rectangle(solidarr[0][K]*25+10,solidarr[1][K]*25+50,solidarr[0][K]*25+35,solidarr[1][K]*25+75);
							}
						}
					}
				}
			setcolor(WHITE);
			rectangle(10,50,635,475);

			flag[i]=0;

			//after bomb blast to renew array of bombs

			for(int g=i;g<count;g++)
			{	start[g]=start[g+1];
				end[g]=end[g+1];
				bombarr[0][g]=bombarr[0][g+1];
				bombarr[1][g]=bombarr[1][g+1];
				}
			count--;
			i--;
			}
		}
	if(count==0)
		BOMB=0;
	}

void powerups(int &lives,int &brange,int &maxbombs)
{
	powerup();
	setcolor(LIGHTGRAY);
	outtextxy(420,20,strup);
	int rnum=rand()%6;
	if(set==4)            //if set is 4 only then pass through power up is active
		rnum=rand()%7;
	if(rnum==0 && lives<9)			//to increase lives
	{
		strcpy(strup,"LIVES");
		setcolor(LIGHTGRAY);
		outtextxy(170,20,strlives);
		lives++;
		strlives[0]=lives+48;
		setcolor(DARKGRAY);
		if(shield==1)
			setcolor(CYAN);
		outtextxy(170,20,strlives);
		}
	else if(rnum==1 && brange<4)            //to increase bomb range
	{
		strcpy(strup,"BOMB RANGE");
		brange++;
		setcolor(LIGHTGRAY);
		outtextxy(250,20,strbrange);
		strbrange[0]++;
		setcolor(DARKGRAY);
		if(shield==1)
			setcolor(CYAN);
		outtextxy(250,20,strbrange);
		}
	else if(rnum==2 && maxbombs<5)          //to increase no of bombs
	{
		strcpy(strup,"BOMBS");
		maxbombs++;
		setcolor(LIGHTGRAY);
		outtextxy(330,20,strmaxbombs);
		strmaxbombs[0]++;
		setcolor(DARKGRAY);
		if(shield==1)
			setcolor(CYAN);
		outtextxy(330,20,strmaxbombs);
		}
	else if(rnum==3)		//score powerup
	{
		strcpy(strup,"SCORE");
		updatescore(5);
		}
	else if(rnum==4)                        //to increase time by 30
	{
		strcpy(strup,"TIME");
		setcolor(LIGHTGRAY);
		outtextxy(80,20,strmaintime);
		if(strmaintime[1]>='7')
		{	strmaintime[0]++;
			if(strmaintime[1]=='7')
				strmaintime[1]='0';

			else if(strmaintime[1]=='8')
				strmaintime[1]='1';

			else if(strmaintime[1]=='9')
				strmaintime[1]='2';
			}
		else
			strmaintime[1]+=3;
		maintime+=30;
		setcolor(DARKGRAY);
		if(shield==1)
			setcolor(CYAN);
		outtextxy(80,20,strmaintime);
		}
	else if(rnum==5 && shield==0)	//to give a shield
	{
		int rnum2=rand()%5;   //to increase rareness of shield as it is powerful
		if(rnum2%5==0)
		{
			strcpy(strup,"SHIELD");
			shield=1;
			setcolor(CYAN);
			if(maintime<=10)
				setcolor(RED);
			outtextxy(20,20,"TIMER:");
			outtextxy(80,20,strmaintime);
			setcolor(CYAN);

			outtextxy(120,20,"LIVES:");
			outtextxy(170,20,strlives);

			outtextxy(200,20,"RANGE:");
			outtextxy(250,20,strbrange);

			outtextxy(280,20,"BOMBS:");
			outtextxy(330,20,strmaxbombs);

			outtextxy(550,20,"SCORE:");
			outtextxy(600,20,strscore);

			outtextxy(350,20,"POWER UP:");
			outtextxy(420,20,strup);
			drawshield(535,18);
			}
		else
			powerups(lives,brange,maxbombs);
		}
	else if(rnum==6 && pass==0)
	{
		strcpy(strup,"PASS THROUGH");
		pass=1;
		}
	else
		powerups(lives,brange,maxbombs);

	setcolor(DARKGRAY);
	if(shield==1)
		setcolor(CYAN);
	outtextxy(420,20,strup);
	}


void speedwormmovement(clock_t wstart[15],clock_t wend[15],int i,int m[15],double speed)
//movement of worms
{
	time_t t;
	srand((unsigned) time(&t));
	int j,k,l,g,c,Q[10];
	double x;

	//slow worms ; speed=5 ;time=0.5s
	wend[i]=clock();

	//CLK_TCK is a inbuilt macro
	//clock function calculates the tick of clock and can be used
	//to find difference b/w two times ( diff can be in mini/micro sec)

	if(((wend[i]-wstart[i])/CLK_TCK)>=speed&&((wend[i]-wstart[i])/CLK_TCK)<=2)
	{
		label:
			if(m[i]==0)
			{
				//checking whether it is leaving the screen
				if(wormy[i]-5<60)
				{
					do
					{
						Q[i]=rand()%4;
						}while(Q[i]==m[i]);
					m[i]=Q[i];
					goto label;
					}

				c=wormsarr[1][i];

				//checking whether the block is changed or not

				if((c-1)*25+50<=wormy[i]-5-7&&c*25+50>=wormy[i]-5-7)
				{
					m[i]=rand()%4;

					//check for soft block

					for(j=0;j<100;j++)
					{
						if(c-1==softarr[1][j]&&wormsarr[0][i]==softarr[0][j]&&speed!=0.4)
							break;
						}
					//check for hard blocks
					for(k=0;k<96;k++)
					{
						if(c-1==solidarr[1][k]&&wormsarr[0][i]==solidarr[0][k])
							break;
						}
					//check for bombs
					for(l=0;l<count;l++)
					{
						if(c-1==bombarr[1][l]&&wormsarr[0][i]==bombarr[0][l])
							break;
						}
					//check for other worms
					for(g=0;g<10;g++)
					{       if(g==i)
							continue;
						if(c-1==wormsarr[1][g]&&wormsarr[0][i]==wormsarr[0][g])
							break;
						}

					if(j==100&&k==96&&l==count&&g==10&&(wormx[i]==wormsarr[0][i]*25+20||wormx[i]==wormsarr[0][i]*25+25)&&!(flagdoor==1&&c-1==doory&&wormsarr[0][i]==doorx))
					{
						if(speed==0.5)
							replaceslowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							replacemediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							replacefastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							replacepassthroughworm(wormx[i],wormy[i]);

						wormy[i]-=5;
						wormsarr[1][i]--;

						//to check is powerup is their it is destroyed when worm passes
						for(int d=0;d<4;d++)
						{
							if(wormsarr[1][i]==powery[d]&&wormsarr[0][i]==powerx[d]&&pflag[d]==1)
							{
								pflag[d]=-1;
								repexplode(powerx[d]*25+20,powery[d]*25+60);
								powerx[d]=powery[d]=-1;
								}
							}
						if(speed==0.5)
							slowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							mediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							fastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							passthroughworm(wormx[i],wormy[i]);
						}
					else
					{
						do
						{
							Q[i]=rand()%4;
							}while(Q[i]==m[i]);
						m[i]=Q[i];
						goto label;
						}
					}
				else
				{
					if(speed==0.5)
						replaceslowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						replacemediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						replacefastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						replacepassthroughworm(wormx[i],wormy[i]);
					wormy[i]-=5;
					if(speed==0.5)
						slowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						mediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						fastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						passthroughworm(wormx[i],wormy[i]);
					}
				}
			else if(m[i]==1)
			{
				//checking whether it is leaving the screen

				if(wormy[i]+5>460)
				{
					do
					{
						Q[i]=rand()%4;
						}while(Q[i]==m[i]);
					m[i]=Q[i];
					goto label;
					}
				c=wormsarr[1][i];

				//checking whether the block is changed or not

				if((c+1)*25+50<wormy[i]+5+10&&(c+2)*25+50>wormy[i]+5+10)
				{
					m[i]=rand()%4;
					//check for soft block
					for(j=0;j<100;j++)
					{
						if(c+1==softarr[1][j]&&wormsarr[0][i]==softarr[0][j]&&speed!=0.4)
							break;
						}
					//check for hard blocks
					for(k=0;k<96;k++)
					{
						if(c+1==solidarr[1][k]&&wormsarr[0][i]==solidarr[0][k])
							break;
						}
					//check for bombs
					for(l=0;l<count;l++)
					{
						if(c+1==bombarr[1][l]&&wormsarr[0][i]==bombarr[0][l])
							break;
						}
					//check for other worms
					for(g=0;g<10;g++)
					{       if(g==i)
							continue;
						if(c+1==wormsarr[1][g]&&wormsarr[0][i]==wormsarr[0][g])
							break;
						}
					if(j==100&&k==96&&l==count&&g==10&&(wormx[i]==wormsarr[0][i]*25+20||wormx[i]==wormsarr[0][i]*25+25)&&!(flagdoor==1&&c+1==doory&&wormsarr[0][i]==doorx))
					{
						if(speed==0.5)
							replaceslowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							replacemediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							replacefastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							replacepassthroughworm(wormx[i],wormy[i]);
						wormy[i]+=5;
						wormsarr[1][i]++;

						//to check is powerup is their it is destroyed when worm passes
						for(int d=0;d<4;d++)
							if(wormsarr[1][i]==powery[d]&&wormsarr[0][i]==powerx[d]&&pflag[d]==1)
							{	pflag[d]=-1;
								repexplode(powerx[d]*25+20,powery[d]*25+60);
								powerx[d]=powery[d]=-1;
								}
						if(speed==0.5)
							slowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							mediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							fastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							passthroughworm(wormx[i],wormy[i]);
						}
					else
					{
						do
						{
							Q[i]=rand()%4;
							}while(Q[i]==m[i]);
						m[i]=Q[i];
						goto label;
						}
					}
				else
				{
					if(speed==0.5)
						replaceslowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						replacemediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						replacefastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						replacepassthroughworm(wormx[i],wormy[i]);

					wormy[i]+=5;
					if(speed==0.5)
						slowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						mediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						fastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						passthroughworm(wormx[i],wormy[i]);
					}
				}
			else if(m[i]==2)
			{
				//checking whether it is leaving the screen

				if(wormx[i]-5<20)
				{
					do
					{
						Q[i]=rand()%4;
						}while(Q[i]==m[i]);
					m[i]=Q[i];
					goto label;
					}
				c=wormsarr[0][i];

				//checking whether the block is changed or not

				if((c-1)*25+10<=wormx[i]-5-7&&c*25+10>=wormx[i]-5-7)
				{
					m[i]=rand()%4;
					//check for soft block
					for(j=0;j<100;j++)
					{
						if(c-1==softarr[0][j]&&wormsarr[1][i]==softarr[1][j]&&speed!=0.4)
							break;
						}
					//check for hard blocks
					for(k=0;k<96;k++)
					{
						if(c-1==solidarr[0][k]&&wormsarr[1][i]==solidarr[1][k])
							break;
						}
					//check for bombs
					for(l=0;l<count;l++)
					{
						if(c-1==bombarr[0][l]&&wormsarr[1][i]==bombarr[1][l])
							break;
						}
					//check for other worms
					for(g=0;g<10;g++)
					{       if(g==i)
							continue;
						if(c-1==wormsarr[0][g]&&wormsarr[1][i]==wormsarr[1][g])
							break;
						}
					if(j==100&&k==96&&l==count&&g==10&&(wormy[i]==wormsarr[1][i]*25+60)&&!(flagdoor==1&&c-1==doorx&&wormsarr[1][i]==doory))
					{
						if(speed==0.5)
							replaceslowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							replacemediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							replacefastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							replacepassthroughworm(wormx[i],wormy[i]);

						wormx[i]-=5;
						wormsarr[0][i]--;

						//to check is powerup is their it is destroyed when worm passes
						for(int d=0;d<4;d++)
							if(wormsarr[1][i]==powery[d]&&wormsarr[0][i]==powerx[d]&&pflag[d]==1)
							{	pflag[d]=-1;
								repexplode(powerx[d]*25+20,powery[d]*25+60);
								powerx[d]=powery[d]=-1;
								}
						if(speed==0.5)
							slowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							mediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							fastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							passthroughworm(wormx[i],wormy[i]);
						}
					else
					{
						do
						{
							Q[i]=rand()%4;
							}while(Q[i]==m[i]);
						m[i]=Q[i];
						goto label;
						}
					}
				else
				{
					if(speed==0.5)
						replaceslowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						replacemediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						replacefastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						replacepassthroughworm(wormx[i],wormy[i]);
					wormx[i]-=5;
					if(speed==0.5)
						slowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						mediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						fastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						passthroughworm(wormx[i],wormy[i]);
					}
				}
			else if(m[i]==3)
			{
				//checking whether it is leaving the screen

				if(wormx[i]+5>625)
				{
					do
					{
						Q[i]=rand()%4;
						}while(Q[i]==m[i]);
					m[i]=Q[i];
					goto label;
					}

				c=wormsarr[0][i];

				//checking whether the block is changed or not

				if((c+1)*25+10<=wormx[i]+5+7&&(c+2)*25+10>=wormx[i]+5+7)
				{
					m[i]=rand()%4;
					//check for soft block
					for(j=0;j<100;j++)
					{
						if(c+1==softarr[0][j]&&wormsarr[1][i]==softarr[1][j]&&speed!=0.4)
							break;
						}
					//check for hard blocks
					for(k=0;k<96;k++)
					{
						if(c+1==solidarr[0][k]&&wormsarr[1][i]==solidarr[1][k])
							break;
						}
					//check for bombs
					for(l=0;l<count;l++)
					{
						if(c+1==bombarr[0][l]&&wormsarr[1][i]==bombarr[1][l])
							break;
						}
					//check for other worms
					for(g=0;g<10;g++)
					{       if(g==i)
							continue;
						if(c+1==wormsarr[0][g]&&wormsarr[1][i]==wormsarr[1][g])
							break;
						}
					if(j==100&&k==96&&l==count&&g==10&&(wormy[i]==wormsarr[1][i]*25+60)&&!(flagdoor==1&&c+1==doorx&&wormsarr[1][i]==doory))
					{
						if(speed==0.5)
							replaceslowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							replacemediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							replacefastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							replacepassthroughworm(wormx[i],wormy[i]);
						wormx[i]+=5;
						wormsarr[0][i]++;

						//to check is powerup is their it is destroyed when worm passes
						for(int d=0;d<4;d++)
							if(wormsarr[1][i]==powery[d]&&wormsarr[0][i]==powerx[d]&&pflag[d]==1)
							{	pflag[d]=-1;
								repexplode(powerx[d]*25+20,powery[d]*25+60);
								powerx[d]=powery[d]=-1;
								}
						if(speed==0.5)
							slowworm(wormx[i],wormy[i]);
						else if(speed==0.3)
							mediumworm(wormx[i],wormy[i]);
						else if(speed==0.2)
							fastworm(wormx[i],wormy[i]);
						else if(speed==0.4)
							passthroughworm(wormx[i],wormy[i]);
						}
					else
					{
						do
						{
							Q[i]=rand()%4;
							}while(Q[i]==m[i]);
						m[i]=Q[i];
						goto label;
						}
					}
				else
				{
					if(speed==0.5)
						replaceslowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						replacemediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						replacefastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						replacepassthroughworm(wormx[i],wormy[i]);
					wormx[i]+=5;
					if(speed==0.5)
						slowworm(wormx[i],wormy[i]);
					else if(speed==0.3)
						mediumworm(wormx[i],wormy[i]);
					else if(speed==0.2)
						fastworm(wormx[i],wormy[i]);
					else if(speed==0.4)
						passthroughworm(wormx[i],wormy[i]);
					}
				}
			wstart[i]=clock();	//after movement time counting starts for next movement
			if(speed==0.4)
			{
				for(int g=0;g<100;g++)
				{      	if(wormsarr[0][i]==softarr[0][g]&&wormsarr[1][i]==softarr[1][g])
						makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

					else if(wormsarr[0][i]-1==softarr[0][g]&&wormsarr[1][i]==softarr[1][g])//&&x-7<=(posx*25)+10)
						makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

					else if(wormsarr[0][i]+1==softarr[0][g]&&wormsarr[1][i]==softarr[1][g])//&&x+5>=(posx+1)*25+10)
						makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

					else if(wormsarr[1][i]-1==softarr[1][g]&&wormsarr[0][i]==softarr[0][g])//&&y-7<=(posy*25)+50)
						makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

					else if(wormsarr[1][i]+1==softarr[1][g]&&wormsarr[0][i]==softarr[0][g])//&&y+12>=(posy+1)*25+50)
						makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);
					}
				passthroughworm(wormx[i],wormy[i]);
				}
		}
	}

void wormsmovement(clock_t wstart[15],clock_t wend[15],int i,int m[15])
//function controlling worm movement
{
	if(set==1&&wormsarr[0][i]!=-1&&wormsarr[1][i]!=-1)
		speedwormmovement(wstart,wend,i,m,0.5);
	else if(set==2&&wormsarr[0][i]!=-1&&wormsarr[1][i]!=-1)
	{
		if(i<5)
			speedwormmovement(wstart,wend,i,m,0.5);
		else
			speedwormmovement(wstart,wend,i,m,0.3);
		}
	else if(set==3&&wormsarr[0][i]!=-1&&wormsarr[1][i]!=-1)
	{
		if(i<4)
			speedwormmovement(wstart,wend,i,m,0.5);
		else if(i<7)
			speedwormmovement(wstart,wend,i,m,0.3);
		else
			speedwormmovement(wstart,wend,i,m,0.2);
		}
	else if(set==4&&wormsarr[0][i]!=-1&&wormsarr[1][i]!=-1)
	{
		if(i<3)
			speedwormmovement(wstart,wend,i,m,0.5);
		else if(i<6)
			speedwormmovement(wstart,wend,i,m,0.3);
		else if(i<8)
			speedwormmovement(wstart,wend,i,m,0.2);
		else
			speedwormmovement(wstart,wend,i,m,0.4);
		}

	}

int createworm(int i,clock_t wstart[15],int posx,int posy)
//for initial creation of worm

{	int A,B;
	A=random(22)+3;	//worm not in first three rows
	B=random(14)+3;	//worm not in first three columns

	//to check whether this isn't a soft block
	for(int j=0;j<100;j++)
	{
		if(B==softarr[1][j]&&A==softarr[0][j])
			break;
		}

	//to check whether this isn't a solid block

	for(int k=0;k<96;k++)
	{
		if(B==solidarr[1][k]&&A==solidarr[0][k])
			break;
		}

	//to check whether this isn't another worm
	for(int q=0;q<10;q++)
	{	if(q==i)
			continue;
		if(A==wormsarr[0][q]&&B==wormsarr[1][q])
			break;
		}
	for(int g=0;g<count;g++)
		if(A==bombarr[0][g]&&B==bombarr[1][g])
			break;
	if(k!=96||j!=100||q!=10||g!=count||(A==posx&&B==posy))
	{
		return 0;
		}
	wormsarr[0][i]=A;
	wormsarr[1][i]=B;
	wstart[i]=clock();
	wormx[i]=wormsarr[0][i]*25+20;
	wormy[i]=wormsarr[1][i]*25+60;
	if(set==1)
		slowworm(wormx[i],wormy[i]);
	else if(set==2)
	{
		//first five worms are slow next 5 are medium
		if(i<5)
			slowworm(wormx[i],wormy[i]);
		else
			mediumworm(wormx[i],wormy[i]);
		}
	else if(set==3)
	{
		//first 4 worms are slow, next 3 are medium, next three are fast
		if(i<4)
			slowworm(wormx[i],wormy[i]);
		else if(i<7)
			mediumworm(wormx[i],wormy[i]);
		else
			fastworm(wormx[i],wormy[i]);
		}
	else if(set==4)
	{
		//first 3 - slow, next 3 - medium, next 2 - fast, next 2 - passthrough
		if(i<3)
			slowworm(wormx[i],wormy[i]);
		else if(i<6)
			mediumworm(wormx[i],wormy[i]);
		else if(i<8)
			fastworm(wormx[i],wormy[i]);
		else
			passthroughworm(wormx[i],wormy[i]);
		}
	return 1;
	}
void playermovement()
{
	originalplayer(20,60,0);
	char ch;
	int bombx,bomby,l=0,A,B,i,m[10],f=1,win=0,pownum;
	int x=20,y=60,speed=5,posx=0,posy=0,j,k,BOMB=0;
	time_t start[5],end[5];
	clock_t wstart[10],wend[10];
	//making 2 objects of predefined class time_t to measure start and end time

	//making 2 objects of predefined class clock_t to measure
	//start,end for bomb and wstart and wend for worms
	//it has a function which can calculate diff b/w two times(diff may be mini/micro sec,not necessary to be whole no)

	noofworms=10;
	pass=0;
	/*
		win is a flag =1 when all worms are killed and door is found and captured
		m=array to save random motion of worm
		bombx=pixel x of bomb
		bomby=pixel y of bomb
		wormx=pixel x of worm
		wormy=pixel y of worm
		maxbombs= max no of bombs that can be planted
		BOMB=1 when any bomb is active
		x=position  of x pixel
		y=position of y pixel
		posx=coordinate position of x
		posy=coordinate position of y
		lives=number of lives left for player
		brange=distance (number of squares) of the bomb explosion
		pflag=1 if a power up is present on the field, otherwise 0
		powerx=coordinate x position of power up
		powery=coordinate y position of power up
		*/


	//to start time for movement of worms and select a random position
	//no of worms=10
	for(i=0;i<10;i++)
	{	while(createworm(i,wstart,posx,posy)!=1);
		}


	stop=clock();

	while(win!=1)            //Till all conditions for a win are not met
	{
		/*
			UP= 72    	   //C++ has multiple interpretations for these ascii
			DOWN= 80
			LEFT= 75
			RIGHT= 77
			27-ASCII value of escape
			13-ASCII value of enter
			*/

		ch=0;
		time_t t;
		srand((unsigned) time(&t));

		if(flagmap==0)   	//when normal level
		{
			//for timer
			timer();

			/*to check whether the player has come in contact with worm
			  if it does and player does not have shield power up then lives deducted
			  and player is reset to starting position
			  if shield power up is on then the worm is killed
			  */
			for(int q=0;q<10;q++)
			{
				if(wormsarr[0][q]==posx&&wormsarr[1][q]==posy)
				{       if(shield==0)
						updatelives(lives,posx,posy,x,y,brange,maxbombs);
					else
						wormupdate(q);
					}
				}
			//checking whether all worms are killed and player finds door

			//checking if no worms are left + door found + coordinates of door = coordinates of player
			if(noofworms==0&&flagdoor==1&&doorx==posx&&doory==posy)
				win=1;
			}
		else    		//flagmap is 1 for bonus level
		{
			timebonus();
			for(int q=0;q<10;q++)
				if(wormsarr[0][q]==-1 && wormsarr[1][q]==-1)
					while(createworm(q,wstart,posx,posy)!=1);

			if(bonusleveltime==0)
			{
				bonusflag=0;
				cleardevice();
				setbkcolor(WHITE);
				setcolor(RED);
				settextstyle(1,0,4);
				outtextxy(230,215,"TIME'S UP!");
				delay(1000);
				getch();
				break;
				}
			else if(bonusflag==1)
			{
				bonusflag=0;
				setcolor(RED);
				settextstyle(1,0,4);
				outtextxy(200,215,"BONUS LEVEL OVER!");
				delay(1000);
				getch();
				break;
				}
			}

		//for worms

		for(i=0;i<10;i++)
		{
			if(f==1)
				m[i]=rand() % 4;
			/*
			m= 0		upper motion
			m= 1		downward motion
			m= 2		left motion
			m= 3		right motion
				*/
			wormsmovement(wstart,wend,i,m);
			}
		f=0;

		//for bomb

		if(kbhit())
			ch=getch();

		if(maintime==0||ch==27)
		{
			setcolor(RED);
			cleardevice();
			if(ch!=27)
				levelend();
			setbkcolor(WHITE);
			settextstyle(1,0,4);
			outtextxy(230,215,"GAME OVER");
			if(maintime==0)
				HIGH();
			delay(1000);
			//SAVING FOR CONTINUE

			if(ch==27)
			{
				int A=1;
				ifstream fin1("CONT.TXT");
				ofstream fout1("TEMPO.TXT");
				while(!fin1.eof())
				{
					fin1.read((char *) &OBJ,sizeof(OBJ));
					if(strcmp(OBJ.name,NAME)==0)
					{
						OBJ.lives=lives;
						OBJ.score=score;
						OBJ.set=set;
						OBJ.brange=brange;
						OBJ.maxbombs=maxbombs;
						OBJ.level=level;

						A=0;
						}
					fout1.write((char *) &OBJ,sizeof(OBJ));
					}
				fin1.close();
				if(A==1)
				{
					strcpy(OBJ.name,NAME);
					OBJ.lives=lives;
					OBJ.score=score;
					OBJ.set=set;
					OBJ.brange=brange;
					OBJ.maxbombs=maxbombs;
					OBJ.level=level;
					fout1.write((char *) &OBJ,sizeof(OBJ));
					}
				fout1.close();
				remove("CONT.TXT");
				rename("TEMPO.TXT","CONT.TXT");
				}
			//ONCE TIME IS UP REMOVING PLAYER DATA FROM CONTINUE
			else
			{
				ifstream fin2("CONT.TXT");
				ofstream fout2("TEMPO.TXT");
				while(!fin2.eof())
				{
					fin2.read((char *) &OBJ,sizeof(OBJ));
					if(strcmp(OBJ.name,NAME)!=0)
						fout2.write((char *) &OBJ,sizeof(OBJ));
					}
				fin2.close();
				fout2.close();
				remove("CONT.TXT");
				rename("TEMPO.TXT","CONT.TXT");
				}

			set=1;
			score=0;
			brange=1;
			maxbombs=1;
			strlives[0]=51;
			strbrange[0]=strmaxbombs[0]=49;
			lives=3;
			controller(1);
			}

		bombblast(start,end,BOMB,x,y,posx,posy,lives,brange,wormx,wormy,maxbombs);
		//to activate power up is player comes their
		for(int d=0;d<4;d++)
		{
			if(posx==powerx[d]&&posy==powery[d]&&pflag[d]==1)
			{	powerups(lives,brange,maxbombs);
				pflag[d]=-1;
				setcolor(GREEN);
				rectangle(powerx[d]*25+14,powery[d]*25+54,powerx[d]*25+31,powery[d]*25+71);
				setfillstyle(1,GREEN);
				floodfill(powerx[d]*25+20,powery[d]*25+60,GREEN);
				originalplayer(x,y,pass);
				powerx[d]=-1;
				powery[d]=-1;
				}
			}

		//to check if player inside soft block when pass=1
		//player cannot plant bomb while in a soft block
		for(int s=0;s<100;s++)
			if(posx==softarr[0][s]&&posy==softarr[1][s])
				break;
		//player movement

		if(ch==72&&y!=60)
		{
			if((posy-1)*25+50<y-speed-7&&posy*25+50>y-speed-7)    //if we reach next block
			{
				//check for soft block
				for(j=0;j<100;j++)
				{
					if(posy-1==softarr[1][j]&&posx==softarr[0][j]&&pass==0)
						break;
					}
				//check for hard blocks
				for(k=0;k<96;k++)
				{
					if(posy-1==solidarr[1][k]&&posx==solidarr[0][k])
						break;
					}
				//check for bombs
				for(l=0;l<count;l++)
				{
					if(posy-1==bombarr[1][l]&&posx==bombarr[0][l])
						break;
					}
				if(j==100&&k==96&&(x==posx*25+20||x==posx*25+25)&&l==count)
				{
					replaceplayer(x,y);
					for(l=0;l<count;l++)
						if((posy==bombarr[1][l]&&posx==bombarr[0][l])||(posy+1==bombarr[1][l]&&posx==bombarr[0][l]))
						{	bombx=bombarr[0][l]*25+20;
							bomby=bombarr[1][l]*25+60;
							bomb(bombx,bomby);
							}
					if(pass==1)
						for(j=0;j<100;j++)
							if(posy==softarr[1][j]&&posx==softarr[0][j])
								makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
					y-=speed;
					posy--;
					originalplayer(x,y,pass);
					}
				}
			else if(x==posx*25+20||x==posx*25+25)
			{       if(pass==1)
					for(j=0;j<100;j++)
						if(posy==softarr[1][j]&&posx==softarr[0][j])
							makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
				if(flag[count-1]==1)
				{
					replaceplayer(x,y);
					bombx=bombarr[0][count-1]*25+20;
					bomby=bombarr[1][count-1]*25+60;
					bomb(bombx,bomby);
					y-=speed;
					originalplayer(x,y,pass);
					}
				else if(flagdoor==1&&( (posx==doorx&&posy==doory)||(posx-1==doorx&&posy==doory)||(posx+1==doorx&&posy==doory)||(posx==doorx&&posy-1==doory)||(posx==doorx&&posy+1==doory)))
				{
					replaceplayer(x,y);
					door((doorx*25)+10,(doory*25)+50);
					y-=speed;
					originalplayer(x,y,pass);
					}

				else
				{
					replaceplayer(x,y);
					y-=speed;
					originalplayer(x,y,pass);
					}
				}
			}
		else if(ch==80&&y!=460)
		{
			if((posy+1)*25+50<=y+speed+10&&(posy+2)*25+50>=y+speed+10)           //if we reach next block
			{
				//check for soft block
				for(j=0;j<100;j++)
				{
					if(posy+1==softarr[1][j]&&posx==softarr[0][j]&&pass==0)
						break;
					}
				//check for hard blocks
				for(k=0;k<96;k++)
				{
					if(posy+1==solidarr[1][k]&&posx==solidarr[0][k])
						break;
					}
				//check for bombs
				for(l=0;l<count;l++)
				{
					if(posy+1==bombarr[1][l]&&posx==bombarr[0][l])
						break;
					}
				if(j==100&&k==96&&(x==posx*25+20||x==posx*25+25)&&l==count)
				{
					replaceplayer(x,y);
					for(l=0;l<count;l++)
						if((posy==bombarr[1][l]&&posx==bombarr[0][l])||(posy-1==bombarr[1][l]&&posx==bombarr[0][l]))
						{	bombx=bombarr[0][l]*25+20;
							bomby=bombarr[1][l]*25+60;
							bomb(bombx,bomby);
							}
					if(pass==1)
						for(j=0;j<100;j++)
							if(posy==softarr[1][j]&&posx==softarr[0][j])
								makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
					y+=speed;
					posy++;
					originalplayer(x,y,pass);
					}
				}
			else if(x==posx*25+20||x==posx*25+25)
			{       if(pass==1)
					for(j=0;j<100;j++)
						if(posy==softarr[1][j]&&posx==softarr[0][j])
							makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
				if(flag[count-1]==1)
				{
					replaceplayer(x,y);
					bombx=bombarr[0][count-1]*25+20;
					bomby=bombarr[1][count-1]*25+60;
					bomb(bombx,bomby);
					y+=speed;
					originalplayer(x,y,pass);
					}
				else if(flagdoor==1&&( (posx==doorx&&posy==doory)||(posx-1==doorx&&posy==doory)||(posx+1==doorx&&posy==doory)||(posx==doorx&&posy-1==doory)||(posx==doorx&&posy+1==doory)))
				{
					replaceplayer(x,y);
					door(doorx*25+10,doory*25+50);
					y+=speed;
					originalplayer(x,y,pass);
					}
				else
				{
					replaceplayer(x,y);
					y+=speed;
					originalplayer(x,y,pass);
					}
				}
			}
		else if(ch==75&&x!=20)
		{
			if((posx-1)*25+10<x-speed-7&&posx*25+10>x-speed-7)           //if we reach next block
			{
				//check for soft block
				for(j=0;j<100;j++)
				{
					if(posx-1==softarr[0][j]&&posy==softarr[1][j]&&pass==0)
						break;
					}
				//check for hard blocks
				for(k=0;k<96;k++)
				{
					if(posx-1==solidarr[0][k]&&posy==solidarr[1][k])
						break;
					}
				//check for bombs
				for(l=0;l<count;l++)
				{
					if(posx-1==bombarr[0][l]&&posy==bombarr[1][l])
						break;
					}
				if(j==100&&k==96&&(y==posy*25+60)&&l==count)
				{
					replaceplayer(x,y);
					for(l=0;l<count;l++)
						if((posy==bombarr[1][l]&&posx==bombarr[0][l])||(posy==bombarr[1][l]&&posx+1==bombarr[0][l]))
						{	bombx=bombarr[0][l]*25+20;
							bomby=bombarr[1][l]*25+60;
							bomb(bombx,bomby);
							}
					if(pass==1)
						for(j=0;j<100;j++)
							if(posy==softarr[1][j]&&posx==softarr[0][j])
								makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
					x-=speed;
					posx--;
					originalplayer(x,y,pass);
					}
				}
			else
			{       if(pass==1)
					for(j=0;j<100;j++)
						if(posy==softarr[1][j]&&posx==softarr[0][j])
							makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
				if(flag[count-1]==1)
				{
					replaceplayer(x,y);
					bombx=bombarr[0][count-1]*25+20;
					bomby=bombarr[1][count-1]*25+60;
					bomb(bombx,bomby);
					x-=speed;
					originalplayer(x,y,pass);
					}
				else if(flagdoor==1&&( (posx==doorx&&posy==doory)||(posx-1==doorx&&posy==doory)||(posx+1==doorx&&posy==doory)||(posx==doorx&&posy-1==doory)||(posx==doorx&&posy+1==doory)))
				{
					replaceplayer(x,y);
					door(doorx*25+10,doory*25+50);
					x-=speed;
					originalplayer(x,y,pass);
					}
				else
				{
					replaceplayer(x,y);
					x-=speed;
					originalplayer(x,y,pass);
					}
				}
			}
		else if(ch==77&&x!=625)
		{
			if((posx+1)*25+10<=x+speed+5&&(posx+2)*25+10>=x+speed+5)           //if we reach next block
			{
				//check for soft block
				for(j=0;j<100;j++)
				{
					if(posx+1==softarr[0][j]&&posy==softarr[1][j]&&pass==0)
						break;
					}
				//check for hard blocks
				for(k=0;k<96;k++)
				{
					if(posx+1==solidarr[0][k]&&posy==solidarr[1][k])
						break;
					}
				//check for bombs
				for(l=0;l<count;l++)
				{
					if(posx+1==bombarr[0][l]&&posy==bombarr[1][l])
						break;
					}
				if(j==100&&k==96&&(y==posy*25+60)&&l==count)
				{
					replaceplayer(x,y);
					for(l=0;l<count;l++)
						if((posy==bombarr[1][l]&&posx==bombarr[0][l])||(posy==bombarr[1][l]&&posx-1==bombarr[0][l]))
						{	bombx=bombarr[0][l]*25+20;
							bomby=bombarr[1][l]*25+60;
							bomb(bombx,bomby);
							}
					if(pass==1)
						for(j=0;j<100;j++)
							if(posy==softarr[1][j]&&posx==softarr[0][j])
								makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
					x+=speed;
					posx++;
					originalplayer(x,y,pass);
					}
				}
			else
			{       if(pass==1)
					for(j=0;j<100;j++)
						if(posy==softarr[1][j]&&posx==softarr[0][j])
							makesoftbox(softarr[0][j]*25+10,softarr[1][j]*25+50);
				if(flag[count-1]==1)
				{
					replaceplayer(x,y);
					bombx=bombarr[0][count-1]*25+20;
					bomby=bombarr[1][count-1]*25+60;
					bomb(bombx,bomby);
					x+=speed;
					originalplayer(x,y,pass);
					}
				else if(flagdoor==1&&( (posx==doorx&&posy==doory)||(posx-1==doorx&&posy==doory)||(posx+1==doorx&&posy==doory)||(posx==doorx&&posy-1==doory)||(posx==doorx&&posy+1==doory)))
				{
					replaceplayer(x,y);
					door(doorx*25+10,doory*25+50);
					x+=speed;
					originalplayer(x,y,pass);
					}
				else
				{
					replaceplayer(x,y);
					x+=speed;
					originalplayer(x,y,pass);
					}
				}
			}
		/*
			checking if space is entered bomb planted only if
			(i)	count!=maxbombs
			(ii)	he isn't on a soft block when pass is active
			(iii)	he isn't on the door
			*/
		else if(ch==' '&&count!=maxbombs&&s==100&&(!(posx==doorx&&posy==doory&&flagdoor==1)))
		{
			replaceplayer(x,y);
			bombarr[0][count]=posx;
			bombarr[1][count]=posy;
			bombx=bombarr[0][count]*25+20;
			bomby=bombarr[1][count]*25+60;
			bomb(bombx,bomby);
			start[count]=time(NULL);
			count++;
			BOMB=1;
			flag[count-1]=1;
			originalplayer(x,y,pass);
			}

		//is their is movement of player ch=0, then
		//it will make soft blocks if their around the player if he
		//is on soft block when power up is active
		if(ch!=0)
		{
			for(int g=0;g<100;g++)
			{      	if(posx==softarr[0][g]&&posy==softarr[1][g])
					makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

				else if(posx-1==softarr[0][g]&&posy==softarr[1][g])//&&x-7<=(posx*25)+10)
					makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

				else if(posx+1==softarr[0][g]&&posy==softarr[1][g])//&&x+5>=(posx+1)*25+10)
					makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

				else if(posy-1==softarr[1][g]&&posx==softarr[0][g])//&&y-7<=(posy*25)+50)
					makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);

				else if(posy+1==softarr[1][g]&&posx==softarr[0][g])//&&y+12>=(posy+1)*25+50)
					makesoftbox(softarr[0][g]*25+10,softarr[1][g]*25+50);
				}
			if(flagdoor==1&&( (posx==doorx&&posy==doory)||(posx-1==doorx&&posy==doory)||(posx+1==doorx&&posy==doory)||(posy-1==doory&&posx==doorx)||(posy+1==doory&&posx==doorx)))
			{
				door(doorx*25+10,doory*25+50);
				}
			originalplayer(x,y,pass);
			}
		}
	if(win==1)
	{
		updatescore(6);
		levelend();
		}
	}

void controls() 	//to display page of controls
{
	setlinestyle(0,0,3);
	setcolor(BLUE);
	rectangle(1,1,getmaxx()-1,getmaxy()-1);
	setfillstyle(9,LIGHTBLUE);
	floodfill(5,5,BLUE);
	setlinestyle(0,0,3);
	settextstyle(7,0,7);
	setcolor(RED);
	outtextxy(150,20,"CONTROLS");
	outtextxy(130,20,"_________");
	settextstyle(1,0,2);
	setcolor(WHITE);
	outtextxy(50,130,"UP    : ");
	outtextxy(50,170,"DOWN  : ");
	outtextxy(50,210,"LEFT  : ");
	outtextxy(50,250,"RIGHT : ");
	outtextxy(50,290,"BOMB  : ");
	setcolor(GREEN);
	outtextxy(150,130,"ARROW UP");
	outtextxy(150,170,"ARROW DOWN");
	outtextxy(150,210,"ARROW LEFT");
	outtextxy(150,250,"ARROW RIGHT");
	outtextxy(150,290,"SPACE BAR");

	//for graphic

	setcolor(WHITE);
	rectangle(450,130,500,180);
	rectangle(400,180,450,230);
	rectangle(450,180,500,230);
	rectangle(500,180,550,230);
	rectangle(350,270,600,310);

	setfillstyle(1,DARKGRAY);
	floodfill(460,150,WHITE);
	floodfill(410,190,WHITE);
	floodfill(460,190,WHITE);
	floodfill(510,190,WHITE);
	floodfill(360,280,WHITE);


	outtextxy(425,280,"SPACE BAR");

	line(475,140,475,165);
	line(475,140,465,150);
	line(475,140,485,150);
	line(410,205,440,205);
	line(410,205,420,195);
	line(410,205,420,215);
	line(475,195,475,220);
	line(475,220,465,210);
	line(475,220,485,210);
	line(510,205,540,205);
	line(540,205,530,195);
	line(540,205,530,215);

	settextstyle(0,0,1);
	outtextxy(50,450,"PRESS ESCAPE TO EXIT");

	char ch=getch();
	while(ch!=27)
		ch=getch();
	if(ch==27)
		controller(1);
	}

void highscore()	//FUNCTION TO SEE HIGH SCORE
{
	setlinestyle(0,0,3);
	setcolor(WHITE);
	rectangle(1,1,638,478);
	setfillstyle(9,LIGHTGREEN);
	floodfill(5,5,WHITE);
	setlinestyle(0,0,0);
	settextstyle(0,0,5);
	outtextxy(100,50,"HIGH SCORES");
	outtextxy(100,60,"___________");
	settextstyle(9,0,1);
	ifstream fin("HIGHSCOR.TXT");
	int y=150;
	for(int i=0;i<5;i++)
	{
		fin.read((char *) &obj,sizeof(obj));
		setcolor(BLUE);
		obj.display(y);
		y+=40;
		}
	fin.close();
	setcolor(WHITE);
	settextstyle(0,0,1);
	outtextxy(50,450,"PRESS ESCAPE TO EXIT");

	char ch=getch();
	while(ch!=27)
		ch=getch();
	if(ch==27)
		controller(1);

	}

void CONTINUE()		//TO CONTINUE THE GAME
{
	ifstream fin("CONT.TXT");
	int M=0;
	long tempscr;
	while(!fin.eof())
	{
		fin.read((char *) &OBJ,sizeof(OBJ));
		if(strcmp(OBJ.name,NAME)==0)
		{
			M=1;
			maxbombs=OBJ.maxbombs;
			brange=OBJ.brange;
			lives=OBJ.lives;
			set=OBJ.set;
			score=OBJ.score;
			level=OBJ.level;

			strlives[0]=lives+48;
			strbrange[0]=brange+48;
			strmaxbombs[0]=maxbombs+48;
			tempscr=score;
			for(int cnt=1;tempscr>=10;cnt*=10)
				tempscr/=10;
			tempscr=score;
			int i=0;
			while(cnt)
			{	strscore[i]=tempscr/cnt+48;
				tempscr%=cnt;
				cnt/=10;
				i++;
				}
			strscore[i]='\0';
			break;
			}
		}
	if(M==0)
	{
		set=1;
		score=0;
		brange=1;
		maxbombs=1;
		strlives[0]=51;
		strbrange[0]=strmaxbombs[0]=49;
		lives=3;
		}
	fin.close();
	controller(2);
	}

void controller(int a=0)	    //FUNCTION TO CONTROL THE PROGRAM AND COVER PAGE
{                              //a=0 when open first time,a=1 when called from
				//from a function and a=2 when continue
	int choice=1;
	char ch;

	int gdriver = DETECT, gmode;
	initgraph(&gdriver, &gmode, "C:\\TC\\BGI");
	cleardevice();

	if(a==0)
	{
		animation(NAME);
		cleardevice();
		closegraph();
		initgraph(&gdriver, &gmode, "C:\\TC\\BGI");
		}
	cover();
	int i=10000,j=5000,l=20;
	if(a!=2)
	{
		while(ch!=13)
		{
			ch=0;
			if(!kbhit())
			{
				mainbackground(i,j,l);
				if(j<2000)
				{
					i=10000;
					j=5000;
					l=20;
					}
				}
			else
				ch=getch();
			if(ch==72&&choice!=1)
			{
				choice--;
				switch(choice)
				{
					case 1: setcolor(WHITE);
						outtextxy(300,330,"- C O N T I N U E");
						setcolor(YELLOW);
						outtextxy(300,300,"- P L A Y");

								break;
					case 2: setcolor(WHITE);
						outtextxy(300,360,"- H I G H  S C O R E");
						setcolor(YELLOW);
						outtextxy(300,330,"- C O N T I N U E");

								break;
					case 3: setcolor(WHITE);
						outtextxy(300,390,"- C O N T R O L S");
						setcolor(YELLOW);
						outtextxy(300,360,"- H I G H  S C O R E");

								break;
					case 4: setcolor(WHITE);
						outtextxy(300,420,"- E X I T");
						setcolor(YELLOW);
						outtextxy(300,390,"- C O N T R O L S");
					}
				}
			else if(ch==80&&choice!=5)
			{
				choice++;
				switch(choice)
				{
					case 2: setcolor(YELLOW);
						outtextxy(300,330,"- C O N T I N U E");
						setcolor(WHITE);
						outtextxy(300,300,"- P L A Y");

								break;
					case 3:	setcolor(YELLOW);
						outtextxy(300,360,"- H I G H  S C O R E");
						setcolor(WHITE);
						outtextxy(300,330,"- C O N T I N U E");

								break;
					case 4: setcolor(YELLOW);
						outtextxy(300,390,"- C O N T R O L S");
						setcolor(WHITE);
						outtextxy(300,360,"- H I G H  S C O R E");

								break;
					case 5: setcolor(YELLOW);
						outtextxy(300,420,"- E X I T");
						setcolor(WHITE);
						outtextxy(300,390,"- C O N T R O L S");
					}
				}
			}
		}
	switch(choice)
	{
		case 1: cleardevice();
			if(a!=2)
			{
				difficultylevel(set);
				level=0;
				}
			cleardevice();
			setcolor(DARKGRAY);
			settextstyle(0,0,0);
			setlinestyle(0,0,0);
			for(;set<=4;set++)
			{
				for(;level<=2;level++)        //j<num   where num is the number
				{                            //of levels per set, excluding bonus
					char ch[2];
					ch[0]=set+48;
					ch[1]='\0';
					outtextxy(250,10,"SET:");
					outtextxy(300,10,ch);
					if(level!=0)
						score+=200;
					shield=0;
					levelstart();
					char ch1[2];
					ch1[0]=level+49;
					ch1[1]='\0';
					outtextxy(350,10,"LEVEL:");
					outtextxy(400,10,ch1);
					map();
					cleardevice();
					}
				level=0;
				outtextxy(200,10,"BONUS");
				bonus();
				cleardevice();
				}
							break;

		case 2: cleardevice();
			CONTINUE();
							break;

		case 3: cleardevice();
			highscore();
							break;

		case 4: cleardevice();
			controls();
							break;

		case 5: cleardevice();
			setbkcolor(RED);
			delay(500);
			exit(0);
		}
	closegraph();
	getch();
	}
void main()
{
	controller();
	}


