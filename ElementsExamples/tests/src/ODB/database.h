// file      : database.hxx
//
// Create concrete database instance
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::auto_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <odb/database.hxx>

#include <odb/pgsql/database.hxx>

inline std::auto_ptr<odb::database>
create_database (int& argc, char* argv[])
{
  using namespace std;
  using namespace odb::core;

  if (argc > 1 && argv[1] == string ("--help"))
  {
    cout << "Usage: " << argv[0] << " [options]" << endl
         << "Options:" << endl;

    odb::pgsql::database::print_usage (cout);

    exit (0);
  }

  ///auto_ptr<database> db (new odb::pgsql::database (argc, argv));
  auto_ptr<database> db (new odb::pgsql::database ("euclid", "3ucl1d", "euclidb", "meharga3.isdc.unige.ch"));

  return db;
}

#endif // DATABASE_HXX
