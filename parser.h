#ifndef __PARSER_H_LOADED___
#define __PARSER_H_LOADED___

#include <string>
#include <memory>
#include "command.h"


#include <boost/tokenizer.hpp>

class Parser
{
	public:
		using token_iterator = boost::token_iterator<boost::char_separator<char>, std::string::const_iterator, std::string>;

		Parser(const std::string &cmd);
		virtual ~Parser();
		virtual std::unique_ptr<Command> parse();

		static Tuple parse_tuple(token_iterator beg, token_iterator end);
	private:
		enum Token {
			INSERT,
			TRUNCATE,
			INTERSECT,
			SYMDIFF,
			NOOP
		};

		const boost::char_separator<char> sep{" \n"};

		boost::tokenizer<boost::char_separator<char>> tokens;
		token_iterator tok_cmd;

		Token get_token();
		Table &get_table();

		static std::map<std::string, Token> Tokens;
};

#endif
