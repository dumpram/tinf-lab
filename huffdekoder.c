#include <stdio.h>
#include <string.h>

typedef struct Octet {
        char code[256];
} octet;

octet octetTable[256];

int foundInTable(char* buffer, char *position) {
    int i;
    for(i = 0; i < 256; i++) {
          if(!strcmp(octetTable[i].code, buffer)) {
                                         *position = i;
                                         return 1;
          } 
    }
    return 0; 
}

int main(int argc, char *argv[]) {
    
    int i;
    unsigned char data;
    unsigned char buffer[256];
    int currentPos = 0;
    int bitCounter = 7;
    unsigned char position;
    int size;
    
    if (argc != 4) {
       printf("Invalid number of arguments provided! Provide 3 arguments!");
       system("pause");
       return 0;
    }
    
    FILE *table = fopen(argv[1], "r");
    FILE *input = fopen(argv[2], "rb");
    FILE *output = fopen(argv[3], "wb");
    
    for(i = 0; i < 256; i++) {
          if(fscanf(table, "%s", octetTable[i].code) == EOF) {
                           printf("Table error");
                           system("pause");
                           return 0;
          } 
    }
    
    fseek(input, 0, SEEK_END);
    size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    i = 0;
    while(fread(&data, sizeof(char), 1, input) && (i++) != (size - 3)) {
                       while(bitCounter != -1) {
                           buffer[currentPos++] = ((data >> (bitCounter--)) & 1) + 48;
                           buffer[currentPos] = '\0';
                           if(foundInTable(buffer, &position)) {
                                                    fwrite(&position, sizeof(char), 1, output);
                                                    currentPos = 0;
                           }
                       }
                       bitCounter = 7;           
    }
    unsigned char lastData = fread(&data, sizeof(char), 1, input);
    unsigned char lastPos = fread(&data, sizeof(char), 1, input);
    for (currentPos = 0; currentPos < lastPos; currentPos++) {
        buffer[currentPos] = (lastData >> (7 - currentPos)) % 2 + 48;
    } 
    buffer[currentPos] = '\0';
    if(foundInTable(buffer, &position)) {
                            fwrite(&position, sizeof(char), 1, output);                        
    }
}
