//
//  queue.c
//  ABE
//
//  Created by Tran Vinh Duc on 11/25/17.
//  Copyright © 2017 Tran Vinh Duc. All rights reserved.
//

#include "queue.h"

static Node * queue[MAX_QUEUE +1];
static int head, tail;

void qpush(Node *v)
{
    queue[tail++] = v;
    if (tail  > MAX_QUEUE) tail = 0;
}

Node * qpop ()
{
    Node *t = queue[head++];
    
    if (head > MAX_QUEUE) head = 0;
    
    return t;
}

void queueinit ()
{
    head = 0;
    tail = 0;
}

int queueempty ()
{
    return head==tail;
}

