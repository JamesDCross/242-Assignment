#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "mylib.h"
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <time.h>

/**
 * Prints to screen the usage/arguments for the main function.
 */
void usage(){
    printf("Usage: ./tree-main [Option]... <Stdin>\n");
    printf("\n");
    printf("Perform various operations using a binary tree.\n");
    printf("By default,words are read from stdin and added to the hash table\n");
    printf("before being printed out alongside their frequencies to stdout\n");
    printf("\n");
    printf("-c FILENAME Check spelling of words in FILENAME using words\n");
    printf("            from stdin as dictionary.Print timing and unknown words");
    printf(" to\n");
    printf("            stderr(ignore -d & -o)\n");
    printf("-d          only print the tree depth(ignore -o)\n");
    printf("-f FILENAME write DOT output to FILENAME(if -o given)\n");
    printf("-o          Output the tree in DOT form to file 'tree-view.dot'\n");
    printf("-r 	    Make the tree and RBT (the default is a BST)\n");
    printf("-h          Display this message\n");
}

/**
 * Main function. Operations stated in usage().
 *
 * @param argc - argc the number being passed to argv.
 * @param argv - array of arguments refered to in usage().
 * @return EXIT_SUCCESS.
 */
int main(int argc, char ** argv){

    clock_t start1,end1;
    clock_t start2,end2;
    /*Count of unknown words*/
    int count = 0;

    char word[256];

    const char *optstring = "c:dpf:orh";
    /*The parameter after -c, filename, string */
    char* cvalue = NULL;
    /*The parameter after -f, filename, string */
    char* fvalue = NULL;
    /*Input option*/
    char option;
    tree b;
    /*Treetype:default bst*/
    tree_t treetype = BST;
    /*enable -o*/
    int oflag = 0;
    /*enable -d*/
    int dflag = 0;
    /*enable -c*/
    int cflag = 0;
    /*enable -f*/
    int fflag = 0;

    /*Receive input from user, set up arguments*/
    while((option = getopt(argc, argv, optstring)) != EOF){
	switch(option){
            case 'c':cflag = 1;cvalue=optarg; break; 
            case 'd':dflag = 1; break; 
            case 'o':oflag = 1; break;     
            case 'f':fflag = 1;fvalue=optarg; break;
            case 'r':treetype = RBT; break; 
            case 'h':usage(); return 0;
            default :usage(); return 0;
	}
    }
    b = tree_new(treetype);
    start1 = clock();

    while(getword(word, sizeof word, stdin) != EOF) {
	b=tree_insert(b, word);
    }
    end1 = clock();

    if(cflag == 1){
        /*open a file and read the file(decided by -c filename)
         *then search the string in tree.
         *If it fails to find the word, count number increase and
         * print the unknown word to stdout.
         */
        FILE *file;
        file = fopen(cvalue, "r");
        start2 = clock();
        while (getword(word, sizeof word, file) != EOF) {
            if(tree_search(b,word) == 1){
		printf("%s\n",word);
		count++;	
            }
        }
        /*closes the file*/
        fclose(file);
        end2 = clock();
        /*print statics to std err*/
        fprintf(stderr, "Fill time	: %f\n",(end1-start1)/(double)
                CLOCKS_PER_SEC);
        fprintf(stderr, "Search time	: %f\n",(end2-start2)/(double)
                CLOCKS_PER_SEC);
        fprintf(stderr, "Unknown words	= %d\n", count);		
    }
    else {
        if(dflag == 1) {
            printf("%d\n", tree_depth(b));
        }
        else {
            if(oflag==1) {
		FILE *fp; 
		if(fflag==1) {
                    fp=fopen(fvalue, "wb+");
                    printf("Creating dot file '%s'\n",fvalue);
		} else { 
                    fp=fopen("tree-view.dot","wb+");
                    printf("Creating dot file 'tree-view.dot'\n");
		}		
		tree_output_dot(b, fp);
		/*closes the file*/
		fclose(fp); 
            }
        }
    }

    /*free memory*/
    tree_free(b);
    return 0;
}

