// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.


//Link to the header file
#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>


//Use the cimg namespace to access functions easily
using namespace cimg_library;
using namespace std;

void draw_descriptor_image(CImg<double> image, const vector<SiftDescriptor> descriptors, const char *filename)
{
  for(unsigned int i=0; i < descriptors.size(); i++)
    {
      int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
      double color_point[] = {255.0, 255.0, 0};
      for(int x=-2; x<3; x++)
	for(int y=-2; y<3; y++)
	  if(x==0 || y==0)
	    for(int c=0; c<3; c++){
	      //Find if coordinates are in workspace to draw crosshair
	      tx1 = (descriptors[i].col + y - 1);
	      ty1 = (descriptors[i].row + x - 1);
	      if (tx1 >= 0 && tx1 < image.width() && ty1 >= 0 && ty1 < image.height())
		image( tx1, ty1, 0, c) = color_point[c];				
	    }
    }
  image.get_normalize(0,255).save(filename);
}

int main(int argc, char **argv)
{
  try {
    
    /*
      TEST CODE - STARTS
    */
    cout << "argv[1]: " << argv[1] << endl;
    string part = argv[1];
    
    if(part == "part1"){
      // Billboard
    }	
    else if(part == "part2"){
      cout << "starting part 2 " << endl;
      // Blending
      // Given 2 images and a mask
      string image1_str = argv[2];
      string image2_str = argv[3];
      string mask_str = argv[4];
      
      // Read in the images
      CImg<double> image1;
      CImg<double> image2;
      CImg<double> mask;

      image1.load(image1_str.c_str());
      image2.load(image2_str.c_str());
      mask.load(mask_str.c_str());

      // 1. For each img, compute the Gaussian pyramid and the Laplacian pyramid
      // Gaussian pyramids
      //int rounds = 5;		// this is the number of levels in the Gaussian pyramid

      // Here is the Gaussian low-pass filter:
       CImg<double> filter(5, 5, 1, 1, 0);

       filter(0, 0) = 1;
       filter(0, 1) = 4;
       filter(0, 2) = 6;
       filter(0, 3) = 4;
       filter(0, 4) = 1;

       filter(1, 0) = 4;
       filter(1, 1) = 16;
       filter(1, 2) = 24;
       filter(1, 3) = 16;
       filter(1, 4) = 4;

       filter(2, 0) = 6;
       filter(2, 1) = 24;
       filter(2, 2) = 36;
       filter(2, 3) = 24;
       filter(2, 4) = 6;

       filter(3, 0) = 4;
       filter(3, 1) = 16;
       filter(3, 2) = 24;
       filter(3, 3) = 16;
       filter(3, 4) = 4;

       filter(4, 0) = 1;
       filter(4, 1) = 4;
       filter(4, 2) = 6;
       filter(4, 3) = 4;
       filter(4, 4) = 1;
 
       // convolve each image with the filter 5 times to get a total of 6 levels of the pyramid
       // IMAGE 1 PYRAMID
       CImg<double> G0_1 = image1;
       CImg<double> G1_1;
       G1_1 = G0_1.get_convolve(filter * 1/256);
       G1_1.resize(153, 153);
       G1_1.save("image1_G1.jpg", -1, 6);

       CImg<double> G2_1;
       G2_1 = G1_1.get_convolve(filter * 1/256);
       G2_1.resize(77, 77);
       G2_1.save("image1_G2.jpg", -1, 6);

       CImg<double> G3_1;
       G3_1 = G2_1.get_convolve(filter * 1/256);
       G3_1.resize(39, 39);
       G3_1.save("image1_G3.jpg", -1, 6);

       CImg<double> G4_1;
       G4_1 = G3_1.get_convolve(filter * 1/256);
       G4_1.resize(20, 20);
       G4_1.save("image1_G4.jpg", -1, 6);

       CImg<double> G5_1;
       G5_1 = G4_1.get_convolve(filter * 1/256);
       G5_1.resize(10, 10);
       G5_1.save("image1_G5.jpg", -1, 6);

       // IMAGE 2 PYRAMID
       CImg<double> G0_2 = image2;
       CImg<double> G1_2;
       G1_2 = G0_2.get_convolve(filter * 1/256);
       G1_2.resize(153, 153);
       G1_2.save("image2_G1.jpg", -1, 6);

       CImg<double> G2_2;
       G2_2 = G1_2.get_convolve(filter * 1/256);
       G2_2.resize(77, 77);
       G2_2.save("image2_G2.jpg", -1, 6);

       CImg<double> G3_2;
       G3_2 = G2_2.get_convolve(filter * 1/256);
       G3_2.resize(39, 39);
       G3_2.save("image2_G3.jpg", -1, 6);

       CImg<double> G4_2;
       G4_2 = G3_2.get_convolve(filter * 1/256);
       G4_2.resize(20, 20);
       G4_2.save("image2_G4.jpg", -1, 6);

       CImg<double> G5_2;
       G5_2 = G4_2.get_convolve(filter * 1/256);
       G5_2.resize(10, 10);
       G5_2.save("image2_G5.jpg", -1, 6);
       /*
       std::string filename = "image1_G";
       
       CImg<double>[] levels(6); 	// store all the levels of the pyramid here
       levels[0] = image1;
       for (int i = 1; i <= rounds; i++) {
         CImg<double> G_level = convolve(levels[i-1], filter);
         levels[i] = G_level;
         G_level.save(filename + NumberToString(i) + ".png", -1, 6);
       }
       */
      // Now the Laplacian pyramids
      //
      //
      /*
      CImg<double> L_filter(3, 3);
      L_filter(0,0) = 0;
      L_filter(0,1) = 1;
      L_filter(0,2) = 0;

      L_filter(1,0) = 1;
      L_filter(1,1) = -4;
      L_filter(1,2) = 1;

      L_filter(2,0) = 0;
      L_filter(2,1) = 1;
      L_filter(2,2) = 0;
      */
      
      // IMAGE 1 PYRAMID 
      CImg<double> L0_1(20, 20, 1, 3);
      //CImg<double> LOG_1;
      CImg<double> resized_G5_1;
      resized_G5_1 = G5_1.get_resize_doubleXY();
      resized_G5_1.save("image1_resized_test.jpg", -1, 6);
      for(int i=0; i < resized_G5_1.width(); i++) {
        for (int j=0; j < resized_G5_1.height(); j++) {
          for (int k=0; k < 3; k++) {
            L0_1(i, j, 0, k) = resized_G5_1(i, j, 0, k) - G4_1(i, j, 0, k);
          }
        }
      }
      //LOG_1 = resized_G5_1 - G4_1;
      //L0_1 = G0_1.get_convolve(LOG_1);
      L0_1.save("image1_L0_NEW.jpg", -1, 6);

       // IMAGE 2 PYRAMID

      // 2. Build Gaussian for the mask    
       CImg<double> G0_mask = mask;
       CImg<double> G1_mask;
       G1_mask = G0_mask.get_convolve(filter * 1/256);
       G1_mask.resize(153, 153);
       G1_mask.save("mask_G1.jpg", -1, 6);

       CImg<double> G2_mask;
       G2_mask = G1_mask.get_convolve(filter * 1/256);
       G2_mask.resize(77, 77);
       G2_mask.save("mask_G2.jpg", -1, 6);

       CImg<double> G3_mask;
       G3_mask = G2_mask.get_convolve(filter * 1/256);
       G3_mask.resize(39, 39);
       G3_mask.save("mask_G3.jpg", -1, 6);

       CImg<double> G4_mask;
       G4_mask = G3_mask.get_convolve(filter * 1/256);
       G4_mask.resize(20, 20);
       G4_mask.save("mask_G4.jpg", -1, 6);

       CImg<double> G5_mask;
       G5_mask = G4_mask.get_convolve(filter * 1/256);
       G5_mask.resize(10, 10);
       G5_mask.save("mask_G5.jpg", -1, 6);

      // 3. Blended Laplacian pyramid
      //  - sum of images in corresponding level of the two images
      //  - formula used in given in assignment
      //
      // 4. Form the blended image  
    }
    else if(part == "part3"){
      // RANSAC
    }
    else if(part == "part4"){
      // Panorama
    }
    
    
    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}
