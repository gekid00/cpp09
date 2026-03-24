#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

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

BitcoinExchange::loadData(const std::string& filename)
{
	std::ifstream file(filename);
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
		try
		{
			double rate = std::stod(rateStr);
			if (rate < 0)
				continue;
			_exchangeRates[date] = rate;
		}
		catch (const std::exception& e)
		{
			continue;
		}
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
	
	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));
	
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
	if (it == _exchangeRates.begin())
		return -1;
	if (it != _exchangeRates.end() &&
	 it->first == date)
		return it->second;
	--it;
	return it->second;
}