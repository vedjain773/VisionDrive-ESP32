#include <iostream>
#include <opencv2/opencv.hpp>

double median(std::vector<double> vec)
{
    int size = vec.size();
    double value = 0;

    if (size == 0)
    {
        return value;
    }

    if (size % 2 == 0)
    {
        value = (vec[size / 2] + vec[size / 2 + 1]) * 0.5;
    }
    else
    {
        value = vec[size / 2];
    }

    return value;
}

std::vector<double> getMedianValues(cv::Mat img)
{
    std::vector<double> hue_vals;
    std::vector<double> sat_vals;
    std::vector<double> val_vals;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);

            hue_vals.push_back(pixel[0]);
            sat_vals.push_back(pixel[1]);
            val_vals.push_back(pixel[2]);
        }
    }

    std::vector<double> medians;
    medians.push_back(median(hue_vals));
    medians.push_back(median(sat_vals));
    medians.push_back(median(val_vals));

    return medians;
}

double distance(double x2, double x1, double y2, double y1)
{
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    return distance;
}