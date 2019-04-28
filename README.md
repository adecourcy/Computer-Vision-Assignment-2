# A2 Report
#### Alex DeCourcy (adecourc) and Katie Spoon (kspoon) 

This was a class project. I have kept the portios of the readme that I completed, and removed the portions that my partner, Katie Spoon, worked on. The code can be found in the file a2.cpp. It wasn't my choice to keep all the code in this file, it was a class requirement.

## Part 3
Part 3 was implemented in the typical way. The only point of note in our RANSAC implementation is that with regards to the number of model trials, we did not consider a bad model to be a "trial". That is to say that if 10 sets of points are chosen, and 2 sets resulted in poor models, rejected by the algorithm, then the algorithm is only considered to have gone through 8 trials.

For sift, without RANSAC, as threshold value of 0.5 was used. This was chosen after trial and error with various values. This value seemed to give the best balance between rejecting poor points and accepting enough points to find correspondences.

For RANSAC, a sift threshold value of 0.5 was also used. We originally tested on more liberal values (0.7, 0.8, etc) with the idea to permit more initial matches at first, and allow RANSAC to filter out the ones that were truly bad. However, this seemed to create more problems than it solved.

An inlier distance threshold of 40 was used, an inlier ratio threshold of 0.5 was used, and 100 trials were used. More trials seemed to greatly improve RANSAC matches, especially in the case where there were very few good matches. There other parameters were chosen after trial and error. I don't have any intuitive explanation of why they work better than the others we tried, they just did.

Using RANSAC did seem to give noticably better correspondances, but they didn't seem much more than moderately better from my tests and examinations by eye. RANSAC also seemed to filter out good correspodences.

Our program attempts to output an error message when it is given 2 images with poor correspondances and it can't find any matches. However, there were a lot of edge cases to consider and there may be a few cases in which the program may crash if it is given 2 poorly matching images.

#### Big Ben with Sift:
![alt_text](sift%20-%20Big%20Ben.png)

#### Big Ben with RANSAC:
![alt_text](ransac%20-%20Big%20Ben.png)

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
![alt_text](panorama%20books%20-%20stitch.png)

TV:  
![alt_text](panorama%20TV%20-%20stitch.png)

#### Book panorama without smoothing:
![alt_text](panaroma%20books%20-%20no%20stitch.png)















