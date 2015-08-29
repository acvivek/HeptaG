#include <stdio.h>
#include <ctype.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <setjmp.h>
#include "mouse.h"
jmp_buf jmpbuf;		//for goto function
/*Positions of the individual icons*/
int bx=10,by=10;
int cx=500,cy=10;
int ex=500,ey=300;
int nx=10,ny=300;
int ax=250,ay=75;
int sx=250,sy=250;
int gx=100,gy=175;
int hx=400,hy=175;
int game_status=0;
/*Variables used by all games functions*/
int life=3;
int score=0;
int level=1;
/*Common functions num to char*/
char * conv(int n)
{
	int x,l=0;
	char a[10];
	if(n==0)
	{
		a[l++]='0';
		a[l]='\0';
		return a;
	}
	while(n>0)
	{
		x=n%10;
		a[l++]=x+48;
		n=n/10;
	}
	a[l]='\0';
	strrev(a);
	a[l]='\0';
	return a;
}
/*Display score ,level,lives*/
void disp_stats()
{
	static int l,s,v,c=0;
	char * m,x[5],y[5],z[5];
	int i;
	if(c!=0)
	{
		setcolor(BLACK);
		m=conv(s);
		for(i=0;i<strlen(m);i++)
			x[i]=*(m+i);
		x[i]='\0';
		m=conv(l);
		for(i=0;i<strlen(m);i++)
			y[i]=*(m+i);
		y[i]='\0';
		m=conv(v);
		for(i=0;i<strlen(m);i++)
			z[i]=*(m+i);
		z[i]='\0';
		outtextxy(500,30,x);
		outtextxy(500,230,y);
		outtextxy(500,430,z);
	}
	setcolor(WHITE);
	outtextxy(500,10,"Score");
	outtextxy(500,210,"Lives");
	outtextxy(500,410,"Level");
	m=conv(score);
	for(i=0;i<strlen(m);i++)
		x[i]=*(m+i);
	x[i]='\0';
	m=conv(life);
	for(i=0;i<strlen(m);i++)
		y[i]=*(m+i);
	y[i]='\0';
	m=conv(level);
	for(i=0;i<strlen(m);i++)
		z[i]=*(m+i);
	z[i]='\0';
	outtextxy(500,30,x);
	outtextxy(500,230,y);
	outtextxy(500,430,z);
	l=life,s=score,v=level;
	c++;
}

/*Variables for bricks game dir,no. of bricks,paddle pos*/
int up_down=0;
int left_right=0;
int test_over=0;
int prev_mx,prev_bx,prev_by;

/*Indicates that a brick has been hit by characteristic sounds*/
void music()
{
	int a[]={0,425,105,280,340,120};
	for(int i=1;i<=5;i++)
	{
		sound(a[i]);
		delay(25);
		nosound();
	}
}
/*Class Defintion For Brick Class
Draws The Bricks Checks For Collision*/
class brick
{
	int a[30][30];         //Holds color attribute of brick
	int r;                 //number of brick in row & col
	int c;
public:
	brick()
	{
		r=5;
		c=10;
	}
	void set_array();
	void check_brick();
	void draw_brick(int,int);
	void brick_collision(int,int);
	int get_r()
	{
		return r;
	}
	int get_c();
};
brick b;
/*Class Defn For Ball: Checks Collision & Moves The BALL*/
class ball
{
	int ball_x;
	int ball_y;
public:
	ball()
	{
	   ball_x=205;      //initial ball position
	   ball_y=290;
		     //No Deflection
	}
	ball(int m,int n)   //Param Constructor For Each Instance of the ball
	{
		ball_x=m;
		ball_y=n;
	}
	void draw_ball(int,int,int);
	void erase_ball(int);
	void move_ball();
	void collision();
};
/*Class Defn for Paddle Moves The Paddle & Checks Collision with Ball*/
class paddle
{
	int pad_x;
	int pad_y;
public:
	paddle()
	{
	pad_y=300;
	}
	paddle(int mx)  //For Every instance of the paddle
	{
	pad_x=mx;
	}
	void pad_erase();
	void pad_show(int);
	void get_pos();
	int get_mx()
	{
	return pad_x;
	}
	void pad_collision(int);
};

/*MEMBER FUNCTIONS OF CLASS BALL*/
/*Takes Care that the Ball is Never Stationary Once the SpaceBar is Pressed*/
void ball::draw_ball(int x,int y,int r)
{
	erase_ball(r);
	setcolor(BROWN);
	for(int i=r;i>=0;i--)
	circle(x,y,i);
	prev_bx=x;
	prev_by=y;
	b.check_brick();
}
void ball::erase_ball(int r)
{
	setcolor(BLACK);
	for(int i=r;i>=0;i--)
	circle(prev_bx,prev_by,i);
}
void ball::move_ball()
{
	if(up_down==0 && left_right==0)
	{
	       ball_x-=5;
	       ball_y-=10;
	       draw_ball(ball_x,ball_y,10);
	       collision();      //check collision after every ball movement
	       return ;

	}
	if(up_down==0 && left_right==1)
	{
		ball_x+=5;
		ball_y-=10;
		draw_ball(ball_x,ball_y,10);
		collision();      //check collision after every ball movement
		return ;
	}
	if(up_down==1 && left_right==0)
	{
		ball_x-=5;
		ball_y+=10;
		draw_ball(ball_x,ball_y,10);
		collision();      //check collision after every ball movement
		return ;
	}
	if(up_down==1 && left_right==1)
	{
		ball_x+=5;
		ball_y+=10;
		draw_ball(ball_x,ball_y,10);
		collision();      //check collision after every ball movement
		return ;
	}
}
/*
*** CORE FUNCTION
Checks For collision of Ball with any other object
Checks The Boundary Limits of The Ball
Checks For collision with Brick
Checks for collision with Paddle
*/
void ball::collision()
{
	int px,choice=0;
	asm mov ax,0x03;
	asm INT 0x33;
	asm mov mx,cx;
	paddle p(mx);
	if(ball_x-10<=0 && ball_y+10<=300)	//Left Boundary
	{
		left_right=1;
		return;
	}
	else if(ball_x+10>=40*b.get_c() && ball_y+10<=300)   //Right Boundary
	{
		left_right=0;
		return;
	}
	else if(ball_y-10<=0)       //Upper Limit of the Arena
	{
	    up_down=1;
	    return ;
	}
	else if(ball_y+10>=400)  //Player Has Lost Track of the Ball
	{
		life--;
		score-=2;
		if(life==0)
		{
			setcolor(BLACK);
			outtextxy(400,450,"Press Space to resume");
			setcolor(RED);
			settextstyle(1,0,1);
			outtextxy(200,200,"Game Over");
			delay(5000);
			longjmp(jmpbuf,1);
		}
		else
		{
			ball_x=205;
			ball_y=300;
			up_down=0;
			b.check_brick();
			setcolor(WHITE);
			outtextxy(400,450,"Press Space to resume");
			while(choice!=32)
			{
			asm mov ah,0x07;
			asm INT 0x21;
			asm mov ah,0x00;
			asm mov choice,ax;
			}
			setcolor(BLACK);
			outtextxy(400,450,"Press Space to resume");
			return;
		}
	}
	else if(ball_y+10==300)
	{
		p.pad_collision(ball_x);
		return;
	}
	else if(ball_y-10<=20*b.get_r())
	{
		b.brick_collision(ball_x,ball_y);
		return;
	}
}
/*MEMBER FUNCTIONS OF PADDLE
  Displays Paddle*/
void paddle::pad_erase()
{
       setcolor(BLACK);
       int x=prev_mx-20;
       int y=pad_y;
       setfillstyle(SOLID_FILL,BLACK);
       bar(x,y+1,x+40,y+2);
}
void paddle::pad_show(int mx)
{
	setcolor(GREEN);
	pad_x=mx;
	int x=pad_x-20;
	int y=pad_y;
	prev_mx=pad_x;
	setfillstyle(SOLID_FILL,GREEN);
	bar(x,y+1,x+40,y+2);
}
/*Moves the Paddle Based on Mouse Movement*/
void paddle::get_pos()
{
	int mx;
	ball ob;
	asm mov ax,0x03;
	asm INT 0x33;
	asm mov mx,cx;
	if(mx==pad_x) return;
	pad_erase();
	delay(10);
	pad_show(mx);
}
/*Checks collision with Paddle
Paddle is divided into 3 segments Left,Middle,Right &
 Moves Deflectd the Ball Based in these regions*/
void paddle::pad_collision(int ball_x)
{
	pad_show(get_mx());
	if(abs(pad_x-ball_x)<20)
		up_down=0;
}
/*MEMBER FUNCTIONS OF CLASS BRICK*/
/*Sets all the bricks & their color*/
int brick::get_c()
{
	return c;
}
void brick::set_array()
{
	int i,j;
	static int count=1;
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			count++;
			a[i][j]=count%4+1;
		}
	}
}
/*Checks Which Bricks have been Broken & updates*/
void brick::check_brick()
{
	int i,j;
	if(test_over==r*c) game_status=1;
	for(i=0;i<r;i++)
		for(j=0;j<c;j++)
				draw_brick(i,j);
}
/*Draws the bricks on the contents of the array
	1 RED 2 BLUE 3WHITE 0 BRICK  BROKEN*/
void brick::draw_brick(int row,int col)
{
	int x,y,i,j;
	setcolor(WHITE);
	rectangle(0,0,c*40,400);
	x=col*40;    //Mapping Array to Real Coordinates
	y=row*20;
	if(a[row][col]==0)
	{
		setfillstyle(SOLID_FILL,BLACK);
		a[row][col]=-1;
	}
	else if(a[row][col]==-1) return;
	else if(a[row][col]==1)
		setfillstyle(SOLID_FILL,RED);
	else if(a[row][col]==2)
		setfillstyle(SOLID_FILL,BLUE);
	else if(a[row][col]==3)
		setfillstyle(SOLID_FILL,WHITE);
	else if(a[row][col]==4)
		setfillstyle(SOLID_FILL,MAGENTA);
	bar(x,y,x+40,y+20);
	setcolor(BLACK);
	rectangle(x,y,x+40,y+20);
}
/*Brick collision Detection
if Ball Hits Brick & Color is not 0 it is broken & Ball is deflected*/
void brick::brick_collision(int ball_x,int ball_y)
{
	int i,j;
	ball ob;
	j=ball_x/40;
	i=(ball_y-20)/20;
	if(a[i][j]!=-1)       //If There is a brick
	{
	     a[i][j]=0;
	     draw_brick(5,15);
	     test_over++;
	     up_down=1;
	     music();
	     score++;
	     if(score>=r*c/2) score+=2;
	}
}
//Called from main reinitialisation
void brick_main()
{
	cleardevice();
	hide_mouse();
	int choice,but,mx,my,s=0;
	up_down=0;
	paddle p;
	rectangle(0,0,b.get_c()*40+1,400);
	b.set_array();
	b.check_brick();
	ball ob;
	cleardevice();
	while(game_status!=1)
	{
		 b.check_brick();
		 if(s==0)
		 {
			while(choice!=32)
			{
				asm mov ah,0x07;
				asm INT 0x21;
				asm mov ah,0x00;
				asm mov choice,ax;
			}
		 }
		s=1;
		ob.move_ball();
		b.check_brick();
		disp_stats();
		p.get_pos();
	}
	setcolor(RED);
	settextstyle(1,0,1);
	outtextxy(200,200,"U Won!");
	delay(5000);
	closegraph();
}
/*********************CANNON BALL**************************/
/*Checks whether Cannon has hit Tower */
int collision(int a,int b,int c,int d,int x,int y)
{
	int e=(a+c)/2;
	float xc,yc;
	int m=(b-d)/(a-e);
	if(x>a && x<c && y+10==b) return 1;
	if(x+10>=a && x+10<=e && y>=b && y<=d)
	{
		if(abs((y+m*x-d-m*a)/sqrt(1+m*m))<=10)
		return 1;    //Ball is tangential to the line of the tower
	}
	return 0;
}

/*Draws triangular Tower */
void tower(int x1,int y1,int x2,int y2,int c)
{
	setcolor(c);
	line(x1,y2,x2,y2);
	line(x1,y2,(x1+x2)/2,y1);
	line(x2,y2,(x1+x2)/2,y1);
	setfillstyle(SOLID_FILL,c);
	floodfill((x1+x2)/2,(y1+y2)/2,c);
}
/*Cannon Defn for Cannon */
class canon
{
	float x;
	float y;
	float vel;
	float deg;
public:
	canon()
	{
		x=40;
		y=300;
	}
	void get();
	void canon_show();
	void canon_draw();
};
/*Gets the Velocity & Angle for shooting the target*/
void canon::get()
{
	char a[10],v[10],d[10],c=' ';
	a[0]='\0';
	static char z[10];
	int i=0;
	setcolor(BLACK);
	outtextxy(250,10,z);
	setcolor(WHITE);
	outtextxy(10,10,"Enter the velocity & angle");
	while(c!='\r')
	{
		c=getch();
		if(c=='\b')
		{
			setcolor(BLACK);
			outtextxy(250,10,a);
			setcolor(WHITE);
			a[--i]='\0';
		}
		else if(isdigit(c) || c==' ')
		{
			a[i++]=c;
			a[i]='\0';
		}
		outtextxy(250,10,a);
	}
	a[i]='\0';
	strcpy(z,a);
	int j=0;
	for(i=0;i<strlen(a);i++)
	{
		if(a[i]==' ')
		{
			d[j]='\0';
			strcpy(v,d);
			j=0;
		}
		else
		{
			d[j++]=a[i];
		}
	}
	d[j]='\0';
	vel=atoi(v);
	deg=atoi(d);
	if(vel*deg==0)
	get();
	deg=deg*(M_PI/180.);
}
/*Draws the Cannon Barrel*/
void canon::canon_draw()
{
	setcolor(BROWN);
	rectangle(2,380,40,360);
	rectangle(15,360,35,340);
	line(20,340,30,300);
	line(30,340,40,310);
}
/*Moves the cannon on the basis of Velocity & Angle on a Parabolic Path*/
void canon::canon_show()
{
	int x1,x2,y1,flag=0,count=0,hit=0;
	int h=100,prevx,prevy;
	int towx1,towx2,towy1,towy2;
	while(life>=0 && y<=500 && level<=5)
	{
		disp_stats();
		if(flag==0)
		{
			x1=rand()%300+150;
			x2=x1+20;
			y1=rand()%200+150; //get random positions for the tower
			canon_draw();
			tower(towx1,towy1,towx2,towy2,BLACK);
			tower(x1,y1,x2,y1+h,RED);
			towx1=x1,towy1=y1,towx2=x2,towy2=y1+h;
			setcolor(GREEN);
			circle(x,y,10);
			get();
			flag=1;
		}
		//Calculation of the parabolic Path

		y=300-(x*tan(deg)-(9.8*x*x)/(2*vel*vel*cos(deg)*cos(deg)));
		canon_draw();
		setcolor(BLACK);
		circle(prevx,prevy,10);
		setcolor(GREEN);
		circle(x,y,10);
		if(collision(x1,y1,x2,y1+h,x,y)==1)
		{
			setcolor(BLACK);
			circle(x,y,10);
			flag=0;
			x=40;
			y=300;
			count++;
			sound(100);
			delay(100);
			nosound();
			hit++;
			score+=level;
		}
		//Cannon has gone beyond the limits
		if(x>460 || y<90 || y>460)
		{
			sound(250);
			delay(500);
			nosound();
			setcolor(BLACK);
			circle(x,y,10);
			life--;
			if(life==0) break;
			flag=0;
			x=40;
			y=300;
		}
		//Each Level completed
		if(count==level+12)
		{
		       score+=life;
		       level++;
		       count=0;
		       x=40;
		       y=300;
		       flag=0;
		       h-=10;
		       life=3;
		}
		prevx=x,prevy=y;
		x++;
		delay(3);
	}
}
void canon_main()
{
	cleardevice();
	setcolor(WHITE);
	rectangle(0,0,479,479);
	level=1;
	canon c;
	c.canon_show();
	setcolor(RED);
	settextstyle(1,0,1);
	if(level==6) outtextxy(200,200,"U Have Won!");
	else outtextxy(200,200,"Game Over");
	delay(5000);
}
/***********************ALPHA BETA*******************/
FILE * ab_fp;
int ab_status=0;
int max(int a,int b)
{
	if(a>b)
	return a;
	return b;
}
int min(int a,int b)
{
	if(a<b)
	return a;
	return b;
}

class ab
{
	int e;
	char word[8];
	char guess[8];
	int alpha;
	int beta;
	public:
		ab()
		{
			alpha=0;
			beta=0;
		}
		void assign_word();
		void guess_word();
		void comp_word();
};
void ab::assign_word()
{
	int n,i=0;
	n=rand()%275;
	while(!feof(ab_fp))
	{
		fscanf(ab_fp,"%s",word);
		i++;
		if(i==n)
		break;
	}
}
void ab::guess_word()
{
	int cor=0;
	while(cor==0)
	{
	fflush(stdin);
	printf("\nEnter the word\t\t\t");
	scanf("%s",guess);
	if(strcmp(guess,"giveup")==0)
	{
		clrscr();
		printf("\nShame on u");
		printf("\nTry Again next time");
		getch();
		longjmp(jmpbuf,3);
	}
	printf("\nEnter the entry(1L,2R) ");
	scanf("%d",&e);
	if(strlen(guess)>=4 && strlen(guess)<=6) cor=1;
	else printf("\nInvalid Word");
	}
}
void ab::comp_word()
{
	int s,l1,l2,i,j;
	char a[8];
	char b[8];
	strcpy(a,word);
	strcpy(b,guess);
	l1=strlen(a);
	l2=strlen(b);
	if(e==1) s=i=j=0;
	else if(e==2) s=j=(max(l1,l2)-min(l1,l2));
	i=0,j=s;
	while(i<min(l1,l2))
	{
		if(a[j]==b[i])
		{
			alpha++;
			a[j]=b[i]=' ';
		}
		i++;
		j++;
	}
	if(alpha==max(l1,l2)) ab_status=1;
	printf("\t\t\t\t%d",alpha);
	alpha=0;
	j=s,i=0;
	while(i<min(l1,l2))
	{
		if(a[j]==b[i] && a[j]!=' ')
		{
			beta++;
			a[j]=b[i]=' ';
			i++;
			j=s;
			continue;
		}
		j++;
		if(j==max(l1,l2) && e==2) {j=s;i++;}
		else if(j==min(l1,l2) && e==1) {j=s;i++;}
	}
	printf("\t%d",beta);
	beta=0;
}
void ab_main()
{
	closegraph();
	srand(time(NULL));
	int chance=0;
	clrscr();
	printf("\nWelcome to AlphaBeta");
	printf("\nType giveup to accept defeat");
	printf("\nAll the Best");
	getch();
	clrscr();
	ab_fp=fopen("words.txt","r");
	if(ab_fp==NULL)
		longjmp(jmpbuf,3);
	ab o;
	o.assign_word();
	while(ab_status==0)
	{
		chance++;
		o.guess_word();
		o.comp_word();
	}
	printf("\nYou have taken %d chances",chance);
	getch();
}

/*************************BATTLE SHIP*************************/
char table[9][9];
int matrix[9][9];
int row=8,col=8;
int r,c;

/*To get Random Value for The postion for thw weapons based on 'num'*/
int getrand(int num)
{
	srand(time(NULL));
	return rand()%num+1;

}
/*To convert a selected position in the circle to Adjacency Matrix*/
int circle_to_mat(int x,int y)
{
	int i,j;
	for(i=1;i<=row;i++)
	{
		for(j=1;j<=col;j++)
		{
		  if(abs(x-20*j)<=10 && abs(y-20*i)<=10)
		  {
			r=i;
			c=j;
		  }
		}
       }
       if(matrix[r][c]==999)
       {
		matrix[r][c]=1;
		return 1;
       }
       else return 0;
}
/*Draw a Final Map after the weapons have been struck*/
void final_draw()
{
	int i,j;
	cleardevice();
	for(i=1;i<=row;i++)
		for(j=1;j<=col;j++)
			circle(j*20,i*20,10);
	for(i=1;i<=row;i++)
	{
		for(j=1;j<=col;j++)
		{
			if(table[i][j]=='a')
				outtextxy(j*20,i*20,"A");
			else if(table[i][j]=='b')
				outtextxy(j*20,i*20,"B");
			else if(table[i][j]=='c')
				outtextxy(j*20,i*20,"C");
			else if(table[i][j]=='d')
				outtextxy(j*20,i*20,"D");
			else if(table[i][j]=='s')
				outtextxy(j*20,i*20,"S");
		}
	}
}
/*Draw the map at every instant */
void draw_map()
{
	int i,j;
	for(i=1;i<=row;i++)

		for(j=1;j<=col;j++)

			circle(j*20,i*20,10);
	for(i=1;i<=row;i++)
	{
		for(j=1;j<=col;j++)
		{
			if(matrix[i][j]==1 || matrix[i][j]==0)
				outtextxy(j*20,i*20,"X");
		}
	}

}
/*Set the selected points in the matrix to 0 after 3 points */
void set_mat()
{
	int i,j;
	for(i=1;i<=row;i++)
		for(j=1;j<=col;j++)
			if(matrix[i][j]==1) matrix[i][j]=0;
}
/*Add the score once the weapon has been struck*/
void check_score(int r,int c)
{
	if(table[r][c]!='x')
	score++;
}
/*Display the Weapon types after 3 chances*/
void disp_char()
{
	int i,j,l=0;
	static char z[10];
	setcolor(BLACK);
	outtextxy(400,100,z);
	for(i=1;i<=row;i++)
		for(j=1;j<=row;j++)
			if(matrix[i][j]==1)
				z[l++]=table[i][j];
	z[l]='\0';
	setcolor(GREEN);
	outtextxy(400,100,z);
	setcolor(WHITE);
}
/*Functions to check for inclusion of weapons at a point*/

int check_a(int x,int y,int num)
{
	int i;
	for(i=0;i<=num-1;i++)
		if(table[x+i][y+i]!='x')
			return 0;
	return 1;
}
int check_b(int x,int y,int num)
{
	int i;
	for(i=0;i<=num-1;i++)
		if(table[x+i][y]!='x')
			return 0;
	return 1;
}
int check_c(int x,int y,int num)
{
	int i;
	for(i=0;i<=num-1;i++)
		if(table[x][y+i]!='x')
			return 0;
	return 1;
}
int check_d(int x,int y,int num)
{
	int i;
	for(i=0;i<=num-1;i++)
		if(table[x+i][y-i]!='x')
			return 0;
	return 1;
}
int check_s(int x,int y,int num)
{
	int i;
	for(i=0;i<=num-1;i++)
		if(table[x][y]!='x')
			return 0;
	return 1;
}
/*Random generation of A,B,C,D,S Positions*/
void set(char ch)
{
	int i,j,num,x,y;
	if(ch=='a')
	{
		num=5;
		x=getrand(5);
		y=getrand(5);
		while(!(check_a(x,y,num)==1))
		{
			x=getrand(5);
			y=getrand(5);
		}
		for(i=0;i<=(num-1)/2;i++)
		table[x+i][y+i]=ch;
		for(i=-1;i<(num-1)/2;i++)
		table[x+1-i][y+1+i]=ch;
	}
	else if(ch=='b')
	{
		num=4;
		x=getrand(5);
		y=getrand(8);
		while(!(check_b(x,y,num)==1))
		{
			x=getrand(5);    //max placed  (8,5)
			y=getrand(8);

		}
		for(i=0;i<=num-1;i++)
		table[x+i][y]=ch;
	}
	else if(ch=='c')
	{
		num=3;
		x=getrand(8);
		y=getrand(6);
		while(!(check_c(x,y,num)==1))
		{
			x=getrand(8);
			y=getrand(6);  //c placed in (6,8)
		}
		for(i=0;i<=num-1;i++)
		table[x][y+i]=ch;
	}
	else if(ch=='d')
	{
		num=2;
		x=getrand(7);
		y=getrand(7);
		while(!(check_d(x,y,num)==1))
		{
			x=getrand(7);    //cat be placed in (8,8)
			y=getrand(7);
		}
		for(i=0;i<=num-1;i++)
		table[x+i][y-i]=ch;
	}
	else if(ch=='s')
	{
		num=1;
		x=getrand(8); //s can be placed anywhere in remaining places
		y=getrand(8);
		while(!(check_s(x,y,num)==1))
		{
			x=getrand(8);
			y=getrand(8);
		}
		table[x][y]=ch;
	}

}
void batl_main()
{
	cleardevice();
	show_mouse();
	int i,j,but_count=1,b=0,chance=0,choice;
	for(i=1;i<=row;i++)
		for(j=1;j<=col;j++)
			matrix[i][j]=999;
	/*Initailize the table */
	for(i=1;i<=col;i++)
		for(j=1;j<=col;j++)
			table[i][j]='x';
	set('a');   //random position for 5 A's
	set('b');   //random position for 4 B's
	set('c');   //random position for 3 C's
	set('d');   //random position for 2 D's
	set('s');   //random position for 1 S
	while(score!=15)         //Totally 15 weapons to be shot
	{
		while(b!=1)
		{
			draw_map();           //Draw the circles marked
			show_mouse();
			get_button_press(0);
			if(mclick==1)
			{
			while(get_button_release(0)==1)
			mouse_status();
			b=1;
			}
		}
		b=0;
		if(circle_to_mat(mx,my))
		{
			but_count++;               //Increment # of clicks
			check_score(r,c);          //weapon shot or not
		}

		if(but_count==5)
		{
			chance++;
			disp_char();
			set_mat();             //Set matrix after 3 chances
			but_count=1;
		}
	}
	delay(1000);
	final_draw();
	setcolor(RED);
	settextstyle(1,0,1);
	outtextxy(10,300,"You have won ");
	outtextxy(10,400,"Chances ");
	char *a = conv(chance);
	char z[10];
	for(i=0;i<strlen(a);i++)
	z[i]=*(a+i);
	z[i]='\0';
	outtextxy(200,400,z);
	delay(5000);
}
/*************************** NIBBLE ********************************/
int n_delay=125;       	//delay at each level
int direction=0;
int food_drawn=0;	//bool
int food_x,food_y;     	//positions
int trail_x[60],trail_y[60],trail_count=0;      //what trails to be removed
int length=15;  	//15-25 how many to del
/*Deletes the last few trails of the worm*/
void del_trail()
{
	int i;
	setcolor(BLACK);
	setfillstyle(SOLID_FILL,BLACK);
	circle(trail_x[0],trail_y[0],5);
	floodfill(trail_x[0],trail_y[0],BLACK);
	for(i=1;i<trail_count;i++)
	{
		trail_x[i-1]=trail_x[i];
		trail_y[i-1]=trail_y[i];
	}
	trail_count--;
}
/*Randomly draws food for worm*/
void draw_food()
{
	int x=0,y=0;
	srand(time(NULL));
	while(getpixel(x-5,y)==BLUE || getpixel(x+5,y)==BLUE
		|| getpixel(x,y-5)==BLUE || getpixel(x,y+5)==BLUE ||
		 y<10 || x<10 || x>470 || y>470)
	{
		x=rand()%480;
		y=rand()%480;
	}
	setcolor(RED);
	setfillstyle(SOLID_FILL,RED);
	circle(x,y,5);
	floodfill(x,y,RED);
	food_drawn=1;
	food_x=x,food_y=y;
}
/*collision of worm an food & boundary*/
int collision(int x,int y)
{
	int c;
	if(direction==1)
		c=getpixel(x,y-1);
	else if(direction==2)
		c=getpixel(x+1,y);
	else if(direction==3)
		c=getpixel(x-1,y);
	else if(direction==4)
		c=getpixel(x,y+1);
	if(c==BLUE) return 1;
	if(getpixel(x,y-5)==RED || getpixel(x,y+5)==RED ||
	getpixel(x-5,y)==RED || getpixel(x+5,y)==RED )
	{
		length--;
		score++;
		food_drawn=0;
		setcolor(BLACK);
		setfillstyle(SOLID_FILL,BLACK);
		circle(food_x,food_y,5);
		floodfill(food_x,food_y,BLACK);
		delay(100);
	}
	return 0;
}
class nibble
{
	int x,y;
	public:
		nibble()
		{
			x=y=150;
		}
	void draw_one(int);
	void move();
};
/*Draws basic unit of worm*/
void nibble::draw_one(int c)
{
	int i;
	setfillstyle(SOLID_FILL,c);
	setcolor(c);
	circle(x,y,5);
	floodfill(x,y,c);
}
/*Moves the worm*/
void nibble::move()
{
	int i;
	draw_one(BLUE);
	trail_x[trail_count]=x,trail_y[trail_count]=y;
	trail_count++;
	if(trail_count==50)
		del_trail();
	if(direction==1)
		y-=10;
	else if(direction==2)
		x+=10;
	else if(direction==3)
		x-=10;
	else if(direction==4)
		y+=10;
	if(x<=0 || x>=475 || y>=475 || y<=0 || collision(x,y)==1)
	{
		life--;
		sound(200);
		delay(500);
		nosound();
		x=y=150;
		length=15;
		trail_count=0;
		direction=0;
		food_drawn=0;
		for(int j=0;j<trail_count;j++)
			trail_x[j]=trail_y[j]=0;
		setviewport(0,0,640,480,1);
		clearviewport();
		setcolor(WHITE);
		rectangle(0,0,479,479);
		disp_stats();
	}
	else
	{
		draw_one(BLUE);
		if(trail_count%length==0)
		{
			del_trail();
			length++;
			if(length==30) length=15;
		}
	delay(n_delay);
	}
}
/*Gets key movements for moving the worm*/
void get_arrow_keys()
{
	int c;
	fflush(stdin);
	if(kbhit())
	{
		c=getch();
		switch(c)
		{
			case 72:
				if(direction!=4)
				direction=1;
			break;
			case 77:
				if(direction!=3)
				direction=2;
				break;
			case 75:
				if(direction!=2)
				direction=3;
				break;
			case 80:
				if(direction!=1)
				direction=4;
				break;
		}
	}
}
void nib_main()
{
	hide_mouse();
	cleardevice();
	nibble nb;
	n_delay=125;
	food_drawn=0;
	setcolor(WHITE);
	rectangle(0,0,479,479);
	disp_stats();
	while(1)
	{
		get_arrow_keys();
		nb.move();
		disp_stats();
		if(food_drawn==0)draw_food();
		if(score%10==0 && score!=0)
		{
			if(score/10==level)
			{
			sound(250);
			delay(100);
			nosound();
			setviewport(0,0,640,60,1);
			clearviewport();
			setcolor(WHITE);
			rectangle(0,0,479,479);
			level++;
			life=3;
			if(level==6)
			{
				setcolor(RED);
				settextstyle(1,0,1);
				outtextxy(200,200,"U Won !");
				delay(2000);
				longjmp(jmpbuf,4);
			}
			disp_stats();
			food_drawn=0;
			n_delay-=25;
			}
		}
		if(life==0)
		{
			settextstyle(1,0,1);
			outtextxy(200,200,"Game Over");
			delay(5000);
			longjmp(jmpbuf,4);
		}
	}
}
/****************************GOBBLE********************************/
int arena[60][60];
int arena_to_be_modified=0;
int diamond_to_be_drawn=0;
struct pos
{
	int x,y;
};
/*Sets the area with 15  M 1 H*/
void set_initial_arena()
{
	int i,j,c=0;
	srand(time(NULL));
	i=rand()%60;
	j=rand()%60;
	arena[i][j]=1;
	while(c<15)
	{
		i=rand()%60;
		j=rand()%60;
		if(arena[i][j]==0)
		{
			arena[i][j]=2;
			c++;
		}
	}
}
/*The matrix mapped to the screen*/
void matrix_to_arena()
{
	int i,j;
	arena_to_be_modified=0;
	setcolor(WHITE);
	rectangle(0,0,480,479);
	for(i=0;i<60;i++)
	{
		for(j=0;j<60;j++)
		{
			if(arena[i][j]==1)
			{
			setcolor(BLUE);
			outtextxy(j*8+4,i*8+4,"H");
			}
			else if(arena[i][j]==2)
			{
			setcolor(RED);
			outtextxy(j*8+4,i*8+4,"M");
			}
			else if(arena[i][j]==3)
			{
			setcolor(YELLOW);
			outtextxy(j*8+4,i*8+4,"D");
			}
		}
	}
}
/*Gets the position of the H*/
struct pos get_h_pos()
{
	struct pos p;
	int i,j;
	for(i=0;i<60;i++)
		for(j=0;j<60;j++)
			if(arena[i][j]==1)
			{
				p.x=i;
				p.y=j;
			}
	return p;
}
/*Moves the H based on keybd movements*/
void g_move(int d)
{
	struct pos p=get_h_pos();
	if(d==3)//up
	{
		if(p.y-1>=0)
		{
			if(arena[p.x][p.y-1]==2) life--;
			else if(arena[p.x][p.y-1]==3) diamond_to_be_drawn=0;
			arena[p.x][p.y-1]=1;
			arena[p.x][p.y]=0;
		}
		else if(p.y==0)
		{
			if(arena[p.x][59]==2) life--;
			else if(arena[p.x][59]==3) diamond_to_be_drawn=0;
			arena[p.x][0]=0;
			arena[p.x][59]=1;
		}
	}
	else if(d==4)//right
	{
		if(p.x+1<60)
		{
			if(arena[p.x+1][p.y]==2) life--;
			else if(arena[p.x+1][p.y]==3) diamond_to_be_drawn=0;
			arena[p.x+1][p.y]=1;
			arena[p.x][p.y]=0;
		}
		else if(p.x==59)
		{
			if(arena[0][p.y]==2) life--;
			else if(arena[0][p.y]==3) diamond_to_be_drawn=0;
			arena[59][p.y]=0;
			arena[0][p.y]=1;
		}
	}
	else if(d==1)//left
	{
		if(p.x-1>=0)
		{
			if(arena[p.x-1][p.y]==2) life--;
			else if(arena[p.x-1][p.y]==3) diamond_to_be_drawn=0;
			arena[p.x-1][p.y]=1;
			arena[p.x][p.y]=0;
		}
		else if(p.x==0)
		{
			if(arena[59][p.y]==2) life--;
			else if(arena[59][p.y]==3) diamond_to_be_drawn=0;
			arena[0][p.y]=0;
			arena[59][p.y]=1;
		}
	}
	else if(d==2)//down
	{
		if(p.y+1<60)
		{
			if(arena[p.x][p.y+1]==2) life--;
			else if(arena[p.x][p.y+1]==3) diamond_to_be_drawn=0;
			arena[p.x][p.y+1]=1;
			arena[p.x][p.y]=0;
		}
		else if(p.y==59)
		{
			if(arena[p.x][0]==2) life--;
			else if(arena[p.x][0]==3) diamond_to_be_drawn=0;
			arena[p.x][0]=1;
			arena[p.x][59]=0;
		}
	}
}
/*The M moves based on the movements of the H*/
void g_move_enemy()
{
	struct pos p=get_h_pos();
	int i,j;
	i=rand()%60;
	j=rand()%60;
			if(arena[i][j]==2)
			{
				if(abs(p.x-i)>abs(p.y-j))
				{
					if(p.x<i)
					{
						if(i-1>=0)
						{
							if(arena[i-1][j]==0)
							{
								arena[i-1][j]=2;
								arena[i][j]=0;
							}
							else if(arena[i-1][j]==1)
							{
								arena[i][j]=0;
								life--;
							}
						}
					}
					else
					{
						if(i+1<60)
						{
							if(arena[i+1][j]==0)
							{
								arena[i+1][j]=2;
								arena[i][j]=0;
							}
							else if(arena[i+1][j]==1)
							{
								arena[i][j]=0;
								life--;
							}
						}
					}
				}
				else
				{
					if(p.y<j)
					{
						if(arena[i][j-1]==0)
							{
								arena[i][j-1]=2;
								arena[i][j]=0;
							}
							else if(arena[i][j-1]==1)
							{
								arena[i][j]=0;
								life--;
							}
					}
					else
					{
						if(arena[i][j+1]==0)
							{
								arena[i][j+1]=2;
								arena[i][j]=0;
							}
							else if(arena[i][j+1]==1)
							{
								arena[i][j]=0;
								life--;
							}
					}
				}
			}
}
void get_keybd()
{
	int c;
	if(kbhit())
	{
		arena_to_be_modified=1;
		c=getch();
		switch(c)
		{
			case 72:
				g_move(1);
				break;
			case 77:
				g_move(2);
				break;
			case 75:
				g_move(3);
				break;
			case 80:
				g_move(4);
				break;
		}
	}
}
/*Sets D to be collected by H*/
void set_diamond()
{
	int i,j;
	i=rand()%60;
	j=rand()%60;
	while(arena[i][j]!=0)
	{
		i=rand()%60;
		j=rand()%60;
	}
	arena[i][j]=3;
	diamond_to_be_drawn=1;
	score++;
	if(score==10) game_status=1;
}

void gob_main()
{
	life=5;
	int gd=DETECT,gm;
	for(int i=0;i<60;i++)
	for(int j=0;j<60;j++)
	arena[i][j]=0;
	game_status=0;
	arena_to_be_modified=0;
	diamond_to_be_drawn=0;
	set_initial_arena();
	initgraph(&gd,&gm,"c:/tc/");
	cleardevice();
	matrix_to_arena();
	while(1)
	{
		get_keybd();
		g_move_enemy();
		if(diamond_to_be_drawn==0)
			set_diamond();
		if(arena_to_be_modified==1)
		{
			g_move_enemy();
			cleardevice();
			disp_stats();
			matrix_to_arena();
		}
		if(life<=0)
		{
			setcolor(RED);
			settextstyle(1,0,1);
			outtextxy(200,200,"U Lost");
			delay(5000);
			longjmp(jmpbuf,7);
		}
		if(game_status==1) break;
	}
	setcolor(RED);
	settextstyle(1,0,1);
	outtextxy(200,200,"U Won");
	delay(5000);
}
/*****************************HUNTER*********************************/
int target_health=100;
int shooter_health=100;
int bullet_num=150;
int shooter_moving=1;
/*sounds which indicate which of the 2 have been hit*/
void scream(int a,int b)
{
	sound(a);
	delay(20);
	sound(b);
	delay(50);
	nosound();
}
class target
{
	int x,y;
	int bx,by,bc;
	public:
	target()
	{
		x=240;
		y=50;
		bc=0;
	}
	void draw(int,int);
	void move();
	void bullet_move();
};
class shooter
{
	int x,y;
	int bx,by,bc;
	public:
	shooter()
	{
		x=240;
		y=450;
		bc=0;
	}
	void draw(int,int);
	void move();
	void shoot();
	void bullet_fire();
	void bullet_move();
};
/*Decreases the health of the shooter/target coz of shot*/
void bul_disp()
{
	int i;
	char * a,b[5];
	setcolor(BLACK);
	a=conv(bullet_num);
	for(i=0;i<strlen(a);i++)
		b[i]=*(a+i);
	b[i]='\0';
	outtextxy(500,400,b);
	bullet_num--;
	setcolor(BROWN);
	a=conv(bullet_num);
	for(i=0;i<strlen(a);i++)
		b[i]=*(a+i);
	b[i]='\0';
	outtextxy(500,400,b);
}
void decrease_health(int w)
{
	char * m,b[5];
	int i;
	if(w==1)
	{
		scream(100,200);
		setcolor(BLACK);
		m=conv(shooter_health);
		for(i=0;i<strlen(m);i++)
			b[i]=*(m+i);
		b[i]='\0';
		outtextxy(500,350,b);
		shooter_health--;
		setcolor(BROWN);
		m=conv(shooter_health);
		for(i=0;i<strlen(m);i++)
			b[i]=*(m+i);
		b[i]='\0';
		outtextxy(500,350,b);
	}
	else
	{
		scream(400,500);
		setcolor(BLACK);
		m=conv(target_health);
		for(i=0;i<strlen(m);i++)
			b[i]=*(m+i);
		b[i]='\0';
		outtextxy(500,150,b);
		target_health--;
		setcolor(RED);
		m=conv(target_health);
		for(i=0;i<strlen(m);i++)
			b[i]=*(m+i);
		b[i]='\0';
		outtextxy(500,150,b);
	}
}
void target::bullet_move()
{
	if(bc==0)
	{
		bx=x,by=y+10;
		bc=1;
	}
	putpixel(bx,by,BLACK);
	by++;
	if(getpixel(bx,by)==BROWN)
	{
		bc=0;
		decrease_health(1);
	}
	putpixel(bx,by,YELLOW);
}
void target::draw(int a,int b)
{
	setfillstyle(SOLID_FILL,BLACK);
	bar(x-20,y,x+20,y+20);
	bar(x-2,y+40,x+2,y);
	setfillstyle(SOLID_FILL,RED);
	x=a,y=b;
	bar(x-20,y,x+20,y+20);
	bar(x-2,y+40,x+2,y);
}
void target::move()
{
	int x1=x;
	if(shooter_moving==1)
	{
	static int dir=0,count=0;
	count++;
	if(count%100!=0) return;
	if(x1==455) dir=1;
	if(x1==25) dir=0;
	if(dir==0 ) x1++;
	else if(dir==1) x1--;
	}
	else
	{
		if(x1>mx) x1--;
		else if(x1<mx) x1++;
		if(x1>455) x1--;
		if(x1<25) x1++;
		shooter_moving=1;
	}
	draw(x1,y);
}
/*initialises the bullet*/
void shooter::bullet_fire()
{
	if(bc==1) return;
	bx=x;
	by=y-25;
	bc=1;
}
void shooter::bullet_move()
{
	int i;
	if(bc==0 ) return;
	putpixel(bx,by,BLACK);
	by--;
	if(getpixel(bx,by)==RED)
	{
		bc=0;
		decrease_health(0);
	}
	putpixel(bx,by,WHITE);
}
void shooter::draw(int a,int b)
{
	setfillstyle(SOLID_FILL,BLACK);
	bar(x-20,y,x+20,y+20);
	bar(x-2,y-20,x+2,y);
	setfillstyle(SOLID_FILL,BROWN);
	x=a,y=b;
	bar(x-20,y,x+20,y+20);
	bar(x-2,y-20,x+2,y);
}
void shooter::move()
{
	static int c=0;
	mouse_status();
	if(mx==x || mx>455 || mx<25)
	{
	c++;
	if(c%150==0) shooter_moving=0;
	return;
	}
	else draw(mx,450);
}
/*Gets click*/
void shooter::shoot()
{
	if(bullet_num==0) return;
	get_button_press(0);
	if(mclick==1)
	{
		while(get_button_release(0)==1);
			bullet_fire();
			bul_disp();
	}
}
void hunt_main()
{
	hide_mouse();
	target_health=100;
	shooter_health=100;
	bullet_num=150;
	cleardevice();
	setcolor(WHITE);
	rectangle(0,0,479,479);
	game_status=0;
	outtextxy(540,150,"Target H+");
	outtextxy(540,350,"Shooter H+");
	outtextxy(540,400,"Bullets");
	shooter sh;
	target tr;
	while(game_status==0)
	{
		sh.move();
		tr.move();
		sh.shoot();
		tr.bullet_move();
		sh.bullet_move();
		if(target_health*shooter_health==0) game_status=1;
		tr.bullet_move();
	}
	setcolor(RED);
	settextstyle(1,0,1);
	if(shooter_health==0)
		outtextxy(200,200,"U Lost");
	else
		outtextxy(200,200,"U Won!");
	delay(5000);
	show_mouse();
}
/*Draw the icons for the main menu*/
void exit_choice()
{
	setcolor(WHITE);
	rectangle(ex,ey,ex+60,ey+100);
	rectangle(ex+10,ey+40,ex+35,ey+60);
	line(ex+35,ey+30,ex+35,ey+70);
	line(ex+55,ey+50,ex+35,ey+30);
	line(ex+55,ey+50,ex+35,ey+70);
	setcolor(RED);
	setfillstyle(SOLID_FILL,BROWN);
	floodfill(ex+50,ey+50,WHITE);
	floodfill(ex+25,ey+50,WHITE);
	outtextxy(ex+10,ey+110,"Exit");
}
void hunt_choice()
{
	setcolor(WHITE);
	rectangle(hx,hy,hx+60,hy+100);
	setcolor(BROWN);
	rectangle(hx+20,hy+80,hx+40,hy+90);
	rectangle(hx+28,hy+60,hx+32,hy+80);
	setfillstyle(SOLID_FILL,BROWN);
	floodfill(hx+30,hy+70,BROWN);
	floodfill(hx+30,hy+85,BROWN);
	setcolor(RED);
	setfillstyle(SOLID_FILL,RED);
	bar(hx+5,hy+10,hx+15,hy+20);
	bar(hx+25,hy+10,hx+35,hy+20);
	bar(hx+45,hy+10,hx+55,hy+20);
	outtextxy(hx+10,hy+110,"Hunter");
}
void gob_choice()
{
	setcolor(WHITE);
	rectangle(gx,gy,gx+60,gy+100);
	setcolor(BLUE);
	outtextxy(gx+10,gy+10,"H");
	setcolor(RED);
	outtextxy(gx+20,gy+20,"M");
	setcolor(YELLOW);
	outtextxy(gx+30,gy+70,"D");
	setcolor(RED);
	outtextxy(gx+10,gy+110,"Gobble");
}

void nib_choice()
{
	setcolor(WHITE);
	rectangle(nx,ny,nx+60,ny+100);
	setcolor(BLUE);
	line(nx+30,ny+80,nx+30,ny+60);
	line(nx+30,ny+60,nx+50,ny+60);
	line(nx+50,ny+60,nx+50,ny+40);
	line(nx+50,ny+40,nx+20,ny+40);
	line(nx+20,ny+40,nx+20,ny+20);
	setfillstyle(SOLID_FILL,YELLOW);
	bar(nx+10,ny+10,nx+20,ny+20);
	setcolor(RED);
	outtextxy(nx+10,ny+110,"Nibble");
}
void ab_choice()
{
	setcolor(WHITE);
	rectangle(ax,ay,ax+60,ay+100);
	settextstyle(1,0,6);
	setcolor(RED);
	outtextxy(ax+10,ay+10,"A");
	outtextxy(ax+30,ay+30,"B");
	settextstyle(0,0,1);
	outtextxy(ax,ay+110,"AlphaBeta");
}
void batl_choice()
{
	setcolor(WHITE);
	rectangle(sx,sy,sx+60,sy+100);
	setcolor(DARKGRAY);
	setfillstyle(SOLID_FILL,DARKGRAY);
	line(sx+15,sy+80,sx+45,sy+80);
	line(sx+15,sy+80,sx+5,sy+60);
	line(sx+45,sy+80,sx+55,sy+60);
	line(sx+5,sy+60,sx+55,sy+60);
	floodfill(sx+30,sy+70,DARKGRAY);
	bar(sx+20,sy+50,sx+40,sy+60);
	line(sx+30,sy+50,sx+45,sy+10);
	line(sx+40,sy+50,sx+55,sy+15);
	line(sx+45,sy+10,sx+55,sy+15);
	floodfill(sx+45,sy+15,DARKGRAY);
	setcolor(RED);
	outtextxy(sx-10,sy+110,"Battle Ship");
}
void canon_choice()
{
	setcolor(WHITE);
	rectangle(cx,cy,cx+60,cy+100);
	setcolor(RED);
	circle(cx+30,cy+60,25);
	setfillstyle(SOLID_FILL,RED);
	floodfill(cx+30,cy+60,RED);
	setcolor(BLUE);
	arc(cx+35,cy+45,120,180,25);
	setcolor(LIGHTRED);
	circle(cx+25,cy+25,5);
	setfillstyle(SOLID_FILL,YELLOW);
	floodfill(cx+25,cy+25,LIGHTRED);
	setcolor(RED);
	outtextxy(cx+10,cy+110,"Cannon");
}
void brick_choice()
{
	int i,j,c=0;
	rectangle(bx,by,bx+60,by+100);
	for(i=bx;i<bx+60;i+=20)
		for(j=by;j<by+60;j+=10)
		{
			setfillstyle(SOLID_FILL,c);
			bar(i+1,j+1,i+19,j+10);
			c++;
		}
	setcolor(BROWN);
	setfillstyle(SOLID_FILL,BROWN);
	circle(bx+30,by+80,10);
	floodfill(bx+30,by+80,BROWN);
	setcolor(RED);
	outtextxy(bx+10,by+110,"Bricks");
}
/*Which option selected by the user*/
int get_choice()
{
	get_button_press(0);
	if(mclick==1)
	{
		while(get_button_release(0)==1)
		{

			mouse_status();
		}
		if(mx>=bx && mx<=bx+60 && my>=by && my<=by+100)
			return 1;
		if(mx>=cx && mx<=cx+60 && my>=cy && my<=cy+100)
			return 2;
		if(mx>=ax && mx<=ax+60 && my>=ay && my<=ay+100)
			return 3;
		if(mx>=nx && mx<=nx+60 && my>=ny && my<=ny+100)
			return 4;
		if(mx>=ex && mx<=ex+60 && my>=ey && my<=ey+100)
			return 5;
		if(mx>=sx && mx<=sx+60 && my>=sy && my<=sy+100)
			return 6;
		if(mx>=gx && mx<=gx+60 && my>=gy && my<=gy+100)
			return 7;
		if(mx>=hx && mx<=hx+60 && my>=hy && my<=hy+100)
			return 8;
	}
	else
	{
		mouse_status();
		if(mx>=bx && mx<=bx+60 && my>=by && my<=by+100)
		{
			setcolor(GREEN);
			rectangle(bx,by,bx+60,by+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(bx,by,bx+60,by+100);
		}
		if(mx>=cx && mx<=cx+60 && my>=cy && my<=cy+100)
		{
			setcolor(GREEN);
			rectangle(cx,cy,cx+60,cy+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(cx,cy,cx+60,cy+100);
		}
		if(mx>=ax && mx<=ax+60 && my>=ay && my<=ay+100)
		{
			setcolor(GREEN);
			rectangle(ax,ay,ax+60,ay+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(ax,ay,ax+60,ay+100);
		}
		if(mx>=nx && mx<=nx+60 && my>=ny && my<=ny+100)
		{
			setcolor(GREEN);
			rectangle(nx,ny,nx+60,ny+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(nx,ny,nx+60,ny+100);
		}
		if(mx>=ex && mx<=ex+60 && my>=ey && my<=ey+100)
		{
			setcolor(GREEN);
			rectangle(ex,ey,ex+60,ey+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(ex,ey,ex+60,ey+100);
		}
		if(mx>=sx && mx<=sx+60 && my>=sy && my<=sy+100)
		{
			setcolor(GREEN);
			rectangle(sx,sy,sx+60,sy+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(sx,sy,sx+60,sy+100);
		}
		if(mx>=gx && mx<=gx+60 && my>=gy && my<=gy+100)
		{
			setcolor(GREEN);
			rectangle(gx,gy,gx+60,gy+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(gx,gy,gx+60,gy+100);
		}
		if(mx>=hx && mx<=hx+60 && my>=hy && my<=hy+100)
		{
			setcolor(GREEN);
			rectangle(hx,hy,hx+60,hy+100);
		}
		else
		{
			setcolor(WHITE);
			rectangle(hx,hy,hx+60,hy+100);
		}
	}
	return 0;
}

int main()
{
	int gd=DETECT,gm,ch=0;
	while(ch!=5)
	{
		initgraph(&gd,&gm,"c:/tc/");
		cleardevice();
		brick_choice();
		canon_choice();
		nib_choice();
		ab_choice();
		batl_choice();
		gob_choice();
		hunt_choice();
		exit_choice();
		mx=my=0;
		while(1)
		{
			show_mouse();
			ch=get_choice();
			if(ch==0) continue;
			else break;
		}
		int jmp_val=setjmp(jmpbuf);
		if(jmp_val==0)
		{
			hide_mouse();
			if(ch==1) brick_main();
			else if(ch==2) canon_main();
			else if(ch==3) ab_main();
			else if(ch==4) nib_main();
			else if(ch==6) batl_main();
			else if(ch==7) gob_main();
			else if(ch==8) hunt_main();
			else if(ch==5) return 0;
		}
		closegraph();
		score=0;level=1;life=3;game_status=0;
	}
	return 1;
}
