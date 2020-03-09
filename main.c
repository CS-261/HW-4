#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include "avl.h"
#include <limits.h>


int FindMinPath(struct AVLTree *tree, TYPE *path);
void printBreadthFirstTree(struct AVLTree *tree);
int printLevel(struct AVLnode* node, int level);
void preorder(struct AVLnode* curr, int* minCost, int totalSum, int* length, int* cLength, TYPE* path, TYPE* cPath, TYPE pVal);

/* -----------------------
The main function
  param: argv = pointer to the name (and path) of a file that the program reads for adding elements to the AVL tree
*/
int main(int argc, char** argv) {

	FILE *file;
	int len, i;
	TYPE num; /* value to add to the tree from a file */
	struct timeval stop, start; /* variables for measuring execution time */
	int pathArray[50];  /* static array with values of nodes along the min-cost path of the AVL tree -- as can be seen, the tree cannot have the depth greater than 50 which is fairly sufficient for out purposes*/

	struct AVLTree *tree;
	
	tree = newAVLTree(); /*initialize and return an empty tree */
	
	file = fopen(argv[1], "r"); 	/* filename is passed in argv[1] */
	assert(file != 0);

	/* Read input file and add numbers to the AVL tree */
	while((fscanf(file, "%i", &num)) != EOF) addAVLTree(tree, num);
	
	/* Close the file  */
	fclose(file);
	
	printf("\nPrinting the tree breadth-first : \n");
	printBreadthFirstTree(tree);

	gettimeofday(&start, NULL);

	/* Find the minimum-cost path in the AVL tree*/
	len = FindMinPath(tree, pathArray);
	
	gettimeofday(&stop, NULL);

	/* Print out all numbers on the minimum-cost path */
	printf("\nThe minimum-cost path is: \n");
	for(i = 0; i < len; i++)
		printf("%d ", pathArray[i]);
	printf("\n");

	printf("\nYour execution time to find the mincost path is %f microseconds\n", (double) (stop.tv_usec - start.tv_usec));

        /* Free memory allocated to the tree */
	deleteAVLTree(tree); 
	
	return 0;
}
  
/* --------------------
Finds the minimum-cost path in an AVL tree
   Input arguments: 
        tree = pointer to the tree,
        path = pointer to array that stores values of nodes along the min-cost path, 
   Output: return the min-cost path length 

   pre: assume that
       path is already allocated sufficient memory space 
       tree exists and is not NULL
*/
int FindMinPath(struct AVLTree *tree, TYPE *path)
{

    /* FIX ME */
	struct AVLnode * curr;
	int length;
	int minCost;
	int cLength;
	TYPE cPath[256];

	curr = tree->root;
	minCost = INT_MAX;
	cLength = 0;
	length = 0;
	path[length] = tree->root->val;
	length++;

	if(tree->cnt > 1)
		preorder(curr, &minCost, 0, &length, &cLength, path, cPath, curr->val);

	return length;

}

void preorder(struct AVLnode* curr, int* minCost, int totalSum, int* length, int* cLength, TYPE* path, TYPE* cPath, TYPE pVal) {

	int i;

	cPath[*cLength] = curr->val;
	(*cLength)++;
	if(!LT(pVal, 0))
		totalSum += abs(pVal - curr->val);

	if(totalSum >= (*minCost)) {
		(*cLength)--;
		return;
	}

	if(curr->left == NULL && curr->right == NULL) {
		if(LT(totalSum, (*minCost))) {
			*minCost = totalSum;
			for(i = 0; i < *cLength; i++)
				path[i] = cPath[i];
			*length = *cLength;
			(*cLength)--;
		}
	} else {
		if(curr->left)
			preorder(curr->left, minCost, totalSum, length, cLength, path, cPath, curr->val);
		if(curr->right)
			preorder(curr->right, minCost, totalSum, length, cLength, path, cPath, curr->val);
		(*cLength)--;
	}

}


double slope(struct AVLnode* parent, struct AVLnode* child) {

	assert(parent);
	assert(child);
	assert(parent->left == child || parent->right == child);

	return (double)abs(parent->val - child->val) / (parent->height - child->height);

}

/* -----------------------
Printing the contents of an AVL tree in breadth-first fashion
  param: pointer to a tree
  pre: assume that tree was initialized well before calling this function
*/
void printBreadthFirstTree(struct AVLTree *tree)
{
   
    /* FIX ME */
	int level = 1;
	while(printLevel(tree->root, level)) level++;
	printf("\n");

}

int printLevel(struct AVLnode* node, int level) {

	int left, right;

	if(!node) return 0;
	if(level == 1) {
		printf("%i ", node->val);
		return 1;
	}

	left = printLevel(node->left, level - 1);
	right = printLevel(node->right, level - 1);

	return left || right;	

}