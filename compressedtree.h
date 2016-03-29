typedef struct node_ctree node_ctree;
struct node_ctree {
    node_ctree **sons;
	int pointed;
};
typedef node_ctree* ctree;

ctree init_ctree(int size_a);
int add_word(ctree tree, char *alphabet, int size_a, char *word, int current, int n, int *num_elements);
ctree del_ctree(ctree tree, int *num_elements);
void print_ctree(ctree tree, char *alphabet, int size_a, char *word, int current);
