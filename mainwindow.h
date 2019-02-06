#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QVBoxLayout>

#include "opencv2/opencv.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

const float calibrationSquareDimension = 0.01905f; //meters
const Size chessboardDimensions = Size(6,9);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateImagesList();
    void createKnowBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners);
    void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false);
    void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficients);
    bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients);

    void on_btn_LoadImages_clicked();

    void on_btn_Calibrate_clicked();

private:
    Ui::MainWindow *ui;
    QStringList m_ImagesNames;
    int m_ImagesQuantity;
    float m_CalibrationSquareDimension = 0.01905f;
    Size m_ChessboardDimensions = Size(6,9);
};

#endif // MAINWINDOW_H
