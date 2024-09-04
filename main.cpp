#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;


typedef struct structureOfNode {
    int data;
    int difference;
    struct structureOfNode *child[2];
} treeNodeObject;
void treeRotation(treeNodeObject **node, int leftOrRight) {
    int leftSide;
	int rightSide;
	int signature;
	if(leftOrRight) {
		leftSide = 0;
		rightSide = 1;
		signature = 1;
	} else {
		leftSide = 1;
		rightSide = 0;
		signature = -1;
	}
    if((*node)->child[leftSide] == NULL){ 
      exit(1);
    }
    else {
        treeNodeObject* left = (*node)->child[leftSide];
        int x = (*node)->difference * signature;
		int y = left->difference * signature;
		int nx = (y >= 0) ? (x - 1 - y) : (x - 1);
		int ny = (y >= 0) ? ((x - 1 - y >= 0) ? (y - 1) : (x - 2)) : ((x - 1 >= 0) ? (y - 1) : (x + y- 2));
        (*node)->difference = nx * signature;
        left->difference = ny * signature;
        (*node)->child[leftSide] = (*node)->child[leftSide]->child[rightSide];
        left->child[rightSide] = *node;
        *node = left;
    }
}
int treeModifier(treeNodeObject **node, int data, int removeOrAdd) {
    if(*node == NULL) {
        if (!removeOrAdd) {
			return 0;
		} else {
            *node = new treeNodeObject();
            if(*node == NULL){
              exit(1);
            }
            else{
                (*node)->data = data;
                (*node)->difference = 0; 
                (*node)->child[0] = NULL;
                (*node)->child[1] = NULL;
            }
            return *node != NULL;
        }
    }
    else {
        int leftSidetreeModifier;
		int tmpDeltaDifference;
		int tmpDeltaSignature;
        treeNodeObject *nextNode;
        if(data == (*node)->data) {
            if(removeOrAdd) {
                return 0;
            }
            else {
                if((*node)->child[1] == NULL) {
                    nextNode = (*node)->child[0];
                    delete(*node);
                    *node = nextNode;
                    return -1;
                }
                else if((*node)->child[0] == NULL) {
                    nextNode = (*node)->child[1];
                    delete(*node);
                    *node = nextNode;
                    return -1;
                }
                else{
                    nextNode = (*node)->child[0];
					while (nextNode->child[1] != NULL) {
						nextNode = nextNode->child[1];
					}
                   
                    (*node)->data = nextNode->data;
               
                    leftSidetreeModifier = 0;
                    tmpDeltaSignature = 1;
                    tmpDeltaDifference = treeModifier(&(*node)->child[leftSidetreeModifier], nextNode->data, removeOrAdd); 
                }
            }
        }
        else {
            leftSidetreeModifier = (data < (*node)->data) ? 0 : 1;
			tmpDeltaSignature = (data < (*node)->data) ? 1 : -1;
            tmpDeltaDifference = treeModifier(&(*node)->child[leftSidetreeModifier], data, removeOrAdd);
        }
        if (tmpDeltaDifference != 0) {
            int tmpDifference = (*node)->difference;
            int performRotation = 0;
            int rotateLeftSide;
			int differenceSignature;
			int rotateRight;
            (*node)->difference = (*node)->difference + tmpDeltaDifference * tmpDeltaSignature; 
            if ((*node)->difference > 1) {
                //perform right rotation
                performRotation = 1;
                rotateLeftSide = 0;
                differenceSignature = 1;
                rotateRight = 1;
            }
            else if((*node)->difference < -1) {
             
                performRotation = 1;
                rotateLeftSide = 1;
                differenceSignature = -1;
                rotateRight = 0;
            }
            if (performRotation) {
                int differenceInChild = (*node)->child[rotateLeftSide]->difference;
      
                if((*node)->child[rotateLeftSide]->difference * differenceSignature < 0) {
                    treeRotation(&(*node)->child[rotateLeftSide], !rotateRight);
                }
      
                treeRotation(node, rotateRight);
                if(tmpDeltaDifference < 0 && differenceInChild != 0){
                  return -1;
                }
                else{
                  return 0;
                }
            }
   
            if (tmpDeltaDifference > 0 && tmpDifference == 0) {
                return 1;
            }
            else if (tmpDeltaDifference < 0 && tmpDifference != 0) {
                return -1;
            }
            else {
                return 0;
            }
        } else {
            return 0;
        }
    }
}
void postOrderTraverse(const treeNodeObject *node) {
	if (node == NULL) {
	  cout << endl;
	  return;
	}
    postOrderTraverse(node->child[0]);
    postOrderTraverse(node->child[1]);
    cout << node->data << " ";	
}

void inOrderTraverse(const treeNodeObject *node) {
	if(node==NULL) {
	  cout << endl;
	  return;
	}
    inOrderTraverse(node->child[0]);
    cout << node->data << " ";
    inOrderTraverse(node->child[1]);
}
void preOrderTraverse(const treeNodeObject *node) {
	if(node==NULL) {
	  cout << endl;
	  return;
	}
    cout << node->data << " ";
    preOrderTraverse(node->child[0]);
    preOrderTraverse(node->child[1]);
}
void printDataOfTree(string tmp, treeNodeObject *dummy) {
	if (dummy == NULL){
      cout << "EMPTY" << endl;
	  return;
    }
	if(tmp == "IN"){
        inOrderTraverse(dummy);
    }
    else if (tmp == "POST"){
        postOrderTraverse(dummy);
    }
    else if(tmp == "PRE"){
        preOrderTraverse(dummy);
    }
}
int main(void) {
    string inputData;
    getline( std::cin, inputData );
    istringstream is( inputData );
    vector<string> vectorData( ( istream_iterator<string>( is ) ), istream_iterator<string>() );
    treeNodeObject *root = NULL;
	int i = 0;
    while (i < vectorData.size()) {
		string tmp = vectorData[i];
		if(i==vectorData.size()-1) {
			printDataOfTree(vectorData[i],root);
		}
        else{
            int add = (tmp[0] == 'A') ? 1 : 0;
            int value = 0;
            tmp.erase(0,1);
            stringstream function(tmp);
            function >> value;
            treeModifier(&root, value, add);
        }
		i++;
	}
    return 0;
}
