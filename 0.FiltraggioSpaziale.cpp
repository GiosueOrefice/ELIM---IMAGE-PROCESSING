#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void myFilter2D(Mat src, Mat& dst, Mat kernel);
void checkMat(Mat my, Mat filter);

int main(int argc, char** argv) {

    String imageName = "messi5.jpg";

    Mat image;
    image = imread(imageName, IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    image.convertTo(image, CV_32F);
    image /= (float)255;
 
    
    //my filter2d
    int dimKernel = 9;
    Mat kernel = Mat::ones(dimKernel, dimKernel, CV_32F) / (float)(dimKernel * dimKernel);

    Mat filteredImg;
    myFilter2D(image,filteredImg, kernel);

    Mat output1;
    filter2D(image, output1, image.type(), kernel);

    //checkMat(filteredImg, output1);

    imshow("Filter2d", output1);
    imshow("MY filter2d image", filteredImg);
    waitKey(0);
    
    
    //laplaciano 45°
    Mat laplacianImg45;

    float dataLaplacian45[9] = { 0,1,0,1,-4,1,0,1,0 };
    Mat kernelLaplacian45 = Mat(3, 3, CV_32F, dataLaplacian45);
   
    filter2D(image, laplacianImg45, image.type(), kernelLaplacian45);
    //normalize(laplacianImg45, laplacianImg45, 0, 255, NORM_MINMAX, CV_8U);

    
    //laplaciano 90°
    Mat laplacianImg90;
   
    float dataLaplacian90[9] = { 1,1,1,1,-8,1,1,1,1 };
    Mat kernelLaplacian90 = Mat(3, 3, image.type(), dataLaplacian90);

    filter2D(image, laplacianImg90, image.type(), kernelLaplacian90);
    //normalize(laplacianImg90, laplacianImg90, 0, 255, NORM_MINMAX, CV_8U);

    Mat out1;
    Laplacian(image, out1, image.type(), 3);
    //normalize(out1, out1, 0, 255, NORM_MINMAX, CV_8U);
    
    imshow("Laplaciano 45", laplacianImg45);
    imshow("Laplaciano 90", laplacianImg90);
    imshow("ORIGINAL LAPLACIAN", out1);
    waitKey(0);


    //sobel x
    Mat sobelImg_x,output3;
    float dataSobel_gx[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    Mat kernelSobel_gx = Mat(3, 3, CV_32F, dataSobel_gx);
    
    filter2D(image, sobelImg_x, image.type(), kernelSobel_gx);
    Sobel(image, output3, image.type(), 1, 0);

    cout << "sobel x" << endl;
    checkMat(sobelImg_x, output3);

    imshow("sobel x filter2d", sobelImg_x);
    imshow("sobel x ", output3);
    waitKey(0);
    
    //sobel y
    Mat output4, sobelImg_y;
    float dataSobel_gy[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
    Mat kernelSobel_gy = Mat(3, 3, CV_32F, dataSobel_gy);

    filter2D(image, sobelImg_y, image.type(), kernelSobel_gy);
    Sobel(image, output4, image.type(), 0, 1);

    cout << "sobel y" << endl;
    checkMat(sobelImg_y, output4);

    imshow("sobel y filter2d", sobelImg_y);
    imshow("sobel y ", output4);
    
   
    waitKey(0);

    //sobel xy
    Mat sobelxy;
    Sobel(image, sobelxy, image.type(), 1, 1);
    imshow("sobel xy ", sobelxy);
    waitKey(0);

    
    Mat gradMagnitude = abs(sobelImg_x) + abs(sobelImg_y);
    Mat gradMagnitude2;

    pow(sobelImg_x, 2, sobelImg_y);
    pow(sobelImg_y, 2, sobelImg_y);
    sqrt(sobelImg_y + sobelImg_y, gradMagnitude2);
  
    imshow("Magnitudo formula 1", gradMagnitude);
    imshow("Magnitudo formula 2", gradMagnitude2);
    waitKey(0);

 
    Mat sharperImg45 = image + (-1 * laplacianImg45);
    Mat sharperImg90 = image + (-1 * laplacianImg90);

   
    imshow("Sharper image laplancian45", sharperImg45);
    imshow("Sharper image laplancian90", sharperImg90);
    waitKey(0);
    
    return(0);
}

void myFilter2D(Mat src, Mat& dst, Mat kernel) {

    dst = Mat(src.rows, src.cols, src.type());
    int range = floor(kernel.rows / 2);

    copyMakeBorder(src, src, range, range, range, range, BORDER_CONSTANT, Scalar(0));

    for (int i = 0; i < dst.rows; i++)
        for (int j = 0; j < dst.cols; j++)
            dst.at<float>(i, j) = (float)sum(kernel.mul(src(Rect(j, i, kernel.cols, kernel.rows))))[0];
}

void checkMat(Mat mat1, Mat mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        cout << "size diversi" << endl;
        return;
    }
    int cont = 0;
    for (int i = 0; i < mat1.rows; i++) 
        for (int j = 0; j < mat1.cols; j++) 
            if ((int)mat1.at<float>(i, j) != (int)mat2.at<float>(i, j))
                cont++;
        
    cout << "Numero di pixel diversi: "<<cont<<endl;
}
