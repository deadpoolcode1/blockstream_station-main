#include <mysql/mysql.h>
#include <stdio.h>
const char *server = "localhost";
const char *user = "root";
const char *password = "12345678";
const char *database = "blockstreams";

void database_on_get(void)
{
        MYSQL *conn;
        MYSQL_ROW last_row;
        conn = mysql_init(NULL);
        MYSQL_FIELD* column;
        MYSQL_ROW record;

        /* Connect to database */
        if (!mysql_real_connect(conn, server,
                                user, password, database, 0, NULL, 0))
        {
                fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
        }

        if (mysql_query(conn, "SELECT * FROM `blockstreamst`")) {
                printf("Unable to query table blockstreamst\n");
                mysql_close(conn);
                exit(1);
        }
        
        MYSQL_RES* rs = mysql_store_result(conn);
        if (rs == NULL) {
                printf("Unable to compile SQL statement\n");
                mysql_close(conn);
                exit(1);
        }
        while (column = mysql_fetch_field(rs)) {
                printf("%s ", column->name);
        }
        printf("\n");
        while (record = mysql_fetch_row(rs)) {
                last_row = record;
        }
        printf("%s %s\n", last_row[0], last_row[1]);
        /* close connection */
        mysql_close(conn);
}

void database_on_set(int temp, int humid) {
        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW row;

        conn = mysql_init(NULL);

        /* Connect to database */
        if (!mysql_real_connect(conn, server,
                                user, password, database, 0, NULL, 0))
        {
                fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
        }

        char cmd_query[128] = {0x00};
        snprintf(cmd_query, sizeof(cmd_query), "Insert into `blockstreamst` values(%d,%d)", temp, humid);
        if (mysql_query(conn, cmd_query)) {
                printf("Unable to insert data into `blockstreamst` table\n");
                mysql_close(conn);
                exit(1);
        }

        /* close connection */
        mysql_close(conn);
}
