#ifndef PVSQLITE_H
#define PVSQLITE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class PVSQLite
{
public:
    sqlite3 *db;                        // Database Connection Pointer
    int rc;                             // SQLite Return Code
    char *ErrMsg;                       // sqlite3_exec()'s Error Message
    std::string Msg;                    // Message for GUI
    std::string db_filename;
    std::string db_table_name;
    std::vector<std::string> ModuleData;    // Data to be inserted or fetched
    std::vector<std::vector<std::string>> Modules;       // List of modules in Database

    PVSQLite(std::string& filename);        // Opens new conection

    int create_table();
    int insert_record();
    int fetch_records();

    //SQL Callback Functions
    static int default_callback(void* NotUsed, int argc, char** argv, char** azColName);
    static int fetch_callback(void* data, int NumCols, char **ColData, char **ColName);

    ~PVSQLite();
};

#endif // PVSQLITE_H
