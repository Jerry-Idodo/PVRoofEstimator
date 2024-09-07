#include "pvsqlite.h"
#include <QDebug>


PVSQLite::PVSQLite(std::string& filename)
    :db_filename(filename), db_table_name("PV_TABLE")
{
    rc = sqlite3_open(filename.c_str() ,&db);
}

PVSQLite::~PVSQLite()
{
    sqlite3_close(db);
}

int PVSQLite::default_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    return 0;
}

int PVSQLite::fetch_callback(void* data, int NumCols, char **ColData, char **ColName) {
    std::vector<std::vector<std::string>>* Modules = static_cast<std::vector<std::vector<std::string>>*>(data);
    std::vector<std::string> row;
    for (int i = 0; i < NumCols; i++) {
        row.push_back(ColData[i] ? ColData[i] : "NULL");
    }
    Modules->push_back(row);
    return 0;
}

int PVSQLite::create_table()
{
    int rc;
    char sql[] = "CREATE TABLE IF NOT EXISTS PVTABLE "\
                 "( Id INTEGER PRIMARY KEY, "\
                 "Name CHAR(50) NOT NULL, "\
                 "Manufacturer CHAR(50), "\
                 "Length REAL NOT NULL, "\
                 "Width REAL NOT NULL, "\
                 "Rating REAL NOT NULL)";


    rc = sqlite3_exec(db, sql, default_callback, 0, &ErrMsg);
    if (rc != SQLITE_OK) {
        Msg = "SQL creation error: ";
        Msg += ErrMsg;
        free(ErrMsg);
    } else {
        Msg = "PV table created";
    }

    return rc;
}

int PVSQLite::insert_record()
{
    int rc;
    std::string sql;

    if (ModuleData.size() != 5)
        return -1;

    sql = "INSERT INTO PVTABLE (Name, Manufacturer, Length, Width, Rating) ";
    sql += "VALUES ( ";
    sql +="'" + ModuleData[0] + "', ";
    sql +="'" + ModuleData[1] + "', ";
    sql += ModuleData[2] + ", ";
    sql += ModuleData[3] + ", ";
    sql += ModuleData[4] + ")";

    qDebug() << sql;

    rc = sqlite3_exec(db, sql.c_str(), default_callback, 0, &ErrMsg);
    if (rc != SQLITE_OK) {
        Msg = "SQL insertion error: ";
        Msg += ErrMsg;
        free(ErrMsg);
    } else {
        Msg = "Module Info Inserted";
    }

    return rc;
}

int PVSQLite::fetch_records() {
    int rc;
    std::string sql = "SELECT * FROM PVTABLE;";
    Modules.clear();  // Clear previous data

    rc = sqlite3_exec(db, sql.c_str(), fetch_callback, &Modules, &ErrMsg);
    if (rc != SQLITE_OK) {
        Msg = "SQL fetch error: ";
        Msg += ErrMsg;
        free(ErrMsg);
    } else {
        Msg = "Records fetched successfully";
    }

    return rc;
}
