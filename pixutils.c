#include "pixutils.h"
#include "lodepng.h"

//private methods
static pixMap *pixMap_init(); //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source); //copy one pixmap to another
static int pixMap_read(pixMap *p,char *filename); //read in a pixmap from file

static pixMap* pixMap_init(){
 //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
 pixMap *p;
 p = malloc(sizeof(pixMap));
 p->hight = 0;
 p->width = 0;
 return p;
 
}	
pixMap* pixMap_init_filename(char *filename){
	//use pixMap_int to create the pixMap object
	//use pixMap read to fill the fields of the object
	//return the pointer to the new object
	pixMap *pix = pixMap_init();
	pixMap_read(pix, filename);
	return pix;
}
static int pixMap_read(pixMap *p,char *filename){
 //read and allocate image, read in width and height using using lodepng_decode32_file
  //example is in lodepng/examples - one liner
  unsigned error;
  error = lodepng_decode32_file(p->image, &(p->width), &(p->height), filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
 
	//then allocate p->pixArray to hold p->height pointers
	p->pixArray = malloc(sizeof(rgba*) * p->height);
	//set p->pixArray[0] to p->image and p->pixArray[i]=p->pixArray[i-1]+p->width
	
	p->pixArray[0] = p->image;
	for(int i =1; i < p->height; i++){
		p->pixArray[i] = p->pixArray[i-1]+p->width;
	}
	
	return 0;
}	
static void pixMap_copy(pixMap *dest,pixMap *source){
	//if source image is zero then reset dest and copy width and height
	if(source == 0){ //how do I reset dest?
		pixMap_reset(dest);
	} 
	//if source image is not zero
	else {
	  //if width or height are different
	  if(!(dest->width == dest->height)){// different than what?
	    //if width*height is different then we need to allocate dest->image /*allocate what?*/
	    if(!width*height){ /*Again, different than what?*/
			/*allocate dest->image*/
	      //if dest->image is zero use malloc to allocate memory for dest->image
	      if(dest->image == 0){
			  dest->image = malloc(sizeof(/*how much do I need?*/));
		  }
	      //else use realloc to allocate memory dest->image
	      else {
			  dest->image = realloc(sizeof(/*how much?*/));
		  }
	    //
		}
	    //if dest->height is different
	    if(!dest->height/*different than what?*/){
			//malloc or realloc dest->pixArray depending on whether dest->pixArray is zero
			dest->pixArray = malloc();
	    //
		}
	    //even if the height is the same set dest->pixArray[0] to dest->image and dest->pixArray[i]=dest->pixArray[i-1]+source->width 
	    dest->pixArray[0] = dest->image;
	    for(int i = 1; i < source->width; i++){
			dest->pixArray[i] = dest->pixArray[i-1]+source->width;
		}
	  //
	  }
	  //do a memcpy from source->image to dest->image
	  memcpy(dest->image, source->image);
	  //set dest->width and dest->height to source values
	  dest->width = source->width;
	  dest->height = source->height;
	//
	}
}

static void pixMap_reset(pixMap *p){
	free(
	//free all the memory in the fields and set everything to zero
	
}	


void pixMap_destroy(pixMap *p){
	//reset pixMap and free the object p
}


void pixMap_rotate (pixMap *p, float theta){
 //make a new temp blank pixMap structure
 //copy p to temp
 //set the values in the image to zero using memset - Note that the 3rd argument of memset is the size in BYTES

 //calculate the coordinates ox and oy of the middle of the png graphic
 //calculate the values of sine and cosine used by the rotation formula 

	//for(int y=0;y<p->height;y++){   //two for loops to loop through each pixel in the original
	 //for(int x=0;x<p->width;x++){
	 
	    //calculate the new rotated coordinates rotx roty using the formula from 
	    //http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	    //use the answer from stackoverflowery
 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)	
	
	    //if rotated coordinates are within the height and width limits
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
	      //i.e. 	memcpy(temp->pixArray[roty]+rotx,p->pixArray[y]+x,sizeof(rgba))
	    //
	    
	  //
	//  
	//copy the temp pixMap to the original
	//destroy the temp;
}	

void pixMap_gray (pixMap *p){
	//for() loop through pixels using two for loops 
	for(int i = 0; i < 
	  //for()
	    //calculate average value of r and g and b values (assign to a float variable)
	    //for example the red value of for the pixel at height i, width j would be p->pixel[i][j].r
	    //round float variable to integer (add 0.5 before casting to integer)
	    //assign the rounded value to r,g and b values
	  //
	//   
}
int pixMap_write(pixMap *p,char *filename){
 //write out to filename using lodepng_encode32_file
 //example is in lodepng/examples - one liner
	return 0;
}	 
