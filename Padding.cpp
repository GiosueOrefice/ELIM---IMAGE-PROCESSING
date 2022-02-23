#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define MYBORDER_CONSTANT 0
#define MYBORDER_WRAP 1
#define MYBORDER_REFLECT 2
#define MYBORDER_REPLICATE  3


void paddingImage(Mat& source, Mat& dest, int top, int bottom, int left, int right, int borderType);
void borderCostant(Mat& source, Mat& dest, int left, int top, int right, int bottom);
void borderWrap(Mat& source, Mat& dest, int left, int top, int right, int bottom);
void borderReflect(Mat& source, Mat& dest, int left, int top, int right, int bottom);
void borderReplicate(Mat& source, Mat& dest, int left, int top, int right, int bottom);

int main(int argc, char** argv) {
    String imageName = "messi5.jpg";
    Mat imageSource,imageDest;
    Mat imageCopyMakeBorder;

    int cont = 0;
    int left, top, right, bottom;
    
    imageSource = imread(imageName, IMREAD_GRAYSCALE);
    
    if (imageSource.empty()){
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    left = 50;
    top = 70;
    right = 80;
    bottom = 40;
    paddingImage(imageSource, imageDest, top, bottom, left, right, MYBORDER_REPLICATE);
    copyMakeBorder(imageSource, imageCopyMakeBorder, top, bottom, left, right, BORDER_REPLICATE);

    //CHECK VALORI DI INTENSITA' UGUALE
    for (int i = 0; i < imageDest.rows; i++) {
        for (int j = 0; j < imageDest.cols; j++) {
            if (imageDest.at<unsigned char>(i, j) != imageCopyMakeBorder.at<unsigned char>(i, j))
                cont++;
        }
    }

    cout << "NUMERO PIXEL DIVERSI TRA LE DUE IMMAGINI "<<cont << endl;

    imshow("MY FUNCTION", imageDest);
    imshow("ORIGINAL FUNCTION", imageCopyMakeBorder);
    imwrite("test.png", imageDest);
    waitKey(0);

    return 0;
}


void paddingImage(Mat &source, Mat &dest, int top, int bottom, int left, int right,int borderType) {
    int newRows = source.rows + top + bottom;
    int newCols = source.cols + left + right;

    dest = Mat(newRows, newCols, source.type());
    //source.copyTo(dest(Range(top, dest.rows - bottom), Range(left, dest.cols - right)));
    source.copyTo(dest(Rect(left,top,source.cols,source.rows)));
    switch (borderType){
        case MYBORDER_CONSTANT: {
            borderCostant(source, dest, left, top, right, bottom);
            break;
        }
        case MYBORDER_WRAP: {
            borderWrap(source, dest, left, top, right, bottom);
            break;
        }
        case MYBORDER_REFLECT: {
            borderReflect(source, dest, left, top, right, bottom);
            break;
        }
        case MYBORDER_REPLICATE: {
            borderReplicate(source, dest, left, top, right, bottom);
            break;
        }
        default:
            break;
    }
    
}

void borderCostant(Mat& source, Mat& dest, int left, int top, int right, int bottom) {

    dest(Rect(0, 0, source.cols+left+right, top)).setTo(0);
    dest(Rect(0, top, left, source.rows)).setTo(0);
    dest(Rect(0, source.rows+top, source.cols + left + right, bottom)).setTo(0);
    dest(Rect(source.cols+left, top, right, source.rows)).setTo(0);
    
}


void borderWrap(Mat& source, Mat& dest, int left, int top, int right, int bottom) {

    //TOP-CENTER
    source(Rect(0, source.rows - top, source.cols, top)).copyTo(dest(Rect(left,0,source.cols,top)));
    //TOP-LEFT
    source(Rect(source.cols-left, source.rows - top, left, top)).copyTo(dest(Rect(0, 0, left, top)));
    //LEFT-CENTER
    source(Rect(source.cols - left, 0, left, source.rows)).copyTo(dest(Rect(0, top, left, source.rows)));
    //LEFT-BOTTOM
    source(Rect(source.cols-left, 0, left, bottom)).copyTo(dest(Rect( 0, dest.rows - bottom, left, bottom)));
    //BOTTOM-CENTER
    source(Rect(0, 0, source.cols, bottom)).copyTo(dest(Rect(left, dest.rows-bottom, source.cols, bottom)));
    //BOTTOM-RIGHT
    source(Rect(0, 0, right, bottom)).copyTo(dest(Rect(dest.cols-right, dest.rows - bottom, right, bottom)));
    //RIGHT-CENTER
    source(Rect(0, 0, right, source.rows)).copyTo(dest(Rect(dest.cols - right, top, right, source.rows)));
    //TOP-RIGHT
    source(Rect(0, source.rows-top, right, top)).copyTo(dest(Rect(dest.cols - right, 0, right, top)));
    
}


void borderReflect(Mat& source, Mat& dest, int left, int top, int right, int bottom) {

   //TOP-CENTER
    flip(source(Rect(0, 0, source.cols, top)), dest(Rect(left, 0, source.cols, top)),0);
    //bottom-center
    flip(source(Rect(0, source.rows-bottom, source.cols, bottom)), dest(Rect(left,dest.rows-bottom, source.cols, bottom)), 0);
    //left
    flip(dest(Rect(left, 0, left, dest.rows)), dest(Rect(0, 0, left, dest.rows)), 1);
    //right
    flip(dest(Rect(dest.cols-right*2, 0, right, dest.rows)), dest(Rect(dest.cols-right, 0, right, dest.rows)), 1);
   
}

void borderReplicate(Mat& source, Mat& dest, int left, int top, int right, int bottom) {
    //LEFT-CENTER
    for (int j = 0; j < left; j++) 
       source(Rect(0, 0, 1, source.rows)).copyTo(dest(Rect(j, top, 1, source.rows)));

    //TOP-CENTER
    for (int i = 0; i < top; i++) 
        source(Rect(0, 0, source.cols, 1)).copyTo(dest(Rect(left, i, source.cols, 1)));
    
    //BOTTOM-CENTER
    for (int i = 0; i < bottom; i++)
        source(Rect(0, source.rows-1, source.cols, 1)).copyTo(dest(Rect(left, dest.rows-i-1, source.cols, 1)));

    //RIGHT-CENTER
    for (int i = 0; i < right; i++)
        source(Rect(source.cols-1, 0, 1, source.rows)).copyTo(dest(Rect(dest.cols-i-1, top, 1, source.rows)));

    //TOP-LEFT
    for (int i = 0; i < left; i++)
        dest(Rect(left, 0, 1,top)).copyTo(dest(Rect(i, 0, 1, top)));

    //BOTTOM-LEFT
    for (int i = 0; i < left; i++)
        dest(Rect(left, dest.rows-bottom, 1, bottom)).copyTo(dest(Rect(i, dest.rows-bottom, 1, bottom)));

    //BOTTOM-RIGHT
    for (int i = 0; i < right; i++)
        dest(Rect(dest.cols-right-1, dest.rows - bottom-1, 1, bottom)).copyTo(dest(Rect(dest.cols-i-1, dest.rows - bottom, 1, bottom)));

    //TOP-RIGHT
    for (int i = 0; i < right; i++)
        dest(Rect(dest.cols - right - 1, 0, 1, top)).copyTo(dest(Rect(dest.cols - i - 1, 0, 1, top)));
}
