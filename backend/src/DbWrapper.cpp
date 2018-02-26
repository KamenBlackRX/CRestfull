#include "../include/DbWrapper.h"

extern "C" {
/* Private Variables */
PGconn *conn;     // Connection Pointer
PGresult *res;    //Result point for sql query.
const char *info; // flags info
int nFields;
int i,
    j;
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
int CreateConnection(char *ip, char **FLAGS)
{

    char *info = (char *)calloc(50, sizeof(char *));

    info = "host=127.0.0.1 user=blank password=blank port=5432 dbname = blank"; //Defaut value

    if (FLAGS[0] != "port")
    {
        printf("We dont have a connection info. will try default values.");
        info = "dbname = postgres";
    }

    conn = PQconnectdb(info);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        clean_exit(conn);
        return -1;
    }

    free(info);

    return 0;
}

int DeleteConnection()
{
    /* close the connection to the database and cleanup */
    PQfinish(conn);
    return 0;
}

/**
     * Execute a non paginate query
     * @param char** type_sql  Path or query for exectuion
     * @return char** return the response of query.
     */
char **executeNonPaginateQuery(char *type_sql, const char* FLAG)
{
    // Get info conncetion and try to connect it
    char **info = (char **)calloc(10, 10 * sizeof(char));
    info[0] = "port";
    info[1] = "5432";
    info[2] = "db";
    info[3] = "teste";

    CreateConnection("localhost", info);

    char **retval;
    // If we got a null type_sql, run a default query.
    // TO-DO Implement a error message.
    if (type_sql == "" || type_sql == NULL)
    {

        /* Start a transaction block */
        res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
            PQclear(res);
            clean_exit(conn);
        }

        /*
        * Should PQclear PGresult whenever it is no longer needed to avoid memory
        * leaks
        */
        PQclear(res);

        /*
        * Fetch rows from pg_database, the system catalog of databases
        */
        res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
            PQclear(res);
            clean_exit(conn);
        }

        PQclear(res);

        res = PQexec(conn, "FETCH ALL in myportal");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
            PQclear(res);
            clean_exit(conn);
        }

        /* first, print out the attribute names */
        nFields = PQnfields(res);
        for (i = 0; i < nFields; i++)
            printf("%-15s", PQfname(res, i));
        printf("\n\n");

        /* next, print out the rows */
        for (i = 0; i < PQntuples(res); i++)
        {
            for (j = 0; j < nFields; j++)
            {
                printf("%-15s", PQgetvalue(res, i, j));
                retval[j] = PQgetvalue(res, i, j);
            }
            printf("\n");
        }

        PQclear(res);

        /* close the portal ... we don't bother to check for errors ... */
        res = PQexec(conn, "CLOSE myportal");
        PQclear(res);

        /* end the transaction */
        res = PQexec(conn, "END");
        PQclear(res);
    }
    else
    {
        ///Insert Routine.
        if(FLAG == "I")
        {
            // Check to see that the backend connection was successfully made
            if (PQstatus(conn) != CONNECTION_OK)
            {
                fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
                PQfinish(conn);
                exit(1);
            }

            res = PQexec(conn, type_sql);

            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "INSERT failed: %s", PQerrorMessage(conn));
                DeleteConnection();
            }

            PQclear(res);
        }

        //Update Routine.
        if(FLAG == "U")
        {
            //Execute sql and return his results.
            res = PQexec(conn, type_sql);

        }
    }

    DeleteConnection();
    return retval;

}

/**
     * Setup and run query in asyncmode.
     */
}