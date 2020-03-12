#include <stdio.h>
#include <string.h>
#define MAX_SIZE 65536

typedef struct Node TreeNode;

struct Node {
	
	TreeNode *child[256];
	
	unsigned short value;
	
	int isNodeRoot; 

};

unsigned short value = 0;
	
TreeNode *createNode() {
		int i;
		if (value == 65535) {
           return NULL;
        }
		TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
		for(i = 0; i < 256; i++) {
			node->child[i] = NULL;
		}
		node->value = value;
		node->isNodeRoot = 0;
		value++;
		return node;
	}
	
TreeNode *createRootNode() {
		int  i;
		TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
		for(i = 0; i < 256; i++) {
			node->child[i] = createNode();
		}
		node->isNodeRoot = 1;
		return node;
	}

int existInDictionary(TreeNode *root, unsigned char *word, int length, int depth) {
	if(root == NULL) {
		return 0;
	}
	if(length == depth) {
		return 1;
	}
	else { 
        return existInDictionary(root->child[*word], word + 1, length, depth + 1);
    }
}

unsigned short getCodeFromDictionary(TreeNode *root, unsigned char *word, int length, int depth) {
	if(root == NULL) {
		return -1;
	}
	if(length == depth) {
		return root->value;
	}
	return getCodeFromDictionary(root->child[*word], word + 1, length, depth + 1);
}

TreeNode *addToDictionary(TreeNode *root, unsigned char *word) {
	if(root == NULL) {
		root = createNode();
		return root;
	} else {
      if (root->child[*word] == NULL) {
	     root->child[*word] = addToDictionary(root->child[*word], word + 1);
      } else {
        addToDictionary(root->child[*word], word + 1);
      }
   }
   return root;
}



char *cat(unsigned char *workWord, unsigned char newSymbol, int *length) {
     char *temp = (char *) malloc(sizeof(char) * (*length + 1));
     int i;
     for(i = 0; i < *length; i++) {
           temp[i] = workWord[i];
     }
     temp[*length] = newSymbol;
     return temp;
}

void printDictionary(TreeNode *root) {
     int i;
     if (root == NULL) {
        return;
     }
     if (!(root->isNodeRoot)) {
        printf("%d\n", root->value);
        system("pause");
     }
     for(i = 0; i < 256; i++) {
           printDictionary(root->child[i]);
     }
}

int main(int argc, char *argv[]) {
	
	unsigned char *workWord;
	unsigned char newSymbol;
	unsigned char data;
	unsigned short value;
	unsigned char *temp;
	int length = 1;
	
    if (argc != 3) {
		printf("Provide 2 arguments!");
		return;
	}
	

	FILE *input = fopen(argv[1], "rb");
	FILE *output = fopen(argv[2], "wb");
	
	TreeNode *dictionary = createRootNode();
	
	workWord = (unsigned char *) malloc(1 * sizeof(char));
	if (!fread(&workWord[0], sizeof(char), 1, input)) {
		printf("Input file is empty");
		return;
	}
	while(fread(&newSymbol, sizeof(char), 1, input)) {
		temp = cat(workWord, newSymbol, &length);
		if(existInDictionary(dictionary, temp, length + 1, 0)) {
			workWord = temp;
			length++;
		} else {
            value = getCodeFromDictionary(dictionary, workWord, length, 0);
			fwrite(&value, sizeof(unsigned short), 1, output);
			addToDictionary(dictionary, temp);
			*workWord = newSymbol;
			length = 1;
		}
	}
	value = getCodeFromDictionary(dictionary, workWord, length, 0);
	fwrite(&value, sizeof(unsigned short), 1, output);
	//printDictionary(dictionary);
    return 0;
}
