#include "pixutils.h"

int main(int argc, char *argv[]){
  char *inputfile="hello",*outputfile=0;
  float degrees=0, grayFlag=0;
  int i = 0;
  fprintf(stdout, "This is the input file before the assignment %s\n", inputfile);
  fprintf(stdout, "This is the output before the assignment %s\n", outputfile);
  
  while(i < argc){
	  fprintf(stdout, "This is the argv[%d] %s\n", i, argv[i]);
	  if(argv[i] == "-i"){
		  inputfile = argv[(i+1)];
		  fprintf(stdout, "This is the input file inside the comparison %s\n", argv[(i+1)]);
	  } 
	  if (argv[i] == '-o'){
		  outputfile = argv[(i+1)];
		  fprintf(stdout, "This is the output file inside the comparison %s\n", argv[(i+1)]);
	  } 
	  if (argv[i] == '-r'){
		  degrees = atof(argv[i+1]);
	  } 
	  if (argv[i] == '-g'){
		  grayFlag = 1;
	  }
	  i++;
  }
  //write the parser yourself or use a package like https://github.com/skeeto/optparse
  
		//check for flags -i -o -r -g - can be in any order
  //-i is followed by the input png
  //-o is followed the output png
  //-r is followd by the rotation angle in degrees (float) <optional for user>
  //-g is whether th png should be grayed <optional for user>
  
  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees)pixMap_rotate(p,degrees);
  if(grayFlag)pixMap_gray(p);
  pixMap_write(p,outputfile);
  pixMap_destroy(p);
  
  return 0;
}

