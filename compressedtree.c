#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compressedtree.h"

ctree init_ctree(int size_a) {
	ctree newElement = (ctree)malloc(sizeof(node_ctree));
	newElement->sons = (ctree)malloc(sizeof(node_ctree)*size_a);
	for(int i = 0; i < size_a; i++) newElement->sons[i] = NULL;
	return newElement;
}

int add_word(ctree tree, char *alphabet, int size_a, char *word, int current, int n, int *num_elements) {
	if(current < n) {
		char letter = word[current];
		int index = -1;
		for(int i = 0; i < size_a; i++)
			if(alphabet[i] == letter)
				index = i;
		if(index != -1) {		
			if(tree->sons[index] == NULL) {
				tree->sons[index] = init_ctree(size_a);
				if(current == n-1) (*num_elements)++;
				return add_word(tree->sons[index], alphabet, size_a, word, current+1, n, num_elements);
			} else {
				
				return add_word(tree->sons[index], alphabet, size_a, word, current+1, n, num_elements);
			} 
		} else {
			printf("Adding word in CT: letter doesn't recognized.\n");
			return -1;
		}
	} else return 0;
}

void print_ctree(ctree tree, char *alphabet, int size_a, char *word, int current) {
	int see = 0;
	for(int i = 0; i < size_a; i++) {
		if(tree->sons[i] != NULL) {
			see = 1;
			word[current] = alphabet[i];
			print_ctree(tree->sons[i], alphabet, size_a, word, current+1);
		}
	}
	if(!see) {
		printf("%.*s\n", current, word);
	}
}
