#include <stdio.h>

#define  MAX_VALUE 65536


unsigned short dictionaryArrayCounter = 256;

struct String {
	unsigned char *value;
	int length;
};

typedef struct String string;

string *dictionaryArray[MAX_VALUE];

string *currentDictionaryWord;

string *createDictionaryArrayItem(unsigned char* value, int length) {
	int i;
	string *forExport = (string *) malloc(sizeof(string));
	forExport->value = (unsigned char *) malloc(sizeof(char) * length);
	forExport->length = length;
	for(i = 0; i < length; i++) {
		forExport->value[i] = value[i];
	}
	return forExport;
}

void initDicitionaryArray() {
    int i;
    unsigned char j;
	for (i = 0; i < MAX_VALUE; i++) {
		if (i < 256) {
              j = i;
			dictionaryArray[i] = createDictionaryArrayItem(&j, 1);
		} else {
			dictionaryArray[i] = NULL;
		}
	}
}


typedef struct Node TreeNode;

struct Node {
	
	TreeNode *child[256];
	
	unsigned short value;
	
	int isNodeRoot; 

};

unsigned short value = 0;
	
TreeNode *createNode() {
		int i;
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
     }
     for(i = 0; i < 256; i++) {

           printDictionary(root->child[i]);
     }
}

TreeNode *dictionary;


void appendCurrentDictionaryWord(string *toAppend) {
	int length, i;
	unsigned char* forExport;
	length = currentDictionaryWord->length + toAppend->length;
	currentDictionaryWord->value = (unsigned char *) realloc(currentDictionaryWord->value, sizeof(char) * length);
	for(i = currentDictionaryWord->length; i < length; i++) {
		currentDictionaryWord->value[i] = toAppend->value[i - currentDictionaryWord->length]; 
	}
	currentDictionaryWord->length = length;
	return;
}

int currentDictionaryWordDoesntExist() {
	return !existInDictionary(dictionary, currentDictionaryWord->value, currentDictionaryWord->length, 0);
}


void addNewWordToDictionary() {
     if (dictionaryArrayCounter == 65535) {
        return;
     }                           
	addToDictionary(dictionary, currentDictionaryWord->value);
	dictionaryArray[dictionaryArrayCounter] = createDictionaryArrayItem(currentDictionaryWord->value, currentDictionaryWord->length);
	dictionaryArrayCounter++;
	currentDictionaryWord->length = 0;
}



int main(int argc, char *argv[]) {
	unsigned short bajt;
	int temp = 1;
	string *tempCurrentWord;

	if (argc != 3) {
		printf("Invalid number of arguments!");
		return 0;
	}

	
	FILE *ulaz = fopen(argv[1] , "rb");
	FILE *izlaz = fopen(argv[2], "wb");
	
	dictionary = createRootNode();
	initDicitionaryArray();
	
	currentDictionaryWord = createDictionaryArrayItem("", 0);

	/* while(fread(&bajt, sizeof(unsigned short), 1, ulaz)) {
		if (dictionaryArray[bajt] != NULL) {
			fwrite(dictionaryArray[bajt]->value, sizeof(char) * dictionaryArray[bajt]->length, 1, izlaz);
		} 
		appendCurrentDictionaryWord(dictionaryArray[bajt]);
		if(currentDictionaryWordDoesntExist()) {
			addNewWordToDictionary();
			appendCurrentDictionaryWord(dictionaryArray[bajt]);
		}
	} */
	
	while(fread(&bajt, sizeof(unsigned short), 1, ulaz)) {
			if (dictionaryArray[bajt] == NULL) {
				tempCurrentWord = createDictionaryArrayItem(currentDictionaryWord->value, currentDictionaryWord->length);
				while(!currentDictionaryWordDoesntExist() && temp <= tempCurrentWord->length) {
					appendCurrentDictionaryWord(createDictionaryArrayItem(tempCurrentWord->value, (temp++)));
				}
				temp = 1;
				if(currentDictionaryWordDoesntExist()) {
					addNewWordToDictionary();
					appendCurrentDictionaryWord(dictionaryArray[bajt]);
				}
			} else {
				while(!currentDictionaryWordDoesntExist() && temp <= dictionaryArray[bajt]->length) {
					appendCurrentDictionaryWord(createDictionaryArrayItem(dictionaryArray[bajt]->value, (temp++)));
				}
				temp = 1;
				if (currentDictionaryWordDoesntExist()) {
					addNewWordToDictionary();
                    appendCurrentDictionaryWord(dictionaryArray[bajt]);	
				}
			}
		fwrite(dictionaryArray[bajt]->value, sizeof(char) * dictionaryArray[bajt]->length, 1, izlaz);
	}
	fclose(ulaz);
	fclose(izlaz);
}
