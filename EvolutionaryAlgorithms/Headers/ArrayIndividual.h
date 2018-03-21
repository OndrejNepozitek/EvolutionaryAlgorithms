#pragma once

template<std::size_t Size, typename TElement, typename TFitness>
class ArrayIndividual
{
	TElement elements[Size];
public:

	TFitness fitness;
	using fitness_type = TFitness;
	
	TElement operator[](const std::size_t  index) const
	{
		return elements[index];
	}

	TElement & operator[](const std::size_t  index)
	{
		return elements[index];
	}

	std::size_t size()
	{
		return Size;
	}

	auto begin()
	{
		return std::begin(elements);
	}

	auto end()
	{
		return std::end(elements);
	}
};
