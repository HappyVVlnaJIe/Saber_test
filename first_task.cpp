#include <iostream>

void MyBitset(int numb)
{
	int max_bit = 1 << 30;
	std::cout << (numb < 0);
	while (max_bit != 0)
	{
		std::cout << ((numb & max_bit) != 0);
		max_bit >>= 1;
	}
}