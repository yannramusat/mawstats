/* GENERAL FUNCTIONS */
double gettime(void);
int readline(char * buffer, int size, FILE * file);
char *to_dir(const char *dir_name, const char *file);
char *itos(int n);
/* SPECIFIC FUNCTIONS */
void proceed_word(FILE * fic, int n, int m, char * word);
void generate_entry(FILE * fic, char * alphabet, int size_a, int n, int m, int cur, char * word);