#include "schema.h"
#include <stdexcept>

using namespace std;

TableRegistry TableRegistry::instance;

TableRegistry::TableRegistry()
{	
	static Table tableA;
	static Table tableB;

	tables.emplace("A", tableA);
	tables.emplace("B", tableB);
}

TableRegistry::~TableRegistry()
{
}

Table &TableRegistry::getTable(const std::string &name)
{
	auto &tables = instance.tables;

	auto f = tables.find(name);
	if (f == tables.end())
	{
		throw std::runtime_error("Table " + name + " not found");
	}

	return f->second;
}

