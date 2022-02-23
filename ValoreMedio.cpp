#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void valoreMedio3x3(Mat src, Mat& dst);

int main(){
	Mat imageSrc = imread("messi5.jpg", IMREAD_GRAYSCALE);
	Mat imageDst;

	valoreMedio3x3(imageSrc, imageDst);
	imshow("ORIGINAL", imageSrc);
	imshow("VALORE MEDIO", imageDst);
	waitKey(0);

}

void valoreMedio3x3(Mat src, Mat& dst) {
	copyMakeBorder(src, dst, 1, 1, 1, 1,BORDER_CONSTANT, 0);
	copyMakeBorder(src, src, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	for (int i = 1; i < dst.rows-1; i++) {
		for (int j = 1; j < dst.cols-1; j++) {
			dst.at<unsigned char>(i,j)= (src.at<unsigned char>(i-1,j-1) + src.at<unsigned char>(i , j - 1) + src.at<unsigned char>(i +1 , j - 1) + 
				src.at<unsigned char>(i -1, j ) + src.at<unsigned char>(i, j) + src.at<unsigned char>(i +1, j) +
				src.at<unsigned char>(i - 1, j + 1) + src.at<unsigned char>(i, j + 1) + src.at<unsigned char>(i + 1, j + 1)) / 9;

		}
	}
}