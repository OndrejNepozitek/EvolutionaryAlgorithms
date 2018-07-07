#ifndef UTILS_H
#define UTILS_H
#include <random>

namespace ea
{
	/**
	 * \brief Returns the mersenne twister generator for the current thread.
	 * \return 
	 */
	inline std::mt19937 & rng_gen()
	{
		thread_local std::random_device rd;
		thread_local auto generator = std::mt19937(rd());

		return generator;
	}
}
#endif // UTILS_H
