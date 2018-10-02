#include "Chess.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

std::vector<std::vector<cv::Point3f>> wlib::Chess::objects_;
std::vector<std::vector<cv::Point2f>> wlib::Chess::corners_;
std::vector<cv::Point3f> wlib::Chess::frame_objects_;

namespace {
const cv::TermCriteria kTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 20, 0.03);
}

wlib::Chess::Chess(void) noexcept {}
wlib::Chess::~Chess(void) {}

bool wlib::Chess::detectChess(cv::Mat & input_frame)
{
	cv::Size kPatternSize(kPatternColumn, kPatternRow);
	bool ret = cv::findChessboardCorners(input_frame, kPatternSize, this->frame_corners_);
	if (ret) {
		cv::Mat gray(input_frame.cols, input_frame.rows, CV_8UC1);
		cv::cvtColor(input_frame, gray, CV_BGR2GRAY);
		cv::cornerSubPix(gray, this->frame_corners_, cv::Size(3, 3), cv::Size(-1, -1), kTermCriteria);
		
		cv::drawChessboardCorners(input_frame, kPatternSize, this->frame_corners_, ret);
	}
	return ret;
}

void wlib::Chess::addFrame(void)
{
	this->_initialize();
	if (this->frame_corners_.size() != this->frame_objects_.size()) return;
	this->objects_.emplace_back(this->frame_objects_);
	this->corners_.emplace_back(this->frame_corners_);
}

wlib::CameraParameter wlib::Chess::calcParameter(const cv::Size & image_size) const
{
	assert(this->objects_.size() == this->corners_.size());

	cv::InputArray obj(this->objects_), crn(this->corners_);
	for (int i = 0; i < this->corners_.size(); ++i) {
		std::cout << "[" << i << "] obj = " << obj.getMat(i).checkVector(3, CV_32F);
		std::cout << " crn = " << crn.getMat(i).checkVector(2, CV_32F) << std::endl;
	}

	CameraParameter ret;
	ret.result = 0;
	ret.result = cv::calibrateCamera(this->objects_, this->corners_, image_size, ret.cameraMat, ret.distCoeff, ret.rvecs, ret.tvecs);

	return ret;
}

std::size_t wlib::Chess::getSampleCount(void)
{
	return Chess::corners_.size();
}

void wlib::Chess::_initialize()
{
	// objects‚É’Ç‰Á
	if (Chess::frame_objects_.empty()) {
		for (int j = 0; j < kPatternRow; j++) {
			for (int k = 0; k < kPatternColumn; k++) {
				Chess::frame_objects_.emplace_back(j * kChessSize, k * kChessSize, 0.0f);
			}
		}
	}
}

std::string wlib::CameraParameter::toString() const
{
	std::stringstream out;
	out << std::fixed << std::setprecision(8);
	out << "RESULT = " << this->result << std::endl;
	out << "(Fx, Fy) = (" << this->cameraMat.at<double>(0, 0) << ", " << this->cameraMat.at<double>(1, 1) << ")\t";
	out << "(Cx, Cy) = (" << this->cameraMat.at<double>(0, 2) << ", " << this->cameraMat.at<double>(1, 2) << ")" << std::endl;
	if (this->distCoeff.cols >= 4) {
		out << "k1 = " << this->distCoeff.at<double>(0, 0) << " k2 = " << this->distCoeff.at<double>(0, 1) << " p1 = " << this->distCoeff.at<double>(0, 2) << " p2 = " << this->distCoeff.at<double>(0, 3);
	}
	if (this->distCoeff.cols >= 5) {
		out << " k3 = " << this->distCoeff.at<double>(0, 4);
	}
	out << std::endl;
	if (this->distCoeff.cols >= 8) {
		out << "k4 = " << this->distCoeff.at<double>(0, 5) << " k5 = " << this->distCoeff.at<double>(0, 6) << " k6 = " << this->distCoeff.at<double>(0, 7);
	}
	if (this->distCoeff.cols >= 12) {
		out << " s1 = " << this->distCoeff.at<double>(0, 8) << " s2 = " << this->distCoeff.at<double>(0, 9) << " s3 = " << this->distCoeff.at<double>(0, 10) << " s4 = " << this->distCoeff.at<double>(0, 11);
	}
	out << std::endl;
	
	return out.str();
}

std::string wlib::CameraParameter::toFileString() const
{
	std::stringstream out;
	out << std::fixed << std::setprecision(8);
	out << "#RESULT = " << this->result << std::endl;
	out << "Camera.fx: " << this->cameraMat.at<double>(0, 0) << std::endl;
	out << "Camera.fy: " << this->cameraMat.at<double>(1, 1) << std::endl;
	out << "Camera.cx: " << this->cameraMat.at<double>(0, 2) << std::endl;
	out << "Camera.cy: " << this->cameraMat.at<double>(1, 2) << std::endl << std::endl;

	out << "Camera.k1: " << this->distCoeff.at<double>(0, 0) << std::endl;
	out << "Camera.k2: " << this->distCoeff.at<double>(0, 1) << std::endl;
	out << "Camera.p1: " << this->distCoeff.at<double>(0, 2) << std::endl;
	out << "Camera.p2: " << this->distCoeff.at<double>(0, 3) << std::endl;

	return out.str();
}
