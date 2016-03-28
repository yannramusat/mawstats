#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#include "functions.h"
#include "chainedlist.h"

extern list_word list;
extern double moyl;
extern double moyL;
extern int max_sum;
extern int max_distinct;

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

void proceed_word(FILE *fic, int n, int m, char *word) { // TODO Verbose mode
	static int index = 0;
	fprintf(fic, ">%d\n", index);
	fprintf(fic, "%.*s\n", n, word);
	for(int j = 0; j <= n-m; j++) {
		fprintf(fic, ">%d-%d\n", index, j);
		fprintf(fic, "%.*s\n", m, &(word[j]));
	}
	index++;
}

void proceed_stat(FILE *fic, FILE *ficr, int n, int m, char *word, int v) {
	static int index = 0;
	/* Parsing sequences */
	char buffer[n+2];
	readline(buffer, n+2, fic);
	if(v) printf("Sequence no %s ", &buffer[1]);
	fprintf(ficr, "%d ", atoi(&buffer[1]));
	int num = 0;
	while(readline(buffer, n+2, fic) != 0) num++;
	if(v) printf("has %d maws.\n", num);
	fprintf(ficr, "%d ", num);
	moyL += num;

	/* Parsing sub-sequences */
	int tot = 0;
	for(int j = 0; j <= n-m; j++) {
		readline(buffer, n+2, fic);
		if(v) printf("	Sub no %s ", &buffer[1]);
		int num2 = 0;
		while(readline(buffer, n+2, fic) != 0) num2++;
		if(v) printf("has %d maws.\n", num2);
		fprintf(ficr, "%d ", num2);
		moyl += num2; tot += num2;
	}
	fprintf(ficr, "\n");
	if(tot > max_sum) {
		max_sum = tot;
		list = del_list(list);
		list = add_last(list, word, index, n);
	}
	else if(tot == max_sum) {
		list = add_last(list, word, index, n);
	}
	index++;
}

void generate_entry_stats(FILE *fic, FILE *ficr, char *alphabet, int size_a, int n, int m, int cur, char *word, int action, int v) {
	if(cur==n) {
		if(action)
			proceed_stat(fic, ficr, n, m, word, v);
		else
			proceed_word(fic, n, m, word);
	} else {
		for(int i = 0; i < size_a; i++) {
			word[cur] = alphabet[i];
			generate_entry_stats(fic, ficr, alphabet, size_a, n, m, cur+1, word, action, v);	
		}
	}
}

void display_stats(int size_a, int n, int m, FILE *fics) {
	double divi = pow(size_a, n);
	moyL /= divi;
	moyl /= divi;
	fprintf(fics, "%f %f %d\n", moyL, moyl, max_sum);
	print_list(list, fics, 0);
	printf("MoyL=%f, Moyl=%f, Max=%d\n", moyL, moyl, max_sum);
	print_list(list, stdout, 1);
}
