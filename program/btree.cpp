#include "btree.h"

btree::btree(int treeOrd){
	order = treeOrd;
	root = new node();
}

bool btree::deleteNode(int num){

}

void btree::split(node *loc, int num){
	//Find median
	for (vector<int>::iterator it = loc->keys.begin(); it != loc->keys.end(); it++){ 
		if ((*it) > num){
			loc->keys.insert(it, num);
			break;
		}
	}
	int medianLoc = loc->keys.size()/2;
	int median = loc->keys[medianLoc];
	//Split based on median
	node *left = new node();
	node *right = new node();
	for (vector<int>::iterator it = loc->keys.begin();it != loc->keys.end();it++){
		if (*it < median){
			left->keys.push_back(*it);
		}else if(*it > median){
			right->keys.push_back(*it);
		}
	}
	loc->pointers.push_back(left);
	loc->pointers.push_back(right);
	loc->keys.clear();
	loc->keys.push_back(median);
	display();
}

void btree::display(){
}

bool btree::insert(int num){
	node *loc = traverse(root, num);
	//Nothing in tree
	if (loc->keys.size()==0){
		cout << "Node is empty procedure"<<endl;
		loc->keys.push_back(num);
		return true;
	//Leaf node is not full
	}else if (loc->keys.size() < order){
		cout << "Node is not full procedure"<<endl;
		bool inserted = false;
		for (vector<int>::iterator it = loc->keys.begin(); it != loc->keys.end(); it++){ 
			if ((*it) > num){
				inserted = true;
				loc->keys.insert(it, num);
				break;
			}else if ((*it) == num){
				cout << "Already in tree"<<endl;
				return false;
			}
		}
		if (!inserted) loc->keys.push_back(num);
		for (vector<int>::iterator it = loc->keys.begin();it!= loc->keys.end();it++) cout << *it << " ";
		cout <<endl;
		return true;
	//Leaf node is full
	}else{
		cout << "Node is full procedure"<<endl;
		split(loc, num);
		return true;		
	}
}

node *btree::traverse(node *cur, int num){
	//If the node has no children
	if (cur->pointers.size() == 0) return cur; 
	int count = 0;
	for (vector<int>::iterator it = cur->keys.begin(); it != cur->keys.end(); it++){
		if (num < *it){
			//Search further into the tree
			return traverse(cur->pointers[count], num);
		}else if (num == (*it)){
			//Key is in this node
			return cur;
		}
		count ++;
	}
}

bool btree::select(int num){

}

string btree::getJson(){
	return json;
} 

btree::~btree(){
	
}
