/* 
 * File:   msq.h
 * Author: arju
 *
 * Created on 6 December, 2016, 10:48 PM
 */

#ifndef MSQ_H
#define	MSQ_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SIZE 1
#define MSG_KEY 8

    struct data_packet
    {
            int type;
            int data[SIZE];
    };

    struct msq_st
    {
            key_t msg_key;
            int msg_id;
            int size_read;
            struct data_packet packet_in;
    };
    
//Function prototypes of this module

int init_queue(struct msq_st * msq);
int read_queue(struct msq_st * msq);
int write_queue(struct msq_st * msq,struct data_packet *packet);
int print_packet(struct msq_st * msq);


#ifdef	__cplusplus
}
#endif

#endif	/* MSQ_H */

