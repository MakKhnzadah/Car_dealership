#include <iostream>
#include "sqlite3/sqlite3.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <sqlite3.h>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>

using namespace std;

class DatabaseManager
        {
private:
    sqlite3 *db;

    static int callback(void *data, int argc, char **argv, char **azColName)
    {
        int i;
        for (i = 0; i < argc; i++)
        {
            cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "\n";
        return 0;
    }

    void executeSQL(const string &sql)
    {
        char *zErrMsg = 0;
        int rc;
        const char *data = "Callback function called";

        rc = sqlite3_exec(db, sql.c_str(), callback, (void *) data, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cerr << "SQL error: " << zErrMsg << "\n";
            sqlite3_free(zErrMsg);
        } else
        {
            cout << "Operation done successfully" << "\n";
        }
    }

public:
    DatabaseManager(const string &dbName)
    {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
        {
            cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        } else
        {
            cout << "Opened database successfully." << "\n";
        }
    }

    ~DatabaseManager()
    {
        sqlite3_close(db);
    }

    void addCar(const string &registration_number, const string &make, const string &model)
    {
        string sql = "INSERT INTO Cars (registration_number, Make, Model, Status) VALUES ('" +
                     registration_number + "', '" + make + "', '" + model + "', 'Available');";
        executeSQL(sql);
    }

    void editCar(const string &registration_number, const string &new_make,
                 const string &new_model, const string &new_status)
    {
        string sql = "UPDATE Cars SET Make = '" + new_make +
                     "', Model = '" + new_model +
                     "', Status = '" + new_status +
                     "' WHERE registration_number = '" + registration_number + "';";
        executeSQL(sql);
    }

    void removeCar(const string &registration_number)
    {
        string sql = "DELETE FROM Cars WHERE registration_number='" + registration_number + "';";
        executeSQL(sql);
    }

    void addCustomer(const string &name, const string &phone_number, const string &email)
    {
        string sql = "INSERT INTO Customers (name, phone_number, email_address) VALUES ('" +
                     name + "', '" + phone_number + "', '" + email + "');";
        executeSQL(sql);
    }

    void editCustomer(const string &customer_id, const string &new_name,
                      const string &new_phone_number, const string &new_email)
    {
        string sql = "UPDATE Customers SET name = '" + new_name +
                     "', phone_number = '" + new_phone_number +
                     "', email_address = '" + new_email +
                     "' WHERE customer_id = '" + customer_id + "';";
        executeSQL(sql);
    }

    void removeCustomer(const string &customer_id)
    {
        string sql = "DELETE FROM Customers WHERE customer_id='" + customer_id + "';";
        executeSQL(sql);
    }



// Add these functions inside the DatabaseManager class

    void exportToJson(const string &filename)
    {
        fstream outFile("filename");
        if (!outFile.is_open())
        {
            cerr << "Failed to open " << filename << "\n";
            return;
        }

        sqlite3_stmt *stmt;
        const char *carsSql = "SELECT * FROM Cars;";
        const char *customersSql = "SELECT * FROM Customers;";

        // Start the JSON output
        outFile << "{" << "\n";

        // Export Cars
        outFile << "\"Cars\": [";
        if (sqlite3_prepare_v2(db, carsSql, -1, &stmt, NULL) == SQLITE_OK)
        {
            bool first = true;
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                if (!first)
                {
                    outFile << ",";
                } else
                {
                    first = false;
                }
                outFile << "\n  {";
                outFile << "\"registration_number\": \"" << sqlite3_column_text(stmt, 0) << "\", ";
                outFile << "\"Make\": \"" << sqlite3_column_text(stmt, 1) << "\", ";
                outFile << "\"Model\": \"" << sqlite3_column_text(stmt, 2) << "\", ";
                outFile << "\"Status\": \"" << sqlite3_column_text(stmt, 3) << "\"";
                outFile << "}";
            }

            sqlite3_finalize(stmt);
        }
        outFile << "\n]," << "\n";

        // Export Customers
        outFile << "\"Customers\": [";
        if (sqlite3_prepare_v2(db, customersSql, -1, &stmt, NULL) == SQLITE_OK)
        {
            bool first = true;
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                if (!first)
                {
                    outFile << ",";
                } else
                {
                    first = false;
                }
                outFile << "\n  {";
                outFile << "\"customer_id\": " << sqlite3_column_int(stmt, 0) << ", ";
                outFile << "\"name\": \"" << sqlite3_column_text(stmt, 1) << "\", ";
                outFile << "\"phone_number\": \"" << sqlite3_column_text(stmt, 2) << "\", ";
                outFile << "\"email_address\": \"" << sqlite3_column_text(stmt, 3) << "\"";
                outFile << "}";
            }
            sqlite3_finalize(stmt);
        }
        outFile << "\n]" << "\n";

        // Close the JSON output
        outFile << "}" << "\n";

        outFile.close();
        cout << "Exported data to " << filename << "\n";
    }

/*    // Add other methods here...
    void importFromJson(const char *string) {

    }
};
*/
    void importFromJson(const string &filename)
    {
        fstream inFile(filename);
        if (!inFile.is_open())
        {
            cerr << "Failed to open " << filename << "\n";
            return;
        }

        // Dummy code for illustration: actual implementation would involve parsing the JSON file
        // and inserting its content into the database. You would typically use a JSON library for this.
        string line;
        while (getline(inFile, line))
        {
            // Parse the JSON line and extract data to insert into the database
        }

        inFile.close();
        cout << "Imported data from " << filename << "\n";
    }


    void displayMenu()
    {
        cout << "\nCar Rental Management System\n";
        cout << "================================\n";
        cout << "1) Add Customer\n";
        cout << "================================\n";
        cout << "2) Edit Customer\n";
        cout << "================================\n";
        cout << "3) Remove Customer\n";
        cout << "================================\n";
        cout << "4) Add Car\n";
        cout << "================================\n";
        cout << "5) Edit Car\n";
        cout << "================================\n";
        cout << "6) Remove Car\n";
        cout << "================================\n";
        cout << "7) Assign Car to Customer\n";
        cout << "================================\n";
        cout << "8) Unassign Car from Customer\n";
        cout << "================================\n";
        cout << "9) Show Statistics\n";
        cout << "================================\n";
        cout << "10) Exit\n";
        cout << "================================\n";
        cout << "Select an option: ";
    }


    int main()
    {
        DatabaseManager dbManager("car_rental.sqlite");

        int option;
        while (true)
        {
            displayMenu();
            cin >> option;
            cin.ignore();

            string registration_number, make, model, status;
            string customer_id, name, phone_number, email;

            switch (option)
            {
                case 1:
                    cout << "Enter customer name: ";
                    getline(cin, name);
                    cout << "Enter customer phone number: ";
                    getline(cin, phone_number);
                    cout << "Enter customer email address: ";
                    getline(cin, email);
                    dbManager.addCustomer(name, phone_number, email);
                    break;

                case 2:
                    cout << "Enter customer ID: ";
                    cin >> customer_id;
                    cin.ignore();
                    cout << "Enter new customer name: ";
                    getline(cin, name);
                    cout << "Enter new customer phone number: ";
                    getline(cin, phone_number);
                    cout << "Enter new customer email address: ";
                    getline(cin, email);
                    dbManager.editCustomer(customer_id, name, phone_number, email);
                    break;

                case 3:
                    cout << "Enter customer ID to remove: ";
                    cin >> customer_id;
                    dbManager.removeCustomer(customer_id);
                    break;

                case 4:
                    cout << "Enter car registration number: ";
                    getline(cin, registration_number);
                    cout << "Enter car make: ";
                    getline(cin, make);
                    cout << "Enter car model: ";
                    getline(cin, model);
                    dbManager.addCar(registration_number, make, model);
                    break;

                case 5:
                    cout << "Enter car registration number to edit: ";
                    getline(cin, registration_number);
                    cout << "Enter new car make: ";
                    getline(cin, make);
                    cout << "Enter new car model: ";
                    getline(cin, model);
                    cout << "Enter new car status: ";
                    getline(cin, status);
                    dbManager.editCar(registration_number, make, model, status);
                    break;

                case 6:
                    cout << "Enter car registration number to remove: ";
                    getline(cin, registration_number);
                    dbManager.removeCar(registration_number);
                    break;

                case 10:
                    cout << "Exiting program.\n";



                    // ... (Other case statements) ...
                case 11:
                    dbManager.exportToJson("export.json");
                    break;

                case 12:
                    dbManager.importFromJson("import.json");
                    break;
                    // ...


                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }

        return 0;
    }
};














/*using namespace std;



static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    cout << "\n";
    return 0;
}

void executeSQL(sqlite3* db, const string& sql)
{
    char *zErrMsg = 0;
    int rc;
    const char* data = "Callback function called";

    rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << zErrMsg << "\n";
        sqlite3_free(zErrMsg);
    } else
    {
        cout << "Operation done successfully" << "\n";
    }
}

void addCar(sqlite3* db, const string& registration_number, const string& make, const string& model)
{
    string sql = "INSERT INTO Cars (registration_number, Make, Model, Status) VALUES ('" +
                      registration_number + "', '" + make + "', '" + model + "', 'Available');";
    executeSQL(db, sql);
}

void editCar(sqlite3* db, const string& registration_number, const string& new_make,
             const string& new_model, const string& new_status)
{
    string sql = "UPDATE Cars SET Make = '" + new_make +
                      "', Model = '" + new_model +
                      "', Status = '" + new_status +
                      "' WHERE registration_number = '" + registration_number + "';";
    executeSQL(db, sql);
}

void removeCar(sqlite3* db, const string& registration_number)
{
    string sql = "DELETE FROM Cars WHERE registration_number='" + registration_number + "';";
    executeSQL(db, sql);
}

void addCustomer(sqlite3* db, const string& name, const string& phone_number, const string& email)
{
    string sql = "INSERT INTO Customers (name, phone_number, email_address) VALUES ('" +
                      name + "', '" + phone_number + "', '" + email + "');";
    executeSQL(db, sql);
}

void editCustomer(sqlite3* db, const string& customer_id, const string& new_name,
                  const string& new_phone_number, const string& new_email)
{
    string sql = "UPDATE Customers SET name = '" + new_name +
                      "', phone_number = '" + new_phone_number +
                      "', email_address = '" + new_email +
                      "' WHERE customer_id = '" + customer_id + "';";
    executeSQL(db, sql);
}


void removeCustomer(sqlite3* db, const string& customer_id)
{
    string sql = "DELETE FROM Customers WHERE customer_id='" + customer_id + "';";
    executeSQL(db, sql);
}

class DatabaseManager
{

    sqlite3* db;

public:
    DatabaseManager(const string& dbName)
    {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
        {
            cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        } else
        {
            cout << "Opened database successfully." << "\n";


            initializeDatabase();
        }
    }

    ~DatabaseManager()
    {
        sqlite3_close(db);
    }

    void addCustomer();
    void editCustomer();
    void removeCustomer();
    void addCar();
    void editCar();
    void removeCar();
    void assignCarToCustomer();
    void unassignCarFromCustomer();
    void showStatistics();

private:
    void initializeDatabase();
};



void displayMenu()
{
    cout << "\nCar Rental Management System\n";
    cout << "1) Add Customer\n";
    cout << "2) Edit Customer\n";
    cout << "3) Remove Customer\n";
    cout << "4) Add Car\n";
    cout << "5) Edit Car\n";
    cout << "6) Remove Car\n";
    cout << "7) Assign Car to Customer\n";
    cout << "8) Unassign Car from Customer\n";
    cout << "9) Show Statistics\n";
    cout << "10) Exit\n";
    cout << "Select an option: ";
}

int main(int argc, char* argv[])
{
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("car_rental.sqlite", &db);

    if (rc)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        return 0;
    } else {
        cout << "Opened database successfully" << "\n";
    }

    addCar(db, "SD75128", "Toyota", "Camry");


    // ... (Other code remains the same)

    // Sample usage for editing car and customer:
    // Edit car information
    editCar(db, "ABC123", "Honda", "Civic", "Available");

    removeCar(db, "AD34887");


    addCustomer(db, "John Doe", "187654890", "man.koi@uia.com");
    // Edit customer information
    editCustomer(db, "1", "Jony Des", "093211211", "jony.des@outlook.com");

    removeCustomer(db, "1");


    sqlite3_close(db);
    DatabaseManager db("car_rental.db");
    int option;

    while (true)
    {
        displayMenu();
        cin >> option;
        cin.ignore();

        switch (option)
        {
            case 1:
                db.addCustomer();
                break;
            case 2:
                db.editCustomer();
                break;
            case 3:
                db.removeCustomer();
                break;
            case 4:
                db.addCar();
                break;
            case 5:
                db.editCar();
                break;
            case 6:
                db.removeCar();
                break;
            case 7:
                db.assignCarToCustomer();
                break;
            case 8:
                db.unassignCarFromCustomer();
                break;
            case 9:
                db.showStatistics();
                break;
            case 10:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}



class DatabaseManager
        {

    sqlite3* db;

public:
    DatabaseManager(const string& dbName)
    {
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
        {
            cerr << "Error opening database: " << sqlite3_errmsg(db) << "\n";
        } else
        {
            cout << "Opened database successfully." << "\n";


            initializeDatabase();
        }
    }

    ~DatabaseManager()
    {
        sqlite3_close(db);
    }

    void addCustomer();
    void editCustomer();
    void removeCustomer();
    void addCar();
    void editCar();
    void removeCar();
    void assignCarToCustomer();
    void unassignCarFromCustomer();
    void showStatistics();

private:
    void initializeDatabase();
};




void displayMenu()
{
    cout << "\nCar Rental Management System\n";
    cout << "1) Add Customer\n";
    cout << "2) Edit Customer\n";
    cout << "3) Remove Customer\n";
    cout << "4) Add Car\n";
    cout << "5) Edit Car\n";
    cout << "6) Remove Car\n";
    cout << "7) Assign Car to Customer\n";
    cout << "8) Unassign Car from Customer\n";
    cout << "9) Show Statistics\n";
    cout << "10) Exit\n";
    cout << "Select an option: ";
}


int main()
{
    DatabaseManager db("car_rental.db");
    int option;

    while (true)
    {
        displayMenu();
        cin >> option;
        cin.ignore();

        switch (option)
        {
            case 1:
                db.addCustomer();
                break;
            case 2:
                db.editCustomer();
                break;
            case 3:
                db.removeCustomer();
                break;
            case 4:
                db.addCar();
                break;
            case 5:
                db.editCar();
                break;
            case 6:
                db.removeCar();
                break;
            case 7:
                db.assignCarToCustomer();
                break;
            case 8:
                db.unassignCarFromCustomer();
                break;
            case 9:
                db.showStatistics();
                break;
            case 10:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}




class list_Customer
{
private:
    static int nextId;
    int customer_id;
    string name, email_address;
    int phone_numer;

void addCustomer(SQLite::Database& db)
{
    string name, email_address;
    int customer_id, phone_number;

    cout << " enter customer name :";
    if (cin.peek() == '\n') cin.ignore();

    getline(cin, phone_number);
    cout << "enter the custmer phone_number";

    getline(cin, email_address );
     cout << "enter customer email_address:";
     cin >> email_address;

    try
    {
        SQLite::Statement query(db, "INSERT INTO customers (name, email_address, phone_number) VALUES (?, ?, ?)");
        query.bind(1, customer_id);
        query.bind(2, name);
        query.bind(3, email_address);
        query.bind(4, phone_numer);

        query.exec();
        long long id = db.getLastInsertRowid();


        cout << "Added customer: id: " << customer_id << ", name: " << name << ", email_address: "
             << email_address << ", phone_number: " << phone_number << "\n";
    }
    catch (exception& e)
    {
        cerr << "SQLite exception: " << e.what() << "\n";
    }
}

    void editCustomer(SQLite::Database& db)
    {
       int customer_id, phone_number;
       string name, email_address;

        cout << "Enter customer's ID: ";
        cin >> customer_id;
        cin.ignore();

        try
        {
            SQLite::Statement query(db, "SELECT * FROM students WHERE id = ?");
            query.bind(1, customer_id);

            if (query.executeStep())
            {
                cout << "Enter new customer: ";
                getline(cin, name);

                cout << "Enter new customer_email: ";
                getline(cin, email_address);

                cout << "Enter new phone_number: ";
                getline(cin, phone_number);

                cout << "Enter new customer_id: ";
                cin >> customer_id;
                cin.ignore(); // (input buffer)


                SQLite::Statement updateQuery(db, "UPDATE students SET name = ?, email = ?, year = ? WHERE id = ?");
                updateQuery.bind(1, customer_id);
                updateQuery.bind(2, name);
                updateQuery.bind(3, email_address);
                updateQuery.bind(4, phone_numer);
                updateQuery.exec();

                // قم بتغيير هذا الخط ليتناسب مع الإخراج المتوقع
                cout << "Customer was edited successfully" << "\n";



            } else
            {
                cout << "customer not found" << "\n";
            }
        }
        catch (const exception& e)
        {
            cerr << "An error occurred: " << e.what() << "\n";
        }



    }

    void removeCustomer(SQLite::Database& db)
        {
            if (customers.empty())
            {
                cout << " no customers to remove " << "\n";

                return;
            }

            for (size_t i = 0; i < customers.size(); ++i)
            {
                cout << i + 1 << " : " << customers[i].getName() << " ID :" << customers[i].getId() << ":" << "\n";
            }

            cout << " enter the number of the customers to remove. or " << customers.size() + 1 << " to go back :";

            int choice;
            cin >> choice;

            if (choice < 1 || choice > static_cast<int>(customers.size()))
            {
                cout << " invalid choice " << "\n";
                return;
            }

            int customerID = customers[choice - 1].getId();
            customer.erase(customers.begin() +  (choice - 1)) ;

            testResults.erase
                    (
                            remove_if(testResults.begin(), testResults.end(),
                                      [customerID](const TestResult& result)
                                      {
                                          return result.getCustomerId() == customerID;
                                      }),

                            testResults.end()
                    );

            cout << " customer removed " << "\n";
        }


        class Cars
    {
    private:
        static int nextId;
        int registration_nummer;
        string make;
        int model;


        void addCar(vector<Cars>& cars)
        {
            int registration_nummer, model;
            string make;

            cout << " enter car type :";
            if (cin.peek() == '\n') cin.ignore();

            getline(cin, );
            cout << "enter the car model";

            getline(cin, model );
            int car;
            car.emplace_bace(registration_nummer, model, make);


        }


            vector<list_Customer> students;

            vector<Cars> tests;

            vector<TestResult> testResults;

            while (true)
            {
                int choice;
                cout << " choice : ";
                cout << "\n====================\n";
                cout << "1. add customer\n";
                cout << "2. edit customer\n";
                cout << "3. remove customer\n";
                cout << "=====================\n";
                cout << "4. add car\n";
                cout << "5. edit car\n";
                cout << "6. remove car\n";
                cout << "=====================\n";
                cout << "7. add \n";
                cout << "8. edit \n";
                cout << "9. remove \n";
                cout << "======================\n";
                cout << "10. show all info\n";
                cout << "11. exit";
                cout << "\n======================\n ";

                cin >> choice;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << " invalid input. Please enter a number between 1. and 11. ." << "\n";

                    continue;
                }

                switch (choice)
                {
                    case 1:
                        addCustomer(customers);
                        break;

                    case 2:
                        editCustomer(customers);
                        break;

                    case 3:
                        removeCustomer(customers, testResults);
                        break;

                    case 4:
                        addCar(cars);
                        break;

                    case 5:
                        editCar(cars);
                        break;

                    case 6:
                        removeCar(cars);
                        break;

                    case 7:
                        add();
                        break;

                    case 8:
                        edit();
                        break;

                    case 9:
                        remove();
                        break;

                    case 10:
                        displayAllInfo(cars, customers, testResults);
                        break;

                    case 11:
                        cout << " exiting program ." << "\n";
                        return 0;

                    default:
                        cout << " invalid choice. please try again .";
                        cout << "=====================================\n";
                }
            }

        }
*/