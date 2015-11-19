
#include "thirdparty/mysql++/mysql++.h"
#include <iostream>
#include <unistd.h>
#include <unistd.h>

using namespace std;
using namespace mysqlpp;


int main(int argc, char *argv[]) {
  Connection  cnn ("test", "10.66.163.167", "root",
      "guo123!@#", 3306);

  try {
    while (true) {
      if (!cnn.connected()) {
        cout << "disconnect, need reconnect!" << endl;
      } else {
        cout << "connected already" << endl;
      }

      Query query(&cnn, true);


      // delete
      if (query.exec("delete from test")) {
        // if (query.exec("delete from invalid_table")) {
        cout << "delete success!" << endl;
      } else {
        cout << "delete failed!" << endl;
      }

      // insert
      query.exec("insert into test set name='user1', age=1");
      query.exec("insert into test set name='user2'");
      // query.exec("insert into test set name='user2'");

      // select
      Query select_query = cnn.query("select * from test");
      // store result
      StoreQueryResult res = select_query.store();
      for (size_t i = 0; i < res.num_rows(); ++i) {
        cout << i << ",  name: " << res[i]["name"] << ", age: " << res[i]["age"] << endl;
      }

      StoreQueryResult::const_iterator it;
      for (it = res.begin(); it != res.end(); ++it) {
        mysqlpp::Row row = *it;
        for (size_t i = 0; i < row.size(); i++) {
          cout << '\t' << row[i];
        }
        cout << endl;
      }

      // use result
      cout << endl;
      cout << "use result stye start" << endl;
      Query use_query= cnn.query("select * from test");
      UseQueryResult use_res = use_query.use();
      while (mysqlpp::Row row = use_res.fetch_row()) {
        for (size_t i = 0; i < row.size(); i++) {
          cout << '\t' << row[i];
        }
        cout << endl;
      }

      break;
      }
    } catch (Exception e) {
      cout << "encount error: " << e.what() << endl;
    }

    return 0;
  }
