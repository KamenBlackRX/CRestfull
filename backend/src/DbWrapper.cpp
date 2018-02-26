#include "../include/DbWrapper.h"

extern "C" {
/* Private Variables */
PGconn *conn;     // Connection Pointer
PGresult *res;    //Result point for sql query.
char *info; // flags info
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

    info = (char *)calloc(50, sizeof(char *));

    info = "host=127.0.0.1 user=blank password=blank port=5432 dbname = blank"; //Defaut value

    if (FLAGS[0] != "port")
    {
        printf("We dont have a connection info. will try default values.");
        info = "dbname = postgres";
    }

    printf("String Connection: %s\n", info);
    conn = PQconnectdb(info);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        clean_exit(conn);
        return -1;
    }

    if(info == NULL)
        free(info);

    return 0;
}

int DeleteConnection()
{
    /* close the connection to the database and cleanup */
    clean_exit(conn);
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
        printf("Proceding with null sql");
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
        printf("\nProceed with a given sql");
        ///Insert Routine.
        printf("\nDebug: FLAG -> %s", FLAG);
        if(FLAG[0] == 'I')
        {
            printf("\nAttempt to connect...");
            // Check to see that the backend connection was successfully made
            if (PQstatus(conn) != CONNECTION_OK)
            {
                fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
                DeleteConnection();
                exit(1);
            }
            else
            {
                printf("\nConnection to database sucessfull!");
            }

            printf("\nExecuting query:\n %s", type_sql);

            res = PQexec(conn, type_sql);
            char* status = PQresStatus(PQresultStatus(res));

            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "\nINSERT failed: %s", PQerrorMessage(conn));
                DeleteConnection();
                exit(-1);
            }
            else
            {
              printf("\nInsert was made with sucesss!");
              printf("\nQuery %s executed with status: %s\n ", type_sql, status);
            }

            PQclear(res);
            DeleteConnection();
            return &status;
        }

        //Update Routine.
        //TODO Need implementation
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