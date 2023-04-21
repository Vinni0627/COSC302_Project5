/*
Project 5

Inspired by Camille Crumpton's video explanation


*/



#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;



class Node{
    public:
        typedef enum{SOURCE, SINK, WORD, DICE} Node_Type;
        Node(int id, Node_Type type, string word); //constructor for nodes
        ~Node(); //default destructor
        bool has_letter(char c);
        int id; //node id
        Node_Type type; //type of node it is (source, sink, word or dice)
        vector<bool> letters; //length 26 with letters contained in word set to 1
        int visited; //for BFS
        vector<class Edge*> adj; //adjacency list
        class Edge *backedge; //previous edge for Edmonds-Karp
};


class Edge{
    public:
    //from -> to
        Node *to; //node edge is pointing to
        Node *from; //node edge is pointing from
        Edge(Node *to, Node *from, bool reverse = false); //constructor for edges
        ~Edge(){}; //default destructor
        Edge *reverse; //edge going the other way
        int original; //original weight per edge
        int residual; //allows for updated weighting during Edmonds-Karp
};



class Graph{
    public:
        Graph(); //constructor initializes graph with source node
        ~Graph(); //destructor to deallocate memory of graph
        Node *source; //not necessary but makes code more readable
        Node *sink;
        vector<Node *> nodes; //holds the nodes
        vector<int> spellingIds; //order of flow to spell word
        int min_nodes; //min number of dice nodes
        string word;

        bool BFS(); //breadth first search for Edmonds-Karp
        bool spell_word(); //runs Edmonds-Karp to see if we can spell the word
        void delete_word_from_graph(); //deletes the word nodes but leaves the dice nodes
        void print_node_order(string word); //print spelling Ids and word
};



Node::Node(int id, Node_Type type, string word) {
    this->id = id;
    this->type = type;
	this->letters.resize(26, false);

    if (type == DICE || type == WORD) {
        //cout << word << endl;
        for (int i = 0; i < word.size(); i++) {
            //cout << word[i] << endl;
            this->letters[word[i]-'A'] = true;
        }
    } else if(type == SOURCE || type == SINK) {
		for (int i = 0; i < letters.size(); i++) {
			letters[i] = 1;
		}
	}
	this->visited = false;
}

// Edge Constructor
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

// Graph Constructor
Graph::Graph(){
    min_nodes = 0;
    source = new Node(0, Node::Node_Type::SOURCE, "");
    
}

// Graph Deconstructor
Graph::~Graph(){
    for(auto &node:nodes) {
        for(auto &edge:node->adj) {
			delete edge;
		}
		delete node;
    }
}

bool Graph::BFS(){
    int count = 0;
	queue<Node*> q;

	Node *temp;
	Edge *edge;

	for(auto &node:nodes){
		node->backedge = NULL;
		node->visited = -1;
	}

	source->visited = 0;
	q.push(source);

	while(!q.empty()){

		temp = q.front();
		q.pop();

		for(int i = 0; i < temp->adj.size(); i++){
			edge = temp->adj[i];

			if(edge->to->visited == -1 && edge->original == 1){
				edge->to->visited = edge->from->visited + 1;
				q.push(edge->to);
				count++;
			}
		}
	}
	if(count == 0) return false;
	return true;
}

bool Graph::spell_word(){           //Edmonds-Karp algorithm
	Node *temp;
    Edge *edge;
    Edge *rev;
	int num_letters = 0;
    bool check = true;

    int num_word = nodes.size() - min_nodes - 1;

    while(check){
        temp = nodes[nodes.size()-1];
		check = BFS();
        while(check){

            if(temp == source) break;

            edge = temp->backedge;

          	if(edge == NULL){
				check = false;
				break;
			}
			// flipping the edges
			rev = edge->reverse;
			edge->original = 0;
			edge->residual = 1;
			rev->original = 1;
			rev->residual = 0;
			temp = edge->from;
        } 
    }
	//check word is spelled
	for(int i = min_nodes; i < nodes.size() - 1; i++){
		for(auto &e: nodes[i]->adj){
			if(e->to->type == Node::Node_Type::SINK && e->residual == 1){
				num_letters++;
			}
			if(nodes[i]->type == Node::Node_Type::WORD && e->original == 1){
				spellingIds.push_back(e->to->id);
			}
		}
	}
	//if spellingIDs vector matches the amount of word nodes, then it can spell the word
	if(num_letters == num_word){
		return true;
	}
	return false;
}


void add_dice_to_graph(Graph &graph, string die, int id){
    Node *dice;
    Edge *edge;
    dice = new Node(id, Node::Node_Type::DICE, die);

    graph.nodes.push_back(dice);
    edge = new Edge(dice, graph.source);
    graph.source->adj.push_back(edge);
    dice->adj.push_back(edge->reverse);
}

void add_word_to_graph(Graph &graph, string word, int id){
    Node *node;
    Edge *edge;
    for(int i = 0; i < word.length(); i++){
        graph.nodes.push_back(new Node(id+i, Node::Node_Type::WORD, string(1, word[i])));
        for(int j = 1; j < graph.min_nodes;j++){
            for(int k = 0; k < node->letters.size();k++){
                if(graph.nodes[j]->letters[k]==node->letters[k]){
                    edge = new Edge(node, graph.nodes[j]);
                    graph.nodes[j]->adj.push_back(edge);
                    node->adj.push_back(edge->reverse);
                }
            }
        }

    }
}

void Graph::print_node_order(string word){
	for(int i = 0; i < spellingIds.size();i++){
		if(i == spellingIds.size() - 1){
			cout << spellingIds[i]-1 << ":";
		} else {
			cout << spellingIds[i]-1 << ",";
		}
	}
}





int main(int argc, char *argv[]){
    ifstream fin;
    string file1, file2;

    Graph *graph = new Graph();
	Node *sink;
	Edge *e;

	int i, before;
	int count = 1;
	bool spellable;

	file1 = argv[1];
	file2 = argv[2];


    fin.open(file1);

    string line;
    //cout << "here";

    int count = 0;
    while(getline(fin, line)){
        add_dice_to_graph(*graph, line, count++);
    }
    
    graph->min_nodes = graph->nodes.size();

	fin.close();
	fin.open(file2);
	// inserts the word nodes, the sink and all edges into the graph;
	// checks if the word is able to be spelled and prints
   	while(getline(fin, line)){
		add_word_to_graph(*graph, line, count);
		before = graph->nodes.size();

		sink = new Node(graph->nodes.size(), Node::Node_Type::SINK, "");

		graph->nodes.push_back(sink);

		for(i = graph->min_nodes;i < before;i++){
			e = new Edge(sink, graph->nodes[i]);
			graph->nodes[i]->adj.push_back(e);
			sink->adj.push_back(e->reverse);
		}

		if(graph->spell_word()){
			graph->print_node_order(line);		// prints the order of the current word
			cout << line << endl;
		} else {
			cout << "Cannot spell " << line << endl;
		}
		graph->delete_word_from_graph();








   }





    for(auto &node:graph->nodes){
        for(int i = 0; i < node->letters.size(); i++){
            if(node->letters[i]){
                char letter = i+'A';
                cout << (char)letter;
            }
        }
        cout << endl;
    }    
	fin.close();
	delete graph;
    return 0;
}
