#include <iostream>
void RemoveDups(char* str) // ������ ������ ��� ������ "AA BB AA" ��� ����� � ����������? 2 ��������� ������� � ������ ������������ + ����� ������� ����� '\0'
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
