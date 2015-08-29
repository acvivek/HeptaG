#include <dos.h>
int mlr,mx,my,mclick,mdx,mdy;
union REGS i,o;
void show_mouse()
{
	i.h.al=1;
	int86(0x33,&i,&o);
}
void hide_mouse()
{
	i.h.al=2;
	int86(0x33,&i,&o);
}
void mouse_status()
{
	i.h.al=3;
	int86(0x33,&i,&o);
	mlr=o.x.bx;
	mx=o.x.cx;
	my=o.x.dx;
}
void mouse_set(int x,int y)
{
	i.h.al=4;
	i.x.cx=x;
	i.x.dx=y;
	int86(0x33,&i,&o);
}
void get_button_press(int b)
{
	i.h.al=5;
	i.x.bx=b;
	int86(0x33,&i,&o);
	mclick=o.x.ax;
	mx=o.x.cx;
	my=o.x.dx;
}
int get_button_release(int b)
{
	i.h.al=6;
	i.x.bx=b;
	int86(0x33,&i,&o);
	return o.x.ax;
}
void set_limit(int a,int b,int c,int d)
{
	i.h.al=7;
	i.x.cx=a;  //min x
	i.x.dx=b;
	int86(0x33,&i,&o);
	i.h.al=8;
	i.x.cx=c;  //min y
	i.x.dx=d;
	int86(0x33,&i,&o);
}
void graphics_cursor(int a)
{
	i.h.al=9;
	mouse_status();
	i.x.bx=mx;
	i.x.cx=my;
	i.x.dx=a;
	int86(0x33,&i,&o);
}
void get_motion()
{
	i.h.al=0x0b;
	int86(0x33,&i,&o);
	mdx=o.x.cx;
	mdy=o.x.dx;
}

