// guidedfilter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>

using namespace cv;

static cv::Mat boxfilter(const cv::Mat &I, int r)
{
	cv::Mat result;
	cv::blur(I, result, cv::Size(r, r), cv::Point(-1, -1), cv::BORDER_REPLICATE);
	return result;
}

static cv::UMat boxfilter2(const cv::UMat &I, int r)
{
	cv::UMat result;
	cv::blur(I, result, cv::Size(r, r), cv::Point(-1, -1), cv::BORDER_REPLICATE);
	return result;
}

int main()
{
	cv::UMat I2;
	cv::imread("D0tOR_GU0AE3ZJO.bmp", 0).copyTo(I2);

	cv::UMat p2 = I2;

	int r = 17; // try r=2, 4, or 8
	double eps = 0.2 * 0.2; // try eps=0.1^2, 0.2^2, 0.4^2

	eps *= 255 * 255;   // Because the intensity range of our images is [0, 255]

	cv::UMat mean_p2 = boxfilter2(p2, r);
	cv::UMat mean_I2 = boxfilter2(I2, r);
	cv::UMat mean_Ip2 = boxfilter2(I2.mul(p2), r);
	cv::UMat mean_II2 = boxfilter2(I2.mul(I2), r);

	cv::UMat var_I2;
	subtract(mean_II2, mean_I2.mul(mean_I2), var_I2);

	cv::UMat cov_Ip2;
	subtract(mean_Ip2, mean_I2.mul(mean_p2), cov_Ip2); // this is the covariance of (I, p) in each local patch.

	UMat epsadd2;
	add(var_I2, eps, epsadd2);

	cv::UMat a2;
	divide(cov_Ip2, epsadd2, a2); // Eqn. (5) in the paper;

	cv::UMat b2;
	subtract(mean_p2, a2.mul(mean_I2), b2); // Eqn. (6) in the paper;

	cv::UMat mean_a2 = boxfilter2(a2, r);
	cv::UMat mean_b2 = boxfilter2(b2, r);

	cv::UMat q2;
	add(mean_a2.mul(I2), mean_b2, q2);

	cv::Mat q = q2.getMat(ACCESS_RW);
	cv::Mat I = I2.getMat(ACCESS_RW);

	imshow("ORG", I);
	imshow("q", q);
	waitKey(0);

#if 0
	cv::UMat I2;
	cv::imread("D0tOR_GU0AE3ZJO.jpg", 0).copyTo(I2);
	cv::Mat I = I2.getMat(ACCESS_RW);

	cv::UMat p2 = I2;
	cv::Mat p = I2.getMat(ACCESS_RW);

	int r = 4; // try r=2, 4, or 8
	double eps = 0.2 * 0.2; // try eps=0.1^2, 0.2^2, 0.4^2

	eps *= 255 * 255;   // Because the intensity range of our images is [0, 255]

	cv::UMat mean_p2 = boxfilter2(p2, r);
	cv::Mat mean_p = mean_p2.getMat(ACCESS_RW);


	cv::UMat mean_I2 = boxfilter2(I2, r);
	cv::Mat mean_I = mean_I2.getMat(ACCESS_RW);

	cv::UMat mean_Ip2 = boxfilter2(I2.mul(p2), r);
	cv::Mat mean_Ip = mean_Ip2.getMat(ACCESS_RW);

	cv::UMat mean_II2 = boxfilter2(I2.mul(I2), r);
	cv::Mat mean_II = mean_II2.getMat(ACCESS_RW);

	cv::UMat var_I2;
	subtract(mean_II2, mean_I2.mul(mean_I2), var_I2);
	cv::Mat var_I = var_I2.getMat(ACCESS_RW);

	cv::UMat cov_Ip2;
	subtract(mean_Ip2,mean_I2.mul(mean_p2), cov_Ip2); // this is the covariance of (I, p) in each local patch.
	cv::Mat cov_Ip = cov_Ip2.getMat(ACCESS_RW);

	UMat epsadd2;
	add(var_I2, eps, epsadd2);
	cv::Mat epsadd = epsadd2.getMat(ACCESS_RW);

	cv::UMat a2;
	divide (cov_Ip2 , epsadd2, a2); // Eqn. (5) in the paper;
	cv::Mat a = a2.getMat(ACCESS_RW);

	//	matMulDeriv
	cv::UMat b2;
	subtract(mean_p2,a2.mul(mean_I2),b2); // Eqn. (6) in the paper;
	cv::Mat b = b2.getMat(ACCESS_RW);

	cv::UMat mean_a2;
	mean_a2 = boxfilter2(a2, r);
	cv::Mat mean_a = mean_a2.getMat(ACCESS_RW);

	cv::UMat mean_b2;
	mean_b2 = boxfilter2(b2, r);
	cv::Mat mean_b = mean_b2.getMat(ACCESS_RW);

	cv::UMat q2;
	add(mean_a2.mul(I2), mean_b2,q2);
	cv::Mat q = q2.getMat(ACCESS_RW);

	imshow("ORG", I);
	imshow("q", q);
	waitKey(0);
#endif
}

#if 0
int main()
{
	cv::Mat I = cv::imread("D0tOR_GU0AE3ZJO.jpg", 0);
	cv::Mat p = I;

	int r = 4; // try r=2, 4, or 8
	double eps = 0.2 * 0.2; // try eps=0.1^2, 0.2^2, 0.4^2

	eps *= 255 * 255;   // Because the intensity range of our images is [0, 255]

	cv::Mat mean_p = boxfilter(p, r);
	cv::Mat mean_I = boxfilter(I, r);
	cv::Mat mean_Ip = boxfilter(I.mul(p), r);
	cv::Mat mean_II = boxfilter(I.mul(I), r);
	cv::Mat var_I = mean_II - mean_I.mul(mean_I);
	cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); // this is the covariance of (I, p) in each local patch.

	cv::Mat a = cov_Ip / (var_I + eps); // Eqn. (5) in the paper;
	cv::Mat b = mean_p - a.mul(mean_I); // Eqn. (6) in the paper;

	cv::Mat mean_a = boxfilter(a, r);
	cv::Mat mean_b = boxfilter(b, r);

	cv::Mat q = mean_a.mul(I) + mean_b;

	imshow("ORG", I);
	imshow("q", q);
	waitKey(0);
}
#endif
#if 0
	UMat src;

	imread("D0tOR_GU0AE3ZJO.jpg").copyTo(src);
	imshow("src", src);

	waitKey(0);
    return 0;

	cv::Mat mean_p = boxfilter(p, r);
	cv::Mat mean_Ip = boxfilter(I.mul(p), r);
	cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); // this is the covariance of (I, p) in each local patch.

	cv::Mat a = cov_Ip / (var_I + eps); // Eqn. (5) in the paper;
	cv::Mat b = mean_p - a.mul(mean_I); // Eqn. (6) in the paper;

	cv::Mat mean_a = boxfilter(a, r);
	cv::Mat mean_b = boxfilter(b, r);

	return mean_a.mul(I) + mean_b;
#endif
