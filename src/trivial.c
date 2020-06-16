#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG
#define TOP_SIZE 10

typedef struct word {
    char* word;
    int count;
} word_t;

int main(int argc, char** argv) {
    char* sPath;
    int bFilenameSpecified = 0;

    switch (argc) {
        case 1:
            printf("Specify file.\n");
            return -1;
        case 2:
            printf("Only path specified.\n");
            sPath = argv[1];
            break;
        default:
            printf("Both path and filename specified. Great!\n");
            sPath = argv[1];
            bFilenameSpecified = 1;
            break;
    }

    int maximum = 0;
    FILE* file = fopen(sPath, "r");
    if (!file) {
        printf("Error with file.\n");
        return -2;
    }
    char buf[255];

    word_t top[TOP_SIZE];

    for (int i = 0; i < TOP_SIZE; ++i) {
        word_t word;
        word.count = 0;
        top[i] = word;
    }

    // algo
    while (fscanf(file, "%s", buf) != EOF) {
        // form word
        #ifdef DEBUG
            printf("%s\n", buf);
        #endif
        word_t word;
        word.word = buf;//(char*)malloc(strlen(buf));
        //strcpy(word.word, buf);

        //already in dict?
        int copy = 0;
        for (int i = 0; i < TOP_SIZE; ++i) {
            if (!top[i].count) {
                break;
            }
            if (!strcmp(top[i].word, word.word)) {
                copy = 1;
                break;
            }
        }

        if (!copy) {

        //search this word
                int count = 0;
                FILE* temp = fopen("file", "r");
                char sTemp[255];
                while (fscanf(temp, "%s", sTemp) != EOF) {
                    if (!strcmp(sTemp, word.word)) {
                        count++;
                    }
                }
                fclose(temp);
                word.count = count;
                #ifdef DEBUG
                    printf("%s mentioned %d\n", word.word, word.count);
                #endif

                // insert
                for(int i = 0; i < TOP_SIZE; ++i) {
                    if (top[i].count == 0) {
                        word_t new_word;
                        new_word.count = word.count;
                        top[i] = new_word;
                        top[i].word = (char*)malloc((strlen(word.word) + 1) * sizeof(char));
                        strcpy(top[i].word, word.word);
                        #ifdef DEBUG
                            printf("%s pushed\n", top[i].word);
                        #endif
                        maximum++;
                        #ifdef DEBUG
                            printf("now maximum is %d\n", maximum);
                        #endif
                        break;
                    } else {
                        if (!strcmp(top[i].word, word.word)) {
                            #ifdef DEBUG
                                printf("%s is already exists\n", top[i].word);
                            #endif
                            break;
                        }
                        if (word.count > top[i].count) {
                            #ifdef DEBUG
                                printf("%s is bigger than %s, moving\n", word.word, top[i].word);
                            #endif
                            if (top[TOP_SIZE - 1].count != 0)
                            {
                                #ifdef DEBUG
                                    printf("Removing element, it is %s %d\n", top[TOP_SIZE-1].word, top[TOP_SIZE - 1].count);
                                #endif
                                free(top[TOP_SIZE - 1].word);
                            }
                            for (int j = TOP_SIZE - 2; j >= i; --j) {
                                top[j+1] = top[j];
                            }
                            word_t new_word;
                            new_word.count = word.count;
                            top[i] = new_word;
                            top[i].word = (char*)malloc((strlen(word.word) + 1) * sizeof(char));
                            strcpy(top[i].word, word.word);
                            break;
                        }
                        #ifdef DEBUG
                            printf("%s not pushed\n", word.word);
                        #endif
                    }
                    
                }


        }
        

    }

    //display
    printf("Top-%d most popular words in", TOP_SIZE);
    if (bFilenameSpecified) {
        for (int i = 2; i < argc; ++i) {
            printf(" %s", argv[i]);
        }
    } else {
        printf(" UNKNOWN file");
    }
    printf("\n");
    for (int i = 0; i < TOP_SIZE; ++i) {
        if (top[i].count)
            printf("%d. %s: %d\n", i + 1, top[i].word, top[i].count);
        else break;
    }


    return 0;
}