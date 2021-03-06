#include "Chess.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>

int main(int argh, char* argv[])
{
	int webcam_id = 0;
	std::cout << "Opening webcam device." << std::endl;
	std::cout << "webcam ID = ? ";
	std::cin >> webcam_id;

	cv::VideoCapture cap(webcam_id);	//デバイスのオープン
	if (!cap.isOpened())
	{
		std::cerr << "Failed to open webcam." << std::endl;
		return -1;
	}

	wlib::CameraParameter camera_parameter;

	for (bool exit = false; !exit; )
	{
		cv::Mat frame;
		std::vector<cv::Point2f> corners;
		cap >> frame; // get a new frame from camera

		wlib::Chess chess;

		chess.detectChess(frame);
		
		cv::imshow("window", frame);//画像を表示

		switch (cv::waitKey(1)) {
		case 'q':
			exit = true; break;
		case 's':
			chess.addFrame();
			std::cout << "added samples. SAMPLE NUM = " << chess.getSampleCount() << std::endl;
			break;
		case 'c':
			chess.addFrame();
			std::cout << "added samples. SAMPLE NUM = " << chess.getSampleCount() << std::endl;
			std::cout << "Calculating..." << std::endl;
			camera_parameter = chess.calcParameter(cv::Size(frame.cols, frame.rows));
			std::cout << camera_parameter.toString() << std::endl;

			std::cout << "File exporting..." << std::endl;
			{
				std::ofstream ofst("param.txt");
				ofst << camera_parameter.toFileString() << std::endl;
			}
			std::cout << "Finished." << std::endl;
			break;
		}
	}
	cv::destroyAllWindows();
	return 0;
}
