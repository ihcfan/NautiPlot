#include <iostream>
#include <stdio.h>
#include <sstream>
#include "Database.h"

namespace sql {

	Database::Database()
		: sql_(NULL)
	{
	
	}

	Database::~Database()
	{
		if (sql_) {
			sqlite3_close(sql_);
			sql_ = NULL;
			std::cout << "SQL database disconnected" << std::endl;
		}

        sqlite3_close(sql_);
	}

	Collection Database::Query(const std::string& predicate)
	{
		std::cout << "Do query" << std::endl;
        clearResults();
		if (sqlite3_exec(sql_, predicate.c_str(), sqlite3Callback, this, nullptr) == 0) {
			std::cout << "Did query 1" << std::endl;

			std::vector<std::string> fields;

		}
		else {
			std::cerr << "Query error" << std::endl;
		}
		return std::move(results_);
	}

	bool Database::Startup(const std::string& database)
	{
        
        int result = sqlite3_open_v2( database.c_str(), &sql_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

        if (result == SQLITE_OK) {
            std::cout << "SQL database: \"" << database << "\" connected" << std::endl;
            return true;
        }

        std::cout << "SQL database: \"" << database << "\" NOT connected!" << std::endl;
		return false;
	}

    int sql::Database::sqlite3Callback(void *it, int columns, char **columnData, char **names) {
        Database& me = *static_cast<Database *>(it);
        Dictionary new_dictionary;
        
        for (int i=0; i<columns; i++) {
            new_dictionary[names[i]]=(columnData[i])?columnData[i]:"null";
        }
        me.row_++;
        me.results_[me.row_]=new_dictionary;
        
        
        return 0;
        
    }
    
} // namespace sql
