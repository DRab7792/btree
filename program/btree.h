#include <ctype.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

struct node{
	vector<int> keys; //All the keys of this node in order
	vector<struct node*> pointers; //All the pointers of this node in order
	node *parent; //Pointer to the parent, if root then parent is NULL
	bool selected; //Marks this node as selected if the last command was a select command
};

class btree{
	private:
		int order; //Order of the tree
		int boundary; //Underflow Boundary
		node *root; //Root node
		string jsonAux(node *cur);
		void split(node *loc, int num, vector<struct node*> data);
		void unMarkNodes(node *cur);
		void adjust(node *loc);
		bool vergeUnder(node *cur);
		bool isUnderflowing(node *cur);
		void concat(node *parent, node *left, node*right);
		void RLDistribute(node *parent, node *left, node *right);
		void LRDistribute(node *parent, node *left, node *right);
		node *traverse(node *cur, int num);
	public:
		btree(int treeOrd);
		~btree();
		string getJson();
		void display();
		bool insert(int num);
		bool deleteNode(int num);
		bool select(int num);
};
