#include <array>
#include <chrono>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
enum class SQLQuery : int
{
    query1 = 0,
    query2 = 1,
    query3 = 2 ,
    query4 = 3,
    query5 = 4,
    query6 = 5,
    query7 = 6,
    query8 = 7,
};

std::string toSqlString(SQLQuery query, std::string type)
{
    if(type == "0") {
        switch (query) {
            case SQLQuery::query1: return  "select * from lineitem LIMIT 1";
            case SQLQuery::query2 : return "select * from lineitem LIMIT 100";
            case SQLQuery::query3 : return  "select * from lineitem LIMIT 1000";
            case SQLQuery::query4 : return "select * from lineitem LIMIT 10000";
            case SQLQuery::query5 : return "select * from lineitem LIMIT 100000";
            case SQLQuery::query6 : return "select * from lineitem LIMIT 1000000";
            case SQLQuery::query7 : return "select * from lineitem LIMIT 10000000";
            case SQLQuery::query8 : return "select * from lineitem LIMIT 100000000";
        };

    } else if (type=="1"){
        switch (query) {
            case SQLQuery::query1: return  "query1";
            case SQLQuery::query2 : return "query2";
            case SQLQuery::query3 : return  "query3";
            case SQLQuery::query4 : return "query4";
            case SQLQuery::query5 : return "query5";
        
            };

    }
    return "0";
    
    

}

int toIteration(SQLQuery query)
{
     switch (query) {
     case SQLQuery::query1 : return 10000 ;
     case SQLQuery::query2 : return (100000 / 100) ;
     case SQLQuery::query3 : return (100000 / 1000) ;
     case SQLQuery::query4: return  (100000 / 10000) ;
     case SQLQuery::query5 : return 10 ;
     case SQLQuery::query6 : return  5 ;
     case SQLQuery::query7 : return 2;
     case SQLQuery::query8 : return 1;

    };

    return 0;
}



std::pair<int, std::pair<size_t, duration<double, std::milli>>> execute(std::string sql, SQLHSTMT& hStmt,SQLHDBC hDbc, size_t iteration);
int main(int argc, char* argv[]) {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <dsl> <type>" << std::endl;
        return 1;
    }
    std::string dsl = std::string(argv[1]);
    std::string type = std::string(argv[2]);
    if(type != "0" && type != "1") {
        std::cerr << "type must be 0 or 1" << std::endl;
        return 1;
    }

    // Initialize environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to allocate environment handle." << std::endl;
        return 1;
    }

    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to set environment attribute." << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }

    // Allocate connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to allocate connection handle." << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }

    // Connect to the DSN (already set up in odbc.ini)
    ret = SQLConnect(hDbc, (SQLCHAR*)dsl.c_str(), SQL_NTS, nullptr, 0, nullptr, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to connect to the data source." << std::endl;
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return 1;
    }
    if(type == "0") {
        std::array<SQLQuery, 8> queries{SQLQuery::query1, SQLQuery::query2, SQLQuery::query3, SQLQuery::query4, SQLQuery::query5, SQLQuery::query6, SQLQuery::query7, SQLQuery::query8};
        for (auto query : queries)
            {

                auto t1 = high_resolution_clock::now();
                auto suc = execute(toSqlString(query, type), hStmt, hDbc, toIteration(query));
                auto t2 = high_resolution_clock::now();
                if (suc.first != 0)
                {
                    std::cerr << "Failed to execute query." << std::endl;
                    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
                    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
                    return 1;
                };
                std::cout << "Executed: " << static_cast<int>(query) << " " << toIteration(query) << " times with complete time: " << suc.second.second.count() <<  " size: "  << suc.second.first<< std::endl;
            }
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    } else if(type == "1") {
        std::array<SQLQuery, 8> queries{SQLQuery::query1, SQLQuery::query2, SQLQuery::query3, SQLQuery::query4, SQLQuery::query5, /*SQLQuery::query6, SQLQuery::query7, SQLQuery::query8*/};
        for (auto query : queries)
            {

                auto t1 = high_resolution_clock::now();
                auto suc = execute(toSqlString(query, type), hStmt, hDbc, toIteration(query));
                auto t2 = high_resolution_clock::now();
                if (suc.first != 0)
                {
                    std::cerr << "Failed to execute query." << std::endl;
                    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
                    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
                    return 1;
                };
                std::cout << "Executed: " << static_cast<int>(query) << " " << toIteration(query) << " times with complete time: " << suc.second.second.count() <<  " size: "  << suc.second.first<< std::endl;
            }
            SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
            SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
    
    





    return 0;
}

std::pair<int, std::pair<size_t, duration<double, std::milli>>> execute(std::string sqlString, SQLHSTMT& hStmt, SQLHDBC hDbc, size_t iteration)
{
    duration<double, std::milli> durationComplete{0};
    size_t size = 0;

    for (auto i = 0; i < iteration; i++)
    {
        size = 0;
        auto t1 = high_resolution_clock::now();
        SQLRETURN ret;
        // Allocate statement handle
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "Failed to allocate statement handle." << std::endl;

            return {1,{0, duration<double, std::milli>(0)}};
        }

        SQLCHAR* sql = (SQLCHAR*)(sqlString.c_str());
        ret = SQLPrepare(hStmt, sql, SQL_NTS);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "Failed to prepare the statement." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return {1,{0, duration<double, std::milli>(0)}};
        }

        ret = SQLExecute(hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "Failed to execute the statement." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return {1,{0, duration<double, std::milli>(0)}};
        }
        SQLINTEGER id;


        SQLBindCol(hStmt, 1, SQL_C_LONG, &id, sizeof(id), nullptr);

        ret = SQLFetch(hStmt);
        while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            ret = SQLFetch(hStmt);  // Fetch the next row
            size += 1;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        auto t2 = high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */

        duration<double, std::milli> ms_double = t2 - t1;
        durationComplete+=(ms_double);
    }
    return {0, {size, durationComplete}};




}