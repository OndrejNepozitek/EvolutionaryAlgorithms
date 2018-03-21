#include <iostream>
#include <vector>
#include "Headers/ArrayIndividual.h"
#include "Headers/Operators/OnePtXOver.h"
#include "Headers/Operators/BitFlipMutation.h"
#include "Headers/EvolutionaryAlgorithm.h"
#include "Headers/Selectors/RouletteWheelSelector.h"
#include "Headers/Utils/Benchmarks.h"


int main()
{
	using individual = ea::ArrayIndividual<100, bool, int>;
	using population = std::vector<individual>;

	population pop{};

	for (auto i = 0; i < 50; ++i)
	{
		pop.emplace(pop.end());
	}

	ea::EvolutionaryAlgorithm<population> ea{};

	ea.add_operator(ea::BitFlipMutation<population>{});
	ea.add_operator(ea::OnePtXOver<population>{});
	ea.add_mating_selector(ea::RouletteWheelSelector<population>{});
	ea.set_fitness([](auto & pop) -> void {
		for (auto && individual : pop)
		{
			auto sum = 0;

			for (auto && element : individual)
			{
				if (element)
				{
					++sum;
				}
			}

			individual.fitness = sum;
		}
	});

	ea.set_elitism(0.05);


	//for (int i = 0; i < 10000; ++i)
	//{
	//	pop = ea.evolve(pop);

	//	const auto best = std::max_element(pop.begin(), pop.end(), [](auto i1, auto i2) { return i1.fitness < i2.fitness; });

	//	if (i % 500 == 0)
	//	{
	//		std::cout << "gen: " << i << "; best fitness:" << best->fitness << std::endl;
	//	}
	//}

	for (auto i = 0; i < 3; ++i)
	{
		ea::benchmark_and_output(ea, pop, 5000);
	}

	return 0;
}
