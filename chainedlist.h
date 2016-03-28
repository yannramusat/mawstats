typedef struct element_word element_word;
struct element_word {
    char * word;
	int length;
	int index;
    struct element_word *nxt;
};
typedef element_word* list_word;

list_word add_last(list_word list, const char * word_to_add, int ind, int L, int *num_elements);
void print_list(list_word list, FILE * stats_fic, int *num_elements, int v);
list_word del_list(list_word list, int *num_elements);
