#include "btree.h"

btree::btree(int treeOrd){
	order = treeOrd;
}

bool btree::deleteNode(int num){

}

bool btree::insert(int num){

}

bool btree::select(int num){

}

string btree::getJson(){
	return json;
} 

btree::~btree(){
	delete root;
}
