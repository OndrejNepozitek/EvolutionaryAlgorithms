#pragma once
#include <random>

namespace ea
{
	template<typename TPopulation>
	class RouletteWheelSelector
	{
		using t_individual = typename TPopulation::value_type;
		using t_fitness = typename t_individual::fitness_type;
		using t_population_iterator = typename TPopulation::iterator;

	public:
		void operator()(TPopulation& from, TPopulation& to, const int count);

		static void select(TPopulation& from, TPopulation& to, const int count);
	};

	template <typename TPopulation>
	void RouletteWheelSelector<TPopulation>::operator()(TPopulation& from, TPopulation& to, const int count)
	{
		select(from, to, count);
	}

	template <typename TPopulation>
	void RouletteWheelSelector<TPopulation>::select(TPopulation& from, TPopulation& to, const int count)
	{
		std::random_device rd;
		std::mt19937 e2(rd());
		std::uniform_real_distribution<> dist(0, 1);

		t_fitness fitness_sum = 0;

		for (auto&& individual : from)
		{
			fitness_sum += individual.fitness;
		}

		auto counter = 0; // TODO: remove

		for (auto i = 0; i < count; ++i)
		{
			auto random = dist(e2) * fitness_sum;
			t_fitness sum = 0;

			for (auto&& individual : from)
			{
				sum += individual.fitness;

				if (random <= sum)
				{
					to.push_back(individual);
					++counter;
					break;
				}
			}
		}

		if (counter != count)
		{
			throw std::exception{ "not enough individuals were selected" };
		}
	}
}
