#ifndef queue_h
#define queue_h


#include "bintree.h"

#define MAX_QUEUE 1000

void qpush(Node*);
Node * qpop (void);
void queueinit (void);
int queueempty (void);

#endif /* queue_h */
