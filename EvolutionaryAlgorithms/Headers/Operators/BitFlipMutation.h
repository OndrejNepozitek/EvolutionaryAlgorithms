#pragma once
#include <cstdlib>

namespace ea
{
	template<typename Population>
	class BitFlipMutation
	{
	public:
		void operator()(Population& population);
	};

	template <typename Population>
	void BitFlipMutation<Population>::operator()(Population& population)
	{
		for (auto&& individual : population)
		{
			for (auto&& element : individual)
			{
				if (rand() % 100 < 5)
				{
					element = !element;
				}
			}
		}
	}
}
