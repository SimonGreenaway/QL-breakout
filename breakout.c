#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <qdos.h>

#include "image.h"

//////////
// main //
//////////

char *drive="MDV1_";
library lib;

#define BRICKS (8*14)

sprite bricks[BRICKS];
sprite player,ball;
sprite numbers;

void drawNumber(screen scr,unsigned int x,unsigned int y,unsigned int n,unsigned int digits)
{
	int i;
	x+=digits*8;

	for(i=0;i<digits;i++)
	{
		unsigned int digit=n%10;
		n/=10;

		numbers.y=y;
		numbers.x=x;

		numbers.currentImage=digit;
		spritePlot(scr,&numbers);
		x-=8;
	}
}

void setup()
{
	unsigned int i,frames;

	cls(SCREEN);

	for(i=0;i<BRICKS;i++)
	{
		unsigned int y=i/14;
		unsigned int x=i%14;

		spriteSetup(&bricks[i],"brick");

		spriteAddImage(&bricks[i],&lib,y/2);
		bricks[i].x=x*9+2;
		bricks[i].y=y*4+64;

		bricks[i].draw=1;
		bricks[i].mask=0;
		bricks[i].currentImage=0;

		//printf("i=%d x=%d y=%d i=%d ci=%d\n",i,bricks[i].x,bricks[i].y,y/2,bricks[i].currentImage);

		//printf("%x\n",bricks[i].image[bricks[i].currentImage]->magic);

		spritePlot(SCREEN,&bricks[i]);
	}

	spriteSetup(&player,"player");
	spriteAddImage(&player,&lib,4);
	player.x=100;
	player.y=230;

	spritePlot(SCREEN,&player);

	fillBox(SCREEN,0,0,14*9+2,8,7);
	fillBox(SCREEN,0,0,2,255,7);
	fillBox(SCREEN,14*9,0,14*9+2,255,7);

	spriteSetup(&ball,"ball");
	spriteAddImage(&ball,&lib,5);
	ball.x=player.x+3;
	ball.y=player.y-2;
	ball.dx=1;
	ball.dy=-1;

	spritePlot(SCREEN,&ball);

	spriteSetup(&numbers,"numbers");

	for(i=0;i<10;i++)
		spriteAddImage(&numbers,&lib,6+i);

	frames=getFrames();
	while(1)
	{

		while(getFrames()<=frames);

		frames=getFrames()+1;
		
		ball.draw=0;
		spritePlot(SCREEN,&ball);
		ball.x+=ball.dx;
		ball.y+=ball.dy;
		ball.draw=1;
		spritePlot(SCREEN,&ball);

		if(ball.x==14*9-2) ball.dx=-1;
		else if(ball.x==3) ball.dx=1;

		if(ball.y==9) ball.dy=1;
		else if(ball.y==player.y-1) ball.dy=-1;
	}
}

int main(int argc, char *argv[],char *argp[])
{
	unsigned int s;
	char f[80];

	if(getenv("DRIVE")!=NULL) strcpy(drive,getenv("DRIVE"));

	printf("Default drive is '%s'\n",drive);

	// Parse the args

	setSysBase((unsigned char *)0x28000);

	init(8);

	sprintf(f,"%sbreakout_lib",drive);
	loadLibrary(&lib,f,1);

	setup();

	while(1);
}
