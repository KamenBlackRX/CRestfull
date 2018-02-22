#include "../include/DbWrapper.h"
#include <stdlib.h>
extern "C"
{
    /**
     * Create connection and store his status response.
     * @param char** ip  Address for connection
     * @param char** FLAGS Default flags for connection
     */
    int connection(char** ip, char** FLAGS)
    {
        return 0;
    }

    /**
     * Execute a non paginate query
     * @param char** type_sql  Path or query for exectuion
     * @return char** return the response of query.
     */
    char** executeNonPaginateQuery(char** type_sql)
    {
        char* response = (char*)calloc(256,sizeof(char));
        return &response;
    }
}