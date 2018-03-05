# A2 Report
#### Alex DeCourcy (adecourc) and Katie Spoon (kspoon) 


## Part 1
Part 1 was rather difficult because there were some issues with the homography explanation and how CImg worked. However, after this was clarified it went rather well.

We did an inverse transformation in this part, using nearest-neighbor to fill in the empty gaps. This was done by rounding floating point x and y values to integers and using the rounded value in transformation.

Billboard transformation points were calculated manually. The billboard transformations worked well, but there were some issues with image distoration when certain images were overlayed on the billboard.

Homographies were calculated via the matrix in the Burger book.

#### Billboard overlays:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/overlay_1.jpg)

![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/overlay_2.jpg)

![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/overlay_3.jpg)

## Part 2
We found Part 2 incredibly difficult as well, mostly due to misunderstanding some things about CImg. However, once this was worked out we generated the Gaussian and Laplacian pyramids for each image, the Gaussian for the mask, and then the blended Laplacian pyramid. 

The steps we followed are as follows:
1. We created the Gaussian and Laplacian pyramids for each image, as well as the Gaussian pyramid for the mask.
2. We then created the blended Laplacian pyramid, using the equation from the PDF to weight the images based on the mask values.
3. After that, we simply (not simple!!) combined the layers of the Laplacian pyramid to get our final blended image.

This part was not difficult, however we were not given enough information to complete the problem correctly. Examples of some of the blended images we generated can be found below, but we didn't end up getting the final correct blended image. We had to construct a series of hacks for this problem (normalizing several times, somewhat randomly, multiplying the convolution kernel by 4, dealing with issues in upscaling and downscaling) that were not mentioned in the assignment, causing us to not be able to find where the issue in the series of hacks was. We also tried two different methods of upscaling and downscaling: 1) using CImg's resize() method, and 2) creating our own upscale/downscale, and neither of those worked.

It would have been helpful to have example images for what our Gaussians, Laplacians, and Blended levels should've looked like, because we spent a long time trying to figure out if our Gaussians and Laplacians looked like they should've. 

### Example: Apple and Orange, using the given mask
#### Apple Gaussians:
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/images/part2/apple.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_G1.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_G2.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_G3.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_G4.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_G5.jpg)

#### Apple Laplacians:
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L0.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L1.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L2.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L3.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L4.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image1_L5.jpg)

#### Orange Gaussians:
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/images/part2/orange.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_G1.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_G2.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_G3.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_G4.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_G5.jpg)

#### Orange Laplacians:
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L0.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L1.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L2.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L3.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L4.jpg)

![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/image2_L5.jpg)
 
We had a tough time producing the blended result. Here are some of the bad results we got:  
Here's a pretty one:  
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/result-images/final_blended_mistake1.jpg)

This result was closer, but blurrier:  
![alt text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/blend_try1.png)

#### Blended Result:


## Part 3
Part 3 was implemented in the typical way. The only point of note in our RANSAC implementation is that with regards to the number of model trials, we did not consider a bad model to be a "trial". That is to say that if 10 sets of points are chosen, and 2 sets resulted in poor models, rejected by the algorithm, then the algorithm is only considered to have gone through 8 trials.

For sift, without RANSAC, as threshold value of 0.5 was used. This was chosen after trial and error with various values. This value seemed to give the best balance between rejecting poor points and accepting enough points to find correspondences.

For RANSAC, a sift threshold value of 0.5 was also used. We originally tested on more liberal values (0.7, 0.8, etc) with the idea to permit more initial matches at first, and allow RANSAC to filter out the ones that were truly bad. However, this seemed to create more problems than it solved.

An inlier distance threshold of 40 was used, an inlier ratio threshold of 0.5 was used, and 100 trials were used. More trials seemed to greatly improve RANSAC matches, especially in the case where there were very few good matches. There other parameters were chosen after trial and error. I don't have any intuitive explanation of why they work better than the others we tried, they just did.

Using RANSAC did seem to give noticably better correspondances, but they didn't seem much more than moderately better from my tests and examinations by eye. RANSAC also seemed to filter out good correspodences.

Our program attempts to output an error message when it is given 2 images with poor correspondances and it can't find any matches. However, there were a lot of edge cases to consider and there may be a few cases in which the program may crash if it is given 2 poorly matching images.

#### Big Ben with Sift:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/sift%20-%20Big%20Ben.png)

#### Big Ben with RANSAC:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/ransac%20-%20Big%20Ben.png)

## Part 4
We leveraged our previous functions to build up our panorama. We used the first image given by command-line argument as the "base" image, and transformed all other images to that coordinate system. We did this by multiplication of subsequent homographies. To be clear, a homography was created between image 1 and image 2, and image 2 was transformed with this homography. Next, a homography was created between the untransformed image 2 and image 3. This homography was multiplied with the previous homography, and image 3 was transformed with the resulting matrix. If there were more than 3 images, the procedure was continued as described. At each step, newly the transformed image and all previously transformed images were combined together.

For this part we used a sift threshold of 0.5, an inlier distance threshold of 10, and inlier ratio threshold of 0.7, and the number of trials as 100. Since we know the panorama images should have clear correspondences we increased the threshold to ensure we had very high-quality point matches. We also found that having a high number of trials was very important for finding a good homography.

As we started from the first image and moved to the last image (left to right), the last image would often be heavily skewed. However, the overlaps in the panorama were often very good, even before we introduced an alogrithm to smooth between the edges of the images.

Similar to part 3, the program attempts to give an error and exit gracefully when given poorly matched images, but it may not handle all cases and may crash given poorly matched images.

#### Book panorama without smoothing:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/panaroma%20books%20-%20no%20stitch.png)

#### Book panorama with smoothing
We had a lot of issues with Part 2 so we didn't have a chance to add smoothing to this part. 















