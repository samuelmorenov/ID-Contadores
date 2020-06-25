#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "GL/glut.h"


#include <rcdraw.h>


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <QCoreApplication>
using namespace cv;
using namespace std;



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

   explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:


    Ui::MainWindow *ui;

     // user viewers
    RCDraw *viewer_original;
    RCDraw *viewer_counter1;
    RCDraw *viewer_counter2;
    RCDraw *viewer_counter3;


//---------------------------------------------
    // Qimages associated to viewers
    QImage *Image_Source;
    QImage *image_counter;
    QImage *image_counter2;
    QImage *image_counter3;



     //---------------------------------------------



    Mat  Original_Image, mat_original, mat_processed, mat_counter, mat_counter2, mat_counter3;
    Mat  subImg, subImg2, subImg3;

    int numeroTemplates = 20;
    Mat  templates[20];

    int iterador = 0;


    float numerosCorrectos[36];
    float numerosCalculados[36];
    int cantidadNumeros = 36;
    int contadorNumeros = 0;






public slots: // Connect to user buttons

        void Load_Image();
        void Load_Image_Actual(int it);
        void cargarNumerosCorrectos();
        void Clipping();
        void ExtractNumbers();
        void escribirResultados();
        void GlobalProgress();
        int calcularError(float calculado, float correcto);

        //void extraer_numeros();
        //float obtenerNumero(Mat numeros[4],Mat templates[40]);

private slots:
        void cargarTemplates();
};

#endif // MAINWINDOW_H
