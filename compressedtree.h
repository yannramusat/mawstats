typedef struct node_ctree node_ctree;
struct node_ctree {
    node_ctree **sons;
	int pointed;
};
typedef node_ctree* ctree;

ctree init_ctree(int size_a);
int add_word(ctree tree, char *alphabet, int size_a, char *word, int current, int n, int *num_elements, int pt_val, int *diff);
void del_ctree(ctree tree, int size_a);
void print_ctree(ctree tree, FILE *distinct_fic, char *alphabet, int size_a, char *word, int current);
