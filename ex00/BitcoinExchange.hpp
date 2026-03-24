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
	private:
		std::map<std::string, double> _exchangeRates;
		void loadData(const std::string& filename);
		bool isValidDate(const std::string& date) const;
};

#endif