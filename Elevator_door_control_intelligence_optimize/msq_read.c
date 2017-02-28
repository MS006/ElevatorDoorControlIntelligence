#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "msq.h"

/*Functionality:
 *      Create MSG id*/
int init_queue(struct msq_st * msq)
{
    
    //strcpy(msq->packet_in.data,"def data");
    //msq->packet_in.type=0;
    msq->msg_id=msgget(msq->msg_key,IPC_CREAT|0666);
    perror("error during queue create:");
    return 0;
}

/*Funcitonality
        Read one packet size into "Packet in"*/
int read_queue(struct msq_st * msq)
{
    //send data  to queue
    //printf("size to read %ld \n",(sizeof(msq->packet_in)));
    msq->size_read=msgrcv(msq->msg_id,&(msq->packet_in),(sizeof(msq->packet_in)),0,0);
    //perror("error at rcv:");
    //printf("size_read:%d \n",(int)msq->size_read);
    //printf("data read :%d\n",msq->packet_in.data);
    
    return 0;
}


/*Functionality
        Write the content of  packet passed to the queue*/
int write_queue(struct msq_st * msq,struct data_packet *packet)
{
    //send data  to queue
    //printf("size of data send %ld\n",sizeof(*packet));
    msgsnd(msq->msg_id,packet,sizeof(*packet),IPC_NOWAIT);
    //perror("error at send:");
       
    return 0;
}

/*Functionality
        Prints out the data in packet_in*/
int print_packet(struct msq_st * msq)
{
	int i=0;
                  //printf("data: %s", *data);
	for(i=0;i<SIZE;i++)
	{
		printf("%x - ",msq->packet_in.data[i]);
                                   
	}
	printf("\n");
	return 0;
}


 /* 
// * Module usage example
 * 
 */
 
/* 
int main()
{
        //create a queue structure
        struct msq_st msq;

        //assign queue key
        msq.msg_key=MSG_KEY;
        int i=0;
        
        //create packet to be send and update value
        int j=0x0C;
        struct data_packet x;
        x.type=0;
        for(i-0;i<SIZE;i++)
        {
                x.data[i]=j;
                j++;
        }
        
         
        init_queue(&msq);
        //write_queue(&msq,&x);
        read_queue(&msq);
        
        print_packet(&msq);
        return 0;
}*/

