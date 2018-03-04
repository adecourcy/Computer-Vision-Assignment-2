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
    string part = argv[1];
    
    if(part == "part1"){
      // Billboard
    }	
    else if(part == "part2"){
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
       CImg<double> filter(5, 5);

       filter(0, 0) = 1.0;
       filter(0, 1) = 4.0;
       filter(0, 2) = 6.0;
       filter(0, 3) = 4.0;
       filter(0, 4) = 1.0;

       filter(1, 0) = 4.0;
       filter(1, 1) = 16.0;
       filter(1, 2) = 24.0;
       filter(1, 3) = 16.0;
       filter(1, 4) = 4.0;

       filter(2, 0) = 6.0;
       filter(2, 1) = 24.0;
       filter(2, 2) = 36.0;
       filter(2, 3) = 24.0;
       filter(2, 4) = 6.0;

       filter(3, 0) = 4.0;
       filter(3, 1) = 16.0;
       filter(3, 2) = 24.0;
       filter(3, 3) = 16.0;
       filter(3, 4) = 4.0;

       filter(4, 0) = 1.0;
       filter(4, 1) = 4.0;
       filter(4, 2) = 6.0;
       filter(4, 3) = 4.0;
       filter(4, 4) = 1.0;
       
       filter /= 256.0;
 
       // convolve each image with the filter 5 times to get a total of 6 levels of the pyramid
       // IMAGE 1 PYRAMID
       CImg<double> G0_1 = image1;
       G0_1.save("image1_G0.jpg", -1, 6);

       CImg<double> G1_1;
       G1_1 = G0_1.get_convolve(filter);
       G1_1.resize(153, 153);
       G1_1.save("image1_G1.jpg", -1, 6);

       CImg<double> G2_1;
       G2_1 = G1_1.get_convolve(filter);
       G2_1.resize(77, 77);
       G2_1.save("image1_G2.jpg", -1, 6);

       CImg<double> G3_1;
       G3_1 = G2_1.get_convolve(filter);
       G3_1.resize(39, 39);
       G3_1.save("image1_G3.jpg", -1, 6);

       CImg<double> G4_1;
       G4_1 = G3_1.get_convolve(filter);
       G4_1.resize(20, 20);
       G4_1.save("image1_G4.jpg", -1, 6);

       CImg<double> G5_1;
       G5_1 = G4_1.get_convolve(filter);
       G5_1.resize(10, 10);
       G5_1.save("image1_G5.jpg", -1, 6);

       // IMAGE 2 PYRAMID
       CImg<double> G0_2 = image2;
       CImg<double> G1_2;
       G1_2 = G0_2.get_convolve(filter);
       G1_2.resize(153, 153);
       G1_2.save("image2_G1.jpg", -1, 6);

       CImg<double> G2_2;
       G2_2 = G1_2.get_convolve(filter);
       G2_2.resize(77, 77);
       G2_2.save("image2_G2.jpg", -1, 6);

       CImg<double> G3_2;
       G3_2 = G2_2.get_convolve(filter);
       G3_2.resize(39, 39);
       G3_2.save("image2_G3.jpg", -1, 6);

       CImg<double> G4_2;
       G4_2 = G3_2.get_convolve(filter);
       G4_2.resize(20, 20);
       G4_2.save("image2_G4.jpg", -1, 6);

       CImg<double> G5_2;
       G5_2 = G4_2.get_convolve(filter);
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
      
      // IMAGE 1 PYRAMID 
      CImg<double> L0_1(10, 10, 1, 3);
      L0_1 = G5_1;
      L0_1.save("image1_L0.jpg", -1, 6);

      //CImg<double> smooth0_1 = G0_1.get_convolve(filter);
      //L0_1 = G0_1 - smooth0_1;
      //L0_1.save("image1_L0.jpg", -1, 6);

      CImg<double> L1_1(20, 20, 1, 3);
      CImg<double> smooth1_1 = G4_1.get_convolve(filter);
      L1_1 = G4_1 - smooth1_1;
      L1_1.save("image1_L1.jpg", -1, 6);

      CImg<double> L2_1(39, 39, 1, 3);
      CImg<double> smooth2_1 = G3_1.get_convolve(filter);
      L2_1 = G3_1 - smooth2_1;
      L2_1.save("image1_L2.jpg", -1, 6);

      CImg<double> L3_1(77, 77, 1, 3);
      CImg<double> smooth3_1 = G2_1.get_convolve(filter);
      L3_1 = G2_1 - smooth3_1;
      L3_1.save("image1_L3.jpg", -1, 6);

      CImg<double> L4_1(153, 153, 1, 3);
      CImg<double> smooth4_1 = G1_1.get_convolve(filter);
      L4_1 = G1_1 - smooth4_1;
      L4_1.save("image1_L4.jpg", -1, 6);

      CImg<double> L5_1(307, 307, 1, 3);
      CImg<double> smooth5_1 = G0_1.get_convolve(filter);
      L5_1 = G0_1 - smooth5_1;
      L5_1.save("image1_L5.jpg", -1, 6);
      //CImg<double> resized_G5_1;
      /*
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
      
      CImg<double> L0_1_test;
      CImg<double> L0_2_test;
      CImg<double> L0_3_test;

      //CImg<double> smooth_image1 = image1.get_convolve(filter);
      //L0_1_test = image1 - smooth_image1;
      //L0_1_test.save("image1_hopefully_1.jpg", -1, 6); 
      //
      //

      CImg<double> smooth_image1 = G3_1.get_convolve(filter);
      L0_1_test = G3_1 - smooth_image1;
      L0_1_test.save("image1_hopefully.jpg", -1, 6);

      CImg<double> smooth_image2 = G2_1.get_convolve(filter);
      L0_2_test = G2_1 - smooth_image2;
      L0_2_test.save("image1_hopefully_2.jpg", -1, 6);

      CImg<double> smooth_image3 = G1_1.get_convolve(filter);
      L0_3_test = G1_1 - smooth_image3;
      L0_3_test.save("image1_hopefully_3.jpg", -1, 6);
*/
       // IMAGE 2 PYRAMID
      CImg<double> L0_2(10, 10, 1, 3);
      L0_2 = G5_2;
      L0_2.save("image2_L0.jpg", -1, 6);
 
      CImg<double> L1_2(20, 20, 1, 3);
      CImg<double> smooth1_2 = G4_2.get_convolve(filter);
      L1_2 = G4_2 - smooth1_2;
      L1_2.save("image2_L1.jpg", -1, 6);

      CImg<double> L2_2(39, 39, 1, 3);
      CImg<double> smooth2_2 = G3_2.get_convolve(filter);
      L2_2 = G3_2 - smooth2_2;
      L2_2.save("image2_L2.jpg", -1, 6);

      CImg<double> L3_2(77, 77, 1, 3);
      CImg<double> smooth3_2 = G2_2.get_convolve(filter);
      L3_2 = G2_2 - smooth3_2;
      L3_2.save("image2_L3.jpg", -1, 6);

      CImg<double> L4_2(153, 153, 1, 3);
      CImg<double> smooth4_2 = G1_2.get_convolve(filter);
      L4_2 = G1_2 - smooth4_2;
      L4_2.save("image2_L4.jpg", -1, 6);

      CImg<double> L5_2(307, 307, 1, 3);
      CImg<double> smooth5_2 = G0_2.get_convolve(filter);
      L5_2 = G0_2 - smooth5_2;
      L5_2.save("image2_L5.jpg", -1, 6);

      // 2. Build Gaussian for the mask    
       CImg<double> G0_mask = mask;
       CImg<double> G1_mask;
       G1_mask = G0_mask.get_convolve(filter);
       G1_mask.resize(153, 153);
       G1_mask.save("mask_G1.jpg", -1, 6);

       CImg<double> G2_mask;
       G2_mask = G1_mask.get_convolve(filter);
       G2_mask.resize(77, 77);
       G2_mask.save("mask_G2.jpg", -1, 6);

       CImg<double> G3_mask;
       G3_mask = G2_mask.get_convolve(filter);
       G3_mask.resize(39, 39);
       G3_mask.save("mask_G3.jpg", -1, 6);

       CImg<double> G4_mask;
       G4_mask = G3_mask.get_convolve(filter);
       G4_mask.resize(20, 20);
       G4_mask.save("mask_G4.jpg", -1, 6);

       CImg<double> G5_mask;
       G5_mask = G4_mask.get_convolve(filter);
       G5_mask.resize(10, 10);
       G5_mask.save("mask_G5.jpg", -1, 6);

      // 3. Blended Laplacian pyramid
      //  - sum of images in corresponding level of the two images
      //  - formula used in given in assignment
/*
      if (LB_0.width() == G0_mask.width() == L5_1.width() == L5_2.width() 
       && LB_0.height() == G0_mask.height() == L5_1.height() == L5_2.height()) {
         cout << "same dimensions!" << endl;
      }
      else {
         cout << "mismatched dimensions" << endl;
         cout << "LB_0 width: " << LB_0.width() << endl;
         cout << "Mask_0 width: " << G0_mask.width() << endl;
         cout << "L5_1 width: " << L5_1.width() << endl;
         cout << "L5_2 width: " << L5_2.width() << endl;
         cout << "LB_0 height: " << LB_0.height() << endl;
         cout << "Mask_0 height: " << G0_mask.height() << endl;
         cout << "L5_1 height: " << L5_1.height() << endl;
         cout << "L5_2 height: " << L5_2.height() << endl;
      }
      for (int i = 0; i < LB_0.width(); i++) {
        for (int j = 0; j < LB_0.height(); j++) {
          LB_0(i, j) = ( G0_mask(i, j) * L5_1(i, j) ) + ( (1 - G0_mask(i, j)) * L5_2(i, j) );
          cout << "Pixel val at (" << i << ", " << j << ") is " << LB_0(i, j) << endl;
          cout << "Computation: " << ( G0_mask(i, j) * L5_1(i, j) ) + ( (1 - G0_mask(i, j)) * L5_2(i, j) ) << endl;
          LB_0.save("LB_0.jpg", -1, 6);
        }
      }
*/
      CImg<double> LB_0(307, 307, 1, 3);
      cimg_forXY(LB_0, x, y) {
        LB_0(x, y) = ( G0_mask(x, y) * L5_1(x, y) ) + ( (1 - G0_mask(x, y)) * L5_2(x, y) );
      }
      LB_0.save("LB_0.jpg", -1, 6);

      CImg<double> LB_1(153, 153, 1, 3);
      cimg_forXY(LB_1, x, y) {
        LB_1(x, y) = ( G1_mask(x, y) * L4_1(x, y) ) + ( (1 - G1_mask(x, y)) * L4_2(x, y) );
      }
      LB_1.save("LB_1.jpg", -1, 6);

      CImg<double> LB_2(77, 77, 1, 3);
      cimg_forXY(LB_2, x, y) {
        LB_2(x, y) = ( G2_mask(x, y) * L3_1(x, y) ) + ( (1 - G2_mask(x, y)) * L3_2(x, y) );
      }
      LB_2.save("LB_2.jpg", -1, 6);

      CImg<double> LB_3(39, 39, 1, 3);
      cimg_forXY(LB_3, x, y) {
        LB_3(x, y) = ( G3_mask(x, y) * L2_1(x, y) ) + ( (1 - G3_mask(x, y)) * L2_2(x, y) );
      }
      LB_3.save("LB_3.jpg", -1, 6);
 
      CImg<double> LB_4(20, 20, 1, 3);
      cimg_forXY(LB_4, x, y) {
        LB_4(x, y) = ( G4_mask(x, y) * L1_1(x, y) ) + ( (1 - G4_mask(x, y)) * L1_2(x, y) );
      }
      LB_4.save("LB_4.jpg", -1, 6);

      CImg<double> LB_5(10, 10, 1, 3);
      cimg_forXY(LB_5, x, y) {
        LB_5(x, y) = ( G5_mask(x, y) * L0_1(x, y) ) + ( (1 - G5_mask(x, y)) * L0_2(x, y) );
      }
      LB_5.save("LB_5.jpg", -1, 6);

      // 4. Form the blended image 
      CImg<double> resized_LB_5(20, 20);
      CImg<double> final_L4(20, 20);
      CImg<double> step1(20, 20);
      resized_LB_5 = LB_5.get_resize_doubleXY();
      cout << "Dimensions: Width " << resized_LB_5.width() << " and Height: " << resized_LB_5.height() << endl;
      final_L4 = resized_LB_5.get_convolve(filter);
      cimg_forXY(step1, x, y) {
        step1(x, y) = LB_4(x, y) + final_L4(x, y);
      } 

      CImg<double> resized_LB_4(40, 40);
      CImg<double> final_L3(39, 39);
      CImg<double> step2(39, 39);
      resized_LB_4 = LB_4.get_resize_doubleXY();
      resized_LB_4.resize(39, 39);
      cout << "Dimensions: Width " << resized_LB_4.width() << " and Height: " << resized_LB_4.height() << endl;
      final_L3 = resized_LB_4.get_convolve(filter);
      cimg_forXY(step2, x, y) {
        step2(x, y) = LB_3(x, y) + final_L3(x, y);
      }

      CImg<double> resized_LB_3(78, 78);
      CImg<double> final_L2(77, 77);
      CImg<double> step3(77, 77);
      resized_LB_3 = LB_4.get_resize_doubleXY();
      resized_LB_3.resize(77, 77);
      cout << "Dimensions: Width " << resized_LB_3.width() << " and Height: " << resized_LB_3.height() << endl;
      final_L2 = resized_LB_3.get_convolve(filter);
      cimg_forXY(step3, x, y) {
        step3(x, y) = LB_2(x, y) + final_L2(x, y);
      }

      CImg<double> resized_LB_2(154, 154);
      CImg<double> final_L1(153, 153);
      CImg<double> step4(153, 153);
      resized_LB_2 = LB_3.get_resize_doubleXY();
      resized_LB_2.resize(153, 153);
      cout << "Dimensions: Width " << resized_LB_2.width() << " and Height: " << resized_LB_2.height() << endl;
      final_L1 = resized_LB_2.get_convolve(filter);
      cimg_forXY(step2, x, y) {
        step2(x, y) = LB_1(x, y) + final_L1(x, y);
      }

      CImg<double> resized_LB_1(306, 306);
      CImg<double> final_L0(307, 307);
      CImg<double> step5(307, 307);
      resized_LB_1 = LB_4.get_resize_doubleXY();
      resized_LB_1.resize(307, 307);
      cout << "Dimensions: Width " << resized_LB_1.width() << " and Height: " << resized_LB_1.height() << endl;
      final_L0 = resized_LB_1.get_convolve(filter);
      cimg_forXY(step5, x, y) {
        step5(x, y) = LB_0(x, y) + final_L0(x, y);
      }
      step5.save("final_blended.jpg", -1, 6);
        
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
