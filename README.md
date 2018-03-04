# a2


**PART 1**

Part 1 was rather difficult because there were some issues with the homography explanation and how CImg worked. However, after this was clarified it went rather well.

We did an inverse transformation in this part, using nearest-neighbor to fill in the empty gaps. This was done by rounding floating point x and y values to integers and using the rounded value in transformation.

Billboard transformation points were calculated manually. The billboard transformations worked well, but there were some issues with image distoration when certain images were overlayed on the billboard.

Homographies were calculated via the matrix in the Burger book.

Billboard overlays:


**PART 2**

**PART 3**

Part 3 was implemented in the typical way. The only point of note in our RANSAC implementation is that with regards to the number of model trials, we did not consider a bad model to be a "trial". That is to say that if 10 sets of points are chosen, and 2 sets resulted in poor models, rejected by the algorithm, then the algorithm is only considered to have gone through 8 trials.

For sift, without RANSAC, as threshold value of 0.5 was used. This was chosen after trial and error with various values. This value seemed to give the best balance between rejecting poor points and accepting enough points to find correspondences.

For RANSAC, a sift threshold value of 0.5 was also used. We originally tested on more liberal values (0.7, 0.8, etc) with the idea to permit more initial matches at first, and allow RANSAC to filter out the ones that were truly bad. However, this seemed to create more problems than it solved.

An inlier distance threshold of 40 was used, an inlier ratio threshold of 0.5 was used, and 100 trials were used. More trials seemed to greatly improve RANSAC matches, especially in the case where there were very few good matches. There other parameters were chosen after trial and error. I don't have any intuitive explanation of why they work better than the others we tried, they just did.

Using RANSAC did seem to give noticably better correspondances, but they didn't seem much more than moderately better from my tests and examinations by eye. RANSAC also seemed to filter out good correspodences.

Our program attempts to output an error message when it is given 2 images with poor correspondances and it can't find any matches. However, there were a lot of edge cases to consider and there may be a few cases in which the program may crash if it is given 2 poorly matching images.

Big Ben with Sift:

Big Ben with RANSAC:


**PART 4**

We leveraged our previous functions to build up our panorama. We used the first image given by command-line argument as the "base" image, and transformed all other images to that coordinate system. We did this by multiplication of subsequent homographies. To be clear, a homography was created between image 1 and image 2, and image 2 was transformed with this homography. Next, a homography was created between the untransformed image 2 and image 3. This homography was multiplied with the previous homography, and image 3 was transformed with the resulting matrix. If there were more than 3 images, the procedure was continued as described. At each step, newly the transformed image and all previously transformed images were combined together.

For this part we used a sift threshold of 0.5, an inlier distance threshold of 10, and inlier ratio threshold of 0.7, and the number of trials as 100. Since we know the panorama images should have clear correspondences we increased the threshold to ensure we had very high-quality point matches. We also found that having a high number of trials was very important for finding a good homography.

As we started from the first image and moved to the last image (left to right), the last image would often be heavily skewed. However, the overlaps in the panorama were often very good, even before we introduced an alogrithm to smooth between the edges of the images.

Similar to part 3, the program attempts to give an error and exit gracefully when given poorly matched images, but it may not handle all cases and may crash given poorly matched images.

Book panorama without smoothing:

Book panorama with smoothing
















