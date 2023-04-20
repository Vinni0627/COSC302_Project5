/*
Project 5

Inspired by Camille Crumpton's video explanation


*/



#include <iostream>
#include <string>
#include <vector>
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
        Edge(Node *to, Node *from); //constructor for edges
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
    this->visited = false;

    if (type == DICE || type == WORD) {
        this->letters.resize(26, false);
        //cout << word << endl;
        for (int i = 0; i < word.size();i++) {
            //cout << word[i] << endl;
            this->letters[word[i]-'A'] = true;
        }
    }
}

Edge::Edge(Node *to, Node *from){
    this->to = to;
    this->from = from;
    original = 1;
    residual = 0;
    reverse = nullptr;

}


Graph::Graph(){
    min_nodes = 0;
    source = new Node(0, Node::NodeType::SOURCE, "");
    sink = new Node(1, Node::NodeType::SINK, "");
}

Graph::~Graph(){
    for(auto &node:nodes){
        delete node;
    }
}

bool Graph::BFS(){
    return 0;
}

bool Graph::spell_word(){           //Edmonds-Karp algorithm
    Node *temp;
    Edge *edge;
    Edge *rev;
    int words = nodes.size() - min_nodes-1;

    while(true){
        temp = nodes[nodes.size()-1];

        while(BFS()){

            if(temp == source) break;

            edge = temp->backedge;

            if()

        } 
    }
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

int main(){
    //cout << "now here";
    ifstream Dice1;
    ifstream Words1;
    Dice1.open("Dice1.txt");
    Words1.open("Words1.txt");

    string line;
    Graph *graph = new Graph();
    //cout << "here";

    int count = 0;
    // while(getline(Dice1, line)){
    //     //cout << line << endl;
    //     graph->nodes.push_back(new Node(count, Node::Node_Type::DICE, line));
    // }
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "ENG"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "SAA"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "PRR"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "EAE"));

    graph->min_nodes = graph->nodes.size();
    graph->nodes.push_back(new Node(count++, Node::Node_Type::WORD, string(1, 'R')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::WORD, string(1, 'A')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::WORD, string(1, 'G')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::WORD, string(1, 'E')));

    graph->nodes.push_back(new Node(count++, Node::Node_Type::SINK, "sink"));

    for(int i = 1; i < graph->nodes.size();i++){                                        //source node edges
        graph->nodes[0]->adj.push_back(new Edge(graph->nodes[i], graph->nodes[0]));
        graph->nodes[0]->adj[i-1]->Reverser(Edge(graph->nodes[0], graph->nodes[i]));
    }

    for(auto &node: graph->nodes){

    }





    //cout << "1.";
    // getline(Words1, line);
    // cout << line;
    //     if(line != ""){
    //         for(int i = 0; i < line.size(); i++){
    //             graph->nodes.push_back(new Node(graph->min_nodes+i+1, Node::Node_Type::DICE, "" + line[i]));
    //         }
    //     }

    for(auto &node:graph->nodes){
        for(int i = 0; i < node->letters.size(); i++){
            //cout << "2.";
            if(node->letters[i]){
                char letter = i+'A';
                cout << (char)letter;
            }
        }
        cout << endl;
    }





    
    Dice1.close();
    Words1.close();
    return 0;
}