 /***************************************
* Program 5 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

/* Function prototypes */
void callbackFunction(int, void *);

int main(int argc, char *argv[]) {

    /* Variable declarations */
    int position;

    /* Check to make sure an image and n value was given */
    if (argc == 1) {
      printf("You did not provide enough arguments. Ending program 5.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the input image from command line args - CV_LOAD_IMAGE_COLOR / CV_8UC1 */
    Mat image = imread(argv[1], CV_8UC1);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 5.\n");
      exit(EXIT_FAILURE);
    }

    /* Calculate the max value of n */
    int maxPostion = max(image.cols, image.rows);

    /* Declare the final pastel image window */
    namedWindow("Pastel Image");

    /* Declare the trackbar using the createTrackbar() fuction */
    createTrackbar("Trackbar", "Pastel Image", &position, maxPostion, callbackFunction, &image);

    /* Display the final pastel image */
    imshow("Pastel Image", image);

    /* Pause to admire image(s) */
    waitKey(0);

    /* End of program 5 */
    return 0;
}

/* Callback function for the trackbar */
void callbackFunction(int position, void *inputImage) {

    /* Declare the image to be blurred */
    Mat *image = (Mat*)inputImage;
    Mat pastelImage = image->clone();

    /* Declare the boundaries of the iamge */
    int topBoundry = position;
    int bottomBoundry = pastelImage.rows - position;
    int leftBoundry = position;
    int rightBoundry = pastelImage.cols - position;

    /* Apply the pastel blur to the image */
    for (int i = 0; i < pastelImage.rows; i++) {
      for (int j = 0; j < pastelImage.cols; j++) {

        double distance;

        /* Turn boundary pixels white */
        if (((j + 1) == pastelImage.cols) || (j == 0))
          pastelImage.at<uchar>(i, j) = 255;

        if (((i + 1) == pastelImage.rows) || (i == 0))
          pastelImage.at<uchar>(i, j) = 255;

        /* Apply the pastel color to all internal pixels */
        if (j < leftBoundry) {
          distance = findDistance(Point(j, i), Point(0, i));
          pastelImage.at<uchar>(i, j) = findPastelValue(position, pastelImage.at<uchar>(i, j), distance);
        } else if (j > rightBoundry) {
          distance = findDistance(Point(j, i), Point(pastelImage.cols, i));
          pastelImage.at<uchar>(i, j) = findPastelValue(position, pastelImage.at<uchar>(i, j), distance);
        }

        if (i < topBoundry) {
          distance = findDistance(Point(j, 0), Point(j, i));
          pastelImage.at<uchar>(i, j) = findPastelValue(position, pastelImage.at<uchar>(i, j), distance);
        } else if (i > bottomBoundry) {
          distance = findDistance(Point(j, i), Point(j, pastelImage.rows));
          pastelImage.at<uchar>(i, j) = findPastelValue(position, pastelImage.at<uchar>(i, j), distance);
        }
      }
    }

    /* Display the blurred image to the screen */
    imshow("Pastel Image", pastelImage);
  }

/* Function to find the pastel value of a pixel */
int findPastelValue(int position, int pixel, double distance) {
    double pastel = (double)(pixel) + ((((double)position - distance) / (double)position) * double((255 - pixel)));
    return (int)pastel;
}

/* Function to calculate the distance of 2 points */
double findDistance(Point pos1, Point pos2) {
    double x = pow((pos1.x - pos2.x), 2);
    double y = pow((pos1.y - pos2.y), 2);
    return sqrt(x + y);
}
