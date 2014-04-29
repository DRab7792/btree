#include "btree.h"

btree::btree(int treeOrd){
	order = treeOrd--;
	root = new node();
	root->parent = NULL;
	unMarkNodes(root);
	boundary = ((order+1)/2) -1;
}

void btree::unMarkNodes(node *cur){
	cur->selected = false;
	for (vector<struct node*>::iterator it=cur->pointers.begin(); it!=cur->pointers.end(); it++){
		unMarkNodes(*it);
	}
}

bool btree::isUnderflowing(node *cur) {return cur->keys.size() < boundary;}
bool btree::vergeUnder(node *cur) {return cur->keys.size()== boundary;}

void btree::concat(node *parent, node *left, node *right){
	if (right->pointers.size() > 0)left->pointers.insert(left->pointers.end(), right->pointers.begin(), right->pointers.end());
	vector<int>::iterator key = parent->keys.begin();
	vector<struct node*>::iterator it;
	for (it = parent->pointers.begin(); it!= parent->pointers.end(); it++){
		if (*it==left){
			break;
		}
		key++;
	}
	if (it!= parent->pointers.end() && (it+1)!= parent->pointers.end()){
		it ++;
		parent->pointers.erase(it);
		int temp = *key;
		parent->keys.erase(key);
		left->keys.push_back(temp);
		if (right->keys.size()>0) left->keys.insert(left->keys.end(), right->keys.begin(), right->keys.end());
	}
}

void btree::RLDistribute(node *parent, node *left, node *right){
	vector<int>::iterator key = parent->keys.begin();
	for (vector<struct node*>::iterator it = parent->pointers.begin(); it!= parent->pointers.end(); it++){
		if (*it==left) break;
		key++;
	}
	//Move keys
	int parentKey = *key;
	left->keys.push_back(parentKey);
	int rightKey = right->keys.front();
	right->keys.erase(right->keys.begin());
	parent->keys.insert(key, rightKey);
	for (vector<int>::iterator it= parent->keys.begin(); it!=parent->keys.end(); it++){
		if (*it == parentKey){
			parent->keys.erase(it);
			break;
		}
	}
	if (right->pointers.size()>0){
		node *temp = right->pointers.front();
		left->pointers.push_back(temp);
		right->pointers.erase(right->pointers.begin());
	}
}

void btree::LRDistribute(node *parent, node *left, node *right){
	vector<int>::iterator key = parent->keys.begin();
	for (vector<struct node*>::iterator it = parent->pointers.begin(); it!= parent->pointers.end(); it++){
		if (*it==left) break;
		key++;
	}
	//Move keys
	int parentKey = *key;
	right->keys.insert(right->keys.begin(), parentKey);
	int leftKey = left->keys.back();
	left->keys.pop_back();
	parent->keys.insert(key, leftKey);
	for (vector<int>::iterator it= parent->keys.begin(); it!=parent->keys.end(); it++){
		if (*it == parentKey){
			parent->keys.erase(it);
			break;
		}
	}
	if (left->pointers.size()>0){
		node *temp = left->pointers.back();
		right->pointers.insert(right->pointers.begin(), temp);
		left->pointers.pop_back();
	}
}

void btree::adjust(node *loc){
	bool underflowing  = isUnderflowing(loc);
	node *rightSib = NULL;
	node *parent = loc->parent;
	node *leftSib = NULL;
	//Get left and Right siblings
	if (parent!= NULL){
		for (vector<struct node*>::iterator it = parent->pointers.begin(); it!=parent->pointers.end(); it ++){
			if (*it==loc){
				if (it != parent->pointers.begin()){
					vector<struct node*>::iterator left = it;
					left --;
					leftSib = *left;
				}
				if (it!=parent->pointers.end()){
					vector<struct node*>::iterator right = it;
					right ++;
					if (right!= parent->pointers.end()) rightSib = *right; 
				}
				break;
			}
		}
	}
	//Begin adjusting
	if (underflowing){
		if (loc == root){
			if (loc->pointers.size()==1){
				root = loc->pointers[0];
				loc->pointers[0]->parent = NULL;
				delete loc;
			}
		}else if (leftSib != NULL && !vergeUnder(leftSib)){
			LRDistribute(parent, leftSib, loc);	
		}else if (rightSib != NULL && !vergeUnder(rightSib)){
			RLDistribute(parent, loc, rightSib);
		}else{
			if (rightSib != NULL){
				concat(parent, loc, rightSib);
			}else{
				concat(parent, leftSib, loc);
			}
		}
	}else{
	}
}

bool btree::deleteNode(int num){
	unMarkNodes(root);
	node *loc = traverse(root, num);
	bool exists = false;
	bool hasKids = loc->pointers.size()!=0;
	vector<int>::iterator curKey;
	vector<struct node*>::iterator left, right;
	if (hasKids){
		left = loc->pointers.begin();
		right = loc->pointers.begin();
		right++;
	}
	for (vector<int>::iterator it=loc->keys.begin(); it!=loc->keys.end(); it++){
		if (*it == num){ 
			exists = true;
			curKey = it;
			break;
		}
		if (hasKids){
			left ++;
			right ++;
		}
	}
	if (!exists){
		cout << "Element is not in tree";
		return false;
	}
	//If the element does exist begin deletion
	if (!hasKids){
		//Erase leaf and adjust
		loc->keys.erase(curKey);
		adjust(loc);
		return true;
	}else{
		//Find the leftmost leaf in the right subtree
		node *leftLeaf = *right;
		while (leftLeaf->pointers.size()!=0){
			leftLeaf = leftLeaf->pointers[0];
		}	
		int temp = leftLeaf->keys[0];
		leftLeaf->keys.erase(leftLeaf->keys.begin());
		loc->keys.insert(curKey, temp);
		for (vector<int>::iterator it = loc->keys.begin(); it!=loc->keys.end(); it++){
			if (*it == num){
				loc->keys.erase(it);
				break;
			}
		}
		adjust(leftLeaf);
		return true;
	}
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
		unMarkNodes(root);
		return true;
	//Leaf node is not full
	}else if (loc->keys.size() < order){
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
		unMarkNodes(root);
		return true;
	//Leaf node is full
	}else{
		vector<struct node*> temp;
		split(loc, num, temp);
		unMarkNodes(root);
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
	unMarkNodes(root);
	int reads = 1;
	bool found = false;
	node *cur = root;
	while (!found && cur!= NULL){
		cur->selected = true;
		int back = cur->keys.back();
		int front  = cur->keys.front();
		bool hasKids = cur->pointers.size()!=0;
		if (back >= num && front <= num){
			int last = 0;
			int count = 0;
			for (vector<int>::iterator it=cur->keys.begin(); it!= cur->keys.end(); it++){
				if (*it == num){
					found = true;
					break;
				}else if (num > last && num < *it && hasKids){
					reads ++;
					cur = cur->pointers[count];
					break;
				}else if (num > last && num < *it){
					cur = NULL;
					break;
				}
				last = (*it);
				count ++;
			}
		}else if (front > num && hasKids){
			reads ++;
			cur = cur->pointers.front();
		}else if (back < num && hasKids){
			reads ++;
			cur = cur->pointers.back();
		}else{
			cur = NULL;
		}
	}
	if (found){
		cout << "Physical Reads: "<<reads<<endl;
	}else{
		cout << "Element not found";
		unMarkNodes(root);
	}
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
