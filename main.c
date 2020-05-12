#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100000
#define TRUE 1
#define FALSE 0
#define DEBUG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#define DEBUG_MSG(fmt)  fprintf(stderr, fmt)
//#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__)


typedef struct Node{
    char *key;
    struct Node * left, *right;
}Node;

typedef struct {
    Node **array;
    int top;
}NodesArray;

NodesArray *newArray() {
    NodesArray * nodes = malloc(sizeof(NodesArray));
    nodes->array = malloc(sizeof(Node *) * MAX_SIZE);
    nodes->top = 0;
}

int getElementIndex(NodesArray *array, char *target) {
    for(int i = 0; i < array->top; i++) {
        if(!strcmpi(array->array[i]->key, target)) {
            return i;
        }
    }
    return -1;
}

char *getKey(NodesArray *nodesArray, int i) {
    return nodesArray->array[i]->key;
}

void addNode(NodesArray *nodesArray, Node *node) {
    nodesArray->array[nodesArray->top++] = node;
}


Node *newNode(char *value) {
    Node *newNode = malloc(sizeof(Node));
    newNode->key = malloc(strlen(value)+1);
    strcpy(newNode->key, value);
    newNode->left = newNode-> right = NULL;
    return newNode;
}

Node *insertRecursive(Node *root, char *val) {

    if(root == NULL){
        return newNode(val);
    }
    if(strcmp(val ,root->key) < 0){
        root->left = insertRecursive(root->left, val);
    }
    else if(strcmp(val ,root->key) > 0) {
        root->right = insertRecursive(root->right, val);
    }
    return root;

}

void inOrder(Node *root) {

    if(root) {

        inOrder(root->left);
        printf("%s\n", root->key);
        inOrder(root->right);


    }

}

void preOrder(Node *root) {

    if(root) {

        printf("%s\n", root->key);
        inOrder(root->left);
        inOrder(root->right);

    }

}

void postOrder(Node *root) {

    if(root) {

        inOrder(root->left);
        inOrder(root->right);
        printf("%s\n", root->key);

    }

}

int max(int x, int y) {
    return x > y ? x : y;
}

Node *getMin(Node *root) {
    if(root == NULL)
        return NULL;
    if(root->left == NULL) {
        return root;
    }
    return getMin(root->left);
}

Node *getMax(Node *root) {
    if(root == NULL)
        return NULL;
    if(root->right == NULL) {
        return root;
    }
    return getMax(root->right);
}

Node *search(Node *root, char *val) {
    if(root == NULL)
        return NULL;
    if(strcmpi(root->key ,val) == 0) {
//        DEBUG("key = %s\ttarget = %s\n", root->key, val);
        return root;
    }
    else if(strcmpi(root->key ,val) > 0) {
//        DEBUG("key = %s\ttarget = %s\n", root->key, val);
        search(root->left, val);
    }
    else {
//        DEBUG("key = %s\ttarget = %s\n", root->key, val);
        search(root->right, val);
    }
}

char *getClosestWord(Node *root, char *word) {
    if(root == NULL)
        return NULL;

    if(strcmpi(root->key ,word) > 0) {
        if(root->left == NULL) {
            return root->key;
        }
        getClosestWord(root->left, word);
    }
    else{
        if(root->right == NULL) {
            return root->key;
        }
        getClosestWord(root->right, word);
    }
}

Node *delete(Node *root, char *target) {

    Node *temp;

    if(root == NULL)
        return NULL;
    if(strcmpi(target, root->key) > 0)
        root->right = delete(root->right, target);
    else if(strcmpi(target, root->key) < 0)
        root->left = delete(root->left, target);
    else {

            if(root->right!=NULL && root->left!=NULL )
            {
                temp = getMin(root->right);
                strcpy(root -> key, temp->key);
                root -> right = delete(root->right,temp->key);
            }
            else
            {
                temp = root;
                if(root->left == NULL)
                    root = root->right;
                else if(root->right == NULL)
                    root = root->left;
                free(temp);
            }
    }

    return root;
}

int getHeight(Node *root) {
    if(root == NULL)
        return -1;
    else
        return 1 + max(getHeight(root->left), getHeight(root->right));
}

int getSize(Node *root) {
    if(root == NULL)
        return 0;
    else
        return 1 + getSize(root->left) + getSize(root->right);
}

void insertIterative(Node *root, char *key) {

    Node *currentNode = root;

    if(root == NULL)
        root = newNode(key);

    while (currentNode) {
        if(strcmpi(currentNode->key, key) > 0) {
            if(currentNode->left == NULL) {
                currentNode->left = newNode(key);
                return;
            }
            currentNode = currentNode->left;
        } else if (strcmpi(currentNode->key, key) < 0) {
            if(currentNode->right == NULL) {
                currentNode->right = newNode(key);
                return;
            }
            currentNode = currentNode->right;
        } else {
            return;
        }
    }

}

Node* load(char *path) {

    FILE *fp = fopen(path, "r");

    if(!fp) {
        fprintf(stderr, "File not found !");
        exit(EXIT_FAILURE);
    }

    char tempWord[128];
    Node *root = NULL;

    while (fscanf(fp, "%s", tempWord) != EOF) {
        tempWord[127] = '\0';
        root = insertRecursive(root, tempWord);
        strcpy(tempWord, "");
    }

    return root;

}

Node *getInOrderSuccessor(Node *n) {

}

void addElementsInOrder(Node * root, NodesArray *nodesArray) {

    if(root) {

        addElementsInOrder(root->left, nodesArray);
        addNode(nodesArray, root);
        addElementsInOrder(root->right, nodesArray);

    }


}

void printArray(NodesArray *array) {
    for (int i = 0; i < array->top; i++)
        printf("%s\n", array->array[i]->key);
}



//}

void printIntro(Node *root) {

    puts("Dictionary Loaded successfully...!");
    puts("----------------------------------");

    printf("Size = %d\n", getSize(root));
    puts("----------------------------------");

    printf("Height = %d\n", getHeight(root));
    puts("----------------------------------");

    puts("Enter a sentence or press ENTER to exit : \n");

}

//Node *convertTreeToArray(Node *root) {
//
//    Node **nodes = malloc(sizeof(Node*)* MAX_SIZE);
//
//
//
int countWords(char *sentence) {

    if(!sentence) {
        DEBUG_MSG("No Sentence Entered");
        return -1;
    }

    int count = 1;

    for(int i = 0; i < strlen(sentence); i++) {
        if(sentence[i] == ' ') {
            count++;
        }
    }

    return count;
}

char **splitSentence(char *sentence) {

    int const numberOfWords = countWords(sentence);

    char **splitSentence = malloc(sizeof(char *) * numberOfWords);


    char *token = strtok(sentence, " ");
    splitSentence[0] = malloc(strlen(token) + 1);
    strcpy(splitSentence[0], token);
    if(token[strlen(token)-1] == '\n')
        token[strlen(token)-1] = '\0';

    for(int i = 1; i < numberOfWords; i++) {
        token = strtok(NULL, " ");
        splitSentence[i] = malloc(strlen(token) + 1);
        strcpy(splitSentence[i], token);
        if(token[strlen(token)-1] == '\n')
            token[strlen(token)-1] = '\0';
    }

    return splitSentence;

}

void checkWord(Node *root, char *key) {

    NodesArray *nodesArray = newArray();
    char *mostSuggestedWord;
    int i;

    if(key[strlen(key)-1] == '\n')
        key[strlen(key)-1] = '\0';

    if(search(root, key)) {
        printf("%s - CORRECT\n", key);
    } else {
        addElementsInOrder(root, nodesArray);
        mostSuggestedWord = getClosestWord(root, key);
        printArray(nodesArray);
        i = getElementIndex(nodesArray, mostSuggestedWord);
        printf("%s - INCORRECT, Suggestions : %s %s %s\n", key, getKey(nodesArray, i), getKey(nodesArray ,i-1), getKey(nodesArray, i+1));
    }

}




int main() {

    Node *root = load("EN-US-Dictionary.txt");

    char sentence[1000];

    char **words;

    int length;
    int choice;
    printIntro(root);

    while (fgets(sentence, 999, stdin) != NULL) {


        length = countWords(sentence);

        words = splitSentence(sentence);

        for (int i = 0; i < length; ++i) {
            checkWord(root, words[i]);
        }

        printIntro(root);

//        puts("Choose one of the following:");
//        puts("----------------------------");
//
//        puts("1. Exit");
//
//        puts("2. Continue");
//        puts("----------------------------");

//        scanf("%d", &choice);

//        strcpy(sentence, "");
//
//        if(choice == 1)
//            break;
//        else if(choice == 2) {
//            continue;
//        }

    }



    return 0;
}
