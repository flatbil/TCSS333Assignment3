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
 p->height = 0;
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
  fprintf(stdout, "Hello I am in pixMap_read just before opening the input file.\nfilename is %s\n", filename);
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
	  if((dest->width != source->width) || (dest->height != source->height)){
	    //if width*height is different then we need to allocate dest->image /*allocate what?*/
	    if((source->width*source->height)!=(dest->width*dest->height)){
			/*allocate dest->image*/
			dest->image = malloc(dest->height*dest->width*sizeof(rgba));
	      //if dest->image is zero use malloc to allocate memory for dest->image
	      if(dest->image == 0){
			  dest->image = malloc(dest->height*dest->width*sizeof(rgba));
		  }
	      //else use realloc to allocate memory dest->image
	      else {
			  realloc(dest->image, dest->height*dest->width*sizeof(rgba));
		  }
	    //
		}
	    //if dest->height is different
	    if(dest->height != source->height){
			//malloc or realloc dest->pixArray depending on whether dest->pixArray is zero
			if(dest->pixArray == 0){
				dest->pixArray = malloc(sizeof(rgba*) * dest->height);
			}else{
				realloc(dest->pixArray, sizeof(rgba*) * dest->height);
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
	  memcpy(dest->image, source->image, sizeof(source->image));
	  //set dest->width and dest->height to source values
	  dest->width = source->width;
	  dest->height = source->height;
	//
		}
	}
}

static void pixMap_reset(pixMap *p){
	free(p->pixArray);
	free(p->image);
	p->height = 0;
	p->width = 0;
	//free all the memory in the fields and set everything to zero
	
}	


void pixMap_destroy(pixMap *p){
	//reset pixMap and free the object p
	pixMap_reset(p);
	free(p);
	
}


void pixMap_rotate (pixMap *p, float theta){
 //make a new temp blank pixMap structure
 pixMap *temp; /*does this make a struct?*/
 temp = pixMap_init();
 //copy p to temp
 pixMap_copy(temp, p);
 //set the values in the image to zero using memset - Note that the 3rd argument of memset is the size in BYTES
 memset(temp, temp->image, p->height*p->width*sizeof(rgba));
 //calculate the coordinates ox and oy of the middle of the png graphic
 int ox = (p->width*sizeof(rgba) / 2);
 int oy = (p->height*sizeof(rgba) / 2);
 //calculate the values of sine and cosine used by the rotation formula FOR -theta
 
	for(int y=0;y<p->height;y++){   //two for loops to loop through each pixel in the new rotated image
	 for(int x=0;x<p->width;x++){
	 
	    //calculate the old coordinates rotx roty by rotating by -theta and using the formula described here
	    //http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	    //use the answer from stackoverflowery
	      float s = sin(-theta*3.14/180);
		  float c = cos(-theta*3.14/180);

  // translate point back to origin:
			p->width -= ox;
			p->height -= oy;

  // rotate point
			float rotx = c*(x-ox) - s * (oy-y) + ox;
			float roty = -(s*(x-ox) + c * (oy-y) - oy);

  // translate point back:
			p->width = rotx + ox;
			p->height = roty + oy;
	    //However this answer assumes that y is going from the bottom to the top (mathematical convention)
	    //but the pixmap starts at the upper left hand corner and height grows DOWN (scan order)
	    //so use this formula instead where c is cos(degreesToRadians(-theta)) and s is sin(degreeToRadians(-theta))
	    //    float rotx = c*(x-ox) - s * (oy-y) + ox;
     //    float roty = -(s*(x-ox) + c * (oy-y) - oy);
	    
 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)
 	   int introtx;
 	   int introty;
 	   introtx = (int) ((float)rotx + 0.5);
 	   introty = (int) ((float)roty + 0.5);	
 	   
	
	    //if old coordinates are within the height and width limits
	    if(x-introtx > 0 && y-introty > 0){
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
	      //i.e. 	memcpy(temp->pixArray[y]+x,p->pixArray[roty]+rotx,sizeof(rgba))
	      memcpy(temp->pixArray[y]+x,p->pixArray[introty]+introtx,sizeof(rgba));
	    //
	    }
	    
	  //
	  }
	//
	}  
	//copy the temp pixMap to the original
	memcpy(p, temp, sizeof(*temp));
//destroy the temp;
pixMap_destroy(temp);
}	

void pixMap_gray (pixMap *p){
	//for() loop through pixels using two for loops 
	for(int i = 0; i < sizeof(p->height);i++){
	  //for()
	  for(int j = 0; j < sizeof(p->width); j++){
	    //calculate average value of r and g and b values (assign to a float variable)
	    //for example the red value of for the pixel at height i, width j would be p->pixel[i][j].r
	    float average = (p->pixArray[i][j].r + p->pixArray[i][j].g + p->pixArray[i][j].b)/3;
	    //round float variable to integer (add 0.5 before casting to integer)
	    int castedAverage = (int)(average + 0.5);
	    //assign the rounded value to r,g and b values
	    p->pixArray[i][j].r = castedAverage;
	    p->pixArray[i][j].g = castedAverage;
	    p->pixArray[i][j].b = castedAverage;
	  //
	  }
	//
	}   
}
int pixMap_write(pixMap *p,char *filename){
 //write out to filename using lodepng_encode32_file
 //example is in lodepng/examples - one liner
	/*Encode the image*/
	unsigned error = lodepng_encode32_file(filename, p->image, p->width, p->height);

	/*if there's an error, display it*/
	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	return 0;
}	 
