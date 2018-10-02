#include "Chess.hpp"
#include <sstream>

std::vector<cv::Point3f> wlib::Chess::objects_;
std::vector<cv::Point2f> wlib::Chess::corners_;
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
	if (this->frame_corners_.empty()) return;
	this->_initialize();
	this->objects_.insert(this->objects_.end(), this->frame_objects_.begin(), this->frame_objects_.end());
	this->corners_.insert(this->corners_.end(), this->frame_corners_.begin(), this->frame_corners_.end());
	this->frame_corners_.clear();
}


wlib::CameraParameter wlib::Chess::calcParameter(const cv::Size & image_size) const
{
	assert(this->objects_.size() == this->corners_.size());

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
	out << "RESULT = " << this->result << std::endl;
	/*out << "(Fx, Fy) = (" << this->cameraMat.at<float>(0, 0) << ", " << this->cameraMat.at<float>(1, 1) << ")\t";
	out << "(Cx, Cy) = (" << this->cameraMat.at<float>(2, 0) << ", " << this->cameraMat.at<float>(2, 1) << ")" << std::endl;
	if (this->distCoeff.elemSize() >= 4) {
		out << "k1 = " << this->distCoeff.at<float>(0) << "k2 = " << this->distCoeff.at<float>(1) << "p1 = " << this->distCoeff.at<float>(2) << "p2 = " << this->distCoeff.at<float>(3);
	}
	if (this->distCoeff.elemSize() >= 5) {
		out << "k3 = " << this->distCoeff.at<float>(4);
	}
	out << std::endl;
	if (this->distCoeff.elemSize() >= 8) {
		out << "k4 = " << this->distCoeff.at<float>(5) << "k5 = " << this->distCoeff.at<float>(6) << "k6 = " << this->distCoeff.at<float>(7);
	}
	if (this->distCoeff.elemSize() >= 12) {
		out << "s1 = " << this->distCoeff.at<float>(8) << "s2 = " << this->distCoeff.at<float>(9) << "s3 = " << this->distCoeff.at<float>(10) << "s4 = " << this->distCoeff.at<float>(11);
	}*/
	out << std::endl;
	
	return out.str();
}
