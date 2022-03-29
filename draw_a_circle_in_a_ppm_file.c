/*********************************************/
/***     draw_a_circle_in_a_ppm_file.c     ***/
/*********************************************/
/*  This program inserts an outline of a     */
/*    circle of radius r centered on point   */
/*    x,y in a .ppm file, all passed as      */
/*    shell arguments.                       */
/*********************************************/
/* Authors/maintainers: ciscorx@gmail.com    */
/* License: GNU GPL v3                       */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int isPositiveNumber(char number[])
{
    int i;

    for (i=0; number[i] != NULL && i<12; i++)
    {
        if (!isdigit(number[i]))
            return 0;
    }
    return 1;
}


int main(int argc, char *argv[]) {
  const int dimx = 800, dimy = 800;
  int i, j, r, dy, dx;
  char c;
  char dimensions[100];
  char scanned_string[12];
  int height, width, depth,x,y,w,h;
  int pos_beginning_of_image;
  int pos_end_of_image;
  int pos_top;
  int pos_bottom;
  int horizontal_thickness;
  int vertical_thickness;
  int pos_cur;
  int pos_center;
  char color[3];	     
  color[1]=255;
  color[2]=0;
  color[3]=255;
  
  if (argc == 2 && *argv[1] != NULL && (argv[1][0] == '-' || argv[1][0] == 'h') ) {
      printf("\n");
      printf("  Usage:\n");
      printf("  draw_a_circle_in_a_ppm_file.o <filename> <x> <y> <r>\n");
      printf("     This program inserts the outline of a circle of given radius r to a given location x,y in a given .ppm file.\n");
      exit(1);
  }
  if (argc !=5) {
      printf("This program requires 4 arguments: the file name of the ppm image, the location in the file x y the dimensions of the box l w\n ");
      exit(1);
  }
  if (!isPositiveNumber(argv[2]) || !isPositiveNumber(argv[3]) || !isPositiveNumber(argv[4]) ) {
      printf("x y r must all be positive integers, each having a maximum of 12 digits\n.");
     
      exit(1);
  }
	  
  x=atoi(argv[2]);
  y=atoi(argv[3]);
  r=atoi(argv[4]);
    
    
  FILE *fp = fopen(argv[1], "rb+"); 
  if (fp==NULL) {
      printf("Unable to open .ppm image file: %s", argv[1]);
      exit(1);
  }

  c = fgetc(fp);  /* parse file header */
  if ( c=='P') {
      c = fgetc(fp);
      if (c=='6') 
	  c = fgetc(fp);
      if (c==10) {
	  c = fgetc(fp);
	  if (c=='#')   /* skip any commments */
	      while (c != 10)
		  c = fgetc(fp);
	  /* else */
	      fseek(fp,-1,SEEK_CUR); 
	  
	  fscanf(fp,"%s",dimensions);
	  width=atoi(dimensions);

	  fscanf(fp,"%s",dimensions);

	  height=atoi(dimensions);

	  fscanf(fp,"%s",dimensions);

	  depth=atoi(dimensions);
	  pos_beginning_of_image=ftell(fp);
	  fseek(fp,0,2);
	  pos_end_of_image=ftell(fp);
	  pos_center = pos_beginning_of_image + x*3 + y*width*3;
	  pos_top = pos_center - r*width*3;
	  pos_bottom = pos_center + r*width*3 - 3;
	  
	  /* draw a pixel point at center of circle */
	  if (pos_center < pos_end_of_image && pos_top > pos_beginning_of_image) {
	      fseek(fp,pos_center,0);
	      (void) fwrite(color, 3, 1, fp);
	  }
	  
	  /* draw a pixel point at bottom of circle */
	  if (pos_bottom > pos_beginning_of_image && pos_bottom < pos_end_of_image) {
	      fseek(fp,pos_bottom,0);
	      (void) fwrite(color, 3, 1, fp);
	  }
	  
	  /* draw a pixel point at top of circle */
	  if( pos_top > pos_beginning_of_image && pos_top < pos_end_of_image) {
	      fseek(fp,pos_top,0);
	      (void) fwrite(color, 3, 1, fp);
	  }
	  
	  /* render circle top to down */ 
	  for (dy=2*r;dy>0;dy--) {
	      if((pos_bottom - (2*r-dy)*width*3 < pos_end_of_image) && (pos_bottom - (2*r-dy)*width*3 > pos_beginning_of_image))    {
		  pos_cur = pos_bottom - dy*width*3;
		  fseek(fp,pos_cur,0);
		  dx = round(3*abs(sqrt( r*r - (abs(r)-dy)*(abs(r)-dy))));
		  if (x*3-dx > 0){ 
		      fseek(fp,-dx,1);
		      (void) fwrite(color, 3, 1, fp);
		  }
		  fseek(fp,pos_cur,0);
		  if (x*3+dx< width*3) {
		      fseek(fp,dx,1);   
		      (void) fwrite(color, 3, 1, fp);
		  }
	      }
	  }


	  fflush(fp);
      }
  }
  (void) fclose(fp);
  return EXIT_SUCCESS;
}
