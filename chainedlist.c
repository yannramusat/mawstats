#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chainedlist.h"

list_word add_last(list_word list, const char * word_to_add, int ind, int L, int *num_elements) {
    element_word * newElement = (element_word *)malloc(sizeof(element_word));
	newElement->word = (char *)malloc(L);
    strcpy(newElement->word, word_to_add);
	newElement->index = ind;
	newElement->length = L;
    newElement->nxt = NULL;
    if(list == NULL) {
		(*num_elements)++;
        return newElement;
    } else {
        element_word * tmp = list;
        while(tmp->nxt != NULL) {
            tmp = tmp->nxt;
        }
        tmp->nxt = newElement;
		(*num_elements)++;
        return list;
    }
}

void print_list(list_word list, FILE * stats_fic, int *num_elements, int v) {
    element_word *tmp = list;
	if(v) fprintf(stats_fic, "#elements=%d\n", *num_elements);
	else fprintf(stats_fic, "%d\n", *num_elements);
    while(tmp != NULL) {
        fprintf(stats_fic, "%.*s %d\n", tmp->length, tmp->word, tmp->index);
        tmp = tmp->nxt;
    }
}

list_word del_list(list_word list, int *num_elements) {
    if(list == NULL) {
        return NULL;
    } else {
        element_word *tmp;
        tmp = list->nxt;
		free(list->word);
        free(list);
		*num_elements = 0;
        return del_list(tmp, num_elements);
    }
}
