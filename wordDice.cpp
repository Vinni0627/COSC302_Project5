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
        friend ostream& operator<<(ostream& os, const Node& node); // PROBABLY NOT NEEDED
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
        void Reverser(Edge &reverse_edge);
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
        void add_dice_to_graph(string die, int id); //add dice nodes to graph
        void add_word_to_graph(string word, int& id); //add word (letter) nodes to graph
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
    reverse = NULL;

}
void Edge::Reverser(Edge &reverse_edge){
    this->reverse = &reverse_edge;
    reverse_edge.reverse = this;
    reverse_edge.original = 0;
    reverse_edge.residual = 1;
}   

Graph::Graph(){
    source = new Node(0, Node::SOURCE, "source");
    nodes.push_back(source);
    min_nodes = 0;
}

bool Graph::BFS(){
    return 0;
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

    int count = 1;
    // while(getline(Dice1, line)){
    //     //cout << line << endl;
    //     graph->nodes.push_back(new Node(count, Node::Node_Type::DICE, line));
    // }
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "ENG"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "SAA"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "PRR"));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, "EAE"));

    graph->min_nodes = graph->nodes.size();
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, string(1, 'R')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, string(1, 'A')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, string(1, 'G')));
    graph->nodes.push_back(new Node(count++, Node::Node_Type::DICE, string(1, 'E')));

    graph->nodes.push_back(new Node(count++, Node::Node_Type::SINK, "sink"));



    


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