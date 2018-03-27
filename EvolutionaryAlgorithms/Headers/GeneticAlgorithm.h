#pragma once

namespace ea
{
	template<typename TPopulation>
	void ga_fitness(TPopulation & population)
	{
		for (auto && individual : population)
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
	}
}