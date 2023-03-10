#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDS_FILE_PATH "words.txt" // path to the file containing words


int main()
{
    srand( 1);


    int word_len;
    printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\nYou can change only one letter at a time, and each word along the way must be a valid word.\nEnjoy!");
two:

    printf("\n\nHow many letters do you want to have in the words? ");
    FILE* fp = fopen(WORDS_FILE_PATH, "r"); // open file in read mode
    if (fp == NULL)
    {
        printf("Error: Failed to open file %s\n", WORDS_FILE_PATH);
        return 1;
    }
    scanf("%d", &word_len);


    char* word = (char*) malloc(sizeof(char) * (word_len + 1)); // allocate space for the word
    if (word == NULL)
    {
        printf("Error: Failed to allocate memory for word\n");
        return 1;
    }

    int word_count = 0;
    int max_words = 10; // initial size of array to store words
    char** words = (char**) malloc(sizeof(char*) * max_words); // allocate space for array of words
    if (words == NULL)
    {
        printf("Error: Failed to allocate memory for words array\n");
        return 1;
    }

    // read each word from file and add it to the array if its length is equal to the user input
    while (fscanf(fp, "%s", word) != EOF)
    {
        if (strlen(word) == word_len)
        {
            words[word_count] = (char*) malloc(sizeof(char) * (word_len + 1)); // allocate space for word
            if (words[word_count] == NULL)
            {
                printf("Error: Failed to allocate memory for word\n");
                return 1;
            }
            strcpy(words[word_count], word); // copy word to array
            word_count++;

            // dynamically grow array if needed
            if (word_count == max_words)
            {
                max_words *= 2; // double the size of array
                char** new_words = (char**) realloc(words, sizeof(char*) * max_words);
                if (new_words == NULL)
                {
                    printf("Error: Failed to reallocate memory for words array\n");
                    return 1;
                }
                words = new_words;
            }
        }
    }

    char start_word[20];
    char end_word[20];
    printf("Number of %d-letter words found: %d.\n\n", word_len, word_count);

    int len_start = 0;
    int len_end = 0;
one:
    while(len_end != word_len || len_start != word_len)
    {


        printf("Enter starting and ending words, or 'r' for either for a random word: ");
        scanf("%s %s", start_word, end_word );

        //start of length check
        len_start = strlen(start_word);
        len_end = strlen(end_word);

        int found = 0;
        if(strcmp(start_word, "r") == 0 && strcmp(end_word, "r") == 0)
        {
            // generate two random words of length word_len
            int random_index1 = rand() % word_count; // get a random index within the range of word_count
            int random_index2 = rand() % word_count; // get another random index within the range of word_count
            char* random_word1 = words[random_index1]; // get the word at the first random index
            char* random_word2 = words[random_index2]; // get the word at the second random index
            strcpy(start_word, random_word1); // replace start_word with random_word1
            strcpy(end_word, random_word2); // replace end_word with random_word2
            len_start = strlen(start_word);
            len_end = strlen(end_word);
        }
        else if(strcmp(start_word, "r") == 0 || strcmp(end_word, "r") == 0)
        {
            // generate a random word of length word_len
            int random_index = rand() % word_count; // get a random index within the range of word_count
            char* random_word = words[random_index]; // get the word at the random index
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
            if(len_start != word_len)
            {
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", start_word, word_len);
                continue;
            }

            if(len_end != word_len)
            {
                printf("Your word, '%s', is not a %d-letter word. Try again.\n", end_word, word_len);
                continue;
            }

            // printf("Checking if %s is in the array of words...\n", start_word);
            found = 0;
            for (int i = 0; i < word_count; i++)
            {
                if (strcmp(start_word, words[i]) == 0)
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
            for (int i = 0; i < word_count; i++)
            {
                if (strcmp(end_word, words[i]) == 0)
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
// strcpy(received_word, start_word);
    char dup[100];
    while(1)
    {
        if (strcmp(start_word, end_word) == 0)
        {
            done = 1;
            break;
        }

        //strcpy(dup, received_word);
        printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", num, start_word, end_word);
        scanf("%s", received_word);
        count = 0;
        if (strcmp(received_word, "q") == 0)
        {
            goto start;
        }
        else if(strlen(received_word) == word_len)
        {
            int found = 0;
            for (int i = 0; i < word_count; i++)
            {
                if (strcmp(received_word, words[i]) == 0)
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

                for(i=0; i<word_len; i++)
                {
                    if(start_word[i] != received_word[i])
                    {
                        count++;
                    }
                }
                if (count==0)
                {
                    printf("Your word, '%s', is not exactly 1 character different. Try again.\n", received_word);
                    continue;
                }


                else if(count <= 1)
                {
                    num ++;
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
        printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", store, end_word, num-1);
    }

    /*Enter: 1 to play again,
    2 to change the number of letters in the words and then play again, or
    3 to exit the program.

    */
start:
    printf("\nEnter: \t1 to play again,\n");
    printf("\t\t2 to change the number of letters in the words and then play again, or\n");
    printf("\t\t3 to exit the program.\n");
    printf("Your choice --> ");

    int select = 0;
    scanf("%d", &select);
    if(select == 1)
    {
        len_start = 0;
        len_end = 0;
        goto one;
    }
    else if(select == 2)
    {
        goto two;
    }
    else if(select == 3)
    {
        goto three;
    }








three:



    /*
               if(strcmp(start_word, "r") == 0) {
        printf("Start word is r");
    }
           else{
            printf("It is %s", start_word);
           }
    */






    // free memory

    free(words);
    printf("\nThanks for playing!\n");
    printf("Exiting...");

    fclose(fp); // close file

    return 0;
}
