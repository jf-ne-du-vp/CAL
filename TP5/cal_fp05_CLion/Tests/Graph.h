/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <stack>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;						// content of the vertex
	vector<Edge<T> > adj;		// outgoing edges
	
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool visited = false;		// auxiliary field
	bool processing = false;	// auxiliary field

	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;

	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

	//para Floyd-Warshall
	vector<vector<double>> distMin;     //matriz de distancias minimas
	vector<vector<Vertex<T> *>> vertexSeg;     //matriz de vertices seguintes;

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void unweightedShortestPath(const T &s);    //TODO...
	void dijkstraShortestPath(const T &s);      //TODO...
	void bellmanFordShortestPath(const T &s);   //TODO...
	vector<T> getPathTo(const T &dest) const;   //TODO...

	// Fp05 - all pairs
	void floydWarshallShortestPath();   //TODO...
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    queue<Vertex<T>*> tmp;


	for(auto v : vertexSet){
        v->path = NULL;
        v->dist = INF;
	}
    auto aux = findVertex(orig);
	aux->dist=0;
	tmp.push(aux);

	while(!tmp.empty()){
	    aux = tmp.front();
	    tmp.pop();
	    for(auto it = aux->adj.begin(); it != aux->adj.end(); it++){
	        if((*it).dest->dist == INF){
	            tmp.push((*it).dest);
                (*it).dest->dist = aux->dist + 1;
                (*it).dest->path = aux;
	        }
	    }
	}
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    MutablePriorityQueue<Vertex<T>> aux;

	for(auto v : vertexSet){
	    v->path = NULL;
	    v->dist = INF;
	    v->visited = false;
	}

	auto v = findVertex(origin);
	v->dist = 0;
	v->queueIndex = 0;
	aux.insert(v);

	while(!aux.empty()){
	    v = aux.extractMin();
	    for(auto w : v->adj){
	        if(w.dest->getDist() > (v->getDist() + w.weight)){
	            w.dest->dist = v->getDist() + w.weight;
	            w.dest->path = v;
	            if(!w.dest->visited){       //e por causa desta condicao que temos de por o atributo visited a false
	                aux.insert(w.dest);
	                w.dest->queueIndex = w.weight;
	                w.dest->visited = true;
	            }
	            else {
	                //w.dest->queueIndex = w.dest->dist;
	                aux.decreaseKey(w.dest);
	            }
	        }
	    }
	}
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	for(auto v : vertexSet){
	    v->path = NULL;
	    v->dist = INF;
	    v->visited = false;
	}

	auto v = findVertex(orig);
	v->dist = 0;

	for(int i = 0; i < vertexSet.size(); i++){
	    for(auto ver : vertexSet){
	        for(auto w : ver->adj){
	            if(w.dest->dist > ver->dist + w.weight){
	                w.dest->dist = ver->dist + w.weight;
	                w.dest->path = ver;
	            }
	        }
	    }
	}

	//nos slides diz para ver se ha ciclos de peso negativo

}


template<class T>
vector<T> Graph<T>::getPathTo(const T &dest) const{
	vector<T> res;
	stack<T> aux;   //como pomos do fim para o inicio a pilha e ideal, ou podemos usar reverse
	auto v = findVertex(dest);
	aux.push(v->info);

	while(v->getPath() != NULL){
	    v = v->getPath();
	    aux.push(v->info);
	}

	while(!aux.empty()){
	    res.push_back(aux.top());
	    aux.pop();
	}

	return res;
}



/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    int numVertex = getNumVertex();


    //por matrizes com o tamanho pretendido

    vertexSeg.resize(numVertex);
    for(auto v : vertexSeg)
        v.resize(numVertex);

    distMin.resize(numVertex);
    for(auto d: distMin) {
        d.resize(numVertex);
        cout << "o meu tamanho e : " << d.size() << endl;
    }


    //por tudo a infinito e anterior a NULL
    for(int i = 0; i < numVertex; i++){
        for(int j = 0; j < numVertex; j++){
            distMin.at(i).at(j) = INF;
            vertexSeg.at(i).at(j) = NULL;
        }
    }


    //distancia para ele mesmo a zero e o anterior ele mesmo
    for(int i = 0; i < numVertex; i++) {
        distMin.at(i).at(i) = 0;
        vertexSeg.at(i).at(i) = vertexSet[i];
    }


    //percorrer arestas e por dist para os anterior e o anterior
    for(auto v : vertexSet){
        for(auto a : v->adj) {
            int pos1 = -1, pos2 = -1;
            for (int i = 0; i < numVertex; i++) {
                if (vertexSet[i]->info == v->info) {
                    pos1 = i;
                } else if (vertexSet[i]->info == a.dest->info) {
                    pos2 = i;
                }
            }
            if (pos1 != -1 && pos2 != -1) {
                distMin[pos1][pos2] = a.weight;
                vertexSeg[pos1][pos2] = a.dest;
            }
        }
    }


    //distancias seguintes
    for(int k = 0; k < numVertex; k++){
        for(int i = 0 ; i < numVertex; i++){
            for(int j = 0; j < numVertex; j++){
                if(distMin[i][j] > distMin[i][k] + distMin[k][j]){
                    distMin[i][j] = distMin[i][k] + distMin[k][j];
                    vertexSeg[i][j] = vertexSeg[i][k];
                }
            }
        }
    }

}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;

	//como afinal a matriz de adjacencias tem o seguinte e nao o anterior ja nao preciso da stack
    //stack<T> aux;   //como pomos do fim para o inicio a pilha e ideal, ou podemos usar reverse
	auto v1 = findVertex(orig);
	auto v2 = findVertex(dest);

	if(v1 == NULL || v2 == NULL) return res;


	//saber as posicoes no vertexSet de cada um deles
    int pos1, pos2;
	for(int i = 0; i < getNumVertex(); i++){
	    if(vertexSet[i]->info == v1->info){
	           pos1 = i;
	    }
	}

	for(int i = 0; i < getNumVertex(); i++){
	    if(vertexSet[i]->info == v2->info){
	        pos2 = i;
	    }
	}

	if(vertexSeg[pos1][pos2] ==  NULL) return res;
	//agora preenchemos o vetor
    res.push_back(v1->info);
	while(v1->info != v2->info){
	    v1 = vertexSeg[pos1][pos2];
	    res.push_back(v1->info);
	    for(int i = 0; i < getNumVertex(); i++){
	        if(vertexSet.at(i)->info == v1->info){
	            pos1 = i;
	        }
	    }
	}

	/*
    while(!aux.empty()){
        res.push_back(aux.top());
        aux.pop();
    }
*/

	return res;
}


#endif /* GRAPH_H_ */
