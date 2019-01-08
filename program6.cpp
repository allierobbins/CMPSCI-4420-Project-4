 /***************************************
* Program 6 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

int main(int argc, char *argv[]) {

    /* Variable Declarations */
    double mean = 0.0;
    int rowIndex = 0, columnIndex = 0;
    int width, height;


    /* Check to make sure an image and a n value was given */
    if (argc == 1) {
      printf("You did not provide enough arguments. Ending program 6.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the input image from command line args */
    Mat image = imread(argv[1], CV_8UC1);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 6.\n");
      exit(EXIT_FAILURE);
    }

    /* Declare the binary image */
    Mat binaryImage = Mat::zeros(image.rows, image.cols, CV_8UC1);

    /* Declare the dilated & eroded images */
    Mat dialatedImage = Mat::zeros(image.rows, image.cols, CV_8UC1);
    Mat erodedImage = Mat::zeros(image.rows, image.cols, CV_8UC1);

    /* Intialize the values of the dilated and eroded images */
    for (int rows = 0; rows < dialatedImage.rows; rows++) {
      for (int cols = 0; cols < dialatedImage.cols; cols++) {
        dialatedImage.at<uchar>(rows, cols) = 0;
        erodedImage.at<uchar>(rows, cols) = 255;
      }
    }

    /* Calculate the image mean */
    for (int rows = 0; rows < image.rows; rows++) {
      for (int cols = 0; cols < image.cols; cols++) {
        //Add each pixel intensity value to the mean.
        mean += image.at<uchar>(rows, cols);
      }
    }
    mean = (mean / (double)(image.cols * image.rows));

    /* Threshold the binary image with the input image's mean */
    threshold(image, binaryImage, mean, 255, THRESH_BINARY);

    /* Get the Structuring Element from the file */
    ifstream fileName("structuringElement.txt");
    string line;
    stringstream stringValue;

    /* Read width / height from the file */
    if (fileName.is_open()) {

      /* Read line seperated w/ a line */
      getline(fileName, line);
      size_t space = line.find_first_of(" ");

      if (space != string::npos) {
        stringValue << line.substr(0, space);

        /* Set the width */
        stringValue >> width;
        stringValue.clear();
        stringValue.str("");

        /* Set the height */
        stringValue << line.substr(space, line.length());
        stringValue >> height;

        //Clear the stringstream contents
        stringValue.clear();
        stringValue.str("");
      } else {
        printf("There was an error getting the width/height of the SE. Ending program 6.\n");
        exit(EXIT_FAILURE);
      }
    }

    /* Validate SE */
    if ((width > image.cols) || (width < 1)) {
      printf("Invalid Structuring Element. Ending program 6.\n");
      exit(EXIT_FAILURE);
    }

    if ((height > image.rows) || (height < 1)) {
      printf("Invalid Structuring Element. Ending program 6.\n");
      exit(EXIT_FAILURE);
    }

    /* Declare the Structuring Element (SE) */
    int SE[height][width];

    /* Read in the values of the SE */
    while (getline(fileName, line)) {
      stringValue << line;

      /* Store the values in the SE array */
      if (rowIndex < height) {
        while (columnIndex < width) {
          stringValue >> SE[rowIndex][columnIndex];

          //Check if the value in the file is a '0' or '1'; if not, print
          //an error and the invalid value, then terminate.
          if ((SE[rowIndex][columnIndex] != 0) && (SE[rowIndex][columnIndex] != 1)) {
            printf("Invalid SE element. Ending program 6.\n");
            exit(EXIT_FAILURE);
          }
          columnIndex++;
        }

        /* Reset loop variables */
        columnIndex = 0;
        rowIndex++;
        stringValue.clear();
        stringValue.str("");

      } else
        break;
    }

    /* Perform dilation w/ SE - Alg. 4.3 */
    for (int rows = 0; rows < binaryImage.rows; rows++) {
      for (int cols = 0; cols < binaryImage.cols; cols++) {

        /* Intialize the pixel value to 0 */
        int value = 0;

        /* Loop through the SE */
        for (int r = 0; r < height; r++) {
          for (int c = 0; c < width; c++) {
            int x = cols - c + floor(width / 2);
            int y = rows - r + floor(height / 2);
            int pixel = binaryImage.at<uchar>(y, x);

            /* If the SE overlaps with the image, set the pixel to 255 */
            if ((SE[r][c] == 1) && (pixel == 255))
              value = 255;
          }
        }

        /* Assign the pixel to the dilated image */
        dialatedImage.at<uchar>(rows, cols) = value;
      }
    }

    /* Perform erosion w/ SE - Alg. 4.3 */
    for (int rows = 0; rows < binaryImage.rows; rows++) {
      for (int cols = 0; cols < binaryImage.cols; cols++) {

        /* Intialize the pixel value to 255 */
        int all = 255;

        /* Loop through the SE */
        for (int r = 0; r < height; r++) {
          for (int c = 0; c < width; c++) {
            int x = cols + c - floor(width / 2);
            int y = rows + r - floor(height / 2);
            int pixel = binaryImage.at<uchar>(y, x);

            /* If the SE overlaps with the image, set the pixel to 0 */
            if ((SE[r][c] == 1) && (pixel != 255))
              all = 0;
          }
        }

        /* Assign the pixel to the eroded image */
        erodedImage.at<uchar>(rows, cols) = all;
      }
    }

    /* Display the dialated image to the screen */
    namedWindow("Dialated Image");
    imshow("Dialated Image", dialatedImage);

    /* Display the eroded image to the screen */
    namedWindow("Eroded Image");
    imshow("Eroded Image", erodedImage);

    /* Pause to admire image(s) */
    waitKey(0);

    /* End of program 6 */
    return 0;
}

/********** Sources **************
https://docs.opencv.org/2.4/doc/tutorials/imgproc/threshold/threshold.html
https://docs.opencv.org/2.4/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html
https://stackoverflow.com/questions/14516915/read-numeric-data-from-a-text-file-in-c
https://www.geeksforgeeks.org/cpp-program-read-file-word-word/
*********************************/
