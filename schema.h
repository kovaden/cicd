#ifndef __SCHEMA_H_LOADED___
#define __SCHEMA_H_LOADED___	

#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <cassert>

typedef std::pair<int, std::string> Tuple;

/* Thread-safe wrapper over the std::map */
class Table
{
public:
	typedef int key_t;
	typedef std::map<key_t, std::string> tuple_iterator;

	Table() {}

	auto begin() { return data.begin();}
	auto end() { return data.end();}
	auto rbegin() {return data.rbegin(); }
	auto rend() {return data.rend();}
	
	auto find(key_t k) { return data.find(k); }
	void insert(Tuple t) {		
		assert (!data_lock.try_lock() && "Lock should be already taken");
		data.insert(t);		
	}

	void truncate() {
		auto lock = lock_for_writing();
		data.erase(data.begin(), data.end());
	}

	using mutex_type = std::shared_mutex;
	using read_lock  = std::shared_lock<mutex_type>;
	using write_lock = std::unique_lock<mutex_type>;

	read_lock lock_for_reading() { return read_lock(data_lock); }
	write_lock lock_for_writing() { return write_lock(data_lock); }
private:
	typedef std::map<int, std::string> table_t;		

	table_t data;

	mutable mutex_type data_lock;
};

class TableRegistry
{
	public:
		virtual ~TableRegistry();
		static Table &getTable(const std::string &name);
	private:
		// Only one instance, not expected to be created by user.
		TableRegistry();

		static TableRegistry instance;
		std::map<std::string, Table&> tables;
};

#endif