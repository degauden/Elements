// file      : database.hxx
//
// Create concrete database instance with SQLite
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::auto_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <odb/database.hxx>

#include <odb/connection.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>

inline std::auto_ptr<odb::database> create_database(int& argc, char* argv[]) {
	using namespace std;
	using namespace odb::core;

	if (argc > 1 && argv[1] == string("--help")) {
		cout << "Usage: " << argv[0] << " [options]" << endl << "Options:"
				<< endl;

		odb::sqlite::database::print_usage(cout);

		exit(0);
	}

	auto_ptr<database> db(
			new odb::sqlite::database(argc, argv, false,
					SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
	{
		connection_ptr c(db->connection());

		c->execute("PRAGMA foreign_keys=OFF");

		transaction t(c->begin());
		schema_catalog::create_schema(*db);
		t.commit();

		c->execute("PRAGMA foreign_keys=ON");

	}

	return db;
}

#endif // DATABASE_HXX
