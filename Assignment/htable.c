#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

/**
 * Hash table structure.
 */
struct htablerec {
    int capacity;
    int num_keys;
    int *freq_array;
    char **string_array;
    int *stats;
    hashing_t method;
};

/**
 *  Function prototype for print_stats_line.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full); 

/**
 * Defines the first hash function.
 *
 * @param h - the hash table.
 * @param i - is the original integer key of the string.
 * @return the key index in the hashtable h.
 */
static unsigned int hashfunc1(htable h, unsigned int i) {
    return i % h->capacity ;
}

/**
 * Defines the step size after a collision. Steps for linear hashing
 * or double hashing methods are different sizes.
 * 
 *
 * @param h - the hash table.
 * @param i - the original key of the input string.
 * @return the shift from the current key address.
 */
unsigned int htable_step(htable h, unsigned int i_key) {
    int j = h->capacity;
    
    if(h->method==DOUBLE_H) {
        return (1 + i_key % (j - 1));
    }
    else if(h->method == LINEAR_P) {
        return 1;
    }
    else {
        printf("Hashing undefined");
    }
    return 0;
}

/**
 * Free all of the hash table memory and related memory.
 *
 * @param h - is the hashtable that is no longer in use.
 */
void htable_free(htable h) {
    int i;
	
    for(i = 0; i < h->capacity; i++) {
        if(h->freq_array[i] > 0)
            free(h->string_array[i]);
    }
    free(h->stats);
    free(h->freq_array);
    free(h->string_array);
    free(h);
}

/**
 * Converts the htable word to an integer.
 *
 * @param word - the input string.
 * @return value is a coversion from the input string to an unsigned integer.
 */
static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;

    while(*word != '\0') {
	result = (*word++ + 31 * result);
    }
    return result;
}

/**
 * Inserts the word into the hashtable in the correct index.
 *
 * @param h - the hash table.
 * @param str - is the string to be inserted.
 * @return the frequency of the word in the current hashtable after
 * the word has been inserted.
 */
int htable_insert(htable h, char *str) {
    int k = 0;
    unsigned int seed = htable_word_to_int(str);
    unsigned int i;
        
    /*Calculate the first address of hash function*/
    i = hashfunc1(h,seed);
    /* If the cell is unoccupied, insert the word into this address, set
     * collision as 0, increase the key number by 1, set the key freq as 1.
     */	
    if(strcmp(h->string_array[i],"") == 0) {
        h->string_array[i] = emalloc((strlen(str) + 1) *sizeof str[0]);	
        strcpy(h->string_array[i], str);
        h->freq_array[i] = 1;
        h->stats[h->num_keys] = 0;
        h->num_keys++;
        return 1;
    }
    /* If the key is the same as in the hashtable increase the related freq by
     * 1 and return the key's frequency.
     */
    else if(strcmp(h->string_array[i],str) == 0) {
        h->freq_array[i] += 1;
        return h->freq_array[i];	
    }			
    else {
        /* While loop takes a step to another address, finds an address that is
         * unoccupied or it hosts the same key, if it fails for the same number
         * as its capacity, it will break automatically and stop.  If it finds
         * the expected cell, it will break and do other operation as explained
         * below.
         */
        while(1) {
            if(0 == strcmp(h->string_array[i], "")) {
                break;
            }
            if(strcmp(h->string_array[i], str) == 0) {
                break;
            }
            i += htable_step(h,seed);
            i = i%(h->capacity);
            k++;
            if(k >= h->capacity) {
                return -1;	
            }		
        }
    }
    /* If find an unoccupied place, insert the key into the new address 
     * record the number of collisions and set up word frequency,increase key
     * number.
     */
    if(0 == strcmp(h->string_array[i], "")) {
        h->string_array[i] = emalloc((strlen(str) + 1) *sizeof str[0]);
	
        strcpy(h->string_array[i], str);
        h->freq_array[i] = 1;
        h->stats[h->num_keys] = k;
        h->num_keys++;
        return 1;
    }
    /* Compare input key with that in hashtable 
     * if they are the same,related frequency increase by 1 
     * and return its frequency.
     */
    if(strcmp(h->string_array[i],str) == 0) {
		
        h->freq_array[i] += 1;
        return h->freq_array[i];	
    }	
    return -1;
}

/**
 * Creates a hash table, initiates the related variables and allocates memory.
 *
 * @param capacity - is the size of hashtable.
 * @param method defines how to handle collisions(double hashing or linear
 *        probing).
 * @return htable the hash table created.
 */
htable htable_new(int capacity,hashing_t method) {	
    int i;
    htable result = malloc(sizeof *result);
        
    result->capacity = capacity;
    result->num_keys = 0;
    result->string_array = emalloc(capacity *sizeof (char*));
    result->freq_array = emalloc(capacity *sizeof result->freq_array[0]);
    result->stats = emalloc(capacity *sizeof(int));
        
    for(i = 0;i<capacity; i++) {
	result->string_array[i] = "";
	result->freq_array[i] = 0;
	result->stats[i] = 0;
    }	
    result->method = method;	
    return result; 
}

/**
 * Prints the hash table. Prints the frequency of each word.
 *
 * @param h - the hash table to print.
 * @param void(f)(int freq, char *word) - is a function sent from main file.
 */
void htable_print(htable h, void (f)(int freq, char*word)) {
    int i;
    for(i = 0; i < h->capacity; i++) {
	if(h->freq_array[i] > 0)
            f(h->freq_array[i], h->string_array[i]);
    }	  
}

/**
 * Searches the hash table for the given word.
 *
 * @param h - the hash table to search.
 * @param str - is the string that we search in the given hashtable.
 *
 * @return the frequency of the word or 0 if the word is not found.
 */
int htable_search(htable h, char *str) {
    unsigned int seed = htable_word_to_int(str);
    int k = 0;
    unsigned int i = 0;

    i = hashfunc1(h, seed);	

    while(strcmp(h->string_array[i],str) != 0) { 			
        i += htable_step(h,seed);
        i = i%(h->capacity);
        
        /* k is a counter. 
         * If the program fails to find the string after a certain number of
         * trials, it will break out. The trials are limited to the table
         * capacity.
         */
        k++;
        if(k >= h->capacity)
            return 0;			
    }
    return h->freq_array[i];
}

/**
 * Prints out the entire table, with index
 * stats and the frequency of each word.
 *
 * @li Pos - Index position of the word.
 * @li Freq - Frequency of the word in the hash table.
 * @li Stats - Number of collisions occured at each index.
 * @li Word - The string/word at this position.
 *
 * @param h - the hash table to print.
 */
void htable_print_entire_table(htable h) {
    int i;

    fprintf(stderr,"  Pos  Freq  Stats  Word\n");
    fprintf(stderr,"----------------------------------------\n");
    for(i = 0; i<h->capacity; i++) {
        fprintf(stderr,"%5d %5d %5d   %s\n",i,h->freq_array[i], h->stats[i],
                h->string_array[i]);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print the statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", 
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "----------------------------------------------------\n\n");
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

