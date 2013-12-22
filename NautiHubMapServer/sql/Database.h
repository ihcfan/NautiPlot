#ifndef __SERVER_SQLMANAGER_HPP__
#define __SERVER_SQLMANAGER_HPP__

#include <string>
#include "sqlite3.h"
#include <map>
#include <vector>

namespace sql {
	
	typedef std::map<std::string, std::string> Dictionary;
	typedef std::vector<Dictionary> Collection;

	class Database
	{
		public:

			explicit Database();
			~Database();
			bool Startup(const std::string& database);
			

			Collection Query(const std::string& predicate);

		private:
            sqlite3 *sql_;
            Collection results_;
            int row_;
        

			Database(const Database& rhs) = delete;
			Database(Database&& rhs) = delete;
			Database& operator=(const Database& rhs) = delete;
			Database& operator=(Database&& rhs) = delete;
            void clearResults() { row_=-1; results_.clear(); }
        
            static int sqlite3Callback(void *me, int columns, char **columnData, char **names);
	};

}

#endif
