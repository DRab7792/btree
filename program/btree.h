#include <ctype.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct node{
	vector<int> keys;
	vector<struct node*> pointers;
};

class btree{
	private:
		int order;
		node *root;
		string json;
		void split(node *loc, int num);
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
