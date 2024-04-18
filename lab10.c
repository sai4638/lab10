#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
	int count; 
    struct Trie *next[26];
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!node->next[index]) {
            node->next[index] = (struct Trie *)calloc(1, sizeof(struct Trie));
            if (!node->next[index]) {
                perror("Failed to allocate memory for Trie node");
                exit(EXIT_FAILURE);
            }
        }
        node = node->next[index];
        word++;
    }
    node->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    while (*word) {
        int index = *word - 'a';
        node = node->next[index];
        if (!node) {
            return 0;
        }
        word++;
    }
    return node->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
        if (pTrie) {
        for (int i = 0; i < 26; i++) {
            pTrie->next[i] = deallocateTrie(pTrie->next[i]);
        }
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    return (struct Trie *)calloc(1, sizeof(struct Trie));
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *inFile = fopen(filename, "r");
    if (inFile == NULL) return -1;

    int count = 0;
    fscanf(inFile, "%d\n", &count); 

    char word[100]; 
	
    for (int i = 0; i < count; i++) {
        if (fscanf(inFile, "%s\n", word) == 1) { 
            pInWords[i] = (char *)calloc(strlen(word) + 1, sizeof(char)); 
            if (pInWords[i] == NULL) {
                perror("Failed to allocate memory for word");
                for (int j = 0; j < i; ++j) {
                    free(pInWords[j]);
                }
                fclose(inFile);
                return -1;
            }
            strcpy(pInWords[i], word); 
        }
    }

    fclose(inFile);
    return count;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}