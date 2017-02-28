// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 

#define DURATION_B 10
#define THRESHOLD_A 500
#define AZONE 10000
using namespace std;

void print_array(int *data, int size);
float find_slope(int * y,int size);

int main () {
  string line;
  ifstream myfile ("good_data.txt");
  ofstream analysis_log;
  int point_a=0;
  int counter_a=0;
  int area=0;
  float slope=0;
  int dhs=0;
  int counter_dhs=0;
  int b_zone_buffer[DURATION_B];
  analysis_log.open ("analysis_log.csv");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
          //cout << line << '\n';
          area=atoi(line.c_str());
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
                                  if (slope>0)
                                  {
                                      //Turn the Door hold signal ON
                                      dhs=1;
                                      counter_dhs++;
                                  }
                                  else
                                  {
                                      //Turn the Door hold signal OFF
                                      dhs=0;
                                  }
                              }

                          }
          }
          else
          {
              //dhs ON- Door is kept on for fixed amount of time
              if(counter_dhs>DURATION_B)
              {
                  dhs=0;
                  counter_dhs=0;
              }
              else
              {
                 counter_dhs++;
              }
              
          }

          //analysis_log << line <<","<<point_a <<","<<counter_a<<endl;
          //analysis_log << area <<","<<point_a<<","<<counter_a<<endl;
          cout << area <<","<<point_a<<","<<counter_a<<","<<dhs<<endl;       
    }
    myfile.close();
    analysis_log.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}


// find slope 
float find_slope(int * y,int size)
{
    int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
    float a,b;
    //printf("\n   C program for Linear Curve Fitting \n ");
    n=size;
        
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

