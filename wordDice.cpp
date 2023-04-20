/*
Project 5

Inspired by Camille Crumpton's video explanation


*/



#include <iostream>
#include <string>
#include <vector>

using namespace std;


typedef enum{SOURCE, SINK, DICE, WORD} Node_Type;

class Node {
    public:
        int id;
        Node_Type type;
        vector<bool> letters;
        bool visited;
        vector<Edge*> edges;

        Node(int id, Node_Type type, string letters);
};


class Edge {
    public:
        Node *to;
        Node *from;
        Edge *reverse;
        int original;
        int residual;

        Edge(Node &to_, Node &from_);


};



class Graph {
    public:
        vector<Node*> nodes;
        int BFS();
        int canISpell();
        vector<int> spellingIDs;        //for printing
        int minNodes;

        Graph();
        void DeleteHalfGraph();


};



Node::Node(int id_, Node_Type type_, string letters_){
    id = id_;

    type = type_;

    for(int i = 0; i < letters_.size();i++){
        letters[letters_[i]] = true;
    }


}

Edge::Edge(Node &to_, Node &from_){
    if(to_.id > from_.id && to_.type != from_.type){
        original = 1;
        residual = 0;
    } else {
        original = 0;
        residual = 1;
    }

}

