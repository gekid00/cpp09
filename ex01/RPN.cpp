#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN &src)
{
	*this = src;
}

RPN &RPN::operator=(const RPN &src)
{
	if (this != &src)
	{
		_stack = src._stack;
	}
	return (*this);
}

bool RPN::isNumber(const std::string &token)
{
	if (token.empty())
		return (false);
	for (size_t i = 0; i < token.length(); i++)
	{
		if (!isdigit(token[i]))
			return (false);
	}
	return (true);
}

bool RPN::isOperator(const std::string &token)
{
	return (token == "+" || token == "-" || token == "*" || token == "/");
}

void RPN::applyOperator(const std::string &op)
{
	if (_stack.size() < 2)
	{
		std::cerr << "Error: invalid expression" << std::endl;
		exit(1);
	}

	int b = _stack.top();
	_stack.pop();
	int a = _stack.top();
	_stack.pop();
	int result = 0;

	if (op == "+")
		result = a + b;
	else if (op == "-")
		result = a - b;
	else if (op == "*")
		result = a * b;
	else if (op == "/")
	{
		if (b == 0)
		{
			std::cerr << "Error: division by zero" << std::endl;
			exit(1);
		}
		result = a / b;
	}
	_stack.push(result);
}

int RPN::evaluate(const std::string &expression)
{
	std::istringstream iss(expression);
	std::string token;

	// Vider la stack avant d'évaluer
	while (!_stack.empty())
		_stack.pop();

	while (iss >> token)
	{
		if (isNumber(token))
		{
			int num = std::atoi(token.c_str());
			_stack.push(num);
		}
		else if (isOperator(token))
		{
			applyOperator(token);
		}
		else
		{
			std::cerr << "Error: invalid token '" << token << "'" << std::endl;
			exit(1);
		}
	}

	if (_stack.size() != 1)
	{
		std::cerr << "Error: invalid expression" << std::endl;
		exit(1);
	}

	return (_stack.top());
}
