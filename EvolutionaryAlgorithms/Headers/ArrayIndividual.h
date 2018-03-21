#pragma once
#include <array>

namespace ea
{
	template<std::size_t Size, typename TElement, typename TFitness>
	class ArrayIndividual
	{
		std::array<TElement, Size> elements_;
	public:

		TFitness fitness;
		using fitness_type = TFitness;

		TElement operator[](const std::size_t index) const;

		TElement& operator[](const std::size_t index);

		static std::size_t size();

		typename std::array<TElement, Size>::iterator begin();

		typename std::array<TElement, Size>::iterator end();
	};

	template <std::size_t Size, typename TElement, typename TFitness>
	TElement ArrayIndividual<Size, TElement, TFitness>::operator[](const std::size_t index) const
	{
		return elements_[index];
	}

	template <std::size_t Size, typename TElement, typename TFitness>
	TElement& ArrayIndividual<Size, TElement, TFitness>::operator[](const std::size_t index)
	{
		return elements_[index];
	}

	template <std::size_t Size, typename TElement, typename TFitness>
	std::size_t ArrayIndividual<Size, TElement, TFitness>::size()
	{
		return Size;
	}

	template <std::size_t Size, typename TElement, typename TFitness>
	typename std::array<TElement, Size>::iterator ArrayIndividual<Size, TElement, TFitness>::begin()
	{
		return std::begin(elements_);
	}

	template <std::size_t Size, typename TElement, typename TFitness>
	typename std::array<TElement, Size>::iterator ArrayIndividual<Size, TElement, TFitness>::end()
	{
		return std::end(elements_);
	}
}