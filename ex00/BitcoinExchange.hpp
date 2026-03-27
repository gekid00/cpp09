#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
		double getExchangeRate(const std::string& date) const;
		void processLine(const std::string& line);
	private:
		std::map<std::string, double> _exchangeRates;
		void loadData(const std::string& filename);
		bool isValidDate(const std::string& date) const;
		std::string trim(const std::string& str) const;
		bool parseDouble(const std::string& str, double& result) const;
};

#endif