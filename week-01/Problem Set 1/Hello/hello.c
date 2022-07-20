#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string user_name = get_string("What's your name?\n-> "); // get user's name input
    printf("hello, %s\n", user_name); // greet usercheck50 cs50/problems/2022/x/hello
}