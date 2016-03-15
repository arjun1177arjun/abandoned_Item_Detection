//////define floor function
//
//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/video/video.hpp"
//
//using namespace std;
//using namespace cv;
//
////global vars
//Mat img;
//Mat imgLines;;
//Mat imgLines1;;
//Mat result;;
//int r = 20;
//int flag = 0;
//
//void CallBackFunc(int event, int x, int y, int flags, void* userdata);
//
//void defineFloor()
//{
//
//}
//
//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
//{
//	//update color box boundary around mouse
//
//	imgLines1 = Mat::zeros(img.size(), CV_8UC3);;
//	//bottom
//	int y1 = y - r;
//	for (int x1 = x - r; x1 < x + r; x1++)
//	{
//		if (y1 < 0 || x1 < 0 || y1 >= img.rows || x1 >= img.cols)
//			continue;
//		imgLines1.at<Vec3b>(y1, x1)[0] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[1] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[2] = 255;
//		//cout << to_string(x) + ";" + to_string(y) << endl;
//	}
//
//	//top
//	y1 = y + r;
//	for (int x1 = x - r; x1 < x + r; x1++)
//	{
//		if (y1 < 0 || x1 < 0 || y1 >= img.rows || x1 >= img.cols)
//			continue;
//		imgLines1.at<Vec3b>(y1, x1)[0] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[1] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[2] = 255;
//		//cout << to_string(x) + ";" + to_string(y) << endl;
//	}
//
//	//left
//	int x1 = x - r;
//	for (int y1 = y - r; y1 < y + r; y1++)
//	{
//		if (y1 < 0 || x1 < 0 || y1 >= img.rows || x1 >= img.cols)
//			continue;
//		imgLines1.at<Vec3b>(y1, x1)[0] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[1] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[2] = 255;
//		//cout << to_string(x) + ";" + to_string(y) << endl;
//	}
//
//	//right
//	x1 = x + r;
//	for (int y1 = y - r; y1 < y + r; y1++)
//	{
//		if (y1 < 0 || x1 < 0 || y1 >= img.rows || x1 >= img.cols)
//			continue;
//		imgLines1.at<Vec3b>(y1, x1)[0] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[1] = 0;
//		imgLines1.at<Vec3b>(y1, x1)[2] = 255;
//		//cout << to_string(x) + ";" + to_string(y) << endl;
//	}
//
//	//defining floor
//	if (flags == (EVENT_FLAG_CTRLKEY))			//EVENT_FLAG_LBUTTON
//	{
//		//cout << "WHOA" << endl;
//		for (int y1 = y - r; y1 < y + r; y1++)
//		for (int x1 = x - r; x1 < x + r; x1++)
//		{
//			if (y1 < 0 || x1 < 0 || y1 >= img.rows || x1 >= img.cols)
//				continue;
//			imgLines.at<Vec3b>(y1, x1)[0] = 255;
//			//the blue channel is 255 if it is floor else 0
//			//cout << to_string(x) + ";" + to_string(y) << endl;
//		}
//	}
//	if (flags == (EVENT_FLAG_LBUTTON))
//	{
//		r++;
//	}
//	if (flags == (EVENT_FLAG_RBUTTON))
//	{
//		r--;
//	}
//	if (flags == (EVENT_FLAG_SHIFTKEY + EVENT_FLAG_CTRLKEY))
//	{
//		result = imgLines;
//		imwrite("C:\\Users\\Lenovo\\Desktop\\try15.jpg", result);
//		flag = 1;
//	}
//	imshow("Floor", imgLines);
//	waitKey(30);
//}
//
//int main(int argc, char** argv)
//{
//	VideoCapture cap;
//	cap.open("C:\\Users\\Lenovo\\Desktop\\videoCase\\convt\\try15.avi");
//	if (!cap.isOpened()){ cout << "ERROR" << endl; }
//
//	for (int i = 1; i <= 600;i++)
//	cap.read(img);
//
//	namedWindow("Floor define Window", WINDOW_AUTOSIZE);
//	namedWindow("Floor", WINDOW_AUTOSIZE);
//
//	imgLines = Mat::zeros(img.size(), CV_8UC3);;
//	imgLines1 = Mat::zeros(img.size(), CV_8UC3);;
//
//	//namedWindow("Floor define Window");
//
//	while (true)
//	{
//		//set the callback function for any mouse event
//		setMouseCallback("Floor define Window", CallBackFunc, NULL);
//		//waitKey(30);
//
//		Mat res = img + imgLines1;
//		//show the image
//		imshow("Floor define Window", res);
//		waitKey(30);
//		//cout << to_string(fg) << endl;
//		//cout << to_string(flag) << endl;
//		if (flag == 1)
//			break;
//	}
//	return 0;
//}