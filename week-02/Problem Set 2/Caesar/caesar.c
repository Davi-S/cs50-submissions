#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(int key, char letter);

int main(int argc, string argv[])
{
    // Get a valid key
    if ((argv[1] == NULL) || (!only_digits(argv[1])) || (atoi(argv[1]) < 0) || (argv[2] != NULL))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // store the valid key
    int key = atoi(argv[1]);

    // input plain text
    string plain_text = get_string("plaintext: ");

    // output the cypher text by a for loop
    printf("ciphertext: ");
    for (int i = 0, len = strlen(plain_text); i < len; i++)
    {
        printf("%c", rotate(key, plain_text[i]));
    }
    printf("\n");
    return 0;
}

bool only_digits(string s)
{
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (isdigit(s[i]) == 0)
        {
            return false;
        }
    }
    return true;
}


char rotate(int key, char letter)
{
    if (isupper(letter) || islower(letter)) // make sure thar letter is not a number or anything else, but a letter
    {
        int encrypted_char = (int)letter + key;
        while (encrypted_char > (isupper(letter) ? 'Z' : 'z'))
        {
            encrypted_char -= 26;
        }
        return (char)encrypted_char; // return the char encrypted if it is a letter
    }
    return letter; // just return the given char if it is not a letter
}
