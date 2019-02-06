/*Klasa obsługi obrazów w skali szarości*/
#ifndef IMAGE_H
#define IMAGE_H
#include <opencv2/opencv.hpp>
#include <string>

#define WIDTH 2900
#define HEIGHT 2100
#define SCALE 0.0847

using namespace cv;
using namespace std;

class Image
   : public Mat
{
private:
    string m_inputImageFileName;
    string m_outputImageFileName;
    Mat m_input_image;

    void m_makeUndistortion();
    void m_makeHomography();
public:
    Image();
    Image(string inputImageFileName);
    void makeCalibration();
};

#endif // IMAGE_H
