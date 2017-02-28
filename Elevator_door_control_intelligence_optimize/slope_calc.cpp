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

int main()
{
    int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
    float a,b;
//    printf("\n   C program for Linear Curve Fitting \n ");
    n=20;
        
    int y[20]={10920, 10695 ,10920 ,10920 ,10695 ,10920, 10695, 11060 ,11060 ,11060, 11060 ,11431 ,10990 ,11060 ,11060 ,11060 ,11431 ,11972 ,11972 ,11431};
    int x[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    
    for(i=0;i<=n-1;i++)
    {
        sumx=sumx +x[i];
        sumx2=sumx2 +x[i]*x[i];
        sumy=sumy +y[i];
        sumxy=sumxy +x[i]*y[i];
 
    }
    a=((sumx2*sumy -sumx*sumxy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    b=((n*sumxy-sumx*sumy)*1.0/(n*sumx2-sumx*sumx)*1.0);
    printf("\n\nThe line is Y=%3.3f +%3.3f X",a,b);
    //cout<<"slope"<<b<<endl;
    return(b);
    
}