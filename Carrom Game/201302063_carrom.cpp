/* Program to Simulate a Carrom Board Game in 2D using OpenGL */

//Header Files
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>  //include this to use string streams
#include <string> 
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//Pre-defined Macros
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

using namespace std;

//Function Prototypes
void initRendering();
void setobjects();
void handleResize(int w, int h) ;
void drawgame();
void movestriker(int key, int x, int y);
void keyPressed (unsigned char key, int x, int y) ;
void scoreboard(int value);
void collision(int i, int j);
void resetcoin(int i);
void reset_striker();
void collision(int i, int j);
void update(int value);


//Class for the Board
class board
{
	public:
		int nw1, nw2; //Number of coins on board
		int nb1, nb2;
		int queen;
		int flag_strike, flag_initialize, time_elapsed; 
		char choice; //Black or white coin is preferred?
		int score1, score2;
		int play;
		float tx,ty,tz,theta,th; //For Power Marker on Striker

		void board_set(int a, int b, int c, char cho, int score); //Constructor Function
		void drawquad(float x, float y); //To draw a quad
		void drawScene(); // To draw the entire board scenario
		void createcircle(float rad); //To draw a circle
		void drawline(float a, float b, float c, float d); //To draw a line
		void createcoins(float p1, float p2); //To create and draw all board coins
		void drawTriangle(float val); //To draw a triangle
		void drawBox(float x, float y); // To draw a lined box
		void text(); //Function for printing text on screen
		void pouch(); //Function for collecting player stack of coins
		void instruct(); //Function for displaying game instructions
};

//Class for a ball
class ball
{
	public:
		float ball_rad;
		float points1, points2; //Points obtained of scoring
		char type; // Black, White, Queen or just a pocket?
		float x, y, z; //Location
		float ball_velx, ball_vely;//velocity values of the ball
		float mass; //Mass of the ball

		//Setting Function
		void set(char c,float velx,float vely,float r=0, float p=0, float x_cor=0, float y_cor=0, float z_cor=0, float mass=0);
		void drawBall(); //Function to draw a ball of 'ball_rad' radius
		void set_velx(float value);
		void set_vely(float value);
};

//Declaring the Board & coins Objects
board B;
ball coins[7];
ball strike;
ball pocket[4];

//Member Functions Of ball class
void ball::set(char c,float velx, float vely,float r, float p, float x_cor, float y_cor, float z_cor, float mass)
{
	ball_rad=r;
	points1=points2=p;
	type=c;
	x=x_cor;
	y=y_cor;
	z=z_cor;
	ball_velx=velx;
	ball_vely=vely;
	mass=mass;
}
void ball::set_velx(float value){
	ball_velx=value;
}
void ball::set_vely(float value){
	ball_vely=value;
}		

//Function Declarations of the classes
void ball::drawBall() {

	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<720 ; i++) {
		glVertex2f(ball_rad * cos(DEG2RAD(i)), ball_rad * sin(DEG2RAD(i)));
	}
	glEnd();
}

//Member Functions of Board Class
void board::board_set(int a, int b, int c, char cho, int score)
{
	nw1=nw2=nb1=nb2=0;
	queen=0;
	choice=cho;
	score1=30;
	score2=30;
}

void board::drawquad(float x, float y){
	glBegin(GL_QUADS);
	glVertex2f(-x,-y);
	glVertex2f(x,-y);
	glVertex2f(x,y);
	glVertex2f(-x,y);
	glEnd();
}

void board::drawBox(float x, float y) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-x,-y);
	glVertex2f(x,-y);
	glVertex2f(x,y);
	glVertex2f(-x,y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void board::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -24.0f);

	//big board
	glColor3f(0.38,0.282,0.219);
	drawquad(6,6);

	//inner board
	glColor3f(1,0.904,0.729);
	drawquad(5,5);

	//pockets
	glPushMatrix(); 
	glTranslatef(pocket[0].x, pocket[0].y, pocket[0].z);
	glColor3f(0.082,0.031,0.0078);
	pocket[0].drawBall();
	glPopMatrix();

	glPushMatrix(); 
	glTranslatef(pocket[1].x, pocket[1].y, pocket[1].z);
	glColor3f(0.082,0.031,0.0078);
	pocket[1].drawBall();
	glPopMatrix();

	glPushMatrix(); 
	glTranslatef(pocket[2].x, pocket[2].y, pocket[2].z);
	glColor3f(0.082,0.031,0.0078);
	pocket[2].drawBall();
	glPopMatrix();

	glPushMatrix(); 
	glTranslatef(pocket[3].x, pocket[3].y, pocket[3].z);
	glColor3f(0.082,0.031,0.0078);
	pocket[3].drawBall();
	glPopMatrix();

	//inner-bigcircle
	glPushMatrix(); 
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.8,0.0,0.0);
	createcircle (1.2f);
	glPopMatrix();

	glPushMatrix(); 
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.8,0.0,0.0);
	createcircle (1.2f);
	glPopMatrix();

	//left-top
	glColor3f(0.8,0.0,0.0);
	drawline(-3.8,3.5,-3.8,-3.5);

	glPushMatrix(); 
	glTranslatef(-3.5f, 3.5f, 0.0f);
	createcircle(0.3f);
	glPopMatrix();

	//left-bottom	
	drawline(-3.5,3.8,3.5,3.8);

	glPushMatrix(); 
	glTranslatef(3.5f, 3.5f, 0.0f);
	createcircle(0.3f);
	glPopMatrix();

	//right-bottom
	drawline(3.8,3.5,3.8,-3.5);

	glPushMatrix(); 
	glTranslatef(3.5f,-3.5f, 0.0f);
	createcircle(0.3f);
	glPopMatrix();

	//right-top
	drawline(3.5,-3.8,-3.5,-3.8);

	glPushMatrix(); 
	glTranslatef(-3.5f,-3.5f, 0.0f);
	createcircle(0.3f);
	glPopMatrix();

	switch(choice){
		case 'w': createcoins(10.0, -5.0);
			  break;
		case 'b': createcoins(-5.0, 10.0);
			  break;
	}

	glPushMatrix();
	glTranslatef(B.tx, B.ty, B.tz);
	glRotatef(B.theta, 0.0f, 0.0f, 1.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	drawTriangle(B.th);
	glPopMatrix();
	
	text();

	glPopMatrix();

	glutSwapBuffers();
}

void board::text(){

//Score, Timer & Instructions Display

	glPushMatrix();
	glTranslatef(-10.0f,0.5f,0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawBox(3,2.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f,0.5f,0.0f);
	glColor3f(0.38,0.282,0.619);
	drawquad(3,2.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f,1.7f,0.0f);
	glColor3f(1,0.904,0.729);
	drawquad(2.7,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f,-1.0f,0.0f);
	glColor3f(1,0.904,0.729);
	drawquad(2.7,1);
	glPopMatrix();

	//SCOREBOARD SIGN
	glPushMatrix();
	glTranslatef(-10.0f,3.7f,0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawquad(3,1.0);
	glPopMatrix();

	glColor3f(1.0,1.0,0.7);
	glRasterPos2d(-11.8f,3.5f);
	char scorebo[]="SCOREBOARD";
	for(int j = 0; scorebo[j] != '\0'; j++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , scorebo[j]);

	glColor3f(0.2,0.3,0.3);
	glRasterPos2d(-12.25f,1.5f);
	char str1[]="PLAYER 1";
	for(int j = 0; str1[j] != '\0'; j++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , str1[j]);

	if(B.choice=='w'){
		char col[]="(W)";
		for(int j = 0; col[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , col[j]);
	}else if(B.choice=='b'){
		char col[]="(B)";
		for(int j = 0; col[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , col[j]);
	}

	std::ostringstream ostr1; 
	ostr1 << B.score1; 
	std::string buffer1 = ostr1.str(); 

	glRasterPos2d(-8.5f,1.5f);
	for(int l = 0; buffer1[l] != '\0'; l++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , buffer1[l]);


	glPushMatrix();
	glTranslatef(0.0f,7.7f,0.0f);
	glColor3f(0.5f, 0.0f, 0.0f);
	drawquad(5,1.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f,7.7f,0.0f);
	glColor3f(0.9,0.504,0.729);
	drawquad(3.7,0.5);
	glPopMatrix();

	//Mentions the player who's on strike
	glColor3f(0.2,0.3,0.3);
	glRasterPos2d(-3.3f,7.5f);

	char strname[]="WELCOME TO CARROM 101!";

	for(int j = 0; strname[j] != '\0'; j++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , strname[j]);

	glColor3f(0.2,0.3,0.3);
	glRasterPos2d(-2.0f,-7.5f);

	char strname1[]="PLAYER 1's TURN";
	char strname2[]="PLAYER 2's TURN";

	if(B.play==1){
		for(int j = 0; strname1[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , strname1[j]);
}
	else{
		for(int j = 0; strname2[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , strname2[j]);
	}

	
	glColor3f(0.2,0.3,0.3);
	glRasterPos2d(-12.25f,-1.2f);
	char strpl[]="PLAYER 2";
	for(int j = 0; strpl[j] != '\0'; j++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , strpl[j]);

	if(B.choice=='w'){
		char col[]="(B)";
		for(int j = 0; col[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , col[j]);
	}else if(B.choice=='b'){
		char col[]="(W)";
		for(int j = 0; col[j] != '\0'; j++)
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , col[j]);
	}

	std::ostringstream ostrpl; 
	ostrpl << B.score2; 
	std::string bufferpl = ostrpl.str(); 

	glRasterPos2d(-8.5f,-1.2f);
	for(int l = 0; bufferpl[l] != '\0'; l++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , bufferpl[l]);


	glPushMatrix();
	glTranslatef(11.0f,-3.5f,0.0f);
	glColor3f(0.4f, 0.7f, 0.0f);
	drawquad(3,1.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(11.0f,-3.5f,0.0f);
	glColor3f(0.6,0.904,0.729);
	drawquad(2.7,1);
	glPopMatrix();

	glColor3f(0.8,0.0,0.0);
	glRasterPos2d(10.4f,-3.6f);
	char str2[]="TIMER(s)";
	for(int m = 0; str2[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , str2[m]);

	std::ostringstream ostr; 
	ostr << B.time_elapsed; 
	std::string buffer = ostr.str(); 
	glRasterPos2d(9.0f,-3.6f);
	for(int k = 0; buffer[k] != '\0'; k++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , buffer[k]);
	

	glPushMatrix();
	glTranslatef(0.0f,2.0f,0.0f);

	glPushMatrix();
	glTranslatef(11.0f,0.5f,0.0f);
	glColor3f(0,8.904,0.529);
	drawquad(4.5,3.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(11.0f,0.5f,0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawBox(4.5,3.5);
	glPopMatrix();

	instruct();

	pouch();	

	glPopMatrix();

}

//Coins Dock
void board::pouch(){

	float xpos=-12.0f;
	float ypos=-0.9f;

	if(nw1>0){
		for(int i=1; i<=nw1;i++)
		{	
			//white ball
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0.96f,0.77f, 0.643f);
			coins[1].drawBall();
			glPopMatrix();
			xpos+=0.5;

		}
	}
	if(nb1>0){
		for(int i=1; i<=nb1;i++)
		{	
			//black ball
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0,0,0);
			coins[1].drawBall();
			glPopMatrix();
			xpos+=0.5;

		}
	}
	if(queen==1){
		
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0.8,0.0,0.0);
			coins[0].drawBall();
			glPopMatrix();
			xpos+=0.5;
	}

	xpos=-12.0f;
	ypos=-3.6f;
	
	if(nw2>0){
		for(int i=1; i<=nw2;i++)
		{	
			//white ball
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0.96f,0.77f, 0.643f);
			coins[1].drawBall();
			glPopMatrix();
			xpos+=0.5;

		}
	}
	if(nb2>0){
		for(int i=1; i<=nb2;i++)
		{	
			//black ball
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0,0,0);
			coins[1].drawBall();
			glPopMatrix();
			xpos+=0.5;

		}
	}
	if(queen==2){
			glPushMatrix(); 
			glTranslatef(xpos,ypos,0.0f);
			glColor3f(0.8,0.0,0.0);
			coins[0].drawBall();
			glPopMatrix();
			xpos+=0.5;
	}

}

void board::instruct(){

	glRasterPos2d(9.6f,3.4f);
	char rule[]="Instructions";
	for(int m = 0; rule[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , rule[m]);

	
	glRasterPos2d(7.2f,2.8f);
	char rule1[]="1. To initiate the game, choose ";
	for(int m = 0; rule1[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13 , rule1[m]);

	glRasterPos2d(7.8f,2.4f);
	char rule11[]="'w' (white) or 'b' (black)";
	for(int m = 0; rule11[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13 , rule11[m]);

	glRasterPos2d(7.2f,1.5f);
	char rule2[]="2. Each coin of your chosen colour";
	for(int m = 0; rule2[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, rule2[m]);

	glRasterPos2d(7.8f,1.0f);
	char rule21[]="will give you 10 poins on pocketing";
	for(int m = 0; rule21[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13 , rule21[m]);


	glRasterPos2d(7.8f,0.5f);
	char rule3[]="and a penalty of -5 for any other.";
	for(int m = 0; rule3[m] != '\0'; m++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, rule3[m]);

	glRasterPos2d(7.2,-0.5);
	char rule4[]="3. Press 'ENTER' once your turn";
	for(int i = 0; rule4[i] != '\0'; i++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, rule4[i]);

	glRasterPos2d(7.8,-1.0);
	char rule41[]="is finished";
	for(int i = 0; rule41[i] != '\0'; i++)
		glutBitmapCharacter( GLUT_BITMAP_8_BY_13, rule41[i]);

}

void board::createcoins(float p1, float p2){
	//coins

	//queen
	glPushMatrix(); 
	glTranslatef(coins[0].x,coins[0].y,coins[0].z);
	glColor3f(0.8,0.0,0.0);
	coins[0].drawBall();
	glPopMatrix();
	coins[0].points1=coins[0].points2=50;

	//Striker
	glPushMatrix(); 
	glTranslatef(strike.x,strike.y,strike.z);
	glColor3f(0.596f, 0.596f, 0.596f);
	strike.drawBall();
	glPopMatrix();
	strike.points1=strike.points2=-5;

	if(coins[1].points1==0 )
	{
		coins[1].points1=p1;
		coins[1].points2=p2;
	}

	if(coins[2].points1==0)
	{
		coins[2].points1=p1;
		coins[2].points2=p2;
	}

	if(coins[3].points1==0)
	{
		coins[3].points1=p1;
		coins[3].points2=p2;
	}
	//w1 
	glPushMatrix(); 
	glTranslatef(coins[1].x,coins[1].y,coins[1].z);
	glColor3f(0.96f,0.77f, 0.643f);
	coins[1].drawBall();
	glPopMatrix();

	//w2	
	glPushMatrix(); 
	glTranslatef(coins[2].x,coins[2].y,coins[2].z);
	glColor3f(0.96f,0.77f, 0.643f);
	coins[2].drawBall();
	glPopMatrix();

	//w3	
	glPushMatrix(); 
	glTranslatef(coins[3].x,coins[3].y, coins[3].z);
	glColor3f(0.96f,0.77f, 0.643f);
	coins[3].drawBall();
	glPopMatrix();

	if(coins[4].points1==0)
	{
		coins[4].points1=p2;
		coins[4].points2=p1;
	}

	if(coins[5].points1==0)
	{
		coins[5].points1=p2;
		coins[5].points2=p1;
	}

	if(coins[6].points1==0)
	{
		coins[6].points1=p2;
		coins[6].points2=p1;
	}

	//b1
	glPushMatrix(); 
	glTranslatef(coins[4].x,coins[4].y,coins[4].z);
	glColor3f(0.0,0.0,0.0);
	coins[4].drawBall();
	glPopMatrix();

	//b2
	glPushMatrix(); 
	glTranslatef(coins[5].x,coins[5].y,coins[5].z);
	glColor3f(0.0,0.0,0.0);
	coins[5].drawBall();
	glPopMatrix();

	//b3
	glPushMatrix(); 
	glTranslatef(coins[6].x,coins[6].y, coins[6].z);
	glColor3f(0.0,0.0,0.0);
	coins[6].drawBall();
	glPopMatrix();

}

void board::createcircle(float rad)
{
	glBegin(GL_LINE_LOOP);
	double d;
	for(d=0;d<=360;d++)
	{
		double angle=d*((2*PI)/360);
		double x=cos(angle)*rad;
		double y=sin(angle)*rad;
		glVertex3d(x,y,0);
	}
	glEnd();
}

void board::drawline(float a, float b, float c, float d)
{
	glBegin(GL_LINES);
	glVertex2f(a,b);
	glVertex2f(c,d);
	glEnd();

}

void board::drawTriangle(float val) 
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, val, 0.0f);
	glVertex3f(-0.3f, -0.3f, 0.0f);
	glVertex3f(0.3f, -0.3f, 0.0f);
	glEnd();
}

//Other Functions
void setobjects(){

	//Setting the Pockets
	pocket[0].set('p', 0.0f,0.0f,0.4,0,-4.6f, 4.6f, 0.0f,0);
	pocket[1].set('p', 0.0f,0.0f,0.4,0,4.6f, 4.6f, 0.0f,0);
	pocket[2].set('p', 0.0f,0.0f,0.4,0,4.6f, -4.6f, 0.0f,0);
	pocket[3].set('p', 0.0f,0.0f,0.4,0,-4.6f, -4.6f, 0.0f,0);


	//Setting the coins
	coins[0].set('q',0.0f,0.0f,0.2,50,0,0,0,1);
	coins[1].set('w',0.0f,0.0f,0.2,0,0.8,0.5,0,1);
	coins[2].set('w',0.0f,0.0f,0.2,0,-0.8,0.5,0,1);
	coins[3].set('w',0.0f,0.0f,0.2,0,0,-0.95,0,1);
	coins[4].set('b',0.0f,0.0f,0.2,0,0.8,-0.5,0,1);
	coins[5].set('b',0.0f,0.0f,0.2,0,-0.8,-0.5,0,1);
	coins[6].set('b',0.0f,0.0f,0.2,0,0,0.95,0,1);

	//Setting the Striker
	strike.set('s',0.0f,0.0f,0.3,-5,0.0,-3.5,0,2);

	//Setting the scoreboard
	B.score1=30;
	B.score2=30;
	B.tx = 0.0f;
	B.ty = -3.5f;
	B.tz = 0.0f;
	B.theta = 0.0f; 
	B.th = 1.0f;
	B.flag_strike = 0;
	B.time_elapsed = 0;
	B.flag_initialize=0;
	B.play=1;
	B.nw1=B.nw2=B.nb1=B.nb2=B.queen=0;
}

//Functions required for OpenGL rendering
void initRendering(){
	// We enable 
	glEnable(GL_DEPTH_TEST);      // For 3D modelling (or for overlapping figures in 2D or 3D)
	glEnable(GL_COLOR_MATERIAL);  //Enable color
	glClearColor(1.0f, 1.0f, 0.8f, 1.0f); //Change the background to sky blue
}

//Called when the window is resized
void handleResize(int w, int h) 
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 0.1, 200.0);   // openGL renders the viecone using the coords 1.0 and 200.0
}

void keyPressed (unsigned char key, int x, int y) 
{
	float hitpower;

	switch(key){
		case 'w': 
			if(!B.flag_initialize){
				B.choice = 'w';
				B.flag_initialize = 1;
				glutTimerFunc(1000, scoreboard, 0);
			}
			break;
		case 'b': 
			if(!B.flag_initialize){
				B.choice = 'b';
				B.flag_initialize = 1;
				glutTimerFunc(1000, scoreboard, 0);
			}
			break;
		case 'a':
			if(!B.flag_strike && B.flag_initialize){
				B.theta += 2.0f;
			}
			break;
		case 'c':
			if(!B.flag_strike && B.flag_initialize){
				B.theta -= 2.0f;
			}
			break;
		case 32:
			hitpower = B.th/80;
			if(!B.flag_strike && B.flag_initialize){
				B.tz = 205.0f;
				B.flag_strike = 1;
				strike.set_velx(hitpower*sin(DEG2RAD(-B.theta)));
				strike.set_vely(hitpower*cos(DEG2RAD(-B.theta)));
			}
			break;
		case 13: 
			if(B.flag_strike && B.flag_initialize){
				B.flag_strike = 0;
				reset_striker();
				if(B.play==1){
					B.score1-=strike.points1;
					B.play=2;	
				}
				else if(B.play==2){
					B.score2-=strike.points1;
					B.play=1;
				}
				
			}
			break;
		case 27: exit(0);
	}

}

//Called to draw the scene
void drawgame(){
	B.drawScene();
}

void scoreboard(int value)
{   
	if(B.play==1)
		B.score1 -= 1;
	else if(B.play==2)
		B.score2 -= 1;

	B.time_elapsed += 1;
	glutTimerFunc(1000, scoreboard, 0);
}

void resetcoin(int i)
{
	coins[i].x = coins[i].y = coins[i].z = 205.0f;
	if(B.play==1){
		
		B.score1 += coins[i].points1;
		if(i==1||i==2||i==3)
			B.nw1++;
		if(i==4||i==5||i==6)
			B.nb1++;
		if(i==0)
			B.queen=1;
	}
	else if(B.play==2){
		
		B.score2 += coins[i].points2;
		if(i==1||i==2||i==3)
			B.nw2++;
		if(i==4||i==5||i==6)
			B.nb2++;
		if(i==0)
			B.queen=2;
	}
}

void reset_striker()
{
	strike.x = 0.0f;
	strike.y = -3.5f; 
	strike.ball_velx = 0.0f; 
	strike.ball_vely = 0.0f; 
	B.tx = 0.0f;
	B.ty = -3.5f; 
	B.tz = 0.0f;
	B.theta = 0.0f; 
	B.th = 1.0f;
	B.flag_strike = 0;
	if(B.play==1){
		B.score1 += strike.points1;
		
	}
	else if(B.play==2){
		B.score2 += strike.points2;
		
	}

}

void movestriker(int key, int x, int y) {

	switch(key){
		case GLUT_KEY_LEFT: 
			if(strike.x >= -3.4f && !B.flag_strike && B.flag_initialize){
				strike.x -= 0.1;
				B.tx -= 0.1;
			}
			break;
		case GLUT_KEY_RIGHT:
			if(strike.x <= 3.4f && !B.flag_strike && B.flag_initialize ){
				strike.x += 0.1;
				B.tx += 0.1;
			}
			break;
		case GLUT_KEY_UP: 
			if (B.th <= 20.0f && !B.flag_strike && B.flag_initialize){
				B.th += 0.3f;
			}
			break;
		case GLUT_KEY_DOWN:
			if (B.th >= 1.0f && !B.flag_strike && B.flag_initialize){
				B.th -= 0.3f;
			}
			break;
	}

}

void collision_coins(int i, int j)
{
	float D, vx, vy, dot_prod, scale, xc, yc;

	D = pow(coins[i].x - coins[j].x,2) + pow(coins[i].y - coins[j].y,2);

	if(D <= (coins[j].ball_rad + coins[i].ball_rad)*(coins[j].ball_rad + coins[i].ball_rad))
	{
		vx = coins[j].ball_velx - coins[i].ball_velx;
		vy = coins[j].ball_vely - coins[i].ball_vely;
		dot_prod = (coins[i].x - coins[j].x)*vx + (coins[i].y - coins[j].y)*vy;

		if(dot_prod > 0)
		{
			scale = dot_prod/D;
			xc = (coins[i].x - coins[j].x)*scale;
			yc = (coins[i].y - coins[j].y)*scale;

			coins[i].ball_velx += xc;
			coins[i].ball_vely += yc;
			coins[j].ball_velx -= xc;
			coins[j].ball_vely -= yc;
		}
	}


}

void update(int value) {
	int i,k;
	float D, vx, vy, dot_prod, scale, xc, yc;
	float hitpower = B.th/80;
	float temp = 1 - (0.003/hitpower);
	
	for(i=0;i<7;i++)
	{
		D = pow(coins[i].x - strike.x, 2)+ pow(coins[i].y - strike.y, 2);

		//Collision of Striker & Coins	
		if(D <= (strike.ball_rad + coins[i].ball_rad)*(strike.ball_rad + coins[i].ball_rad))
		{
			vx = strike.ball_velx - coins[i].ball_velx;
			vy = strike.ball_vely - coins[i].ball_vely;
			dot_prod = (coins[i].x - strike.x)*vx + (coins[i].y - strike.y)*vy;

			if(dot_prod > 0)
			{
				scale = dot_prod/D;
				xc = (coins[i].x - strike.x)*scale;
				yc = (coins[i].y - strike.y)*scale;

				coins[i].ball_velx += (4/3.0)*xc;
				coins[i].ball_vely += (4/3.0)* yc;
				strike.ball_velx -= (2/3.0) * xc;
				strike.ball_vely -= (2/3.0) * yc;

			}
		}

		coins[i].ball_velx*=temp;
		coins[i].ball_vely*=temp;

		coins[i].x += coins[i].ball_velx;
		coins[i].y += coins[i].ball_vely;

		for(k=0;k<7;k++)
		{
			if(i!=k)
				collision_coins(i,k);
		}

		//Pocketing the coins
		if( ((coins[i].x - pocket[0].x)*(coins[i].x - pocket[0].x) + (coins[i].y - pocket[0].y)*(coins[i].y - pocket[0].y)) < (pocket[0].ball_rad * pocket[0].ball_rad) )
			resetcoin(i);
		if( ((coins[i].x - pocket[1].x)*(coins[i].x - pocket[1].x) + (coins[i].y - pocket[1].y)*(coins[i].y - pocket[1].y)) < (pocket[1].ball_rad * pocket[1].ball_rad) )
			resetcoin(i);
		if( ((coins[i].x - pocket[2].x)*(coins[i].x - pocket[2].x) + (coins[i].y - pocket[2].y)*(coins[i].y - pocket[2].y)) < (pocket[2].ball_rad * pocket[2].ball_rad) )
			resetcoin(i);
		if( ((coins[i].x - pocket[3].x)*(coins[i].x - pocket[3].x) + (coins[i].y - pocket[3].y)*(coins[i].y - pocket[3].y)) < (pocket[3].ball_rad * pocket[3].ball_rad) )
			resetcoin(i);

		//Coins hitting the boundaries
		if(coins[i].x + coins[i].ball_rad > 5 || coins[i].x - coins[i].ball_rad < -5)
			coins[i].ball_velx *= -1;

		if(coins[i].y + coins[i].ball_rad > 5 || coins[i].y - coins[i].ball_rad < -5)
			coins[i].ball_vely *= -1;
	}

	// Handle Striker's collisions with box
	if(strike.x + strike.ball_rad > 5 || strike.x - strike.ball_rad < -5)
		strike.ball_velx *= -1;
	if(strike.y + strike.ball_rad > 5 || strike.y - strike.ball_rad < -5)
		strike.ball_vely *= -1;

	//Friction
	strike.ball_velx*=temp;
	strike.ball_vely*=temp;

	strike.x += strike.ball_velx;
	strike.y += strike.ball_vely;

	//Pocketing the Striker
	if( ((strike.x - pocket[0].x)*(strike.x - pocket[0].x) + (strike.y - pocket[0].y)*(strike.y - pocket[0].y)) < (pocket[0].ball_rad * pocket[0].ball_rad) )
		reset_striker(); 
	if( ((strike.x - pocket[1].x)*(strike.x - pocket[1].x) + (strike.y - pocket[1].y)*(strike.y - pocket[1].y)) < (pocket[1].ball_rad * pocket[1].ball_rad) )
		reset_striker();
	if( ((strike.x - pocket[2].x)*(strike.x - pocket[2].x) + (strike.y - pocket[2].y)*(strike.y - pocket[2].y)) < (pocket[2].ball_rad * pocket[2].ball_rad) )
		reset_striker(); 
	if( ((strike.x - pocket[3].x)*(strike.x - pocket[3].x) + (strike.y - pocket[3].y)*(strike.y - pocket[3].y)) < (pocket[3].ball_rad * pocket[3].ball_rad) )
		reset_striker(); 

	glutTimerFunc(10, update, 0);

}

int main(int argc, char** argv)
{
	//Settin of Object Values
	setobjects();	
/*
	struct timeval tim;
	gettimeofday(&tim,NULL);
	t1=tim.tv_sec+(tim.tv_usec/1000000.0);  
*/	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );  
	glutInitWindowSize(1360, 1080);

	//Create the window
	glutCreateWindow("CARROMS GAME V1.0");
	initRendering();

	//Set handler functions
	glutDisplayFunc(drawgame);
	glutIdleFunc(drawgame);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keyPressed);

	glutSpecialFunc(movestriker);	
//	glutMouseFunc(mouseButton);

	//glutMotionFunc(mouseMove);
	glutTimerFunc(10, update, 0);
	// Continuously call the callback functions (mentioned above)

	glutMainLoop();

	return 0;
}
