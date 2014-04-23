#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <time.h> 
#include <cstdio>
#include <ctime>
#include "btree.h"
using namespace std;

btree *tree;

bool handleCommand(char *command, int len){
	char *cmd = strtok(command, " ");
	for(int i = 0; cmd[i]; i++){
		cmd[i] = tolower((int)cmd[i]);
	}
	string cmdStr (cmd);
	bool quit = false;
	bool success = false;
	if(cmdStr == "new"){
		char *num = strtok(NULL, " ");
		int node = atoi(num);
		if (tree != NULL) delete tree;
		tree = new btree(num);
		cout << "Tree created"<<endl;
	}else if(cmdStr == "run"){
		char *file = strtok(NULL, " ");
		ifstream in(file);
		char command[100];
		while(in.good()){
			in.getline(command, 100);
			handleCommand(command, 100);
		}
		in.close();
		cout << "End File"<<endl;
	}else if(cmdStr == "quit"){
		quit = true;
	}
	if (btree!=NULL){
		if (cmdStr == "insert"){
			char *num = strtok(NULL, " ");
			int node = atoi(num);
			success = tree->insert(node);
		}else if(cmdStr == "select"){
			char *num = strtok(NULL, " ");
			int node = atoi(num);
			success = tree->select(node);
		}else if(cmdStr == "delete"){
			char *num = strtok(NULL, " ");
			int node = atoi(num);
			success = tree->deleteNode(node);
		}else if(cmdStr == "getjson"){
			cout << tree->getJson()<<endl;
		}
		if (success){
			cout << "Command executed"<<endl;
		}
	}else{
		cout << "Please initiate b tree"<<endl;
	}
	return quit;
}

int main(int argc, char **argv){
	tree = NULL;
	bool timer = false;
	int endtime = 0;
	if (argc == 2){
		timer = true;
		int i = atoi(argv[1]);
		endtime=clock() + (i * CLOCKS_PER_SEC); 
	}
	bool quit = false;
	while (quit == false){
		cout << ">  ";
		char command[100];
		cin >> command;
		quit = handleCommand(command, 100);
		if (timer && clock() > endtime) quit = true;
	}
	if (tree != NULL) delete tree;
	return 0;
}
