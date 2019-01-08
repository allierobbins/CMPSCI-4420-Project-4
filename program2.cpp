 /***************************************
* Program 2 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

int main(int argc, char *argv[]) {

    /* Variable declarations */
    int option;
    int zoom, centerRow, centerColumn, height, width;
    int z = 0, r = 0, c = 0, h = 0, w = 0;

    /* Check to make sure all needed values were given and are positive */
    while ((option = getopt(argc, argv, "z:R:C:H:W:")) != -1) {
      switch (option) {
        case 'z':
            zoom = atoi(optarg);
            if (zoom <= 0) {
              printf("Zoom factor cannot be negative. Ending program 2.\n");
              exit(EXIT_FAILURE);
            } else {
              z = 1;
            }
            break;
        case 'R':
            centerRow = atoi(optarg);
            if (centerRow <= 0) {
              printf("R cannot be negative. Ending program 2.\n");
              exit(EXIT_FAILURE);
            } else {
              r = 1;
            }
            break;
        case 'C':
            centerColumn = atoi(optarg);
            if (centerColumn <= 0) {
              printf("C cannot be negative. Ending program 2.\n");
              exit(EXIT_FAILURE);
            } else {
              c = 1;
            }
            break;
        case 'H':
            height = atoi(optarg);
            if (height <= 0) {
              printf("H cannot be negative. Ending program 2.\n");
              exit(EXIT_FAILURE);
            } else {
              h = 1;
            }
            break;
        case 'W':
            width = atoi(optarg);
            if (width <= 0) {
              printf("W cannot be negative. Ending program 2.\n");
              exit(EXIT_FAILURE);
            } else {
              w = 1;
            }
            break;
        default:
          printf("You did not provide enough arguments. Ending program 2.\n");
          exit(EXIT_FAILURE);
      }
    }

    /* If no command arguments were provided */
    if ((option == -1) && (argc == 1)) {
      printf("You did not provide enough arguments. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }

    /* If not all the arguments were provided */
    if ((z == 0) || (r == 0) || (c == 0) || (h == 0) || (w == 0)) {
        printf("One of the values was either not entered or negative. Ending program 2.\n");
        exit(EXIT_FAILURE);
    }

    /* If too few command arguments were provided */
    if (argc < 13) {
      printf("You did not provide enough arguments. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the input image from command line args */
    Mat image = imread(argv[optind], CV_8UC1);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the output image name from the command line */
    string outputImageName = argv[optind + 1];

    /* Declare the zoomed image based on the given arg. values */
    Mat zoomedImage(height, width, CV_8UC1);

    /* Declare the start/stop positions of the zoomed window s*/
    int zStartRow = centerRow - (height / 2);
    int zStartColumn = centerColumn - (width / 2);
    int zStopRow = centerRow + (height / 2);
    int zStopColumn = centerColumn + (width / 2);

    /* Check for valid values */
    if (zoom > max(image.rows, image.cols)) {
      printf("Zoom window cannot be bigger than the image. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }
    if ((zStartRow < 0) || (zStartColumn < 0)) {
      printf("The zoom window must fall within the input image. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }
    if ((height > image.rows) || (width > image.cols)) {
      printf("The zoom window must be smaller than the input image. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }
    if ((zStopRow > image.rows) || (zStopColumn > image.cols)) {
      printf("The zoom window must expand past the input image. Ending program 2.\n");
      exit(EXIT_FAILURE);
    }

    /* Declare the starting positions of the zoomed image */
    int startRowPosition = centerRow - ((height / 2) / zoom);
    int startColumnPosition = centerColumn - ((width / 2) / zoom);

    /* Declare the center values R and C */
    int centerR = (zoom / 2);
    int centerC = (zoom / 2);

    /* Declare the maximum row and column that we will zoom to */
    int maxRow = centerRow + ((height / 2) / zoom);
    int maxColumn = centerColumn + ((width / 2) / zoom);


    /* Zoom into the specified pixel */
    for (int rows = startRowPosition; rows < maxRow; rows++) {
      for (int cols = startColumnPosition; cols < maxColumn; cols++) {
        for (int i = 0; i < zoom; i++) {
          for (int j = 0; j < zoom; j++) {
            zoomedImage.at<uchar>(centerR + (i - (zoom / 2)), centerC + (j - (zoom / 2))) = image.at<uchar>(rows, cols);
          }
        }
        centerC += zoom;
      }
      centerC = (zoom / 2);
      centerR += zoom;
    }

    /* Return to the starting positions */
    centerR = startRowPosition;
    centerC = startColumnPosition;

    /* Transfer the pixel of the zoomed image to the position in the orig. image */
    for (int rows = 0; rows < zoomedImage.rows; rows++) {
      for (int cols = 0; cols < zoomedImage.cols; cols++) {
        if ((centerR < image.rows) && (centerC < image.cols)) {
          image.at<uchar>(centerR, centerC) = zoomedImage.at<uchar>(rows, cols);
          centerC++;
        }
      }
      /* Return to the starting column position */
      centerC = startColumnPosition;
      centerR++;
    }

    /* Display the image to the screen */
    namedWindow("Zoomed Area Image");
    imshow("Zoomed Area Image", image);

    /* Pause to admire image */
    waitKey(0);

    /* End of program 2 */
    return 0;
  }
