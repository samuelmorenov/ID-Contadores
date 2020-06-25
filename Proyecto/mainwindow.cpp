/*
  Mainwindow gestiona el proceso global de adquirir una imagen
  de una caja browser, extraer las subimágenes de los contadores
  y lanzar un OCR para leer los números.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <iostream>   // std::cout
#include <string>
#include <QFileDialog>

RNG rng(12345);

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;



using namespace std;
// Constructor member
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);





    // Original Image and sub-images
    this->Image_Source = new QImage(720,540, QImage::Format_RGB888);
    this->image_counter = new QImage(304,130, QImage:: QImage::Format_Indexed8);
    this->image_counter2 = new QImage(304,130, QImage:: QImage::Format_Indexed8);
    this->image_counter3 = new QImage(304,130, QImage:: QImage::Format_Indexed8);

    // viewers
    viewer_original = new RCDraw(720,540, Image_Source, ui->viewer_original);
    viewer_counter1 = new RCDraw(304,130, image_counter, ui->viewer_counter1);
    viewer_counter2 = new RCDraw(304,130, image_counter2, ui->viewer_counter2);
    viewer_counter3 = new RCDraw(304,130, image_counter3, ui->viewer_counter3);




    // connect to button
    connect ( ui->Load_button, SIGNAL (clicked()), this, SLOT( Load_Image() ) );
    connect ( ui->Clip_button, SIGNAL (clicked()), this, SLOT( Clipping() ) );
    connect ( ui->OCR_button, SIGNAL (clicked()), this, SLOT( ExtractNumbers() ) );
    connect ( ui->GLOBAL_button, SIGNAL (clicked()), this, SLOT( GlobalProgress() ) );


    cargarTemplates();



}
// Destructor member
MainWindow::~MainWindow()
{
    delete ui;

    delete Image_Source;
    delete image_counter;
    delete image_counter2;
    delete image_counter3;
    delete viewer_original;
    delete viewer_counter1;
    delete viewer_counter2;
    delete viewer_counter3;
 }


// Metodo que carga todas las imagenes de Templates de la carpeta "../templates" y los mete en el vector "templates"
void MainWindow::cargarTemplates(){
    for (int i = 0; i < numeroTemplates; ++i) {
            string sA;
            string name;
            sA = static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
            name = "../templates/" + sA + ".png";
            cv::Mat template_img= imread(name, CV_LOAD_IMAGE_COLOR);
            cvtColor(template_img, template_img, CV_BGR2GRAY);
            templates[i]=template_img.clone();

    }

}

//Metodo que introduce manualmente todos los numeros correctos de las capturas en el vector numerosCorrectos
void MainWindow::cargarNumerosCorrectos(){


    numerosCorrectos[0]=214.2;     numerosCorrectos[1]=239.7;     numerosCorrectos[2]=195.3;
    numerosCorrectos[3]=198.6;     numerosCorrectos[4]=206.4;     numerosCorrectos[5]=212.0;
    numerosCorrectos[6]=190.6;     numerosCorrectos[7]=208.1;     numerosCorrectos[8]=234.6;
    numerosCorrectos[9]=170.7;     numerosCorrectos[10]=196.6;    numerosCorrectos[11]=210.9;
    numerosCorrectos[12]=156.2;    numerosCorrectos[13]=183.5;    numerosCorrectos[14]=178.6;
    numerosCorrectos[15]=134.2;    numerosCorrectos[16]=162.2;    numerosCorrectos[17]=182.9;
    numerosCorrectos[18]=167.5;    numerosCorrectos[19]=157.8;    numerosCorrectos[20]=160.9;
    numerosCorrectos[21]=127.8;    numerosCorrectos[22]=160.0;    numerosCorrectos[23]=149.6;
    numerosCorrectos[24]=149.8;    numerosCorrectos[25]=178.0;    numerosCorrectos[26]=146.9;
    numerosCorrectos[27]=165.9;    numerosCorrectos[28]=169.3;    numerosCorrectos[29]=135.0;
    numerosCorrectos[30]=157.3;    numerosCorrectos[31]=187.8;    numerosCorrectos[32]=135.0;
    numerosCorrectos[33]=150.3;    numerosCorrectos[34]=169.3;    numerosCorrectos[35]=147.4;



}

//Carga la imagen seleccionada desde la caja de directorio
void MainWindow::Load_Image()
{
     // Looking for a image name in a directory with browsing box
    QString Directory="../capturas";
    QString fn = QFileDialog::getOpenFileName(this,"Choose a frame to download",Directory, "Images (*.png *.xpm *.jpg)");

    Original_Image= imread(fn.toStdString(), CV_LOAD_IMAGE_COLOR);
    cv::resize(Original_Image, mat_original, Size(720, 540), 0, 0, cv::INTER_CUBIC);
    memcpy(Image_Source->bits(),mat_original.data,mat_original.rows*mat_original.cols*sizeof(uchar)*3);
    viewer_original->update();
}

//Carga la imagen con el numero actual del iterador que se le pasa por parametro.
void MainWindow::Load_Image_Actual(int it){

    string sA;
    string name;
    sA = static_cast<std::ostringstream*>(&(std::ostringstream() << it))->str();
    name = "../capturas/capturas_" + sA + ".jpg";

    Original_Image= imread(name, CV_LOAD_IMAGE_COLOR);
    cv::resize(Original_Image, mat_original, Size(720, 540), 0, 0, cv::INTER_CUBIC);
    memcpy(Image_Source->bits(),mat_original.data,mat_original.rows*mat_original.cols*sizeof(uchar)*3);
    viewer_original->update();

}

//Metodo que recorta la imagen cargada y las muestra en los frames
void MainWindow::Clipping()
{
    //Constantes:
    int alto = 260;
    int ancho = 608;
    int altoP = 130;
    int anchoP = 304;
    int x = 500;
    int y = 975;


    subImg = Original_Image.clone();
    subImg = (subImg (cv::Range(x, alto+x), cv::Range(y, ancho+y)));
    cvtColor(subImg, subImg, CV_BGR2GRAY);
    cv::resize(subImg, mat_counter, Size(anchoP, altoP), 0, 0, cv::INTER_CUBIC);
    memcpy(image_counter->bits(),mat_counter.data,mat_counter.rows*mat_counter.cols*sizeof(uchar));
    viewer_counter1->update();



    x = 800;
    subImg2 = Original_Image.clone();
    subImg2 = (subImg2 (cv::Range(x, alto+x), cv::Range(y, ancho+y)));
    cvtColor(subImg2, subImg2, CV_BGR2GRAY);
    cv::resize(subImg2, mat_counter2, Size(anchoP, altoP), 0, 0, cv::INTER_CUBIC);
    memcpy(image_counter2->bits(),mat_counter2.data,mat_counter2.rows*mat_counter2.cols*sizeof(uchar));
    viewer_counter2->update();




    x = 1100;

    subImg3 = Original_Image.clone();
    subImg3 = (subImg3 (cv::Range(x, alto+x), cv::Range(y, ancho+y)));
    cvtColor(subImg3, subImg3, CV_BGR2GRAY);
    cv::resize(subImg3, mat_counter3, Size(anchoP, altoP), 0, 0, cv::INTER_CUBIC);
    memcpy(image_counter3->bits(),mat_counter3.data,mat_counter3.rows*mat_counter3.cols*sizeof(uchar));
    viewer_counter3->update();




}


//Metodo que compara los numeros de los frames recortados con las imagenes de los templates, guarda el numero con mayor coincidencias y lo guarda en resultado
void MainWindow::ExtractNumbers(){

    if (contadorNumeros == 36) contadorNumeros = 0;


    for (int z = 1; z < 4; z++){
        cv::Mat debug_img;
        if(z == 1) debug_img = subImg.clone();
        if(z == 2) debug_img = subImg2.clone();
        if(z == 3) debug_img = subImg3.clone();


        Mat numeroEnPosicion[4];


        Rect numero1(60,0,100,260);
        Rect numero2(160,0,100,260);
        Rect numero3(260,0,100,260);
        Rect numero4(360,0,100,260);

        numeroEnPosicion[0]=debug_img(numero1);
        numeroEnPosicion[1]=debug_img(numero2);
        numeroEnPosicion[2]=debug_img(numero3);
        numeroEnPosicion[3]=debug_img(numero4);


        double minVal; double maxVal;
        Point minLoc, maxLoc, matchLoc;
        int valorSimilitud[numeroTemplates];
        Rect rectangulo;
        Mat aux;

        Mat aux_templates[numeroTemplates],aux_recorte,aux_compare;

        //adaptiveThreshold:
        double maxValue = 255;
        int adaptiveMethod = 0; //ADAPTIVE_THRESH_GAUSSIAN_C = 1
        int thresholdType = 0; //THRESH_BINARY = 0
        int blockSize = 17;
        double C = 3;


        for(int i =0; i <numeroTemplates; i++){
           adaptiveThreshold(templates[i], aux_templates[i], maxValue , adaptiveMethod, thresholdType, blockSize,C);
        }



        float resultado = 0;
        for (int n = 0; n < 4; n++) {
            for (int i = 0; i < numeroTemplates; ++i) {


                int match_method = CV_TM_CCORR_NORMED;
                cv::Mat result_mat;

                //**//
                matchTemplate(numeroEnPosicion[n], templates[i], result_mat, match_method);
                normalize(result_mat, result_mat, 0, 1, NORM_MINMAX, -1, Mat());
                minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
                matchLoc = maxLoc;



                rectangulo.x=matchLoc.x;
                rectangulo.y=matchLoc.y;
                rectangulo.width=templates[i].cols;
                rectangulo.height=templates[i].rows;
                aux= numeroEnPosicion[n](rectangulo);


                adaptiveThreshold(aux, aux_recorte, maxValue , adaptiveMethod, thresholdType, blockSize, C);
                cv::compare(aux_recorte,aux_templates[i],aux_compare,CMP_NE);

                valorSimilitud[i]= countNonZero(aux_compare);

            }


            int menor = valorSimilitud[0];
            float posicionDelMenor = 0;
            for(int i=1; i<numeroTemplates; i++){

               if(menor > valorSimilitud[i]){
                   menor = valorSimilitud[i];
                   posicionDelMenor= i;
               }
            }




                if(posicionDelMenor>=10 && posicionDelMenor < 20) posicionDelMenor = posicionDelMenor -10;
                else if(posicionDelMenor >=20 && posicionDelMenor <30)posicionDelMenor = posicionDelMenor -20;
                else if(posicionDelMenor>=30 && posicionDelMenor <40) posicionDelMenor = posicionDelMenor -30;




            if(n==0) resultado = resultado + (100*posicionDelMenor);
            if(n==1) resultado = resultado + (10*posicionDelMenor);
            if(n==2) resultado = resultado + posicionDelMenor;
            if(n==3) resultado = resultado + (posicionDelMenor*0.1);

            }



        QString resultadoString=QString::number(resultado);

        numerosCalculados[contadorNumeros]=resultado;
        contadorNumeros++;



        if(z == 1) ui->resultado1->setPlainText(resultadoString);
        if(z == 2) ui->resultado2->setPlainText(resultadoString);
        if(z == 3) ui->resultado3->setPlainText(resultadoString);
    }


}

//Metodo que toma los valores de todos los numeros obtenidos, los compara con los correctos, calcula el porcentaje de fidelidad y lo guarda en el archivo ../Resultados.txt
void MainWindow::escribirResultados(){
    ofstream salida;
    salida.open("../Resultados.txt");
    if(salida.is_open()){
        float fidelidades[12];

        salida<<"_________________________________________________________________________________________________________________________"<<endl;
        salida<<"|                                                                                                                  	|"<<endl;
        salida<<"|Imagen	|Calculado 1	|Correcto 1	||Calculado 2	|Correcto 2	||Calculado 3	|Correcto 3	||Fidelidad	|"<<endl;
        salida<<"|_______________________________________________________________________________________________________________________|"<<endl;


        for (int numImagenes = 0; numImagenes < 12; ++numImagenes) {
            salida << "|"<< numImagenes+1<<"\t"<<"| "<<"";


            float f[3];


            for (int cont = 0; cont < 3; ++cont) {
                float calculado = numerosCalculados[numImagenes*3+cont];
                float correcto = numerosCorrectos[numImagenes*3+cont];
                salida <<calculado<<"  "<<"\t"<<"| "<<""<<correcto<<"   "<<"\t"<<"|| "<<"";

                int auxF = calcularError(calculado, correcto);

                f[cont] = auxF;

            }

            int fidelidad = (f[0]+f[1]+f[2])/3;
            fidelidades[numImagenes] = fidelidad;
            salida <<fidelidad<<"%"<<"\t"<<"\t"<<"|"<<endl;

        }

        float fidelidadTotal = 0;
        for (int var = 0; var < 12; ++var) {
            fidelidadTotal = fidelidadTotal + fidelidades[var];
        }
        fidelidadTotal = fidelidadTotal/12;



        salida<<"|                                                                                                                       |"<<endl;
        salida<<"|Total"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"|| "<<fidelidadTotal<<"%"<<"\t"<<"\t"<<"|"<<endl;
        salida<<"|_______________________________________________________________________________________________________________________|"<<endl;
        salida.close();
        contadorNumeros = 0;

}


    else{
        exit(1);
        cout << "Error en el fichero de salida." << endl;
    }


}

//metodo que dados el dato calculado y el dato correcto devuelve el porcentaje de error (como entero)
int MainWindow::calcularError(float calculado, float correcto){
    int resultado = 0;

    int a1 = calculado/100;
    int o1 = correcto/100;
    int a2 = (calculado-(a1*100))/10;
    int o2 = (correcto -(o1*100))/10;
    int a3 = calculado-(a1*100+a2*10);
    int o3 = correcto-(o1*100+o2*10);
    int aa4 = calculado;
    float a4 = calculado - aa4;
    int oo4 = correcto;
    float o4 = correcto - oo4;


    if(a1-o1==0) resultado = resultado +25;
    if(a2-o2==0) resultado = resultado +25;
    if(a3-o3==0) resultado = resultado +25;
    if(a4-o4==0) resultado = resultado +25;


    return resultado;


}

//Metodo que carga todas las imagenes, las recorta obtiene los numeros y los escribe en el archivo de salida
void MainWindow::GlobalProgress(){

    contadorNumeros = 0;
    cargarNumerosCorrectos();
    for (int var = 1; var <= 12; ++var) {
        cout << "Leyendo la imagen numero " << var << "..." << endl;
        Load_Image_Actual(var);
        Clipping();
        ExtractNumbers();
    }
    cout << "Todas las imagenes leidas." << endl;
    escribirResultados();
    cout << "Fichero creado."<< endl;

}




