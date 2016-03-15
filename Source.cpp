//All levels of PETS DATABASE;

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream> 
#include <math.h>       /* sqrt */
#include "speech.h"
#include <ctime>
#include "opencv2/objdetect/objdetect.hpp"
#include <fstream>
#include <ctime>
#include <cmath>        // std::abs

using namespace cv;
using namespace std;

//global variables
Mat img, bg, dst, gray_dst, thres_dst, blur_img, kernel, fgmg2, element, imgLines, imgLines1, img1, img_all_contour;
int photo = 0;
//Mat floor;
Ptr <BackgroundSubtractorMOG2> pmog2;
int flag = 0;
int a, dsum, dmax = 0;
int dmin = 100;
int len = 0;
int frameNum = 0;
double dist; int c = 0;
int z, u, speed, i, f, w, e, t; int r = 20;//for loop vars
int N = 2;
int as = -1;

double oldx[100], newx[100]; int id[100]; int tolerance[100], frame[100], area[100], area_new[100], roi_hs[100];
double oldy[100], newy[100];
int tempBagTag[100], permaBagTag[100], bagToll[100];
int manToll[100];
int birth[100];				//store birth frame num of each id
int bag[100], owner[100];	//1 for that id index if bag or owner respectively
int numberOfBags = 0;	// i.e number of total linkages
int newLinkageFlag = 0;
int p;//absdiff in linkage function
int ownerOfBag[100];
int abandonedTag[100];
int clickTag[100];
int whichNumBag[100];
int whichNumOwner[100];
int abandonedFrameNum[100];
vector<Vec3b> colors;
int num;
int bagMax;
int drawContour = 1;	//if 0 means it is contour of man
int clickx = -1;
int clicky = -1;
int dispOwnerx = -1;
int dispOwnery = -1;
int pauseKey = 0;
int AHour[100];
int AMin[100];
int ASec[100];
int timVar = 0;

//function definitions
void dumpContour();
void checkAbandoned();
void linkage();
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

CascadeClassifier body_cascade;

int main(int argc, char *argv[])
{
	//colors
	for (z = 1; z <= 25; z++)
	{
		colors.push_back(Vec3b((uchar)0, (uchar)0, (uchar)255));		//RED
		colors.push_back(Vec3b((uchar)51, (uchar)255, (uchar)153));		//Green
		colors.push_back(Vec3b((uchar)102, (uchar)153, (uchar)255));	//Orange
		colors.push_back(Vec3b((uchar)255, (uchar)255, (uchar)0));		//Blue
	}

	//voice
	Speech::Voice voice;
	voice.setRate(0);
	//PETS 1 	F:\\data1\\data1\\pets1.avi");
	//PETS 3	F:\\data1\\pets s3\\pets3.avi");		//takes away bag
	//PETS 4    F:\\data1\\pets s4\\pets4.avi");		//start at frame 400
	//PETS 5    F:\\data1\\pets s5\\pets5.avi");					//caution change area
	//PETS 6	F:\\data1\\pets full s6\\S6-T3-H\\pets6.avi");		//start at frame 400  dual owner
	//PETS 7	F:\\data1\\pets s7\\pets7.avi");

	//num=8		F:\\dataset\\try3.avi");		//1 owner 1 bag
	//num=9		F:\\dataset\\try5.avi");		//1 owner 1 bag
	//num=9		F:\\dataset\\try6.avi");		//1 owner 1 bag and takes away (floor try5)
	//num=9		F:\\dataset\\try7.avi");		//1 owner 1 bag and leaves frame (floor try5)
	//num=10	F:\\dataset\\try8.avi");		//pande sir overlap with abandoned bags
	//num=11	F:\\dataset\\try9.avi");		//owner returns and maintains close proximity to bag
	//num=11	F:\\dataset\\try10.avi");		//owner returns and maintains close proximity to bag (proper)
	//num=12	F:\\dataset\\try11.avi");		//owner returns and maintains close proximity to bag
	//num=13	F:\\dataset\\try13.avi");		//owner returns and maintains close proximity to bag
	//num=14	F:\\dataset\\try14.avi");		//owner returns and maintains close proximity to bag
	//num=15	F:\\dataset\\try15.avi");		//owner returns and maintains close proximity to bag
	//area;start frame bg;clear prev image data;correct vids1,3,5,6

	num = 5;
	//num = stoi(argv[1]);

	//////////////////////////////////for exe file///
	//acc to num get video file and floor
	//num = stoi(argv[1]);

	//num = 5;
	//LOAD VIDEO FILE IN CAP object
	VideoCapture cap;
	Mat floor;
	if (num == 1)
	{
		cap.open("F:\\data1\\data1\\pets1.avi");
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 3)
	{
		cap.open("F:\\data1\\Video-Cases\\pets3.avi");		//takes away bag
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 4)
	{
		cap.open("F:\\data1\\Video-Cases\\pets4.avi");		//start at frame 400
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 5)
	{
		cap.open("F:\\data1\\Video-Cases\\pets5.avi");					//caution change area
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 6)
	{
		cap.open("F:\\data1\\Video-Cases\\pets6.avi");		//start at frame 400  dual owner
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 7)
	{
		cap.open("F:\\data1\\pets s7\\pets7.avi");
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\result.jpg");
	}
	else
	if (num == 9)
	{
		cap.open("F:\\data1\\convt\\try6.avi");		//1 owner 1 bag and takes away (floor try5)
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try6.jpg");
	}
	else
	if (num == 10)
	{
		cap.open("F:\\data1\\convt\\try8.avi");		//pande sir overlap with abandoned bags
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try8.jpg");
	}
	else
	if (num == 11)
	{
		cap.open("F:\\data1\\convt\\try10.avi");		//owner returns and maintains close proximity to bag (proper)
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try10.jpg");
	}
	else
	if (num == 12)
	{
		cap.open("F:\\data1\\convt\\try11.avi");		//owner returns and maintains close proximity to bag
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try11.jpg");
	}
	else
	if (num == 13)
	{
		cap.open("F:\\data1\\convt\\try13.avi");		//owner returns and maintains close proximity to bag
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try13.jpg");
	}

	else
	if (num == 14)
	{
		cap.open("F:\\data1\\convt\\try14.avi");		//owner returns and maintains close proximity to bag
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try14.jpg");
	}
	else
	if (num == 15)
	{
		cap.open("F:\\data1\\convt\\try15.avi");		//owner returns and maintains close proximity to bag
		floor = imread("C:\\Users\\Lenovo\\Desktop\\floor-pics\\try15.jpg");
	}
	//num=8		F:\\dataset\\try3.avi");		//1 owner 1 bag
	//num=9		F:\\dataset\\try5.avi");		//1 owner 1 bag
	//num=9		F:\\dataset\\try6.avi");		//1 owner 1 bag and takes away (floor try5)
	//num=9		F:\\dataset\\try7.avi");		//1 owner 1 bag and leaves frame (floor try5)
	//num=10	F:\\dataset\\try8.avi");		//pande sir overlap with abandoned bags
	//num=11	F:\\dataset\\try9.avi");		//owner returns and maintains close proximity to bag
	//num=11	F:\\dataset\\try10.avi");		//owner returns and maintains close proximity to bag (proper)
	//num=12	F:\\dataset\\try11.avi");		//owner returns and maintains close proximity to bag
	//num=13	F:\\dataset\\try13.avi");		//owner returns and maintains close proximity to bag
	//num=14	F:\\dataset\\try14.avi");		//owner returns and maintains close proximity to bag
	//num=15	F:\\dataset\\try15.avi");		//owner returns and maintains close proximity to bag
	/////////////////////////////////////////////////

	//cout << to_string(num) << endl;
	if (!cap.isOpened()){ cout << "ERROR" << endl; }

	//bg frame
	int start;
	if (num == 4)
		start = 381;
	else
	if (num == 6)
		start = 321;
	else
	if (num == 5)
		start = 451;
	else
	if (num == 13)
		start = 51;
	else
	if (num == 14)
		start = 201;
	else
	if (num == 15)
		start = 301;
	else
		start = 1;

	////

	//bagMax
	if (num == 5)
		bagMax = 2500;
	else
	if (num == 8)
		bagMax = 5000;
	else
	if (num == 9)
		bagMax = 4000;
	else
	if (num == 10)
		bagMax = 2000;
	else
	if (num == 11)
		bagMax = 4000;
	else
	if (num == 12)
		bagMax = 2500;
	else
	if (num == 13)
		bagMax = 2600;
	else
	if (num == 14)
		bagMax = 2600;
	else
	if (num == 15)
		bagMax = 2600;
	else
		bagMax = 2000;
	//
	// N calib
	if (num == 3 || num == 4 || num == 15)
		N = 4;

	for (int n = 1; n < start; n++)
	{
		cap.read(bg); frameNum++;
	}

	pmog2 = createBackgroundSubtractorMOG2(false);

	//////////////////initialisations for first case

	for (u = 0; u < 100; u++)
	{
		roi_hs[u] = area_new[u] = area[u] = oldx[u] = oldy[u] = newx[u] = newy[u] = id[u] = tolerance[u] = frame[u] = tempBagTag[u] = permaBagTag[u] = bagToll[u] = manToll[u] = 0;
		bag[u] = owner[u] = abandonedTag[u] = abandonedFrameNum[u] = 0, clickTag[u] = 0;
		ownerOfBag[u] = -1;
		whichNumBag[u] = whichNumOwner[u] = -1;
		AHour[u] = -1; AMin[u] = -1; ASec[u] = -1;
	}
	///////////////////////////////////////////////////
	if (!body_cascade.load("F:\\IDM\\compressed\\hs.xml\\HS.xml"))
	{
		printf("--(!)Error loading\n"); return -1;
	};

	while (true)
	{
		if (waitKey(1) == 61)
		{
			N++;
			cout << "N=" + to_string(N) << endl;
		}
		else
		if (waitKey(1) == 45)
		{
			if (num == 3 || num == 4 || num == 15)
				N = 4;
			else
				N = 2;
			cout << "N=normalised" << endl;
		}
		//N is the speedup of the video as compared to original video. At N=2 the video plays at the same rate as the original.
		//so for quick program test use N=4,6	we don't have all day
		//N = 4;
		for (int speed = 1; speed <= N; speed++)
		{
			cap.read(img);
			frameNum++;
		}
		img.copyTo(img1);
		img.copyTo(img_all_contour);
		//imshow("INPUT", img); waitKey(30);

		pmog2->apply(img, fgmg2, 0);
		element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
		morphologyEx(fgmg2, fgmg2, MORPH_OPEN, element);
		threshold(fgmg2, thres_dst, 254, 255, THRESH_BINARY);

		//imshow("Thresholded", thres_dst);
		//waitKey(30);

		kernel = getStructuringElement(MORPH_RECT, Size(15, 15), Point(-1, -1));
		morphologyEx(thres_dst, thres_dst, MORPH_CLOSE, kernel);

		///////////////////////////////////////DRAWING CONTOURS BIGGER THAN A VALUE---------------------
		vector<vector<Point> > contours;
		vector<vector<Point> > cont_new(100);
		vector<Vec4i> hierarchy;
		findContours(thres_dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());

		newLinkageFlag = 0;
		for (i = 0; i < contours.size(); i++)
		{
			drawContour = 1;
			if (contourArea(contours[i])>100) //Bag is around 650 the area unit is in pixels
			{
				//check if its lowest point is on "floor" or not
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));

				Rect ri = boundingRect(contours[i]);
				Mat roi = img_all_contour(ri);
				//imshow("ROI", roi);

				vector<Rect> humans;

				equalizeHist(dst, dst);
				body_cascade.detectMultiScale(roi, humans, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

				for (size_t ij = 0; ij < humans.size(); ij++)
				{
					Point center(humans[ij].x + humans[ij].width*0.5, humans[ij].y + humans[ij].height*0.5);
					ellipse(roi, center, Size(humans[ij].width*0.5, humans[ij].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
					//rectangle(img, humans[ij].tl(), humans[ij].br(), Scalar(255, 0, 255), 4, 8, 0);
				}

				//rectangle(img, boundRect[i].tl(), boundRect[i].br(), (0, 0, 255), 0, 8, 0);
				int lowX = boundRect[i].x + boundRect[i].width / 2;
				int lowY = boundRect[i].y + boundRect[i].height;
				//putText(img, "P", Point(lowX,lowY), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 255, 0), 1, CV_AA);
				if (floor.at<Vec3b>(lowY, lowX)[0] == 0)
				{
					//drawContours(img, contours, i, Scalar(100, 100, 100), 2, 8, hierarchy, 0, Point());
					continue;
				}

				/////////////////////// height>width condition//////
				//if (boundRect[i].height < boundRect[i].width)
				//	continue;
				////////////////////////////////////////////////////

				//cout << "contour AREA bigger than " << endl;
				Moments newMoments = moments(contours[i]);
				flag = 0;
				newx[i] = newMoments.m01 / newMoments.m00;
				newy[i] = newMoments.m10 / newMoments.m00;
				area_new[i] = contourArea(contours[i]);

				c = 0;
				//search to give id
				for (r = 0; r < 100; r++)
				{
					dist = sqrt((newx[i] - oldx[r])*(newx[i] - oldx[r]) + (newy[i] - oldy[r])*(newy[i] - oldy[r]));
					if (dist < 50 && area_new[i] >= (area[r] / 2))
					{
						if (humans.size()>0) roi_hs[r] = -1;

						//////////////////////////click feature//
						if (clickx != -1 && clicky != -1)
						{
							if (clickx >= boundRect[i].x && clickx <= boundRect[i].x + boundRect[i].width)
							if (clicky >= boundRect[i].y && clicky <= boundRect[i].y + boundRect[i].height)
							{
								clickTag[r] = 1; clickx = -1; clicky = -1;
							}
						}

						//display owner feature//
						if (dispOwnerx != -1 && dispOwnery != -1)
						{
							if (dispOwnerx >= boundRect[i].x && dispOwnerx <= boundRect[i].x + boundRect[i].width)
							if (dispOwnery >= boundRect[i].y && dispOwnery <= boundRect[i].y + boundRect[i].height)
							{
								//display owner
								for (int y = 1; y < 100; y++)
								{
									if (whichNumBag[y] == r)
									{
										Mat xyz1 = imread("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(y) + "\\owner" + to_string(y) + ".jpg");
										imshow("OWNER", xyz1);
										waitKey(30);
										Mat xyz2 = imread("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(y) + "\\SCENE" + to_string(y) + ".jpg");
										imshow("SCENE", xyz2);
										waitKey(30);
									}
								}
								clickx = -1; clicky = -1;
							}
						}
						/////////////////////////
						/////////////////////////////////////////

						if (dist < 2 && roi_hs[r] == 0)
						{
							bagToll[r]++;
							if (bagToll[r] > 2)
							{
								manToll[r] = 0;
								permaBagTag[r] = 1;
								if (bag[r] == 1)
								{
									cout << "checkAbandoned()------------------------" << endl;
									checkAbandoned();
								}
								else
								{
									//numberOfBags++;
									linkage();
								}
							}
						}
						else
						{
							if (abandonedTag[r] == 1)
							{
								manToll[r]++;
								cout << "mantoll" + to_string(manToll[r]) << endl;
								if (manToll[r] >= 35)
								{
									//delete bag contour
									cout << "deleting bag of id;" + to_string(e) << endl;
									bag[e] = 0;
									//delete owner and whichNumBag linkage
									owner[ownerOfBag[e]] = -1;
									for (int s = 0; s < 100; s++)
									if (whichNumBag[s] == e)
									{
										whichNumOwner[s] = -1; whichNumBag[s] = -1; cout << "bag was number" + to_string(s) << endl;
									}
									ownerOfBag[e] = -1;
									abandonedTag[e] = 0;
									abandonedFrameNum[e] = 0;

									oldx[e] = 0;
									oldy[e] = 0;
									id[e] = 0;
									tolerance[e] = 0;
									//tempBagTag[e] = 0;
									permaBagTag[e] = 0;
									abandonedTag[e] = 0;
									clickTag[e] = 0;
									abandonedFrameNum[e] = 0;
									ownerOfBag[e] = -1;
									bagToll[e] = 0;
									manToll[e] = 0;
									birth[e] = 0;
								}
								flag = 1;
								break;
							}
							if (permaBagTag[r] == 1)
							{
								manToll[r]++;
								if (manToll[r] >= 5)
								{
									permaBagTag[r] = 0;
									bagToll[r] = 0;
									//remove bag and owner linkage and setups
									cout << "deleting bag of id;" + to_string(r) << endl;
									bag[r] = 0;
									//delete owner and whichNumBag linkage
									owner[ownerOfBag[r]] = -1;
									for (int s = 0; s < 100; s++)
									if (whichNumBag[s] == r)
									{
										whichNumOwner[s] = -1; whichNumBag[s] = -1;
									}
									ownerOfBag[r] = -1;
									abandonedTag[r] = 0;
									abandonedFrameNum[r] = 0;

									permaBagTag[r] = 0;
									abandonedTag[r] = 0;
									clickTag[e] = 0;
									ownerOfBag[r] = -1;
									bagToll[r] = 0;
									manToll[r] = 0;
									birth[r] = frameNum;
								}
							}
						}

						//printing appropriately
						if (abandonedTag[r] == 1)
						{
							int g1 = 0;
							//identify which num bag is this tag r
							for (int h = 0; h < 100; h++)
							{
								if (clickTag[r] == 1)
								{
									cout << "clickTag" << endl;
									g1 = 1;
									break;
								}
								if (whichNumBag[h] == r)
								{
									putText(img, "ABANDONED" + to_string(h), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA); g1 = 1;

									///////////getting current time/////////
									time_t currentTime;
									struct tm *localTime;

									time(&currentTime);                   // Get the current time
									localTime = localtime(&currentTime);  // Convert the current time to the local time

									int Hour = localTime->tm_hour;
									int Min = localTime->tm_min;
									int Sec = localTime->tm_sec;
									////////////////////////////////////////
									int numMin;
									int numSec;
									if (AMin[h] == Min)
									{
										numMin = 0;
										numSec = Sec - ASec[h];
									}
									if (Min - AMin[h]>0)
									{
										numMin = (Min - AMin[h] - 1) + (60 - ASec[h] + Sec) / 60;
										numSec = (60 - ASec[h] + Sec) % 60;
									}
									if (numMin != 0)
										putText(img, to_string(numMin) + "min", Point(newy[i], newx[i] + 20), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA); g1 = 1;
									putText(img, to_string(numSec) + "sec", Point(newy[i] + 30, newx[i] + 40), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA); g1 = 1;
									cout << "abandonedFrameNUm" + to_string(abandonedFrameNum[r]) << endl;
									//if (abs(frameNum - abandonedFrameNum[r])>300)
									//{
									//if (frameNum % 10 == 0)
									//cout << '\a';
									//if ((frameNum - abandonedFrameNum[r]) > 300)
									//{
									//	cout << "------------------->>>>>>>>" + to_string(frameNum - abandonedFrameNum[r]) << endl;
									//	if ((frameNum - abandonedFrameNum[r]) % 2 == 0)
									//	if ((frameNum - abandonedFrameNum[r]) % 360 == 0)
									//	{
									//		voice.speak("Black Abandoned Bag at terminal 1");
									//		cout << "---->>" + to_string((frameNum - abandonedFrameNum[r])) << endl;
									//		//waitKey(0);
									//	}
									//	else
									if (Sec - ASec[h] == 15 || Sec - ASec[h] == 45)
									{
										voice.speak("Black Abandoned Bag at terminal 1");
										cout << "---->>" + to_string((frameNum - abandonedFrameNum[r])) << endl;
										//waitKey(0);
									}
									//}
									//if (frameNum % 110 == 0)
									//	voice.speak("in electronics section");
									//}
									//waitKey(1000);
									break;
								}
							}
							if (g1 == 0)
							{
								cout << "LOOK FOR G1 PROBLEM" + to_string(as) << endl;
								//waitKey(0);
							}
						}
						else
						if (permaBagTag[r] == 1 && bag[r] == 1)
						{
							int g2 = 0;
							//identify which num bag is this tag r
							for (int h = 0; h < 100; h++)
							{
								if (whichNumBag[h] == r)
								{
									//putText(img, "BAG" + to_string(h), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA); g2 = 1;
									Rect ri = boundingRect(contours[i]);
									Mat roi = img1(ri);
									imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(h) + "\\bag" + to_string(h) + ".jpg", roi);
									break;
								}
							}
							if (g2 == 0)
							{
								cout << "LOOK FOR G2 PROBLEM " + to_string(r) + ";;" + to_string(newx[i]) + ";" + to_string(newy[i]) << endl;
								//waitKey(0);
							}
						}
						else
						if (owner[r] == 1)
						{
							int g4 = 0;
							//identify which num bag is this tag r
							for (int h = 0; h < 100; h++)
							{
								if (whichNumOwner[h] == r)
								{
									//putText(img, "OWNER" + to_string(h), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA); g4 = 1;
									Rect ri = boundingRect(contours[i]);
									Mat roi = img1(ri);
									//imshow("owner", roi);
									imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(h) + "\\owner" + to_string(h) + ".jpg", roi);
									//waitKey(0);
									break;
								}
							}
							if (g4 == 0)
							{
								cout << "LOOK FOR G4 PROBLEM " << endl; //waitKey(0);
							}
						}
						else
						{
							drawContour = 0;
							/*flag = 1;
							oldx[r] = newx[i];
							oldy[r] = newy[i];
							frame[r] = 1;
							break;*/
						}
						//putText(img, "MAN" + to_string(id[r]), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(255, 255, 255), 1, CV_AA);

						//putText(img, "H" + to_string(contourArea(contours[i])), Point(newy[i] - 30, newx[i] - 30), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(255, 255, 255), 1, CV_AA);
						if (clickTag[r] == 1)
							drawContour = 0;
						if (drawContour == 0)
						{
							//only on img_all_contour
							//img.copyTo(img_all_contour);
							// + to_string(id[r])
							putText(img_all_contour, "MAN" + to_string(id[r]), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(255, 255, 255), 1, CV_AA);
							if (contourArea(contours[i])>800)
								drawContours(img_all_contour, contours, i, colors[r], 2, 8, hierarchy, 0, Point());
						}
						else
						{
							drawContours(img, contours, i, colors[r], 2, 8, hierarchy, 0, Point());
							drawContours(img_all_contour, contours, i, colors[r], 2, 8, hierarchy, 0, Point());
							putText(img_all_contour, to_string(id[r]), Point(newy[i], newx[i]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(255, 255, 255), 1, CV_AA);
						}
						flag = 1;
						oldx[r] = newx[i];
						oldy[r] = newy[i];
						area[r] = area_new[i];
						frame[r] = 1;
						break;
					}
				}
				//means new object
				if (flag == 1)
					continue;

				//new object put in empty space
				for (f = 0; f < 100; f++)
				{
					if (oldx[f] == 0 && oldy[f] == 0)
					{
						oldx[f] = newx[i];
						oldy[f] = newy[i];
						area[f] = area_new[i];
						id[f] = f;
						tolerance[f] = 0;
						bagToll[f] = 0;
						manToll[f] = 0;
						bag[f] = 0;
						owner[f] = 0;
						abandonedTag[f] = 0;
						clickTag[f] = 0;
						abandonedFrameNum[f] = 0;
						ownerOfBag[f] = -1;
						for (int e = 0; e < 100; e++)
						{
							if (whichNumBag[e] == f)
								whichNumBag[e] = -1;
							if (whichNumOwner[e] == f)
								whichNumOwner[e] = -1;
						}
						frame[f] = 1;
						//putText(img, "New Object" + to_string(id[f]), Point(oldy[f], oldx[f]), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(250, 0, 0), 1, CV_AA);
						birth[f] = frameNum;
						//drawContours(img, contours, i, colors[f], 2, 8, hierarchy, 0, Point());
						break;
					}
				}
			}
		}
		dumpContour();

		//frame initialised to all zeroes
		for (t = 0; t < 100; t++)
			frame[t] = 0;
		///////////////////////////////////////ALL BIG CONTOURS DRAWN--------------------------------
		putText(img, "FRAME-" + to_string(frameNum), Point(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA);
		//current time clock
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);
		char mbstr[100];
		strftime(mbstr, sizeof(mbstr), "%I %M %S", std::localtime(&t));
		//if (strftime(mbstr, sizeof(mbstr), "%I %M %S",localtime(&t))){
		//std::cout << mbstr << '\n';
		//String tim;
		//string str(mbstr);
		putText(img, mbstr, Point(30, 50), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 1, CV_AA);
		//}
		////////////////////////// Click Feature///////////////
		setMouseCallback("Final OUTPUT", CallBackFunc, NULL);
		///////////////////////////////////////////////////////
		imshow("Final OUTPUT", img);
		waitKey(30);

		////////////////////////////////////////////////////////// ALL Contours////////
		imshow("All Contours", img_all_contour);
		waitKey(30);
		///////////////////////////////////////////////////////////////////////////////
		if (photo == 1)
		{
			imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(numberOfBags) + "\\bag_AND_owner" + to_string(numberOfBags) + ".jpg", img);
			photo = 0;
		}
		if (newLinkageFlag == 1)
		{
			imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(numberOfBags) + "\\bag_AND_owner" + to_string(numberOfBags) + ".jpg", img);
			photo = 1;
		}
		cout << "" << endl;

		//cout << to_string(num) << endl;
		//cout << "else" << endl;
		//system("CLS");
		//if (frameNum >= 1250)
		//	waitKey(0);
		if (waitKey(1) == 80 || waitKey(1) == 112)
		{
			if (pauseKey == 1)
				pauseKey = 0;
			else
			if (pauseKey == 0)
				pauseKey = 1;
			cout << "pauseKey=" + to_string(pauseKey) << endl;
			waitKey(1000);
		}
		if (pauseKey == 1)
			waitKey(0);
	}
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == (EVENT_FLAG_RBUTTON))
	{
		// if x,y in bounding box make it noise
		clickx = x;
		clicky = y;
		cout << "hello" << endl;
		return;
	}
	else
	if (event == (EVENT_FLAG_LBUTTON))
	{
		// if x,y in bounding box make it noise
		dispOwnerx = x;
		dispOwnery = y;
		return;
	}
	return;
}

void checkAbandoned()
{
	timVar = 0;
	// rth bag in question
	if (ownerOfBag[r] == -2 || abandonedTag[r] == 1)
	{
		abandonedTag[r] = 1;
		if (abandonedFrameNum[r] == 0)
			abandonedFrameNum[r] = frameNum;
		for (int y = 1; y <= 100; y++)
		{
			if (whichNumBag[y] == r)
			if (AHour[y] == -1 || AMin[y] == -1 || ASec[y] == -1)
			{
				//assign cuurent time as abandoned time for this bag
				time_t currentTime;
				struct tm *localTime;

				time(&currentTime);                   // Get the current time
				localTime = localtime(&currentTime);  // Convert the current time to the local time

				int Hour = localTime->tm_hour;
				int Min = localTime->tm_min;
				int Sec = localTime->tm_sec;

				AHour[y] = Hour;
				AMin[y] = Min;
				ASec[y] = Sec;
			}
		}
		///////////////////////////////////////////////////////HTML generate///////////////////////////////
		ofstream myfile;
		myfile.open("C:/Users/Lenovo/Desktop/bag.html");
		myfile << "<!DOCTYPE html><html><head></head><body style=\"background-color:#0099ff\" >"; //starting html
		myfile << "<h1>Abandoned Bag Present at the Baggage Counter </h1>";
		myfile << "<p><span style='font-weight: bold'>" << "Bag No &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp || &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp " << "</span><span>" << "Image Of Bag &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp || &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp " << "</span>" << "<span>" << "Time Of Abandoned" << "</span></p>";

		//add some html content
		//as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
		for (int i = 1; i <= numberOfBags; i++)
		{
			//getting time//////////////////////////

			///////////////////////////////////////
			if (AHour[i] != -1)
			{
				timVar++;
				myfile << "<p><span style='font-weight: bold'>" << to_string(timVar) + ") &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp &nbsp || &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp " << "</span><span>" << "<img src = \"C:/Users/Lenovo/Desktop/open cv/image data/" + to_string(i) + "/bag" + to_string(i) + ".jpg\" >" << "</span>" << "<span>" << " &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp || &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp" << AHour[i] << ":" << AMin[i] << ":" << ASec[i] << "</span></p>";
			}
		}

		//ending html
		myfile << "</body></html>";
		myfile.close();
		///////////////////////////////////////////////////////////////////////////////////////////////////

		cout << "returning" << endl;
		return;
	}
	int bagTag = r;
	int manTag = ownerOfBag[bagTag];
	//cout << "bagTag" + to_string(r) << endl;
	cout << "manTag" + to_string(manTag) << endl;
	double dist1 = sqrt((oldx[manTag] - oldx[r])*(oldx[manTag] - oldx[r]) + (oldy[manTag] - oldy[r])*(oldy[manTag] - oldy[r]));
	cout << "dist" + to_string(dist1) << endl;
	if (dist1 > 300)
	{
		abandonedTag[r] = 1;
		if (abandonedFrameNum[r] == 0)
			abandonedFrameNum[r] = frameNum;
		line(img, Point(oldy[manTag], oldx[manTag]), Point(oldy[r], oldx[r]), colors[r], 3, 8);
		//cout << "abandoned with dist=" + to_string(dist1) + ";" << endl;
	}
	else
	{
		abandonedTag[r] = 0;
		abandonedFrameNum[r] = 0;
		line(img, Point(oldy[manTag], oldx[manTag]), Point(oldy[r], oldx[r]), colors[r], 3, 8);
	}
}

void linkage()
{
	bag[r] = 1;
	//linkageTag[r] = 1;	//means this id is bag
	//find id of owner
	//closest and same birth

	p = abs(birth[r] - frameNum);
	if (p <= 6)
	{
		//if bag is new contour

		//search for closest contour
		int closest = 6000;	//frame=0
		int abc = -1;
		for (int x = 0; x < 100; x++)
		{
			if (x == r)
				continue;
			dist = sqrt((oldx[x] - oldx[r])*(oldx[x] - oldx[r]) + (oldy[x] - oldy[r])*(oldy[x] - oldy[r]));
			if (dist<200 && dist<closest)
			{
				closest = dist;
				abc = x;
			}
		}
		cout << "final abc-" + to_string(abc) << endl;
		if (abc == -1)
		{
			//wrong detection of bag //detect it as man
			bag[r] = 0;
			as++;
		}
		else
		{
			if (owner[abc] == 1)
				return;
			owner[abc] = 1;
			ownerOfBag[r] = abc;
			numberOfBags++;
			whichNumBag[numberOfBags] = r;
			whichNumOwner[numberOfBags] = abc;
			imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(numberOfBags) + "\\SCENE" + to_string(numberOfBags) + ".jpg", img1);
			newLinkageFlag = 1;
			cout << "numberOfBags=" + to_string(numberOfBags) << endl;
			cout << "-------------------------------" << endl;
		}
	}
	else
	{
		//search for newest contour and should be close
		int newest = frameNum;	//frame=0
		int abc = -1;
		for (int x = 0; x < 100; x++)
		{
			if (x == r)
				continue;
			dist = sqrt((oldx[x] - oldx[r])*(oldx[x] - oldx[r]) + (oldy[x] - oldy[r])*(oldy[x] - oldy[r]));
			if (abs(birth[x] - birth[r]) < newest && dist<200)
			{
				newest = abs(birth[x] - birth[r]);
				abc = x;
			}
		}
		cout << "final abc-" + to_string(abc) << endl;
		if (abc == -1)
		{
			//wrong detection of bag //detect it as man
			bag[r] = 0;
			as++;
		}
		else
		{
			//if (owner[abc] == 1)
			//	return;
			owner[abc] = 1;
			ownerOfBag[r] = abc;
			numberOfBags++;
			whichNumBag[numberOfBags] = r;
			whichNumOwner[numberOfBags] = abc;
			imwrite("C:\\Users\\Lenovo\\Desktop\\open cv\\image data\\" + to_string(numberOfBags) + "\\SCENE" + to_string(numberOfBags) + ".jpg", img1);
			newLinkageFlag = 1;
			cout << "numberOfBags=" + to_string(numberOfBags) << endl;
			cout << "-------------------------------" << endl;
		}
		//linkageTag[owner[numberOfBags]] = 2;	//2 means this id is owner
	}
}

void dumpContour()
{
	//cout << "at dump point" << endl;
	for (w = 0; w < 100; w++)
	if (oldx[w] != 0)
	{
		//cout << "tolerance of " + to_string(w) + ";tag increases" << endl;
		tolerance[w]++;
	}
	for (w = 0; w < 100; w++)
	if (frame[w] == 1)
		tolerance[w] = 0;

	for (e = 0; e < 100; e++)
	{
		int tolLimit;
		if (abandonedTag[e] == 1)
			tolLimit = 30;
		else
		if (permaBagTag[e] == 1)
			tolLimit = 15;
		else
		if (owner[e] == 1)
			tolLimit = 15;
		else
			tolLimit = 4;

		if (tolerance[e] >= tolLimit && oldx[e] != 0)
		{
			//if this cm was of owner then put ownerOfBag tag = -2 and corr whichNumOwner = -1
			for (int a = 0; a < 100; a++)
			if (ownerOfBag[a] == e)
			{
				cout << "deleting owner" << endl;
				abandonedTag[a] = 1;
				if (abandonedFrameNum[a] == 0)
					abandonedFrameNum[a] = frameNum;
				for (int s = 0; s < 100; s++)
				if (whichNumBag[s] == a)
					whichNumOwner[s] = -1;
				ownerOfBag[a] = -2; owner[e] = 0; break;
			}

			//if this cm was of bag
			if (permaBagTag[e] == 1 || tempBagTag[e] == 1)
			{
				cout << "deleting bag of id;" + to_string(e) << endl;
				bag[e] = 0;
				//delete owner and whichNumBag linkage
				owner[ownerOfBag[e]] = -1;
				for (int s = 0; s < 100; s++)
				if (whichNumBag[s] == e)
				{
					AHour[s] = -1;
					AMin[s] = -1;
					ASec[s] = -1;
					whichNumOwner[s] = -1; whichNumBag[s] = -1; cout << "bag was number" + to_string(s) << endl;
				}

				///////////////////////////////////////////////////////HTML generate///////////////////////////////
				ofstream myfile;
				myfile.open("C:/Users/Lenovo/Desktop/bag.html");
				myfile << "<!DOCTYPE html><html><head></head><body style=\"background-color:#0099ff\" >"; //starting html
				myfile << "<h1>Abandoned Bag Present at the Baggage Counter </h1>";
				myfile << "<p><span style='font-weight: bold'>" << "Bag No &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp||&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp  &nbsp" << "</span><span>" << "Image Of Bag &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp||&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp " << "</span>" << "<span>" << "Time Of Abandoned" << "</span></p>";

				//add some html content
				//as an example: if you have array of objects featuring the properties name & value, you can print out a new line for each property pairs like this:
				for (int i = 1; i <= numberOfBags; i++)
				{
					//getting time//////////////////////////

					///////////////////////////////////////
					if (AHour[i] != -1)
					{
						timVar++;
						myfile << "<p><span style='font-weight: bold'>" << to_string(timVar) + ") &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp|| &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp" << "</span><span>" << "<img src = \"C:/Users/Lenovo/Desktop/open cv/image data/" + to_string(i) + "/bag" + to_string(i) + ".jpg\" >" << "</span>" << "<span>" << " &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp  ||  &nbsp &nbsp&nbsp &nbsp&nbsp &nbsp&nbsp &nbsp  " << AHour[i] << ":" << AMin[i] << ":" << ASec[i] << "</span></p>";
					}
				}

				//ending html
				myfile << "</body></html>";
				myfile.close();
				///////////////////////////////////////////////////////////////////////////////////////////////////


				ownerOfBag[e] = -1;
				abandonedTag[e] = 0;

				clickTag[e] = 0;
				abandonedFrameNum[e] = 0;
			}

			oldx[e] = 0;
			oldy[e] = 0;
			area[e] = 0;
			roi_hs[e] = 0;
			id[e] = 0;
			tolerance[e] = 0;
			//tempBagTag[e] = 0;
			permaBagTag[e] = 0;
			abandonedTag[e] = 0;
			clickTag[e] = 0;
			abandonedFrameNum[e] = 0;
			ownerOfBag[e] = -1;
			bagToll[e] = 0;
			manToll[e] = 0;
			birth[e] = 0;
		}
	}
}