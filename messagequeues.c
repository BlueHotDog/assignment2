#include "messagequeues.h"

mqd_t QUEUE_Create(string name) {
    printf("Creating message queue %s \n", name);



    struct mq_attr wanted_attrs;


    /* filling the attribute structure */
    wanted_attrs.mq_flags = 0; /* no exceptional behavior (just O_NONBLOCK currently available)  */
    wanted_attrs.mq_maxmsg = 1; /* room for at most 100 messages in the queue */
    wanted_attrs.mq_msgsize = 2; /* maximum size of a message */
    wanted_attrs.mq_curmsgs = 0; /* this (current number of messages) will be ignored */
    
    mqd_t ret = mq_open("/test", O_RDWR | O_CREAT,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

    if (ret == -1) {
        if (errno == 17) {
            if (mq_unlink(name) == -1) {
                perror("Error while creating message queue\n");
                printf("Error was:%s\n", strerror(errno));
            } else {
                mqd_t ret = mq_open("/test", (O_RDWR | O_CREAT), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &wanted_attrs);
            }

        }
        perror("Error while creating message queue\n");
        printf("Error was:%s\n", strerror(errno));
        return -1;
    }
    return ret;

}

mqd_t QUEUE_CreateForProcess(PID processID) {
    string name = QUEUE_GetProcessQueueName(processID);
    mqd_t toReturn = QUEUE_Create(name);
    free(name);
    return toReturn;
}

mqd_t QUEUE_Open(string name) {
    printf("Opening message queue %s \n", name);


    struct mq_attr attr;
    memset(&attr, 0, sizeof (struct mq_attr));
    attr.mq_maxmsg = MAX_MESSAGE_SIZE + 1;
    attr.mq_msgsize = MAX_MESSAGE_SIZE + 1;
    attr.mq_flags = 0;


    mqd_t ret = mq_open(name, O_RDWR, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), &attr);
    if (ret == -1) {
        perror("Error while opening message queue\n");
        printf("Error was:%s\n", strerror(errno));
        return -1;
    }
    return ret;
}

mqd_t QUEUE_OpenForProcess(PID processID) {

    string name = QUEUE_GetProcessQueueName(processID);
    mqd_t toReturn = QUEUE_Open(name);
    free(name);
    return toReturn;
}

int QUEUE_Send(mqd_t queue, string text) {
    struct mq_attr attr;
    int t = mq_getattr(queue, &attr);

    printf("t=%d maxmsg=%ld\n", t, attr.mq_maxmsg);

    ASSERT(strlen(text) <= MAX_MESSAGE_SIZE);
    int ret = mq_send(queue, text, strlen(text), 0);
    if (ret == -1) {
        perror("Error while sending message queue\n");
        printf("Error was:%s\n", strerror(errno));
        return 0;
    }
    return ret;
}

string QUEUE_Read(mqd_t queue) {
    char msg_buffer[9]; /* For printing a null terminated string for testing */
    string toReturn = "abc";

    struct mq_attr attr;
    int t = mq_getattr(queue, &attr);

    printf("t=%d maxmsg=%ld\n", t, attr.mq_msgsize);

    int ret = mq_receive(queue, toReturn, strlen (toReturn), 0);
    if (ret == -1) {
        perror("Error while reading message queue\n");
        printf("Error was:%s\n", strerror(errno));
        return 0;
    }
    return toReturn;
}

string QUEUE_GetProcessQueueName(PID processID) {
    string name = calloc(MAX_MESSAGE_SIZE, sizeof (char));
    sprintf(name, "/%d", processID);
    return name;
}

int QUEUE_Close(mqd_t queue) {
    int ret = mq_close(queue);
    if (ret == -1) {
        perror("Error while closing message queue\n");
        printf("Error was:%s\n", strerror(errno));
    }
    return ret;
}