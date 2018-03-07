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
We had a lot of issues with Part 2 as well, due to misunderstanding some things about CImg and problems with the assignment instructions. However, once this was worked out we generated the Gaussian and Laplacian pyramids for each image, the Gaussian for the mask, the blended Laplacian pyramid, and finally the finished, blended image. 

The steps we followed are as follows:
1. We created the Gaussian and Laplacian pyramids for each image, as well as the Gaussian pyramid for the mask.
2. We then created the blended Laplacian pyramid, using the equation from the PDF to weight the images based on the mask values.
3. After that, we simply (not simple!!) combined the layers of the Laplacian pyramid to get our final blended image.

We had a lot of trouble completing Part 2 correctly because the instructions in the PDF weren't exceptionally specific and allowed for a lot of room for interpretation. We found that even very small changes to the algorithm implementation resulted in large problems with the end result, and it would have helped to have more detailed instructions for this, or to simply be left on our own to find a working solution with more specific details. Most other groups we talked with about this problem suffered similar issues with implementing the part of the assignment, and ended up constructing a series of haphazard hacks that eventually (somehow) gave them an acceptable result. We, too, attempted our own series of hacks, in an unsuccessful attempt to fix our problem. Ultimately, we ended up basing our code off of the computer vision lab python implementation (https://compvisionlab.wordpress.com/2013/05/13/image-blending-using-pyramid/) which gave us enough specific direction to make a base working model of the blending. After that, we were able to make a few minor modifications that seemed to work better in part 4 of the assignment.

It would have been helpful to have example images for what our Gaussians, Laplacians, and Blended levels should've looked like, because we spent a long time trying to figure out if our Gaussians and Laplacians looked like they should've. We spent at least 30 hours (and that is a modest estimate - it was probably more) working on Part 2 alone, mostly due to the unclear instructions. After referencing the computer vision lab code, it took us less than 3 hours to get a working implementation. To be specific, a few of the issues we encountered when implementing this algorithm off of the PDF instructions (the last of which is really what the reference python code helped us with) are as follows:

1) The fact that the mask image was only a single color layer was not communicated. This was not obivous, trivial, or readily apparent. Since all our other images were 3 layers, we started under the assumption that the mask image also had 3 layers. This lead to a huge number of problems when trying to create our laplacian blends, and a lot of time wasted on debugging.
2) The fact that the mask image had values of 0 or 255 and needed to be normalized was also not communicated. This created few problems than 1) but it still would have been helpful to know this before hand. As it isn't unusual in the graphics world for RGB colors to run on a scale of 0-1 this, again, wasn't immeadiately obvious and caused some problems.
3) We found that upscaling had to be applied in a *very* specific way in order to get the blending to work properly, and this was probably one of biggest issues we had in troubleshoot our issues with part 2. In particular, we found that, when construction the laplacian pyramid, you may either upscale by using interpolation during resizing and then applying the filter gaussian filter given in the assignment, or by filling every other row and column with 0s, and then applying 4 times the gaussian filter given in the assignement. We found that trying to calculate the laplacian by substracting the same image, smoothed with the gaussian given in the assignment, or by setting every other row to 0 in the upscale and smoothing with the filter (not 4 times the filter) would creat *huge* problems when trying create the final blend. This was primary issue the computer vision lab code helped us diagnose.

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

And here is the final result:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/blended%20-%20good.png)


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

#### Book panorama with smoothing
We attempted smooting on the panorama, and made a number of changes to the smoothing functions in attempt to make it better, but ultimately abandoned the idea and left the images without smoothing as they actually seemed to look better. We ran into 3 particular issues that led to this decision:

1) Even when we had really good smoothing results, there was some very odd and unpleasant image distoration. An example of this can be seen between the first and second images on the TV panorama, as well as on the yoga book between the second and third images on the book panorama. Rather than having an obivous, sharp border where 1 image ends and the other begins, we have a strange transition where objects fade in and out, or bend strangely where the images meet. Given this, smoothing didn't real seem to much improve the effect of having a single panoramic image.
2) We had some inconsistent results we attempting to perform smoothing between the images. Many times it would work well but on occasion, we would end up with a very color-distorted blend. The behavior was rather inconsistent and I believed it was ultimately due to the fact that images will have more or less black space on their overlapping sections depending on how RANSAC aligns them. If they have too much, the end result has some strange color effects. We tried to compensate for this by removing negative color values on the final blend and then normalizing. For the most part, that fixed the issue with the color effects, but we would still get distortion on occasional test runs. This also led to issue 3.
3) After blending, the colors on the overlapping sections rarely matched well with the images on either side of them. They were quite a bit darker and made it seem like they were added in after the fact (which they were). As mentioned in part 2, we believe this was due to the fact that there are often a lot of black pixels in the overlap area.

Here are our results:  
Books:  
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/panorama%20books%20-%20stitch.png)

TV:  
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/panorama%20TV%20-%20stitch.png)

#### Book panorama without smoothing:
![alt_text](https://github.iu.edu/cs-b657-sp2018/adecourc-kspoon-a2/blob/master/panaroma%20books%20-%20no%20stitch.png)















