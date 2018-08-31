void bomb()		//FOR BOMB,voice of blast
{
	for(int i=40,j=0;i>=30;i++)
	{
		if(i==60||j==1)
		{
			j=1;
			i-=2;
			}
		sound(i);
		delay(3);
		nosound();         //after the termination of program, if nosound is
				   //is their sound ends,if not we here a beep
		}
	}

void live()		//when player dies
{
	for(int i=200;i>=100;i--)
	{
		sound(i);
		delay(1);
		nosound();
		}
	}

void powerup()		//FOR POWER UP
{
	for(int i=100;i<=250;i++)
	{
		sound(i);
		delay(1);
		nosound();
		}
	}

void levelend()		//WHEN LEVEL ENDS
{
	int i,j,k,l;
	i=2000;
	j=200;
	k=500;
	l=6000;
	for(;i<=5000;i+=30,j+=2,k-=2,l-=30)
	{
		sound(i);
		delay(5);
		nosound();
		sound(j);
		delay(10);
		nosound();
		sound(k);
		delay(10);
		nosound();
		sound(l);
		delay(5);
		nosound();
		}
	}

void levelstart() 	//WHEN LEVEL STARTS
{
	int i,j,k,l,m,n;
	i=150;
	j=200;
	k=500;
	m=1000;
	for(;i<=300;i+=10,j+=5,k-=10,m-=10)
	{
		sound(i);
		delay(10);
		nosound();
		sound(j);
		delay(5);
		nosound();
		sound(k);
		delay(10);
		nosound();
		sound(m);
		delay(10);
		nosound();
		}
	}

void mainbackground(int &i,int &j,int &l) 	//FOR MAIN BACKGROUND
{
	int k=0;
	for(;j>=2000;j+=100,l+=5,i-=100)
	{
		if(kbhit())
			break;
		if(j==9000 || k==1)
		{
			k=1;
			j-=200;
			}
		sound(j);
		delay(10);
		nosound();
		sound(l);
		delay(10);
		nosound();
		sound(i);
		delay(10);
		nosound();
		}
	}
void walking()  		//FOR WALKING,IN ANIMATION
{
	for(int i=20,j=0;i>=0;i++)
	{
		if(i==30||j==1)
		{
			i-=2;
			j=1;
			}
		sound(i);
		delay(5);
		nosound();
		}
	}
void bomba()  		//BOMB SOUND IN ANIMATION
{
	for(int i=100;i>=20;i--)
	{
		sound(i);
		delay(1);
		nosound();
		if(i<=60)
		{
			sound(i);
			delay(20);
			nosound();
			}
		if(i<=40)
		{
			sound(i);
			delay(25);
			nosound();
			}
		}
	}
