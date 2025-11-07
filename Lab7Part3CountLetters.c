#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define MAX 1000

void countLetters(char text[], int counts[])
{
	for (int i = 0; text[i] != '\0'; i++)
	{
		char ch = text[i];
		if (ch >= 'A' && ch <= 'Z')
		{
			counts[ch - 'A']++;
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			counts[ch - 'a']++;
		}
	}
}


int main()
{
	printf("Enter text (empty line to finish):\n");
	char text[MAX];
	int counts[26] = { 0 };
	while (true)
	{
		fgets(text, MAX, stdin);

		if (text[strlen(text) - 1] != '\n')
		{
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF);
		}

		if (strlen(text) == 1 && text[0] == '\n')
			break;

		countLetters(text, counts);
	}

	printf("Distribution of letters in the input:\n");
	for (int i = 0; i < 26; i++)
		printf("%c: %d ", 'A' + i, counts[i]);

	printf("\n");

	return 0;
}