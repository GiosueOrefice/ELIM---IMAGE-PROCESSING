#include <stdio.h>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

void harrisCornering(Mat src, Mat& dst, float k, int thresholdValue) {

    //1. Calcolare le derivate parziali rispetto ad x e y (Dx, Dy)
    Mat dx, dx_2;
    Sobel(src, dx, CV_32FC1, 1, 0, 3, BORDER_DEFAULT);

    Mat dy, dy_2;
    Sobel(src, dy, CV_32FC1, 0, 1, 3, BORDER_DEFAULT);

    //2. Calcolare Dx2, Dy2 e Dx*Dy
    pow(dx, 2, dx_2);//C00
    pow(dy, 2, dy_2);//C11

    Mat dxdy;
    multiply(dx, dy, dxdy);//C01, C10

    //3. Applicare un filtro Gaussiano a Dx2, Dy2 e Dx* Dy
    //4. Si ottengono C00, C11, C01 (e quindi anche C10)
    GaussianBlur(dx_2, dx_2, Size(5,5), 2.0, 0.0, BORDER_DEFAULT);
    GaussianBlur(dy_2, dy_2, Size(5, 5), 0.0, 2.0, BORDER_DEFAULT);
    GaussianBlur(dxdy, dxdy, Size(5, 5), 2.0, 2.0, BORDER_DEFAULT);

    //5. Calcolare l'indice R
    Mat dx2_dy2;
    multiply(dx_2, dy_2, dx2_dy2);
    Mat dxdy2;
    multiply(dxdy, dxdy, dxdy2);
    Mat detrm = dx2_dy2 - dxdy2; //Determinante = (C00 * C11) - (C10 * C01)

    Mat traccia_2;
    pow((dx_2 + dy_2), 2, traccia_2); //Traccia = C00+C11

    Mat R = detrm - k * traccia_2; //R = determinante + k*traccia²

    // 6. Normalizzare l'indice R in [0, 255]
    normalize(R, R, 0, 255, NORM_MINMAX, CV_32FC1);
    //7. Sogliare R
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
            if ((int)R.at<float>(i, j) > thresholdValue)
                circle(dst, Point(j, i), 5, Scalar(0), 1, 8, 0);

}

int main(int argc, char** argv) {

    String imageName = "blox.jpg";

    //Leggo l'immagine
    Mat image;
    image = imread(imageName, IMREAD_GRAYSCALE);
    if (image.empty()) {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }


    Mat harris(image.rows, image.cols, image.type());
    image.copyTo(harris);

    harrisCornering(image, harris, 0.04, 100);

    imshow("Immagine originale", image);
    imshow("Harris cornering", harris);
    waitKey(0);
    destroyAllWindows();

    return 0;

}
