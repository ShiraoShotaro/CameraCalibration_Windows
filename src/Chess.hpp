#pragma once
#ifndef WLIB_CHESS_HPP
#define WLIB_CHESS_HPP

#include <vector>
#include <string>
#include "opencv/cv.hpp"

namespace wlib {

struct CameraParameter {
	cv::Mat cameraMat;
	cv::Mat distCoeff;
	std::vector<cv::Mat> rvecs, tvecs;
	double result;

	std::string toString() const;

	std::string toFileString() const;
};

class Chess {
public:

	Chess(void) noexcept;

	virtual ~Chess(void);

	static constexpr unsigned int kPatternColumn = 9;
	static constexpr unsigned int kPatternRow = 7;
	static constexpr float kChessSize = 28;
	static constexpr unsigned int kPatternSize = kPatternColumn * kPatternRow;

	bool detectChess(cv::Mat & input_frame);

	void addFrame(void);

	CameraParameter calcParameter(const cv::Size & image_size) const;

	static std::size_t getSampleCount(void);

private:

	static std::vector<std::vector<cv::Point3f>> objects_;
	static std::vector<cv::Point3f> frame_objects_;

	static std::vector<std::vector<cv::Point2f>> corners_;
	std::vector<cv::Point2f> frame_corners_;



	static void _initialize();

};

}

#endif
