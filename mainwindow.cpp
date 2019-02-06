#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImagesList(){
    QVBoxLayout *imagesListLayout = new QVBoxLayout(this); //layout na etykiety z nazwami plikow

    for(int i=0; i<m_ImagesQuantity; i++){
        QLabel *lbl_oneImageName = new QLabel(m_ImagesNames.at(i)); //pobranie nazwy pliku i wpisanie jej do etykietki
        imagesListLayout->addWidget(lbl_oneImageName); //dodanie etykietki do layoutu
    }

    ui->scrollAreaWidgetContents->setLayout(imagesListLayout); //umieszczenie layoutu w scrollArenie głównego okna (wypisanie nazw)
    ui->btn_Calibrate->setEnabled(true); //ustawienie przycisku Calibrate jako dostępny
}

void MainWindow::on_btn_LoadImages_clicked(){
    QString filter = "PNG (*.png)"; // filter wyświetlanych plikow
    m_ImagesNames = QFileDialog::getOpenFileNames(this,"Select Images", QDir::currentPath(), filter); //otworzenie nowego okna z możliwością wyboru zdjec do kalibracji
                                                                                                    // i zapisanie wybranych nazw do listy.
    m_ImagesQuantity = m_ImagesNames.size(); // zliczenie liczby wybranych plikow i wpisanie tej wartości do zmiennej
    ui->lbl_ImagesQuantity->setText(QString::number(m_ImagesQuantity)); // aktualizacja wyświetlanej liczby wybranych plików
    updateImagesList(); // funkcja wypisujaca nazwy wybranych zdjec
}

void MainWindow::createKnowBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners){
    for(int i=0; i<boardSize.height; i++){
        for(int j=0; j<boardSize.width; j++){
            corners.push_back(Point3d(j*squareEdgeLength, i*squareEdgeLength, 0.0f));
        }
    }
}

void MainWindow::getChessboardCorners(vector<Mat> images, vector<vector<Point2f>> &allFoundCorners, bool showResults){
    for(vector<Mat>::iterator iter = images.begin(); iter != images.end(); iter++){
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, Size(9,6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if(found){
            allFoundCorners.push_back(pointBuf);
        }

        if(showResults){
            drawChessboardCorners(*iter, Size(9,6), pointBuf, found);
            imshow("Looking for Corners", *iter);
        }
    }

}

void MainWindow::cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoefficients){
    vector<vector<Point2f>> checkerboardImageSpacePoints;
    getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);

    vector<vector<Point3f>> worldSpaceCornerPoints(1);

    createKnowBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

    vector<Mat> rVecrors, tVectors;
    distanceCoefficients = Mat::zeros(8,1,CV_64F);

    calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, cameraMatrix, distanceCoefficients, rVecrors, tVectors);
}

bool MainWindow::saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients)
{
    ofstream outStream(name);
    if(outStream){
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outStream << rows << endl << columns << endl;

        for(int r = 0; r<rows; r++){
            for(int c = 0; c<columns; c++){
                double value = cameraMatrix.at<double>(r, c);
                outStream << value << endl;
            }
        }

        rows = distanceCoefficients.rows;
        columns = distanceCoefficients.cols;

        outStream << rows << endl << columns << endl;

        for(int r = 0; r<rows; r++){
            for(int c = 0; c<columns; c++){
                double value = distanceCoefficients.at<double>(r, c);
                outStream << value << endl;
            }
        }

        outStream.close();
        return true;
    }
    return false;
}

void MainWindow::on_btn_Calibrate_clicked()
{
    QString image_name;
    string image_file_name;
    Mat frame;
    Mat drawToFrame;
    Mat cameraMatrix = Mat::eye(3,3,CV_64F);
    Mat distanceCoefficients;
    vector<Mat> Images;

    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    namedWindow("Images", CV_WINDOW_AUTOSIZE);

    for(int i=0; i<m_ImagesQuantity; i++){
        bool found = false;
        image_name      =  m_ImagesNames.at(i);
        image_file_name =  image_name.toStdString();
        frame           =  imread(image_file_name,CV_LOAD_IMAGE_COLOR);

        vector<Vec2f> foundPoints;
        found = findChessboardCorners(frame, m_ChessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);
        if(found){
            imshow("Images", drawToFrame);
            Mat temp;
            frame.copyTo(temp);
            Images.push_back(temp);
        }
        else
            imshow("Images", frame);

        waitKey(300);
    }

    cameraCalibration(Images, chessboardDimensions, calibrationSquareDimension, cameraMatrix, distanceCoefficients);
    saveCameraCalibration("CameraCalibrationData", cameraMatrix, distanceCoefficients);
}
