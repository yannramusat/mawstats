typedef struct node_ctree node_ctree;
struct node_ctree {
    node_ctree *sons;
};
typedef node_ctree* ctree;

ctree add_word(ctree tree, int *num_elements);
ctree del_ctree(ctree tree, int *num_elements);
void print_ctree(ctree tree, int *num_elements);
