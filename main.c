#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W_LENGTH 128
#define S_LENGTH 256
#define DEBUG(fmt, ...) //fprintf(stderr,fmt, __VA_ARGS__)
#define DEBUG_MSG(fmt)  //fprintf(stderr, fmt)

typedef struct Node {
    struct Node *left;
    struct Node *right;
    char *word;
} Node;

typedef struct {
    Node *root;
} Tree;

Tree *init() {
    Tree *t = malloc(sizeof(Tree));
    t->root = NULL;
    return t;
}

Node *newNode(char s[]) {
    Node *n = malloc(sizeof(Node));

    if(n){
        n->word = malloc(strlen(s)+1);
        n->word[strlen(s)] = '\0';
        if(n->word)
            strcpy(n->word, s);

        char *pos;
        if (pos=strchr(n->word, '\n')) *pos = '\0';

        DEBUG("new Node Line %s\n", n->word);

        n->left = n->right = NULL;
        return n;
    }

    return (Node*) NULL;
}

int max (int x, int y) { return x > y? x : y;}

int getTreeHeight(Node *n) {
    if (!n) return -1;
    return 1 + max(getTreeHeight(n->right), getTreeHeight(n->left));
}

void insert(Node *newNode, Node **treeRoot) {
    if (!*treeRoot) {
        *treeRoot = newNode;
        DEBUG("tree root in insert %d\n", treeRoot);
        DEBUG("no root %s\n", treeRoot->word);
        return;
    }

    if (strcasecmp(newNode->word, (*treeRoot)->word) > 0){
        return insert(newNode, &(*treeRoot)->right);
    }

    if (strcasecmp(newNode->word, (*treeRoot)->word) < 0) {
        return insert(newNode, &(*treeRoot)->left);
    }
}

void readLinesPrints(int size) {
    printf("Dictionary loaded successfully\n");
    printf("_______________________________________________\n");
    printf("size = %d\n", size);
    printf("_______________________________________________\n");
}

Tree *readLines(char filename[]) {
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("File Not Exist \n");
        exit(EXIT_FAILURE);
    }

    Tree *tree = init();
    int size = 0;

    while (!feof(file))
    {
        char Line[W_LENGTH];
        fgets(Line, W_LENGTH, file);
        insert(newNode(Line), &tree->root);
        size++;
    }
    readLinesPrints(size);

    DEBUG_MSG("readLinesPrints Fn\n");

    fclose(file);
    return tree;
}

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
    if(!temp) return NULL;
    if(!temp->left) return temp;
    return getDepthL(temp->left);
}

Node* getDepthR(Node* temp){
    if(!temp) return NULL;
    if(!temp->right) return temp;
    return getDepthR(temp->right);
}

void getSuccessorPredecessor(Node* root, Node** pre, Node** suc, Node* node){
    if(!root) return;

    if(strcasecmp(node->word, root->word)==0){
        if(root->right)  *suc = getDepthL(root->right);
        else *suc = root->left;
        if(root->left) *pre = getDepthR(root->left);
        else *pre = root->right;
        return;
    }

    if(strcasecmp(node->word, root->word)>0){
        *pre = root;
        getSuccessorPredecessor(root->right, &(*pre), &(*suc), node);
    }else{
        *suc = root;
        getSuccessorPredecessor(root->left, &(*pre), &(*suc), node);
    }
}

void suggest(Node* root,Node* node, char word[]){
    Node* parent, *pre, *suc;
    parent = getParent(root, word);
    pre = suc = (Node*) NULL;

    getSuccessorPredecessor(root, &pre, &suc, node);

    if(parent)
        printf("Suggestions : %s", parent->word);
    if(suc)
        printf(", %s", suc->word);
    if(pre)
        printf(", %s", pre->word);
    printf("\n");
}

Node * searchWord(Node* root, char word[]){
    Node * prev = NULL;
    while (root){
        prev = root;
        if(strcasecmp(root->word, word) > 0)
            root = root->left;
        else if (strcasecmp(root->word, word) < 0)
            root = root->right;
        else if (strcasecmp(root->word, word)==0)
            return root;
    }
    return prev;
}

void isCorrectWord(char word[], Node* root){
    Node * searchedNode = searchWord(root, word);
    if(searchedNode) {
        if (!strcasecmp(searchedNode->word, word)) printf("CORRECT\n");
        else {
            printf("Incorrect, ");
            suggest(root, searchedNode, word);
        }
    }
}

void checkWords(char sentence[], Tree* tree) {
    char *token;
    token = strtok(sentence, " ");
    while (token) {
        isCorrectWord(token, tree->root);
        token = strtok(NULL, " ");
    }
}

int main() {
    Tree *tree = readLines("EN-US-Dictionary.txt");
    char sentence[S_LENGTH+1];

    printf("Height = %d\n", getTreeHeight(tree->root));
    printf("_______________________________________________\n");

    while(1){
        printf("Enter a sentence :\n");

        if(fgets(sentence, S_LENGTH, stdin))
        {
            char *pos;
            if (pos=strchr(sentence, '\n')) *pos = '\0';
            checkWords(sentence, tree);
        }
    }

    return 0;
}
