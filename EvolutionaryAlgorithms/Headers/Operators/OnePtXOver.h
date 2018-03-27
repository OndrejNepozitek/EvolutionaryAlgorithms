#pragma once
#include <cstdlib>

namespace ea
{
	template<typename Population>
	class OnePtXOver
	{
	public:
		void operator()(Population& population);

		static void operate(Population& population);
	};

	template <typename Population>
	void OnePtXOver<Population>::operator()(Population& population)
	{
		operate(population);
	}

	template <typename Population>
	void OnePtXOver<Population>::operate(Population& population)
	{
		auto individual_size = population[0].size();

		for (auto i = 0; i < population.size() / 2; ++i)
		{
			auto p1 = population[2 * i];
			auto p2 = population[2 * i + 1];

			if (rand() % 100 < 5)
			{
				auto position = rand() % individual_size;

				auto temp = p1[position];
				p1[position] = p2[position];
				p2[position] = temp;
			}
		}
	}
}
