#ifndef __COMMAND_H_LOADED___
#define __COMMAND_H_LOADED___

#include <vector>
#include <string>

#include "schema.h"


class CommandResult
{
	public:
		using ResultTuple = std::tuple<int, std::string, std::string>;

		CommandResult();

		CommandResult(const std::string err);

		virtual ~CommandResult();

		bool isOk() const {
			return ok;
		}
		bool hasResult() const {
			return has_result;
		}

		Table getResult() const;

		virtual void print();

		void add_row(ResultTuple &row) {
			result.emplace_back(row);
			has_result = true;
		}

		std::string toString();
	private:
		bool ok;
		bool has_result;
		std::string error;
		std::vector<ResultTuple> result;
};

class Command 
{
	public:

		Command() {}
		virtual ~Command() {}
		virtual CommandResult run() = 0;
};


class NoopCommand : public Command
{
	public:
		NoopCommand() {}
		virtual ~NoopCommand() {}
		virtual CommandResult run() { return {}; }
};


class InsertCommand : public Command
{
	public:
		InsertCommand(Table &table, Tuple tuple);
		virtual ~InsertCommand();
		virtual CommandResult run();
	private:
		Table &table;
		Tuple tuple;
};

class TruncateCommand : public Command
{
	public:
		TruncateCommand(Table &table);
		virtual ~TruncateCommand();
		virtual CommandResult run();
	private:
		Table &table;
};

class IntersectCommand : public Command
{
	public:
		IntersectCommand(Table &table1, Table &table2);
		virtual ~IntersectCommand();
		virtual CommandResult run();
	private:
		Table &tableA;
		Table &tableB;
};	

class SymmDiffCommand: public Command
{
	public:
		SymmDiffCommand(Table &table1, Table &table2);
		virtual ~SymmDiffCommand();
		virtual CommandResult run();
	private:
		Table &tableA;
		Table &tableB;
};	

#endif