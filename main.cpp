#include <iostream>
#include <fstream>
#include "LinkedStack.h"
#include "LinkedList.h"
#include <string>
using namespace std;


//The program will ask the user to enter the number of nodes and number of links. 
//It will then load the correct .txt file from the many .txt files provided by the lecturer

int main(){
	bool run = true;				//run determines if the end of file is reached
	int numberOfNodes = 0;			//numberOfNodes is the number of nodes chosen by the user
	int numberOfLinks = 0;			//numberOfLinks is the number of links chosen by the user
	int chosenFile = 0;				//chosenFile is the number of the file the user chooses
	char cNode1, cNode2;			//cNode1 and cNode2 are for storing two adjacent nodes which will be read from the .txt file. They are characters.
	int node1, node2;				//node1 and node2 are the nodes of cNode1 and cNode2 converted from character to integer
	bool allNodesTravelled = true;	//allNodesTravelled checks if all nodes has been travelled in the given network
	ifstream load;					//load is to read .txt files
	ofstream save;					//save is to save the connected networks into a new .txt file
	string network = "";		
	
	cout << "Please choose a file to check by choosing a number: " << endl;
	cout << "1) 5 Nodes 6 Links" << endl;
	cout << "2) 6 Nodes 7 Links" << endl;
	cout << "3) 6 Nodes 8 Links" << endl;
	cout << "4) 7 Nodes 6 Links" << endl;
	cout << "Chosen file -> ";
	cin >> chosenFile;
	
	while(!cin || (chosenFile < 1) || (chosenFile > 4)){
		cout << "Chosen file -> ";
		cin >> chosenFile;
	}
	
	if(chosenFile == 1){
		numberOfNodes = 5;
		numberOfLinks = 6;
		load.open("5node6link.txt");
		save.open("5node6linkConnected.txt");
	}
	else if(chosenFile == 2){
		numberOfNodes = 6;
		numberOfLinks = 7;
		load.open("6node7link.txt");
		save.open("6node7linkConnected.txt");
	}
	else if(chosenFile == 3){
		numberOfNodes = 6;
		numberOfLinks = 8;
		load.open("6node8link.txt");
		save.open("6node8linkConnected.txt");
	}
	else if(chosenFile == 4){
		numberOfNodes = 7;
		numberOfLinks = 6;
		load.open("7node6link.txt");
		save.open("7node6linkConnected.txt");
	}

	//Adjacency matrix of size numberOfNodes * numberOfNodes is declared
	//Ex: 5 number of nodes = 5*5 adjacency matrix
	int **adjMat = new int* [numberOfNodes];
	for(int i = 0; i < numberOfNodes; i++)
		adjMat[i] = new int [numberOfNodes];
	
	//Check if the node has been travelled already or not
	bool *travelled = new bool[numberOfNodes];
	
	//Initialise all values in the adjacency matrix to 0 and all values in travelled array to false (never travelled to this node before)
	for(int i = 0; i < numberOfNodes; i++){
		for(int j = 0; j < numberOfNodes; j++)
			adjMat[i][j] = 0;
		travelled[i] = false;
	}
	
	//Linked Stack to keep track of travel path. Always start from the first node which is node 1. In the array it is position 0. Thus, travelled[0] is
	//always true because we always start from it.
	LinkedStack<int> findPath;
	findPath.push(0);
	travelled[0] = true;
	
	//topNode = 0 because travel always start from the first node
	int topNode = 0;
	int nextNode;
	
	int tempFinder = 0;

	//The while loop will force the program to read data until the file ends (run = false)
	while (run) {
		
		//Every time the for loop runs once, all the linked nodes of one line of the .txt file, which is one network is loaded into the program

		for(int i = 0; i < numberOfLinks; i++){
			load >> cNode1;
			if( load.eof() ){
				//If the end of file is loaded, then exit loop
				cout << "End. Please check folder for new .txt file";
				run = false;
				break;
			} 
			load >> cNode2;
			
			//Array starts from 0. Convert nodes to their original value - 1. Example: 12 means node 1 and node 2 is linked.
			//In the adjacency matrix, [0][1] and [1][0] will be linked.
			node1 = cNode1 - '1';
			node2 = cNode2 - '1';
			network = network + cNode1 + cNode2 + " ";
			
			//Add the value to the adjacency matrix
			adjMat[node1][node2] = 1;
			adjMat[node2][node1] = 1;
		}
		
		
		//If run is true (not yet end of file), travel the adjacency matrix
		if(run){
			
			//Keep travelling the adjacency matrix until the starting city is popped out of the stack. This means that there are no more paths to be travelled
			//from the first node
			while(!findPath.isEmpty()){
				for(int finder = 0; finder < numberOfNodes; finder++){
					
					//If a new node is connected and is not travelled before, go to that node and push its number in the stack. We will travel from that node
					if(adjMat[topNode][finder] == 1 && travelled[finder] == false){
						nextNode = finder;
						travelled[finder] = true;
						findPath.push(nextNode);
						break;
					}
					tempFinder = finder;
				}
				
				//If all the paths for the particular node have been travelled and no path has been found, it must be popped from the stack
				if(tempFinder == numberOfNodes-1){
					findPath.pop();
				}
				
				//Go back to previous node after all routes for the current node has been travelled
				topNode = findPath.peek();
			}
			
			//After traversing the entire adjacency matrix, check if all nodes are travelled. If all nodes are travelled, all nodes are connected.
			if(findPath.isEmpty()){
				for(int i = 0; i < numberOfNodes; i++){
					if(travelled[i] == false){
						allNodesTravelled = false;
					}
				}
			}
			
			//If all nodes has been travelled, output the network configuration
			if(allNodesTravelled == true){
				cout << network << endl;
				save << network << endl;
			}
			
			//Initialise all variables to their original values after each network
			allNodesTravelled = true;
			for(int i = 0; i < numberOfNodes; i++){
				for(int j = 0; j < numberOfNodes; j++){
					adjMat[i][j] = 0;
				}
				travelled[i] = false;
			}
			
			findPath.push(0);
			travelled[0] = true;
			network = "";
		}
		
	}
	
	save.close();
	load.close();
	
	//Deletion of dynamic variables
	for(int i = 0; i < numberOfNodes; i++){
		delete []adjMat[i];
	}
	
	delete []adjMat;
	delete []travelled;
}