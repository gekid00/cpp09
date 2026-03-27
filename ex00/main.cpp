#include "BitcoinExchange.hpp"
#include <fstream>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}
	BitcoinExchange btc;
	
	// Ouvre le fichier d'entrée (passé en argument)
	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	std::string line;
	// Lit la première ligne
	std::getline(inputFile, line);
	
	// Si c'est le header "date | value", le saute
	if (line != "date | value")
		btc.processLine(line);
	
	// Traite les autres lignes
	while (std::getline(inputFile, line))
		btc.processLine(line);

	inputFile.close();
	return 0;
}
