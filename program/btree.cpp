#include "btree.h"

btree::btree(int treeOrd){
	order = treeOrd;
	root = new node();
	root->parent = NULL;
}

bool btree::deleteNode(int num){

}

void btree::split(node *loc, int num, vector<struct node*> leftData, vector<struct node*> rightData){
	//Find median after adding new key to the mix
	bool inserted = false;
	for (vector<int>::iterator it = loc->keys.begin(); it != loc->keys.end(); it++){ 
		if ((*it) > num){
			loc->keys.insert(it, num);
			inserted = true;
			break;
		}
	}
	if (!inserted) loc->keys.push_back(num);
	int medianLoc = loc->keys.size()/2;
	int median = loc->keys[medianLoc];
	//Split based on median
	node *left = new node();
	node *right = new node();
	left->parent = loc;
	right->parent = loc;
	loc->pointers.push_back(left);
	loc->pointers.push_back(right);
	for (vector<int>::iterator it = loc->keys.begin();it != loc->keys.end();it++){
		if (*it < median){
			left->keys.push_back(*it);
		}else if(*it > median){
			right->keys.push_back(*it);
		}
	}
	loc->keys.clear();
	loc->keys.push_back(median);
	right->pointers = rightData;
	left->pointers = leftData;
	//Update the parent's of the children nodes
	for (vector<struct node*>::iterator it = right->pointers.begin();it!=right->pointers.end();it++) (*it)->parent = right;
	for (vector<struct node*>::iterator it = left->pointers.begin();it!=left->pointers.end();it++) (*it)->parent = left;

	//Push back up so higher nodes are filled
	if (loc->parent == NULL){
		node *temp = new node();
		temp->parent = NULL;
		root->parent = temp;
		root = temp;		
	}
	if (loc->parent->keys.size() < order){
		cout << "Median insert start"<<endl;
		bool inserted = false;
		for (vector<int>::iterator it = loc->parent->keys.begin();it!=loc->parent->keys.end();it++){
			if ((*it) > median){
				loc->parent->keys.insert(it, median);
				inserted  = true;
				break;
			}
		}
		if (!inserted) loc->parent->keys.push_back(median);
		left->parent = loc->parent;
		right->parent = loc->parent;
		inserted = false;
		cout << "Parent pointers insert start"<<endl;
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin(); it!=loc->parent->pointers.end();it++){
			if ((*it)->keys.size() > 0 && (*it)->keys[0] > left->keys[0]){
				it = loc->parent->pointers.insert(it, right);
				loc->parent->pointers.insert(it, left);
				inserted = true;
				break;
			}
		}
		if (!inserted){
			loc->parent->pointers.push_back(left);
			loc->parent->pointers.push_back(right);
		}
		cout << "end"<<endl;
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin(); it!=loc->parent->pointers.end();){  
			if ((*it)==loc){
				loc->parent->pointers.erase(it);
			}else{
				++it;
			}
		}
		delete loc;
		cout << "99 Problems"<<endl;
	}else{
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin(); it!=loc->parent->pointers.end();){  
			if ((*it)==loc){
				loc->parent->pointers.erase(it);
			}else{
				++it;
			}
		}
		if (loc->parent->keys[0] < loc->keys[0]){
			split(loc->parent, median, loc->parent->pointers, loc->pointers);
		}else{
			split(loc->parent, median, loc->pointers, loc->parent->pointers);
		}
	
		delete loc;
	}
}

bool btree::insert(int num){
	node *loc = traverse(root, num);
	//Nothing in tree
	if (loc->keys.size()==0){
		loc->keys.push_back(num);
		return true;
	//Leaf node is not full
	}else if (loc->keys.size() < order){
		cout << "split not required"<<endl;
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
		//for (vector<int>::iterator it = loc->keys.begin();it!= loc->keys.end();it++) cout << *it << " ";
		//cout <<endl;
		return true;
	//Leaf node is full
	}else{
		cout << "Split required"<<endl;
		vector<struct node*> temp;
		split(loc, num, temp, temp);
		return true;		
	}
}

node *btree::traverse(node *cur, int num){
	//If the node has no children
	if (cur->pointers.size() == 0) return cur; 
	int count = 0;
	for (vector<int>::iterator it = cur->keys.begin(); it != cur->keys.end(); it++){
		if (num < (*it)){
	
			//Search further into the tree
			if (cur->pointers[count]){
				return traverse(cur->pointers[count], num);
			}
		}else if (num == (*it)){
			//Key is in this node
			return cur;
		}
		count ++;
	}
	if (cur->pointers.size()>0){
		return traverse(cur->pointers.back(), num);
	}else{
		return cur;
	}
}

bool btree::select(int num){

}

string btree::jsonAux(node *cur){
	stringstream ss;
	ss << "{";
	ss << "\"keys\": [";
	int count = 0;
	if (cur->parent != NULL) cout << "Parent: "<<cur->parent->keys[0]<<", keys: ";
	for (vector<int>::iterator it = cur->keys.begin(); it!= cur->keys.end() && count <= order;it++){
		cout << (*it)<<" ";
		ss << "\"" << *it << "\"";
		count ++;
		if (*it!=cur->keys.back()) ss << ", ";
	}
	cout << endl;
	ss << "], \"children\": [";
	for (vector<struct node*>::iterator it = cur->pointers.begin(); it!=cur->pointers.end();it++){
		ss << jsonAux(*it);
		if (*it != cur->pointers.back()) ss << ", ";
	}
	ss << "]}";
	return ss.str();
}

string btree::getJson(){
	stringstream ss;
	ss << "[" << jsonAux(root) << "]";
	return ss.str();
} 

btree::~btree(){
	
}
