#include "queue.h"
#include "bintree.h"

Node *createNode(int type, int label, int index)  // type = 0: leaf, 1: And, 2: Or node
{
    Node *x = (Node *) malloc (sizeof(Node));
    x->d.type = type;
    x->d.label = label;
    x->d.index = index;
    x->l = 0;
    x->r = 0;
    /* for (int i = 0; i < LSSS_MAX_COL; i++) { */
    /*     x->d.vector[i] = 0; */
    /* } */
    return x;
}

Node * ands2tree (Set b, int index) //set index
{
    Node *root = 0;

    if (b.size <= 0) return root;
    if (b.size == 1)
    {
        root = createNode(0, b.elements[0], index);
        return root;
    }

    root = createNode(1, 0, 0);
    Node *tmp = root;
    int i = 0;
    for (; i < b.size -2; i++) {
 //      printf("%d AND\n", b.S[i]);
        tmp->l = createNode(0, b.elements[i], index);
        tmp->r = createNode(1, 0, 0);
        tmp = tmp->r;
    }
 //   printf("%d %d\n", b.S[i], b.S[i+1]);
    tmp->l = createNode(0, b.elements[i], index);
    tmp->r = createNode(0, b.elements[i+1], index);
    return root;
}

Node * dnf2tree (DNF dnf)
{
    Node *root = 0;

    if (dnf.m <= 0) return root;
    if (dnf.m == 1)
    {
        root = ands2tree(dnf.clauses[0], 0);
        return root;
    }
    root = createNode(2, 0, 0);
    Node *tmp = root;
    int i = 0;
    for (; i < dnf.m -2; i++) {
       // printf ("dnf.m = %d", dnf.m);
        tmp->l = ands2tree(dnf.clauses[i], i);
        tmp->r = createNode(2, 0, 0);
        tmp = tmp->r;
    }
    tmp->l = ands2tree(dnf.clauses[i], i);
    tmp->r = ands2tree(dnf.clauses[i+1], i+1);
    return root;
}


// For Test only
void visit (Node *t)
{
    switch (t->d.type)
    {
        case 0: printf("%d ", t->d.label);
            break;
        case 1: printf ("AND ");
            break;
        case 2: printf ("OR ");
            break;
    }
}
void freeBinTree (Node *root)
{
    queueinit();
    qpush(root);

    while (!queueempty())
    {
        root = qpop(); if (DEBUG) visit(root);
        if (root->l != 0) qpush(root->l);
        if (root->r != 0) qpush(root->r);
	free(root->d.vector);
        free(root);
    }

}
