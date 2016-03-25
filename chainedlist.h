typedef struct element_word element_word;
struct element_word {
    char * word;
	int index;
    struct element_word *nxt;
};
typedef element_word* list_word;

list_word add_last(list_word list, const char * word_to_add, int ind, int L);
void print_list(list_word list, FILE * stats_fic);
list_word del_list(list_word list);
