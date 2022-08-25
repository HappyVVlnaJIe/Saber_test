#include <iostream>
void RemoveDups(char* str) // задать вопрос про строку "AA BB AA" что будет в результате? 2 указателя сделать и просто перекидывать + потом сделать конец '\0'
{
	char* last = str;
	while (*str != '\0')
	{
		if (*str != *last)
		{
			last++;
			*last = *str;
		}
		str++;
	}
	if (last != str)
	{
		last++;
		*last = '\0';
	}
}
