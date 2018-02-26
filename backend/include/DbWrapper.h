#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "/usr/include/postgresql/libpq-fe.h"

#pragma warning "This wrapper need implement in C code.\nNeed generate obj for linkage"

extern "C"
{


    /**
     * Exit and close connection in free thread way.
    */
    static void clean_exit(PGconn *conn);

    /**
     * Create connection and store his status response.
     * @param char** ip  Address for connection
     * @param char** FLAGS Default flags for connection
    */
    int CreateConnection(char*, char**);

    /**
     * Delete and close a active connection.
     * @return  A code for sucessfull or failed connection.
    */
    int DeleteConnection();

    /**
     * Execute a non paginate query
     * @param char** type_sql  Path or query for exectuion
     * @return char** return the response of query.
    */
    char** executeNonPaginateQuery(char*,const char*);

}