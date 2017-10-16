#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>

using namespace std;
using namespace boost;

//---Direction Enumeration---// 
typedef enum {X, N, NE, NW, S, SE, SW, E, W, J} Direction;

//---Declarations for Boost Library types---// 
typedef property<edge_name_t, string, property<edge_weight_t, int> > EdgeProperty;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeProperty> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, edge_name_t>::type NameMap;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

//---Function Declarations---// 
int checkBounds(int i, int j, int r, int c);
Direction strtodir(string inString);
void buildGraph(Graph& g, vector<vector<string> >& vec, int r, int c, int& stop);
bool findJoJo(Graph& g, stack<Edge>& path, int start, int stop, int vertices);
void showOutput(stack<Edge> path, NameMap names, WeightMap weights);



int main()
{

	//---Variable and Data Struture Declarations---//
	int row, col, start_r, start_c, start_v, stop_v, numVertices;
	vector<vector<string> > jungleMap;
	Graph jungleGraph;
	stack<Edge> pathToJoJo;

	ifstream inFile;

	// Open Input File 
	inFile.open("tarzan.txt");

	if(!inFile.is_open()) 
	{
		cout << "Sorry, file could not be opened. Please try again." << endl;
		cout << endl;
		return -1;
	}

	
	inFile >> row >> col; 
	inFile >> start_r >> start_c;


	jungleMap.resize(row, vector<string>(col));		// Create 2D vector of size r by c
	numVertices = row*col;							// Calculate number of vertices
	start_v = col*(start_r-1) + (start_c-1);		// Calculate vertex number of starting point

	
	for(int i = 0; i < row; ++i)
		for(int j = 0; j < col; ++j)				// Read in data from tarzan.txt file
			inFile >> jungleMap[i][j];				// Assign to corresponding element of jungleMap

	
	//---Function Calls---//

	buildGraph(jungleGraph, jungleMap, row, col, stop_v);

	bool found;
	found = findJoJo(jungleGraph, pathToJoJo, start_v, stop_v, numVertices);
	if(found)
	{
		WeightMap weights = get(edge_weight, jungleGraph);
		NameMap names = get(edge_name, jungleGraph);
		showOutput(pathToJoJo, names, weights);
		
	}
	else
	{
		cout << "Sorry Tarzan, we couldn't find JoJo." << endl;
	}


	return 0;

}

/*==========================================================
Input: index, i and j
Output: the vertex number associated with index i and j;
if functions returns -1, vertex number does not exist
===========================================================*/
int checkBounds(int i, int j, int r, int c)
{
	if(i >= 0 && i < r && j >= 0 && j < c)
		return i*c + j;
	else
		return -1;
}
/*=========================================================
Converts a string type to a Direction type
Input: string 
Output: enumeration of the string value
=========================================================*/

Direction strtodir(string inString)
{
	if(inString == "X")		return X;
	if(inString == "N")		return N;
	if(inString == "NE")	return NE;
	if(inString == "NW")	return NW;
	if(inString == "S")		return S;
	if(inString == "SE")	return SE;
	if(inString == "SW")	return SW;
	if(inString == "E")		return E;
	if(inString == "W")		return W;
	if(inString == "J")		return J;
}

/*==========================================================
Input: Empty Graph g, 2D vector of strings, number of rows,
number of columns, and location of JoJo in 2D vector
Output: Weighted Directed Adjacency List representing jungleMap
==========================================================*/
void buildGraph(Graph& g, vector<vector<string> >& vec, int r, int c, int& stop)
{
	int vertex, check, storei[2], storej[2];

	for(int i = 0; i < r; ++i)
	{
		for(int j = 0; j < c; ++j)
		{
		
			switch(strtodir(vec[i][j]))
			{
				case X: 	continue;

				case J: 	stop = i*c + j; continue;

				case N: 	storei[0] = i-3, storei[1] = i-4;
							storej[0] = j, storej[1] = j;		break;

				case NE: 	storei[0] = i-3, storei[1] = i-4;
							storej[0] = j+3, storej[1] = j+4; 	break; 

				case NW:	storei[0] = i-3, storei[1] = i-4;
							storej[0] = j-3, storej[1] = j-4; 	break;

				case S:		storei[0] = i+3, storei[1] = i+4;
							storej[0] = j, storej[1] = j;		break;

				case SE:	storei[0] = i+3, storei[1] = i+4;
							storej[0] = j+3, storej[1] = j+4;	break;

				case SW:	storei[0] = i+3, storei[1] = i+4;
							storej[0] = j-3, storej[1] = j-4;	break;

				case E:		storei[0] = i, storei[1] = i;
							storej[0] = j+3, storej[1] = j+4;	break;

				case W: 	storei[0] = i, storei[1] = i;
							storej[0] = j-3, storej[1] = j-4;	break;
			}

			vertex = i*c + j;	

			check = checkBounds(storei[0], storej[0], r, c);
			if(check != -1 && vec[storei[0]][storej[0]] != "X")
				add_edge(vertex, check, EdgeProperty(vec[i][j], 3), g);

			check = checkBounds(storei[1], storej[1], r, c);
			if(check != -1 && vec[storei[1]][storej[1]] != "X")
				add_edge(vertex, check, EdgeProperty(vec[i][j], 4), g);				
		}
	}	

}

/*==========================================================================
Input: Graph g, empty stack path, starting position, stopping position, and
number of vertices
Output: Whether or not a path to JoJo exists; if path exists, empty stack is
filled with edges that lead to JoJo.
===========================================================================*/
bool findJoJo(Graph& g, stack<Edge>& path, int start, int stop, int vertices)
{
	Vertex start_V = vertex(start, g);
	Vertex stop_V = vertex(stop, g);

	vector<Vertex> pred(vertices);
	vector<int> dist(vertices);

	dijkstra_shortest_paths_no_color_map(g, start_V, distance_map(&dist[0]).predecessor_map(&pred[0]));

	for(Vertex u = pred[stop_V]; u != stop_V; stop_V = u, u = pred[stop_V])
		path.push(edge(u, stop_V, g).first);
	

	if(stop_V == start_V)
		return true;
	else
		return false; 
}

/*=====================================================================
Input: Stack containing path to JoJo, names, and weights of each edge
Output: file "Output.txt" containing list of edges leading to JoJo
=====================================================================*/
void showOutput(stack<Edge> path, NameMap names, WeightMap weights)
{
	ofstream outFile;
	outFile.open("output.txt");

	while(!path.empty())
	{
		outFile << names[path.top()] << "-" << weights[path.top()] << " ";
		path.pop();
	}

	cout << endl;
	outFile.close();
}