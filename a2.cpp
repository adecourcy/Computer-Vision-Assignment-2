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
#include <algorithm>


//Use the cimg namespace to access functions easily
using namespace cimg_library;
using namespace std;

void draw_descriptor_image(CImg<double> image,
                           const vector<SiftDescriptor> descriptors, 
                           const char *filename)
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
double find_distance(const SiftDescriptor &original_descriptor,
                     const SiftDescriptor &match_descriptor)
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
int find_match(const SiftDescriptor &original_descriptor,
               const vector<SiftDescriptor> &match_descriptors,
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
vector<int> find_all_matches(const vector<SiftDescriptor> &original_descriptors,
                             const vector<SiftDescriptor> &match_descriptors,
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



CImg<double> combine_images(const CImg<double> &original_image,
                            const CImg<double> &new_image)
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
                        const vector<SiftDescriptor> &image1_descriptors,
                        const vector<SiftDescriptor> &image2_descriptors,
                        const vector<int> &all_matches,
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



// Get unique set of numbers
vector<int> get_random_numbers(int num_numbers,
                               vector<int> random_generator)
{
  vector<int> random_numbers(4);
  random_shuffle(random_generator.begin(), random_generator.end());

  for (int i = 0; i < num_numbers; i++) {
    random_numbers[i] = random_generator[i];
  }
  return random_numbers;

}



vector<double> get_random_homography(const vector<SiftDescriptor> &image1_descriptors,
                                     const vector<SiftDescriptor> &image2_descriptors,
                                     const vector<int> &all_matches,
                                     vector<int> random_generator)
{
  int max_value = (all_matches.size() / 2) - 1;
  vector<int> point_indices = 
      get_random_numbers(4, random_generator);


  vector<double> from_1(2);
  vector<double> to_1(2);
  vector<double> from_2(2);
  vector<double> to_2(2);
  vector<double> from_3(2);
  vector<double> to_3(2);
  vector<double> from_4(2);
  vector<double> to_4(2);

  int index_1, index_2, index_3, index_4;

  index_1 = point_indices[0] * 2;
  index_2 = point_indices[1] * 2;
  index_3 = point_indices[2] * 2;
  index_4 = point_indices[3] * 2;


  to_1[0] = image1_descriptors[all_matches[index_1]].col;
  to_1[1] = image1_descriptors[all_matches[index_1]].row;

  to_2[0] = image1_descriptors[all_matches[index_2]].col;
  to_2[1] = image1_descriptors[all_matches[index_2]].row;

  to_3[0] = image1_descriptors[all_matches[index_3]].col;
  to_3[1] = image1_descriptors[all_matches[index_3]].row;

  to_4[0] = image1_descriptors[all_matches[index_4]].col;
  to_4[1] = image1_descriptors[all_matches[index_4]].row;

  from_1[0] = image2_descriptors[all_matches[index_1 + 1]].col;
  from_1[1] = image2_descriptors[all_matches[index_1 + 1]].row;

  from_2[0] = image2_descriptors[all_matches[index_2 + 1]].col;
  from_2[1] = image2_descriptors[all_matches[index_2 + 1]].row;

  from_3[0] = image2_descriptors[all_matches[index_3 + 1]].col;
  from_3[1] = image2_descriptors[all_matches[index_3 + 1]].row;

  from_4[0] = image2_descriptors[all_matches[index_4 + 1]].col;
  from_4[1] = image2_descriptors[all_matches[index_4 + 1]].row;


  vector<double> transformation_matrix = 
          get_transformation_matrix(from_1, to_1,
                                    from_2, to_2,
                                    from_3, to_3,
                                    from_4, to_4);
          /*
  printf("from_1: %f %f\n", from_1[0], from_1[1]);
  printf("from_2: %f %f\n", from_2[0], from_2[1]);
  printf("from_3: %f %f\n", from_3[0], from_3[1]);
  printf("from_4: %f %f\n", from_4[0], from_4[1]);

  printf("to_1: %f %f\n", to_1[0], to_1[1]);
  printf("to_2: %f %f\n", to_2[0], to_2[1]);
  printf("to_3: %f %f\n", to_3[0], to_3[1]);
  printf("to_4: %f %f\n", to_4[0], to_4[1]);

  for (int i = 0; i < transformation_matrix.size(); i++) {
    printf("%f ", transformation_matrix[i]);
  }
  printf("\n");

  //exit(0);
  */


  return transformation_matrix;
}



double check_inlier(double x_1,
                    double x_2,
                    double y_1,
                    double y_2,
                    double inlier_dist_threshold)
{

  //printf("%f %f\n", x_1, x_2);
  //printf("%f %f\n", y_1, y_2);
  //printf("%f %f\n", (x_1 - x_2), (y_1 - y_2));


  //printf("%f %f\n", pow((x_1 - x_2), 2.0), pow((y_1 - y_2), 2.0));
  //printf("%f\n", );

  double distance =
        sqrt(pow((x_1 - x_2), 2.0) + 
             pow((y_1 - y_2), 2.0));

  //printf("distance: %f\n", distance);
  //printf("\n");

  if (distance < inlier_dist_threshold) {
    return distance;
  } else {
    return -1.0;
  }

}



double evaluate_model(const vector<SiftDescriptor> &image1_descriptors,
                      const vector<SiftDescriptor> &image2_descriptors,
                      const vector<int> &all_matches,
                      const vector<double> &transformation_matrix,
                      double inlier_dist_threshold,
                      double inlier_ratio_threshold)
{

  int total_inliers = 0;

  double pixel_distance;
  double total_distance = 0.0;

  vector<double> transformed_pixels(2);
  SiftDescriptor descriptor_1, descriptor_2;

  for (int match_element = 0;
       match_element < all_matches.size();
       match_element += 2) {

    descriptor_1 = image1_descriptors[all_matches[match_element]];
    descriptor_2 = image2_descriptors[all_matches[match_element+1]];

    transformed_pixels = 
        transform_pixel(transformation_matrix,
                        descriptor_2.col,
                        descriptor_2.row,
                        1.0);

    //printf("%f %f\n", descriptor_1.col, descriptor_1.row);
    //printf("%f %f\n", descriptor_2.col, descriptor_2.row);
    //printf("%f %f\n", transformed_pixels[0], transformed_pixels[1]);

    pixel_distance = check_inlier(descriptor_1.col,
                                  transformed_pixels[0],
                                  descriptor_1.row,
                                  transformed_pixels[1],
                                  inlier_dist_threshold);

    if (pixel_distance != -1.0) {
      total_inliers += 1;
      total_distance += pixel_distance;
    }

  }
  //printf("total inliers: %d\n", total_inliers);
  //printf("ratio: %f\n", ((double) total_inliers / (double) (all_matches.size() / 2)));

  //exit(0);

  if (((double) total_inliers / (double) (all_matches.size() / 2))
       < inlier_ratio_threshold) {
    return -1.0;
  } else {
    return total_distance;
  }

}



vector<double> get_best_homography(const vector<SiftDescriptor> &image1_descriptors,
                                   const vector<SiftDescriptor> &image2_descriptors,
                                   const vector<int> &all_matches,
                                   double inlier_dist_threshold,
                                   double inlier_ratio_threshold,
                                   int number_trials)
{

  vector<double> best_homography;
  double best_distance = -1.0;

  vector<double> current_homography;
  double current_distance;

  int max_bad_models = 4*number_trials;

  vector<int> random_generator(all_matches.size() / 2);
  for (int i = 0; i < (all_matches.size() / 2); i++) {
    random_generator[i] = i;
  }

  for (int trial = 0; trial < number_trials; trial++) {
    current_homography = 
        get_random_homography(image1_descriptors,
                              image2_descriptors,
                              all_matches,
                              random_generator);

    current_distance = 
        evaluate_model(image1_descriptors,
                       image2_descriptors,
                       all_matches,
                       current_homography,
                       inlier_dist_threshold,
                       inlier_ratio_threshold);

    if (current_distance == -1.0) {
      //printf("bad model\n");
      trial--;
      max_bad_models--;
      if (max_bad_models < 0) {
        best_homography.push_back(-1);
        return best_homography;
      }
      continue;
    }

    if (best_distance == -1.0) {
      //printf("first model\n");
      best_distance = current_distance;
      best_homography = current_homography;
    } else if (current_distance < best_distance) {
      //printf("better model\n");
      best_distance = current_distance;
      best_homography = current_homography;
    }

  }

  return best_homography;

}



vector<int> get_inliers(const vector<SiftDescriptor> &image1_descriptors,
                        const vector<SiftDescriptor> &image2_descriptors,
                        const vector<int> &all_matches,
                        const vector<double> &transformation_matrix,
                        double inlier_dist_threshold)
{
  vector<int> inliers;

  vector<double> transformed_pixels(2);
  SiftDescriptor descriptor_1;
  SiftDescriptor descriptor_2;

  double pixel_distance;

  for (int match_element = 0;
       match_element < all_matches.size();
       match_element += 2) {

    /*
    printf("get descriptors\n");
    printf("match element: %d\n", match_element);
    printf("all_matches size: %d\n", all_matches.size());
    printf("image1_descriptors size: %d\n", image1_descriptors.size());
    printf("image2_descriptors size: %d\n", image2_descriptors.size());
    printf("all_matches index 1: %d\n", all_matches[match_element]);
    printf("all_matches index 2: %d\n", all_matches[match_element+1]);
    */

    //printf("image_1\n");
    descriptor_1 = image1_descriptors[all_matches[match_element]];
    //printf("image_2\n");
    descriptor_2 = image2_descriptors[all_matches[match_element+1]];

    //descriptor_1 = image1_descriptors[all_matches[match_element]];
    //descriptor_2 = image2_descriptors[all_matches[match_element+1]];

    //printf("transform_pixels\n");
    //printf("transform_matrix_size %d\n", transformation_matrix.size());
    //exit(0);

    transformed_pixels = 
        transform_pixel(transformation_matrix,
                        descriptor_2.col,
                        descriptor_2.row,
                        1.0);

    //printf("get pixel distance\n");

    pixel_distance = check_inlier(descriptor_1.col,
                                  transformed_pixels[0],
                                  descriptor_1.row,
                                  transformed_pixels[1],
                                  inlier_dist_threshold);

    //printf("check distance\n");

    if (pixel_distance != -1.0) {
      inliers.push_back(all_matches[match_element]);
      inliers.push_back(all_matches[match_element+1]);
    }

  }

  //printf("return inliers\n");

  return inliers;
}



vector<int> get_height_width(const vector<double> &transformation_matrix,
                             const CImg<double> &transform_from)
{
  vector<double> transformed_pixel;
  vector<int> height_width(2);
  height_width[0] = 0;
  height_width[1] = 0;

  transformed_pixel =
      transform_pixel(transformation_matrix,
                      (double) (transform_from.width() - 1),
                      (double) (transform_from.height() - 1),
                      1.0);

  if (transformed_pixel[0] > (double) height_width[0]) {
    height_width[0] = (int) ceil(transformed_pixel[0]);
  }

  if (transformed_pixel[0] > (double) height_width[0]) {
    height_width[0] = (int) ceil(transformed_pixel[0]);
  }

  if (transformed_pixel[1] > (double) height_width[1]) {
    height_width[1] = (int) ceil(transformed_pixel[1]);
  }

  transformed_pixel =
      transform_pixel(transformation_matrix,
                      0.0,
                      (double) (transform_from.height() - 1),
                      1.0);

  if (transformed_pixel[0] > (double) height_width[0]) {
    height_width[0] = (int) ceil(transformed_pixel[0]);
  }

  if (transformed_pixel[1] > (double) height_width[1]) {
    height_width[1] = (int) ceil(transformed_pixel[1]);
  }


  transformed_pixel =
      transform_pixel(transformation_matrix,
                      (double) (transform_from.width() - 1),
                      0.0,
                      1.0);

  if (transformed_pixel[0] > (double) height_width[0]) {
    height_width[0] = (int) ceil(transformed_pixel[0]);
  }

  if (transformed_pixel[1] > (double) height_width[1]) {
    height_width[1] = (int) ceil(transformed_pixel[1]);
  }

  transformed_pixel =
      transform_pixel(transformation_matrix,
                      0.0,
                      0.0,
                      1.0);

  if (transformed_pixel[0] > (double) height_width[0]) {
    height_width[0] = (int) ceil(transformed_pixel[0]);
  }

  if (transformed_pixel[1] > (double) height_width[1]) {
    height_width[1] = (int) ceil(transformed_pixel[1]);
  }

  return height_width;


}



CImg<double> merge_images(const CImg<double> &base_image,
                          const CImg<double> &new_image)
{

  int height, width;
  if (base_image.width() > new_image.width()) {
    width = base_image.width();
  } else {
    width = new_image.width();
  }

  if (base_image.height() > new_image.height()) {
    height = base_image.height();
  } else {
    height = new_image.height();
  }

  CImg<double> new_base_image(width, height, 1, 3);
  
  for (int width = 0; width < new_image.width(); width++) {
    for (int height = 0; height < new_image.height(); height++) {
      for (int color = 0; color < 3; color++) {
        new_base_image(width, height, 0, color) =
            new_image(width, height, 0, color);
      }
    }
  }
  

  for (int width = 0; width < base_image.width(); width++) {
    for (int height = 0; height < base_image.height(); height++) {
      if ((base_image(width, height, 0, 0) != 0.0) ||
          (base_image(width, height, 0, 1) != 0.0) ||
          (base_image(width, height, 0, 2) != 0.0)) {

        for (int color = 0; color < 3; color++) {
          new_base_image(width, height, 0, color) =
              base_image(width, height, 0, color);
        }
      }
    }
  }

  return new_base_image;
}



vector<double> matrix_multiply(vector<double> best_homography,
                               vector<double> transform_matrix)
{
  if (transform_matrix.size() == 0) {
    return best_homography;
  }

  vector<double> new_matrix(9);

  new_matrix[0] = 
      best_homography[0] * transform_matrix[0] +
      best_homography[3] * transform_matrix[1] +
      best_homography[6] * transform_matrix[2];

  new_matrix[3] = 
      best_homography[0] * transform_matrix[3] +
      best_homography[3] * transform_matrix[4] +
      best_homography[6] * transform_matrix[5];

  new_matrix[6] = 
      best_homography[0] * transform_matrix[6] +
      best_homography[3] * transform_matrix[7] +
      best_homography[6] * transform_matrix[8];



  new_matrix[1] = 
      best_homography[1] * transform_matrix[0] +
      best_homography[4] * transform_matrix[1] +
      best_homography[7] * transform_matrix[2];

  new_matrix[4] = 
      best_homography[1] * transform_matrix[3] +
      best_homography[4] * transform_matrix[4] +
      best_homography[7] * transform_matrix[5];

  new_matrix[7] = 
      best_homography[1] * transform_matrix[6] +
      best_homography[4] * transform_matrix[7] +
      best_homography[7] * transform_matrix[8];


  new_matrix[2] = 
      best_homography[2] * transform_matrix[0] +
      best_homography[5] * transform_matrix[1] +
      best_homography[8] * transform_matrix[2];

  new_matrix[5] = 
      best_homography[2] * transform_matrix[3] +
      best_homography[5] * transform_matrix[4] +
      best_homography[8] * transform_matrix[5];

  new_matrix[8] = 
      best_homography[2] * transform_matrix[6] +
      best_homography[5] * transform_matrix[7] +
      best_homography[8] * transform_matrix[8];


  return new_matrix;

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

  srand(unsigned(time(0)));

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


    }
    else if(part == "part3"){
      double sift_threshold = 0.5;

      double ransac_threshold = 0.5;
      double inlier_dist_threshold = 40; 
      double inlier_ratio_threshold = 0.5;
      int number_trials = 100;

      if (argc != 4) {
        printf("Program Usage ./a2 part3 [input_image1] [input_image2]\n");
        exit(0);
      }

      CImg<double> input_image1;
      input_image1.load(argv[2]);
      CImg<double> input_image2;
      input_image2.load(argv[3]);
      CImg<double> combined_image = 
          combine_images(input_image1, input_image2);

      Sift sift;

      vector<SiftDescriptor> image1_descriptors =
                        sift.compute_sift(input_image1);
      vector<SiftDescriptor> image2_descriptors =
                        sift.compute_sift(input_image2);

      vector<int> all_matches =
            find_all_matches(image1_descriptors,
                             image2_descriptors,
                             sift_threshold);

      CImg<double> sift_matches = 
          draw_lines(combined_image,
                     image1_descriptors,
                     image2_descriptors,
                     all_matches,
                     input_image1.width());

      sift_matches.save("sift_maches.png");

      //printf("all_matches\n");

      all_matches =
          find_all_matches(image1_descriptors,
                           image2_descriptors,
                           ransac_threshold);

      if (all_matches.size() < 8) {
        printf("Not enough matches found\n");
        exit(0);
      }

      //printf("transformation matrix\n");

      vector<double> transformation_matrix = 
          get_best_homography(image1_descriptors,
                              image2_descriptors,
                              all_matches,
                              inlier_dist_threshold,
                              inlier_ratio_threshold,
                              number_trials);

      if (transformation_matrix.size() == 1) {
        printf("No good models found\n");
        exit(0);
      }
      //printf("inliers\n");

      vector<int> inliers = 
          get_inliers(image1_descriptors,
                      image2_descriptors,
                      all_matches,
                      transformation_matrix,
                      inlier_dist_threshold);

      //printf("ransac matches\n");

      CImg<double> ransac_matches =
          draw_lines(combined_image,
                     image1_descriptors,
                     image2_descriptors,
                     inliers,
                     input_image1.width());   

      ransac_matches.save("ransac_maches.png");   

    }
    else if(part == "part4"){
      if (argc < 5) {
        printf("Program Usage ./a2 part5 [images]\n");
        printf("At least 3 images must be provided\n");
        exit(0);
      }

      double ransac_threshold = 0.5;
      double inlier_dist_threshold = 10; 
      double inlier_ratio_threshold = 0.7;
      int number_trials = 100;

      vector<int> height_width;

      Sift sift;
      vector<SiftDescriptor> transform_to_descriptors;            
      vector<SiftDescriptor> transform_from_descriptors;
      vector<int> all_matches;

      CImg<double> base_image;
      base_image.load(argv[2]);
      vector<double> best_homography;

      CImg<double> transform_to;
      CImg<double> transform_from;
      CImg<double> transformed_image;
      vector<double> current_transform(0);

      for (int image_num = 3; image_num < argc; image_num++) {

        printf("image_num %d\n", image_num);

        transform_to.load(argv[image_num - 1]);
        transform_from.load(argv[image_num]);

        transform_to_descriptors = 
            sift.compute_sift(transform_to);
        transform_from_descriptors = 
            sift.compute_sift(transform_from);

        all_matches =
            find_all_matches(transform_to_descriptors,
                             transform_from_descriptors,
                             ransac_threshold);

        best_homography = 
          get_best_homography(transform_to_descriptors,
                              transform_from_descriptors,
                              all_matches,
                              inlier_dist_threshold,
                              inlier_ratio_threshold,
                              number_trials);

        current_transform = 
            matrix_multiply(best_homography,
                            current_transform);

        /*
        for (int i = 0; i < 9; i++) {
          printf("%f ", best_homography[i]);
        }
        printf("\n");

        for (int i = 0; i < 9; i++) {
          printf("%f ", current_transform[i]);
        }
        */

        printf("\n\n");

        height_width =
            get_height_width(current_transform,
                             transform_from);

        transformed_image =
            transform_image(transform_from,
                            current_transform,
                            height_width[0],
                            height_width[1]);

        //base_image.save("base_image.png");
        //transformed_image.save("transformed_image.png");
        base_image = merge_images(base_image, transformed_image);
        //base_image.save("after_transform.png");

        //base_image.save("merged_image.png");
        //exit(0);
      }

      base_image.save("panaroma.png");


    }
    
    
    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}
