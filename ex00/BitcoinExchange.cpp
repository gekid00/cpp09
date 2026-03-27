#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>

BitcoinExchange::BitcoinExchange()
{
	loadData("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_exchangeRates = other._exchangeRates;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::loadData(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file " << filename << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;
		
		std::string date = line.substr(0, commaPos);
		std::string rateStr = line.substr(commaPos + 1);
		if (!isValidDate(date))
			continue;
		std::stringstream ss(rateStr);
		double rate;
		if (!(ss >> rate) || rate < 0)
			continue;
		_exchangeRates[date] = rate;
	}
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	if (!std::isdigit(date[0]) || !std::isdigit(date[1]) || !std::isdigit(date[2]) || !std::isdigit(date[3]) ||
		!std::isdigit(date[5]) || !std::isdigit(date[6]) ||
		!std::isdigit(date[8]) || !std::isdigit(date[9]))
		return false;
	
	std::stringstream ss1(date.substr(0, 4));
	std::stringstream ss2(date.substr(5, 2));
	std::stringstream ss3(date.substr(8, 2));
	int year, month, day;
	ss1 >> year;
	ss2 >> month;
	ss3 >> day;
	
	if (month < 1 || month > 12)
		return false;
	// Tableau des jours par mois
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	// Vérifier année bissextile
	bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (isLeapYear)
		daysInMonth[1] = 29;
	if (day < 1 || day > daysInMonth[month - 1])
		return false;
	
	return true;
}

double BitcoinExchange::getExchangeRate(const std::string& date) const
{
	if (!isValidDate(date))
		return -1;
	
	std::map<std::string, double>::const_iterator it = _exchangeRates.lower_bound(date);
	
	// Si la date est exactement en base, retourne le taux
	if (it != _exchangeRates.end() && it->first == date)
		return it->second;
	
	// Sinon, prend la date précédente (la plus proche avant)
	if (it == _exchangeRates.begin())
		return -1;  // Pas de date avant celle-ci
	
	--it;
	return it->second;
}

void BitcoinExchange::processLine(const std::string& line)
{
	// Étape 1: Cherche le séparateur " | "
	size_t pipePos = line.find(" | ");
	if (pipePos == std::string::npos)
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}

	// Étape 2: Extrait et nettoie la date et la valeur
	std::string date = trim(line.substr(0, pipePos));
	std::string valueStr = trim(line.substr(pipePos + 3));

	// Étape 3: Valide la date
	if (!isValidDate(date))
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}

	// Étape 4: Parse et valide la valeur
	double value;
	if (!parseDouble(valueStr, value))
	{
		std::cout << "Error: bad input => " << line << std::endl;
		return;
	}

	// Étape 5: Vérifie que la valeur est positive
	if (value < 0 || (value == 0 && valueStr[0] == '-'))
	{
		std::cout << "Error: not a positive number." << std::endl;
		return;
	}

	// Étape 6: Vérifie que la valeur n'est pas trop grande
	if (value > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return;
	}

	// Étape 7: Récupère le taux de change
	double rate = getExchangeRate(date);
	if (rate < 0)
	{
		std::cout << "Error: date not found in database." << std::endl;
		return;
	}

	// Étape 8: Calcule et affiche le résultat
	double result = value * rate;
	std::cout << date << " => " << value << " = " << std::fixed << std::setprecision(2) << result << std::endl;
}

// Supprime les espaces/tabs au début et à la fin d'une chaîne
std::string BitcoinExchange::trim(const std::string& str) const
{
	size_t start = 0;
	size_t end = str.length();
	
	while (start < end && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t'))
		end--;
	
	return str.substr(start, end - start);
}

// Essaie de convertir une string en double
// Retourne true si succès, false si erreur
bool BitcoinExchange::parseDouble(const std::string& str, double& result) const
{
	// Utilise stringstream en interne pour convertir
	std::stringstream ss(str);
	if (!(ss >> result))
		return false;
	
	// Vérifie qu'il n'y a rien après le nombre
	char extra;
	if (ss >> extra)
		return false;
	
	return true;
}