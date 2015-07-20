#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SSD_util.h"
#define MAXLINELENGTH 1000
#define MAXFILELEN    256

char saved_fname[256];
SCENE thescene;
CAMERA vcamera;

//#define DEBUG_FLAG
void display(void);
int Render_SSD(SCENE *ascene, CAMERA *acamera);
void swap(int *startX, int *startY, int *endX, int *endY, float *cStart, float *cEnd);
void specify_color(int x, int startX, int endX, float *startRgba, float *endRgba);
void drawpoint(int width, int x, int y, int fix);
void drawline(int startX, int startY, int endX, int endY, float *startRgba, float *endRgba, int width);
int max(int a, int b, int c);
int min(int a, int b, int c);
void drawtriangle(int aX, int aY,int bX,int bY,int cX,int cY,float *a_rgba,float *b_rgba,float *c_rgba);


void display(void)
{
  Render_SSD(&thescene, &vcamera);
}

void init (void)
{
  /* select clearing color  to the specified background  */
  glClearColor(thescene.bcolor.rgba[0], thescene.bcolor.rgba[1], 
	       thescene.bcolor.rgba[2], thescene.bcolor.rgba[3]);
  glClear (GL_COLOR_BUFFER_BIT);
  /* initialize viewing values  */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, thescene.screen_w-1.0, 
	  0.0, thescene.screen_h-1.0, -1.0, 1.0);
  
}

void swap(int *startX, int *startY, int *endX, int *endY, float *cStart, float *cEnd)
{
	int tmp;
	float tmpC;
	int i;

	tmp = *startX;
	*startX = *endX;
 	*endX = tmp;
	
	tmp = *startY;
	*startY = *endY;
 	*endY = tmp;			

	for(i = 0; i < 4; i++){
		tmpC = cStart[i];
		cStart[i] = cEnd[i];
	 	cEnd[i] = tmpC;	
	}	

}

void drawpoint(int width, int x, int y, int fix)
{
	int tag = -(width/2);
	int i;

	for ( i = tag; i < tag + width; i++)
	{
		if(fix == 0)
		  glVertex2i(x,y+i);

		else
		  glVertex2i(x+i,y);

	}
}

void specify_color(int x, int startX, int endX, float *startRgba, float *endRgba){
		float rgbap[4];
		float percent = (float)(x - startX)/(float)(endX - startX);
		rgbap[0] = startRgba[0] + (endRgba[0] - startRgba[0]) * percent;
		rgbap[1] = startRgba[1] + (endRgba[1] - startRgba[1]) * percent;
		rgbap[2] = startRgba[2] + (endRgba[2] - startRgba[2]) * percent;
		rgbap[3] = startRgba[3] + (endRgba[3] - startRgba[3]) * percent;
		glColor3f(rgbap[0],rgbap[1],rgbap[2]);
}

void drawline(int startX, int startY, int endX, int endY, float *startRgba, float *endRgba, int width)
{
	int dx,dy,d,kx,ky;
   	int x,y;
    
    	if( startX > endX){               /* ensure x0<x1*/    
        	swap(&startX,&startY,&endX,&endY,startRgba,endRgba);  
   	}
       dx=2*(endX-startX);
	dy=2*(startY-endY);
	kx=endX-startX;
	ky=endY-startY;
       x = startX;
    	y = startY;
    		
    	if((abs(kx)>=abs(ky))&&(ky>=0)){           /* when 0<k<=1 */
      		//d = (dy*2) - dx;
	
		d=dy*(startX+1)+(endX-startX)*(2*startY+1)+2*(startX*endY-endX*startY);
        	while(x<=endX){
			specify_color(x, startX, endX, startRgba, endRgba);
			drawpoint(width, x, y, 0);        
			if(d < 0){
                		//d += (dy - dx)*2;       /* NE */
                		d +=dx;
                		y++;
            		}
            		
                		d += dy;        /* N */
               		x++;
            		      
            //specify_color(x, startX, endX, startRgba, endRgba);
		//drawpoint(width, x, y, 0);
        	}
        	return;
    	}	  
    	if((abs(ky)>abs(kx))&&(ky>0)){           /* when k>1 */
      		
		d = (startY-endY)*(2*startX+1)+dx*(startY+1)+2*(startX*endY-endX*startY);
        	while(y<=endY){
			specify_color(y, startY, endY, startRgba, endRgba);
			drawpoint(width, x, y, 1);
            		if(d > 0){
                		d += dy; /*  NE  */
                		x++;
                		//y++;
           		}
            		
                		d += dx;     /* N */
                		y++;
            		
        	}
        	return;
    	}
	
    	//dy = 2*(startY - endY);    
    	if((abs(kx)>=abs(ky))&&(ky<0)){           /* when -1=<k<0 */
    		startY=-startY;
		endY=-endY;
		y=startY;
		x=startX;
		dy=2*(startY-endY);
		d=dy*(startX+1)+(endX-startX)*(2*startY+1)+2*(startX*endY-endX*startY);
        	while(x<=endX){
			specify_color(x, startX, endX, startRgba, endRgba);
			drawpoint(width, x, -y, 0);        
			if(d < 0){
                		
                		d += dx;
				//x++;
                		y++;
            		}
            		
                		d += dy;        /* N */
               		x++;
            		       
            //specify_color(x, startX, endX, startRgba, endRgba);
		//drawpoint(width, x, y, 0);
        	}
        	return;

    	}
       if( (abs(ky)>abs(kx))&&(ky<0)){           /* when k<-1 */
        	startY=-startY;
		endY=-endY;
		y=startY; 
		dy=2*(startY-endY);
		dx=2*(endX-startX);
		d = (startY-endY)*(2*startX+1)+dx*(startY-1)+2*(startX*endY-endX*startY);
        	while(y<=endY){
			specify_color(y, startY, endY, startRgba, endRgba);
			drawpoint(width, x, -y, 1);
            	if(d > 0){
                		d += dy; /*  NE  */
                		x++;
                		//y++;
           		}
            	
                		d += dx;     /* N */
                		y++;
            		
        	//specify_color(x, startX, endX, startRgba, endRgba);
		//drawpoint(width, x, y, 1);
        	}
        	return;


    	}
	
}

int max(int a, int b, int c){
	int max=a;	
	if(b>max)
		max=b;
	if(c>max)
		max=c;
	return max;
}
int min(int a, int b, int c){
	int min=a;
	if(b<min)
		min=b;
	if(c<min)
		min=c;
	return min;
}

void drawtriangle(int aX, int aY,int bX,int bY,int cX,int cY,float *a_rgba,float *b_rgba,float *c_rgba){
	int xmax,xmin,ymax,ymin;
	xmax = max(aX,bX,cX);
	xmin = min(aX,bX,cX);
	ymax = max(aY,bY,cY);
	ymin = min(aY,bY,cY);
		
	float a_bc = (bY-cY) * aX + (cX-bX) * aY + bX * cY - cX * bY;
	float b_ac = (aY-cY) * bX + (cX-aX) * bY + aX * cY - cX * aY;
	float c_ab = (aY-bY) * cX + (bX-aX) * cY + aX * bY - bX * aY;

	float edge_bc = (bY-cY) * -1 + (cX-bX) * -1 + bX * cY - cX * bY;
	float edge_ac = (aY-cY) * -1 + (cX-aX) * -1 + aX * cY - cX * aY;
	float edge_ab = (aY-bY) * -1 + (bX-aX) * -1 + aX * bY - bX * aY;

	if(edge_bc == 0)
		edge_bc = (bY-cY) * -2 + (cX-bX) * -1 + bX * cY - cX * bY;
	if(edge_ac == 0)
		edge_ac = (aY-cY) * -2 + (cX-aX) * -1 + aX * cY - cX * aY;
	if(edge_ab == 0)
		edge_ab = (aY-bY) * -2 + (bX-aX) * -1 + aX * bY - bX * aY;


	int x,y;
	float p_rgba[4];

	for (y = ymin; y <= ymax; y++)
	{
		for (x = xmin; x <= xmax; x++)
		{
			float alpha = (bY-cY) * x + (cX-bX) * y + bX * cY - cX * bY;
			float beta = (aY-cY) * x + (cX-aX) * y + aX * cY - cX * aY;
			float gamma = (aY-bY) * x + (bX-aX) * y + aX * bY - bX * aY;

			alpha = alpha/a_bc;
			beta = beta/b_ac;
			gamma = gamma/c_ab;

			if(alpha >= 0 && beta >= 0 && gamma >= 0)
			{
				if( (alpha > 0 || a_bc * edge_bc >0) && (beta > 0 || b_ac * edge_ac >0) && (gamma > 0 || c_ab * edge_ab > 0))
				{				
					p_rgba[0] = alpha * a_rgba[0] + beta * b_rgba[0] + gamma * c_rgba[0];
					p_rgba[1] = alpha * a_rgba[1] + beta * b_rgba[1] + gamma * c_rgba[1];
					p_rgba[2] = alpha * a_rgba[2] + beta * b_rgba[2] + gamma * c_rgba[2];

					glColor3f(p_rgba[0],p_rgba[1],p_rgba[2]);
					glVertex2i(x,y);
				}	
			}
		}
	}
}

int Render_SSD(SCENE *ascene, CAMERA *acamera)
{
  int ii, j;
  /* We clear all pixels  */
  glClearColor(ascene->bcolor.rgba[0], ascene->bcolor.rgba[1],
	       ascene->bcolor.rgba[2], ascene->bcolor.rgba[3]);
  glClear (GL_COLOR_BUFFER_BIT);
  
  /* We can draw all the triangles here */
  glBegin(GL_POINTS);
  for (ii=0; ii < ascene->ntriangles; ii++) {
    
	int aX, aY, bX, bY, cX, cY;
	float a_rgba[4],b_rgba[4],c_rgba[4];
	int i;

	aX = ascene->triangles[ii].vertices[0].xyzw[0];
	aY = ascene->triangles[ii].vertices[0].xyzw[1];
	bX = ascene->triangles[ii].vertices[1].xyzw[0];
	bY = ascene->triangles[ii].vertices[1].xyzw[1];
	cX = ascene->triangles[ii].vertices[2].xyzw[0];
	cY = ascene->triangles[ii].vertices[2].xyzw[1];

	for (i = 0; i < 4; i++)
	{
		a_rgba[i] = ascene->triangles[ii].vertices[0].rgba[i];
		b_rgba[i] = ascene->triangles[ii].vertices[1].rgba[i];
		c_rgba[i] = ascene->triangles[ii].vertices[2].rgba[i];

	}

	drawtriangle(aX,aY,bX,bY,cX,cY,a_rgba,b_rgba,c_rgba);	
     
  }
  glEnd();
  

  glBegin(GL_POINTS);
  //draw polyline
  int i;
  int startX,startY,endX,endY;
  int width;
  float startRgba[4],endRgba[4];
  for (ii=0; ii < ascene->npolylines; ii++) {

    
    width = ascene->polylines[ii].width;

    for (j=0; j < ascene->polylines[ii].nvertices-1; j++) {

	startX = ascene->polylines[ii].vertices[j].xyzw[0];
	startY = ascene->polylines[ii].vertices[j].xyzw[1];
	endX = ascene->polylines[ii].vertices[j+1].xyzw[0];
	endY = ascene->polylines[ii].vertices[j+1].xyzw[1];

	for (i = 0; i < 4; i++)
	{
		startRgba[i] = ascene->polylines[ii].vertices[j].rgba[i];
		endRgba[i] = ascene->polylines[ii].vertices[j+1].rgba[i];

	}
	drawline(startX, startY, endX, endY,startRgba,endRgba,width);

    }

  }
  glEnd();

  glBegin(GL_POINTS);
  
  //draw line
  for (ii=0; ii < ascene->nlines; ii++)
  {

	width = ascene->lines[ii].width;

	startX = ascene->lines[ii].vertices[0].xyzw[0];
	startY = ascene->lines[ii].vertices[0].xyzw[1];
	endX = ascene->lines[ii].vertices[1].xyzw[0];
	endY = ascene->lines[ii].vertices[1].xyzw[1];

	for (i = 0; i < 4; i++)
	{
		startRgba[i] = ascene->lines[ii].vertices[0].rgba[i];
		endRgba[i] = ascene->lines[ii].vertices[1].rgba[i];
	}

	drawline(startX, startY, endX, endY, startRgba,endRgba,width);

  }

  glEnd();

  glFlush ();
  return 0;
}


/*
 * Declare initial window size, position, and display mode 
 * (single buffer and RGBA).  Open window with the executable
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
  int ii, jj, kk, argc_1;
  char **my_argv;
  char ssd_fname[MAXFILELEN];
  if (argc < 2) {
    printf("%s:%d Usage: %s SSD_file\n", 
	   __FILE__, __LINE__, argv[0]);
    return 0;
  }
  strcpy(ssd_fname, argv[1]);
  strcpy(saved_fname,"graphics_tmp.ppm");
  argc_1 = argc - 1;
  my_argv = (char **)malloc(sizeof(char *) * argc);
  my_argv[0] = argv[0];
  for (ii=2; ii <= argc; ii++) {
    my_argv[ii-1] = argv[ii];
  }
  glutInit(&argc_1, my_argv);
  free(my_argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  /* Set the default size and background */
  Read_SSD_Scene(ssd_fname, &thescene, saved_fname);
  glutInitWindowSize (thescene.screen_w, thescene.screen_h);
  glutInitWindowPosition (50, 50);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);  
  //glutMouseFunc(mouse);
  //glutKeyboardFunc(ssd_keyboard);
  glutMainLoop();
  return 0;   /* ANSI C requires main to return int; it will never be 
		 reached as glutMainLoop() does not return. */
}


