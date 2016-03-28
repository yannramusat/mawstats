typedef struct node_ctree node_ctree;
struct node_ctree {
    
};
typedef node_ctree* ctree;

ctree add_word(ctree tree, char *word, int length, int *num_elements);
ctree del_ctree(ctree tree, int *num_elements);
void print_ctree(ctree tree, char *current word, int current, int *num_elements);
