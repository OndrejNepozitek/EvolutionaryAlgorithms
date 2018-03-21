#pragma once
#include <cstdlib>

template<typename Population>
class BitFlipMutation
{
public:
	void operator()(Population & population)
	{
		for (auto && individual : population)
		{
			for (auto && element : individual)
			{
				if (rand() % 100 < 5)
				{
					element = !element;
				}
			}
		}
	}
};
