#include <stdio.h>
#include <stdlib.h>

#pragma warning "This wrapper need implement in C code.\nNeed generate obj for linkage"

extern "C"
{
    /**
     * Create connection and store his status response.
     * @param char** ip  Address for connection
     * @param char** FLAGS Default flags for connection
     */
    int connection(char**, char**);

    /**
     * Execute a non paginate query
     * @param char** type_sql  Path or query for exectuion
     * @return char** return the response of query.
     */
    char** executeNonPaginateQuery(char**);


}