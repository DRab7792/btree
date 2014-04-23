#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <time.h> 
#include <cstring>
#include <cstdio>
#include <ctime>
#include "btree.h"
using namespace std;

btree *tree;

bool handleCommand(char *command, int len){
	string cmd (command);
	int delim = cmd.find(" ");
	string argStr, cmdStr;
	int arg = 2;
	if (delim != -1){
		argStr = cmd.substr((delim+1));
		char *cArgStr = new char [argStr.length()];
		strcpy(cArgStr, argStr.c_str());
		arg = atoi(cArgStr);
		delete cArgStr;
		cmdStr = cmd.substr(0, delim);
	}else{
		cmdStr = cmd;
	}
	bool success = false;
	if(cmdStr == "new"){
		if (tree != NULL) delete tree;
		tree = new btree(arg);
		cout << "Tree created";
	}else if(cmdStr == "run"){
		ifstream in(argStr.c_str());
		if (in.good()){
			char curCmd[100];
			while(in.good()){
				in.getline(curCmd, 100);
				cout << curCmd<<endl;
				if (strcmp(curCmd, "\n") > 0) handleCommand(curCmd, 100);
			}
			cout << "End File";
		}else{
			cout << "File not found";
		}
		in.close();
	}else if(cmdStr == "quit"){
		if (tree) delete tree;
		cout << "Goodbye"<<endl;
		return true;
	}else if (tree!=NULL){
		if (cmdStr == "insert"){
			success = tree->insert(arg);
		}else if(cmdStr == "select"){
			success = tree->select(arg);
		}else if(cmdStr == "delete"){
			success = tree->deleteNode(arg);
		}else if(cmdStr == "getjson"){
			cout << tree->getJson()<<endl;
		}
		if (success){
			cout << "Command executed";
		}
	}else if(tree == NULL && (cmdStr=="insert" || cmdStr=="select" || cmdStr=="delete" || cmdStr=="getjson")){
		cout << "Please initiate b tree";
	}else{
		cout << "Invalid command";
	}
	cout <<endl<<"> ";
	return false;
}

int main(int argc, char **argv){
	tree = NULL;
	bool timer = false;
	time_t cur;
	time_t end;
	if (argc == 2){
		timer = true;
		int i = atoi(argv[1]);
		time(&end);
		end += i;
	}
	bool quit = false;
	cout << "> ";
	while (quit == false){
		if (timer){
			time(&cur);
			if (cur > end){
				cout << "Sorry time is up"<<endl;
				break;
			}
		}
		char command[100];
		cin.getline(command, 100);
		quit = handleCommand(command, 100);
	}
	return 0;
}
