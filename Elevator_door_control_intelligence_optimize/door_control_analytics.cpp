

    /*
     * To change this license header, choose License Headers in Project Properties.
     * To change this template file, choose Tools | Templates
     * and open the template in the editor.
     */

    /*
     * File:   main.cpp
     * Author: Shady
     *
     * Created on 25 February, 2017, 11:13 PM
     */

    #include <cstdlib>
    #include <stdio.h>
    #include <stdlib.h>
    #include<fstream>
    #include <iostream>
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <time.h>

#include "msq.h"

#define BUFF_SIZE 20
#define DHS_WAIT 10
#define SIMULATION 1

void print_array(int *data, int size);

    using namespace std;

    /*
     *
     */

    void init();
    void azone();
    void instslp();
    void chkbuf();
    void clearbuf();
    void calcslp();
    void DHS();
    void slpchk();
    float find_slope(int * y,int * x,int size);

    int i = 1;
    int DataIn[] = {0,0,0};
    int Buf[BUFF_SIZE];
    int buf_cnt=0;
    int DHScount;
    int DataIn_cnt;
    int i_slp;
    int DHS_flag=0;
    int x[BUFF_SIZE];
    int s=0;
    int slope;


    enum states1
    {
        s_init,
        s_azone,
        s_instslp,
        s_chkbuf,
        s_clearbuf,
        s_calcslp,
        s_DHS,
        s_slpchk,
        
    } states;
    
    //create a queue structure
        struct msq_st msq;
        
        ofstream myfile;
        ifstream simulation_data;
        

        
    int main(int argc, char** argv) 
    {
        DataIn_cnt =0;
        DHS_flag=0;
        
        //assign queue key
        msq.msg_key=MSG_KEY;
        
        //initialize queue
        init_queue(&msq);
        
        cout<<"program started\n"<<endl;
        
        //Analytics log file
        myfile.open ("debug_log.txt");
        
        //simulation will read log file for input
        if(SIMULATION)
        {
            simulation_data.open ("Human_detection_log.txt");
         
        }
        
        //buzzer to idicate the begining
        s = open ("/dev/console", O_WRONLY);
         write (s, "\a", 1);
         sleep(1);
         write (s, "\a", 1);
         write (s, "\a", 1);
         sleep(1);
         
         //initializa the X array for slope calculation
         for(i=0;i<BUFF_SIZE;i++)
         {
             x[i]=i+1;
             
         }
        
         //State machine loop
       while(1)
       {
             //fflush(stdout);
            switch(states)
            {                     
        
                case s_init :
                        init();
                        break;
                case s_azone :
                        azone();
                        break;
                case s_instslp :
                        instslp();
                        break;
                case s_chkbuf :
                        chkbuf();
                        break;
                case s_clearbuf :
                        clearbuf();
                        break;
                case s_calcslp :
                        calcslp();
                        break;
                case s_DHS :
                        DHS();
                        break;
                 case s_slpchk :
                        slpchk();
                        break;
            }
         
       }
       
       return (EXIT_SUCCESS);
    }
    // /*****************End of main****************************************/
    
//Funcitonalities    
// *Collec the data from message queue and update the main buffer 
//   *Clear buffer if there in case of zero
    
void init()
{
     
    cout<<"DHS value"<<DHS_flag<<endl;
    
    //if simulation, get data from log file
    if(SIMULATION)
    {
        DataIn[1]=DataIn[0];                                          //Old data stored for averaging
        simulation_data>>DataIn[0];                            // New data to DataIn[0]
        //cout<<"simulated data :"<<DataIn[0]<<endl;
    }
    //If no simulation, read the value from message queue
    else
    {
        read_queue(&msq);
        print_packet(&msq);
        DataIn[1]=DataIn[0];                                            //Old data stored for averaging
        DataIn[0]=msq.packet_in.data[0];                        // New data to DataIn[0]

    }
    
    //zero data implies no human detected, the buffer is cleared here
    if(DataIn[0]==0)
    {
        states = s_clearbuf;
    }
    
    //Moving avearge of 3 elements
    //DataIn[0]=(DataIn[0]+DataIn[1]+DataIn[2])/3;
    //DataIn[1]=DataIn[0];
    //DataIn[2]=DataIn[1];
    
    //Log data
    myfile<<DataIn[0]<<","<<DHS_flag<<","<<slope<<endl;
    /*
     if(DHS_flag==0)
       {
            printf("Init\n");
           DHScount =0;
           DataIn_cnt++;
           states = s_azone;

       }
       else
       {
           states = s_DHS;
       }
     */

       DHScount =0;
       DataIn_cnt++;
       states = s_azone;
}





//* Check if data falls in azone
void azone()
{
    int Azone[] = {10000, 16000};
    
   //Check for Azone limit
    if(Azone[0]<DataIn[0] &  Azone[1]>DataIn[0])
    {
        // printf("Azone OK\n");
         states = s_instslp;                    //got to instantaneous sllope calculation
    }
       else
    {
            DHS_flag=0;
            states = s_clearbuf;                //clear buffer 
    }

}

     
    void instslp()
    {
       /*
        
        //check for 2 data points
  
        if (DataIn_cnt > 1)
        {
             // Calculate instantaneous slope
            i_slp = DataIn[0] - DataIn[1];
            states = s_slpchk;
        }
        else
        {
            states = s_init;
            
        }
        */
        states = s_slpchk;
    }

    void slpchk()
    {
       /*
        
        // Check for Positive instantaneous slope
        if(i_slp > 0)
        {
          // Append Data to Buffer  
           // printf("Append Data\n");
            
            Buf[buf_cnt]=DataIn[0];
            buf_cnt++;
            states = s_chkbuf;
        }
        else
        {
            states = s_clearbuf;
            
        }
        */
           Buf[buf_cnt]=DataIn[0];
           buf_cnt++;
           states = s_chkbuf;
    }

    void chkbuf()
    {
        
        // Check buffer size
        //printf("Buffer size\n");
        
        int BUFSIZE = BUFF_SIZE;
        
        if( buf_cnt >= BUFSIZE)
        {
          // go to slope calc
            states = s_calcslp;               //When buffer is big enough, proceed for slope calculation
        }
        else
        {
            states = s_init;                    // the logic waits for the buffer to build
            
        }
        
    }
     


void clearbuf()
{
    int i=0;
    
    //reset the buffer
    for(i=0;i<BUFF_SIZE;i++)
    {
        Buf[i]=0;
    }

    // Clear data buffer
    buf_cnt =0;
    DataIn_cnt =0;
    //DHS_flag=0;
    //printf("Clear Buffer\n");
    states = s_init;
}
     

//Check the slope of the buffer
// Control the DHS according to slope    
    
void calcslp()
{
        
    slope=find_slope(Buf,x,BUFF_SIZE);                  //slope calculated 

    //print_array(Buf,BUFF_SIZE);
   // print_array(x,BUFF_SIZE);

    //Control DHS according to slope
    int slope_thr = 1;
    if( slope >= slope_thr)
    {
        states = s_DHS;
    }
    else
    {
        /*
        //Decreament Buf point
        for(i=0;i<9;i++)
        {
            Buf[i]=Buf[i+1];
        }
        buf_cnt--;
        states = s_init;
        */
        DHS_flag=0;
        states = s_clearbuf;
    }
}

void DHS()
{
/*
// Wait DHS time out


if( DHScount >= DHS_WAIT)
{
    DHScount =0 ;
    //DHS_flag=0;
    states = s_clearbuf;
}
else
{
  // DHS
     DHS_flag=1;
    states = s_init;
    DHScount++;
    s = open ("/dev/console", O_WRONLY);
    write (s, "\a", 1);
    close(s);


  }

 */

 DHScount =0 ;
 DHS_flag=1;
 states = s_clearbuf;


}


float find_slope(int * y,int * x,int size)
{
int n,i,sumx=0,sumy=0,sumxy=0,sumx2=0;
float a,b;
//printf("\n   C program for Linear Curve Fitting \n ");
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
//printf("\n\nSlope %5.5f X\n",b);
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


