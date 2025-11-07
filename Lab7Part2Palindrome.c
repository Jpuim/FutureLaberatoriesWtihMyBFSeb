#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


bool isPalindrome(char* letters)
{
    if (letters == NULL)
        return false;

    int length = 0;
    while (letters[length] != '\0')
        length++;

    for (int i = 0; i < length / 2; i++)
    {
        if (letters[i] != letters[length - 1 - i])
            return false;
    }
    return true;
}


int main()
{

    char buffer[20];
    printf("Enter a word: ");
    scanf("%s", buffer);

    if (isPalindrome(buffer))
        printf("PALENDROME!\n");
    else
        printf("NOT A PALENDROME!\n");

    return 0;
}