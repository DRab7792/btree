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
		node *root;
		string jsonAux(node *cur);
		void split(node *loc, int num, vector<struct node*> data);
		void unMarkNodes(node *cur);
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
