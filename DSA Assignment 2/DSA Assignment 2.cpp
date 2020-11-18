// DSA Assignment 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include<cstdlib>
#include<string>
#include<iomanip>
#include<fstream>
#include<queue>

#include "AvlTree.h"
using namespace std;

int main()
{
	//calling the AVLTreeMenu function within the AVLTree Header
	AvlTree<WORD, string> myTree;
	//Running the AVLMenu Function
	myTree.AVL_Menu();
	//return 0 since the main is a int
	return 0;
}
