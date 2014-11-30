#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <prepared_statement.h>

using namespace sql;
using namespace std;

int main(void) {

    cout << "Running " << endl;
    
    try {

        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;



        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306","root","root");

        con->setSchema("test");

        stmt = con->createStatement();

        cout << "CREATE TABLE test " << endl;
        stmt->execute("DROP TABLE IF EXISTS test");
        stmt->execute("CREATE TABLE test(id INT(11) NOT NULL AUTO_INCREMENT, code VARCHAR(255) COLLATE utf8_unicode_ci DEFAULT NULL, PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1");

        pstmt = con->prepareStatement("INSERT INTO test(code) VALUES (?)");
        for (int i = 1; i <= 10; i++) {
            // pstmt->setInt(1, i);
            pstmt->setString(1, "xxx");
            pstmt->executeUpdate();
        }
        delete pstmt;

        res = stmt->executeQuery("SELECT id, code FROM test ORDER BY id ASC");
        
        int i = 0;
        while (res->next()) {
            // cout << "\tid: " << res->getInt("id") << "\t code: " << res->getString("code") << endl;
            cout << "\t " << ++i << " id: " << res->getInt("id") << "\t code: " << res->getString("code") << endl;
            // cout << "\t" << ++i << " \tcode: " << res->getInt("id") << endl;
        }
    
        delete res;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }


    return 0;
}

