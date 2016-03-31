#define QUERY_IN "gen_test.in"
#define QUERY_OUT "gen_test.out"
#define MAW_OUT "maw.out"
#define RESULT "results.out"
#define STATS "stats.out"
#define DISTINCT "distinct.out"

/* Not distincts (for subfactors) */
list_word list = NULL;
int num_elements = 0;
double moyl = 0;
int max_sum = 0;
/* Distincts ones (for subfactors) */
list_word list_d = NULL;
int num_elements_d = 0;
double moyd = 0;
int max_distinct = 0;
/* Maws of the word */
list_word list_m = NULL;
int num_elements_m = 0;
double moyL = 0;
int max_maw = 0;
/* Changes */
list_word list_c = NULL;
int num_elements_c = 0;
double moyc = 0;
int max_changes = 0;
