#include<stdio.h>
//#include<conio.h>
#include<math.h>

float find_slope(int * y,int * x,int size);

int main()
{
    int y[21]={5130, 11060, 5335, 11060, 5376, 10268, 10640, 5130, 5335, 9916, 5335, 9702, 5130, 9916, 9438, 5130, 9504, 5335, 9438, 5335, 5130   };
    int x[21]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int size=21;
    printf("slope %3.3f", find_slope(x,y,size));
    
}

/*
int main()
{
 
    int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
    float a,b;
    printf("\n   C program for Linear Curve Fitting \n ");
    n=6;
        
    int y[6]={2,-50,10,17,28,39};
    int x[6]={1,2,3,4,5,6};
    
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
    printf("\n\nSlope %3.3f X",b);
    return(0);
}*/


float find_slope(int * y,int * x,int size)
{
    int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
    float a,b;
    printf("\n   C program for Linear Curve Fitting \n ");
    n=size;
        
    //int y[6]={2,-50,10,17,28,39};
    //int x[6]={1,2,3,4,5,6};
    
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
    printf("\n\nSlope %5.5f X\n",b);
    return(b);
    
}

