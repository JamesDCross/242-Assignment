#include <stdio.h> 
#include <stdlib.h> 
#include "mylib.h"
#include <assert.h>
#include <ctype.h>

/**
 *Allocates memory.
 *@param size_t s The byte size of memory to be allocated
 *
 * return the pointer point to the current data structure
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
	fprintf(stderr, "Memory allocation failed!\n");
	exit(EXIT_FAILURE);
    }
    return result;
}

/**
 *Reallocates memory.
 *
 *@param void *p void pointer.
 *@param size_t The byte size of memory to be allocated
 */
void *erealloc(void *p, size_t s) {
    p = realloc(p, s);
    if (NULL == p) {
	fprintf(stderr, "Memory allocation failed!\n");
	exit(EXIT_FAILURE);
    }
    return p;
}

/**
 *Gets words from a file stream.
 *
 *@param char *s Pointer to char array
 *@param int limit Number of words
 *@param FILE *stream Pointer to the file input
 *@return int EOF or (w - s)
 */
int getword(char *s, int limit, FILE *stream){
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c);
    if(EOF == c) {
	return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
	*w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);	
        } else if('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}
