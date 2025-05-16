#include <bits/stdc++.h>
#include <iostream>
#include "operations.hpp"
#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture video(0);
    int h = 0, s = 0, v = 0;

    while (video.isOpened()) {
        cv::Mat frame;
        video.read(frame); // Read one frame from the video
        cv::namedWindow("Image", cv::WINDOW_NORMAL);
        cv::resizeWindow("Image", 640, 480);
        cv::imshow("Image", frame);

        if (cv::waitKey(10) == 'q') // Wait for "q" key to be pressed
        {
            cv::Rect bbox = cv::selectROI("Image", frame, false, false); // Select a region of interest on the image
            cv::Mat hsv;
            cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); // Convert BGR color model to HSV
            cv::Mat obj_img = hsv(cv::Rect(bbox.x, bbox.y, bbox.width, bbox.height));
            std::vector<double> medians = getMedianValues(obj_img); // Find the median of HSV in the selected region.
            h = medians[0];
            s = medians[1];
            v = medians[2];
            break;
        }
    }

    while (video.isOpened())
    {
        cv::Mat frame;
        video.read(frame);

        cv::Mat frame_hsv;
        cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);
        cv::Mat frame_mask;

        cv::Scalar lowerb(h - 5, std::max(0, s - 50), std::max(0, v - 50));
        cv::Scalar upperb(h + 5, std::min(s + 50, 255), std::min(v + 50, 255));

        cv::inRange(frame_hsv, lowerb, upperb, frame_mask);

        cv::Mat kernel = (cv::Mat_<double>(3, 3) << 1., 1., 1.,
                          1., 1., 1.,
                          1., 1., 1.);

        cv::Mat cleaned;
        
        cv::morphologyEx(frame_mask, cleaned, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(cleaned, cleaned, cv::MORPH_CLOSE, kernel);
        cv::GaussianBlur(frame_mask, frame_mask, cv::Size(5, 5), 0);
        
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> heirarchy;
        cv::findContours(cleaned, contours, heirarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        int idx = 0, counter = 0;
        double current_area = 0;
        double max_area = 0;

        for (auto n : contours)
        {
            current_area = cv::contourArea(n);

            if (current_area > max_area)
            {
                max_area = current_area;
                idx = counter;
            }

            counter++;
        }

        double epsilon = 0.015 * cv::arcLength(contours[idx], true);

        cv::approxPolyDP(contours[idx], contours[idx], epsilon, true);

        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contours[idx], center, radius);
        cv::circle(frame, center, 5, cv::Scalar(255, 0, 0), -1);
        cv::drawContours(frame, contours, idx, cv::Scalar(0, 255, 255));

        int count = 0;
        for (int i = 0; i < contours[idx].size(); i++)
        {
            auto n = contours[idx][i];
            if (n.y <= center.y)
            {
                if (distance(n.x, center.x, n.y, center.y) > 110)
                {
                    cv::circle(frame, n, 5, cv::Scalar(0, 0, 255), -1);
                    count++;
                }
            }
        }

        if (count >= 3)
        {
            std::cout << "Forward" << std::endl;
        }
        else
        {
            std::cout << "Backward" << std::endl;
        }

        cv::namedWindow("Output", cv::WINDOW_NORMAL);
        cv::resizeWindow("Output", 640, 480);
        cv::imshow("Output", frame);

        cv::waitKey(500);
    }

    return 0;
}