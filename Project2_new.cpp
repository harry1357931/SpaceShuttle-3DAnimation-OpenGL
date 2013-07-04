/* 
  Project 2 -- Peripheral Drift Illusion
  This project is different from Space Shuttle 3D animation project.
  CSCI 368 (Computer Graphics)
  Prof. Xiang
  Summer 2, 2012    
  Author: Gurpreet Singh

*/
#include<gl/glut.h>

GLfloat centers[8][2]={{100, 100},{300, 100},{500, 100},{700, 100}, {100, 300},{300, 300},{500, 300},{700, 300}};      // Centres of 8 circles...
GLfloat leftborder[8][3] = {{0, 0, 0}, {9.525, 0, 1}, {9.525, 3.57, 0}, {3.57, 3.57, 1}, {3.57, 29.76, 1}, {9.525, 29.76, 0}, {9.525, 33.333, 1}, {0, 33.333, 0}};             // {x-coordinate, y-coordinate, Border-User-defined color code}
GLfloat rightborder[8][3] = {{9.525, 3.57, 1}, {9.525, 0, 0}, {19.05, 0, 1}, {19.05, 33.333, 0}, {9.525, 33.333, 0}, {9.525, 29.76, 1}, {15.48, 29.76, 0}, {15.48, 3.57, 1}};
GLfloat bluerectangle[4][2] = {{3.57, 3.57}, {15.48, 3.57}, {15.48, 29.76}, {3.57, 29.76}};

void draw_left_border(int rn){                                                       // rn = main ring number

	glColor3f(leftborder[rn][2], leftborder[rn][2], leftborder[rn][2]);              // Black left border
	glBegin(GL_POLYGON);                                                             // left border
	   for(int i=0; i<8; i++)
	   {
	     glVertex2f(leftborder[i][0], leftborder[i][1]);
	   }
    glEnd();
}

void draw_blue_rectangle(void){
	glColor3f(0, 0, 1);              // Blue Color
	glBegin(GL_POLYGON);             // Blue rectangle
	   for(int i=0; i<4; i++)
	   {
	     glVertex2f(bluerectangle[i][0], bluerectangle[i][1]);
	   }
    glEnd();
}

void draw_right_border(int rn){
	glColor3f(rightborder[rn][2], rightborder[rn][2], rightborder[rn][2]);              // White Color
	glBegin(GL_POLYGON);             // White Right border
	   for(int i=0; i<8; i++)
	   {
	     glVertex2f(rightborder[i][0], rightborder[i][1]);
	   }
    glEnd();
}

void draw_main_rectangle(float rn){            // rn = main ring number
    draw_left_border(rn);
    draw_right_border(rn);
    draw_blue_rectangle();
  
}

void draw_ring(float rn){                   // rn = main ring number
    for(int r=0; r<360; r+=45){
	    glPushMatrix();
		glRotated(r, 0, 0, 1);              // need to modify....other stuff too....
		glTranslated(-9.525, 50, 0);        // middle value = 50 is radius of the circle....
		draw_main_rectangle(rn);
		glPopMatrix();
	}
}

void display(void){

	 glClear(GL_COLOR_BUFFER_BIT);
	 
     for(int i=0; i<8; i++){
	    
        glPushMatrix();                                     // Save a copy of the identity matrix
	    glTranslated(centers[i][0], centers[i][1], 0);      // Translate the centre of new ring
	    draw_ring(i);                                        // Draw current ring
	    glPopMatrix();                                      // Reset CTM to identity for next ring  

	 }
	 
	 glFlush();
   
 }

void main(int argc, char **argv){
		
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(800, 400);
	 glutInitWindowPosition(100, 100);       //upper-let of display to upper-left of windows. 
     glutCreateWindow("Peripheral Drift Illusion: By Gurpreet Singh");
	 glClearColor(1.0, 1.0, 0.0, 0.0);      //sets background color (r,g,b,alpha) for the window.
     glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
     gluOrtho2D(0, 800, 0, 400);	        //sets object space window to image area
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
     glutDisplayFunc(display);              //register display call back function
	 glutMainLoop();                        //enter event-processing loop, always needs this line. 
     
}


  
