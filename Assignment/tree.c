#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include <string.h>
#include "mylib.h"

/**
 * Tree structure.
 */
struct treerec{
    char *key;
    tree_color color;
    tree left;
    tree right;
    tree parent;
    tree_t type;
    int frequency;
};

/**
 * Static method declaration.
 */
static void tree_output_dot_aux(tree t, FILE *out); 

/**
 * This method decides the key's position in the left or right branch 
 * of the tree in alphabetical order.
 *
 * @param bstr is the key of the tree node.
 * @param istr is the input string.
 */
int LRBranch(char* bstr, char* istr){    
    char key = *bstr;
    char input = *istr;
    int x = 0;
    
    if(key != '\0' && input != '\0')
	x = key-input;
    else if(key=='\0' && input !='\0')
	return 1;
    else if(key!='\0' && input == '\0')
	return 0;
    if(key=='\0' && input=='\0')
	printf("exception in selecting left/right subtrees!\n");
    if( x < 0 )
	return 1;
    else if(x>0)
	return 0;
    else return LRBranch(bstr+1,istr+1); 			
}

/**
 * This method inserts a given string into a given tree.
 *
 * @param b is the tree that we are inserting the input string into.
 * @param str is the input string.
 * @return find_root(b) returns the root node.
 * @return tree_fix(b) returns the output of the tree_fix method.
 */
tree tree_insert(tree b, char *str){
    tree tmp;
    /*If the root(node) is null then return*/
    if(b == NULL || str == NULL ) return b;
    /*If the current node is not null but its key is null 
      then insert into the current node and return root node*/
    if(b->key==NULL){
	b->key=emalloc((strlen(str)+1)*sizeof(char));
	strcpy(b->key,str);
	if(b->type==BST)
            b->color=BLACK;	
	b->frequency = 1;
	return find_root(b);
    }
    else {
        /*If the left/right branch is null create a node and insert the
          string into this node*/
        if(strcmp(b->key,str)==0){
            b->frequency +=1;
            return find_root(b);		
        }
        /* otherwise do recursion on that branch*/
        if( LRBranch(b->key,str) ){
			
            if(b->right==NULL){
                tree_t typet=b->type;
                tmp=tree_new(typet);
                b->right=tmp;
                tmp->parent=b;
                tree_insert(b->right,str);
                /*after insertion, if it is a red black tree do tree fix*/
                if(b->type == RBT)
                    return tree_fix(b->right);
                /* and tree fix will return the new root node*/
                else return find_root(b);	
            }		
            else tree_insert(b->right,str);
        }
        else {
            if(b->left==NULL){
                tree_t typet=b->type;
                tmp=tree_new(typet);
                b->left=tmp;	
                tmp->parent=b;
                tree_insert(b->left,str);
                if(b->type == RBT)
                    return tree_fix(b->left);
                else return find_root(b);
            }
            else tree_insert(b->left,str);
        }
    }
    return find_root(b);
}

/**
 * This method searches the tree for a given input.
 *
 * @param tree b is the tree we will search for the input string.
 * @param str is the string we are searching for.
 * @return 0 =string found, 1 = string not found.
 */
int tree_search(tree b, char *str){
    if(b==NULL) return 1;

    if(b->key==NULL) return 1;
    else {
        if( strcmp(b->key,str)==0){
            return 0;		
            }
        else if(LRBranch(b->key,str))
            return tree_search(b->right,str);
        else    return tree_search(b->left,str); 
    }		
}

/**
 * Finds the depth of the tree recursively using subtrees.
 *
 * @param tree b is the root node of the tree.
 * @return depthl returns the depth of the left subtree with root b.
 * @return depthr returns the depth of the left subtree with root b. 
 */
int tree_depth(tree b){
    int depthl=0;
    int depthr=0;
    if(b == NULL) return 0;
    else{

        if(b->left ==NULL && b->right==NULL)
            return 0;
	
        if(b->left !=NULL)	
            depthl =  tree_depth(b->left)+1;	

        if(b->right!=NULL)
            depthr =  tree_depth(b->right)+1;
	
        if(depthl>depthr)
            return depthl;
        else return depthr;
    }
}

/**
 * Frees the memory the tree is using recursively.
 *
 * @param tree b is the root node of the tree.
 * @return null if a nodes memory is freed successfully.
 */
tree tree_free(tree b){
	
    if(b!=NULL){		
	if(b->left !=NULL){
            tree_free(b->left);
        }
	if(b->right !=NULL){
            tree_free(b->right);
        }
	if(b->key!=NULL){
            free(b->key);
        }
	free(b);
    }
    return b;	
}

/**
 * Traverses the tree inorder.
 *
 * @param tree b is the root node of the tree.
 * @param void (f)(tree_color color, char *str) is a function from another file.
 */
void tree_inorder(tree b, void (f)(tree_color color, char *str)){
		
    if(b != NULL) {	
	tree_inorder( (b->left), f);
	f(b->color,b->key);
        tree_inorder( (b->right), f);
    }
}

/**
 * Traverses the tree postorder.
 *
 * @param tree b is the root node of the tree.
 * @param void (f)(tree_color color, char *str) is a function from another file.
 */
void tree_postorder(tree b, void (f)(tree_color color, char *str)){
		
    if(b != NULL) {	
	tree_postorder( (b->left), f);	
        tree_postorder( (b->right), f);
	f(b->color,b->key);
    }
}

/**
 * Finds the root node of the tree.
 *
 * @param tree b is the root node of the tree or a node in the tree.
 * @return find_root() the root node of the tree.
 * @return NULL.
 */
tree find_root(tree b){
    if(b==NULL)
	return b;	
    if(b->parent == NULL)
	return b;
    /*if the node's parent is null, then return the current node*/
    if(b->parent != NULL)
	return find_root(b->parent);
    return NULL;
}

/**
 * Create a new tree structure of typet defined below.
 *
 * @param tree_t typet BST or RBT,the type of tree.
 * @return b a new tree node.
 */
tree tree_new(tree_t typet){
    tree b;
    b= emalloc(sizeof( *b));
    b->key= NULL;
    b->left=NULL;
    b->right=NULL;
    b->parent=NULL;
    b->type= BST;
    b->type = typet;
	
    if(typet==RBT)
	b->color=RED;
    else b->color=BLACK;

    b->frequency = 1;
    return b; 
}

/**
 * Traverses the tree in preorder.
 *
 * @param tree b is the root node of the tree.
 * @param void (f)(tree_color color, char *str) is a function from another file.
 */
void tree_preorder(tree b, void (f)(tree_color color,char *str)){
    if (b!= NULL){
    	f(b->color,b->key);

	tree_preorder((b->left) ,f);
	tree_preorder((b->right),f);
    }
}

/**
 * Finds the minimum value key in the tree recursively. 
 *
 * @param tree b is the root node.
 * @return b the lowest value key in the tree.
 * @return NULL.
 */
tree tree_min(tree b){
    if(b==NULL) return NULL;
		
    if(b->left !=NULL)
	tree_min(b->left);
	
    if(b->left==NULL && b->key!=NULL){
        return b;	
    }
    return NULL;
}

/**
 * Finds the maximum value key in the tree recursively. 
 *
 * @param tree b is the root node.
 * @return b the highest value key in the tree.
 * @return NULL.
 */
tree tree_max(tree b){
    if(b==NULL) return b;
		
    if(b->right !=NULL)
	tree_max(b->right);
	
    if(b->right==NULL && b->key !=NULL){
	return b;
    }
    return NULL;
}

/**
 * Rotates the relevant tree/subtree to the right. 
 *
 * @param tree b is the root node we apply the right rotate operation to.
 * @return b is the new 'root' of the given tree or subtree.
 */
tree right_rotate(tree b){
	
    if(NULL== b || NULL==b->left)
	return b;
    else{
        tree tmp=b;
        /*make the left child of root to be the new root*/
        b=b->left;
		
        b->parent=tmp->parent;

        if(b->right!=NULL){
            /* make the old root to be new right child of the new root*/
            tmp->left=b->right;
            /* make the old right child to be the left child of the old root*/
            b->right->parent=tmp;
        }
        else {
            /*make the old root's left child to be null*/
            tmp->left = NULL;
        }
        /*the new root get the parent of the old root
          then set the old root's parent to be new root*/
        b->right=tmp;
        tmp->parent=b;		
        return b;		
    }
}

/**
 * Rotates relevant tree/subtree to the left. 
 *
 * @param tree b is the root node we apply the left rotate operation to.
 * @return b is the new 'root' of the given tree or subtree.
 */
tree left_rotate(tree b){

    if(NULL==b || NULL==b->right)
	return b;
    else{
        tree tmp=b;
        /*make the right child of root to be the new root*/
        b=b->right;
        b->parent=tmp->parent;
        if(b->left!=NULL){
            /* make the old root to be new left child of the new root*/
            tmp->right=b->left;
            b->left->parent=tmp;
        }
        else {
            /*make the old root's right child to be null*/
            tmp->right =  NULL;
        }
        /* the new root get the parent of the old root
           then set new root as the old root's parent*/
        b->left=tmp;
        tmp->parent=b;	
        return b;
    }
}

/**
 * Adjusts the node colours in the RBT tree to the correct sequence. 
 *
 * @param tree b is the node with a red parent, for which the
 *  colour needs to be fixed.
 * @return b the root of the current tree after the colors
 *  of all the nodes are fixed.
 */
tree tree_fix(tree b){    
    /* Recursive operation from the current to the grandparent(if it exists)
     * exactly the same operation as that in the labbook
     * add some code to fix the pointer problems that may break the tree's link
     * it fixes the 'root' recursively 
     * if the grandparent does not exist and parent exist, just repaint parent black
     * if the parent does not exist, repaint itself black
     */    
    tree new_root1;
    tree new_root2;

    if(b->parent == NULL) {
        b->color = BLACK;
        return b;
    }
    else if(b->parent->parent==NULL) {
        b->parent->color = BLACK;
        return b->parent;
    }
    else {
        if((b->parent->parent->left == b->parent && b->parent->left == b)
             &&  IS_RED(b->parent) && IS_RED(b->parent->left)){
            if(IS_BLACK(b->parent->parent->right)){
                tree R = b->parent->parent->parent;
                new_root1=right_rotate(b->parent->parent);
                if(R != NULL && R->left == new_root1->right)
                    R->left = new_root1;		
                else if(R!=NULL && R->right== new_root1->right)
                    R->right = new_root1;
                new_root1->color=BLACK;
                new_root1->right->color=RED;
                return tree_fix(new_root1);
            }
            if(IS_RED(b->parent->parent->right)){
                b->parent->parent->color= RED;
                b->parent->parent->left->color= BLACK;
                b->parent->parent->right->color=BLACK;
                return tree_fix(b->parent->parent);
            }
        }
        if((b->parent->parent->left == b->parent && b->parent->right == b)  
            && IS_RED(b->parent) && IS_RED(b->parent->right)){
            if(IS_RED(b->parent->parent->right)){
                b->parent->parent->color= RED;
                b->parent->parent->left->color= BLACK;
                b->parent->parent->right->color=BLACK;
                return tree_fix(b->parent->parent);
            }
            if(IS_BLACK(b->parent->parent->right)) {
                
                tree R1 = b->parent->parent;
                tree R2 = b->parent->parent->parent;
			
                new_root1=left_rotate(b->parent);
                R1->left = new_root1;
			
                new_root2=right_rotate(R1);
			
                if(R2 != NULL && R2->left == new_root2->right)
                    R2->left = new_root1;		
                else if(R2!=NULL && R2->right== new_root2->right)
                    R2->right = new_root1;
					
                new_root2->color=BLACK;
                new_root2->right->color=RED;	
                return tree_fix(new_root2);	
            }
        }
        if((b->parent->parent->right == b->parent && b->parent->left == b)
            && IS_RED(b->parent) && IS_RED(b->parent->left)) {
            if(IS_RED(b->parent->parent->left)) {
                b->parent->parent->color= RED;
                b->parent->parent->left->color= BLACK;
                b->parent->parent->right->color=BLACK;
                return tree_fix(b->parent->parent);
            }
            if(IS_BLACK(b->parent->parent->left)) {
			
                tree R1 = b->parent->parent;
                tree R2 = b->parent->parent->parent;

                new_root1=right_rotate(b->parent);
                R1->right = new_root1;

                new_root2=left_rotate(R1);

                if(R2 != NULL && R2->left == new_root2->left)
                    R2->left = new_root1;		
                else if(R2!=NULL && R2->right== new_root2->left)
                    R2->right = new_root1;

                new_root2->color=BLACK;
                new_root2->left->color=RED;

                return tree_fix(new_root2);		
            }
        }
        if((b->parent->parent->right == b->parent && b->parent->right == b) 
            && IS_RED(b->parent) && IS_RED(b->parent->right)) {
		
            if(IS_RED(b->parent->parent->left)){
                b->parent->parent->color= RED;
                b->parent->parent->left->color= BLACK;
                b->parent->parent->right->color=BLACK;
                return tree_fix(b->parent->parent);
            }
            if(IS_BLACK(b->parent->parent->left)) {

                tree R = b->parent->parent->parent;

                new_root1=left_rotate(b->parent->parent);
			
                if(R != NULL && R->left == new_root1->left)
                    R->left = new_root1;		
                else if(R!=NULL && R->right== new_root1->left)
                    R->right = new_root1;

                new_root1->color=BLACK;
                new_root1->left->color=RED;	
                return tree_fix(new_root1);	
            }
        }
    }
    return find_root(b);
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == t->type  && RED == t->color) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}
