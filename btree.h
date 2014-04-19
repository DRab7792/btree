#include <ctype.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct node{
	vector<int> limits;
	vector<struct node*> pointers;
};

class btree{
	private:
		int order;
		node *root;
	public:
		btree(int treeOrd);
		~btree();
		string getJson();
		void insert(int num);
		bool deleteNode(int num);
		void select(int num);
};
