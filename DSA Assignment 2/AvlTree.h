#ifndef AVLTREE_H
#define AVLTREE_H

#include "pch.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <queue>
using namespace std;

//	==================== MACROS ====================
#define LH +1     // Left High
#define EH  0     // Even High
#define RH -1     // Right High

// 	WORD Definitions
struct WORD {
	string key;
	int frequency;
	WORD() : key(""), frequency(0) {}
	bool operator <(const WORD& secondOperand) const {
		return (frequency < secondOperand.frequency);
	}
};

void print(WORD input) {
	cout << input.key << "<" << input.frequency << ">" << endl;
}


// 	NODE Definitions
template <class TYPE>
struct NODE
{
	TYPE    data;
	NODE   *left;
	NODE   *right;
	int     bal;
}; // NODE

// Class Declaration
template <class TYPE, class KTYPE>
class AvlTree
{
private:
	int          count;
	NODE<TYPE>  *tree;

	NODE<TYPE> *_insert(NODE<TYPE>  *root,
		NODE<TYPE>  *newPtr,
		bool&       taller);

	NODE<TYPE>  *leftBalance(NODE<TYPE>  *root,
		bool&        taller);

	NODE<TYPE>  *rotateLeft(NODE<TYPE>  *root);
	NODE<TYPE>  *rightBalance(NODE<TYPE>  *root,
		bool&        taller);
	NODE<TYPE>  *rotateRight(NODE<TYPE>  *root);
	NODE<TYPE> *_delete(NODE<TYPE>  *root,
		KTYPE        dltKey,
		bool&        shorter,
		bool&        success);

	NODE<TYPE>  *dltLeftBalance(NODE<TYPE>  *root,
		bool&        smaller);
	NODE<TYPE>  *dltRightBalance(NODE<TYPE>  *root,
		bool&        shorter);
	NODE<TYPE> *_retrieve(KTYPE        key,
		NODE<TYPE>  *root);

	void  _traversal(void(*process)(TYPE dataProc),
		NODE<TYPE>    *root);
	void  _traversal_preorder(void(*process)(TYPE dataProc),
		NODE<TYPE>    *root);
	void  _traversal_postorder(void(*process)(TYPE dataProc),
		NODE<TYPE>    *root);
	void  _destroyAVL(NODE<TYPE>  *root);

	//  	The following function is used for debugging.
	void  _print(NODE<TYPE> *root, int   level);

public:
	AvlTree(void);
	~AvlTree(void);
	bool  AVL_Insert(TYPE   dataIn);
	bool  AVL_Delete(KTYPE  dltKey);
	bool  AVL_Retrieve(KTYPE  key, TYPE& dataOut);
	void  AVL_Traverse(void(*process)(TYPE  dataProc)); //in-order
	void  AVL_Traverse_preorder(void(*process)(TYPE  dataProc)); //in-order
	void  AVL_Traverse_postorder(void(*process)(TYPE  dataProc)); //in-order
	bool  AVL_Empty(void);
	bool  AVL_Full(void);
	int   AVL_Count(void);
	/**student defined functions*/
	void  AVL_Menu();
	void  AVL_ReadFile();
	void  AVL_WriteFile();
	void  AVL_Search(void(*process)(TYPE  dataProc));

	bool AVL_CheckPresent(KTYPE key);
	NODE<TYPE>* AVL_ModifyNode(KTYPE key);

	void  AVL_Traverse_preorder(queue<KTYPE>& q); //overloaded
	void  _traversal_preorder(queue<KTYPE>& q, NODE<TYPE> *root); //overloaded

	void  AVL_Traverse(priority_queue<TYPE>& pq, KTYPE input); //overloaded
	void  _traversal(priority_queue<TYPE>& pq, KTYPE input, NODE<TYPE> *root); //overloaded



//      The following function is used for debugging.
	void  AVL_Print(void);
}; // class AvlTree

/*	=================== Constructor ===================
	Initializes private data.
	   Pre     class is being defined
	   Post    private data initialized
*/

template <class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE> ::AvlTree(void)
{
	//	Statements
	tree = NULL;
	count = 0;
}	//  Constructor

/**
---------------------STUDENT DEFINED FUNCTIONS------------------------
**/

/*	==================== AVL_Menu ====================
	This function draws the main menu of the program
*/

template <class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE> ::AVL_Menu()
{
	int input = 0;
	while (input != 6) {

		/**check if the tree is empty for the print and output functions to prevent errors**/
		cout << "\n---------------------------------------" << endl;
		cout << "Choose one of the following functions: " << endl;
		cout << "1. Load Dictionary from a file." << endl;
		cout << "2. Output dictionary to a file." << endl;
		cout << "3. Print dictionary." << endl;
		cout << "4. Print the AVL tree of the dictionary." << endl;
		cout << "5. Input a word with a guess." << endl;
		cout << "6. Exit program." << endl;
		cout << "---------------------------------------\n" << endl;

		cin >> input;
		switch (input) {
		case 1:
			AVL_ReadFile();
			break;
		case 2:
			AVL_WriteFile();
			break;
		case 3:
			if (AVL_Empty()) { cout << "\nPlease ensure the tree has input, preceeding printing." << endl; }
			else {
				AVL_Traverse(print);
				cout << "\nThe total number of words in the tree are: " << count << endl;
			}
			break;
		case 4:
			if (AVL_Empty()) { cout << "\nPlease ensure the tree has input, preceeding printing balance." << endl; }
			else { AVL_Print(); }
			break;
		case 5:
			if (AVL_Empty()) { cout << "\nPlease ensure the tree has input, preceeding search." << endl; }
			else { AVL_Search(print); }
			break;
		default:
			break;
		}
	}

}

/*	==================== AVL_Readfile ====================
	This function reads words from a file and stores them in the AVL tree
*/
template <class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE> ::AVL_ReadFile()
{
	string fileName;
	string inputWord = "";
	NODE<TYPE>* inputNode;

	cout << "Please enter the name of the document <e.g. dictionary.txt>:" << endl;
	cin >> fileName;

	ifstream dictionary(fileName.c_str());


	if (!dictionary) {
		cout << "The document specified is invalid. Terminating program." << endl;
		exit(1);
	}
	else {
		while ((dictionary >> inputWord) && AVL_Full() == false) {

			if (AVL_CheckPresent(inputWord)) {
				inputNode = AVL_ModifyNode(inputWord);
				inputNode->data.frequency += 1;
			}
			else {
				WORD input;
				input.key = inputWord;
				input.frequency = 1;
				AVL_Insert(input);
			}
		}
		cout << "The dictionary has been loaded successfully.\n" << endl;
	}
}

/*  ==================== AVL_WriteFile ====================
	This function prints the AVL tree to a file
*/

template <class TYPE, class KTYPE>
void AvlTree<TYPE, KTYPE> ::AVL_WriteFile()
{
	string fileName;
	queue<KTYPE> treeQueue;


	cout << "Please enter the name of the document <e.g. dictionary.txt>:" << endl;
	cin >> fileName;

	ofstream dictionary(fileName.c_str(), ios::trunc);

	if (!dictionary) {
		cout << "The document specified is invalid. Terminating program." << endl;
		exit(1);
	}
	else {
		if (AVL_Empty()) {
			cout << "Error! The tree is empty, please enter information into the tree\n";
			cout << "before attempting to write the tree to a file." << endl;
			return;
		}
		else {
			AVL_Traverse_preorder(treeQueue);
			while (!treeQueue.empty()) {
				dictionary << treeQueue.front() << endl;
				treeQueue.pop();
			}
		}
	}

}

/*	==================== AVL_CheckPresent ====================
	This function checks if a word already exists within the tree.
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE>
::AVL_CheckPresent(KTYPE   key)
{
	//	Local Definitions
	NODE<TYPE> *node;

	//	Statements
	if (!tree)
		return false;

	node = _retrieve(key, tree);
	if (node)
	{
		return true;
	} // if found
	else
		return false;
}

/*	==================== AVL_ModifyNode ====================
	If a word is already present within the tree, and you want to modify it either
	to increment its frequency or change its word.
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::AVL_ModifyNode(KTYPE   key)
{
	//	Local Definitions
	NODE<TYPE> *node;

	//	Statements
	node = _retrieve(key, tree);
	return node; //since I already checked if it was present before.
}

/*	==================== AVL_Search ====================
	searches for a word within the tree and returns any match
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Search(void(*process)(TYPE  dataProc))
{
	KTYPE input;
	priority_queue<TYPE> pq;

	cout << "Enter a guess: " << endl;
	cin >> input;

	AVL_Traverse(pq, input);

	if (!pq.empty()) {
		cout << endl;
		while (!pq.empty()) {
			process(pq.top());
			pq.pop();
		}

	}
	else {
		cout << "\nNo matches to word specified." << endl;
	}

}

/*	==================== AVL_Traverse overload ====================
	searches for a word within the tree and returns any match
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Traverse(priority_queue<TYPE>& pq, KTYPE input)
{
	//	Statements
	_traversal(pq, input, tree);
	return;
}

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_traversal(priority_queue<TYPE>& pq, KTYPE input,
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		_traversal(pq, input, root->left);
		if ((root->data.key.find(input) != string::npos) && (root->data.key.at(0) == input.at(0))) {
			pq.push(root->data);
		}
		_traversal(pq, input, root->right);
	} //  if
	return;
}	//  _traversal





/*	==================== AVL_Traverse_preorder && _traversal_preorder ====================
	This function stores each word in the tree within a queue before it is writtent to a file
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Traverse_preorder(queue<KTYPE>& q)
{
	//	Statements
	_traversal_preorder(q, tree);
	return;
}

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_traversal_preorder(queue<KTYPE>& q,
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		for (int i = 0; i < root->data.frequency; i++) {
			q.push(root->data.key);
		}
		_traversal_preorder(q, root->left);
		_traversal_preorder(q, root->right);
	} //  if
	return;
}


/**
---------------------END OF STUDENT DEFINED FUNCTIONS------------------------
**/



/*	==================== AVL_Insert ====================
	This function inserts new data into the tree.
	   Pre    dataIn contains data to be inserted
	   Post   data have been inserted or memory overflow
	   Return success (true) or overflow (false)
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::AVL_Insert(TYPE dataIn)
{
	//	Local Definitions
	NODE<TYPE>  *newPtr;
	bool         taller;

	//	Statements
	if (!(newPtr = new NODE<TYPE>))
		return false;
	newPtr->bal = EH;
	newPtr->right = NULL;
	newPtr->left = NULL;
	newPtr->data = dataIn;

	tree = _insert(tree, newPtr, taller);
	count++;
	return true;
}	//  Avl_Insert

/*	======================= _insert =======================
	This function uses recursion to insert the new data into
	a leaf node in the AVL tree.
	   Pre    application has called AVL_Insert, which passes
			  root and data pointers
	   Post   data have been inserted
	   Return pointer to [potentially] new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::_insert(NODE<TYPE>  *root,
	NODE<TYPE>  *newPtr,
	bool&        taller)
{
	//	Statements
	if (!root)
	{
		root = newPtr;
		taller = true;
		return  root;
	} //  if NULL tree

	if (newPtr->data.key < root->data.key)
	{
		root->left = _insert(root->left,
			newPtr,
			taller);
		if (taller)
			//  Left subtree is taller
			switch (root->bal)
			{
			case LH: // Was left high--rotate
				root = leftBalance(root, taller);
				break;

			case EH: // Was balanced--now LH
				root->bal = LH;
				break;

			case RH: // Was right high--now EH
				root->bal = EH;
				taller = false;
				break;
			} // switch
	} //  new < node
	else
		//  new data >= root data
	{
		root->right = _insert(root->right,
			newPtr,
			taller);
		if (taller)
			// Right subtree is taller
			switch (root->bal)
			{
			case LH: // Was left high--now EH
				root->bal = EH;
				taller = false;
				break;

			case EH: // Was balanced--now RH
				root->bal = RH;
				break;

			case RH: // Was right high--rotate
				root = rightBalance(root, taller);
				break;
			} //  switch
	} //  else new data >= root data
	return root;
}	//  _insert

/*	===================== leftBalance =====================
	The tree is out of balance to the left. This function
	rotates the tree to the right.
	   Pre     the tree is left high
	   Post    balance restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>  *AvlTree<TYPE, KTYPE>
::leftBalance(NODE<TYPE>  *root,
	bool&        taller)
{
	// 	Local Definitions
	NODE<TYPE>  *rightTree;
	NODE<TYPE>  *leftTree;

	//	Statements
	leftTree = root->left;
	switch (leftTree->bal)
	{
	case LH: // Left High--Rotate Right
		root->bal = EH;
		leftTree->bal = EH;

		// Rotate Right
		root = rotateRight(root);
		taller = false;
		break;
	case EH: // This is an error
		cout << "\n\a\aError in leftBalance\n";
		exit(100);
	case RH: // Right High - Requires double rotation:
			 // first left, then right
		rightTree = leftTree->right;
		switch (rightTree->bal)
		{
		case LH: root->bal = RH;
			leftTree->bal = EH;
			break;
		case EH: root->bal = EH;
			leftTree->bal = EH;
			break;
		case RH: root->bal = EH;
			leftTree->bal = LH;
			break;
		} //  switch rightTree

		rightTree->bal = EH;
		//  Rotate Left
		root->left = rotateLeft(leftTree);

		// Rotate Right
		root = rotateRight(root);
		taller = false;
	} // switch leftTree
	return root;
}	// leftBalance

/*	===================== rotateLeft ======================
	This function exchanges pointers so as to rotate the
	tree to the left.
	   Pre  root points to tree to be rotated
	   Post NODE rotated and new root returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::rotateLeft(NODE<TYPE>  *root)
{
	//	Local Definitions
	NODE<TYPE>  *tempPtr;

	//	Statements
	tempPtr = root->right;
	root->right = tempPtr->left;
	tempPtr->left = root;

	return tempPtr;
}	//  rotateLeft

/*	===================== rotateRight =====================
	This function exchanges pointers to rotate the tree
	to the right.
	   Pre   root points to tree to be rotated
	   Post  NODE rotated and new root returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::rotateRight(NODE<TYPE> *root)
{
	//	Local Definitions
	NODE<TYPE>  *tempPtr;

	//	Statements
	tempPtr = root->left;
	root->left = tempPtr->right;
	tempPtr->right = root;

	return tempPtr;
}	//  rotateRight

/*	====================  rightBalance ===================
	The tree is out-of-balance to the right. This function
	rotates the tree to the left.
	   Pre     The tree is right high
	   Post    Balance restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>* AvlTree<TYPE, KTYPE>
::rightBalance(NODE<TYPE> *root, bool& taller)
{

	//	Local Definitions
	NODE<TYPE> *rightTree;
	NODE<TYPE> *leftTree;

	//	Statements
	rightTree = root->right;
	switch (rightTree->bal)
	{
	case LH: // Left High - Requires double rotation:
			 //             first right, then left
		leftTree = rightTree->left;

		//  First Rotation - Left
		switch (leftTree->bal)
		{
		case RH: root->bal = LH;
			rightTree->bal = EH;
			break;

		case EH: root->bal = EH;
			rightTree->bal = EH;
			break;

		case LH: root->bal = EH;
			rightTree->bal = RH;
			break;
		} //  switch
		leftTree->bal = EH;

		root->right = rotateRight(rightTree);
		root = rotateLeft(root);
		taller = false;
		break;

	case EH: // Deleting from balanced node
		root->bal = EH;
		taller = false;
		break;

	case RH: // Right High - Rotate Left
		root->bal = EH;
		rightTree->bal = EH;
		root = rotateLeft(root);
		taller = false;
		break;
	} // switch
	return root;
}	//  rightBalance

/*	====================== AVL_Delete ======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	   Pre    dltKey contains key to be deleted
	   Post   the node is deleted and its space recycled
			  -or- an error code is returned
	   Return success (true) or not found (false)
*/

template <class TYPE, class KTYPE>
bool  AvlTree <TYPE, KTYPE> ::AVL_Delete(KTYPE  dltKey)
{
	//	Local Definitions
	bool shorter;
	bool success;

	NODE<TYPE>  *newRoot;

	//	Statements
	newRoot = _delete(tree, dltKey, shorter, success);
	if (success)
	{
		tree = newRoot;
		count--;
	} // if
	return success;
}	// AVL_Delete

/*	======================== _delete =======================
	This function deletes a node from the tree and rebalances
	it if necessary.
	   Pre    dltKey contains key of node to be deleted
			  shorter is Boolean indicating tree is shorter
	   Post   the node is deleted and its space recycled
			  -or- if key not found, tree is unchanged
	   Return true if deleted, false if not found
			  pointer to root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::_delete(NODE<TYPE> *root,
	KTYPE       dltKey,
	bool&       shorter,
	bool&       success)
{
	//  Local Definitions
	NODE<TYPE> *dltPtr;
	NODE<TYPE> *exchPtr;
	NODE<TYPE> *newRoot;

	// 	Statements
	if (!root)
	{
		shorter = false;
		success = false;
		return NULL;
	} //  if -- base case

	if (dltKey < root->data.key)
	{
		root->left = _delete(root->left, dltKey,
			shorter, success);
		if (shorter)
			root = dltRightBalance(root, shorter);
	} // if less
	else if (dltKey > root->data.key)
	{
		root->right = _delete(root->right, dltKey,
			shorter, success);
		if (shorter)
			root = dltLeftBalance(root, shorter);
	} //  if greater
	else
		//  Found equal node
	{
		dltPtr = root;
		if (!root->right)
			// Only left subtree
		{
			newRoot = root->left;
			success = true;
			shorter = true;
			delete (dltPtr);
			return newRoot;            //  base case
		} //  if true
		else
			if (!root->left)
				//  Only right subtree
			{
				newRoot = root->right;
				success = true;
				shorter = true;
				delete (dltPtr);
				return newRoot;        // base case
			} //  if
			else
				//  Delete NODE has two subtrees
			{
				exchPtr = root->left;
				while (exchPtr->right)
					exchPtr = exchPtr->right;

				root->data = exchPtr->data;
				root->left = _delete(root->left,
					exchPtr->data.key,
					shorter,
					success);
				if (shorter)
					root = dltRightBalance(root, shorter);
			} //  else

	} // equal node
	return root;
}	// _delete

  // ================== dltLeftBalance ==================
/*	The tree is out-of-balance to the left (left high)--
	rotates the tree to the right.
	   Pre     The tree is left high
	   Post    Balance has been restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::dltLeftBalance(NODE<TYPE>  *root,
	bool&        smaller)
{
	//	Local Definitions
	NODE<TYPE>  *rightTree;
	NODE<TYPE>  *leftTree;

	//	Statements
	switch (root->bal)
	{
	case RH:  root->bal = EH;
		break;

	case EH:  // Delete occurred on right
		root->bal = LH;
		smaller = false;
		break;

	case LH:  leftTree = root->left;
		switch (leftTree->bal)
		{
		case LH:
		case EH: // Rotate Single Left
			if (leftTree->bal == EH)
			{
				root->bal = LH;
				leftTree->bal = RH;
				smaller = false;
			} // if
			else
			{
				root->bal = EH;
				leftTree->bal = EH;
			} // else

			root = rotateRight(root);
			break;

		case RH:	//Double Rotation
			rightTree = leftTree->right;
			switch (rightTree->bal)
			{
			case LH: root->bal = RH;
				leftTree->bal = EH;
				break;
			case EH: root->bal = EH;
				leftTree->bal = EH;
				break;
			case RH: root->bal = EH;
				leftTree->bal = LH;
				break;
			} //  switch
			rightTree->bal = EH;
			root->left = rotateLeft(leftTree);
			root = rotateRight(root);
			break;
		} //  switch : leftTree->bal

	} //  switch : root->bal
	return root;
}	// dltLeftBalance

/*	=================== dltRightBalance ==================
	The tree is shorter after a delete on the left.
	Adjust the balance factors and rotate the tree
	to the left if necessary.
	   Pre     the tree is shorter
	   Post    balance factors adjusted and balance restored
	   Returns potentially new root
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::dltRightBalance(NODE<TYPE>  *root,
	bool&        shorter)
{
	//  Local Definitions
	NODE<TYPE>  *rightTree;
	NODE<TYPE>  *leftTree;

	//	Statements
	switch (root->bal)
	{
	case LH: //  Deleted Left--Now balanced
		root->bal = EH;
		break;
	case EH: //  Now Right high
		root->bal = RH;
		shorter = false;
		break;
	case RH: //  Right High - Rotate Left
		rightTree = root->right;
		if (rightTree->bal == LH)
			// Double rotation required
		{
			leftTree = rightTree->left;

			switch (leftTree->bal)
			{
			case LH: rightTree->bal = RH;
				root->bal = EH;
				break;
			case EH: root->bal = EH;
				rightTree->bal = EH;
				break;
			case RH: root->bal = LH;
				rightTree->bal = EH;
				break;
			} // switch

			leftTree->bal = EH;

			// Rotate Right then Left
			root->right = rotateRight(rightTree);
			root = rotateLeft(root);
		} //  if rightTree->bal == LH
		else
		{
			//  Single Rotation Only
			switch (rightTree->bal)
			{
			case LH:
			case RH: root->bal = EH;
				rightTree->bal = EH;
				break;
			case EH: root->bal = RH;
				rightTree->bal = LH;
				shorter = false;
				break;
			} // switch rightTree->bal
			root = rotateLeft(root);
		} // else
	} //  switch root bal
	return root;
}	//  dltRightBalance

/*	==================== AVL_Retrieve ===================
	Retrieve node searches the tree for the node containing
	the requested key and returns pointer to its data.
	   Pre     dataOut is variable to receive data
	   Post    tree searched and data returned
	   Return  true if found, false if not found
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE>
::AVL_Retrieve(KTYPE   key, TYPE& dataOut)
{
	//	Local Definitions
	NODE<TYPE> *node;

	//	Statements
	if (!tree)
		return false;

	node = _retrieve(key, tree);
	if (node)
	{
		dataOut = node->data;
		return true;
	} // if found
	else
		return false;
}	//  AVL_Retrieve

/*	===================== _retrieve =====================
	Retrieve searches tree for node containing requested
	key and returns its data to the calling function.
	   Pre     AVL_Retrieve called: passes key to be located
	   Post    tree searched and data pointer returned
	   Return  address of matching node returned
			   if not found, NULL returned
*/

template <class TYPE, class KTYPE>
NODE<TYPE>*  AvlTree<TYPE, KTYPE>
::_retrieve(KTYPE       key,
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		if (key < root->data.key)
			return _retrieve(key, root->left);
		else if (key > root->data.key)
			return _retrieve(key, root->right);
		else
			// Found equal key
			return (root);
	} // if root
	else
		//Data not in tree
		return root;
}	//  _retrieve

/*	==================== AVL_Traverse ====================
	Process tree using inorder traversal.
	   Pre   process used to "visit" nodes during traversal
	   Post  all nodes processed in LNR (inorder) sequence
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Traverse(void(*process)(TYPE dataProc))
{
	//	Statements
	_traversal(process, tree);
	return;
}	// end AVL_Traverse

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Traverse_preorder(void(*process)(TYPE dataProc))
{
	//	Statements
	_traversal_preorder(process, tree);
	return;
}	// end AVL_Traverse_preorder

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::AVL_Traverse_postorder(void(*process)(TYPE dataProc))
{
	//	Statements
	_traversal_postorder(process, tree);
	return;
}	// end AVL_Traverse_postorder

/*	===================== _traversal =====================
	Traverse tree using inorder traversal. To process a
	node, we use the function passed when traversal is called.
	   Pre   tree has been created (may be null)
	   Post  all nodes processed
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_traversal(void(*process)(TYPE dataproc),
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		_traversal(process, root->left);
		process(root->data);
		_traversal(process, root->right);
	} //  if
	return;
}	//  _traversal

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_traversal_preorder(void(*process)(TYPE dataproc),
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		process(root->data);
		_traversal_preorder(process, root->left);
		_traversal_preorder(process, root->right);
	} //  if
	return;
}	//  _traversal_preorder

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_traversal_postorder(void(*process)(TYPE dataproc),
	NODE<TYPE> *root)
{
	//	Statements
	if (root)
	{
		_traversal_preorder(process, root->left);
		_traversal_preorder(process, root->right);
		process(root->data);
	} //  if
	return;
}	//  _traversal_postorder
/*	=================== AVL_Empty ==================
	Returns true if tree is empty, false if any data.
	   Pre      tree has been created; may be null
	   Returns  true if tree empty, false if any data
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::AVL_Empty(void)
{
	//	Statements
	return (count == 0);
}	//  AVL_Empty

/*	=================== AVL_Full ===================
	If there is no room for another node, returns true.
	   Pre      tree has been created
	   Returns  true if no room, false if room
*/

template <class TYPE, class KTYPE>
bool   AvlTree<TYPE, KTYPE> ::AVL_Full(void)
{
	//	Local Definitions
	NODE<TYPE>  *newPtr;

	//	Statements
	newPtr = new NODE<TYPE>;
	if (newPtr)
	{
		delete  newPtr;
		return false;
	} // if
	else
		return true;
}	//  AVL_Full

/*	=================== AVL_Count ===================
	Returns number of nodes in tree.
	   Pre     tree has been created
	   Returns tree count
*/

template <class TYPE, class KTYPE>
int  AvlTree<TYPE, KTYPE> ::AVL_Count(void)
{
	// 	Statements
	return (count);
}	//  AVL_Count

/*	=================== Destructor ===================
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      tree is a pointer to a valid tree
	   Post     all data have been deleted
*/

template <class TYPE, class KTYPE>
AvlTree<TYPE, KTYPE> :: ~AvlTree(void)
{
	//	Statements
	if (tree)
		_destroyAVL(tree);
}	// Destructor

/*	=================== _destroyAVL ===================
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in postorder sequence.
	   Pre   tree is being destroyed
	   Post  all data have been deleted
*/

template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE>
::_destroyAVL(NODE<TYPE>  *root)
{
	//	Statements
	if (root)
	{
		_destroyAVL(root->left);
		_destroyAVL(root->right);
		delete root;
	} // if
	return;
}	//  _destroyAVL

/*  ============================= AVL_Print =============================
	This function prints the AVL tree by calling a recursive inorder
	traversal. NOTE: THIS IS NOT AN APPLICATION ADT FUNCTION. IT IS
	USED ONLY FOR DEBUGGING PURPOSES.

	To correctly visualize the tree when turned sideways, the actual
	traversal is RNL.
	Pre	 Tree must be initialized. Null tree is OK.
		 Level is node level: root == 0
	Post Tree has been printed.
*/
template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> ::AVL_Print(void)
{
	/*  statements */
	_print(tree, 0);
	return;
}   /* AVL_PRINT */

/*  ============================= _print =============================
	This function is not a part of the ADT. It is included to verify
	that the tree has been properly built by printing out the tree
	structure.
*/

/*  This function uses recursion to print the tree. At each level, the
	level number is printed along with the node contents (an integer).
	Pre		root is the root of a tree or subtree
			level is the level of the tree: tree root is 0
	Post    Tree has been printed.
*/
template <class TYPE, class KTYPE>
void  AvlTree<TYPE, KTYPE> ::_print(NODE<TYPE> *root,
	int         level)
{
	/* Local Definitions */
	int i;

	/* Statements */
	if (root)
	{
		_print(root->right, level + 1);

		cout << "Balance Factor:" << setw(3) << root->bal
			<< " | Level:" << setw(3) << level;

		for (i = 0; i <= level; i++)
			cout << "....";
		cout << setw(3) << root->data.key;
		if (root->bal == LH)
			cout << " (LH)\n";
		else if (root->bal == RH)
			cout << " (RH)\n";
		else
			cout << " (EH)\n";

		_print(root->left, level + 1);
	} /* if */

} /* AVL_Print */

#endif // AVLTREE_H
