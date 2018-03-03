# a2


**PART 1**

Part 1 was rather difficult because there were some issues with the homography explanation and how CImg worked. However, after this was clarified it went rather well.

We did an inverse transformation in this part, using nearest-neighbor to fill in the empty gaps. This was done by rounding floating point x and y values to integers and using the rounded value in transformation.

Billboard transformation points were calculated manually. The billboard transformations worked well, but there were some issues with image distoration when certain images were overlayed on the billboard.

Homographies were calculated via the matrix in the Burger book.

**PART 2**

**PART 3**

Part 3 was implemented in the typical way. There isn't really much to comment on in terms of implementation.

For sift, without RANSAC, as threshold value of 0.5 was used. This was chosen after trial and error with various values. This value seemed to give the best balance between rejecting poor points and accepting enough points to find correspondences.

For RANSAC, a coarse threshold value of 0.5 was also used. We originally tested on more liberal values (0.7, 0.8, etc) with the idea to permit more initial matches at first, and allow RANSAC to filter out the ones that were truly bad. However, this seemed to create more problems than it solved.

An inlier distance threshold of 40 was used, an inlier ratio threshold of 0.5 was used, and 100 trials were used. More trials seemed to greatly improve RANSAC matches, especially in the case where there were very few good matches. There other parameters were chosen after trial and error. I don't have any intuitive explanation of why they work better than the others we tried, they just did.

Using RANSAC did seem to give noticably better correspondances, but they didn't seem much more than moderately better from my tests and examinations by eye.

Our program attempts to output an error message when it is given 2 images with poor correspondances and it can't find any matches. However, there were a lot of edge cases to consider and there may be a few cases in which the program may crash if it is given 2 poorly matching images.

**PART 4**

We leveraged our previous functions to build up our panorama.





















