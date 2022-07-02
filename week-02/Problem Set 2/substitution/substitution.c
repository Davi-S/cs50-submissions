#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool only_alphabetical(string s);
bool unique_chars(string s);
char rotate(char letter, string key);

int main(int argc, string argv[])
{
    string key = argv[1];

    // Get a valid key
    if ((key == NULL) || (argv[2] != NULL)) // Too much or no arguments
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if ((strlen(key) != 26) || (!only_alphabetical(key)) || (!unique_chars(key)))
    {
        printf("key must be 26 alphabetical character long without repeating\n");
        return 1;
    }

    // Ask from plain text
    string plain_text = get_string("plaintext: ");

    printf("ciphertext: ");
    for (int i = 0, len = strlen(plain_text); i < len; i++)
    {
        printf("%c", rotate(plain_text[i], key));
    }
    printf("\n");

    return 0;
}

bool only_alphabetical(string s) // Test if all the chars in the string are digits (0 to 9)
{
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (isdigit(s[i]) != 0)
        {
            return false;
        }
    }
    return true;
}

bool unique_chars(string s) // Test if all chars are not repeating
{
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if ((s[i] == s[j]) && (j != i))
            {
                return false;
            }
        }
    }
    return true;
}

// TODO remake logic on rotate
char rotate(char letter, string key)
{
    if (isupper(letter) || islower(letter)) // make sure thar letter is not a number or anything else, but a letter
    {
        int key_value = isupper(letter) ? letter - 'A' : letter - 'a';
        letter = isupper(letter) ? toupper(key[key_value]) : tolower(key[key_value]);
    }
    return letter;
}