 /***************************************
* Program 3 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

/* Function prototypes */
void callbackFunction(int, void *);

int main(int argc, char *argv[]) {

    /* Variable declarations */
    int position;

    /* Check to make sure a image and n value was given */
    if (argc == 1) {
      printf("You did not provide enough arguments. Ending program 3.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the input image from command line args */
    Mat image = imread(argv[1], CV_8UC1);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 3.\n");
      exit(EXIT_FAILURE);
    }

    /* Declare the image window */
    namedWindow("Original Image");
    imshow("Original Image", image);

    /* Declare the trackbar using the createTrackbar() fuction */
    createTrackbar("Original Image", "Smoothed Kernal Image", &position, 7, callbackFunction, &image);

    /* Display the image to the screen */
    namedWindow("Smoothed Kernal Image");
    imshow("Smoothed Kernal Image", image);

    /* Pause to admire image(s) */
    waitKey(0);

    /* End of program 3 */
    return 0;
}

/* Callback function for the trackbar */
void callbackFunction(int position, void *data) {

    /* Variable Declarations */
    double average = 0.0;
    int sum = 0;

    /* Check that the trackbar position is odd */
    if ((position % 2 == 0) || (position == 0)) {
      printf("Please enter an odd filter size.\n");
    } else {

      /* Declare the input image and the blurred image */
      Mat *image = (Mat*) data;
      Mat smoothedImage = (*image).clone();

      /* Create the kernel using the provided dimensions */
      int kernel[position][position];

      /* Calculate the midpoints of the width height */
      int midWidth = floor((position - 1) / 2);
      int midHeight = floor((position - 1) / 2);

      /* Default the kernal values */
      for (int i = 0; i < position; i++) {
        for (int j = 0; j < position; j++) {
          kernel[i][j] = 1;
        }
      }

      for (int r = 0; r < smoothedImage.rows; r++) {
        for (int c = 0; c < smoothedImage.cols; c++) {

          /* Apply kernel to all positions on the image */
          for (int j = 0; j < position; j++) {
            for (int i = 0; i < position; i++) {
              int x = (c + (i - midWidth));
              int y = (r + (j - midHeight));

              /* Check position of x, y on the image */
              if (x < 0)
                x = -(x) - 1;
              else if (x >= smoothedImage.cols)
                x = (2 * smoothedImage.cols) - x - 1;

              if (y < 0)
                y = -(y) - 1;
              else if (y >= smoothedImage.rows)
                y = (2 * smoothedImage.rows) - y - 1;

              /* Calculate the average and sum for this run */
              average += smoothedImage.at<uchar>(y, x);
              sum++;

            }
          }

          /* Apply averaging to the neighbooring pixels */
          average = (average / sum);
          smoothedImage.at<uchar>(r, c) = (int)(average);

          /* Reset the sum and average values for the next loop */
          sum = 0;
          average = 0.0;

          for (int i = 0; i < 7; i++){
            for (int j = 0; j < 7; j++){
              kernel[i][j] = kernel[i + 1][j];
            }
          }
        }
      }

      /* Display the kernal image to the screen */
      namedWindow("Smoothed Kernal Image");
      imshow("Smoothed Kernal Image", smoothedImage);
    }

} /* End of callback function*/


/********** Sources **************
https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
https://docs.opencv.org/3.1.0/d4/d13/tutorial_py_filtering.html
https://www.tutorialspoint.com/opencv/opencv_filter2d.htm
http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html
*********************************/
