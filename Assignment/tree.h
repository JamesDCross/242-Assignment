#ifndef tree_H_
#define tree_H_


#define IS_BLACK(x) ((NULL ==(x)) ||(BLACK == (x)->color))
#define IS_RED(x) ((NULL !=(x)) && (RED == (x)->color))

typedef enum {RED,BLACK} tree_color;
typedef enum {BST,RBT} tree_t;
typedef struct treerec *tree;

extern tree tree_free(tree b);
extern void tree_inorder(tree b, void f(tree_color color,char *str));
extern tree tree_insert(tree b, char *str);
extern tree tree_new();
extern void tree_preorder(tree b, void f(tree_color color,char *str));
extern void tree_postorder(tree b, void f(tree_color color,char *str));
extern int tree_search(tree b, char *str);
extern tree tree_min(tree b);
extern tree tree_max(tree b);
extern tree right_rotate(tree b);
extern tree left_rotate(tree b);
extern tree tree_fix(tree b);
extern int LRBranch(char* bstr, char* istr);
extern tree find_root(tree b);
extern int tree_depth(tree b);
extern void tree_output_dot(tree t, FILE *out);
#endif
