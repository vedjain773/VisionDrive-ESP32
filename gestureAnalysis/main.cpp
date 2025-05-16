#include <bits/stdc++.h>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <iostream>
#include <opencv2/opencv.hpp>

double distance(double x2, double x1, double y2, double y1)
{
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    return distance;
}

int main()
{
    ix::WebSocket webSocket;
    std::string url("ws://192.168.1.2/ws");

    webSocket.setUrl(url);

    std::cout << "Connecting to... " << url << std::endl;
    
    webSocket.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                std::cout << "received message: " << msg->str << std::endl;
                std::cout << "> " << std::flush;
            }
            else if (msg->type == ix::WebSocketMessageType::Open)
            {
                std::cout << "Connection established" << std::endl;
                std::cout << "> " << std::flush;
            }
            else if (msg->type == ix::WebSocketMessageType::Error)
            {
                std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
                std::cout << "> " << std::flush;
            }
        }
    );

    webSocket.start();
    webSocket.send("hello world");

    cv::VideoCapture video(0);
    while (video.isOpened())
    {
        cv::Mat frame;
        video.read(frame);

        cv::Mat frame_hsv;
        cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);
        cv::Mat frame_mask;

        cv::Scalar lowerb(0, 48, 80);
        cv::Scalar upperb(20, 235, 255);

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
            // std::cout << "Forward" << std::endl;
            webSocket.send("forward");
        }
        else
        {
            // std::cout << "Backward" << std::endl;
            webSocket.send("backward");
        }

        cv::namedWindow("Output", cv::WINDOW_NORMAL);
        cv::resizeWindow("Output", 640, 480);
        cv::imshow("Output", frame);

        cv::waitKey(500);
    }

    return 0;
}