#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define W_LENGTH 64
#define S_LENGTH 256

typedef struct Node{
    struct Node* left;
    struct Node* right;
    char* word;
}Node;

typedef struct{
    Node* root;
} Tree;

Tree* init(){
    Tree* t = malloc(sizeof(Tree));
    t->root = NULL;
    return t;
}

Node* newNode(char s[]){
    Node *n = malloc(sizeof(Node));
    n->word = malloc(strlen(s));
    strcpy(n->word, s);
    n->left=n->right=NULL;
    return n;
}

int getTreeHeight(Node* n){
    if(!n) return 0;
    int rHeight = getTreeHeight(n->right);
    int lHeight = getTreeHeight(n->left);
    if(rHeight>lHeight) return rHeight+1;
    return lHeight+1;

}

void insert(Node* newNode, Node* treeRoot){
    if(!treeRoot) {treeRoot=newNode;return;}
    if(strcasecmp(newNode->word, treeRoot->word)>0) return insert(newNode, treeRoot->right);
    if(strcasecmp(newNode->word, treeRoot->word)<0) return insert(newNode, treeRoot->left);
    return;
}

void readLinesPrints(int size){
    printf("Dictionary loaded successfully\n");
    printf("_______________________________________________\n");
    printf("size = %d\n", size);
    printf("_______________________________________________\n");
}

Tree* readLines(char* filename){
    FILE* file = fopen(filename,"r");
    Tree* tree = init(); int size = 0;
    while(!feof(file)) {
        char* Line = malloc(sizeof(*Line)*W_LENGTH);
        Line[W_LENGTH-1] = '\0';
        fgets(Line, W_LENGTH, file);
        printf("%s , size = %d\n", Line, size);
        insert(newNode(Line), tree->root);
        size++;
    }
    readLinesPrints(size);
    fclose(file);
    return tree;
}

int isEmpty(Tree* tree) {return getTreeHeight(tree->root);}

Node* getParent(Node* root, char word[]){
    if(!root) return NULL;
    Node * parent = NULL;
    while(root){
        if(strcasecmp(root->word, word)>0) {
            parent = root;
            root = root->left;
        }
        else if(strcasecmp(root->word, word)<0){
            parent = root;
            root = root->right;
        }
        else break;
    }
    return parent;
}


Node* getDepthL(Node* temp){
    if(!temp->left) return temp;
    return getDepthL(temp->left);
}

Node* getDepthR(Node* temp){
    if(!temp->right) return temp;
    return getDepthL(temp->right);
}

void getSuccessorPredecessor(Node* root, Node* pre, Node* suc, Node* node){
    if(!root) return;
    if(strcasecmp(node->word, root->word)==0){
        if(root->right)  suc = getDepthL(root->right);
        else suc = root->left;
        if(root->left) pre = getDepthR(root->left);
        else pre = root->right;
        return;
    }
    if(strcasecmp(node->word, root->word)>0){
        pre = root;
        getSuccessorPredecessor(root->right, pre, suc, node);
    }else{
        suc = root;
        getSuccessorPredecessor(root->left, pre, suc, node);
    }
}

void suggest(Node* node, char word[]){
    Node* parent, *pre, *suc;
    parent = getParent(node);
    pre = suc = (Node*) NULL;
    getSuccessorPredecessor()

    printf("Suggestions : %s", node->word);
    if(node->left) printf(", %s", node->left->word);
    if(node->right) printf(", %s", node->right->word);
    printf("\n");
}

Node* searchWord(Node* root, char word[]){
    if(!root||!strcasecmp(root->word, word)) return root;
    if(strcasecmp(root->word, word)>0) return searchWord(root->left, word);
    return searchWord(root->right, word);
}

void isCorrectWord(char word[], Node* root){
    printf("%s - ", word);
    if(searchWord(root, word)) printf("CORRECT\n");
    else {
        printf("Incorrect, ");
        suggest(root, word);
    }
}

void checkWords(char* sentence, Tree* tree) {
    char *token;
    token = strtok(sentence, " ");
    while (token) {
        isCorrectWord(token, tree->root);
        token = strtok(NULL, " ");
    }
}

int main()
{
    Tree* tree = readLines("EN-US-Dictionary.txt");
    char sentence[S_LENGTH];
    printf("Height = %d\n", getTreeHeight(tree->root));
    printf("_______________________________________________\n");
    printf("Enter a sentence :\n");
    if(fgets(sentence, S_LENGTH, stdin)) checkWords(sentence, tree);
    return 0;
}
