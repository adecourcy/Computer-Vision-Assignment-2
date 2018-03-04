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



vector<CImg<double> > get_gaussian_pyramid(CImg<double> image,
                                           CImg<double> filter,
                                           int pyramid_size)
{

  vector<CImg<double> > gaussian_pyramid(pyramid_size);
  gaussian_pyramid[0] = image;

  for (int i = 1; i < pyramid_size; i++) {

   CImg<double> G_prev = gaussian_pyramid[i - 1];
   int G_prev_rows = G_prev.width();
   int G_prev_cols = G_prev.height();

   CImg<double> G_curr = G_prev.get_convolve(filter);
   // truncating to half size. Maybe we should round??
   G_curr.resize(G_prev_rows/2, G_prev_cols/2, 1, 3); 
   gaussian_pyramid[i] = G_curr;
  }

  return gaussian_pyramid;
}


vector<CImg<double> > get_laplacian(vector<CImg<double> > gaussian_pyramid,
                                    CImg<double> filter,
                                    int pyramid_size)
{

  vector<CImg<double> > laplacian_pyramid(gaussian_pyramid.size());
  laplacian_pyramid[0] = 
      gaussian_pyramid[gaussian_pyramid.size() - 1];

  int L_counter = 1;
  for (int i = gaussian_pyramid.size() - 2; i >= 0; i--) {
   CImg<double> G_curr = gaussian_pyramid[i];
   CImg<double> G_smooth = G_curr.get_convolve(filter);
   
   // subtract the smoothed version from the normal to get the Laplacian
   laplacian_pyramid[L_counter] = G_curr - G_smooth;
   L_counter += 1;
  }

  return laplacian_pyramid;

}



vector<CImg<double> > get_laplacian_blend(vector<CImg<double> > laplacian_pyramid_1,
                                          vector<CImg<double> > laplacian_pyramid_2,
                                          vector<CImg<double> > mask_pyramid)
{

  vector<CImg<double> > LB(laplacian_pyramid_1.size()); 
  int start_size = laplacian_pyramid_1[laplacian_pyramid_1.size() - 1].width();

  int L_counter = laplacian_pyramid_1.size() - 1;
  for (int i = 0; i < laplacian_pyramid_1.size(); i++) {
    CImg<double> LB_curr(start_size, start_size, 1, 3);
    CImg<double> mask_curr = mask_pyramid[i];
    CImg<double> img1_L_curr = laplacian_pyramid_1[L_counter];
    CImg<double> img2_L_curr = laplacian_pyramid_2[L_counter];
     
    cimg_forXYC(LB_curr, x, y, c) {
      LB_curr(x, y, c) =
          ( (mask_curr(x, y, 1) / 255.0) * img1_L_curr(x, y, c) ) +
          ( (1 - (mask_curr(x, y, 1) / 255.0)) * img2_L_curr(x, y, c) ); 
    }

    LB[i] = LB_curr;
    L_counter -= 1;
    start_size /= 2;
  }

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

      // normalize mask values to be between 0 and 1 instead of 0 and 255
      mask.get_normalize(0,255);

      // 1. For each img, compute the Gaussian pyramid and the Laplacian pyramid
      // Gaussian pyramids
      //int rounds = 5;   // this is the number of levels in the Gaussian pyramid

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
       
       vector<CImg<double> > img1_Gpyr(6);
       img1_Gpyr[0] = image1;

       for (int i = 1; i < 6; i++) {

         CImg<double> G_prev = img1_Gpyr[i - 1];
         int G_prev_rows = G_prev.width();
         int G_prev_cols = G_prev.height();

         CImg<double> G_curr = G_prev.get_convolve(filter);
         G_curr.resize(G_prev_rows/2, G_prev_cols/2, 1, 3); 
         img1_Gpyr[i] = G_curr;
         G_curr.save("image1_G" + c_str(i) + ".jpg", -1, 6);

       }

       //CImg<double> G0_1 = image1;
       //G0_1.save("image1_G0.jpg", -1, 6);

       // IMAGE 2 PYRAMID
       
       vector<CImg<double> > img2_Gpyr(6);
       img2_Gpyr[0] = image2;
       for (int i = 1; i < 6; i++) {
         CImg<double> G_prev = img2_Gpyr[i - 1];
         int G_prev_rows = G_prev.width();
         int G_prev_cols = G_prev.height();

         CImg<double> G_curr = G_prev.get_convolve(filter);
         G_curr.resize(G_prev_rows/2, G_prev_cols/2, 1, 3);
         img2_Gpyr[i] = G_curr;
         G_curr.save("image2_G" + c_str(i) + ".jpg", -1, 6);
       }


      // Now the Laplacian pyramids
      //
      
      // IMAGE 1 PYRAMID 
      //

       vector<CImg<double> > img1_Lpyr(6);
       img1_Lpyr[0] = img1_Gpyr[5];

       int L_counter = 1;
       for (int i = 4; i >= 0; i--) {
         CImg<double> G_curr = img1_Gpyr[i];
         CImg<double> G_smooth = G_curr.get_convolve(filter);
         
         // subtract the smoothed version from the normal to get the Laplacian
         img1_Lpyr[L_counter] = G_curr - G_smooth;
         img1_Lpyr[L_counter].save("image1_L" + c_str(L_counter) + ".jpg", -1, 6);
         L_counter += 1;
       }
       

       // IMAGE 2 PYRAMID


      vector<CImg<double> > img2_Lpyr(6);
      img2_Lpyr[0] = img2_Gpyr[5];
      int L_counter = 1;
      for (int i = 4; i >= 0; i--) {
        CImg<double> G_curr = img2_Gpyr[i];
        CImg<double> G_smooth = G_curr.get_convolve(filter);

        // subtract the smoothed version from the normal to get the Laplacian
        img2_Lpyr[L_counter] = G_curr - G_smooth;
        img2_Lpyr[L_counter].save("image2_L" + c_str(L_counter) + ".jpg", -1, 6);
        L_counter += 1;
       }

      // 2. Build Gaussian for the mask
      //
      
       vector<CImg<double> > mask_Gpyr(6);
       mask_Gpyr[0] = mask;

       for (int i = 1; i < 6; i++) {
         CImg<double> G_prev = mask_Gpyr[i - 1];
         int G_prev_rows = G_prev.width();
         int G_prev_cols = G_prev.height();

         CImg<double> G_curr = G_prev.get_convolve(filter);
         G_curr.resize(G_prev_rows/2, G_prev_cols/2, 1, 1);
         mask_Gpyr[i] = G_curr;
         //G_curr.save("image1_G" + c_str(i) + ".jpg", -1, 6);
       }


vector<CImg<double> > LB(6); 
int start_size = 307;
int L_counter = 5;
for (int i = 0; i < 6; i++) {
  CImg<double> LB_curr(start_size, start_size, 1, 3);
  CImg<double> mask_curr = mask_Gpyr[i];
  CImg<double> img1_L_curr = img1_Lpyr[L_counter];
  CImg<double> img2_L_curr = img2_Lpyr[L_counter];
   
  cimg_forXYC(LB_curr, x, y, c) {
    LB_curr(x, y, c) = ( mask_curr(x, y, c) * img1_L_curr(x, y, c) ) + ( (1 - mask_curr(x, y, c)) * img2_L_curr(x, y, c) ); 
  }
  LB[i] = LB_curr;
  L_counter -= 1;
  start_size /= 2;
}


      // 4. Form the blended image 
      CImg<double> final_L4(20, 20, 1, 3);
      CImg<double> step1(20, 20, 1, 3);
      LB_5.resize(20, 20, 1, 3);
      final_L4 = LB_5.get_convolve(filter);
      cimg_forXYC(step1, x, y, c) {
        step1(x, y, c) = LB_4(x, y, c) + final_L4(x, y, c);
      } 

      //CImg<double> resized_step1(40, 40, 1, 3);
      CImg<double> final_L3(39, 39, 1, 3);
      CImg<double> step2(39, 39, 1, 3);
      step1.resize(39, 39, 1, 3);
      final_L3 = step1.get_convolve(filter);
      cimg_forXYC(step2, x, y, c) {
        step2(x, y, c) = LB_3(x, y, c) + final_L3(x, y, c);
      }   
      
      CImg<double> final_L2(77, 77, 1, 3);
      CImg<double> step3(77, 77, 1, 3);
      step2.resize(77, 77, 1, 3);
      final_L2 = step2.get_convolve(filter);
      cimg_forXYC(step3, x, y, c) {
        step3(x, y, c) = LB_2(x, y, c) + final_L2(x, y, c);
      }
      
      //CImg<double> resized_step3(154, 154, 1, 3);
      CImg<double> final_L1(153, 153, 1, 3);
      CImg<double> step4(153, 153, 1, 3);
      step3.resize(153, 153, 1, 3);
      final_L1 = step3.get_convolve(filter);
      cimg_forXYC(step4, x, y, c) {
        step4(x, y, c) = LB_1(x, y, c) + final_L1(x, y, c);
      }

      //CImg<double> resized_step4(306, 306, 1, 3);
      CImg<double> final_L0(307, 307, 1, 3);
      CImg<double> step5(307, 307, 1, 3);
      step4.resize(307, 307, 1, 3);
      final_L0 = step4.get_convolve(filter);
      cimg_forXYC(step5, x, y, c) {
        step5(x, y, c) = LB_0(x, y, c) + final_L0(x, y, c);
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
