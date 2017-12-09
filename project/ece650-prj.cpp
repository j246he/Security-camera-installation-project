#include "input.h"
#include "helper.h"
//using namespace std;

MGraph G;
int flag;
int isend;
int randomEdge1;
int randomEdge2;

//Intialize the mutex
pthread_mutex_t mutex_in = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_out = PTHREAD_MUTEX_INITIALIZER;


void pclock(char *, clockid_t);


int Random(int m, int n)
{
    int pos, dis;
    if (m == n)
    {
        return m;
    }
    else if (m > n)
    {
        pos = n;
        dis = m - n + 1;
        return rand() % dis + pos;
    }
    else
    {
        pos = m;
        dis = n - m + 1;
        return rand() % dis + pos;
    }
}


int main(){
    
    
    while(1){
        
        flag = 0;
        isend = 0;
        
        mulock(LOCK, &mutex_in);
        mulock(LOCK, &mutex_out);
        
        pthread_t thread_cnf, thread_approx1, thread_approx2, thread_IO;
        void *result_print;
        
        // Create thread for CNF
        if (pthread_create(&thread_cnf, NULL, &CNF_SAT_VC, (void *)&G) == -1) {
            puts("fail to create pthread thread_cnf");
            exit(1);
        }
        // Create thread for APPROX1
        if (pthread_create(&thread_approx1, NULL, &ApproxVc1, (void *)&G) == -1) {
            puts("fail to create pthread thread_approx1");
            exit(1);
        }
        // Create thread for APPROX2
        if (pthread_create(&thread_approx2, NULL, &ApproxVc2, (void *)&G) == -1) {
            puts("fail to create pthread thread_approx2");
            exit(1);
        }
        
        // Create thread for I/O
        if (pthread_create(&thread_IO, NULL, &IO_Thread, NULL) == -1) {
            puts("fail to create pthread thread_IO");
            exit(1);
        }
        
        //Wait thread terminate
        if (pthread_join(thread_IO, &result_print) == -1) {
            puts("fail to recollect thread_IO");
            exit(1);
        }
        if(isend == 1)
            return 0;
        else{
            
            int ret;
            //check the thread status
            if((ret = pthread_kill(thread_IO, 0))) {
            }
            else {
                //kill the thread
                ret = pthread_cancel(thread_IO);
            }
            //check the thread status
            if((ret = pthread_kill(thread_cnf, 0))) {
                
            }
            else {
                //kill the thread
                ret = pthread_cancel(thread_cnf);
            }
            //check the thread status
            if((ret = pthread_kill(thread_approx1, 0))) {
                
            }
            else {
                //kill the thread
                ret = pthread_cancel(thread_approx1);
            }
            //check the thread status
            if((ret = pthread_kill(thread_approx2, 0))) {
                
            }
            else {
                //kill the thread
                ret = pthread_cancel(thread_approx2);
            }
        }
        
    }
    
    return 0;
}


int CreateMGraph(MGraph *Graph)
{
loop:
    int i, j, k;
    std::cin >> v;
    Graph->numVertexes = v;    //input the number of vertices
    char E;
    int num = (v + 1)*(v + 1) * 7;
    char *p = new char[num];
    int count = 0;
    int countFigure = 0;
    int DecNum = 0;
    int countP = 0;
    int tmp;
    
    std::cin >> E;
    for (int i = 0; i < num; ++i) {
        std::cin >> p[i];
        countP++;
        if (p[i] == '}') {
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
                std::cerr << "Error:invalid edge." << std::endl;
                std::cin.clear();
                countP = 0;
                if (std::cin >> E) {
                    if (E == 'V') { goto loop; }
                    for (int i = 0; i < num; ++i) {
                        std::cin >> p[i];
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
            Graph->numEdges = countFigure - DecNum;
            //std::cout << G->numEdges << std::endl;
            
            std::cin.clear();
            
            
            
            for (int i = 0; i < Graph->numVertexes; ++i) {
                for (int j = 0; j < Graph->numVertexes; ++j)
                {
                    if (i == j) {
                        Graph->arc[i][j] = 0;
                    }
                    else {
                        Graph->arc[i][j] = 9999;
                    }
                    
                }
            }
            int randomedge;
            randomedge = Random(0, Graph->numEdges/2-1);
            //cout << Graph->numEdges << endl;
            randomEdge1 = stringFinal[2*randomedge];
            randomEdge2 = stringFinal[2*randomedge+1];
            //cout << randomedge << endl;
            //cout << randomEdge1 << "," << randomEdge2 << endl;
            
            for (int k = 0; k < Graph->numEdges; k=k+2){
                i = stringFinal[k];
                j = stringFinal[k + 1];
                if (i != j) {
                    Graph->arc[i][j] = 1;
                    Graph->arc[j][i] = Graph->arc[i][j];
                }
            }
            
            delete[]stringFinal;
            return 1;
        }
    }
    //else {
    return 2;
    //}
}


/************************************************************************/
/* CNF - SAT - VC                                                       */
/************************************************************************/

void* CNF_SAT_VC(void *graph){
    mulock(LOCK, &mutex_in);
    //std::cout << "This is CNF!" << std::endl;
    MGraph * Graph = (MGraph *)graph;
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    Minisat::Lit x[MAXVEX][MAXVEX];
    
    int k = 1;
    for (k = 1; k < v; ++k) {
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < k; ++j) {
                x[i][j] = Minisat::mkLit(solver->newVar());
            }
        }
        
        Minisat::vec<Minisat::Lit> constr1[k];
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < v; ++j) {
                constr1[i].push(x[j][i]);
            }
            solver->addClause(constr1[i]);
        }
        
        Minisat::vec<Minisat::Lit> constr2[v*k*(k - 1) / 2];
        int i = 0;
        for (int m = 0; m < v; ++m) {
            for (int p = 0; p < k; ++p) {
                for (int q = 0; q < k; ++q) {
                    if (p < q) {
                        constr2[i].push(~x[m][p]);
                        constr2[i].push(~x[m][q]);
                        solver->addClause(constr2[i]);
                        i++;
                    }
                }
            }
        }
        
        Minisat::vec<Minisat::Lit> constr3[k*v*(v - 1) / 2];
        int j = 0;
        for (int m = 0; m < k; ++m) {
            for (int p = 0; p < v; ++p) {
                for (int q = 0; q < v; ++q) {
                    if (p < q) {
                        constr3[j].push(~x[p][m]);
                        constr3[j].push(~x[q][m]);
                        solver->addClause(constr3[j]);
                        j++;
                    }
                }
            }
        }
        
        int count = 0;
        Minisat::vec<Minisat::Lit> constr4[(Graph->numEdges) * 2];
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < v; ++j)
            {
                if ((Graph->arc[i][j]) == 1) {
                    for (int m = 0; m < k; ++m) {
                        constr4[count].push(x[i][m]);
                        constr4[count].push(x[j][m]);
                    }
                    solver->addClause(constr4[count]);
                    count++;
                }
            }
        }
        
        bool res = solver->solve();
        if (res == 1) {
            //std::cout << "the size of k:" << k << std::endl;
            int m = 0;
            for (int i = 0; i < v; ++i) {
                for (int j = 0; j < k; ++j) {
                    if (Minisat::toInt(solver->modelValue(x[i][j])) == 0) {
                        Graph -> cnf_vc[m] = i;
                        m++;
                    }
                }
            }
            solver.reset(new Minisat::Solver());
            break;
        }
        else {
            solver.reset(new Minisat::Solver());
            continue;
        }
        
    }
    Graph-> cnf_size = k;
    
    // 	clockid_t cid;
    // 	int retcode;
    // 	retcode = pthread_getcpuclockid(pthread_self(), &cid);
    
    // 	if (retcode) {
    // 		errp("pthread_getcpuclockid", retcode);
    // 	}
    // 	else {
    // 		pclock("Thread CNF cpu time: ", cid);
    // 	}
    
    //mulock(LOCK, &mutex_flag);
    flag++;
    if(flag == 3){
        mulock(UNLOCK,&mutex_out);
    }
    //mulock(UNLOCK, &mutex_flag);
    mulock(UNLOCK, &mutex_in);
    
    
    return NULL;
}


/************************************************************************/
/* APPROX - VC - 1                                                      */
/************************************************************************/

void* ApproxVc1(void *graph) {
    
    
    mulock(LOCK, &mutex_in);
    //std::cout << "This is A1!" << std::endl;
    MGraph * Graph = (MGraph *)graph;
    
    /*check if the matrix is 0*/
    int degree[v];
    int max = -1;
    int index = 0;
    //int S[v];
    int count = 0;
    int arc[v][v];
    
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            arc[i][j] = Graph->arc[i][j];
        }
        /*cout << endl;*/
    }
    
    while (max != 0) {
        
        for (int i = 0; i < v; ++i) {
            degree[i] = 0;
        }
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < v; ++j) {
                if (arc[i][j] == 1) {
                    degree[i]++;
                }
            }
        }
        
        max = degree[0];
        index = 0;
        for (int i = 1; i < v; ++i) {
            if (degree[i] > max) {
                max = degree[i];
                index = i;
            }
        }
        Graph -> approx1_vc[count] = index;
        count++;
        for (int i = 0; i < v; ++i) {
            arc[index][i] = 0;
            arc[i][index] = 0;
        }
        
    }
    
    sort(Graph -> approx1_vc, Graph -> approx1_vc + count - 1);
    Graph-> approx1_size = count;
    
    
    // 	clockid_t cid;
    // 	int retcode;
    // 	retcode = pthread_getcpuclockid(pthread_self(), &cid);
    
    // 	if (retcode) {
    // 		errp("pthread_getcpuclockid", retcode);
    // 	}
    // 	else {
    // 		pclock("Thread APPROX1 cpu time: ", cid);
    // 	}
    
    //mulock(LOCK, &mutex_flag);
    flag++;
    if(flag == 3){
        mulock(UNLOCK,&mutex_out);
    }
    //mulock(UNLOCK, &mutex_flag);
    mulock(UNLOCK, &mutex_in);
    
    
    return NULL;
}


/************************************************************************/
/* APPROX - VC - 2                                                      */
/************************************************************************/

void* ApproxVc2(void *graph) {
    
    mulock(LOCK, &mutex_in);
    
    //std::cout << "This is A2!" << std::endl;
    MGraph * Graph = (MGraph *)graph;
    int arc[v][v];
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            arc[i][j] = Graph->arc[i][j];
        }
    }
    
    for (int i = 0; i < v; ++i) {
        Graph -> approx2_vc[i] = -1;
    }
    int count1 = 2;
    int flagIn = 0;
    //random edge
    Graph->approx2_vc[0] = randomEdge1;
    Graph->approx2_vc[1] = randomEdge2;
    for (int k = 0; k < v; ++k) {
        arc[randomEdge1][k] = 0;
        arc[k][randomEdge1] = 0;
        arc[randomEdge2][k] = 0;
        arc[k][randomEdge2] = 0;
        
    }
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            if (arc[i][j] == 1) {
                for (int k = 0; k < v; ++k) {
                    if (( Graph -> approx2_vc[k] == i) || ( Graph -> approx2_vc[k] == j)) {
                        flagIn = 1;
                        break;
                    }
                    else {
                        flagIn = 2;
                    }
                }
                if (flagIn == 2) {
                    Graph -> approx2_vc[count1] = i;
                    count1++;
                    Graph -> approx2_vc[count1] = j;
                    count1++;
                    for (int k = 0; k < v; ++k) {
                        arc[i][k] = 0;
                        arc[k][i] = 0;
                        arc[j][k] = 0;
                        arc[k][j] = 0;
                        
                    }
                }
            }
        }
    }
    sort( Graph -> approx2_vc,  Graph -> approx2_vc + count1);
    Graph-> approx2_size = count1;
    
    // 	clockid_t cid;
    // 	int retcode;
    // 	retcode = pthread_getcpuclockid(pthread_self(), &cid);
    
    // 	if (retcode) {
    // 		errp("pthread_getcpuclockid", retcode);
    // 	}
    // 	else {
    // 		pclock("Thread APPROX2 cpu time: ", cid);
    // 	}
    
    
    //mulock(LOCK, &mutex_flag);
    flag++;
    if(flag == 3){
        mulock(UNLOCK,&mutex_out);
    }
    //mulock(UNLOCK, &mutex_flag);
    mulock(UNLOCK, &mutex_in);
    
    return NULL;
}


/************************************************************************/
/* Output-Result                                                        */
/************************************************************************/

void* IO_Thread(void *args){
    
    //std::cout << "This is I/O! "<<std::endl;
    char V;
    int n = 10;
    //Input
    if(std::cin.peek() == EOF) {
        isend = 1;
        return NULL;
        
    }
    
    else{
        std::cin >> V;
        if (V == 'V') {
            n = CreateMGraph(&G);
            if (n == 0) {
                isend = 1;
                return NULL;
            }
        }
        else {
            isend = 1;
            return NULL;
        }
        
    }
    
    
    mulock(UNLOCK, &mutex_in);
    mulock(LOCK, &mutex_out);
    
    MGraph * Graph = &G;
    
    //Output
    if (n == 2) {
        
        std::cout <<"CNF-SAT-VC: "<<std::endl;
        std::cout << "APPROX-VC-1: " << std::endl;
        std::cout << "APPROX-VC-2: " << std::endl;
    }
    else{
        
        //Output CNF result
        std::cout << "CNF-SAT-VC: ";
        for (int i = 0; i < (Graph ->cnf_size - 1); ++i) {
            std::cout << Graph -> cnf_vc[i] << ",";
        }
        std::cout << Graph -> cnf_vc[Graph ->cnf_size - 1] << std::endl;
        
        //Output APPROX1 result
        std::cout << "APPROX-VC-1: ";
        for (int i = 0; i < Graph ->approx1_size - 2; ++i) {
            std::cout << Graph ->approx1_vc[i] << ",";
        }
        std::cout << Graph ->approx1_vc[Graph ->approx1_size - 2] << std::endl;
        
        //Output APPROX2 result
        std::cout << "APPROX-VC-2: ";
        for (int i = 0; i < Graph ->approx2_size - 1; ++i) {
            std::cout << Graph ->approx2_vc[i] << ",";
        }
        std::cout << Graph ->approx2_vc[Graph ->approx2_size - 1] << std::endl;
    }
    mulock(UNLOCK, &mutex_out);
    
    return NULL;
}
