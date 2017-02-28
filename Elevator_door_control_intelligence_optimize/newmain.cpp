#include <iostream>
#include<fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
 
using namespace std;
using namespace cv;
 
int main (int argc, const char * argv[])
{
    //VideoCapture cap(CV_CAP_ANY);
    //VideoCapture cap(1);
    VideoCapture cap("/home/arju/NetBeansProjects/CppApplication_1/MyVideo.avi");
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);   
    int area_old=0;
    int area_new=0;
    int approach_flag=0;
    ofstream myfile;
    myfile.open ("Human_detection_log.txt");
    if (!cap.isOpened())
        return -1;
 
    Mat img;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 
    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
    while (true)
    {
        cap >> img;
        if (!img.data)
            continue;
 
        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
 
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
                found_filtered.push_back(r);
        }
        for (i=0; i<found_filtered.size(); i++)
        {
	    Rect r = found_filtered[i];
                      r.x += cvRound(r.width*0.1);
	    r.width = cvRound(r.width*0.8);
	    r.y += cvRound(r.height*0.06);
	    r.height = cvRound(r.height*0.9);
	    rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
                      area_new=r.area();
                      if (area_new-area_old>500)
                      {
                          cout<<"approaching..\n"<<endl;
                          area_old=area_new;
                      }
                      cout<<"area :"<<r.area()<<endl;
                      myfile << r.area()<<endl;
        }
        imshow("video capture", img);
        if (waitKey(20) >= 0)
            break;
    }
    myfile.close();
    return 0;
}