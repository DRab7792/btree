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

void batch(ostream &out, string argStr);

bool handleCommand(ostream &out, char *command, int len){
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
		out << "Tree created";
	}else if(cmdStr == "run"){
		batch(out, argStr);
	}else if(cmdStr == "quit"){
		if (tree) delete tree;
		out << "Goodbye"<<endl;
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
			out << "Command executed";
		}
	}else if(tree == NULL && (cmdStr=="insert" || cmdStr=="select" || cmdStr=="delete" || cmdStr=="getjson")){
		out << "Please initiate b tree";
	}else{
		out << "Invalid command";
	}
	out <<endl<<"> ";
	return false;
}

void batch(ostream &out, string argStr){
	ifstream in(argStr.c_str());
	if (in.good()){
		char curCmd[100];
		while(in.good()){
			in.getline(curCmd, 100);
			cout << curCmd<<endl;
			if (strcmp(curCmd, "\n") > 0) handleCommand(out, curCmd, 100);
		}
		out << "End File";
	}else{
		out << "File not found";
	}
	in.close();
}

int main(int argc, char **argv){
	tree = NULL;
	bool timer = false;
	time_t cur;
	time_t end;
	if (argc > 1){
		string file(argv[1]);
		if (argc==3){
			ofstream out(argv[2]);
			batch(cout, file);
			out << tree->getJson() <<endl;
			out.close();
		}else batch(cout, file);	
	}else{
		bool quit = false;
		cout << "> ";
		while (quit == false){
			char command[100];
			cin.getline(command, 100);
			quit = handleCommand(cout, command, 100);
		}
	}
	return 0;
}
