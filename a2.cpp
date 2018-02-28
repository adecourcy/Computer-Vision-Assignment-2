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
#include <cmath>


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



vector<double> transform_pixel(const vector<double> &transformation_matrix,
                               int x,
                               int y,
                               double z)
{
  vector<double> coordinates(2);
  double new_z;

  new_z = ((double) x) * transformation_matrix[6] + 
          ((double) y) * transformation_matrix[7] +
          z * transformation_matrix[8];

  coordinates[0] = (((double) x) * transformation_matrix[0] + 
                    ((double) y) * transformation_matrix[1] +
                    z * transformation_matrix[2]) / new_z;

  coordinates[1] = (((double) x) * transformation_matrix[3] + 
                    ((double) y) * transformation_matrix[4] +
                    z * transformation_matrix[5]) / new_z;



  return coordinates;
}



vector<double> get_inverse_matrix(const vector<double> &original_matrix)
{

  double A = 
      (original_matrix[4] * original_matrix[8] - 
       original_matrix[5] * original_matrix[7]);

  double B = 
      (0 - (original_matrix[3] * original_matrix[8] -
            original_matrix[5] * original_matrix[6]));

  double C =
      (original_matrix[3] * original_matrix[7] -
       original_matrix[4] * original_matrix[6]);

  double D =
      (0 - (original_matrix[1] * original_matrix[8] -
            original_matrix[2] * original_matrix[7]));

  double E = 
      (original_matrix[0] * original_matrix[8] - 
       original_matrix[2] * original_matrix[6]);

  double F = 
      (0 - (original_matrix[0] * original_matrix[7] -
            original_matrix[1] * original_matrix[6]));

  double G = 
      (original_matrix[1] * original_matrix[5] - 
       original_matrix[2] * original_matrix[4]);

  double H = 
      (0 - (original_matrix[0] * original_matrix[5] -
            original_matrix[2] * original_matrix[3]));

  double I =
      (original_matrix[0] * original_matrix[4] - 
       original_matrix[1] * original_matrix[3]);

  vector<double> inverse_matrix(9);
  double determinant = 
    original_matrix[0] * A +
    original_matrix[1] * B +
    original_matrix[2] * C;

  if (determinant == 0) {
    printf("Matrix is not invertible! Exiting...\n");
    exit(0);
  }


  inverse_matrix[0] = A / determinant;
  inverse_matrix[1] = D / determinant;
  inverse_matrix[2] = G / determinant;

  inverse_matrix[3] = B / determinant;
  inverse_matrix[4] = E / determinant;
  inverse_matrix[5] = H / determinant;

  inverse_matrix[6] = C / determinant;
  inverse_matrix[7] = F / determinant;
  inverse_matrix[8] = I / determinant;

  return inverse_matrix;

}



CImg<double> transform_image(const CImg<double> &original_image,
                             const vector<double> &transformation_matrix,
                             int width,
                             int height)
{
  vector<double> cmpr_coords(2);

  CImg<double> transformed_image(width,
                                 height,
                                 1,
                                 3);
  transformed_image.fill(0.0);

  /*
  for (int x = 0; x < original_image.width(); x++) {
    for (int y = 0; y < original_image.height(); y++) {
      cmpr_coords = 
        transform_pixel(transformation_matrix,
                        x,
                        y,
                        1);

      cmpr_coords[0] = round(cmpr_coords[0]);
      cmpr_coords[1] = round(cmpr_coords[1]);
      if (cmpr_coords[0] < 0.0 ||
          cmpr_coords[0] >= (float) transformed_image.width() ||
          cmpr_coords[1] < 0.0 ||
          cmpr_coords[1] >= (float) transformed_image.height()) {

          continue;
      } else {
        for (int color = 0; color < 2; color++) {
          transformed_image(cmpr_coords[0],
                            cmpr_coords[1],
                            0,
                            color) = 
            original_image(x, y, 0, color);
        }
      }
    }
  }
  */
  
  

  
  vector<double> inverse_matrix = 
      get_inverse_matrix(transformation_matrix);

  for (int x = 0; x < transformed_image.width(); x++) {
    for (int y = 0; y < transformed_image.height(); y++) {
      cmpr_coords = 
        transform_pixel(inverse_matrix,
                        x,
                        y,
                        1);

      cmpr_coords[0] = round(cmpr_coords[0]);
      cmpr_coords[1] = round(cmpr_coords[1]);
      if (cmpr_coords[0] < 0.0 ||
          cmpr_coords[0] >= (float) original_image.width() ||
          cmpr_coords[1] < 0.0 ||
          cmpr_coords[1] >= (float) original_image.height()) {

          continue;
      } else {
        for (int color = 0; color < 3; color++) {
          transformed_image(x,
                            y,
                            0,
                            color) = 
            original_image(cmpr_coords[0], cmpr_coords[1], 0, color);
        }
      }
    }
  }  


  return transformed_image;


}


vector<double> get_transformation_matrix(vector<double> from_1,
                                         vector<double> to_1,
                                         vector<double> from_2,
                                         vector<double> to_2,
                                         vector<double> from_3,
                                         vector<double> to_3,
                                         vector<double> from_4,
                                         vector<double> to_4)
{

  CImg<double> P(8, 8, 1, 1);
  P.fill(0.0);

  P(0,0) = from_1[0];
  P(1,0) = from_1[1];
  P(2,0) = 1;
  P(6,0) = 0 - (from_1[0] * to_1[0]);
  P(7,0) = 0 - (from_1[1] * to_1[0]);

  P(3,1) = from_1[0];
  P(4,1) = from_1[1];
  P(5,1) = 1;
  P(6,1) = 0 - (from_1[0] * to_1[1]);
  P(7,1) = 0 - (from_1[1] * to_1[1]);



  P(0,2) = from_2[0];
  P(1,2) = from_2[1];
  P(2,2) = 1;
  P(6,2) = 0 - (from_2[0] * to_2[0]);
  P(7,2) = 0 - (from_2[1] * to_2[0]);

  P(3,3) = from_2[0];
  P(4,3) = from_2[1];
  P(5,3) = 1;
  P(6,3) = 0 - (from_2[0] * to_2[1]);
  P(7,3) = 0 - (from_2[1] * to_2[1]);



  P(0,4) = from_3[0];
  P(1,4) = from_3[1];
  P(2,4) = 1;
  P(6,4) = 0 - (from_3[0] * to_3[0]);
  P(7,4) = 0 - (from_3[1] * to_3[0]);

  P(3,5) = from_3[0];
  P(4,5) = from_3[1];
  P(5,5) = 1;
  P(6,5) = 0 - (from_3[0] * to_3[1]);
  P(7,5) = 0 - (from_3[1] * to_3[1]);



  P(0,6) = from_4[0];
  P(1,6) = from_4[1];
  P(2,6) = 1;
  P(6,6) = 0 - (from_4[0] * to_4[0]);
  P(7,6) = 0 - (from_4[1] * to_4[0]);

  P(3,7) = from_4[0];
  P(4,7) = from_4[1];
  P(5,7) = 1;
  P(6,7) = 0 - (from_4[0] * to_4[1]);
  P(7,7) = 0 - (from_4[1] * to_4[1]);




  CImg<double> A(1,8,1,1);
  A(0) = to_1[0];
  A(1) = to_1[1];
  A(2) = to_2[0];
  A(3) = to_2[1];
  A(4) = to_3[0];
  A(5) = to_3[1];
  A(6) = to_4[0];
  A(7) = to_4[1];

  

  vector<double> transformation_matrix(9);

  A.solve(P);

  for (int i = 0; i < 8; i++) {
    transformation_matrix[i] = A(i);
  }

  transformation_matrix[8] = 1.0;

  return transformation_matrix;
}



// Transform the book according to the PDF description
CImg<double> transform_book(CImg<double> input_image)
{
  vector<double> from_1(2);
  vector<double> from_2(2);
  vector<double> from_3(2);
  vector<double> from_4(2);

  vector<double> to_1(2);
  vector<double> to_2(2);
  vector<double> to_3(2);
  vector<double> to_4(2);

  to_1[0] = 318.0;
  to_1[1] = 256.0;

  to_2[0] = 534.0;
  to_2[1] = 372.0;

  to_3[0] = 316.0;
  to_3[1] = 670.0;

  to_4[0] = 73.0;
  to_4[1] = 473.0;

  from_1[0] = 141.0;
  from_1[1] = 131.0;

  from_2[0] = 480.0;
  from_2[1] = 159.0;

  from_3[0] = 493.0;
  from_3[1] = 630.0;

  from_4[0] = 64.0;
  from_4[1] = 601.0;

  vector<double> H = get_transformation_matrix(from_1,
                                               to_1,
                                               from_2,
                                               to_2,
                                               from_3,
                                               to_3,
                                               from_4,
                                               to_4);

  CImg<double> transformed_image = 
      transform_image(input_image,
                      H,
                      input_image.width(),
                      input_image.height());

  return transformed_image;

}



// Given an image, overlay that image on to the billboards
void imageOnBillboards(CImg<double> input_image)
{
  CImg<double> billboard_1;
  CImg<double> billboard_2;
  CImg<double> billboard_3;
  billboard_1.load("./images/part1/billboard1.jpg");
  billboard_2.load("./images/part1/billboard2.png");
  billboard_3.load("./images/part1/billboard3.jpg");

  vector<double> from_1(2);
  vector<double> from_2(2);
  vector<double> from_3(2);
  vector<double> from_4(2);

  vector<double> to_1(2); // bottom left
  vector<double> to_2(2); // top left
  vector<double> to_3(2); // bottom right
  vector<double> to_4(2); // top right

  // Input image from coordinates

  from_1[0] = 0.0;
  from_1[1] = input_image.height() - 1;

  from_2[0] = 0.0;
  from_2[1] = 0.0;

  from_3[0] = input_image.width() - 1;
  from_3[1] = input_image.height() - 1;

  from_4[0] = input_image.width() - 1;
  from_4[1] = 0.0;

  // billboard 1 to coordinates

  to_1[0] = 101.0;
  to_1[1] = 203.0;

  to_2[0] = 101.0;
  to_2[1] = 60.0;

  to_3[0] = 531.0;
  to_3[1] = 203.0;

  to_4[0] = 531.0;
  to_4[1] = 60.0;

  vector<double> bill_1_transform = get_transformation_matrix(from_1,
                                                              to_1,
                                                              from_2,
                                                              to_2,
                                                              from_3,
                                                              to_3,
                                                              from_4,
                                                              to_4);

  CImg <double> bill_1_overlay = transform_image(input_image,
                                                 bill_1_transform,
                                                 billboard_1.width(),
                                                 billboard_1.height());

  for (int row = 0; row < bill_1_overlay.width(); row++) {
    for (int col = 0; col < bill_1_overlay.height(); col++) {
      if (bill_1_overlay(row, col, 0, 0) != 0.0 &&
          bill_1_overlay(row, col, 0, 1) != 0.0 &&
          bill_1_overlay(row, col, 0, 2) != 0.0) {
        for (int color = 0; color < 3; color++) {
          billboard_1(row, col, 0, color) = bill_1_overlay(row, col, 0, color);
        }
      }
    }
  }

  billboard_1.save("overlay_1.jpg", -1, 6);

  // billboard 2 to coordinates

  to_1[0] = 148.0;
  to_1[1] = 623.0;

  to_2[0] = 176.0;
  to_2[1] = 53.0;

  to_3[0] = 1125.0;
  to_3[1] = 701.0;

  to_4[0] = 1106.0;
  to_4[1] = 260.0;

  vector<double> bill_2_transform = get_transformation_matrix(from_1,
                                                              to_1,
                                                              from_2,
                                                              to_2,
                                                              from_3,
                                                              to_3,
                                                              from_4,
                                                              to_4);

  CImg <double> bill_2_overlay = transform_image(input_image,
                                                 bill_2_transform,
                                                 billboard_2.width(),
                                                 billboard_2.height());

  for (int row = 0; row < bill_2_overlay.width(); row++) {
    for (int col = 0; col < bill_2_overlay.height(); col++) {
      if (bill_2_overlay(row, col, 0, 0) != 0.0 &&
          bill_2_overlay(row, col, 0, 1) != 0.0 &&
          bill_2_overlay(row, col, 0, 2) != 0.0) {
        for (int color = 0; color < 3; color++) {
          billboard_2(row, col, 0, color) = bill_2_overlay(row, col, 0, color);
        }
      }
    }
  }

  billboard_2.save("overlay_2.jpg", -1, 6);


  // billboard 3 to coordinates

  to_1[0] = 610.0;
  to_1[1] = 607.0;

  to_2[0] = 617.0;
  to_2[1] = 288.0;

  to_3[0] = 1260.0;
  to_3[1] = 604.0;

  to_4[0] = 1259.0;
  to_4[1] = 261.0;

  vector<double> bill_3_transform = get_transformation_matrix(from_1,
                                                              to_1,
                                                              from_2,
                                                              to_2,
                                                              from_3,
                                                              to_3,
                                                              from_4,
                                                              to_4);

  CImg <double> bill_3_overlay = transform_image(input_image,
                                                 bill_3_transform,
                                                 billboard_3.width(),
                                                 billboard_3.height());

  for (int row = 0; row < bill_3_overlay.width(); row++) {
    for (int col = 0; col < bill_3_overlay.height(); col++) {
      if (bill_3_overlay(row, col, 0, 0) != 0.0 &&
          bill_3_overlay(row, col, 0, 1) != 0.0 &&
          bill_3_overlay(row, col, 0, 2) != 0.0) {
        for (int color = 0; color < 3; color++) {
          billboard_3(row, col, 0, color) = bill_3_overlay(row, col, 0, color);
        }
      }
    }
  }

  billboard_3.save("overlay_3.jpg", -1, 6);


}



// Find the euclidean distance between 2 sift descriptors
double find_distance(SiftDescriptor original_descriptor,
                     SiftDescriptor match_descriptor)
{
  double distance = 0.0;

  for (int element = 0; element < 128; element++) {
    distance += 
      pow((original_descriptor.descriptor[element] -
           match_descriptor.descriptor[element]), 2.0);
  }

  distance = sqrt(distance);

  return distance;
}


/*
  If there is a match above the threshold, return the index of that element.
  Otherwise, return -1.
*/
int find_match(SiftDescriptor original_descriptor,
               vector<SiftDescriptor> match_descriptors,
               double threshold)
{
  double best_distance = 100000000.0;
  double second_best_distance = 100000000.0;
  double current_distance;

  int best_descriptor_element;

  for (int element = 0; element < match_descriptors.size(); element++) {
    current_distance =
        find_distance(original_descriptor, match_descriptors[element]);

    if (current_distance < best_distance) {

      best_descriptor_element = element;
      second_best_distance = best_distance;
      best_distance = current_distance;

    } else if (current_distance < second_best_distance) {
      second_best_distance = current_distance;
    }
  }

  //printf("best: %f, second best: %f\n", best_distance, second_best_distance);

  if ((best_distance / second_best_distance) > threshold) {
    return -1;
  } else {
    return best_descriptor_element;
  }

}


/*
  Return a vector containing all matches. All even elements of the vector
  correspond to the indices of the original_descriptors vector. All odd
  elements correspond to the indices of the match_descriptors vector.

  Given an even element, the match  is the next odd element. For example,
  matches[0] corresponds to matches[1]
*/
vector<int> find_all_matches(vector<SiftDescriptor> original_descriptors,
                             vector<SiftDescriptor> match_descriptors,
                             double threshold)
{
  int match_index;
  vector<int> matches;

  for (int element = 0; element < original_descriptors.size(); element++) {

    match_index = find_match(original_descriptors[element],
                             match_descriptors,
                             threshold);

    if (match_index == -1) {
      continue;
    } else {
      matches.push_back(element);
      matches.push_back(match_index);
    }
  }

  return matches;
}



CImg<double> combine_images(CImg<double> original_image,
                            CImg<double> new_image)
{
  int larger_height;

  if (original_image.height() > new_image.height()) {
    larger_height = original_image.height();
  } else {
    larger_height = new_image.height();
  }

  CImg<double> combined_image(original_image.width() + new_image.width(),
                              larger_height,
                              1,
                              3);
  combined_image.fill(0.0);

  for (int x = 0; x < original_image.width(); x++) {
    for (int y = 0; y < original_image.height(); y++) {
      for (int color = 0; color < 3; color++) {
        combined_image(x, y, 0, color) = original_image(x, y, 0, color);
      }
    }
  }

  for (int x = original_image.width();
       x < original_image.width() + new_image.width();
       x++) {
    for (int y = 0; y < new_image.height(); y++) {
      for (int color = 0; color < 3; color++) {
        combined_image(x, y, 0, color) = new_image(x, y, 0, color);
      }
    }
  }

  return combined_image;
}



CImg<double> draw_lines(CImg<double> combined_image,
                        vector<SiftDescriptor> image1_descriptors,
                        vector<SiftDescriptor> image2_descriptors,
                        vector<int> all_matches,
                        int added_width)
{

  int x_1, x_2, y_1, y_2;

  for (int element = 0; element < all_matches.size(); element += 2) {
    x_1 = image1_descriptors[all_matches[element]].col;
    y_1 = image1_descriptors[all_matches[element]].row;
    x_2 = image2_descriptors[all_matches[element+1]].col + added_width;
    y_2 = image2_descriptors[all_matches[element+1]].row;

    const unsigned char color[] = { 0,255,0 };
    combined_image.draw_line(x_1, y_1, x_2, y_2, color);
  }

  return combined_image;

}



int main(int argc, char **argv)
{ 
  /*
    CImg <double> input_image1;
    input_image1.load("book1.jpg");

    CImg <double> input_image2;
    input_image2.load("book2.jpg");

    CImg <double> combined_image = 
        combine_images(input_image1, input_image2);

    combined_image.save("combined_image.png");
    exit(0);
  */

  try {

    string part = argv[1];
    
    if(part == "part1"){
      if (argc != 3) {
        printf("Program Usage ./a2 part1 [input_image]\n");
        exit(0);
      }

      CImg<double> input_image;
      input_image.load(argv[2]);
      imageOnBillboards(input_image);
    }	
    else if(part == "part2"){
      double threshold;

      if ((argc != 4) && (argc != 5)) {
        printf("Program Usage ./a2 part1 [input_image1] [input_image2] [threshold]\n");
        printf("The threshold parameter is optional\n");
        exit(0);
      }

      if (argc == 5) {
        threshold = strtod(argv[4], NULL);
      } else {
        threshold = 0.5;
      }
      CImg<double> input_image1;
      input_image1.load(argv[2]);
      CImg<double> input_image2;
      input_image2.load(argv[3]);

      Sift sift;

      vector<SiftDescriptor> image1_descriptors =
                        sift.compute_sift(input_image1);
      vector<SiftDescriptor> image2_descriptors =
                        sift.compute_sift(input_image2);

      printf("Finding matches\n");
      vector<int> all_matches =
            find_all_matches(image1_descriptors,
                             image2_descriptors,
                             threshold);

      printf("Finding Images\n");
      CImg<double> combined_image = 
          draw_lines(combine_images(input_image1, input_image2),
                     image1_descriptors,
                     image2_descriptors,
                     all_matches,
                     input_image1.width());


      combined_image.save("sift_lines.png");

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
