/***************************************
* Program 4 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

int main(int argc, char *argv[]) {

    /* Variable declarations */
    int size = 0, resolution = 0;
    double sum = 0, total = 0, runningSum;

    /* Check to make sure the correct # of args were given */
    if (argc < 4) {
      printf("You did not provide enough arguments. Ending program 4.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the image from the command line args */
    Mat image = imread(argv[1], CV_8UC1);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 4.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the specified size and resolution */
    size = strtol(argv[2], NULL, 10);
    resolution = strtol(argv[3], NULL, 10);

    /* Specify the resolution as a power of 2 */
    resolution = (int)pow(2, resolution);

    /* Declare the center of the kernal */
    int center = floor((0.5 * (size - 1)));

    /* Declare the kernal */
    double kernel[size][size];

    /* Calculate the distance and kernal values */
    for (int r = 0; r < size; r++) {
      for (int c = 0; c < size; c++) {

        double distance = (1 / (2 * PI * pow(resolution, 2))) * exp(-(pow(r - size / 2, 2) + pow(c - size / 2, 2)) / (2 * pow(resolution, 2)));
        kernel[r][c] = (distance * resolution);

        sum += kernel[r][c];
      }
    }

    /* Normalize the kernel */
    for (int r = 0; r < size; r++) {
      for (int c = 0; c < size; c++) {
        kernel[r][c] = (kernel[r][c] / sum);
        total += (kernel[r][c] * resolution);
      }
    }

    /* Clone the blurred image from the original image */
    Mat blurredImage = image.clone();

    /* Apply kernel to image based on Alg. 5.1, loop through the pixels of the image */
    for (int r = 0; r < image.rows; r++) {
      for (int c = 0; c < image.cols; c++) {

        /* Reset the running sum at the beginning of a pixel */
        runningSum = 0.0;

        /* Loop through the kernal */
        for (int i = 0; i < size; i++) {
          for (int j = 0; j < size; j++) {
            int y = (r + center) - i;
            int x = (c + center) - j;

            /* Reflect any boundary pixels */
            reflect(y, 0, image.rows);
            reflect(x, 0, image.cols);

            /* Increment the running sum for the blurred image */
            runningSum += kernel[j][i] * image.at<uchar>(y, x);
          }
        }

        /* Assign the blurred pixel to the final image */
        blurredImage.at<uchar>(r, c) = runningSum;
      }
    }

    /* Display the kernal image to the screen */
    namedWindow("Blurred Image");
    imshow("Blurred Image", blurredImage);

    /* Pause to admire image */
    waitKey(0);

    /* End of program 4 */
    return 0;
  }

void reflect(int &position, int min, int max) {
  /* Locate and negate any boundary pixels */
  if (position < 0)
    position = -(position) - 1;
  else if (position >= max)
    position = (2 * max) - position - 1;
}
