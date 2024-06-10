#include "parser.h"
#include <boost/tokenizer.hpp>
#include <map>
#include <vector>
#include <string>


std::map<std::string, Parser::Token> Parser::Tokens{
	{"INSERT", INSERT},
	{"TRUNCATE", TRUNCATE},
	{"INTERSECTION", INTERSECT},
	{"SYMMETRIC_DIFFERENCE", SYMDIFF},
	{"NOOP", NOOP}
};

Parser::Parser(const std::string &cmd) : tokens(cmd, sep)
{
	tok_cmd = tokens.begin();
}

Parser::~Parser()
{
}

Parser::Token Parser::get_token()
{
	if (tok_cmd == tokens.end())
	{
		return NOOP;
	}

	auto cmd_type = Tokens.find(*tok_cmd);
	if (cmd_type == Tokens.end())
	{
		throw std::runtime_error("Unknown command: " + *tok_cmd);
	}
	tok_cmd ++;

	return cmd_type->second;
}

Table &Parser::get_table()
{
	if (tok_cmd == tokens.end())
	{
		throw std::runtime_error("Table name expected");
	}

	auto &table = TableRegistry::getTable(*tok_cmd);
	tok_cmd ++;
	return table;
}

std::unique_ptr<Command> Parser::parse()
{	
	if (tok_cmd == tokens.end())
	{
		return std::make_unique<NoopCommand>();
	}	

	auto cmd_type = get_token();

	switch (cmd_type)
	{
	case INSERT:
	{
		Table &table = get_table();
		Tuple tuple = parse_tuple(tok_cmd, tokens.end());
		return std::make_unique<InsertCommand>(table, tuple);
	}
		
	case TRUNCATE:
	{	
		Table &table = get_table();
		return std::make_unique<TruncateCommand>(table);
	}
		
	case INTERSECT:
	{
		auto &table1 = TableRegistry::getTable("A");
		auto &table2 = TableRegistry::getTable("B");
		return std::make_unique<IntersectCommand>(table1, table2);
	}
	case SYMDIFF:
	{
		auto &table1 = TableRegistry::getTable("A");
		auto &table2 = TableRegistry::getTable("B");
		return std::make_unique<SymmDiffCommand>(table1, table2);	
	}	
	default:
		break;
	}

	return std::make_unique<NoopCommand>();
}

Tuple Parser::parse_tuple(token_iterator beg, token_iterator end)
{
	int id = std::stoi(*beg);
	beg ++;
	if (beg == end) {
		throw std::runtime_error("Tuple is not complete");
	}

	std::string value = *beg;
	return {id, value};
}
