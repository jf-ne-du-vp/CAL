/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

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
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(const T &source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	// TODO (4 lines)
	// HINT: use the findVertex function to check if a vertex already exists
	if(findVertex(in) != NULL) return false;

	vertexSet.push_back(new Vertex<T>(in));

	return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	// TODO (6 lines)
	// HINT: use findVertex to obtain the actual vertices
	// HINT: use the next function to actually add the edge
	//primeiro ver se existe sourc e dest
	Vertex<T>* src = findVertex(sourc);
	Vertex<T>* dst = findVertex(dest);

	if(src == NULL || dst == NULL) return false;

	//agora chama a proxima para adicionar a aresta ao vertice de partida(src) o seu vertice de destino e o seu peso
	src->addEdge(dst,w);

	return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d,w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	// TODO (5 lines)
	// HINT: Use "findVertex" to obtain the actual vertices.
	// HINT: Use the next function to actually remove the edge.
    auto src = findVertex(sourc);
    auto dst = findVertex(dest);


	return src->removeEdgeTo(dst);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	// TODO (6 lines)
	// HINT: use an iterator to scan the "adj" vector and then erase the edge.
	for(auto it = adj.begin(); it != adj.end(); it++){
	    if(it->dest == d){
	        adj.erase(it);
            return true;
	    }
	}

	return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
	// TODO (10 lines)
	// HINT: use an iterator to scan the "vertexSet" vector and then erase the vertex.
	// HINT: take advantage of "removeEdgeTo" to remove incoming edges.
	auto vRm = findVertex(in);
	if(vRm == NULL) return false;

	//aqui percorre os vertices do grafo, e em cada vertice e que ve (na funcao removeEdgeTo) se ha arestas com destino do verttice que se quer apagar
	for(auto it = vertexSet.begin(); it != vertexSet.end(); it++){
        (*it)->removeEdgeTo(vRm);
	}

	//agora e que apagamos o vertice em questao
	//so uso ciclo porque no erase tenho que lhe dar um iterador da posicao
	for(auto it = vertexSet.begin(); it != vertexSet.end(); it++){
	    if(*it == vRm){
	        vertexSet.erase(it);
	        break;
	    }
	}
	return true;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::dfs() const {
	// TODO (7 lines)
	vector<T> res;

	for(auto it : vertexSet){
	    it->visited = false;
	}

	for(auto it : vertexSet){
	    if(!it->visited){
	        dfsVisit(it,res);
	    }
	}

	return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
	// TODO (7 lines)
	res.push_back(v->info);
	v->visited = true;

	for(auto it : v->adj){
	    if(!it.dest->visited){
	        dfsVisit(it.dest,res);
	    }
	}
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {
	// TODO (22 lines)
	// HINT: Use the flag "visited" to mark newly discovered vertices .
	// HINT: Use the "queue<>" class to temporarily store the vertices.
	vector<T> res;
	queue<Vertex<T>*> tmp;

	for(auto v :vertexSet){
	    v->visited = false;
	}

	tmp.push(findVertex(source));

	while(!tmp.empty()){
	    Vertex<T> *aux = tmp.front();
	    tmp.pop();
	    res.push_back(aux->info);
	    aux->visited = true;
	    for(auto it : aux->adj){
	        if(!(it.dest->visited)){
	            tmp.push(it.dest);
	        }
	    }
	}

	return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
vector<T> Graph<T>::topsort() const {
	// TODO (26 lines)
	vector<T> res;
	queue<Vertex<T> *> tmp;

	//reset indegree
	for(auto v : vertexSet){
	    v->indegree = 0;
	}

	//calcular indegree
	for(auto it = vertexSet.begin(); it != vertexSet.end(); it++){
	    for(auto in = (*it)->adj.begin(); in != (*it)->adj.end(); in++){
	        in->dest->indegree += 1;
	    }
	}

	//ver se ha algum com indegree = 0, que e por ai que queremos comecar
	for(auto v : vertexSet){
	    if(v->indegree == 0){
	        tmp.push(v);
	    }
	}

	while(!tmp.empty()){
	    Vertex<T> * v = tmp.front();
	    tmp.pop();
	    res.push_back(v->info);
	    for(auto it : v->adj){
	        it.dest->indegree -=1;
	        if(it.dest->indegree == 0){
	            tmp.push(it.dest);
	        }
	    }
	}

	return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {
	// TODO (28 lines, mostly reused)
	auto src = findVertex(source);
	queue<Vertex<T> *> tmp;
	int res = 0; //default return
	int aux; //to compare with res

	if(src == NULL) return 0;
	for(auto v : vertexSet){
	    v->visited = false;
	}

	tmp.push(src);

	while(!tmp.empty()){
	    Vertex<T> * v = tmp.front();
	    tmp.pop();
	    v->visited = true;
	    aux = 0; //needs to be restarted in every cicle
	    for(auto it = v->adj.begin(); it != v->adj.end(); it++){
	        if(it->dest->visited == false){
	            tmp.push(it->dest);
	            aux++;
	        }
	    }
	    //remember to update inf as well!!!!
	    if(aux > res){
	        res = aux;
	        inf = v->info;
	    }
	}

	return res;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {
	// TODO (9 lines, mostly reused)
	// HINT: use the auxiliary field "processing" to mark the vertices in the stack.
	return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
	// TODO (12 lines, mostly reused)
	return true;
}

#endif /* GRAPH_H_ */
