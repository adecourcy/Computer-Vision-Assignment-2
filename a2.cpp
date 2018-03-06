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


/*
  Given a pixel and a matrix, transform that pixel to the new coordinate set
  Returns a 2 value vector, the first value being the new x-coordinate, the
  second being the new y-coordinate
*/
vector<double> transform_pixel(const vector<double> &transformation_matrix,
                               int x,
                               int y,
                               double z)
{
  vector<double> coordinates(2);
  double new_z;

  // First calculate the new z value
  new_z = ((double) x) * transformation_matrix[6] + 
          ((double) y) * transformation_matrix[7] +
          z * transformation_matrix[8];

  // Then calculate our new normalized x and y values
  coordinates[0] = (((double) x) * transformation_matrix[0] + 
                    ((double) y) * transformation_matrix[1] +
                    z * transformation_matrix[2]) / new_z;

  coordinates[1] = (((double) x) * transformation_matrix[3] + 
                    ((double) y) * transformation_matrix[4] +
                    z * transformation_matrix[5]) / new_z;



  return coordinates;
}


/*
  Given a matrix in vector format, return the inverse of that matrix,
  in vector format. Calculated using the determinant method
*/
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


/*
  Given a image, and a transformation matrix, transform that image into the
  new coordinate system. The height and with parameters allow for creating a
  new image background of custom size.
*/
CImg<double> transform_image(const CImg<double> &original_image,
                             const vector<double> &transformation_matrix,
                             int width,
                             int height)
{
  // This will be the storage of our transformed pixels
  vector<double> cmpr_coords(2);

  // We will write our transformed image to this variable
  CImg<double> transformed_image(width,
                                 height,
                                 1,
                                 3);
  // Make the image black to avoid garbage initialization values
  transformed_image.fill(0.0);

  // We'll do an inverse transformation, so we'll need the inverse matrix
  vector<double> inverse_matrix = 
      get_inverse_matrix(transformation_matrix);

  // transform the old image to the new coordinates, pixel-by-pixel
  for (int x = 0; x < transformed_image.width(); x++) {
    for (int y = 0; y < transformed_image.height(); y++) {
      // Get our transformed coordinate
      cmpr_coords = 
        transform_pixel(inverse_matrix,
                        x,
                        y,
                        1);

      // Make sure the transformed coordinate is in the bounds of our image
      cmpr_coords[0] = round(cmpr_coords[0]);
      cmpr_coords[1] = round(cmpr_coords[1]);
      if (cmpr_coords[0] < 0.0 ||
          cmpr_coords[0] >= (float) original_image.width() ||
          cmpr_coords[1] < 0.0 ||
          cmpr_coords[1] >= (float) original_image.height()) {

          continue;
      } else {
        for (int color = 0; color < 3; color++) {
          // Assign the transformed pixel to our transformation image
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


// Given a set of 8 points, find a homography matrix, and return a vector of values
vector<double> get_transformation_matrix(vector<double> from_1,
                                         vector<double> to_1,
                                         vector<double> from_2,
                                         vector<double> to_2,
                                         vector<double> from_3,
                                         vector<double> to_3,
                                         vector<double> from_4,
                                         vector<double> to_4)
{

  // Set up the matrix per Burger book
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

  // Turn our CImg matrix into a vector
  for (int i = 0; i < 8; i++) {
    transformation_matrix[i] = A(i);
  }

  transformation_matrix[8] = 1.0;

  return transformation_matrix;
}



void transform_lincolm()
{

  CImg<double> input_image;
  input_image.load("./images/part1/lincoln.png");

  vector<double> transform_matrix(9);
  transform_matrix[0] = 0.907;
  transform_matrix[1] = 0.258;
  transform_matrix[2] = -182.0;

  transform_matrix[3] = -0.153;
  transform_matrix[4] = 1.44;
  transform_matrix[5] = 58;

  transform_matrix[6] = -0.000306;
  transform_matrix[7] = 0.000731;
  transform_matrix[8] = 1.0;

  CImg<double> transformed_image = 
      transform_image(input_image,
                      transform_matrix,
                      input_image.width(),
                      input_image.height());

  transformed_image.save("lincoln_warped.png");

}



// Transform the book according to the PDF description
void transform_book()
{

  CImg<double> input_image;
  input_image.load("./images/part1/book2.jpg");

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

  // Print the homography via the instructions:

  printf("%f %f %f\n", H[0], H[1], H[2]);
  printf("%f %f %f\n", H[3], H[4], H[5]);
  printf("%f %f %f\n", H[6], H[7], H[8]);

  CImg<double> transformed_image = 
      transform_image(input_image,
                      H,
                      input_image.width(),
                      input_image.height());

  transformed_image.save("book_result.png");

}



// Given an image, overlay that image on to the billboards
void imageOnBillboards(CImg<double> input_image)
{

  // Load all our billboard images
  CImg<double> billboard_1;
  CImg<double> billboard_2;
  CImg<double> billboard_3;
  billboard_1.load("./images/part1/billboard1.jpg");
  billboard_2.load("./images/part1/billboard2.png");
  billboard_3.load("./images/part1/billboard3.jpg");

  // Variables for our coorespondences for the homography matrix
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

  billboard_1.save("synthetic_billboard1.png", -1, 6);

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

  billboard_2.save("synthetic_billboard2.png", -1, 6);


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

  billboard_3.save("synthetic_billboard3.png", -1, 6);


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
  // Just use some really large numbers that will be replaced on the first iteration
  double best_distance = 100000000.0;
  double second_best_distance = 100000000.0;
  double current_distance;

  int best_descriptor_element;

  for (int element = 0; element < match_descriptors.size(); element++) {
    // Find the distance between our target descriptor and potential match
    current_distance =
        find_distance(original_descriptor, match_descriptors[element]);

    /*
      Assign the distance between to descriptors to the best distance if
      it is the lowest we've seen so far. Assign the previous best distance
      to the second best distance for ratio comparison later
    */
    if (current_distance < best_distance) {
      best_descriptor_element = element;
      second_best_distance = best_distance;
      best_distance = current_distance;

    /*
      If the new comparison isn't better than our best distance,
      see if it is better than our second best distance
    */
    } else if (current_distance < second_best_distance) {
      second_best_distance = current_distance;
    }
  }


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
  // The index of a matched element
  int match_index;
  // Indices of all matched elements
  vector<int> matches;

  // iterate over our descriptors
  for (int element = 0; element < original_descriptors.size(); element++) {

    // See if there is a match for our descriptor
    match_index = find_match(original_descriptors[element],
                             match_descriptors,
                             threshold);

    // If there is no match, continue to the next descriptor
    if (match_index == -1) {
      continue;
    // Otherwise, store the correspondence
    } else {
      matches.push_back(element);
      matches.push_back(match_index);
    }
  }

  return matches;
}



// Given two images, put then side by side for line correspondences
CImg<double> combine_images(const CImg<double> &original_image,
                            const CImg<double> &new_image)
{
  // Find the larger height of the 2 images
  int larger_height;

  if (original_image.height() > new_image.height()) {
    larger_height = original_image.height();
  } else {
    larger_height = new_image.height();
  }

  // create a new image equal to the combined width and the large height of the 2 images
  CImg<double> combined_image(original_image.width() + new_image.width(),
                              larger_height,
                              1,
                              3);
  // Make sure we don't have any garbage values
  combined_image.fill(0.0);

  // Put the first image on the new background, pixel-by-pixel
  for (int x = 0; x < original_image.width(); x++) {
    for (int y = 0; y < original_image.height(); y++) {
      for (int color = 0; color < 3; color++) {
        combined_image(x, y, 0, color) = original_image(x, y, 0, color);
      }
    }
  }

  // Put the second image on the new background, pixel-by-pixel
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




/*
  Given a combined image, a set of descriptors, and a set of matches,
  draw lines between the corresponding descriptors
*/
CImg<double> draw_lines(CImg<double> combined_image,
                        const vector<SiftDescriptor> &image1_descriptors,
                        const vector<SiftDescriptor> &image2_descriptors,
                        const vector<int> &all_matches,
                        int added_width)
{

  // The x and y values for the corresponding elements
  int x_1, x_2, y_1, y_2;

  for (int element = 0; element < all_matches.size(); element += 2) {
    x_1 = image1_descriptors[all_matches[element]].col;
    y_1 = image1_descriptors[all_matches[element]].row;
    // Our 2nd image will be on the right, so we need to add the width of the first image
    x_2 = image2_descriptors[all_matches[element+1]].col + added_width;
    y_2 = image2_descriptors[all_matches[element+1]].row;

    const unsigned char color[] = { 0,255,0 };
    combined_image.draw_line(x_1, y_1, x_2, y_2, color);
  }

  return combined_image;

}



// Get a set of unique numbers by shuffling all possible numbers in range
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



// Given a set of corresponding sift descriptors,
// create a homography from randomly selected points
vector<double> get_random_homography(const vector<SiftDescriptor> &image1_descriptors,
                                     const vector<SiftDescriptor> &image2_descriptors,
                                     const vector<int> &all_matches,
                                     vector<int> random_generator)
{
  // select random points to use for homography
  vector<int> point_indices = 
      get_random_numbers(4, random_generator);

  // storage for homography coordinates
  vector<double> from_1(2);
  vector<double> to_1(2);
  vector<double> from_2(2);
  vector<double> to_2(2);
  vector<double> from_3(2);
  vector<double> to_3(2);
  vector<double> from_4(2);
  vector<double> to_4(2);

  // The indices of our descriptor points
  int index_1, index_2, index_3, index_4;

  /*
    all_matches contains the indices for our descriptor and it's match.
    Therefore, it contains all_matches.size() / 2 correspondances. We select
    from 0 to all_matches.size() / 2 as correspondances for the homography,
    so we transform the indices back to all_matches indices here
  */
  index_1 = point_indices[0] * 2;
  index_2 = point_indices[1] * 2;
  index_3 = point_indices[2] * 2;
  index_4 = point_indices[3] * 2;


  // Set up our coordinates from our selected correspondances
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

  // Create our homography
  vector<double> transformation_matrix = 
          get_transformation_matrix(from_1, to_1,
                                    from_2, to_2,
                                    from_3, to_3,
                                    from_4, to_4);

  return transformation_matrix;
}



// Given a set of coordinates and a distance threshold, check if a point
// is an inlier or not
double check_inlier(double x_1,
                    double x_2,
                    double y_1,
                    double y_2,
                    double inlier_dist_threshold)
{


  double distance =
        sqrt(pow((x_1 - x_2), 2.0) + 
             pow((y_1 - y_2), 2.0));


  if (distance < inlier_dist_threshold) {
    return distance;
  } else {
    return -1.0;
  }

}



// Given a homography, evaluate the quality of the model
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

  vector<double> transformed_pixels(2); // storage for our pixels transformed by the homography
  SiftDescriptor descriptor_1, descriptor_2;

  // Iterate through all of our correspondances
  for (int match_element = 0;
       match_element < all_matches.size();
       match_element += 2) {

    descriptor_1 = image1_descriptors[all_matches[match_element]];
    descriptor_2 = image2_descriptors[all_matches[match_element+1]];

    // transform the correspondances into the same coordinate set
    transformed_pixels = 
        transform_pixel(transformation_matrix,
                        descriptor_2.col,
                        descriptor_2.row,
                        1.0);

    // Check their distance within the same coordinate set
    pixel_distance = check_inlier(descriptor_1.col,
                                  transformed_pixels[0],
                                  descriptor_1.row,
                                  transformed_pixels[1],
                                  inlier_dist_threshold);

    // If their distance is outside a given threshold, consider the
    // correspondance an outline. Otherwise, consider it an inlier
    // and add the correspondance distance to the total distance
    if (pixel_distance != -1.0) {
      total_inliers += 1;
      total_distance += pixel_distance;
    }

  }

  // If we don't have enough inliers, discard the model
  if (((double) total_inliers / (double) (all_matches.size() / 2))
       < inlier_ratio_threshold) {
    return -1.0;
  } else {
    // Otherwise, return the total distance to evaluate model quality
    return total_distance;
  }

}



// Iterate through a number of trials,
// selecting random homographies, and returning the best one
vector<double> get_best_homography(const vector<SiftDescriptor> &image1_descriptors,
                                   const vector<SiftDescriptor> &image2_descriptors,
                                   const vector<int> &all_matches,
                                   double inlier_dist_threshold,
                                   double inlier_ratio_threshold,
                                   int number_trials)
{

  // The best found homography and its score
  vector<double> best_homography;
  double best_distance = -1.0;

  // The current homography we are examining
  vector<double> current_homography;
  double current_distance;

  /*
    This algorithm only counts a "good" model as a trial, will continue
    searching if we find a "bad" model as if we didn't do a trial at all.
    This variable sets a cut-off point if we can't find enough good models,
    so we won't end up in an infinite loop
  */
  int max_bad_models = 4*number_trials;

  // We'll pick points from this vector later, using a shuffling algorithm
  vector<int> random_generator(all_matches.size() / 2);
  for (int i = 0; i < (all_matches.size() / 2); i++) {
    random_generator[i] = i;
  }

  // Try as many homographies as given trials
  for (int trial = 0; trial < number_trials; trial++) {
    // Get a homography
    current_homography = 
        get_random_homography(image1_descriptors,
                              image2_descriptors,
                              all_matches,
                              random_generator);

    // Get the quality of the homography
    current_distance = 
        evaluate_model(image1_descriptors,
                       image2_descriptors,
                       all_matches,
                       current_homography,
                       inlier_dist_threshold,
                       inlier_ratio_threshold);

    // If this model isn't good enough, discard the homography nad try again
    if (current_distance == -1.0) {
      trial--;
      max_bad_models--;
      // if we've seen too many bad homographies, give the user a message saying
      // we can't find good correspondances
      if (max_bad_models < 0) {
        best_homography.push_back(-1);
        return best_homography;
      }
      continue;
    }

    // If this is our first homography, call it our best
    if (best_distance == -1.0) {
      best_distance = current_distance;
      best_homography = current_homography;
    // Otherwised, compare it to our previous best
    } else if (current_distance < best_distance) {
      best_distance = current_distance;
      best_homography = current_homography;
    }

  }

  return best_homography;

}



// Given a good homography, get all the inlier points in this model
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

  // Go through all our descriptor matches
  for (int match_element = 0;
       match_element < all_matches.size();
       match_element += 2) {

    descriptor_1 = image1_descriptors[all_matches[match_element]];
    descriptor_2 = image2_descriptors[all_matches[match_element+1]];

    // Transform our descriptors to the same coordinate system
    transformed_pixels = 
        transform_pixel(transformation_matrix,
                        descriptor_2.col,
                        descriptor_2.row,
                        1.0);

    // Check if the correspondances are an inlier in our model
    pixel_distance = check_inlier(descriptor_1.col,
                                  transformed_pixels[0],
                                  descriptor_1.row,
                                  transformed_pixels[1],
                                  inlier_dist_threshold);

    // The correspondance is an inlier, add it to our list
    if (pixel_distance != -1.0) {
      inliers.push_back(all_matches[match_element]);
      inliers.push_back(all_matches[match_element+1]);
    }

  }


  return inliers;
}



// Given a transformation matrix and an image, find the height and
// width of the transformed image
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


// Blend code based on computer vision lab code cited in report
CImg<double> downscale(CImg<double> original_image,
                       CImg<double> filter)
{
  CImg<double> image = original_image.get_convolve(filter);


  int new_width, new_height;

  if (image.width() % 2 == 1) {
    new_width = (image.width() - 1) / 2;
  } else {
    new_width = image.width() / 2;
  }

  if (image.height() % 2 == 1) {
    new_height = (image.height() - 1) / 2;
  } else {
    new_height = image.height() / 2;
  }

  image.resize(new_width, new_height);

  return image;

}


CImg<double> upscale(CImg<double> image,
                     int width,
                     int height,
                     CImg<double> filter)
{
  CImg<double> upscaled_image = 
      image.get_resize(width, height);

  upscaled_image = upscaled_image.get_convolve(filter);

  return upscaled_image;

}



vector<CImg<double> > get_gaussian_pyramid(CImg<double> image,
                                           CImg<double> filter,
                                           int pyramid_size)
{

  vector<CImg<double> > gaussian_pyramid(pyramid_size);
  gaussian_pyramid[0] = image;

  for (int i = 1; i < pyramid_size; i++) {

    CImg<double> G_prev = gaussian_pyramid[i - 1];
    CImg<double> G_curr = downscale(G_prev, filter);

    gaussian_pyramid[i] = G_curr;
  }

  return gaussian_pyramid;
}



CImg<double> match_shape(CImg<double> original_image,
                         CImg<double> upscaled_image)
{
  CImg<double> matched_upscale(original_image.width(),
                               original_image.height(),
                               1,
                               original_image.spectrum());


  for (int x = 0; x < original_image.width(); x++) {
    for (int y = 0; y < original_image.height(); y++) {
      for (int color = 0; color < original_image.spectrum(); color++) {
        matched_upscale(x, y, 0, color) =
                upscaled_image(x, y, 0, color);
        //printf("%f %f\n", matched_upscale(x, y, 0, color), upscaled_image(x, y, 0, color));
      }
    }
  }

  return matched_upscale;
}


vector<CImg<double> > get_laplacian_pyramid(vector<CImg<double> > gaussian_pyramid,
                                            CImg<double> filter)
{

  vector<CImg<double> > laplacian_pyramid(gaussian_pyramid.size());
  laplacian_pyramid[0] = 
      gaussian_pyramid[gaussian_pyramid.size() - 1];

  int L_counter = 1;
  for (int i = gaussian_pyramid.size() - 2; i >= 0; i--) {
    CImg<double> G_curr = gaussian_pyramid[i];
    CImg<double> G_smooth = upscale(gaussian_pyramid[i+1],
                                    G_curr.width(),
                                    G_curr.height(),
                                    filter);

    CImg<double> lap_pyr(G_curr.width(), G_curr.height(), 1, G_curr.spectrum());
    lap_pyr.fill(0.0);

    for (int x = 0; x < G_curr.width(); x++) {
      for (int y = 0; y < G_curr.height(); y++) {
        for (int c = 0; c < G_curr.spectrum(); c++) {
          lap_pyr(x, y, 0, c) = G_curr(x, y, 0, c) - G_smooth(x, y, 0, c);
        }
      }
    }
  
    laplacian_pyramid[L_counter] = lap_pyr;
    L_counter += 1;
  }

  return laplacian_pyramid;

}



vector<CImg<double> > get_laplacian_blend(vector<CImg<double> > laplacian_pyramid_1,
                                          vector<CImg<double> > laplacian_pyramid_2,
                                          vector<CImg<double> > mask_pyramid)
{

  vector<CImg<double> > LB(laplacian_pyramid_1.size()); 

  int L_counter = laplacian_pyramid_1.size() - 1;
  for (int i = 0; i < laplacian_pyramid_1.size(); i++) {
    CImg<double> LB_curr(laplacian_pyramid_1[L_counter].width(),
                         laplacian_pyramid_1[L_counter].height(),
                         1,
                         3);

    CImg<double> mask_curr = mask_pyramid[i];
    CImg<double> img1_L_curr = laplacian_pyramid_1[L_counter];
    CImg<double> img2_L_curr = laplacian_pyramid_2[L_counter];

    //printf("iterate images\n");
     
  for (int x = 0; x < img1_L_curr.width(); x++) {
    for (int y = 0; y < img1_L_curr.height(); y++) {
      for (int c = 0; c < img1_L_curr.spectrum(); c++) {
        LB_curr(x, y, 0, c) =
            ( (mask_curr(x, y, 0, 0) / 255.0) * img2_L_curr(x, y, 0, c) ) +
            ( (1 - (mask_curr(x, y, 0, 0) / 255.0)) * img1_L_curr(x, y, 0, c) ); 
      }
    }
  }

    //printf("update index\n");

    LB[i] = LB_curr;
    L_counter -= 1;

    //printf("finish\n");
  }

  return LB;

}



CImg<double> get_blended_image(vector<CImg<double> > laplacian_blend,
                               CImg<double> filter)
{

    vector<CImg<double> > steps(laplacian_blend.size());
    steps[0] = laplacian_blend[laplacian_blend.size() - 1];

    int L_counter = laplacian_blend.size() - 2;
    //int sizes[6] = {10, 20, 39, 77, 153, 307};
    for (int i = 1; i < laplacian_blend.size(); i++) {
      CImg<double> prev_step = steps[i-1];
      CImg<double> curr_LB = laplacian_blend[L_counter];
      CImg<double> curr_smooth =
          upscale(prev_step,
                  curr_LB.width(),
                  curr_LB.height(),
                  filter);

      CImg<double> next_step(curr_LB.width(), curr_LB.height(), 1, 3);

      for (int x = 0; x < next_step.width(); x++) {
        for (int y = 0; y < next_step.height(); y++) {
          for (int c = 0; c < next_step.spectrum(); c++) {
            next_step(x, y, 0, c) = 
                curr_LB(x, y, 0, c) + curr_smooth(x, y, 0, c);
          }
        }
      }

      steps[i] = next_step;
      L_counter -= 1;
    }

  return steps[steps.size() - 1];
}



/*
  This code was going to be used to smooth panorama overlaps, but
  we didn't get part 2 working, so I commented it out
*/
CImg<double> get_smoothed_overlap(const CImg<double> &base_image,
                                  const CImg<double> &new_image,
                                  int left_side_overlap,
                                  int right_side_overlap,
                                  int height)
{

  int overlap_width = (right_side_overlap - left_side_overlap) + 1;

  CImg<double> smoothed_overlap_area(overlap_width, height, 1, 3);
  smoothed_overlap_area.fill(0.0);

  CImg<double> base_image_overlap(overlap_width, height, 1, 3);
  CImg<double> new_image_overlap(overlap_width, height, 1, 3);
  CImg<double> mask_image_overlap(overlap_width, height, 1, 1);
  mask_image_overlap.fill(0.0);
  base_image_overlap.fill(0.0);
  new_image_overlap.fill(0.0);

  int left_side = (int) (overlap_width / 2.0);
  for (int x = 0; x < overlap_width; x++) {
    for (int y = 0; y < height; y++) {
      if (x < left_side) {
        mask_image_overlap(x, y, 0, 0) = 0.0;
      } else {
        mask_image_overlap(x, y, 0, 0) = 255.0; 
      }
    }
  }


  for (int x = 0; x < overlap_width; x++) {
    for (int y = 0; y < new_image.height(); y++) {

      new_image_overlap(x, y, 0, 0) = 
          new_image(x + left_side_overlap, y, 0, 0);
      new_image_overlap(x, y, 0, 1) = 
          new_image(x + left_side_overlap, y, 0, 1);
      new_image_overlap(x, y, 0, 2) = 
          new_image(x + left_side_overlap, y, 0, 2);

    }
  }


  for (int x = 0; x < overlap_width; x++) {
    for (int y = 0; y < base_image.height(); y++) {
      base_image_overlap(x, y, 0, 0) = 
          base_image(x + left_side_overlap, y, 0, 0);
      base_image_overlap(x, y, 0, 1) = 
          base_image(x + left_side_overlap, y, 0, 1);
      base_image_overlap(x, y, 0, 2) = 
          base_image(x + left_side_overlap, y, 0, 2);

    }
  }

  //printf("%d %d\n", base_image_overlap.width(), base_image_overlap.height());
  //printf("%d %d\n", new_image_overlap.width(), base_image_overlap.height());
  //printf("%d %d\n", mask_image_overlap.width(), base_image_overlap.height());

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

  vector<CImg<double> > new_image_gauss_pyr = 
      get_gaussian_pyramid(new_image_overlap, filter, 6);

  new_image_gauss_pyr[0].save("1gn.png");
  new_image_gauss_pyr[1].save("2gn.png");
  new_image_gauss_pyr[2].save("3gn.png");
  new_image_gauss_pyr[3].save("4gn.png");
  new_image_gauss_pyr[4].save("5gn.png");
  new_image_gauss_pyr[5].save("6gn.png");

  vector<CImg<double> > base_image_gauss_pyr = 
      get_gaussian_pyramid(base_image_overlap, filter, 6);

  base_image_gauss_pyr[0].save("1gb.png");
  base_image_gauss_pyr[1].save("2gb.png");
  base_image_gauss_pyr[2].save("3gb.png");
  base_image_gauss_pyr[3].save("4gb.png");
  base_image_gauss_pyr[4].save("5gb.png");
  base_image_gauss_pyr[5].save("6gb.png");

  vector<CImg<double> > mask_gauss_pyr = 
      get_gaussian_pyramid(mask_image_overlap, filter, 6);


  vector<CImg<double> > new_image_lap_pyr = 
      get_laplacian_pyramid(new_image_gauss_pyr, filter);

  new_image_lap_pyr[0].save("1ln.png");
  new_image_lap_pyr[1].save("2ln.png");
  new_image_lap_pyr[2].save("3ln.png");
  new_image_lap_pyr[3].save("4ln.png");
  new_image_lap_pyr[4].save("5ln.png");
  new_image_lap_pyr[5].save("6ln.png");

  vector<CImg<double> > base_image_lap_pyr = 
      get_laplacian_pyramid(base_image_gauss_pyr, filter);


  vector<CImg<double> > lap_blend = 
        get_laplacian_blend(base_image_lap_pyr,
                            new_image_lap_pyr,
                            mask_gauss_pyr);


  smoothed_overlap_area = get_blended_image(lap_blend, filter);

  for (int x = 0; x < smoothed_overlap_area.width(); x++) {
    for (int y = 0; y < smoothed_overlap_area.height(); y++) {
      for (int c = 0; c < 3; c++) {
        if (smoothed_overlap_area(x, y, 0, c) < 0.0) {
          smoothed_overlap_area(x, y, 0, c) = 0.0;
        }
      }
    }
  }

  smoothed_overlap_area.normalize(0, 255);


  /*
  for (int x = 0; x < overlap_width; x++) {
    for (int y = 0; y < new_image.height(); y++) {

      smoothed_overlap_area(x, y, 0, 0) = 
          new_image(x + left_side_overlap, y, 0, 0);
      smoothed_overlap_area(x, y, 0, 1) = 
          new_image(x + left_side_overlap, y, 0, 1);
      smoothed_overlap_area(x, y, 0, 2) = 
          new_image(x + left_side_overlap, y, 0, 2);
    }
  }


  for (int x = 0; x < overlap_width; x++) {
    for (int y = 0; y < base_image.height(); y++) {
      smoothed_overlap_area(x, y, 0, 0) = 
          base_image(x + left_side_overlap, y, 0, 0);
      smoothed_overlap_area(x, y, 0, 1) = 
          base_image(x + left_side_overlap, y, 0, 1);
      smoothed_overlap_area(x, y, 0, 2) = 
          base_image(x + left_side_overlap, y, 0, 2);
    }
  }
  */


  return smoothed_overlap_area;
}



int find_left_edge(const CImg<double> &new_image) {

  for (int x = 0; x < new_image.width(); x++) {
    for (int y = 0; y < new_image.height(); y++) {
      //printf("%f %f %f\n", new_image(x, y, 0, 0),
      //                     new_image(x, y, 0, 1),
      //                     new_image(x, y, 0, 2));
      if (new_image(x, y, 0, 0) != 0.0 ||
          new_image(x, y, 0, 1) != 0.0 ||
          new_image(x, y, 0, 2) != 0.0) {
        return x;
      }
    }
  }

}


/*
  Merge 2 images, transformed to the same coordinate system,
  in order to create a panorama
*/
CImg<double> merge_images(const CImg<double> &base_image,
                          const CImg<double> &new_image)
{

  // Find the height and width of the merged image
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

  
  int left_side_overlap, right_side_overlap;
  right_side_overlap = base_image.width();
  left_side_overlap = find_left_edge(new_image);

  // printf("width: %d\n", new_image.width());
  // printf("left overlap: %d\n", left_side_overlap);
  // printf("right overlap: %d\n", right_side_overlap);

  /*
  CImg<double> smoothed_overlap = 
      get_smoothed_overlap(base_image,
                           new_image,
                           left_side_overlap,
                           right_side_overlap,
                           height);
  */
  

  CImg<double> new_base_image(width, height, 1, 3);

  // Copy the new image first  
  for (int x = 0; x < new_image.width(); x++) {
    for (int y = 0; y < new_image.height(); y++) {
      for (int color = 0; color < 3; color++) {
        new_base_image(x, y, 0, color) =
            new_image(x, y, 0, color);
      }
    }
  }
  
  // and overlay our old base image onto the new image
  for (int x = 0; x < base_image.width(); x++) {
    for (int y = 0; y < base_image.height(); y++) {
      if ((base_image(x, y, 0, 0) != 0.0) ||
          (base_image(x, y, 0, 1) != 0.0) ||
          (base_image(x, y, 0, 2) != 0.0)) {

        for (int color = 0; color < 3; color++) {
          new_base_image(x, y, 0, color) =
              base_image(x, y, 0, color);
        }
      }
    }
  }

  /*  
  for (int x = left_side_overlap; x < right_side_overlap + 1; x++) {
    //printf("%d %d\n", x, (x - left_side_overlap));
    for (int y = 0; y < height; y++) {
      for (int color = 0; color < 3; color++) {
        new_base_image(x, y, 0, color) = 
            smoothed_overlap(x - left_side_overlap, y, 0, color);
      }
    }
  }
  */
  

  return new_base_image;
}



// Given to matrices, represented as vectors, multiply them together
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

  srand(unsigned(time(0)));

  try {

    string part = argv[1];
    
    if(part == "part1"){
      if (argc != 3) {
        printf("Program Usage ./a2 part1 [input_image]\n");
        exit(0);
      }
      transform_book();

      CImg<double> input_image;
      input_image.load(argv[2]);
      transform_lincolm();
      // Put the given input image on all 3 billboards
      imageOnBillboards(input_image);
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

      vector<CImg<double> > image1_gauss_pyramid =
          get_gaussian_pyramid(image1,
                               filter,
                               6);


      vector<CImg<double> > image2_gauss_pyramid =
          get_gaussian_pyramid(image2,
                               filter,
                               6);

      vector<CImg<double> > mask_gauss_pyramid =
          get_gaussian_pyramid(mask,
                               filter,
                               6);

      image1_gauss_pyramid[0].save("image_1_gauss_0.png");
      image1_gauss_pyramid[1].save("image_1_gauss_1.png");
      image1_gauss_pyramid[2].save("image_1_gauss_2.png");
      image1_gauss_pyramid[3].save("image_1_gauss_3.png");
      image1_gauss_pyramid[4].save("image_1_gauss_4.png");
      image1_gauss_pyramid[5].save("image_1_gauss_5.png");

      image2_gauss_pyramid[0].save("image_2_gauss_0.png");
      image2_gauss_pyramid[1].save("image_2_gauss_1.png");
      image2_gauss_pyramid[2].save("image_2_gauss_2.png");
      image2_gauss_pyramid[3].save("image_2_gauss_3.png");
      image2_gauss_pyramid[4].save("image_2_gauss_4.png");
      image2_gauss_pyramid[5].save("image_2_gauss_5.png");

      mask_gauss_pyramid[0].save("mask_gauss_0.png");
      mask_gauss_pyramid[1].save("mask_gauss_1.png");
      mask_gauss_pyramid[2].save("mask_gauss_2.png");
      mask_gauss_pyramid[3].save("mask_gauss_3.png");
      mask_gauss_pyramid[4].save("mask_gauss_4.png");
      mask_gauss_pyramid[5].save("mask_gauss_5.png");


      vector<CImg<double> > image1_lap_pyramid =
          get_laplacian_pyramid(image1_gauss_pyramid,
                                filter);

      vector<CImg<double> > image2_lap_pyramid =
           get_laplacian_pyramid(image2_gauss_pyramid,
                                filter);

      image1_lap_pyramid[0].save("image_1_lap_0.png");
      image1_lap_pyramid[1].save("image_1_lap_1.png");
      image1_lap_pyramid[2].save("image_1_lap_2.png");
      image1_lap_pyramid[3].save("image_1_lap_3.png");
      image1_lap_pyramid[4].save("image_1_lap_4.png");
      image1_lap_pyramid[5].save("image_1_lap_5.png");

      image2_lap_pyramid[0].save("image_2_lap_0.png");
      image2_lap_pyramid[1].save("image_2_lap_1.png");
      image2_lap_pyramid[2].save("image_2_lap_2.png");
      image2_lap_pyramid[3].save("image_2_lap_3.png");
      image2_lap_pyramid[4].save("image_2_lap_4.png");
      image2_lap_pyramid[5].save("image_2_lap_5.png");


      vector<CImg<double> > lap_blend_pyramid =
          get_laplacian_blend(image1_lap_pyramid,
                              image2_lap_pyramid,
                              mask_gauss_pyramid);


      lap_blend_pyramid[0].save("blend_0.png");
      lap_blend_pyramid[1].save("blend_1.png");
      lap_blend_pyramid[2].save("blend_2.png");
      lap_blend_pyramid[3].save("blend_4.png");
      lap_blend_pyramid[4].save("blend_5.png");
      lap_blend_pyramid[5].save("blend_6.png");

      CImg<double> final_blended = 
          get_blended_image(lap_blend_pyramid,
                            filter);

      final_blended.save("final_blended.png");

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
        printf("Program Usage ./a2 part4 [images]\n");
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

        if (all_matches.size() < 8) {
          printf("Not enough matches found\n");
          exit(0);
        }

        best_homography = 
          get_best_homography(transform_to_descriptors,
                              transform_from_descriptors,
                              all_matches,
                              inlier_dist_threshold,
                              inlier_ratio_threshold,
                              number_trials);

        if (best_homography.size() == 1) {
          printf("No good models found\n");
          exit(0);
        }

        current_transform = 
            matrix_multiply(best_homography,
                            current_transform);


        height_width =
            get_height_width(current_transform,
                             transform_from);

        transformed_image =
            transform_image(transform_from,
                            current_transform,
                            height_width[0],
                            height_width[1]);

        base_image = merge_images(base_image, transformed_image);

      }

      base_image.save("panorama.png");


    }
    
    
    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}
