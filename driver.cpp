#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
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
	if (cmdStr == "insert"){
		char *num = strtok(NULL, " ");
		int node = atoi(num);
		tree->insert(node);
	}else if(cmdStr == "select"){
		char *num = strtok(NULL, " ");
		int node = atoi(num);
		tree->select(node);
	}else if(cmdStr == "delete"){
		char *num = strtok(NULL, " ");
		int node = atoi(num);
		tree->deleteNode(node);
	}else if(cmdStr == "getjson"){
		tree->getJson();
	}else if(cmdStr == "new"){
		char *num = strtok(NULL, " ");
		int node = atoi(num);
		delete tree;
		//tree = new btree(num);
	}else if(cmdStr == "quit"){
		quit = true;
	}
	return quit;
}

int main(int argc, char **argv){
	if (argc == 2){
		int order = atoi(argv[1]);
		tree = new btree(order);
	}else if(argc == 3){
		int order = atoi(argv[1]);
		tree = new btree(order);
		ifstream in(argv[2]);
		char command[100];
		while(in.good()){
			in.getline(command, 100);
			handleCommand(command, 100);
		}
		in.close();
	}else{
		cout << "Error, please specify order of tree"<<endl;
		return 0;
	}
	bool quit = false;
	while (quit == false){
		cout << ">  ";
		char command[100];
		cin >> command;
		quit = handleCommand(command, 100);
	}
	delete tree;
	return 0;
}
