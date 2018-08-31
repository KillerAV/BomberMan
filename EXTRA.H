#include"LARGEWOR.H"
void difficultylevel(int &set)		//to check the difficulty level
{
	char ch;
	setcolor(WHITE);
	setlinestyle(0,0,3);
	rectangle(1,1,getmaxx()-1,getmaxy()-1);
	setfillstyle(9,BROWN);
	floodfill(100,100,WHITE);
	settextstyle(1,0,4);
	outtextxy(150,25,"SELECT DIFFICULTY");
	outtextxy(150,30,"_________________");
	rectangle(350,100,550,250);
	rectangle(50,300,250,450);
	rectangle(350,300,550,450);
	setcolor(YELLOW);
	rectangle(50,100,250,250);
	setcolor(WHITE);
	line(50,140,250,140);
	line(50,340,250,340);
	line(350,140,550,140);
	line(350,340,550,340);

	//to make graphics in boxes

	//for beginner box
	largeslowworm(150,180);
	//for amateur box
	largeslowworm(410,180);
	largemediumworm(500,190);
	//for expert box
	largeslowworm(90,380);
	largemediumworm(150,390);
	largefastworm(210,390);
	//for invincible box
	largeslowworm(380,380);
	largemediumworm(435,390);
	largefastworm(483,390);
	largepassthroughworm(525,390);


	settextstyle(1,0,2);
	setcolor(LIGHTGREEN);
	outtextxy(100,110,"BEGINNER");
	outtextxy(100,310,"EXPERT");
	outtextxy(400,110,"AMATEUR");
	outtextxy(400,310,"INVINCIBLE");
	while(ch!=13)
	{
		ch=getch();
		if(ch==72&&set!=1&&set!=2)
		{
			set-=2;
			setcolor(WHITE);
			rectangle(set*300-250,300,set*300-50,450);
			setcolor(YELLOW);
			rectangle(set*300-250,100,set*300-50,250);
			}
		else if(ch==80&&set!=3&&set!=4)
		{
			set+=2;
			setcolor(WHITE);
			rectangle(set*300-850,100,set*300-650,250);
			setcolor(YELLOW);
			rectangle(set*300-850,300,set*300-650,450);
			}
		else if(ch==75&&set!=1&&set!=3)
		{
			set--;
			setcolor(WHITE);
			rectangle(350,set*100,550,set*100+150);
			setcolor(YELLOW);
			rectangle(50,set*100,250,set*100+150);
			}
		else if(ch==77&&set!=2&&set!=4)
		{
			set++;
			setcolor(WHITE);
			rectangle(50,set*100-100,250,set*100+50);
			setcolor(YELLOW);
			rectangle(350,set*100-100,550,set*100+50);
			}
		}
	setlinestyle(0,0,0);
	}

void bomb(int a,int b)          //TO MAKE BOMBS
{
	setcolor(DARKGRAY);
	circle(a,b,7);
	setfillstyle(1,DARKGRAY);
	floodfill(a,b,DARKGRAY);
	setcolor(RED);
	line(a-6,b-6,a-9,b-9);
	}

void replacebomb(int a,int b)	//TO REPLACE BOMB
{
	setcolor(GREEN);
	circle(a,b,7);
	setfillstyle(1,GREEN);
	floodfill(a+1,b+1,GREEN);
	line(a-6,b-6,a-9,b-9);
	}

void makesoftbox(int a,int b)	//TO MAKE SOFT BLOCKS
{
	setcolor(WHITE);
	rectangle(a,b,a+25,b+25);
	setfillstyle(1,LIGHTGRAY);
	floodfill(a+2,b+2,WHITE);
	floodfill(a+2,b+12,WHITE);
	floodfill(a+23,b+12,WHITE);
	floodfill(a+23,b+23,WHITE);
	setcolor(DARKGRAY);
	line(a+1,b+8,a+24,b+8);
	line(a+1,b+16,a+24,b+16);
	line(a+14,b+8,a+14,b+16);
	}

void door(int a,int b)		//Function to make door
{
	setcolor(WHITE);
	rectangle(a,b,a+25,b+25);
	setfillstyle(1,BROWN);
	floodfill(a+2,b+2,WHITE);
	floodfill(a+23,b+23,WHITE);
	floodfill(a+2,b+23,WHITE);
	floodfill(a+23,b+2,WHITE);
	line(a+12,b,a+12,b+25);
	circle(a+14,b+13,1);
	}

void drawpowerup(int x,int y)
{
	setcolor(WHITE);
	rectangle(x,y,x+17,y+17);
	setfillstyle(1,LIGHTCYAN);
	floodfill(x+5,y+5,WHITE);
	settextstyle(3,0,0);
	setcolor(DARKGRAY);
	setusercharsize(1,3,1,4);

	/*
		this function is used when you font size is 0 and you want
		to increase the width or height of text. ratio of first and
		second parameter is width so if 1/3 then width is less and
		ratio of third and fourth parameter is height
		*/

	outtextxy(x+3,y+4,"UP");
	settextstyle(2,0,5);
	}

void bexplode(int a,int b)       //to create the explosions
{
	setcolor(BROWN);
	circle(a+2,b+2,9);
	setfillstyle(1,BROWN);
	floodfill(a+3,b+3,BROWN);
	}

void repexplode(int a,int b)     //to replace bomb visual
{
	setcolor(GREEN);
	rectangle(a-10,b-10,a+15,b+15);
	setfillstyle(1,GREEN);
	floodfill(a+3,b+3,GREEN);
	}
void drawshield(int x,int y) 		//to draw powerup shield
{	setcolor(DARKGRAY);

	/*(x,y)= center of ellipse
	  (7,7)= horizontal axis and vertical axis
		 major axis>minor axis so if (6,8) is taken then
		 horizontal<verticle axis so ellipse has major axis
		 as verticle axis that is y axis. so this decides
		 shape of ellipse
	  (270,330)= angles b/w which ellipse is required. here
		     ellipse is in fourth quad. this means it takes
		     that curve of ellipse that lies within angle
		     range not entire ellipse
		*/

	ellipse(x,y,270,330,7,7);
	ellipse(x+12,y,210,270,7,7);
	putpixel(x+6,y+18,8);
	putpixel(x+5,y+17,8);
	putpixel(x+4,y+16,8);
	putpixel(x+3,y+15,8);
	putpixel(x+2,y+14,8);
	putpixel(x+1,y+13,8);
	putpixel(x+1,y+12,8);
	putpixel(x,y+11,8);
	putpixel(x,y+10,8);
	putpixel(x,y+9,8);
	putpixel(x,y+8,8);

	putpixel(x+7,y+17,8);
	putpixel(x+8,y+16,8);
	putpixel(x+9,y+15,8);
	putpixel(x+10,y+14,8);
	putpixel(x+11,y+13,8);
	putpixel(x+11,y+12,8);
	putpixel(x+12,y+11,8);
	putpixel(x+12,y+10,8);
	putpixel(x+12,y+9,8);
	putpixel(x+12,y+8,8);
	for(int i=y+5;i<y+18;i++)
		putpixel(x+6,i,CYAN);
	for(i=y+6;i<y+17;i++)
	{	putpixel(x+5,i,CYAN);
		putpixel(x+7,i,CYAN);
		}
	for(i=y+7;i<y+16;i++)
	{	putpixel(x+4,i,CYAN);
		putpixel(x+8,i,CYAN);
		}
	for(i=y+7;i<y+15;i++)
	{	putpixel(x+3,i,CYAN);
		putpixel(x+9,i,CYAN);
		}
	for(i=y+8;i<y+14;i++)
	{	putpixel(x+2,i,CYAN);
		putpixel(x+10,i,CYAN);
		}
	for(i=y+8;i<y+12;i++)
	{	putpixel(x+1,i,CYAN);
		putpixel(x+11,i,CYAN);
		}
	}

