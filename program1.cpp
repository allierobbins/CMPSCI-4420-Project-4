/***************************************
* Program 1 for 4420 Assignment 4      *
* Author: Allie Robbins                *
* Language: C++ with OpenCV libraries  *
***************************************/

#include "assignment4.h"

int main(int argc, char *argv[]) {

    /* Variable declarations */
    double distance;
    int pixel;
    int row;
    int column;
    int bit;

    /* Check to make sure an image and a n value was given */
    if (argc <=2 ) {
      printf("You did not provide enough arguments. Ending program1.\n");
      exit(EXIT_FAILURE);
    }

    /* Assign the input image from command line args */
    Mat image = imread(argv[1], CV_8UC1);

    /* Declare the final "added" image according to the dim. of the orig. image*/
    Mat addedImage = Mat::zeros(image.rows, image.cols, CV_8UC1);

    /* Declare the 10 resulting images using a vector of type cv::Mat */
    vector<Mat> tenIntermediateImages (10);
    vector<string> intermediateImageName (10, argv[1]);

    /* Get the n% of bits to be flipped from the command line */
    int flipNBits = strtol(argv[2], NULL, 10);

    /* Declare variable to hold the total intensitites for each int. image */
    vector<int> totalIntenstity((image.rows * image.cols), 0);

    /* Make sure the images can be found/opened */
    if (image.empty()) {
      printf("There was an error opening the image. Ending program 1.\n");
      exit(EXIT_FAILURE);
    }

    /* printf("Percentage of bits to be flipped is: %d", flipNBits); */

    /* Calculate the number of bits to flip in the image */
    int numOfBitsToFlip = round(((image.rows * image.cols) * 8) * ((double)flipNBits / (double)100));
    string imageName = argv[1];
    size_t found = imageName.find_last_of(".");

    /* Declare the random # generater using srand */
    srand(time(NULL));

    /* Create 10 images with added noise based on Alg. 5.10 */
    for (int iter = 0; iter < 10; iter++) {

        /* Declare the noise image*/
        tenIntermediateImages.at(iter) = image.clone();

        /* Nameing Convention for the 10 images */
        string imageNum = "0";
        stringstream stringName;
        stringName << iter;
        imageNum.append(stringName.str());
        intermediateImageName.at(iter).insert(found, imageNum);

        /* Randomly flip the bit valies for the 10 noise images */
        for (int i = 0; i <= numOfBitsToFlip; i++) {

            /* Randomly choose a pixel value from a row/column */
            getPixel(pixel, row, column, tenIntermediateImages.at(iter));

            /* Seperate the pixel into its 8 bits */
            bitset<8> pixelBitValues(pixel);

            /* Randomly select which bit of the pixel to flip */
            bit = rand() % (7 + 0);

            /* Flip the random bit using */
            pixelBitValues.flip(bit);

            /* Get the value of the bitset */
            int pixelValue = (int)pixelBitValues.to_ulong();

            /* Rerwite the pixel value on the noiseImage */
            tenIntermediateImages.at(iter).at<uchar>(row, column) = pixelValue;
        }

        /* Write the noise image that was created */
        imwrite(intermediateImageName.at(iter).c_str(), tenIntermediateImages.at(iter));

        /* Calculate the sum of the nosie images */
        int i = 0;
        for (int r = 0; r < tenIntermediateImages.at(iter).rows; r++) {
          for (int c = 0; c < tenIntermediateImages.at(iter).cols; c++) {
            totalIntenstity[i] += tenIntermediateImages.at(iter).at<uchar>(r, c);
            i++;
          }
        }
    }

    /* Calculate the avg. intensity for each image */
    for (int i = 0; i < (image.rows * image.cols); i++) {
      totalIntenstity[i] /= 10;
    }

    /* Create the noisy image from the int. images */
    int i = 0;
    for (int r = 0; r < image.rows; r++) {
      for (int c = 0; c < image.cols; c++) {
        //Set the pixel at (r, c) in the integrated image to the pixel
        //intensity sum obtained above
        addedImage.at<uchar>(r, c) = totalIntenstity[i];
        i++;
      }
    }

    /* Calculate the distance of the added image from the original image*/
    distance = norm(image, addedImage, NORM_L2);
    printf("The distance of the added image from the original image is %d\n", (int)distance);

    /* Display the original image to the screen */
    namedWindow("Original Image");
    imshow("Original Image", image);

    /* Display the 10 noise-generated images */
    for (int i = 0; i < 10; i++) {
      printImage(tenIntermediateImages.at(i), intermediateImageName.at(i).c_str());
    }

    /* Display the added image to the screen */
    namedWindow("Added Gaussian Noise Image");
    imshow("Added Gaussian Noise Image", addedImage);

    /* Pause to admire image(s) */
    waitKey(0);

    /* End of program1 */
    return 0;
}

void getPixel(int &pixel, int &row, int &column, Mat image) {
  row = rand() % (image.rows + 0);
  column = rand() % (image.cols + 0);
  pixel = image.at<uchar>(row, column);
}

void printImage(Mat image, const char *name) {
  namedWindow(name);
  imshow(name, image);
}

/*************Internet Sources*************
1. https://stackoverflow.com/questions/32889309/adding-gaussian-noise
2. https://cboard.cprogramming.com/c-programming/117223-gaussian-noise-generator.html?s=52571db251d5de8715a78ceaebce5b01
3. http://lazyfoo.net/SDL_tutorials/lesson31/index.php
4. https://docs.opencv.org/3.0-beta/modules/imagecodecs/doc/reading_and_writing_images.html
5. http://www.cplusplus.com/reference/complex/norm/
******************************************/
