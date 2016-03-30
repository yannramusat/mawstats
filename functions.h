/* GENERAL FUNCTIONS */
double gettime(void);
int readline(char *buffer, int size, FILE *file);
char *to_dir(const char *dir_name, const char *file);
char *itos(int n);
/* SPECIFIC FUNCTIONS */
void proceed_word(FILE *fic, int n, int m, char *word);
void proceed_stat(FILE *fic, FILE *ficr, FILE *ficd, char *alphabet, int size_a, int n, int m, char *word, int v);
void generate_entry_stats(FILE *fic, FILE *ficr, FILE* ficd, char *alphabet, int size_a, int n, int m, int cur, char *word, int action, int v);
void display_stats(int size_a, int n, int m, FILE *ficr);
