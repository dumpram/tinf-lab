#include <stdio.h>

typedef struct MultipleNode Node;

struct MultipleNode {
       int counter;
       int flag;
       unsigned char octet;
       Node *left;
       Node *right;
       Node *forward;
       char *code;
       char bit;
};

Node nodeTable[256];
char buffer[256];
void printList(Node* list);


Node* addToList(Node* first, Node* node) {
      
      Node* temp = first;
      Node* prev = NULL;
	  
      if (first == NULL) {
         return node;   
      }
      
      if (node->counter > first->counter) {
         node->forward = first;
         return node;
      }
      
      
      for (prev = first, temp = first->forward; temp != NULL && node->counter <= temp->counter; prev = temp, temp = temp->forward);
      
      prev->forward = node;
      node->forward = temp;
	  
      return first;    
}

Node *refactorList(Node* first) {
     Node *prev;
     Node *temp;
     prev = first;
     temp = first;
     
     if (first == NULL) {
        return NULL;
     }
     if (first->forward == NULL) {
        return first;
     }
     
          if (first->forward->forward == NULL) {
             Node* newNode = (Node *) malloc(sizeof(Node));
             newNode->counter = first->counter + first->forward->counter;
             newNode->left = first;
             newNode->right = first->forward;
             newNode->flag = 0;
             first->flag = 1;
             first->forward->flag= 1;
             first->bit = '1';
             first->forward->bit = '0';
             first = addToList(first, newNode);
             first->forward = NULL;
             return first;
     }
     while(first->forward->forward != NULL) {
                                   prev = first;
                                   first = first->forward;
     }   
     Node* newNode = (Node *) malloc(sizeof(Node));
     newNode->counter = first->counter + first->forward->counter;
     newNode->left = first;
     newNode->right = first->forward;
     newNode->flag = 0;
     first->flag = 1;
     first->forward->flag= 1;
     first->bit = '1';
     first->forward->bit = '0';
     prev->forward = NULL;
     first = addToList(temp, newNode);
     return first;
}

Node* addToList2(Node* first, Node* node) {
      
      Node* temp = first;
      Node* prev = NULL;
	  
      if (first == NULL) {
         return node;   
      }
      
      if (node->counter <= first->counter) {
         node->forward = first;
         return node;
      }
      
      
      for (prev = first, temp = first->forward; temp != NULL && node->counter > temp->counter; prev = temp, temp = temp->forward);
      
      prev->forward = node;
      node->forward = temp;
	  
      return first;    
}

Node* refactorList2(Node *first) {
      Node *newNode;
      
      if(first == NULL) {
               return NULL;
      }
      if(first->forward == NULL) {
                        return first;
      }
      
      newNode = (Node *) malloc(sizeof(Node));
      newNode->counter = first->counter + first->forward->counter;
      newNode->left = first->forward;
      newNode->right = first;
      first->forward->bit = '1';
      first->bit = '0';
      if (first->forward->forward == NULL) {
         return newNode;
      }
      return addToList2(first->forward->forward, newNode);          
}


void initNodeTable() {
     int i;
     for (i = 0; i < 256; i++) {
         nodeTable[i].octet = i;
         nodeTable[i].counter = 0;
         nodeTable[i].left = NULL;
         nodeTable[i].right = NULL;
         nodeTable[i].code = NULL;
     }
}

void printTree(Node* root, int dubina) {
     if(root == NULL) {
             return;
     }
     if (dubina != 0) {
        buffer[dubina - 1] = root->bit;
     }
     if(root->left == NULL) {
         printTree(root->left, dubina + 1);
         buffer[dubina] = '\0';
         root->code = (char *) malloc(sizeof(char)*(dubina + 1));
         sprintf(root->code, "%s", buffer);
        // printf("%s", buffer);
     } else {
       printTree(root->left, dubina + 1);
       printTree(root->right, dubina + 1);
     }
}

void printList(Node* list) {
     while(list != NULL) {
                printf("%d ", list->counter);
                list = list->forward;
     }
     printf("\n");
}

void fill(char *input, char *output) {
     FILE *in = fopen(input, "rb");
     FILE *out = fopen(output, "wb");
     int currentPos = 0;
     int codePos = 0;
     unsigned char data;
     unsigned char result = 0;
     char *code;
     
     while(fread(&data, sizeof(char), 1, in) != 0) {
                        code = nodeTable[data].code;
                        while (code[codePos] != '\0') {
                              if (currentPos == 8) {
                                 currentPos = 0;
                                 fwrite(&result, sizeof(char), 1, out);
                                 result = 0;
                              }
//                              if (code[codePos] == '1') {
//                                 result += 1 << (7 - currentPos);        
//                              }
                              result = result << 1;
                              result += code[codePos] - 48;
                              currentPos++;
                              codePos++;
                        }
                        codePos = 0;
     }
     fwrite(&result, sizeof(char), 1, out);
     fwrite(&currentPos, sizeof(char), 1, out);
}
     
int main(int argc, char *argv[]) {
    unsigned char octet;
    int i;
    
    if(argc != 4) {
       printf("No arguments provided!");
       return 0;
    }
    initNodeTable();
    FILE *inputFile = fopen(argv[1], "rb");
    FILE *huffTable = fopen(argv[2], "w");
    FILE *outputFile = fopen(argv[3], "wb");
    
    while(fread(&octet, sizeof(char), 1, inputFile)) {
                        //printf("Citam: %d\n", octet);
                        nodeTable[octet].counter++;                           
    }
    
    Node* list = NULL;
    
    for(i = 255; i >= 0 ; i--) {
            list = addToList2(list, &nodeTable[i]);         
    }    
    
    //printList(list);
    //system("pause");
    while(list->forward != NULL){
			list = refactorList2(list);
    }
    //printList(list);
    //printf("%d %d", list->left->bit, list->right->bit);
    //system("pause");
	
    printTree(list, 0);  
    //system("pause"); 
    
    for(i = 0; i < 256; i++) {
          fprintf(huffTable, "%s\n", nodeTable[i].code);
    }
	fill(argv[1], argv[3]);
	return 0;
}

