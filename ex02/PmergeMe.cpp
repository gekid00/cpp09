#include "PmergeMe.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other)
	: _vector(other._vector), _deque(other._deque) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_vector = other._vector;
		_deque = other._deque;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::sort(int argc, char *argv[])
{
	if (!parseArguments(argc, argv))
	{
		std::cerr << "Error" << std::endl;
		return;
	}

	// Affiche avant le tri
	display(_vector, "Before: ");

	// Tri avec vector
	std::vector<int> vecCopy = _vector;
	double startVec = getTime();
	mergeInsertSort(vecCopy);
	double endVec = getTime();

	// Tri avec deque
	std::deque<int> dqCopy = _deque;
	double startDq = getTime();
	mergeInsertSort(dqCopy);
	double endDq = getTime();

	// Affiche après le tri
	display(vecCopy, "After: ");

	// Affiche les temps
	std::cout << "Time to process a range of " << _vector.size()
		<< " elements with std::vector : " << (endVec - startVec) << " us" << std::endl;
	std::cout << "Time to process a range of " << _deque.size()
		<< " elements with std::deque : " << (endDq - startDq) << " us" << std::endl;
}

bool PmergeMe::parseArguments(int argc, char *argv[])
{
	if (argc < 2)
		return false;

	// Pour chaque argument
	for (int i = 1; i < argc; i++)
	{
		if (!isValidNumber(argv[i]))
			return false;

		std::stringstream ss(argv[i]);
		int num;
		ss >> num;

		// Pas de négatifs
		if (num < 0)
			return false;

		_vector.push_back(num);
		_deque.push_back(num);
	}

	return true;
}

bool PmergeMe::isValidNumber(const std::string& str) const
{
	if (str.empty())
		return false;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
			return false;
	}

	return true;
}

// =============================================================================
// FORD-JOHNSON POUR VECTOR
// =============================================================================

/*
** Algorithme Ford-Johnson:
** 1. On regroupe les éléments en paires
** 2. On trie les "gagnants" (le plus grand de chaque paire) récursivement
** 3. On construit une chaîne principale avec les gagnants
** 4. On insère les "perdants" (le plus petit de chaque paire)
**    en utilisant la séquence de Jacobsthal pour optimiser
** 5. On insère le dernier élément s'il y en a un (impair)
*/

void PmergeMe::mergeInsertSort(std::vector<int>& arr)
{
	// Cas de base
	if (arr.size() <= 1)
		return;
	if (arr.size() == 2)
	{
		if (arr[0] > arr[1])
			std::swap(arr[0], arr[1]);
		return;
	}

	// ETAPE 1: Créer les paires (gagnant, perdant)
	std::vector<std::pair<int, int> > pairs;
	int straggler = -1;
	bool hasStraggler = false;

	for (size_t i = 0; i + 1 < arr.size(); i += 2)
	{
		int a = arr[i];
		int b = arr[i + 1];
		// (max, min) -> (gagnant, perdant)
		pairs.push_back(std::make_pair(std::max(a, b), std::min(a, b)));
	}

	// S'il y a un nombre impair d'éléments
	if (arr.size() % 2 == 1)
	{
		straggler = arr[arr.size() - 1];
		hasStraggler = true;
	}

	// ETAPE 2: Trier les gagnants récursivement
	std::vector<int> winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	mergeInsertSort(winners);

	// ETAPE 3 & 4: Construire le résultat et insérer les perdants
	arr.clear();
	arr.push_back(winners[0]); // Premier gagnant

	// Récupérer les perdants
	std::vector<int> losers;
	for (size_t i = 0; i < pairs.size(); i++)
		losers.push_back(pairs[i].second);

	// Générer la séquence de Jacobsthal
	std::vector<int> jacobOrder = jacobsthal(losers.size());

	// Insérer les perdants dans l'ordre de Jacobsthal
	std::vector<bool> inserted(losers.size(), false);
	for (size_t i = 0; i < jacobOrder.size() && i < losers.size(); i++)
	{
		int idx = jacobOrder[i];
		if (idx < (int)losers.size() && !inserted[idx])
		{
			binaryInsert(arr, losers[idx]);
			inserted[idx] = true;
		}
	}

	// Insérer les perdants restants
	for (size_t i = 0; i < losers.size(); i++)
	{
		if (!inserted[i])
			binaryInsert(arr, losers[i]);
	}

	// ETAPE 5: Insérer le straggler s'il existe
	if (hasStraggler)
		binaryInsert(arr, straggler);

	// Insérer les gagnants restants
	for (size_t i = 1; i < winners.size(); i++)
		binaryInsert(arr, winners[i]);
}

// Insérer une valeur à la bonne position
void PmergeMe::binaryInsert(std::vector<int>& arr, int value)
{
	// Chercher la position avec binary search
	std::vector<int>::iterator pos = std::lower_bound(arr.begin(), arr.end(), value);
	arr.insert(pos, value);
}

// =============================================================================
// FORD-JOHNSON POUR DEQUE
// =============================================================================

void PmergeMe::mergeInsertSort(std::deque<int>& arr)
{
	// Cas de base
	if (arr.size() <= 1)
		return;
	if (arr.size() == 2)
	{
		if (arr[0] > arr[1])
			std::swap(arr[0], arr[1]);
		return;
	}

	// ETAPE 1: Créer les paires
	std::deque<std::pair<int, int> > pairs;
	int straggler = -1;
	bool hasStraggler = false;

	for (size_t i = 0; i + 1 < arr.size(); i += 2)
	{
		int a = arr[i];
		int b = arr[i + 1];
		pairs.push_back(std::make_pair(std::max(a, b), std::min(a, b)));
	}

	if (arr.size() % 2 == 1)
	{
		straggler = arr[arr.size() - 1];
		hasStraggler = true;
	}

	// ETAPE 2: Trier les gagnants
	std::deque<int> winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	mergeInsertSort(winners);

	// ETAPE 3 & 4: Construire le résultat
	arr.clear();
	arr.push_back(winners[0]);

	// Récupérer les perdants
	std::deque<int> losers;
	for (size_t i = 0; i < pairs.size(); i++)
		losers.push_back(pairs[i].second);

	// Générer la séquence de Jacobsthal
	std::vector<int> jacobOrder = jacobsthal(losers.size());

	// Insérer les perdants
	std::deque<bool> inserted(losers.size(), false);
	for (size_t i = 0; i < jacobOrder.size() && i < losers.size(); i++)
	{
		int idx = jacobOrder[i];
		if (idx < (int)losers.size() && !inserted[idx])
		{
			binaryInsert(arr, losers[idx]);
			inserted[idx] = true;
		}
	}

	// Insérer les perdants restants
	for (size_t i = 0; i < losers.size(); i++)
	{
		if (!inserted[i])
			binaryInsert(arr, losers[i]);
	}

	// Insérer le straggler
	if (hasStraggler)
		binaryInsert(arr, straggler);

	// Insérer les gagnants restants
	for (size_t i = 1; i < winners.size(); i++)
		binaryInsert(arr, winners[i]);
}

void PmergeMe::binaryInsert(std::deque<int>& arr, int value)
{
	// Chercher la position avec binary search
	std::deque<int>::iterator pos = std::lower_bound(arr.begin(), arr.end(), value);
	arr.insert(pos, value);
}

// =============================================================================
// JACOBSTHAL
// =============================================================================

/*
** Séquence de Jacobsthal: 0, 1, 1, 3, 5, 11, 21, 43, ...
** Formule: J(n) = J(n-1) + 2 * J(n-2)
**
** Cette séquence détermine l'ordre optimal pour insérer les perdants.
** Au lieu d'insérer linéairement (0, 1, 2, 3, ...),
** on utilise (0, 1, 1, 3, 5, 11, ...) pour réduire le nombre de comparaisons
*/

std::vector<int> PmergeMe::jacobsthal(int size) const
{
	std::vector<int> seq;

	if (size == 0)
		return seq;

	// Construire la séquence
	int j0 = 0;
	int j1 = 1;
	seq.push_back(j0);

	if (size > 0)
		seq.push_back(j1);

	// Générer les éléments suivants
	while ((int)seq.size() < size)
	{
		int next = j1 + 2 * j0;
		if (next >= size)
			break;
		seq.push_back(next);
		j0 = j1;
		j1 = next;
	}

	// Ajouter les indices restants
	for (int i = 0; i < size; i++)
	{
		if (std::find(seq.begin(), seq.end(), i) == seq.end())
			seq.push_back(i);
	}

	return seq;
}

double PmergeMe::getTime() const
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000.0 + tv.tv_usec;
}
