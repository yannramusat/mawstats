MF=			makefile
CC=			gcc
EXE=    	mawstats
SRC=		mawstats.c functions.c chainedlist.c
OBJ= 		$(SRC:.c=.o)
HD=			functions.h chainedlist.h
CFLAGS=		-std=c99 -Wall
LDFLAGS=	-std=c99

# RULES, NO NEED TO EDIT BELOW THIS LINE

all:    $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< 

$(OBJ): $(MF) $(HD)

.PHONY: clean clean-all

clean:
	rm -f $(OBJ)

clean-all: clean
	rm -f $(EXE)
