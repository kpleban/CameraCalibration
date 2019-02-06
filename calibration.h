#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <opencv2/opencv.hpp>
#include <string>

#define WIDTH 2900
#define HEIGHT 2100
#define SCALE 0.0847

using namespace std;
using namespace cv;

class Calibration
{
private:
    Mat m_homographyMatrix;
    Mat m_cameraMatrix;
    Mat m_distortionMatrix;


public:
    Calibration();

    Mat homographyMatrix() const;
    Mat cameraMatrix() const;
    Mat distortionMatrix() const;
    void setCalibration(const Calibration &calibration);
    void setCalibration(const Mat &homographyMatrix, const Mat &cameraMatrix, const Mat &distortionMatrix);
    void operator =(Calibration &calibration);
    void setHomographyMatrix(const Mat &homographyMatrix);
    void setCameraMatrix(const Mat &cameraMatrix);
    void setDistortionMatrix(const Mat &distortionMatrix);
};

#endif // CALIBRATION_H
