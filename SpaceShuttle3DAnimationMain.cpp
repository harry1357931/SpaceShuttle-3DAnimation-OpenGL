/*  Project4.cpp -- Space Shuttle
 *  Project 4 : 3D Animation Final Project
 *  CS 368 Computer Graphics
 *  Prof Xiang
 *  Summer 2, 2012
 *  
 *  Author: Gurpreet Singh
 *  Project Description:  
 *     1) Firstly Space Shuttle launches in Solar System
 *	   2) Then, Shuttle seperates out from Solid Boosters and Main tank
 *	   3) After that, Shuttle Rotates and Moves near the planets
 *
 *  Input Format: 
 *      No Input is required, just run the program and have fun
 *
 *  OutPut Format:
 *      Since this program is a 3D Animation project. It launches Space shuttle in Solar system, 
 *      then shuttle gets seperated from its Solid boosters, then shuttle move close to the planets and rotates.
 *      You can view shuttle from different angles by selecting some keys. You will find instructions on those keys,
 *      once you will run the program
 *      
 * @Compiler version on which Program is Last Run before uploading to Github: Dev-C++ 5.4.1, Date: 1st July, 2013  
 * @author Gurpreet Singh
 */

#include<iostream>
#include<gl/glut.h>            // OpenGL Graphics Utility Library
#include <gl/glu.h>
#include<math.h>
#include <stdio.h>

using namespace std;
float tx=0,ty=0,tz=0, Trans_y=0, alpha=0, T_z=200, z_viewvol= -120, saturn_tx=0;
unsigned char mainkey='s';       // Controls Main Project in display function, default value: s: for start

// 0:Sun, 1:Jupiter, 2:RedPlanet, 3:Saturn, 4:BluePlanet, 5: LovePlanet 6: LastPlanet  {R, G, B, Tx, Ty, Tz, Radius}....eachplanet: SolidSphere
float planets[6][7]= { {0.96,0.96,0.5,0,65,-2400,60}, {1,0.1,0.1,80,-30,-3600,50}, {0.96,0.33,0.33,-300,0,-1800,30}, 
  {0.96,0.9,1,-440,20,-4200,50}, {0.67,0.95,0.365,100,0,-800,30}, {0.16,0.7,0.96,10,-70,-1600,50}  };

// {Base_Radius, Top_Radius, height, all-color}   // Seperate Color for array...
float BsterCylTypes[9][4]= {{1, 0.1, 3, 0.6}, {1, 0.9, 0.5, 0}, {2, 1, 3, 0.6}, {2, 2, 6, 0.6}, {2.05, 2.05, 0.3, 0}, 
      {2.2, 1.7, 1, 0}, {2.2, 1.7, 1, 0.4}, {2.7, 2.2, 1.5, 0.4}, {2.4, 1.8, 2, 0.6} };                        // total 9 rings

int BsterCylOrder[] = {0, 1, 2, 4, 3, 4, 3, 4, 3, 4, 3, 4, 3, 5, 6, 5, 6, 5, 6, 7, 8};             // Total 19

float BlackStripPlate[7][4] = { {1.9,-6,13,10.31},  {11.7,-6,-13,10.31}, {-2.7,-11,43,7}, {16.2,-11,-42,7}, 
          {-3.8,-14,19,3.17}, {17.2,-14,-19,3.17}, {-3.8,-14,90,21} };

float Shuttle_Plates[3][4] = { {-6,10,1,0.5}, {-11,5,1.9,1}, {-14,3,2.1,1.9} };

GLUquadricObj* leftbooster, *maintank, *shuttle;
GLfloat pos[]= { -10, 20, -60, 1};						//light position         //{-2,4,5,1}
GLfloat amb [] = {0.7, 0.7, 0.7, 1.0};				//Ambient intensity 
GLfloat front_amb_diff[]= {0.8, 0.7, 0.7,1.0};	//Front side property
GLfloat back_amb_diff[]= {0.4,0.7,0.1,1.0};		//Back side property
GLfloat spe[]= {0.25,0.25,0.25,1.0};			//Property for front and back
GLfloat spe2[]= {80};			            //Property for front and back
GLfloat theta=0,dt=0.7, axes[3][3] = {{1,0,0},{0,1,0},{0,0,1}}; 
int axis = 0; 					         	//0:x, 1:Y, 2:Z, 3:light aroundz

/* You must have seen the Orange color tank on which shuttle hangs when the shuttle launches,
 * this function builds that Orange Tank. 
 */				
void shuttle_main_tank(){
	glPushMatrix ();
	   maintank = gluNewQuadric();
   	   gluQuadricDrawStyle(maintank, GLU_FILL);
       gluQuadricNormals(maintank, GLU_SMOOTH);
       gluQuadricOrientation(maintank, GLU_OUTSIDE);

	   glColor3f(1, 0.51, 0.04);
  	   // for Top of Main Orange Color Tank
	   float h=0;
	   for(float r= 4.7; r>=0; r-= 0.5){
	         		    
			glPushMatrix();
			   glTranslated(7.5, 22+h, 0);         // Flexible to take it up and down        
			   glTranslated(0, 0, z_viewvol);            // Translation ....must for cylinders
               glRotated(-90, 1, 0, 0);              // Rotation.....must for cylinders
			   gluCylinder(maintank, r, r, 1 , 100, 100);    
			   h++;
	  	    glPopMatrix(); 
	   }

	   glTranslated(7.5, -18, z_viewvol);
	   glRotated(-90, 1, 0, 0);              
	   gluCylinder(maintank, 4.7, 4.7, 40, 32, 32);                  // Main Tank...orange color...completes main cylinder
	glPopMatrix();

	glPushMatrix();                          // for spherical bottom of main tank...
	   glTranslated(7.5, -18, z_viewvol);
       glRotated(-90, 1, 0, 0);                    // Rotate Cylinder along X-axis...to make it look vertical
	   glutSolidSphere(4.7, 32, 32);
	glPopMatrix();
}

/* This function builds both wings of shuttle. 
 */				

void shuttle_plate(){                // Shuttle Plate == Shuttle Wings.....Wings make in 3 sections
	glColor3f(1, 1, 1);
	for(int i=0; i<3; i++){         // ShuttlePlates: 0: Top Plate, 1: Middle Plate, 2: Bottom Plate
	  glPushMatrix ();                                  
	      glTranslated(6.7, Shuttle_Plates[i][0], 6+z_viewvol);                   
	      glScalef(5, Shuttle_Plates[i][1], 0);
	      glRotated(-90, 1, 0, 0);
	      gluCylinder(shuttle, Shuttle_Plates[i][2], Shuttle_Plates[i][3], 1, 32, 32);                
	   glPopMatrix();
	}

	glColor3f(0.2, 0.2, 0.2);
	//  For all Black Strips on Shuttle around wings // 0:LeftStripTopWing, 1:RightStripTopWing, 2:LeftStripMiddleWing, 3:RightStripMiddleWing, 
	//  4:LeftStripBottomWing, 5:RightStripBottomWing, 6:HorizontalBottomStripBottomWing
    for(int i=0; i<7; i++){          
		glPushMatrix ();                                  
	        glTranslated(BlackStripPlate[i][0], BlackStripPlate[i][1], 6+z_viewvol);                   
	        glRotated(-90, 1, 0, 0);
	        glRotated(BlackStripPlate[i][2], 0, 1, 0);
	        gluCylinder(shuttle, 0.2, 0.2, BlackStripPlate[i][3], 32, 32);               // Cylindrical Black strip along wings
	    glPopMatrix();
     } // for Loop Ends here 
 }

float random(int min, int max){            // random function: self defined....needed to throw flames
   
   again: int i=(rand()%100)/10;
   if(max < 7){	
	  if(i==8||i==7)
	    i= -1;
	  else if(i==9)
	    i = -2;
   }	
   
   if(i>=min && i<=max)
	  return i;
	else
	 goto again;	
}

float random2(int min, int max){                // needed to throw flames

  again: int i=(rand()%100);
  if(i>=min && i<=max)
	  return i;
  else
	 goto again;
}

/*  This function gives the effect of fire, when the shuttle launches
 */

void fire(int j){

	glPushMatrix();
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	    glEnable(GL_BLEND);

		float flamethrower[5][3]={ {7,-20, 8.5}, {6,-20,7.5}, {8,-20,7.5}, {0,-25,0}, {15,-25,0}  };    // {T_x, T_y, T_z} for diferent boosters and shuttle
		
		for(int i=0; i<200; i++){
	       glPushMatrix();
			  glTranslated(flamethrower[j][0] + random(-3,3), flamethrower[j][1]-random2(0,20), flamethrower[j][2]+random(0,5));
	          glTranslated(0, 0, z_viewvol);                // Translation ....must for cylinders
             
			  if(i%3==0){
			      glColor3f(1,0.46, 0);
				  glutWireSphere(0.5, 32,32);
			   }
               else if(i%2==0)
			   { glColor3f(1,0.64,0);
				 glutWireTetrahedron();
			  }
			  else{
				  glColor3f(0.95,0.95,0.2);
				  glutSolidOctahedron();
			  }
		   glPopMatrix();
	     }  // for loop ends here

	   glDisable(GL_BLEND);
	glPopMatrix();
}

/*  This function builds the tail of shuttle
 */
void shuttleTail(){
	glColor3f(0.4, 0.4, 0.4);
  	glPushMatrix ();                    
       glTranslated(6.7, -11, 10+z_viewvol);       
       glRotated(50, 1, 0, 0);
	   glScalef(1, 4, 7);
	   gluCylinder(shuttle, 0.8, 0.5, 1, 32, 32);               
	glPopMatrix();

	glPushMatrix ();                     // Tail...help
       glTranslated(6.7, -16.5, 13.3+z_viewvol);       
       glScalef(1, 4, 2.5);
	   gluCylinder(shuttle, 0.3, 0.5, 1, 32, 32);                  
	glPopMatrix();

	glPushMatrix ();                     // Tail...help
	   glTranslated(6.7, -15.30, 14+z_viewvol);       
       glRotated(90, 1, 0, 0);
	   glScalef(1, 4, 2.5);
	   gluCylinder(shuttle, 0.3, 0.3, 1, 32, 32);                  // Main cylinder of shuttle...white color...completes main cylinder
	glPopMatrix();
}
/*  This function builds the exhaust of shuttle
 */

void shuttle_exhaust(){                    // Use 7 cylindrical rings and 2 cones for each exhaust(Silencer) of Shuttle
  float r=1, T_y=-16, height;
  float exhaust[3][2] = { {6.7, 12}, {5, 7}, {9, 7} };             // {T_x, T_y}
  
  for(int j=0; j<3; j++){
	  glColor3f(0, 0, 0);
	  r=1; 
	  T_y = -16;
     
	  for(int i=0; i<7; i++){ 
	      glPushMatrix ();                                // for shuttle exhaust
	        glTranslated(exhaust[j][0], T_y, exhaust[j][1]+z_viewvol);       
            glRotated(-90, 1, 0, 0);                      // Rotation.....must for cylinders
	        gluCylinder(shuttle, r, r, 0.40, 32, 32);                  // Main cylinder of shuttle...white color...completes main cylinder
	      glPopMatrix();
	      T_y = T_y - 0.5;
	      r= r+0.15;  
      }
   	  height=0;
      glColor3f(0.3, 0.3, 0.3);
    
	  for(int i=0; i<2; i++)  { 
	    glPushMatrix ();                                // for shuttle exhaust
	       glTranslated(exhaust[j][0], T_y, exhaust[j][1]+z_viewvol);       
           glRotated(-90, 1, 0, 0);                      // Rotation.....must for cylinders
	       height = -T_y -16;
	       glutSolidCone(r, height, 32,32);                  // One Cone in the exhaust ring
   	    glPopMatrix();

	    glColor3f(.92,0.75,0.05); 
	    T_y = T_y -0.2;
      }
   }// Main for loop ends here
}
/*  This function builds the main cylndrical body of space shuttle
 */

void shuttle_Discovery(){
       glTranslated(0, -3, 0);
	   shuttle = gluNewQuadric();
       gluQuadricDrawStyle(shuttle, GLU_FILL);
       gluQuadricNormals(shuttle, GLU_SMOOTH);
       gluQuadricOrientation(shuttle, GLU_OUTSIDE);
	   
	   glColor3f(0, 0, 0);      
       glPushMatrix ();                                // for hemispherical topmost front of shuttle...
 	      glTranslated(6.7, 13.5, 9+z_viewvol);       
          glutSolidSphere(1.4, 32, 32); 
	   glPopMatrix();

	   glColor3f(0.7, 0.7, 0.7);     
       glPushMatrix ();                               // for front curve of shuttle...where we have windscreen and stuff
 	      glTranslated(6.7, 7, 9+z_viewvol);       
          glRotated(-90, 1, 0, 0);              
	      gluCylinder(shuttle, 3.2, 1.4, 6.5, 32, 32); 
	   glPopMatrix();

	   glColor3f(0.3, 0.3, 0.3);                  // Color for Windscreen
	   // 0: middle windscreen, 1: left windscreen, 2: right windscreen {T_x, T_y, T_z, R_y}  R_y: Rotation along y-axis 
	   float windscreens[3][4] = { {6.7, 9, 10.9, 0}, {5.3, 8.4, 10.7, 1}, {8, 8.4, 10.7, -1} };

	   for(int i=0; i<3; i++){
	      glPushMatrix ();                     // for front windscreen middle
             glTranslated(windscreens[i][0], windscreens[i][1], windscreens[i][2]+z_viewvol);              
             glRotated(45, 1, windscreens[i][3], 0);              // Rotation.....must for cylinders
	         gluCylinder(shuttle, 1, 0.5, 1, 32, 32); 
	       glPopMatrix();
       }

    glColor3f(0.7, 0.7, 0.7);
    glPushMatrix ();                         // Main Cylindrical Body of Shuttle
	   glTranslated(6.7, -12, 9+z_viewvol);          
       glRotated(-90, 1, 0, 0);             
	   gluCylinder(shuttle, 3.2, 3.2, 19, 32, 32);     // Main cylinder of Shuttle
	glPopMatrix();

	glColor3f(0, 0, 0);                   // Color for Windows
	int windowsY[5]={3, 0, -3, -6, -9};                 // Window pipes in horizontal direction.... 5 Windows in Shuttle
	for(int i=0; i<5; i++){
	  glPushMatrix ();                     // Cubic Windows 
	     glTranslated(6.7, windowsY[i], 10+z_viewvol);       // change 6 to new value..after using cube.....
         glScalef(6.1, 1, 1);
	     glutSolidCube(1);
	  glPopMatrix();
    }

	glColor3f(0.6, 0.6, 0.6);
	// merging cylinders
	glPushMatrix ();                     // Tail Bottom Tank...1st merging cylinder
	   glTranslated(6.7, -15, 9+z_viewvol);     
       glRotated(-90, 1, 0, 0);              
	   gluCylinder(shuttle, 3.2, 3.2, 3, 32, 32);               
	glPopMatrix();

	glPushMatrix ();                     // Tail Bottom Tank...2nd merging Cylinder
       glTranslated(6.7, -15, 10+z_viewvol);       
       glRotated(-90, 1, 0, 0);              
	   gluCylinder(shuttle, 3.2, 3.2, 3, 32, 32);               
	glPopMatrix();

    // for shuttle....exhaust....
    shuttle_exhaust();
    glColor3f(0.4, 0.4, 0.4);
	shuttleTail();
    shuttle_plate();                 // for shuttle plates and strip
	//fire(0);
	//fire(1);
	//fire(2);
}
/*  This function builds the left solid booster of shuttle
 */
void shuttle_leftBooster(bool callfromRight){               // left solid booster
                                      
   	   leftbooster = gluNewQuadric();
   	   gluQuadricDrawStyle(leftbooster, GLU_FILL);
       gluQuadricNormals(leftbooster, GLU_SMOOTH);
       gluQuadricOrientation(leftbooster, GLU_OUTSIDE);

       int j, height=0;       // j to control other array.....height(in the upcoming loop) from prev. loop case...used in next case...
       for(int i=0; i < 21; i++){
		    j = BsterCylOrder[i];
	        height = height + BsterCylTypes[j][2];         // Total Height needed for translation
			glPushMatrix();
		       glColor3f(BsterCylTypes[j][3], BsterCylTypes[j][3], BsterCylTypes[j][3]);
			   glTranslated(0, 20-height, 0);         // Flexible to take it up and down        
			   glTranslated(0, 0, z_viewvol);            
               glRotated(-90, 1, 0, 0);              // Rotation.....must for cylinders
               gluCylinder(leftbooster, BsterCylTypes[j][0], BsterCylTypes[j][1], BsterCylTypes[j][2], 40, 100);    
	  	    glPopMatrix(); 
	   }
	  if(callfromRight==true)
	    return;

	  fire(3);
}
/*  This function builds the right solid booster of shuttle
 */

void shuttle_rightBooster(){              // right solid booster
	glPushMatrix ();
       glTranslated(15, 0, 0);
	   shuttle_leftBooster(true);             
    glPopMatrix(); 
	fire(4);                            // Starts fire for Right Booster
}

// Assembles all part of Space Shuttle: L and R Boosters, Main Tank, Main Shuttle-Discovery
void shuttle_main(){                   
	shuttle_leftBooster(false);        // false: for not calling from right booster function...
    shuttle_rightBooster();
    shuttle_main_tank();
  	shuttle_Discovery();
}

/*  This function gives effect of shuttle seperating from solid boosters & orange color tank
 */
void shuttle_seperate(){
	theta = theta-45;
	glPushMatrix(); 				    // for left booster
	    glRotated(theta, 1, 1, 0); 	
	    shuttle_leftBooster(false); 	    
    glPopMatrix();
 
	glPushMatrix(); 				    //  for right booster
	    glRotated(theta, 1, -1, 0); 	
	    shuttle_rightBooster();	    
    glPopMatrix();
   
	glPushMatrix(); 				   //for Orange main tank
	    glRotated(theta, -1, 0, 1); 	
	    shuttle_main_tank();	    
   glPopMatrix();

   glPushMatrix(); 				       //for shuttle
	    glRotated(theta/3, 1, 0, 0); 	
	    shuttle_Discovery();	 
   glPopMatrix();

   theta = theta +45;                   // Resetting theta

}

void BackGroundSphere(){

  glPushMatrix(); 
   glColor3f(1, 1, 1);
   glTranslated(0, 0, -4000);
   glutWireSphere(4000, 12, 12);                      // Big Wire Sphere.....to give feel of order in Solar System
  glPopMatrix(); 
}

void Jupiter_Moon(){
  // Array stores...{Tx, Tz} of Moons to display them near Jupiter
  int moon[3][2] = {{20, -3600}, {80, -3540}, {80, 3660}};
  for(int i=0; i<3; i++){
     glPushMatrix(); 
       glColor3f(1, 0.8, 0.8);
       glTranslated(moon[i][0], -20, moon[i][1]);                     // centre at Tx, Ty, Tz
       glutSolidSphere(5, 32, 32);                     //radius: 50
     glPopMatrix();
   }
}

void AllPlanets(){                              // All planets and Sun
	 
  for(int i=0; i<6;i++){
    glPushMatrix(); 
      glColor3f(planets[i][0], planets[i][1], planets[i][2]);
      if(i==3){
		  glTranslated(planets[i][3]+saturn_tx, planets[i][4], planets[i][5]);
		  saturn_tx += 1;
	  }else{
	      glTranslated(planets[i][3], planets[i][4], planets[i][5]);                // centre at Tx, Ty, Tz
	   }
	   glutSolidSphere(planets[i][6], 32, 32);                     
    glPopMatrix();
  }
}

void Display_Contol_Menu(){

	fprintf(stdout, "Space Shuttle Project: By Gurpreet Singh");
    cout<<"\n\nPress 's' to Start Main Project....\nPress 'o' to see only Shuttle...\nPress 'r' to see shuttle in different view..."<<endl;
	cout<<"Press 'd' to see shuttle only on different view along different axis...\nPress 'g' to see Solar System or Galaxy..."<<endl;
	cout<<"Press 'p' to seperate shuttle....like what happens during its launch\nPress 'l' to see booster only\n";
	cout<<"Press 'k' to see shuttle discovery only\nPress 'm' to see orange main tank only\n";
}

void keyboard(unsigned char key, int x, int y){
  if(key=='s'||key=='o'||key=='r'||key=='d'||key=='g'||key=='p'||key=='l'||key=='k'||key=='m' )	
	 {    mainkey = key;                // main key used in display function...
     if (key=='p')
		theta = 45;
	else if(key == 's')
	{
	  theta = 0;
	  T_z = 200;
	} 
  
  }
  else{
     mainkey='s';
  
  }

	glutPostRedisplay();

}

void display(void)
{   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();		//Preserve the CTM that Puts the objects in View Volume.

	  glLightfv(GL_LIGHT0, GL_POSITION, pos);                  // lighting on shuttle
	  
 switch(mainkey){
 
   case 's':{  
	  // Displays all Planets 
	   glPushMatrix();
	      glTranslated(0, 0, T_z);
	      BackGroundSphere();
		  AllPlanets();
		  Jupiter_Moon();
	   glPopMatrix();
	   
	   T_z= T_z + 10;                               // To Move along z-axis towards origin.....for moving effect

	   if(theta>359 && theta < 360)                  // just for reseeting the values....when theta reaches 360
	   {  T_z = 0;
	      z_viewvol= -120;
	   }

	   if(theta < 46)       
	   {
		    glPushMatrix(); 				//Preserve CTM for object       
		       glTranslated(tx, ty, 0);
			   glTranslated(6, 0, -120);   
	           glRotated(-theta*1.3, 1, 0, 0); 	
		       glTranslated(-6, 0, 120);   
		       shuttle_main();                  // Shuttle Main
			 glPopMatrix();
		  
		}
		else if(theta>=46 && theta < 80){      
		     glPushMatrix();           
		       glTranslated(6.7, 0, -120);   
	           glRotated(-62, 1, 0, 0); 	//rotate light around z
		       glTranslated(-6.7, 0, 120);
			   shuttle_seperate();            // Shuttle Seperate
		     glPopMatrix();
		 }
		 else{        
		     glPushMatrix(); 	
			    
			    glTranslated(random(-1,1)*0.05, Trans_y + random(-1,1)*0.05, random(-1,1)*0.05 );
			    
				glTranslated(6.7, 0, -120);
	            glRotated(-48, 1, 0, 0); 	
		        glTranslated(-6.7,0,120);

			     glPushMatrix();
				   glTranslated(6.7, 0, -120);
				   glRotated(alpha, 0, 1, 0);
	               alpha = alpha+1;                // for rotation angle...
				   glTranslated(-6.7, 0, 120);
                  
				   shuttle_Discovery();          // Only Shuttle...no main tank...no boosters
				   fire(1);
				   if(T_z > 3600)
		              z_viewvol-=0.20;
		          glPopMatrix();
         
		      glPopMatrix();
		   }
		   break;        // case 's': start ends here
		 } 
     
   case 'o': shuttle_main();                   // Case: 'o'  for only shuttle....display shuttle only
	         break;
   case 'r': glTranslated(6.7, 0, -100);         // case: 'r' for main shuttle in differetn view
	         glRotated(-40, 1, 0, 0);
	         glTranslated(-6.7, 0, 100);
	         shuttle_main();
             break;
   case 'd': glTranslated(6.7, 0, -100);          // case: 'd' for main shuttle in different view...along z-axis
	         glRotated(-40, 1, 0, 1);
	         glTranslated(-6.7, 0, 100);
	         shuttle_main();
             break;
   case 'g':  BackGroundSphere();                 // case: 'g' for galaxy view...or solar system view
		      AllPlanets();
		      Jupiter_Moon();
		      break;
   case 'p': shuttle_seperate();                  // case: 'p' for space shuttle seperate view
	         break;
   case 'l': shuttle_leftBooster(false);
	         break;
   case 'k': shuttle_Discovery();
	         break;
   case 'm': shuttle_main_tank();
	         break;
   }// Switch ends here 

	glPopMatrix();  						         //restore CTM for object
	glutSwapBuffers(); 
}

void idle(void){
	theta = (theta <360) ? theta+dt:dt; //increment rotation angle
	tx= tx - 0.05;
	ty = ty + 0.08;
	glutPostRedisplay();
}

void main(int argc, char **argv){

	 pos[2] = z_viewvol+20;                // To automatically set the position of Light....with view volume in Z-direction
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
     glutInitWindowSize(800, 600);
     glutInitWindowPosition(200, 100); 
	 glutCreateWindow("Project 4: 3D Animation by Gurpreet Singh");
     glClearColor(0, 0, 0, 0);       //sets background color (r,g,b,alpha) for the window.
	 glEnable(GL_DEPTH_TEST);		 //To remove depth buffer
	 glMatrixMode(GL_PROJECTION);
     glLoadIdentity();				//identity matrix for preojection 
	 gluPerspective(45, 1, 20, 9000);
	 
	 glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, front_amb_diff);    //Front side
	 glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_amb_diff);      //Back side
	 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);                //Front and back
	 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, spe2);              //Front and back
	 
	 glLightfv(GL_LIGHT0, GL_AMBIENT, amb); 	//light source
	 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 	//for solid cone
	 
	 glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();				//Identity matrix for Modelview 

	 glLightfv(GL_LIGHT0, GL_POSITION, pos); //light postion is affected by current modelview
	 glEnable(GL_LIGHTING); 	//enable light
	 glEnable(GL_LIGHT0); 		//enable gl_light0
	 glEnable(GL_COLOR_MATERIAL);             // Enable Color Material
	 Display_Contol_Menu();
	 	 
     glutDisplayFunc(display);        //register display call back function
	 glutKeyboardFunc(keyboard);
	 glutIdleFunc(idle);
     glutMainLoop();                     //enter event-processing loop, always needs this line. 
}
// Project 4 ends here...
