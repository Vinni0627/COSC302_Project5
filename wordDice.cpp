/*

Project 5
Vineeth Konjeti, Amal Sam

Last Edited: 4/21/2023

In this project, we are meant to use network flow (Edmonds-Karp algorithm) to be able to use specific lettered dice
and try to make different words with them.


*/





#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<string>



using namespace std;

typedef enum{SOURCE, SINK, WORD, DICE} Node_Type;

//classes set up using Camille Crumpton's classes from CS302
class Edge{
	public:
		//from -> to
		class Node *to; //node edge is pointing to
		class Node *from; //node edge is pointing from

		//constructor for edges
		Edge(class Node *to, class Node *from, bool reverse = false);
		~Edge(){};

		Edge *reverse; //edge going the other way
		int original; //original weight per edge
		int residual; //allows for updated weighting during Edmunds-Karp
};

class Node{
	public:
		//constructor for nodes
		Node(int id, Node_Type type, string word = "");
		~Node(){};

		int id; //node id
		Node_Type type; //type of node it is (source, sink, word or dice)
		vector<bool> letters; //length 26 with letters contained in word set to 1
		int visited; //for BFS
		vector<Edge*> adj; //adjacency list
		Edge *backedge; //previous edge for edmunds karp
};

class Graph{
	public:
		//constructor initializes graph with source node
		Graph();

		//destructor to deallocate memory of graph
		~Graph();

		Node *source; //not necessary but makes code more readable
		vector<Node *> nodes; //holds the nodes
		vector<int> spellingIds; //order of flow to spell word
		int min_nodes; //min number of dice nodes

		void delete_word_from_graph(); //deletes the word nodes but leaves the dice nodes
		bool BFS(); //breadth first search for Edmunds-Karp
		bool spell_word(); //runs Edmunds-Karp to see if we can spell the word
		void print_node_order(string word); //if we find the word can be spelled print out details
};

//Edge constructor 
Edge::Edge(Node *to, Node *from, bool reverse) {
	this->to = to;
	this->from = from;
	if (!reverse) {
		original = 1;
		residual = 0;
		this->reverse = new Edge(from, to, true);
		this->reverse->reverse = this;
	}
	else if (reverse) {
		original = 0;
		residual = 1;
	}
}

//Node constructor 
Node::Node(int id, Node_Type type, string word) {
	this->id = id;
	this->type = type;
	letters.resize(26, 0);
	if (type == DICE || type == WORD) {
		for (int i = 0; i < word.size(); i++) letters[word[i] - 'A'] = 1;
	}
	else if (type == SOURCE || type == SINK) {
		for (int i = 0; i < letters.size(); i++) letters[i] = 1;
	}
	visited = 0;
}

//Graph constructor
Graph::Graph() {
	source = new Node(0, SOURCE);
	nodes.push_back(source);
}

//Graph Destructor -- deletes all edges and nodes as well
Graph::~Graph() {
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes[i]->adj.size(); j++) {
			delete nodes[i]->adj[j];
		}
		delete nodes[i];
	}
}

void Graph::delete_word_from_graph() {
	//deletes the edges from nodes word to sink, sink to word, and dice to word
	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes[i]->adj.size(); j++) {
			if (nodes[i]->adj[j]->to->type == WORD) delete nodes[i]->adj[j];
			else if (nodes[i]->adj[j]->from->type == WORD) delete nodes[i]->adj[j];
		}
		if (nodes[i]->type == DICE) nodes[i]->adj.resize(1);
	}

	spellingIds.clear();
	//sets all the edges to their original value before calling spell_word()
    //deletes the word and sink nodes
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes[i]->type == WORD) delete nodes[i];
		else if (nodes[i]->type == SINK) delete nodes[i];
		else {
			for (int j = 0; j < nodes[i]->adj.size(); j++) {
				if (nodes[i]->adj[j]->from->id < nodes[i]->adj[j]->to->id) {
					nodes[i]->adj[j]->original = 1;
					nodes[i]->adj[j]->residual = 0;
					nodes[i]->adj[j]->reverse->original = 0;
					nodes[i]->adj[j]->reverse->residual = 1;
				}
			}
		}
	}
	nodes.resize(min_nodes);
}

//Breadth for Search - used in Edmonds-Karp
bool Graph::BFS() {
	Edge *edge;
	Node *temp;
	int count = 0;
	queue<Node*> q;

	//sets all the backedges and visited for BFS
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->visited = -1;
		nodes[i]->backedge = NULL;
	}

	source->visited = 0;
	q.push(source);

	//finds the shortest path to the sink
	while(!q.empty()) {
		temp = q.front();
		q.pop();
		for (int i = 0; i < temp->adj.size(); i++) {
			edge = temp->adj[i];
			if (edge->to->visited == -1 && edge->original == 1) {
				edge->to->visited = edge->from->visited + 1;
				edge->to->backedge = edge;
				q.push(edge->to);
				count++;
			}
		}
	}
	//no longer finds a path, return false
	if (count == 0) return false;
	return true;
}

//check if we can spell word with edmonds karp algorithm
bool Graph::spell_word() {
	bool check = true;
	int words = nodes.size() - min_nodes - 1;
	int numLetters = 0;
	Edge *reverse;
	Edge *e;
	Node *temp;

	//calling bfs to check dice
	while (check) {
		temp = nodes[nodes.size() - 1];
		check = BFS();
		//if a path exists, traverse sink to source and reverse original and residual
		while (check) {
			if (temp == source) break;
			e = temp->backedge;
			if (e == NULL) {
				check = false; 
				break;
			}
			reverse = e->reverse;
			e->original = 0;
			e->residual = 1;
			reverse->original = 1;
			reverse->residual = 0;
			temp = e->from;
		}
	}

	//check if word able to be spelled
	for (int i = min_nodes; i < nodes.size() - 1; i++) {
		for (int j = 0; j < nodes[i]->adj.size(); j++) {
			if (nodes[i]->adj[j]->to->type == SINK && nodes[i]->adj[j]->residual == 1) {
				numLetters++;
			}
			if (nodes[i]->type == WORD && nodes[i]->adj[j]->original == 1) spellingIds.push_back(nodes[i]->adj[j]->to->id);
		}
		
	}
	//if the spellingIDs vector == word node #, then it can spell the word
	if (numLetters == words) return true;
	return false;
}

//deletes the edges from nodes word to sink, sink to word, word to dice, and dice to word


void add_word_to_graph(Graph &G, string word, int id) {
	Node *temp;
	Edge *e;
	int i, k;
	int j;
	string letter;
	int count = id;

	for (i = 0; i < word.size(); i++) {
		letter = word[i];
		temp = new Node(count, WORD, letter);
		G.nodes.push_back(temp);
		for (j = 1; j < G.min_nodes; j++) {
			for (k = 0; k < temp->letters.size(); k++) {
				if ((G.nodes[j]->letters[k] == temp->letters[k]) && (temp->letters[k] == 1)) {
					e = new Edge(temp, G.nodes[j]);                                                 //adds corresponding edges
					G.nodes[j]->adj.push_back(e);
					temp->adj.push_back(e->reverse);    
				}
			}
		}
		count++;
	}
}

void add_dice_to_graph(Graph &G, string die, int id) {
	Node *dice;
	Edge *edge;
	dice = new Node(id, DICE, die);
	G.nodes.push_back(dice);
	edge = new Edge (dice, G.source);       
	G.source->adj.push_back(edge);              //adds corresponding edges
	dice->adj.push_back(edge->reverse);
}

//Print function
void Graph::print_node_order(string word) {
	for (int i = 0; i < spellingIds.size(); i++) {
		if (i == spellingIds.size() - 1) cout << spellingIds[i] - 1 << ": ";
		else cout << spellingIds[i] - 1 << ",";
	}
}

/* MAIN FUNCTION */
int main(int argc, char *argv[]){
	ifstream fin;
	string file1, file2;
	string line;
	Graph *graph = new Graph;
	Node *sink;
	Edge *e;
	int i, before;
	int count = 1;
	bool spellable;

	if (argc != 3) cerr << "usage: worddice dice-file word-list\n";
	
	file1 = argv[1];
	file2 = argv[2];

	fin.open(file1);
	if (!fin.is_open()) cerr << "error: unable to open file\n";

	while(getline(fin, line)) {
		add_dice_to_graph(*graph, line, count);
		count++;
	}
	graph->min_nodes = graph->nodes.size();

	fin.close();
	//opens the word file
	fin.open(file2);
	if (!fin.is_open()) cerr << "error: unable to open file\n";

	//inserts the word nodes, the sink, and all of their edges to the graph, also checks if the word can be spelled and prints the result
	while(getline(fin, line)) {
		add_word_to_graph(*graph, line, count);
		//creates the edges for the sink
		before = graph->nodes.size();
		sink = new Node(graph->nodes.size(), SINK);
		graph->nodes.push_back(sink);
		for (i = graph->min_nodes; i < before; i++) {
			e = new Edge(sink, graph->nodes[i]);
			graph->nodes[i]->adj.push_back(e);
			sink->adj.push_back(e->reverse);
		}
		//checks if the word can be spelled
		spellable = graph->spell_word();
		if (spellable) {
			graph->print_node_order(line);
            cout << line << endl;
		}
		else cout << "Cannot spell " << line << endl;
		graph->delete_word_from_graph();
	}
	fin.close();
	delete graph;
	return 0;
}