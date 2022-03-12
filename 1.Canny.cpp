#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat isteresi(Mat src, int lth, int hth) {
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8U);
	for (int i = 1; i < src.rows; i++) {
		for (int j = 1; j < src.cols; j++) {
			if (src.at<uchar>(i, j) > hth) //Se il valore del pixel è maggiore della soglia alta diventa un edge forte
				dst.at<uchar>(i, j) = 255;
			else if (src.at<uchar>(i, j) < lth) //Se il valore del pixel è minore della soglia bassa viene eliminato
				dst.at<uchar>(i, j) = 0;
			else if (src.at<uchar>(i, j) >= lth && src.at<uchar>(i, j) <= hth) { //IL valore del pixel si trova tra le due soglie, quindi è un edge debole
				for (int x = -1; x <= 1; x++) {
					for (int y = -1; y <= 1; y++) { //Controllo i valori nel suo intorno 3x3
						if (src.at<uchar>(i + x, j + y) > hth) //Se il loro valore è maggiore della soglia alta
							dst.at<uchar>(i + x, j + y) = 255; //vengono promossi a punti di edge forti
					}
				}
			}
		}
	}
	return dst;
}
Mat nonMaximaSuppression(Mat magnitudo, Mat orientation) {
	Mat dst = Mat::zeros(magnitudo.rows, magnitudo.cols, CV_8U);
	for (int i = 1; i < magnitudo.rows; i++) {
		for (int j = 1; j < magnitudo.cols; j++) {
			float mag = magnitudo.at<uchar>(i, j);
			float angolo = orientation.at<float>(i, j);
			//normalizziamo l'angolo perche' abbiamo usato phase invece di atan()
		   //se e' maggiore di 180 sottraiamo 360 per avere valori minori di zero
			angolo = angolo > 180 ? angolo - 360 : angolo;
			cout << angolo << endl;
			if (((angolo > -22.5) && (angolo <= 22.5)) || ((angolo > 157.5) || (angolo <= -157.5))) { //VERTICALE
				if (mag > magnitudo.at<uchar>(i, j - 1) && mag > magnitudo.at<uchar>(i, j + 1))
					dst.at<uchar>(i, j) = mag;
			}
			else if (((angolo > -67.5) && (angolo <= -22.5)) || ((angolo > 112.5) && (angolo <= 157.5))) { //+45°
				if (mag > magnitudo.at<uchar>(i - 1, j - 1) && mag > magnitudo.at<uchar>(i + 1, j + 1))
					dst.at<uchar>(i, j) = mag;
			}
			else if (((angolo > -112.5) && (angolo <= -67.5)) || ((angolo > 67.5) && (angolo <= 112.5))) { //ORIZZONTALE
				if (mag > magnitudo.at<uchar>(i - 1, j) && mag > magnitudo.at<uchar>(i + 1, j))
					dst.at<uchar>(i, j) = mag;
			}
			else if (((angolo > -157.5) && (angolo <= -112.5)) || ((angolo > 22.5) && (angolo <= 67.5))) { //-45°
				if (mag > magnitudo.at<uchar>(i + 1, j - 1) && mag > magnitudo.at<uchar>(i - 1, j + 1))
					dst.at<uchar>(i, j) = mag;
			}
		}
	}
	return dst;
}

/*
I risultati dipendono da alcuni parametri :
	-ampiezza della gaussiana nella prima fase
	-Dimensione del filtro nella prima fase
	-T1 e T2 – soglie per l’isteresi nell’ultima fase(lth e hth) 
*/
void myCanny(Mat src, Mat& dst, int k_size, int lth, int hth) {
	// 1. Smoothing gaussiano dell’immagine
	Mat gauss;
	GaussianBlur(src, gauss, Size(5, 5), 0, 0);
	//2. Calcolo del gradiente
	Mat sob_x, sob_y, magnitudo;
	Sobel(gauss, sob_x, CV_32F, 1, 0, k_size); //Calcolo gx
	Sobel(gauss, sob_y, CV_32F, 0, 1, k_size); //Calcolo gy
	magnitude(sob_x, sob_y, magnitudo); //Calcolo la magnitudine
	normalize(magnitudo, magnitudo, 0, 255, NORM_MINMAX, CV_8U); // Normalizzare

	// 3.Soppressione dei non - massimi in direzione ortogonale all’edge
	
	//Calcolo le direzioni del gradiente
	// phase è uguale ad atan(arg), ma quest'ultima ritorna sia positivi che negativi, phase solo positivi (0 - 359)
	Mat orientation;
	phase(sob_x, sob_y, orientation, true);

	Mat nms;
	nms = nonMaximaSuppression(magnitudo, orientation); //Effettuiamo la non maxima suppression

	//4. Selezione degli edge significativi mediante isteresi
	Mat output;
	output = isteresi(nms, lth, hth); //Effettuiamo il thresholding con isteresi
	dst = output;
}
int main(int argc, char** argv) {
	
	int kernel_size = 3;
	Mat src = imread("eye.jpeg", IMREAD_GRAYSCALE), dst;
	if (!src.data) return -1;
	int lth = 25;
	int hth = 35;
	myCanny(src, dst, kernel_size, lth, hth);
	imshow("Input", src);
	imshow("Output", dst);
	waitKey(0);
	return 0;
}
