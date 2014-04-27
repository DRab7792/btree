#include "btree.h"

btree::btree(int treeOrd){
	order = treeOrd;
	root = new node();
	root->parent = NULL;
	unMarkNodes(root);
}

void btree::unMarkNodes(node *cur){
	cur->selected = false;
	for (vector<struct node*>::iterator it=cur->pointers.begin(); it!=cur->pointers.end(); it++){
		return unMarkNodes(*it);
	}
}

bool btree::deleteNode(int num){

}

void btree::split(node *loc, int num, vector<struct node*> data){
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
	//Add the children back, assign them to their parents in the proper order
	if (data.size()>0){
		node *parent;
		for (vector<struct node*>::iterator it = data.begin(); it!= data.end();it++){
			if ((*it)->keys[0] > median){ 
				parent = right;
			}else{
				parent = left;
			}
			(*it)->parent = parent;
			if (parent->pointers.size()==0){
				parent->pointers.push_back(*it);
			}else{
				bool inserted = false;
				for (vector<struct node*>::iterator i=parent->pointers.begin();i!=parent->pointers.end();i++){
					if ((*i)->keys[0] > (*it)->keys[0]){
						parent->pointers.insert(i, *it);
						inserted = true;
						break;
					}
				}
				if (!inserted) parent->pointers.push_back(*it);
			}
		}
	}
	//Push back up so higher nodes are filled
	if (loc->parent == NULL){
		node *temp = new node();
		temp->parent = NULL;
		root->parent = temp;
		root = temp;		
	}
	if (loc->parent->keys.size() < order){
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
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin(); it!=loc->parent->pointers.end();){  
			if ((*it)==loc){
				loc->parent->pointers.erase(it);
			}else{
				++it;
			}
		}
		delete loc;
	}else{
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin(); it!=loc->parent->pointers.end();){  
			if ((*it)==loc){
				loc->parent->pointers.erase(it);
			}else{
				++it;
			}
		}
		vector<struct node*> children;
		for (vector<struct node*>::iterator it = loc->parent->pointers.begin();it!=loc->parent->pointers.end();it++) children.push_back(*it);
		for (vector<struct node*>::iterator it = loc->pointers.begin();it!=loc->pointers.end();it++) children.push_back(*it);
		split(loc->parent, median, children);
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
		split(loc, num, temp);
		return true;		
	}
	unMarkNodes(root);
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
	int reads = 1;
	bool found = false;
	node *cur = root;
	while (!found || cur->pointers.size()==0){
		cur->selected = true;
		int back = cur->keys.back();
		int front  = cur->keys.front();
		if (back >= num && front <= num){
			int last = 0;
			for (vector<int>::iterator it=cur->keys.begin(); it!= cur->keys.end(); it++){
					
				if (*it == num){
					found = true;
					break;
				}
				last = (*it);
			}
		}else if (front > num){
			reads ++;
			cur = cur->pointers.front();
		}else if (back < num){
			reads ++;
			cur = cur->pointers.back();
		}
	}
	cout << "Physical Reads: "<<reads<<endl;
	return found;
}

string btree::jsonAux(node *cur){
	stringstream ss;
	ss << "{";
	ss << "\"keys\": [";
	int count = 0;
	for (vector<int>::iterator it = cur->keys.begin(); it!= cur->keys.end() && count <= order;it++){
		ss << "\"" << *it << "\"";
		count ++;
		if (*it!=cur->keys.back()) ss << ", ";
	}
	ss << "],";
	if (cur->selected){
		ss << "\"selected\":\"true\"";
	}else{
		ss << "\"selected\":\"false\"";
	} 
	ss << ", \"children\": [";
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
