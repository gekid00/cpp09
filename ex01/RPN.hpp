#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <stack>

class RPN
{
	public:
		RPN();
		~RPN();
		RPN(const RPN &src);
		RPN &operator=(const RPN &src);

		int evaluate(const std::string &expression);

	private:
		std::stack<int> _stack;

		bool isOperator(const std::string &token);
		bool isNumber(const std::string &token);
		void applyOperator(const std::string &op);
};

#endif
