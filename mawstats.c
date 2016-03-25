#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <time.h>
#include <getopt.h>

#include "functions.h"
#include "chainedlist.h"

#define QUERY_IN "gen_test.in"
#define QUERY_OUT "gen_test.out"
#define MAW_OUT "maw.out"
#define RESULT "results.out"

int main(int argc, char **argv) {
	int n = 8;		// length of the text
	int m = 3;		// length of the pattern
	int v = 0;		// verbose
	
	int size_a = 2;
	char * alphabet = "AC";

	/* Parsing options */
	while(1) {
		int opt = getopt(argc, argv, "l:L:a:s:vh");
		if(opt < 0) break;
		switch(opt) {
			case 'L': 
				n = atoi(optarg);
				break;
			case 'l': 
				m = atoi(optarg);
				break;
			case 'a':
				alphabet = optarg;
				break;
			case 's':
				size_a = atoi(optarg);
				break;
			case 'v':
				v = 1;
				break;
			case 'h':
			default:
				fprintf (stdout, " Usage: %s\n", argv[0]);
				fprintf (stdout, "  -l,	<int>		The pattern length.	Default value: 3\n");
				fprintf (stdout, "  -L,	<int>		The text length.	Default value: 8\n");
				fprintf (stdout, "  -a,	<string>	Specify the alphabet.	Default value: AC\n");
				fprintf (stdout, "  -s,	<int>		Alphabet size.		Default value: 2\n");
				fprintf (stdout, "  -v,			Verbose mode.		Default: off\n");
				fprintf (stdout, "  -h, 			Display help.\n");
				exit(1);
		}
	} 

	/* Generating directory */
	char * dir_name;
	size_t sz;
	double datedir = gettime();
	sz = snprintf(NULL, 0, "%d-%d-%d-%f", n, m, size_a, datedir);
	if((dir_name = (char *)malloc(sz+1)) != NULL) {
		snprintf(dir_name, sz+1, "%d-%d-%d-%f", n, m, size_a, datedir);
	} else {
		printf("Cannot malloc.\n");
		exit(1);
	}
	if(mkdir(dir_name, S_IRWXU) < 0) {
		perror("mkdir");
		exit(1);
	}

	/* Printed messages */
	printf("Processing with text-length %d and pattern-length %d..\n", n, m);
	printf("Alphabet is %s of size %d.\n", alphabet, size_a);
	printf("Writing in directory %s.\n", dir_name);

	/* Generating test */
	double start = gettime();
	FILE* fic = fopen(to_dir(dir_name, QUERY_IN), "w+");
	if(fic != NULL) {	
		char word[n];
		generate_entry(fic, alphabet, size_a, n, m, 0, word);
		fclose(fic);
	} else {
		printf("Cannot open fic to gen queries.\n");
		exit(1);
	}
	double end = gettime();
	printf("Elapsed time for generating words and subwords: %lf secs.\n", (end-start));

	/* Launching MAW */
	int pid = fork();
	if(pid < 0) {
		perror("fork");
		exit(1);
	} else if(pid == 0) { // LAUNCHER PART
		/* Redirecting stderr to file */
		int fd = open(to_dir(dir_name, MAW_OUT), O_CREAT | O_WRONLY, S_IRWXU);
		if(dup2(fd, STDERR_FILENO) == -1) printf("Cannot dup2.\n");

		/* Call of MAW */
		printf("Start computing maws..\n");
		char *K= itos(n+1);
		if(execl("../maw", "maw", "-a", "DNA", "-i", to_dir(dir_name, QUERY_IN), "-o", 
			to_dir(dir_name, QUERY_OUT), "-k", "2", "-K", K, NULL) == -1) 
				{
			perror("exec");
			exit(1);
		}
    } else { // RESULT PART
		wait(NULL);		
		printf("Computation succeed.\nBegin parsing results..\n");
		fic = fopen(to_dir(dir_name, QUERY_OUT), "r");
		FILE * ficr = fopen(to_dir(dir_name, RESULT), "w+");
		if (fic != NULL && ficr != NULL) {
			/* Parsing results */
			for(int i = 0; i < (1 << n); i++) {					
				/* Parsing sequences */
				char buffer[n+2];
				readline(buffer, n+2, fic);
				if(v) printf("Sequence no %s ", &buffer[1]);
				fprintf(ficr, "Seq no %d ", atoi(&buffer[1]));
				int num = 0;
				while(readline(buffer, n+2, fic) != 0) num++;
				if(v) printf("has %d maws.\n", num);
				fprintf(ficr, "with %d maws. ", num);

				/* Parsing sub-sequences */
				for(int j = 0; j <= n-m; j++) {
					readline(buffer, n+2, fic);
					if(v) printf("	Sub no %s ", &buffer[1]);
					int num2 = 0;
					while(readline(buffer, n+2, fic) != 0) num2++;
					if(v) printf("has %d maws.\n", num2);
					fprintf(ficr, "%d ", num2);
				}
				fprintf(ficr, "\n");
			}
			fclose(fic);
			fclose(ficr);
		}
		else {
			printf("Cannot open out file or create result file.\n");
			exit(1);
		}
	}
	return 0;
}
