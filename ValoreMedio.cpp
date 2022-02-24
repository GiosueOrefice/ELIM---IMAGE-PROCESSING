#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void neighbor3x3(Mat src, Mat& dst);
int valoreMedio3x3(Mat dst, int i, int j);

int main(){
	Mat imageSrc = imread("messi5.jpg", IMREAD_GRAYSCALE);
	Mat imageDst;

	neighbor3x3(imageSrc, imageDst);
	imshow("ORIGINAL", imageSrc);
	imshow("VALORE MEDIO", imageDst);
	waitKey(0);

}

void neighbor3x3(Mat src, Mat& dst) {
	src.copyTo(dst);
	copyMakeBorder(src, src, 1, 1, 1, 1, BORDER_CONSTANT, 0);
	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) 
			dst.at<unsigned char>(i, j) = valoreMedio3x3(src, i+1, j+1);
	}
}

int valoreMedio3x3(Mat mat,int i, int j) {
	int sum = 0;
	for (int k = i - 1; k <= i + 1; k++) {
		for (int z = j - 1; z <= j + 1; z++)
			sum += mat.at<unsigned char>(k, z);
	}

	return int(sum/9);
}
