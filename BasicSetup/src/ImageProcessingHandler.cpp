#include "ImageProcessingHandler.h"

#include <QImage>

#include <opencv2/opencv.hpp>

namespace
{
    //-----------------------------------
    cv::Mat convertQImageToCvMat(QImage& q_image)
    {
        auto size = q_image.size();
        // Ensure that the QImage format is RGB888, necessary to convert from QImage to cv::Mat
        if (q_image.format() != QImage::Format_RGB888)
        {
            q_image = q_image.convertToFormat(QImage::Format_RGB888);
        }

        //QImage to cv::Mat
        cv::Mat qimage_to_mat(size.height(), size.width(), CV_8UC3, const_cast<uchar*>(q_image.bits()), q_image.bytesPerLine());
         //Convert QImage's RGB to OpenCV's BGR, this is essential for proper image rendering
        cv::cvtColor(qimage_to_mat, qimage_to_mat, cv::COLOR_BGR2RGB);
        return qimage_to_mat;
    }

    //-----------------------------------
    QImage convertCvMatToQImage(const cv::Mat& cvImage)
    {
        cv::Mat cv_image_rgb;
        // Ensure it's in RGB format
        cv::cvtColor(cvImage, cv_image_rgb, cv::COLOR_BGR2RGB);

        //Save image locally to see if it's correctly converted
        //QImage test{cv_image_rgb.data, cv_image_rgb.cols, cv_image_rgb.rows, static_cast<qsizetype>(cv_image_rgb.step), QImage::Format_RGB888};
        //test.save("/Users/Hilton/Desktop/opencv_lesson/test.png");

        return {cv_image_rgb.data, cv_image_rgb.cols, cv_image_rgb.rows, static_cast<qsizetype>(cv_image_rgb.step), QImage::Format_RGB888};
    }

    //-----------------------------------
    void toGraScale(QImage& image)
    {
        QSize size = image.size();
        int row = size.width();
        int col = size.height();

        for(int x = 0; x < row; ++x)
        {
            for(int y = 0; y < col; ++y)
            {
                QColor rgb = image.pixelColor(x,y);

                int intensity_pixel = (rgb.red() + rgb.blue() + rgb.green())/3;

                QColor new_color(intensity_pixel,intensity_pixel,intensity_pixel);

                image.setPixelColor(x,y,new_color);
            }
        }
    }

    //-----------------------------------
    //Expects a grayscale image
    void toBinary(QImage& image, int threshold = 128, int upperLimit = 255)
    {
        if(!image.isGrayscale())
        {
            toGraScale(image);
        }

        QSize size = image.size();
        int row = size.width();
        int col = size.height();

        for(int x = 0; x < row; ++x)
        {
            for(int y = 0; y < col; ++y)
            {
                QColor rgb = image.pixelColor(x,y);
                //R,G and B are all the same for each pixel for a grayscale image
                //so we will only take one for our calculation
                int new_intensity = 0;
                if(rgb.red() > threshold)
                {
                    new_intensity = upperLimit;
                }

                QColor new_color(new_intensity,new_intensity,new_intensity);
                image.setPixelColor(x,y,new_color);
            }
        }
    }

    //-----------------------------------
    void toInverseBinary(QImage& image, int threshold = 128, int upperLimit = 255)
    {
        if(!image.isGrayscale())
        {
            toGraScale(image);
        }

        toBinary(image, threshold, upperLimit);

        QSize size = image.size();
        int row = size.width();
        int col = size.height();

        for(int x = 0; x < row; ++x)
        {
            for(int y = 0; y < col; ++y)
            {
                QColor rgb = image.pixelColor(x,y);
                //basically invert white with black and vice-versa
                int new_intensity = upperLimit;
                if(rgb.red() == upperLimit)
                {
                    new_intensity = 0;
                }

                QColor new_color(new_intensity,new_intensity,new_intensity);
                image.setPixelColor(x,y,new_color);
            }
        }
    }
}

//-----------------------------------
ImageProcessingHandler::ImageProcessingHandler(DataManagerImpl &dataManager)
    : m_datamanager(dataManager)
{
}

//-----------------------------------
void ImageProcessingHandler::convertToBinary()
{
    auto active_image = m_datamanager.getActiveImage();

    if(!active_image)
    {
        qWarning() << "No active image";
        return;
    }

    toBinary(active_image->getImage());
}

//-----------------------------------
void ImageProcessingHandler::convertToInverseBinary()
{
    auto active_image = m_datamanager.getActiveImage();

    if(!active_image)
    {
        qWarning() << "No active image";
        return;
    }

    toInverseBinary(active_image->getImage());
}

//-----------------------------------
void imageMouseCoordinates(int event, int x, int y, int flags, void *userdata)
{
    //On mac using the mac's x and y coordinates of mouse does not work.
    //So instead we use this function to detect left click on an image and hence giving us it's
    //proper x,y position. This we can use to transform our image
    if(event == cv::EVENT_LBUTTONDOWN)
    {
        qWarning() << x << y;
    }
}

//-----------------------------------
void ImageProcessingHandler::wrapActiveImage()
{
    auto active_image = m_datamanager.getActiveImage();

    if(!active_image)
    {
        qWarning() << "No active image";
        return;
    }

    auto qimage_to_mat = convertQImageToCvMat(active_image->getImage());

    //we use setMouseCallback function to detect the image x and y coordiantes aka our src_points
    cv::Point2f src_points[] =
    {
        cv::Point{116,571},
        cv::Point{2040,25},
        cv::Point{421, 1651},
        cv::Point{2347,1112}
    };

    int width = 2040 - 116; //topright x -  topleft x
    int height = 1112 - 25; //bottomright y - topright y

    cv::Point2f dest_points[] =
    {
        cv::Point{0,0},
        cv::Point{width,0},
        cv::Point{0,height},
        cv::Point{width,height}
    };
    cv::Mat prespective_mat = cv::getPerspectiveTransform(src_points, dest_points);
    cv::Mat warped_image;
    cv::warpPerspective(qimage_to_mat, warped_image, prespective_mat, cv::Point(width, height));

    QImage warped_qimage = convertCvMatToQImage(warped_image);

    //cv::imshow("Before", qimage_to_mat);
    //cv::imshow("After warp", warped_image);

    //the first parameter must be the image name we want the mouse click to react to.
    //cv::setMouseCallback("Before", &imageMouseCoordinates);

    //cv::waitKey();
}

//-----------------------------------
void ImageProcessingHandler::convertToGrayscale()
{
    auto active_image = m_datamanager.getActiveImage();

    if(!active_image)
    {
        qWarning() << "No active image";
        return;
    }

    toGraScale(active_image->getImage());
}
