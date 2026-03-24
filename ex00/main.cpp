#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	BitcoinExchange btc;

	// Ouvrir le fichier d'entrée
	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	std::string line;
	// Ignorer la première ligne (headers)
	std::getline(inputFile, line);

	// Boucler sur les autres lignes
	while (std::getline(inputFile, line))
	{
		// Vérifier qu'il y a un " | "
		size_t pipePos = line.find(" | ");
		if (pipePos == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		// Extraire la date et la valeur
		std::string date = line.substr(0, pipePos);
		std::string valueStr = line.substr(pipePos + 3);

		// Valider la date
		if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		// Vérifier que ce sont des chiffres
		bool validDate = true;
		for (int i = 0; i < 10; i++)
		{
			if (i == 4 || i == 7)
				continue;
			if (!std::isdigit(date[i]))
			{
				validDate = false;
				break;
			}
		}
		if (!validDate)
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Valider la date (année 0000 est possible aussi)
		int year = std::stoi(date.substr(0, 4));
		int month = std::stoi(date.substr(5, 2));
		int day = std::stoi(date.substr(8, 2));

		if (month < 1 || month > 12 || day < 1 || day > 31)
		{
			std::cout << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Valider la valeur
		double value;
		try
		{
			value = std::stod(valueStr);
		}
		catch (const std::exception& e)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		// Vérifier que la valeur est positive
		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}

		// Vérifier que la valeur n'est pas trop grande
		if (value > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}

		// Obtenir le taux de change
		double rate = btc.getExchangeRate(date);
		if (rate < 0)
		{
			std::cout << "Error: date not found in database." << std::endl;
			continue;
		}

		// Calculer et afficher le résultat
		double result = value * rate;
		std::cout << date << " => " << value << " = " << std::fixed << std::setprecision(2) << result << std::endl;
	}

	inputFile.close();
	return 0;
}
