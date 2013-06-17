#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "database.h" // create_database
#include "Source.h"
#include "Source-odb.h"

using namespace std;
using namespace odb::core;

int main(int argc, char* argv[]) {
	try {
		auto_ptr<database> db(create_database(argc, argv));

		{
		  cout << "Inserting two sources in the database: " << endl
		      << "- 99678, 45.67, 134.67" << endl
          << "- 99690, 123.45, 56.29" << endl;

			Source mySource1(99678, 45.67, 134.67);
			Source mySource2(99690, 123.45, 56.29);

			transaction t(db->begin());
			db->persist(mySource1);
			db->persist(mySource2);
			t.commit();
		}

		{
			typedef odb::result<Source> result;

			transaction t(db->begin());
			result r(db->query<Source>());

      cout << "Reading sources from the database: " << endl;

      for (result::iterator i(r.begin()); i != r.end(); ++i) {
        cout << "- " << i->getSourceId() << ", " << i->getRa() << ", "
            << i->getDec();
        cout << endl;
      }

			t.commit();
		}
	} catch (const odb::exception& e) {
		cerr << e.what() << endl;
		return 1;
	}
}
