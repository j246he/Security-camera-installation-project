#include <stdlib.h>
#include <stdio.h>
// defined std::unique_ptr

#include <memory>

// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <iostream>
//using namespace std;

#define MAXVEX 100
#define QueueSize 100
typedef char VertexType;
typedef int EdgeType;
int v = 0;
class MGraph {
public:
	VertexType vexs[MAXVEX];
	EdgeType arc[MAXVEX][MAXVEX];
	int numVertexes, numEdges;
};
int CreateMGraph(MGraph *G)
{
loop:
	int i, j, k;
	std::cin >> v;
	G->numVertexes = v;	//input the number of vertices
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
			G->numEdges = countFigure - DecNum;
			//std::cout << G->numEdges << std::endl;

			std::cin.clear();



			for (int i = 0; i < G->numVertexes; ++i) {
				for (int j = 0; j < G->numVertexes; ++j)
				{
					if (i == j) {
						G->arc[i][j] = 0;
					}
					else {
						G->arc[i][j] = 9999;
					}

				}
			}

			for (int k = 0; k < G->numEdges; k = k + 2) {
				i = stringFinal[k];
				j = stringFinal[k + 1];
				if (i != j) {
					G->arc[i][j] = 1;
					G->arc[j][i] = G->arc[i][j];
				}
			}

			delete[]stringFinal;
			return 1;
		}
	}
	else {
		return 2;
	}
	return 0;
}

/************************************************************************/
/* main                                                                 */
/************************************************************************/
int main() {
	//vec<Lit> add_tmp;
	std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
	char V;
	MGraph G;
	Minisat::Lit x[MAXVEX][MAXVEX];

	while (std::cin >> V) {
		int flag = 10;
		if (V == 'V') {

			flag = CreateMGraph(&G);
			if (flag == 0) {
				goto end;
			}
		}
		if (flag == 2) {
			std::cout <<std::endl;
		}
		else {
			int flagNoEdge = 0;
			/*for (int i = 0; i < v; ++i) {
				for (int j = 0; j < v; ++j) {
					std::cout << (&G)->arc[i][j] << "  ";
				}
				std::cout << std::endl;
			}*/

			for (int i = 0; i < v; ++i) {
				if (flagNoEdge == 2) {
					break;
				}
				for (int j = 0; j < v; ++j)
				{
					if (((&G)->arc[i][j] != 0) && ((&G)->arc[i][j] != 9999)) {
						flagNoEdge = 2;
						break;
					}
					else {
						/*std::cout << (&G)->arc[i][j] << std::endl;*/
						flagNoEdge = 1;
					}

				}
			}
			if (flagNoEdge == 1) {
				std::cout <<std::endl;
			}
			else {
				int vertexcover[v];
				int k = 1;
				for (k = 1; k < v; ++k) {
					for (int i = 0; i < v; ++i) {
						for (int j = 0; j < k; ++j) {
							x[i][j] = Minisat::mkLit(solver->newVar());
						}
					}
					//std::cout << k << std::endl;
					Minisat::vec<Minisat::Lit> constr1[k];
					for (int i = 0; i < k; ++i) {
						for (int j = 0; j < v; ++j) {
							constr1[i].push(x[j][i]);
						}
						solver->addClause(constr1[i]);
					}
					//std::cout << "1"<<" ";
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
					//std::cout << "2" << " ";
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
					//std::cout << "3" << " ";
					int count = 0;
					Minisat::vec<Minisat::Lit> constr4[((&G)->numEdges) * 2];
					for (int i = 0; i < v; ++i) {
						for (int j = 0; j < v; ++j)
						{
							if (((&G)->arc[i][j]) == 1) {
								for (int m = 0; m < k; ++m) {
									constr4[count].push(x[i][m]);
									constr4[count].push(x[j][m]);
								}
								solver->addClause(constr4[count]);
								count++;
							}
						}
					}
					//std::cout << "4" << std::endl;
					bool res = solver->solve();
					if (res == 1) {
						//std::cout << "the size of k:" << k << std::endl;
						int m = 0;
						for (int i = 0; i < v; ++i) {
							for (int j = 0; j < k; ++j) {
								if (Minisat::toInt(solver->modelValue(x[i][j])) == 0) {
									vertexcover[m] = i;
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
				if (k == v) {
					for (int i = 0; i < (v - 1); ++i) {
						std::cout << i << " ";
					}
					std::cout << v - 1 << std::endl;
				}
				else {
					for (int i = 0; i < (k - 1); ++i) {
						std::cout << vertexcover[i] << " ";
					}
					std::cout << vertexcover[k - 1] << std::endl;
				}
			}

		}


	}

end:	return 0;
}
