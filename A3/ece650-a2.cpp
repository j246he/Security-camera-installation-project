#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#endif
#endif

#include<iostream>
#include <stdio.h>
//#include <stdlib.h>
//#include <crtdbg.h>
using namespace std;

#define MaxVertexNum 100
#define QueueSize 100
int v=0;
int str;
int dst;


typedef enum { FALSE, TRUE }Boolean;
Boolean visited[MaxVertexNum];
int parent[MaxVertexNum];
typedef char VertexType;
typedef int EdgeType;


typedef struct node
{
    int adjvex;
    struct node *next;

}EdgeNode;

typedef struct vnode
{
    VertexType vertex;
    EdgeNode *firstedge;
}VertexNode;

typedef VertexNode AdjList[MaxVertexNum];

typedef struct
{
    AdjList adjlist;
    int n = 0, e;
}ALGraph;

/************************************************************************/
/* delete adj                                                           */
/************************************************************************/
void DeleteGraphAL(ALGraph *G)
{
    if (!G->n) {
        for (int i = 0; i < G->n; i++)
        {
            EdgeNode *q;
            EdgeNode *p = G->adjlist[i].firstedge;
            while (p)
            {
                q = p;
                p = p->next;
                delete q;
            }
            G->adjlist[i].firstedge = 0;
        }
    }

}
int CreateGraphAL(ALGraph *G)
{
    loop:
    DeleteGraphAL(G);
    int i, j, k;
    EdgeNode * s;

    cin >> v;
    G->n = v;	//input the number of vertices

    for (int i = 0; i < G->n; i++)
    {
        G->adjlist[i].vertex = i;
        G->adjlist[i].firstedge = 0;
    }

    char E;
    int num = (v + 1)*(v + 1) * 7;
    char *p = new char[num];
    int count = 0;
    int countFigure = 0;
    int DecNum = 0;
    int countP = 0;
    int tmp;

    cin >> E;
    for (int i = 0; i < num; ++i) {
        cin >> p[i];
        countP++;
        if (p[i] == '}') {
            countP++;
            break;
        }
    }//input E to p


    if (p[1] != '}') {
        k = 0;
        while (k < countP) {

            if (p[k] != '{'&&p[k] != '}'&&p[k] != '<'&&p[k] != '>'&&p[k] != ',') {
                if (k > 0) {
                    if ((p[k - 1] <= '9') && (p[k - 1] >= '0')) {
                        tmp = 10 * (p[k - 1] - 48) + p[k] - 48;
                        k++;
                    }
                    else {
                        tmp = p[k] - 48;
                        k++;
                    }
                }
                else {
                    tmp = p[k] - 48;
                    k++;
                }
            }
            else {
                tmp = 0;
                k++;
            }

            while (tmp >= v) {
                cerr << "Error:invalid edge." << endl;
                cin.clear();
                countP = 0;
                if (cin >> E) {
                    if (E == 'V') { goto loop; }
                    for (int i = 0; i < num; ++i) {
                        cin >> p[i];
                        countP++;
                        if (p[i] == '}') {
                            countP++;
                            break;
                        }
                    }//input E to p
                    k = 0;
                    tmp = 0;
                }
                else {
                    return 0;
                }

            }
        }


        if (v > 0) {
            i = 0;
            while (p[i] != '}') {
                count++;
                if (p[i] != '{'&&p[i] != '}'&&p[i] != '<'&&p[i] != '>'&&p[i] != ',') {
                    countFigure++;
                }
                i++;
            }//caculate size of p
            i = 0;
            j = 0;


            int *stringPointer = new int[countFigure];
            while (p[i] != '}') {
                if (p[i] != '{'&&p[i] != '}'&&p[i] != '<'&&p[i] != '>'&&p[i] != ',') {
                    if (i > 0) {
                        if ((p[i - 1] <= '9') && (p[i - 1] >= '0')) {
                            stringPointer[j - 1] = 10 * (p[i - 1] - 48) + p[i] - 48;
                            DecNum++;
                        }
                        else {
                            stringPointer[j] = p[i] - 48;
                            j++;
                        }
                    }
                    else {
                        stringPointer[j] = p[i] - 48;
                        j++;
                    }
                }
                i++;
            }
            delete[]p;

            int *stringFinal = new int[countFigure - DecNum];
            for (int i = 0; i < countFigure - DecNum; ++i) {
                stringFinal[i] = stringPointer[i];
            }
            delete[]stringPointer;
            G->e = countFigure - DecNum;


            for (int k = 0; k < G->e; k = k + 2)
            {

                i = stringFinal[k];
                j = stringFinal[k + 1];
                if (i != j) {
                    s = new EdgeNode;
                    s->adjvex = j;
                    s->next = G->adjlist[i].firstedge;
                    G->adjlist[i].firstedge = s;

                    s = new EdgeNode;
                    s->adjvex = i;
                    s->next = G->adjlist[j].firstedge;
                    G->adjlist[j].firstedge = s;
                }

            }
            delete[]stringFinal;
        }
    }
    return 1;
}

/************************************************************************/
/* BFS                                                                  */
/************************************************************************/
typedef struct
{
    int front;
    int rear;
    int count;
    int data[QueueSize];
}CirQueue;

void InitQueue(CirQueue *Q)
{
    Q->front = Q->rear = 0;
    Q->count = 0;
}

int QueueEmpty(CirQueue *Q)
{
    return Q->count == 0;
}

int QueueFull(CirQueue *Q)
{
    return Q->count == QueueSize;
}

void EnQueue(CirQueue *Q, int x)
{
    if (QueueFull(Q))
        cerr << "Queue overflow" << endl;
    else
    {
        Q->count++;
        Q->data[Q->rear] = x;
        Q->rear = (Q->rear + 1) % QueueSize;
    }
}

int DeQueue(CirQueue *Q)
{
    int temp;
    if (QueueEmpty(Q))
    {
        cerr << "Queue underflow" << endl;
        return 0;
    }
    else
    {
        temp = Q->data[Q->front];
        Q->count--;
        Q->front = (Q->front + 1) % QueueSize;
        return temp;
    }
}

void printPath() {
    int j = dst;
    int i = 0;
    int *Path = new int[v];
    while (parent[j] != str) {
        Path[i] = parent[j];
        j = parent[j];
        i++;
    }

    i--;
    cout << str << '-';
    for (int j = i; j >= 0; j--) {
        cout << Path[j] << '-';
    }
    cout << dst << endl;
    delete[]Path;
}

void BFS(ALGraph*G, int k)
{
    for (int i = 0; i < v; ++i) {
        parent[i] = -1;
    }
    int i;
    CirQueue Q;
    EdgeNode *p;
    InitQueue(&Q);


    parent[k] = k;
    visited[k] = TRUE;

    EnQueue(&Q, k);
    while (!QueueEmpty(&Q))
    {
        i = DeQueue(&Q);
        p = G->adjlist[i].firstedge;

        while (p)
        {

            if (!visited[p->adjvex])
            {

                visited[p->adjvex] = TRUE;
                EnQueue(&Q, p->adjvex);
                parent[p->adjvex] = i;
            }

            p = p->next;
        }
    }

    if (parent[dst] == -1) {
        cerr << "Error:the short path does not exist." << endl;
    }
    else {
        printPath();
    }

}

void BFSTraverseM(ALGraph *G, int str)
{
    for (int i = 0; i < G->n; i++)
        visited[i] = FALSE;
    BFS(G, str);
}


/************************************************************************/
/* main                                                                 */
/************************************************************************/
int main()
{

    char V;
    int strdst[2];
    ALGraph G;
    while (cin >> V) {
        if (V == 'V') {
            loop1:
            int flag = 2;
            flag=CreateGraphAL(&G);
            if (flag == 0) {
                goto end;
            }
        }
        if (V == 's') {

            for (int i = 0; i < 2; i++)
            {
                cin >> strdst[i];
            }
            while (strdst[0] >= v || strdst[1] >= v) {

                cerr << "Error:the vertice does not exist." << endl;
                cin.clear();
                if (cin >> V) {
                    if (V == 'V') { goto loop1; }
                    for (int i = 0; i < 2; i++)
                    {
                        cin >> strdst[i];
                    }
                }
                else {
                    goto end;
                }
            }
            str = strdst[0];
            dst = strdst[1];	//start and destination
            if (str == dst) {
                cout << str << endl;
            }
            else {
                BFSTraverseM(&G, str);
            }
        }
    }
    end:	return 0;
}
