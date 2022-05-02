#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

double myDistance(Scalar p1, Scalar p2)
{
    return pow(p1[0] - p2[0], 2) +
        pow(p1[1] - p2[1], 2) +
        pow(p1[2] - p2[2], 2);
}

void myKmeans(Mat& src, Mat& dst, int nClusters)
{
    // passo 0 -> sfocatura
    GaussianBlur(src, src, Size(5, 5), 0, 0);

    vector<Scalar> centers_colors;
    vector<vector<Point>> clusters;

    // Passo 1 -> Calcolo i centri
    for (int k = 0; k < nClusters; k++)
    {
        Point center;
        center.x = (src.cols / nClusters) * k; // K-MEANS++
        center.y = (src.rows / nClusters) * k; // Si potrebbero prendere i centri in maniera diversa
        Scalar center_colors(
            src.at<Vec3b>(center)[0],
            src.at<Vec3b>(center)[1],
            src.at<Vec3b>(center)[2]
        );
        centers_colors.push_back(center_colors);
        vector<Point> t;
        clusters.push_back(t);
    }

    // Passo 2 ->  Assegno i pixel ai cluster, ricalcolo i centri usando le medie, fino a che differenza < 0.1
    double oldCenterSum = 0;
    double difference = INFINITY;
    while (difference > 0.1)
    {
     
        for (int k = 0; k < nClusters; k++)
            clusters[k].clear();
 

        // passo 3 -> Aggiorno i pixels nei clusters
        for (int x = 0; x < src.rows; x++)
        {
            for (int y = 0; y < src.cols; y++)
            {
                // Calcolo le differenza di intensità da ogni centro dei cluster e posiziono il pixel nel cluster con differenza di intensità minima
                double minDistance = INFINITY;
                int clusterIndex = 0;
                Point pixel(y, x);
                //Calcolo differeza di intensità di ogni pixel rispetto ad ogni cluster (centri)
                for (int k = 0; k < nClusters; k++)
                {
                    Scalar center = centers_colors[k];
                    Scalar point(
                        src.at<Vec3b>(x, y)[0],
                        src.at<Vec3b>(x, y)[1],
                        src.at<Vec3b>(x, y)[2]
                    );
                    double distance = myDistance(center, point);
                    if (distance < minDistance)
                    {
                        minDistance = distance;
                        clusterIndex = k; // Salvo il pixel nel cluster di indice k
                    }
                }
                clusters[clusterIndex].push_back(pixel); //aggiungo il pixel nel cluster migliore 
            }
        }

        // passo 4 -> Calcolo la media di ogni cluster
        double newCenterSum = 0;
        for (int k = 0; k < nClusters; k++)
        {
            vector<Point> clusterPoints = clusters[k];
            double blue = 0, green = 0, red = 0;
            //cout << "Numero di pixel nel cluster " <<k<<": " << clusterPoints.size() << endl;
            for (int i = 0; i < clusterPoints.size(); i++)
            {
                Point pixel = clusterPoints[i];
                blue += src.at<Vec3b>(pixel)[0];
                green += src.at<Vec3b>(pixel)[1];
                red += src.at<Vec3b>(pixel)[2];
            }
            // Calcolo media dei colori del nuovo centro
            blue /= clusterPoints.size();
            green /= clusterPoints.size();
            red /= clusterPoints.size();
            // Calcolo la distanza del nuovo centro dal vecchio e aggiungo a newCenterSum
            Scalar center = centers_colors[k];
            Scalar newCenter(blue, green, red);
            //cout << "center: " << center << endl;
            //sommo la differenza di intensità tra i nuovi centri e vecchi 
            newCenterSum += myDistance(newCenter, center); 
            // Aggiorno il nuovo centro
            centers_colors[k] = newCenter;
        }
        newCenterSum /= nClusters; //media delle differenze tra i vecchi e nuovi center 
        difference = abs(oldCenterSum - newCenterSum); 
        //cout << "Differenza " << difference << endl<<endl;
        oldCenterSum = newCenterSum;
    }

    // passo 5 -> Disegno l'immagine di output
    for (int k = 0; k < nClusters; k++)
    {
        vector<Point> clusterPoints = clusters[k];
        Scalar center = centers_colors[k];
        for (int i = 0; i < clusterPoints.size(); i++)
        {
            // Assegno il valore del centro agli altri pixel del cluster
            Point pixel = clusterPoints[i];
            dst.at<Vec3b>(pixel)[0] = center[0];
            dst.at<Vec3b>(pixel)[1] = center[1];
            dst.at<Vec3b>(pixel)[2] = center[2];
        }
    }
}

double myDistance(double p1, double p2) {
    return pow(p1 - p2, 2);
}

void kMeansGrayScale(Mat src, Mat& dest, int nClusters) {

    // passo 0 -> sfocatura
    GaussianBlur(src, src, Size(5, 5), 0, 0);

    vector<double> centers;
    vector<vector<Point>> clusters;

    // Passo 1 -> Inizializzazione centri
    for (int k = 0; k < nClusters; k++) {
        int x = src.rows / nClusters * k;
        int y = src.cols / nClusters * k;
        centers.push_back(src.at<uchar>(x,y)); //aggiungere il k-esimo centro
        clusters.push_back(vector<Point>()); //creare il vettore di punti per il k-esemio cluster
    }

    // Passo 2 ->  Assegno i pixel ai cluster, ricalcolo i centri usando le medie, fino a che differenza < 0.1
    double oldDistanceCentersAVG = 0;
    double difference = INFINITY;
    double distance;
    while (difference > 0.1) {

        for (int k = 0; k < nClusters; k++)
            clusters[k].clear(); //pulisce il vector di ogni cluster

        for (int x = 0; x < src.rows; x++) {
            for (int y = 0; y < src.cols; y++) {

                double minDistance = INFINITY;
                int clusterIndex = 0;

                for (int k = 0; k < nClusters; k++) {
                    //calcolo la distanza di ogni pixel rispetto ai centri dei cluster
                    distance = myDistance(centers[k], src.at<uchar>(x, y));
                    if (distance < minDistance) { //se la distanza è minore, aggiorno il cluster di appartenenza
                        minDistance = distance;
                        clusterIndex = k; // Salvo il pixel nel cluster di indice k
                    }
                }

                clusters[clusterIndex].push_back(Point(y, x)); //aggiungi il pixel al cluster
            }
        }

        // 
        double newDistanceCentersAVG = 0;
        for (int k = 0; k < nClusters; k++) {
            vector<Point> clusterPoints = clusters[k];
            double clusterAVG = 0;
            for (int i = 0; i < clusterPoints.size(); i++)
                clusterAVG += src.at<uchar>(clusterPoints[i]); //somma di tutti i pixel del k-esimo cluster

            // Calcolo media del k-esimo cluster
            clusterAVG /= clusterPoints.size();
            //calcolo le distanze tra il vecchio centro e la nuova media del k-esimo cluster
            //vado a sommare tutte queste distanze nella var newCentersAVG
            newDistanceCentersAVG += myDistance(centers[k], clusterAVG);
            // Aggiorno il nuovo centro
            centers[k] = clusterAVG;
        }
        newDistanceCentersAVG /= nClusters; //facciamo la media
        difference = abs(oldDistanceCentersAVG - newDistanceCentersAVG); //differenza tra la vecchia media e la nuova
     
        oldDistanceCentersAVG = newDistanceCentersAVG; //aggiorno la media

    }

        //Disegno l'immagine di output
    for (int k = 0; k < nClusters; k++){
         vector<Point> clusterPoints = clusters[k];
         for (int i = 0; i < clusterPoints.size(); i++)
             dest.at<uchar>(clusterPoints[i]) = (int) centers[k];
    }


    

}

int main(int argc, char* argv[])
{
 
    //Mat input = imread("lena_color.png");
    Mat input = imread("lena_color.png", IMREAD_GRAYSCALE);
    if (input.empty())
    {
        cout << "Can't find image" << endl;
        exit(-1);
    }
    int nClusters = 2;

    Mat dst(input.rows, input.cols, input.type());
    //myKmeans(input, dst, nClusters);
    kMeansGrayScale(input, dst, nClusters);
    imshow("Input image", input);
    imshow("K-Means", dst);
    waitKey(0);
    return 0;
}
