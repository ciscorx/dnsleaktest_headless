/*********************************************/
/***    draw_a_rectangle_in_a_ppm_file.c   ***/
/*********************************************/
/*  This program inserts an outline of a box */
/*    of dimensions wxh to location x,y in a */
/*    .ppm file, all passed as shell         */
/*    arguments.                             */
/*********************************************/
/* Authors/maintainers: ciscorx@gmail.com    */
/* License: GNU GPL v3                       */

#include <stdlib.h>
#include <stdio.h>
 
int isPositiveNumber(char number[])
{
    int i;

    for (i=0; number[i] != 0 && i<12; i++)
    {
        if (!isdigit(number[i]))
            return 0;
    }
    return 1;
}


int main(int argc, char *argv[]) {
  const int dimx = 800, dimy = 800;
  int i, j, cnt;
  char c;
  char dimensions[100];
  char scanned_string[12];
  int height, width, depth,x,y,w,h;
  int pos_beginning_of_image;
  int pos_end_of_image;
  int horizontal_thickness;
  int vertical_thickness;
  char color[3];	     
  color[1]=255;
  color[2]=0;
  color[3]=255;
      
  if (argc == 2 && *argv[1] != 0 && (argv[1][0] == '-' || argv[1][0] == 'h') ) {
      printf("\n");
      printf("  Usage:\n");
      printf("  draw_a_rectangle_in_a_ppm.o <filename> <x> <y> <w> <h>\n");
      printf("     This program inserts the outline of a rectangle of given dimensions hxw, where the upper left corner is positioned at location x,y in a given .ppm file.\n");
      exit(1);
  }
  if (argc !=6) {
      printf("This program requires 5 arguments: the file name of the ppm image, the location in the file x y where the upper left corner of the rectangle will lie, the dimensions of the rectangle w l\n ");
      exit(1);
  }
  if (!isPositiveNumber(argv[2]) || !isPositiveNumber(argv[3]) || !isPositiveNumber(argv[4]) || !isPositiveNumber(argv[5])) {
      printf("x y w h must all be positive integers, each having a maximum of 12 digits\n.");
     
      exit(1);
  }
	  
  x=atoi(argv[2]);
  y=atoi(argv[3]);
  w=atoi(argv[4]);
  h=atoi(argv[5]);
    
    
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
	  /* printf("dimensions of image are %dx%d, dimenions of box is %dx%d, located at %d,%d\n",width,height,w,h,x,y); */
	  
	  /* draw top horizontal line */
	  fseek(fp,pos_beginning_of_image + x*3 + y*width*3,0);
	  for (i = 0; i < w ; i++)
	      (void) fwrite(color, 3, 1, fp);
	  // fseek(fp,0,1);

	  /* draw bottom horizontal line */
	  fseek(fp,pos_beginning_of_image + x*3 + y*width*3 + h*width*3,0);
	  for (i = 0; i < w ; i++)
	      (void) fwrite(color, 3, 1, fp);

          /* draw a diagonal backslash (\) line from top left corner of box */
	  /* fseek(fp,pos_beginning_of_image + x*3 + y*width*3,0); */
	  /* for (j = 0; j < h ; j++) { */
	  /*     fseek(fp,width*3,1); */
	  /*     (void) fwrite(color, 3, 1, fp); */
	  /*     fseek(fp,0,1); */
	      
	  /* } */

          /* draw left vertical line */
	  fseek(fp,pos_beginning_of_image + x*3 + y*width*3,0);
	  for (j = 0; j < h ; j++) {
	      fseek(fp,width*3 - 3,1);
	      (void) fwrite(color, 3, 1, fp);
	      fseek(fp,0,1);
	      
	  }

          /* draw right vertical line */
	  fseek(fp,pos_beginning_of_image + x*3 + y*width*3 + w*3,0);
	  for (j = 0; j < h ; j++) {
	      fseek(fp,width*3 - 3,1);
	      (void) fwrite(color, 3, 1, fp);
	      fseek(fp,0,1);
	  }
	  fflush(fp);
      }
  }
  (void) fclose(fp);
  return EXIT_SUCCESS;
}
