#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StringArray
{
    char **words;
    int size;
    int capacity;
};

void initializeStringArray(struct StringArray *arr)
{
    arr->words = malloc(10 * sizeof(char *));
    arr->size = 0;
    arr->capacity = 10;
}

void addString(struct StringArray *arr, const char *word)
{
    if (arr->size == arr->capacity)
    {
        // Double the capacity of the array if it's full
        arr->capacity *= 2;
        arr->words = realloc(arr->words, arr->capacity * sizeof(char *));
    }

    // Allocate memory for the new string and copy the contents of `word` into it
    arr->words[arr->size] = malloc(strlen(word) + 1);
    strcpy(arr->words[arr->size], word);

    // Increment the size of the array
    arr->size++;
}

void freeStringArray(struct StringArray *arr)
{
    for (int i = 0; i < arr->size; i++)
    {
        free(arr->words[i]);
    }
    free(arr->words);
    arr->size = 0;
    arr->capacity = 0;
}

int main()
{

    srand(1);
    int word_len = 0;
    struct StringArray arr;
    FILE *fp;

    printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\nYou can change only one letter at a time, and each word along the way must be a valid word.\nEnjoy!");
two:
    
    fp = fopen("words.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }
    initializeStringArray(&arr);
    printf("\n\nHow many letters do you want to have in the words? ");
    scanf("%d", &word_len);

    char word[100];
    while (fscanf(fp, "%s", word) == 1)
    {
        if (strlen(word) == word_len)
        {
            addString(&arr, word);
        }
    }
    char start_word[20];
    char end_word[20];
    printf("Number of %d-letter words found: %d.\n\n", word_len, arr.size);

    int len_start = 0;
    int len_end = 0;
one:
    while (len_end != word_len || len_start != word_len)
    {

        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s %s", start_word, end_word);

        len_start = strlen(start_word);
        len_end = strlen(end_word);

        int found = 0;
        if (strcmp(start_word, "r") == 0 && strcmp(end_word, "r") == 0)
        {
            // generate two random words of the length set by player.
            int random_index1 = rand() % arr.size;         // get a random index within the range of words available in the array.
            int random_index2 = rand() % arr.size;         // get another random index.
            char *random_word1 = arr.words[random_index1]; // get the word at the first random index
            char *random_word2 = arr.words[random_index2]; // get the word at the second random index
            strcpy(start_word, random_word1);              // replace start_word with random_word1
            strcpy(end_word, random_word2);                // replace end_word with random_word2
            len_start = strlen(start_word);
            len_end = strlen(end_word);
        }
        else if (strcmp(start_word, "r") == 0 || strcmp(end_word, "r") == 0)
        {
            // generate a random word of length word_len
            int random_index = rand() % arr.size;        // get a random index within the range of word_count
            char *random_word = arr.words[random_index]; // get the word at the random index
            if (strcmp(start_word, "r") == 0)
            {
                strcpy(start_word, random_word);
                len_start = strlen(start_word);
                len_end = strlen(end_word);
            }
            if (strcmp(end_word, "r") == 0)
            {
                strcpy(end_word, random_word); // replace end_word with random_word
                len_start = strlen(start_word);
                len_end = strlen(end_word);
            }
        }

        else
        {
            if (len_start != word_len)
            {
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", start_word, word_len);
                continue;
            }

            if (len_end != word_len)
            {
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", end_word, word_len);
                continue;
            }

            found = 0;
            for (int i = 0; i < arr.size; i++)
            {
                if (strcmp(start_word, arr.words[i]) == 0)
                {
                    found = 1; // set flag if word is found
                    break;
                }
            }
            if (!found)
            {
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", start_word);
                len_start = 0;
                len_end = 0;
                continue;
            }

            found = 0;
            for (int i = 0; i < arr.size; i++)
            {
                if (strcmp(end_word, arr.words[i]) == 0)
                {
                    found = 1; // set flag if word is found
                    break;
                }
            }
            if (!found)
            {
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", end_word);
                len_start = 0;
                len_end = 0;

                continue;
            }
        }
    }

    printf("Your starting word is: %s.\n", start_word);
    printf("Your ending word is: %s.\n", end_word);
    char store[word_len];
    strcpy(store, start_word);

    printf("\nOn each move enter a word of the same length that is at most 1 character different and is also in the dictionary.");
    printf("\nYou may also type in 'q' to quit guessing.\n");

    int i, count = 0;
    int num = 1;
    int done = 0;
    char received_word[word_len];
    while (1)
    {
        if (strcmp(start_word, end_word) == 0)
        {
            done = 1;
            break;
        }

        printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", num, start_word, end_word);
        scanf("%s", received_word);
        count = 0;
        if (strcmp(received_word, "q") == 0)
        {
            goto start;
        }
        else if (strlen(received_word) == word_len)
        {
            int found = 0;
            for (int i = 0; i < arr.size; i++)
            {
                if (strcmp(received_word, arr.words[i]) == 0)
                {
                    found = 1; // set flag if word is found
                    break;
                }
            }
            if (!found)
            {
                printf("Your word, '%s', is not a valid dictionary word. Try again.\n", received_word);
                continue;
            }
            else
            {

                for (i = 0; i < word_len; i++)
                {
                    if (start_word[i] != received_word[i])
                    {
                        count++;
                    }
                }
                if (count == 0)
                {
                    printf("Your word, '%s', is not exactly 1 character different. Try again.\n", received_word);
                    continue;
                }

                else if (count <= 1)
                {
                    num++;
                    if (strcmp(start_word, end_word) == 0)
                    {
                        done = 1;
                        break;
                    }
                    else
                    {
                        strcpy(start_word, received_word);
                        continue;
                    }
                }
                else
                {
                    printf("Your word, '%s', is not exactly 1 character different. Try again.\n", received_word);
                    continue;
                }
            }
        }
        else
        {
            printf("Your word, '%s', is not a %d-letter word. Try again.\n", received_word, word_len);
            continue;
        }
    }

    if (done)
    {
        printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", store, end_word, num - 1);
    }

// Prompt user to eiher play again or quit
start:
    printf("\nEnter: \t1 to play again,\n");
    printf("\t\t2 to change the number of letters in the words and then play again, or\n");
    printf("\t\t3 to exit the program.\n");
    printf("Your choice --> ");

    int select = 0;
    scanf("%d", &select);
    if (select == 1)
    {
        len_start = 0;
        len_end = 0;
        goto one;
    }
    else if (select == 2)
    {
        fclose(fp);
        freeStringArray(&arr);
        goto two;
    }
    else if (select == 3)
    {
        goto three;
    }

three:
    
    printf("\nThanks for playing!\n");
    printf("Exiting...");
    fclose(fp);

    freeStringArray(&arr);

    return 0;
}
