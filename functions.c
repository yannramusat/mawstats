#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#include "functions.h"
#include "chainedlist.h"
#include "compressedtree.h"

extern list_word list;
extern int num_elements;
extern list_word list_d;
extern int num_elements_d;
extern list_word list_m;
extern int num_elements_m;
extern list_word list_c;
extern int num_elements_c;
extern double moyl;
extern double moyL;
extern double moyd;
extern double moyc;
extern int max_maw;
extern int max_sum;
extern int max_distinct;
extern int max_changes;

/* GENERAL FUNCTIONS */

double gettime(void) {
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
}

int readline(char *buffer, int size, FILE *file) {
	int i = 0;
	char c;
	while ((c = fgetc(file)) != '\n' && c != EOF && i < size) {
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return i;
}

char *to_dir(const char *dir_name, const char *file) {
     char *res = (char *) malloc(strlen(dir_name)+strlen(file)+2);
     strcpy(res,dir_name);
	 strcat(res,"/");
     strcat(res,file);
     return res;
}

char *itos(int n) {
	char *s;
	int length;
	length = snprintf(NULL, 0, "%d", n);
	if((s = (char *)malloc(length+1)) != NULL) {
		snprintf(s, length+1, "%d", n);
		return s;
	} else {
		printf("itos: cannot malloc.\n");
		exit(1);
	}
}

/* SPECIFIC FUNCTIONS */

void proceed_word(FILE *fic, int n, int m, char *word, int v) {
	static int index = 0;
	fprintf(fic, ">%d\n", index);
	fprintf(fic, "%.*s\n", n, word);
	if(v) printf(">%d\n%.*s\n", index, n, word);
	for(int j = 0; j <= n-m; j++) {
		fprintf(fic, ">%d-%d\n", index, j);
		fprintf(fic, "%.*s\n", m, &(word[j]));
		if(v) printf(">%d-%d\n%.*s\n", index, j, m, &(word[j]));
	}
	index++;
}

void proceed_stat(FILE *fic, FILE *ficr, FILE *ficd, char *alphabet, int size_a, int n, int m, char *word, int v) {
	static int index = 0;
	/* Parsing sequences */
	char buffer[n+2];
	readline(buffer, n+2, fic);
	if(v) printf("Sequence no %s (%.*s) ", &buffer[1], n, word);
	fprintf(ficr, "%d ", atoi(&buffer[1]));
	int num = 0;
	while(readline(buffer, n+2, fic) != 0) num++;
	if(v) printf("has %d maws.\n", num);
	fprintf(ficr, "%d ", num);
	moyL += num;
	if(num > max_maw) {
		max_maw = num;
		list_m = del_list(list_m, &num_elements_m);
		list_m = add_last(list_m, word, index, n, &num_elements_m);
	} else if(num == max_maw) {
		list_m = add_last(list_m, word, index, n, &num_elements_m);
	}
	/* Parsing sub-sequences */
	ctree tree = init_ctree(size_a);
	int nbrelem = 0;
	int tot = 0;
	int lastnum = 0;
	int changes = 0;
	for(int j = 0; j <= n-m; j++) {
		readline(buffer, n+2, fic);
		if(v) printf("	Sub no %s ", &buffer[1]);
		int num2 = 0;
		int lr;
		while((lr = readline(buffer, n+2, fic)) != 0) {
			add_word(tree, alphabet, size_a, buffer, 0, lr, &nbrelem, j+2, &lastnum);
			num2++;
		}
		if(v) {
			printf("has %d maws.\n", num2);
		}
		fprintf(ficr, "%d ", num2);
		moyl += num2; tot += num2;
		changes += lastnum;
		lastnum = num2;
	}
	moyc += changes;
	if(changes > max_changes) {
	max_changes = changes;
	list_c = del_list(list_c, &num_elements_c);
	list_c = add_last(list_c, word, index, n, &num_elements_c);
	} else if(changes == max_changes) {
		list_c = add_last(list_c, word, index, n, &num_elements_c);
	}
	if(v) {
		printf("Changes: %d\n", changes);
	}
	moyd += nbrelem;
	fprintf(ficr, "%d %d \n", tot, nbrelem);
	char printing_word[n+2];
	if(v) {
		printf("There are %d distincts ones:\n", nbrelem);
		print_ctree(tree, stdout, alphabet, size_a, printing_word, 0);
	}
	fprintf(ficd, "%d %d\n", index, nbrelem);
	print_ctree(tree, ficd, alphabet, size_a, printing_word, 0);
	if(tot > max_sum) {
		max_sum = tot;
		list = del_list(list, &num_elements);
		list = add_last(list, word, index, n, &num_elements);
	} else if(tot == max_sum) {
		list = add_last(list, word, index, n, &num_elements);
	}
	if(nbrelem > max_distinct) {
		max_distinct = nbrelem;
		list_d = del_list(list_d, &num_elements_d);
		list_d = add_last(list_d, word, index, n, &num_elements_d);
	} else if(nbrelem == max_distinct) {
		list_d = add_last(list_d, word, index, n, &num_elements_d);
	}
	del_ctree(tree, size_a);
	index++;
}

void generate_entry_stats(FILE *fic, FILE *ficr, FILE *ficd, char *alphabet, int size_a, int n, int m, int cur, char *word, int action, int v) {
	if(cur==n) {
		if(action)
			proceed_stat(fic, ficr, ficd, alphabet, size_a, n, m, word, v);
		else
			proceed_word(fic, n, m, word, v);
	} else {
		for(int i = 0; i < size_a; i++) {
			word[cur] = alphabet[i];
			generate_entry_stats(fic, ficr, ficd, alphabet, size_a, n, m, cur+1, word, action, v);
		}
	}
}

void display_stats(int size_a, int n, int m, FILE *fics) {
	double divi = pow(size_a, n);
	moyL /= divi;
	moyl /= divi;
	moyd /= divi;
	moyc /= divi;
	// FIC
	fprintf(fics, "%f %f %f %f %d %d %d %d\n", moyL, moyl, moyd, moyc, max_maw, max_sum, max_distinct, max_changes);
	print_list(list_m, fics, &num_elements_m, 0);
	print_list(list, fics, &num_elements, 0);
	print_list(list_d, fics, &num_elements_d, 0);
	print_list(list_c, fics, &num_elements_c, 0);
	// STDOUT
	printf("=== Stats ===\nMoyL=%f, Moyl=%f, Moyd= %f, Moyc=%f, Max=%d, Max_no_distinct=%d, Max_distinct=%d, Max_changes=%d\n", moyL, moyl, moyd, moyc, max_maw, max_sum, max_distinct, max_changes);
	printf("List of elements reaching the maximum number of maws:\n");
	print_list(list_m, stdout, &num_elements_m, 1);
	printf("List of elements reaching the maximum number of sub_maws (not distincts):\n");
	print_list(list, stdout, &num_elements, 1);
	printf("List of elements reaching the maximum number of sub_maws (distincts):\n");
	print_list(list_d, stdout, &num_elements_d, 1);
	printf("List of elements reaching the maximum number of changes:\n");
	print_list(list_c, stdout, &num_elements_c, 1);
}
