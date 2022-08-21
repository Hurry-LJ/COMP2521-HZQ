// Queue ADT header file ... COMP2521 

typedef struct QueueRep *queue;

queue newQueue();               // set up empty queue
void  dropQueue(queue);         // remove unwanted queue
int   QueueIsEmpty(queue);      // check whether queue is empty
void  QueueEnqueue(queue, int); // insert an int at end of queue
void FOFOQueueEnqueue(queue,int);
int   QueueDequeue(queue);      // remove int from front of queue
int   peekQueue(queue);         // displays the data at the top of the queue