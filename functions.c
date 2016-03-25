#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "functions.h"

/* GENERAL FUNCTIONS */

double gettime(void) {
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
}

int readline(char * buffer, int size, FILE * file) {
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

void proceed_word(FILE * fic, int n, int m, char * word) {
	static int index = 0;
	fprintf(fic, ">%d\n", index);
	fprintf(fic, "%.*s\n", n, word);
	for(int j = 0; j <= n-m; j++) {
		fprintf(fic, ">%d-%d\n", index, j);
		fprintf(fic, "%.*s\n", m, &(word[j]));
	}
	index++;
}

void generate_entry(FILE * fic, char * alphabet, int size_a, int n, int m, int cur, char * word) {
	if(cur==n) {
		proceed_word(fic, n, m, word);
	} else {
		for(int i = 0; i < size_a; i++) {
			word[cur] = alphabet[i];
			generate_entry(fic, alphabet, size_a, n, m, cur+1, word);	
		}
	}
}
