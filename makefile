MF=			makefile
CC=			gcc
EXE=    	mawstats
SRC=		mawstats.c functions.c chainedlist.c compressedtree.c
OBJ= 		$(SRC:.c=.o)
HD=			defs.h functions.h chainedlist.h compressedtree.h
CFLAGS=		-std=c99 -Wall
LDFLAGS=	-std=c99 -Wall -lm

# RULES, NO NEED TO EDIT BELOW THIS LINE

all:    $(EXE)

$(EXE): $(OBJ)
	$(CC)  $^ -o $@ $(LDFLAGS)

.c.o:
	$(CC) -c $< $(CFLAGS) 

$(OBJ): $(MF) $(HD)

.PHONY: clean clean-all

clean:
	rm -f $(OBJ)

clean-all: clean
	rm -f $(EXE)
