#include "../include/DbWrapper.h"

extern "C"
{

    /* Exit connection with a clean exit */
    static void clean_exit(PGconn *conn)
    {
        PQfinish(conn);
    }

    /**
     * Create connection and store his status response.
     * @param char** ip  Address for connection
     * @param char** FLAGS Default flags for connection
     */
    int connection(char* ip, char** FLAGS)
    {

    }

    /**
     * Execute a non paginate query
     * @param char** type_sql  Path or query for exectuion
     * @return char** return the response of query.
     */
    char* executeNonPaginateQuery(char* type_sql)
    {
        char* response = (char*)malloc(sizeof(char));
        return response;
    }
}