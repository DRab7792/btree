#include <ctype.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

struct node{
	vector<int> keys;
	vector<struct node*> pointers;
	node *parent;
	bool selected;
};

class btree{
	private:
		int order;
		int boundary;
		node *root;
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
