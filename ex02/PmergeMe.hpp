#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <sys/time.h>

class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		void sort(int argc, char *argv[]);

	private:
		std::vector<int> _vector;
		std::deque<int> _deque;

		// Parsing et validation
		bool parseArguments(int argc, char *argv[]);
		bool isValidNumber(const std::string& str) const;

		// Tri avec vector
		void mergeInsertSort(std::vector<int>& arr);
		void binaryInsert(std::vector<int>& arr, int value);

		// Tri avec deque
		void mergeInsertSort(std::deque<int>& arr);
		void binaryInsert(std::deque<int>& arr, int value);

		// Utilitaires
		std::vector<int> jacobsthal(int size) const;
		double getTime() const;

		template <typename T>
		void display(const T& arr, const std::string& label) const;
};

template <typename T>
void PmergeMe::display(const T& arr, const std::string& label) const
{
	std::cout << label;
	for (typename T::const_iterator it = arr.begin(); it != arr.end(); ++it)
	{
		if (it != arr.begin())
			std::cout << " ";
		std::cout << *it;
	}
	std::cout << std::endl;
}

#endif
