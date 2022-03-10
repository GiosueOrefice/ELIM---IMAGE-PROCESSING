#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

/*
Inizializzare l'accumulatore H
Applicare l'algoritmo di Canny per individuare i punti di edge
Per ogni punto(𝑥, 𝑦) di edge
Per ogni angolo 𝜃 = 0: 360 e per ogni raggio r = Rmin : Rmax
Calcolare 𝑎 b
Incrementare H(𝑎, 𝑏, 𝑟) = H(𝑎, 𝑏, 𝑟) + 1
Le celle H (𝑎, 𝑏, 𝑟) con un valore maggiore di una soglia th corrispondono alle circonferenze nell'immagine
*/

void myHarrisCerchi(Mat src, Mat& dst, int votesThreshold) {

    //Facciamo un po' di smoothing
    GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

    Mat canny;
    Canny(src, canny, 60, 200, 3);

    // Range massimi di raggi
    int r_min = 50;
    int r_max = 800;

    int sizes[] = { canny.rows, canny.cols, r_max - r_min + 1 }; // Matrice dei voti tridimensionale
    Mat votes = Mat(3, sizes, CV_8U, Scalar(0));
    
    int a, b;
    for (int x = 0; x < canny.rows; x++) {
        for (int y = 0; y < canny.cols; y++) {
            if (canny.at<uchar>(x, y) == 255) {
                for (int r = r_min; r <= r_max; r++) {
                    for (int tetha = 0; tetha < 360; tetha++) {
                        int a = y - r * cos(tetha * CV_PI / 180);
                        int b = x - r * sin(tetha * CV_PI / 180);

                        if (a >= 0 && a < canny.cols && b >= 0 && b < canny.rows)
                            votes.at<uchar>(b, a, r - r_min)++; //I voti vanno nell'intervallo r_max - r_min, quindi sottraggo r - r_min per riportarlo nel range
                    }
                }
            }
        }
        cout << canny.rows - x << endl;
    }


    for (int r = r_min; r < r_max; r++) {
        for (int b = 0; b < canny.rows; b++) {
            for (int a = 0; a < canny.cols; a++) {
                if (votes.at<uchar>(b, a, r - r_min) >= votesThreshold) {
                    circle(dst, Point(a, b), 3, Scalar(255), 2, 8, 0);
                    circle(dst, Point(a, b), r, Scalar(255), 2, 8, 0);
                }
            }
        }
    }

}

int main(int argc, char** argv) {

    Mat image = imread("eye.jpeg", IMREAD_GRAYSCALE);

    if (image.empty()) exit(-1);

    

    Mat cerchi= Mat::zeros(image.rows, image.cols, image.type());

    myHarrisCerchi(image, cerchi, 150);
    imshow("Immagine originale", image);
    imshow("Cerchi", cerchi);
    waitKey(0);
    destroyAllWindows();

    return 0;

}