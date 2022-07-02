#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int sentence_count(string text);
int get_index(float letters_p_w, float sentences_p_w);

int main(void)
{
    // Get text input
    string text = get_string("TEXT: ");

    // Get amount of each item to check on the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = sentence_count(text);

    // Amount per word
    float letters_per_word = (float)letters / (float)words * 100.0;
    float sentences_per_word = (float)sentences / (float)words * 100.0;

    // Coleman-Liau formula
    int index = get_index(letters_per_word, sentences_per_word);

    // Print results
    if (1 < index && index < 16)
    {
        printf("Grade %i\n", index);
    }
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
}

int count_letters(string text)
{
    int count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isupper(text[i]) || islower(text[i])) // Test if is a letter
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }
    return count + 1; // Adding one for the last word
}

int sentence_count(string text)
{
    int count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        char letter = text[i];
        if (letter == '.' || letter == '!' || letter == '?') // Ends of sentences
        {
            count++;
        }
    }
    return count;
}

int get_index(float letters_p_w, float sentences_p_w)
{
    float index = 0.0588 * letters_p_w - 0.296 * sentences_p_w - 15.8;
    return round(index);
}