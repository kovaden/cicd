
#include "command.h"
#include "parser.h"

#include <iostream>
#include <sstream>

using namespace std;

InsertCommand::InsertCommand(Table &tbl, Tuple tpl) : table(tbl), tuple(tpl)
{
}

InsertCommand::~InsertCommand()
{
}

CommandResult InsertCommand::run()
{
	// Although it's enough to have read lock for find, it should be upgraded for insert
	// upgradable lock is not a part of standard library, so keep write lock
	auto lock = table.lock_for_writing();
	if (table.find(std::get<0>(tuple)) != table.end())
	{
		return CommandResult("duplicate " + std::to_string(std::get<0>(tuple)));
	}
	table.insert(tuple);
	return CommandResult();
}

TruncateCommand::TruncateCommand(Table &tbl) : table(tbl)
{
}

TruncateCommand::~TruncateCommand()
{
}

CommandResult TruncateCommand::run()
{
	table.truncate();
	return CommandResult();
}


IntersectCommand::IntersectCommand(Table &table1, Table &table2) : tableA(table1), tableB(table2)
{
}

IntersectCommand::~IntersectCommand()
{
}

CommandResult IntersectCommand::run()
{
	CommandResult result;
	auto lockA = tableA.lock_for_reading();
	auto lockB = tableB.lock_for_reading();

	for (auto &rowA : tableA)
	{
		for (auto &rowB : tableB)
		{
			if (rowA.first == rowB.first)
			{

				std::tuple<int, std::string, std::string> result_tuple = std::make_tuple(rowA.first, rowA.second, rowB.second);
				result.add_row(result_tuple);
			}
		}
	}

	return result;
}

SymmDiffCommand::SymmDiffCommand(Table &table1, Table &table2) : tableA(table1), tableB(table2)
{
}

SymmDiffCommand::~SymmDiffCommand()
{
}

CommandResult SymmDiffCommand::run()
{
	CommandResult result;

	auto lockA = tableA.lock_for_reading();
	auto lockB = tableB.lock_for_reading();

	int max_id = std::max(tableA.rbegin()->first, tableB.rbegin()->first);

	for (int id = 0; id <= max_id; id++)
	{
		auto rowA = tableA.find(id);
		auto rowB = tableB.find(id);

		if (rowA == tableA.end() && rowB != tableB.end())
		{
			CommandResult::ResultTuple result_tuple = std::make_tuple(id, "", rowB->second);
			result.add_row(result_tuple);
		}
		else if (rowA != tableA.end() && rowB == tableB.end())
		{
			CommandResult::ResultTuple result_tuple = std::make_tuple(id, rowA->second, "");
			result.add_row(result_tuple);
		}
	}

	return result;
}

CommandResult::CommandResult() : ok(true), has_result(false)
{
}

CommandResult::CommandResult(const std::string err) : ok(false), has_result(false), error(err)
{
}

CommandResult::~CommandResult()
{
}

void CommandResult::print()
{
	std::cout << toString();
}

std::string CommandResult::toString()
{
	stringstream ss;
	if (!ok)
	{
		ss << "ERR " << error << std::endl;
		return ss.str();
	}

	if (has_result)
	{
		for (auto &row : result)
		{
			ss << std::get<0>(row) << "," << std::get<1>(row) << "," << std::get<2>(row) << std::endl;
		}
	}

	ss << "OK" << std::endl;

	return ss.str();
}
