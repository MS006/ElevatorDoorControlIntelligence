#include <iostream>
#include<fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "msq.h"
 
using namespace std;
using namespace cv;

#define DURATION_B 10
#define THRESHOLD_A 500
#define AZONE 10000
#define DIP_2_IGNORE 3000
#define ENABLE_BENCHMARKING 1


void print_array(int *data, int size);
float find_slope(int * y,int size);
int find_minimum(int a[], int n);
int find_maximum(int a[], int n);


int point_a=0;
  int counter_a=0;
  int area=0;
  int area_1=0;
  int area_2=0;
  float slope=0;
  int dhs=0;
  int counter_dhs=0;
  int b_zone_buffer[DURATION_B];
  int s=0;
  clock_t start, end;
  double cpu_time_used;
  
 void analyze();
 
 
int main (int argc, const char * argv[])
{
    VideoCapture cap(CV_CAP_ANY);
     s = open ("/dev/console", O_WRONLY);
         write (s, "\a", 1);
         sleep(1);
         write (s, "\a", 1);
         write (s, "\a", 1);
         sleep(1);
    
        //create a queue structure
        struct msq_st msq;

        //assign queue key
        msq.msg_key=MSG_KEY;
        
        struct data_packet x;
        
        //initialize queue
        init_queue(&msq);
        
        
     //VideoCapture cap(0);
    //VideoCapture cap("/home/arju/NetBeansProjects/CppApplication_1/MyVideo2.avi");
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
        if(ENABLE_BENCHMARKING)
        {
            start=clock();
        }
        
        cap >> img;
        if (!img.data)
            continue;
        //GaussianBlur( img, img, Size( 25, 25 ), 0, 0 );
 
        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
        if(found.size()==0)
        {
             x.data[0]=0;
            write_queue(&msq,&x);
             myfile << 0<<endl;
            
        }
        //cout<<"found size"<<found.size()<<endl;
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                {
                    break;
                    cout<<"no obj check"<<endl;
                }
                    
            if (j==found.size())
            {
                found_filtered.push_back(r);
            }
                
        }
        for (i=0; i<found_filtered.size(); i++)
        {
	    Rect r = found_filtered[i];
                      r.x += cvRound(r.width*0.1);
	    r.width = cvRound(r.width*0.8);
	    r.y += cvRound(r.height*0.06);
	    r.height = cvRound(r.height*0.9);
	    rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
                      
                      myfile << r.area()<<endl;
                               
                      area=r.area();
                      x.data[0]=area;
                      write_queue(&msq,&x);
                      
                      //analyze();
        }
        imshow("video capture", img);
        if(ENABLE_BENCHMARKING)
        {
            end=clock();
        }
        //sleep(1);
        
        if (waitKey(20) >= 0)
           break;
        if(ENABLE_BENCHMARKING)
        {
            cpu_time_used = ((double) (end - start))/CLOCKS_PER_SEC;
            cout<<"loop rate :"<<cpu_time_used<<endl;
        }
        
    }
    myfile.close();
    return 0;
}


// find slope 
float find_slope(int * y,int size)
{
    int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
    float a,b;
    
    int max=0;
    int min=0;
    int diff_array[DURATION_B];
    int p2p=0;
    
    i=0;
    n=size;
    
    // find the difference array
    for(i=0;i<(size-1);i++)
    {
        diff_array[i]=y[i+1]-y[i];
    }
    
    max=diff_array[find_maximum(diff_array,size-1)];
    min=diff_array[find_minimum(diff_array,size-1)];
    p2p=abs(max-min);
    
    if(p2p>DIP_2_IGNORE)
    {
        return(-1);
    }
    
      
    //printf("\n   C program for Linear Curve Fitting \n ");
    
     
    //int y[6]={2,-50,10,17,28,39};
    int x[DURATION_B];
    for(i=0;i<size;i++)
    {
        x[i]=i+1;
    }
    
    for(i=0;i<=n-1;i++)
    {
        sumx=sumx +x[i];
        sumx2=sumx2 +x[i]*x[i];
        sumy=sumy +y[i];
        sumxy=sumxy +x[i]*y[i];
 
    }
    a=((sumx2*sumy -sumx*sumxy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    b=((n*sumxy-sumx*sumy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    //printf("\n\nThe line is Y=%3.3f +%3.3f X",a,b);
    //printf("\n\nSlope %3.3f X\n",b);
    return(b);
    
}


// print array
void print_array(int *data, int size)
{
    int i=0;
    for(i=0;i<size;i++)
    {
        cout<<data[i]<<endl;
        
    }
    printf("\n");
}



//Analytics function

void analyze()
{
    int temp=area;
    //area=atoi(line.c_str());
    area=(area+area_1+area_2)/3;
    //if(area-area_1>DIP_2_IGNORE)
    //{
        //area=area_1;
    //}
    area_2=area_1;
    area_1=temp;
    
          if(dhs==0)
          {
              // Usual A detect logic
                        if(counter_a==0)
                          {
                              //looking for POINT A
                              //if ((AZONE-THRESHOLD_A)<area<(AZONE+THRESHOLD_A))
                              if ((9500<area)&(area<10500))
                              {
                                  //point A detected
                                  point_a=1;
                                  b_zone_buffer[counter_a]=area;
                                  counter_a++;

                              }
                          }

                          else
                          {
                              //Look out for point B
                              b_zone_buffer[counter_a]=area;
                               counter_a++;

                              if (counter_a>DURATION_B)
                              {
                                  //end of A ZONE, start of B ZONE
                                  point_a=0;
                                  counter_a=0;
                                  //print_array(b_zone_buffer,(int)DURATION_B);
                                  slope=find_slope(b_zone_buffer,(int)DURATION_B);
                                  //cout<<slope<<endl;
                                  if (slope>0.003)
                                  {
                                      //Turn the Door hold signal ON
                                      dhs=1;
                                      //write (s, "\a", 1);
                                      counter_dhs++;
                                  }
                                  else
                                  {
                                      //Turn the Door hold signal OFF
                                      dhs=0;
                                      //write (s, "\a", 1);
                                  }
                              }

                          }
          }
          else
          {
              //dhs ON- Door is kept on for fixed amount of time
              if(counter_dhs>DURATION_B+5)
              {
                  dhs=0;
                  counter_dhs=0;
              }
              else
              {
                 counter_dhs++;
              }
              
          }
         cout << area <<","<<point_a<<","<<counter_a<<","<<dhs<<endl;  
        // cout <<dhs<<endl;  
}


int find_minimum(int a[], int n) {
  int c, min, index;
 
  min = a[0];
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (a[c] < min) {
       index = c;
       min = a[c];
    }
  }
 
  return index;
}

int find_maximum(int a[], int n) {
  int c, max, index;
 
  max = a[0];
  index = 0;
 
  for (c = 1; c < n; c++) {
    if (a[c] > max) {
       index = c;
       max = a[c];
    }
  }
 
  return index;
}

