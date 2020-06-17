#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#define TOP_SIZE 10

typedef enum etError {
	etErrorOk = 0,
	etErrorArgument = -1,
	etErrorFile = -2,
	etErrorMalloc = -3
} etError;

typedef struct SWord {
	char *sWord;
	int nCount;
} word_t;

etError insert(word_t *wordTop, int nIdx, char *sWord, int nCount) {
	wordTop[nIdx].sWord = (char *)malloc((strlen(sWord) + 1) * sizeof(char));
	if (!wordTop[nIdx].sWord) {
		return etErrorMalloc;
	}
	strcpy(wordTop[nIdx].sWord, sWord);
	wordTop[nIdx].nCount = nCount;
	return etErrorOk;
}

int main(int argc, char **argv) {
	char *sPath;
	int bFilenameSpecified = 0;
	etError rc = etErrorOk;

	switch (argc) {
	case 1:
		printf("Specify file.\n");
		return etErrorArgument;
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

	int nMaximumIdx = 0;
	FILE *pFile = fopen(sPath, "r");
	if (!pFile) {
		printf("Something wrong with pFile.\n");
		return etErrorFile;
	}
	char buf[255];

	word_t wordTop[TOP_SIZE];

	for (int i = 0; i < TOP_SIZE; ++i) {
		word_t word;
		word.nCount = 0;
		wordTop[i] = word;
	}

	while (fscanf(pFile, "%s", buf) != EOF) {
// form word
#ifdef DEBUG
		printf("Input word: %s\n", buf);
#endif
		word_t word;
		word.sWord = buf;

		// already in dict?
		int bCopy = 0;
		for (int i = 0; i < TOP_SIZE; ++i) {
			if (!wordTop[i].nCount) {
				break;
			}
			if (!strcmp(wordTop[i].sWord, word.sWord)) {
				bCopy = 1;
				break;
			}
		}

		if (!bCopy) {

			// search this word
			int nCount = 0;
			FILE *temp = fopen("file", "r");
			char sTemp[255];
			while (fscanf(temp, "%s", sTemp) != EOF) {
				if (!strcmp(sTemp, word.sWord)) {
					nCount++;
				}
			}
			fclose(temp);
			word.nCount = nCount;
#ifdef DEBUG
			printf("%s mentioned %d\n", word.sWord, word.nCount);
#endif

			// insert
			for (int i = 0; i < TOP_SIZE; ++i) {
				if (wordTop[i].nCount == 0) {
					rc = insert(wordTop, i, word.sWord, word.nCount);
					if (rc != etErrorOk) {
						printf("Error with inserting word into the top.\n");
						return rc;
					}
#ifdef DEBUG
					printf("%s pushed\n", wordTop[i].sWord);
#endif
					nMaximumIdx++;
#ifdef DEBUG
					printf("now max index is %d\n", nMaximumIdx);
#endif
					break;
				} else {
					if (!strcmp(wordTop[i].sWord, word.sWord)) {
#ifdef DEBUG
						printf("%s is already exists\n", wordTop[i].sWord);
#endif
						break;
					}
					if (word.nCount > wordTop[i].nCount) {
#ifdef DEBUG
						printf("%s is meeted more often than %s, moving\n", word.sWord,
									 wordTop[i].sWord);
#endif
						if (wordTop[TOP_SIZE - 1].nCount != 0) {
#ifdef DEBUG
							printf("Removing element from top, it is %s: %d\n",
										 wordTop[TOP_SIZE - 1].sWord, wordTop[TOP_SIZE - 1].nCount);
#endif
							free(wordTop[TOP_SIZE - 1].sWord);
						}
						for (int j = nMaximumIdx - 1; j >= i; --j) {
							wordTop[j + 1] = wordTop[j];
						}
						rc = insert(wordTop, i, word.sWord, word.nCount);
						if (rc != etErrorOk) {
							printf("Error with inserting word into the top.\n");
							return rc;
						}
						break;
					}
#ifdef DEBUG
					printf("%s not pushed\n", word.sWord);
#endif
				}
			}
		}
	}

	// display
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
		if (wordTop[i].nCount)
			printf("%d. %s: %d\n", i + 1, wordTop[i].sWord, wordTop[i].nCount);
		else
			break;
	}

	return 0;
}