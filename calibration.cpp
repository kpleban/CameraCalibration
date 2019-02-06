#include "calibration.h"

Mat Calibration::homographyMatrix() const
{
    return m_homographyMatrix;
}

Mat Calibration::cameraMatrix() const
{
    return m_cameraMatrix;
}

Mat Calibration::distortionMatrix() const
{
    return m_distortionMatrix;
}

void Calibration::setCalibration(const Mat &homographyMatrix, const Mat &cameraMatrix, const Mat &distortionMatrix){
   m_cameraMatrix=cameraMatrix;
   m_homographyMatrix=homographyMatrix;
   m_distortionMatrix=distortionMatrix;
}

void Calibration::setCalibration(const Calibration &calibration)
{
    m_cameraMatrix=calibration.cameraMatrix();
    m_homographyMatrix=calibration.homographyMatrix();
    m_distortionMatrix=calibration.distortionMatrix();
}
void Calibration::operator =(Calibration &calibration)
{
    setCalibration(calibration);
}

void Calibration::setHomographyMatrix(const Mat &homographyMatrix)
{
    m_homographyMatrix = homographyMatrix;
}

void Calibration::setCameraMatrix(const Mat &cameraMatrix)
{
    m_cameraMatrix = cameraMatrix;
}

void Calibration::setDistortionMatrix(const Mat &distortionMatrix)
{
    m_distortionMatrix = distortionMatrix;
}

Calibration::Calibration()
{

}
