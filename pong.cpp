// pong.cpp : Defines the entry point for the console application.

//commented headers are used in windows

//#include "stdafx.h"

#include <string.h>
#include <ctype.h>
//#include <cstring.h>
//#include <windows.h>
#include <iostream>
//#include <conio.h>
#include <sstream>
#include <math.h>

//#include <gl\glu.h>
//#include "GL/freeglut.h"
#include <GL/glut.h>


// window screen number
int view=0;
// window size and update rate (60 fps)
int width = 500;
int height = 200;
int interval = 1000 / 60;

// score
int score_left = 0;
int score_right = 0;
char str[30];

// rackets in general
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// right racket position
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

// ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 3;


void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}


//to display bitmap char (strings)
void display_string(int x, int y, char *string, int font)
{
    int len,i;
	glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
    if(font==1)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
    if(font==2)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	}

}


void drawMenu(){
	display_string(width/2-35,150,"PONG",1);
	display_string(width/3+30,120,"Press Y to Play",2);
	display_string(width/3+30,100,"Press H for Help",2);
	display_string(width/3+30,80,"Press I for Info",2);
	glutPostRedisplay();
	
}

void drawHelp(){
	display_string(width/2-35,150,"HELP",1);
	display_string(30,120,"Press W/S to Move Left plank Up/Down Respectively",2);
	display_string(30,100,"Press 8/5 to Move Right plank Up/Down Respectively",2);
	display_string(30,80,"Points are awarded when ball exits at opponent's side",2);
	display_string(30,60,"Press Q to pause game",2);
	glutPostRedisplay();
	
}

void drawInfo(){
	display_string(width/2-55,150,"Developers",1);
	display_string(width/3+10,120,"Sanketh (4VP19CS078)",2);
	display_string(width/3+10,100,"Sandesh S. C.(4VP19CS078)",2);
	glutPostRedisplay();
	
}


void draw() {
    // clear (has to be done at the beginning)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	if(view==0) drawMenu();   // Main menu window

	else if(view==1) drawHelp();  //Help window
    
	else if(view==3) drawInfo();     //Developer info

	else if(view==2){
		// draw rackets
		drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
		drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

		// draw ball 
		 drawRect(ball_pos_x, ball_pos_y, ball_size, ball_size);

		 // draw score
		 
		 sprintf(str,"%d:%d",score_left,score_right);
	
		 display_string(width / 2 - 10, height - 15, str,2);

	}

    // swap buffers (has to be done at the end)
    glutSwapBuffers();
}

void keyboard(unsigned char b,int x,int y) {
    // left racket
    if ((b=='w'|| b=='W')&& ((racket_left_y + racket_height)<=height)) racket_left_y += racket_speed;
    if ((b=='s'|| b=='S') && ( racket_left_y >=0)) racket_left_y -= racket_speed;
   
    // right racket
    if ((b=='8') && ((racket_right_y + racket_height)<=height)) racket_right_y += racket_speed;
    if ((b=='5') && ( racket_right_y >=0)) racket_right_y -= racket_speed;

	// Navigation
	if((b=='q'|| b=='Q')) view=0;     //mainmenu
	if((b=='y'|| b=='Y')) view=2;     //game window
	if((b=='h'|| b=='H')) view=1;     //help window
	if((b=='i'|| b=='I')) view=3;     //Developer window
}

void vec2_norm(float& x, float &y) {
        // sets a vectors length to 1 (which means that x + y == 1)
        float length = sqrt((x * x) + (y * y));
        if (length != 0.0f) {
            length = 1.0f / length;
            x *= length;
            y *= length;
        }
    }

void updateBall() {
    // fly a bit
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;

	
    // hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = t;
    }
   
    // hit by right racket?
    if (ball_pos_x > racket_right_x &&
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y) {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = t;
    }

    // hit left wall?
    if (ball_pos_x < 0) {
        ++score_right;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = fabs(ball_dir_x); // force it to be positive
        ball_dir_y = 0;
    }

    // hit right wall?
    if (ball_pos_x > width) {
        ++score_left;
        ball_pos_x = width / 2;
        ball_pos_y = height / 2;
        ball_dir_x = -fabs(ball_dir_x); // force it to be negative
        ball_dir_y = 0;
    }

    // hit top wall?
    if (ball_pos_y > height) {
        ball_dir_y = -fabs(ball_dir_y); // force it to be negative
    }

    // hit bottom wall?
    if (ball_pos_y < 0) {
        ball_dir_y = fabs(ball_dir_y); // force it to be positive
    }

    // make sure that length of dir stays at 1
    vec2_norm(ball_dir_x, ball_dir_y);

}

void update(int value) {
    
   if(view==2){
   // update ball
   updateBall();
   }


   // Call update() again in 'interval' milliseconds
   glutTimerFunc(interval, update, 0);
   

   // Redisplay frame
   glutPostRedisplay();
}
void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}



// program entry point
int main(int argc, char** argv) {
    // initialize opengl (via glut)
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Pong");
    
    glutKeyboardFunc(keyboard);
	// Register callback functions  
    glutDisplayFunc(draw);
	
    glutTimerFunc(interval, update, 0);

    // setup scene to 2d mode and set draw color to white
    enable2D(width, height);
    glColor3f(1.0f, 1.0f, 1.0f);

    // start the whole thing
	glutMainLoop();
    return 0;
}

