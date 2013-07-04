
/*
#include <gl/glut.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.141593
#define TWOPI 6.28318530717958647692528

int width = 400, height = 600, vert[100][2], v, n=0; //n number of vertices, v: index of vertex for rubber banding 	
int in[100][2], out[100][2], poly[100][2], n_inner=0, n_outer=0, idx_in_v, idx_out_v, pv;
int type = GL_LINE_STRIP;

bool rubberbanding = false, antialiasing = false;

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	if(n==1 && (type==GL_LINE_STRIP || type==GL_LINE_LOOP )){
		glBegin(GL_POINTS);
		glVertex2iv(vert[0]);					// draw the 1st and only vertex for ployline & close polyline
		glEnd();
	}
	if(n_inner>0){								 // for inner points
		glColor3f(0, 1, 0);						 // green points 
		glBegin(GL_POINTS);
	    for(int i=0; i < n_inner; i++)
			glVertex2iv(in[i]);
		glEnd();
	}
	if(n_outer>0){								// for outer points
		glColor3f(1, 0, 0);						// plot red points
		glBegin(GL_POINTS);
	    for(int i=0; i < n_outer; i++)
	         glVertex2iv(out[i]);
		 glEnd();
	}

	glBegin(type);
		for(int i=0;i<n;i++)
			glVertex2iv(vert[i]);				
	glEnd();
	glutSwapBuffers();							// for double animation
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'a': antialiasing = !antialiasing;
				if(antialiasing){
					glEnable(GL_BLEND);
					glEnable(GL_LINE_SMOOTH);
				}
				else {
					glDisable(GL_BLEND);
					glDisable(GL_LINE_SMOOTH);
				}
				break;
		
		case 'i':
				type = GL_LINE_STRIP;
				break;

		case 'c':
				type = GL_LINE_LOOP;
				break;

		case 'r':
				n=0;
				break;
	
		case 'v':
				type = GL_POINTS;
				break;
	}
	glutPostRedisplay();
}

/*
   Return the angle between two polyors on a plane
   The angle is from polyor 1 to polyor 2, positive anticlockwise
   The result is between -pi -> pi
*/

/*

double angleT(double x1, double y1, double x2, double y2)
{
   double dtheta,theta1,theta2;

   theta1 = atan2(y1,x1);
   theta2 = atan2(y2,x2);
   dtheta = theta2 - theta1;
   while (dtheta > PI)
      dtheta -= TWOPI;
   while (dtheta < -PI)
      dtheta += TWOPI;

   return(dtheta);
}

typedef struct {
   int x,y;
} Point;

int test(Point *polygon,Point p, int n)
{
   int i;
   double angle=0;
   Point p1,p2;

   for (i=0;i<n;i++) {
      p1.x = polygon[i].x - p.x;
      p1.y = polygon[i].y - p.y;
      p2.x = polygon[(i+1)%n].x - p.x;
      p2.y = polygon[(i+1)%n].y - p.y;
      angle += angleT(p1.x,p1.y,p2.x,p2.y);
   }

   if (abs(angle) < PI)				// inside polygon
      return 1;
   else
      return 0;						//outside polygon
}

int findVertex(int x, int y){						// find a vertex that is near x,y
	int dx, dy;
	for(int i=0;i<n;i++){
		dx = vert[i][0]-x;
		dy = vert[i][1]-y;
		if(dx*dx+dy*dy<16)							
			return i;								// return a location index in vert
	}
	return -1;										// return -1 if not found
}


void mouse(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
			if(type == GL_LINE_LOOP){							// if polygon formed then start PIP test
				Point poly = {0,0} , in = {0,0}, out = {0,0}, vert = {x,height-1-y} ;
				
				for(int i=0; i < n; i++){           
					poly.x = vert.x - x;
					poly.y = vert.y - height-1-y;
				} 
				if (int a =(test(&poly, vert, pv)!=0)){	
				     cout << a ;										// inside
					idx_in_v = n_inner++;
					in.x = poly.x;
					in.y = poly.y;
				}
				else{													// outside
					cout << a ;														
					idx_out_v = n_outer++;
					out.x = poly.x;
					out.y = poly.y;
				}
			}
			else{
				v= n++;
				vert[v][0] = x;
				vert[v][1] = height-1-y;
				rubberbanding = true;
			}
			glutPostRedisplay();
		}
		else{
			rubberbanding = false;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN && (v=findVertex(x,height-1-y)) != -1){
			if(glutGetModifiers() == GLUT_ACTIVE_CTRL){
				for(int i=v;i<n;i++){
					vert[i][0] = vert[i+1][0];
					vert[i][1] = vert[i+1][1];
				}
				n--;
			}
			else{
				vert[v][0] = x;
				vert[v][1] = height-1-y;
				rubberbanding = true;
			}
			glutPostRedisplay();
		}
		else{
			rubberbanding = false;
		}
		break;
	}
}

void motion(int x, int y){
	if(rubberbanding){							// if rubberbanding is on, update
		vert[v][0] = x;							// vertex indexed by v with new cursor position
		vert[v][1] = height-1-y;
	}
}
void main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(850,150);
	glutCreateWindow("Edit Polygons");
	glClearColor(0.0, 0.0,0.0,0.0);
	glColor3f(1,1,0);						
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glutDisplayFunc(display);			
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);				
	glutMotionFunc(motion);				
	
	glutMainLoop();
}


*/












/* 
  Project 3 -- Inside/Outside Test & Non Intersecting Polygon
  CSCI 368 (Computer Graphics)
  Prof. Xiang
  Summer 2, 2012    
  Author: Gurpreet Singh
  Project Description: 
     1) User draws polyline and then connect first and last vertex to make it a non self intersecting polygon
     2) This program checks if the point marked by user is Inside or Outside the Polygon
	 3) If point is inside -- draws it Green
	 4) If point is outside -- draws it Red
	 5) "r" to reset

*/



#include<iostream>
#include<gl/glut.h>
#include<math.h>
#include <cstdlib>

using namespace std;

int width=800, height=600, n=0, vert[100][2], type= GL_LINE_STRIP, v;   // n: number of vertices
int outer[100][2], num_outer=0, v_o;
int inner[100][2], num_inner=0, v_i;
double vect_dot, vect[100][2];                                         // vect_dot: Dot product of two vectors
double theta, cos_theta, sum_theta=0, vect1_mag, vect2_mag;             // vect1_mag: Magnitude of vector 1

#define pi 3.14159265
bool rubberbanding = false, antialiasing= false;

void display(){
    
	 glClear(GL_COLOR_BUFFER_BIT);
	 if(n==1 && (type==GL_LINE_STRIP || type== GL_LINE_LOOP)){
	   glBegin(GL_POINTS);
	     glVertex2iv(vert[0]);
       glEnd();
	 } 

	 if(num_outer>0){              // Draws Outer points
       
       glColor3f(1, 0, 0);         // Red color
	   glBegin(GL_POINTS);
	     for(int i=0; i < num_outer; i++){
	         glVertex2iv(outer[i]);
		 }
       glEnd();
	 }

	 if(num_inner>0){               // Draws Inner points
	   
       glColor3f(0, 1, 0);          // Green Color 
	   glBegin(GL_POINTS);
	     for(int i=0; i < num_inner; i++){
	         glVertex2iv(inner[i]);
		 }
       glEnd();
	 }

	 
	 glColor3f(1, 1, 0);
	 glBegin(type);
	    for(int i=0; i<n; i++)
		   glVertex2iv(vert[i]);
     glEnd();

     glutSwapBuffers();

}

char direction_Computing(double xi, double yi, double xj, double yj,
                             double xk, double yk) {
  double a = (xk - xi) * (yj - yi);
  double b = (xj - xi) * (yk - yi);
  return a < b ? -1 : a > b ? 1 : 0;
}

bool OnSegmentorNot(double xi, double yi, double xj, double yj,
                        double xk, double yk) {
  return (xi <= xk || xj <= xk) && (xk <= xi || xk <= xj) &&
         (yi <= yk || yj <= yk) && (yk <= yi || yk <= yj);
 }


 // Check IF line segments (x1, y1)-----(x2, y2) and (x3, y3)-----(x4, y4) INTERSECTS OR NOT? 
bool Check_LineSegmentsIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	  char a1 = direction_Computing(x3, y3, x4, y4, x1, y1);
	  
	  char a2 = direction_Computing(x3, y3, x4, y4, x2, y2);
	  
	  char a3 = direction_Computing(x1, y1, x2, y2, x3, y3);
	  
	  char a4 = direction_Computing(x1, y1, x2, y2, x4, y4);
	  
	  return (((a1 > 0 && a2 < 0) || (a1 < 0 && a2 > 0)) &&
	  ((a3 > 0 && a4 < 0) || (a3 < 0 && a4 > 0))) 
	  || (a1 == 0 && OnSegmentorNot(x3, y3, x4, y4, x1, y1)) 
	  ||  (a2 == 0 && OnSegmentorNot(x3, y3, x4, y4, x2, y2)) 
	  ||  (a3 == 0 && OnSegmentorNot(x1, y1, x2, y2, x3, y3)) 
	  ||  (a4 == 0 && OnSegmentorNot(x1, y1, x2, y2, x4, y4));

}


int findVertex(int x, int y){
	int dx, dy;
	for(int i=0; i<n; i++){
	  
		dx = vert[i][0] - x;
		dy = vert[i][1] - y;
		if(dx*dx + dy*dy < 16)
			 return i;
	}

	return -1;

}

void add_inner_point(int x, int y){

	  cout<<"inner"<<endl;    // delete this
	  v_i = num_inner++;
      inner[v_i][0] = x;
      inner[v_i][1] = y;
}

void add_outer_point(int x, int y){
	  
	  cout<<"outer"<<endl;   // delete this
	  v_o = num_outer++;
      outer[v_o][0] = x;
      outer[v_o][1] = y;
}

void comp_all_vectors(int x, int y){        // Compute all vectors from (x,y) to every vertex of Polygon and save them in vector array
	   
	for(int i=0; i < n; i++){           
		vect[i][0] = vert[i][0] - x;
		vect[i][1] = vert[i][1] - y;
	} 
}

// To compute(using Dot Prdouct) all angles and then sum up all angles formed by point to all vertices of polygon.
void sum_all_angles(){

	sum_theta = 0;
	 for(int i=0; i < n; i++) {            // To find: sum_theta   
	 
		vect_dot = vect[i][0]*vect[i+1][0] + vect[i][1]*vect[i+1][1];       // vect_dot = dot product of vectors 
        vect1_mag = sqrt(pow(vect[i][0] ,2) + pow(vect[i][1] ,2));
	    vect2_mag = sqrt(pow(vect[i+1][0] ,2) + pow(vect[i+1][1] ,2));
	    
		cos_theta = vect_dot/(vect1_mag*vect2_mag);

		theta = acos(cos_theta)*(180/pi);
		sum_theta = sum_theta + theta;
		cout<<"cos_theta: "<<cos_theta<<endl;
		cout<<"theta: "<<theta<<endl;
		
    }
	 cout<<"sum_theta: "<<sum_theta<<endl;
}

void check_point(int x, int y){           // to compute the inner and outer points

   	comp_all_vectors(x, y);        // To compute all vectors and store them in vect array
	 
	// Setting vect[last+1] = vect[0] .....for calculation purpose....to compute theta from vect_0_dot_vect_last
    cout<<"n:"<<n<<endl;
	vect[n][0] = vect[0][0];
	vect[n][1] = vect[0][1];
	 
	 // Rough-- To Print Vectors
	 for(int i=0; i < n+1; i++){        
		 cout<<"("<<vect[i][0]<<" ,"<<vect[i][1]<<")"<<endl; 
     }

     sum_all_angles();         // To compute(using Dot Prdouct) all angles and then sum up all angles formed by point to all vertices of polygon. 
	   
     if(sum_theta > 358 && sum_theta < 362){                   // inner   sum_theta == 360
	     add_inner_point(x, y);
	 }
	 else{                                    // outer  
	     add_outer_point(x, y);
	  }

} // function compute ends here... 

bool check_SelfIntersecting(float x, float y, int start){             // if Updated:  new_x = vert[n-1][0], new_y = vert[n-1][1]

	for(int i= start; i<=n-3; i++){          
	   	if( Check_LineSegmentsIntersect(vert[i][0], vert[i][1], vert[i+1][0], vert[i+1][1], x, y, vert[n-1][0], vert[n-1][1])){
		    return true;
		}
	}

	return false;
}

void mouse(int button, int state, int x, int y){

	switch(button){
	
	  case GLUT_LEFT_BUTTON:
		  
		  if(state == GLUT_DOWN) {
             
			  if(type == GL_LINE_LOOP){                  // To check if the polygon is ready....so as to start inner or outer points testing...      
			      check_point(x, height-1-y);            // Check inner or outer point...and then add it to corresponding array array.
			  }
			  else{
		          
				  if(n>=3 && check_SelfIntersecting(x, height-1-y, 0)){
        			     cout<<"The line segment intersects other line segment....Try Other Point"<<endl;
					
                  }else{
	 					v=n++;                              // add a new vertex
						vert[v][0] = x;
						vert[v][1] = height-1-y;
						rubberbanding = true;               // Start rubber banding
                
				  }
              }
			  glutPostRedisplay();
		  }
		  else
		   { 
			  rubberbanding = false;         // End rubber banding
		  }
		  break;
     
	  case GLUT_RIGHT_BUTTON:
		  if(state == GLUT_DOWN && (v==findVertex(x, height-1-y)) != -1) {
			  if(glutGetModifiers() == GLUT_ACTIVE_CTRL){
				  for(int i=v; i< n-1; i++){                     // Delete vertex indexed by v
					  vert[i][0] = vert[i+1][0];
					  vert[i][1] = vert[i+1][1];

			      }
			      n--;  
			   }
			   else
			   {   
				   vert[v][0] = x;                                       // Attach vertex to cursor
				   vert[v][1] = height-1-y;
				   rubberbanding = true;                                 // Start rubber banding
			   }
			   glutPostRedisplay();

		  }
		  else
		  {  rubberbanding = false;                                 //end rubber banding
		  }
		  break;
   }
}

void keyboard(unsigned char key, int x, int y){
	switch(key){

	     case 'a': antialiasing = !antialiasing;
			       if(antialiasing){
			           glEnable(GL_BLEND);
			           glEnable(GL_LINE_SMOOTH);
			       }else{
			           glDisable(GL_BLEND);
			           glDisable(GL_LINE_SMOOTH);
			       }
				   break;
         
		 case 'c': if(check_SelfIntersecting(vert[0][0], vert[0][1], 1)==false){
        			     
					 type = GL_LINE_LOOP;
                   }
				   else{
				       cout<<"The Edge from First Point to Last Point intersects some other Edge"<<endl;
				   }
			       
			       break;
				   
		 case 'l': type= GL_LINE_STRIP;
			       break;
	     
		 case 'r': n=0;                                    // To Reset Again
			       num_inner= 0;
				   num_outer= 0;
				   type= GL_LINE_STRIP;
			       break;
	     
		 case 'v': type= GL_POINTS;
			       break;
	}

	glutPostRedisplay();

}



void motion(int x, int y){

	if(rubberbanding){                     // if rubber banding is on, update
	     vert[v][0] = x;                    // vertex indexed by v with new         
		 vert[v][1] = height-1-y;           // cursor position        
		 glutPostRedisplay();                       
    
	}

}

void main(int argc, char **argv){
	 
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
     glutInitWindowSize(width, height);
	 glutInitWindowPosition(50, 100);       
     glutCreateWindow("Edit Polygons: By Gurpreet Singh");
	 glClearColor(0.0, 0.0, 0.0, 0.0);       
     glColor3f(1,1,0);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
     gluOrtho2D(0, width, 0, height);	             //sets object space window to image area
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 glutDisplayFunc(display);                      //register display call back function
	 glutKeyboardFunc(keyboard);
	 glutMouseFunc(mouse);
	 glutMotionFunc(motion);
	 glutMainLoop();                        
     
}

// Project- 3 Ends here... 




/*								 
								 
							 
  char d1 = ComputeDirection(x3, y3, x4, y4, x1, y1);
  char d2 = ComputeDirection(x3, y3, x4, y4, x2, y2);
  char d3 = ComputeDirection(x1, y1, x2, y2, x3, y3);
  char d4 = ComputeDirection(x1, y1, x2, y2, x4, y4);
  return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
          ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
         (d1 == 0 && IsOnSegment(x3, y3, x4, y4, x1, y1)) ||
         (d2 == 0 && IsOnSegment(x3, y3, x4, y4, x2, y2)) ||
         (d3 == 0 && IsOnSegment(x1, y1, x2, y2, x3, y3)) ||
         (d4 == 0 && IsOnSegment(x1, y1, x2, y2, x4, y4));

 
*/


/* 
  Project 2 -- Peripheral Drift Illusion
  CSCI 368 (Computer Graphics)
  Prof. Xiang
  Summer 2, 2012    
  Author: Gurpreet Singh
  Project Description: 
     1) Draws 4 X 2 rings with yellow background.
	 2) Each ring consists of 8 main rectangles.
	 3) Each main rectangle consists of left and right border, and blue rectangle in the middle.

*/

/*

#include<gl/glut.h>

// Coordinates of Centre of 8 circles...
int centres[8][2] = {{100, 100},{300, 100},{500, 100},{700, 100}, {100, 300},{300, 300},{500, 300},{700, 300}};    
int leftborder[4][2] = {{0, 0}, {10, 0}, {10, 35}, {0, 35}};            // Initial coordinates of left border
int rightborder[4][2] = {{10, 0}, {20, 0}, {20, 35}, {10, 35}};         
int bluerectangle[4][2] = {{3, 3}, {17, 3}, {17, 32}, {3, 32}};          //Initial coordinates of blue rectangle

//brdr_clr array: Defines color of the left and right borders according to main ring no. {left-border, right-border}
int brdr_clr[8][2] = {{0,1}, {1,0}, {0,1}, {1,0}, {1,0}, {0,1}, {1,0}, {0,1}}; 

void draw_left_border(int ring_num){ 
	
	// Black Color or White Color depends on ring number: ring_num
	glColor3f(brdr_clr[ring_num][0], brdr_clr[ring_num][0], brdr_clr[ring_num][0]);          
	glBegin(GL_POLYGON);                                                          
	   for(int i=0; i<4; i++)
	   {
	     glVertex2i(leftborder[i][0], leftborder[i][1]);
	   }
    glEnd();
}

void draw_right_border(int ring_num){
	
	// White Color or Black Color depends on ring number ring_num
	glColor3f(brdr_clr[ring_num][1], brdr_clr[ring_num][1], brdr_clr[ring_num][1]);              
	glBegin(GL_POLYGON);              
	   for(int i=0; i<4; i++)
	   {
	     glVertex2i(rightborder[i][0], rightborder[i][1]);
	   }
    glEnd();
}

void draw_blue_rectangle(void){
	glColor3f(0, 0, 1);              // Blue Color
	glBegin(GL_POLYGON);             // Blue rectangle
	   for(int i=0; i<4; i++)
	   {
	     glVertex2i(bluerectangle[i][0], bluerectangle[i][1]);
	   }
    glEnd();
}

void draw_main_rectangle(int ring_num){      //draws main rectangle or shield, ring_num = main ring number
    draw_left_border(ring_num);
    draw_right_border(ring_num);
    draw_blue_rectangle();
}

void draw_ring(int ring_num){                     // ring_num = main ring number
    for(int angle=0; angle<360; angle+=45){       // angle: in counter clockwise direction 
	    glPushMatrix();
		glRotated(angle, 0, 0, 1);                // rotate object by "angle" in counterclockwise direction
		glTranslated(-10, 50, 0);                 // radius of the circle = 50
		draw_main_rectangle(ring_num);
		glPopMatrix();
	}
}

void display(void){
    
	 glClear(GL_COLOR_BUFFER_BIT);
     for(int i=0; i<8; i++){                               // Draw 8 main circles in 4 X 2 way... 
	    glPushMatrix();                                    // Save a copy of the identity matrix
	    glTranslated(centres[i][0], centres[i][1], 0);     // Translate the centre of new ring
	    draw_ring(i);                                      // Draw current ring
	    glPopMatrix();                                     // Reset CTM to identity for next ring  
     }
	 glFlush();
  }

void main(int argc, char **argv){
		
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(800, 400);
	 glutInitWindowPosition(100, 100);       
     glutCreateWindow("Peripheral Drift Illusion: By Gurpreet Singh");
	 glClearColor(1.0, 1.0, 0.0, 0.0);      //sets yellow background color 
     glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
     gluOrtho2D(0, 800, 0, 400);	        //sets object space window to image area
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
     glutDisplayFunc(display);              //register display call back function
	 glutMainLoop();                        
     
}

// Project- 2 Ends here... 

  */

//For Naresh...Project 2....Modify it....

   /*  

#include<gl/glut.h>
#include<math.h>
#include <stdio.h>

GLfloat b_rect[4][2] = {{3.57, 3.57}, {15.48, 3.57}, {15.48, 29.76}, {3.57, 29.76}};
//for l_border and r_border: pattern{x-coordinate, y-coordinate, color decider}    
// going to use x, y for first 4 fields of array, and color decider for 8 fields of array... 
GLfloat l_border[8][3] = {{0,0,0},{9.525,0,1},{9.525,33.333,0},{0,33.333,1},{0,0,1},{0,0,0},{0,0,1},{0,0,0}};   
GLfloat circle_centers[8][2]={{110, 110},{310, 110},{510, 110},{710, 110}, {110, 310},{310, 310},{510, 310},{710, 310}};
GLfloat r_border[8][3] ={{9.525,0,1},{19.05,0,0},{19.05,33.333,1},{9.525,33.333,0},{0,0,0},{0,0,1},{0,0,0},{0,0,1}};

void draw_shield(int rn){            // rn = main ring number
    
	// Draws Right Border
	glColor3f(r_border[rn][2], r_border[rn][2], r_border[rn][2]);              // White Color or Black Color depends on ring number rn
	
	glBegin(GL_POLYGON);             // White Right border
	   for(int i=0; i<8; i++)
	   { glVertex2f(r_border[i][0], r_border[i][1]);
	   }
    glEnd();

	// Draws Left Border
	glColor3f(l_border[rn][2], l_border[rn][2], l_border[rn][2]);              // White Color or Black Color depends on ring number rn
	
	glBegin(GL_POLYGON);             
	   for(int j=0; j<8; j++)
	   {  glVertex2f(l_border[j][0], l_border[j][1]);
	   }
    glEnd();

	// Draws Blue Rectangle
	glColor3f(0, 0, 1);              // Blue Color
	
	glBegin(GL_POLYGON);           
	   for(int k=0; k<4; k++)
	   {  glVertex2f(b_rect[k][0], b_rect[k][1]);
	   }
    glEnd();

}

void draw_circular_ring(float rn){                   // rn = main ring number
    for(int r=0; r<360; r+=45){
	    glPushMatrix();
		glRotated(r, 0, 0, 1);              
		glTranslated(-9.525, 50, 0);        // middle value = 50 is radius of the circle....
		draw_shield(rn);
		glPopMatrix();
	}
}

void display(void){
	 glClear(GL_COLOR_BUFFER_BIT);
	 for(int z=0; z<8; z++){
	    glPushMatrix();                                     // Save a copy of the identity matrix
	    glTranslated(circle_centers[z][0], circle_centers[z][1], 0);      // Translate the centre of new ring
	    draw_circular_ring(z);                                        // Draw current ring
	    glPopMatrix();                                      // Reset CTM to identity for next ring  
	 }
	 glFlush();
}

void main(int argc, char **argv){
		
	 glutInit(&argc, argv);
	 
	 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(820, 420);
	 
	 glutInitWindowPosition(200, 150);       //upper-left of display to upper-left of windows. 
     glutCreateWindow("peripheral drift illusion");
	 
	 glClearColor(1.0, 1.0, 0.0, 0.0);      //sets background color yellow
     glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();

	 gluOrtho2D(0, 820, 0, 420);	        //sets object space window to image area
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();

     glutDisplayFunc(display);              //register display call back function
	 glutMainLoop();                        //enter event-processing loop, always needs this line. 
     
}

 

  */

  

 
  
/*


#include<gl/glut.h>
#include <gl/glu.h>
#include<math.h>
#include <stdio.h>

GLfloat x,y,z = -2, dz=0.02,theta=0,dt=0.2,p=3.141593/180;

void startdraw2(void){

		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the image

  glColor3f(1.0, 0.0,0.0);     //Set current color for drawing to red. (0.0 is zero entensity and 1.0 for full intensity)

  glBegin(GL_POLYGON);       //The beginning of a vertext list is marked by glBegin function 
  //Drawing a red triangle

	glVertex3f(-1,01,2);
	glVertex3f(1,-0.5,-4);
	glVertex3f(0.8,1.25,-6);

	glEnd();

	x=cos(theta*p);  //find one corner of square
	y=sin(theta*p);

	glColor3f(0.0, 1.0,0.0); 

	  glBegin(GL_POLYGON);   
	//drawing a green triangle

	glVertex3f(x,y,z);   
	glVertex3f(-y,x,z);       //Use symmerty for other corners
	glVertex3f(-x,-y,z);
	glVertex3f(y,-x,z);

  	glEnd(); //The end of a vertext list is marked by glEnd function 
	glutSwapBuffers();  //swap front and back buffers

}


void idle(void){

	if(dz<0 && z<=-5.5 || dz >0 && z>=-0.6)dz=-dz; //change direction at ends

	z+=dz;							//move square to next z-position
	theta=(theta<360)?theta+dt:dt;  //roate square by another dt
	glutPostRedisplay();			//set flag for redisplay


}

void main(int argc, char **argv){
		
		//glutInit needs to be called before GLUT functions. 

     glutInit(&argc, argv);

		//the following three functions specify attributes of the image window we want to create.
	 glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH| GLUT_DOUBLE);
     glutInitWindowSize(512, 512);
     glutInitWindowPosition(250, 100); //upper-let of display to upper-left of windows. 

		//window is then created using this functions. 
     glutCreateWindow("Double buffer animation");
	 
     glClearColor(0.0, 0.0, 0.0, 0.0);  //sets background color (r,g,b,alpha) for the window.
     
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     
	// glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

     glOrtho(-2, 2, -2, 2, 0.5,6);		//sets object space window to image area

	 //you can use glortho or you can use gluPerspective()

	// gluPerspective(120,1.0,0.5,6);

	 glEnable(GL_DEPTH_TEST); //enable z-buffering 

     glutDisplayFunc(startdraw2);        //register display call back function
	 glutIdleFunc(idle);				//register idle function
     glutMainLoop();                     //enter event-processing loop, always needs this line. 
     
}



  */


/*

#include<gl/glut.h>
#include <gl/glu.h>
#include<math.h>
#include <stdio.h>

void draw_triangle(float r, float g, float b){
 
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex2i(0, 0);
		glVertex2i(220, 0);
		glVertex2i(220, 150);
    glEnd();


}

void display(void){

	 glClear(GL_COLOR_BUFFER_BIT);
	 
	 glMatrixMode(GL_MODELVIEW);
	 
	


	   //glLoadIdentity();             // don't inlcude it as it will be multipled with translation matrix to yield no result
	 draw_triangle(0, 0, 1);       // Draw blue Triangle

	 glRotated(45, 0, 0, 1);                         

	 draw_triangle(1, 0, 1);       // Draw blue Triangle
 
	 glTranslated(300, 200, 0);
	  
	 
      draw_triangle(1, 0, 0);   
    
	glFlush();
   //glutSwapBuffers();  //swap front and back buffers

   

}



void main(int argc, char **argv){
		
		//glutInit needs to be called before GLUT functions. 

     glutInit(&argc, argv);

     //the following three functions specify attributes of the image window we want to create.
	 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	 
	 //glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH| GLUT_DOUBLE);

     glutInitWindowSize(800, 600);

	
     glutInitWindowPosition(100, 100); //upper-let of display to upper-left of windows. 
     glutCreateWindow("Triangles");
	 glClearColor(0.0, 0.0, 0.0, 0.0);  //sets background color (r,g,b,alpha) for the window.
     gluOrtho2D(0, 800, 0, 600);	//sets object space window to image area
     
     glutDisplayFunc(display);        //register display call back function
	 glutMainLoop();                     //enter event-processing loop, always needs this line. 
     
}


    */  



/*

#include<gl/glut.h>
#include <gl/glu.h>
#include<math.h>
#include <stdio.h>

GLfloat x,y,z = -2, dz=0.02,theta=0,dt=0.2,p=3.141593/180;

void startdraw2(void){

		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the image

  glColor3f(1.0, 0.0,0.0);     //Set current color for drawing to red. (0.0 is zero entensity and 1.0 for full intensity)

  glBegin(GL_POLYGON);       //The beginning of a vertext list is marked by glBegin function 
  //Drawing a red triangle

	glVertex3f(-1,01,2);
	glVertex3f(1,-0.5,-4);
	glVertex3f(0.8,1.25,-6);

	glEnd();

	x=cos(theta*p);  //find one corner of square
	y=sin(theta*p);

	glColor3f(0.0, 1.0,0.0); 

	  glBegin(GL_POLYGON);   
	//drawing a green triangle

	glVertex3f(x,y,z);   
	glVertex3f(-y,x,z);       //Use symmerty for other corners
	glVertex3f(-x,-y,z);
	glVertex3f(y,-x,z);

  	glEnd(); //The end of a vertext list is marked by glEnd function 
	glutSwapBuffers();  //swap front and back buffers

}


void idle(void){

	if(dz<0 && z<=-5.5 || dz >0 && z>=-0.6)dz=-dz; //change direction at ends

	z+=dz;							//move square to next z-position
	theta=(theta<360)?theta+dt:dt;  //roate square by another dt
	glutPostRedisplay();			//set flag for redisplay


}

void main(int argc, char **argv){
		
		//glutInit needs to be called before GLUT functions. 

     glutInit(&argc, argv);

		//the following three functions specify attributes of the image window we want to create.
	 glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH| GLUT_DOUBLE);
     glutInitWindowSize(512, 512);
     glutInitWindowPosition(250, 100); //upper-let of display to upper-left of windows. 

		//window is then created using this functions. 
     glutCreateWindow("Double buffer animation");
	 
     glClearColor(0.0, 0.0, 0.0, 0.0);  //sets background color (r,g,b,alpha) for the window.
     
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     
	// glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

     glOrtho(-2, 2, -2, 2, 0.5,6);		//sets object space window to image area

	 //you can use glortho or you can use gluPerspective()

	// gluPerspective(120,1.0,0.5,6);

	 glEnable(GL_DEPTH_TEST); //enable z-buffering 

     glutDisplayFunc(startdraw2);        //register display call back function
	 glutIdleFunc(idle);				//register idle function
     glutMainLoop();                     //enter event-processing loop, always needs this line. 
     
}


*/
