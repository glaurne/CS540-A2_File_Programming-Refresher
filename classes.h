// Include necessary standard library headers
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;  // Include the standard namespace

class Employee {
public:
    int id, manager_id;  // Employee ID and their manager's ID
    char name[201];      // Fixed length array to store employee name, last 1 character added for null ('\0') character 
    char bio[501];       // Fixed length array to store employee biography

    // Default constructor
    Employee() {}

    // Constructor with parameters to initialize an Employee object
    // Will contain a single row of the Employee.csv file 
    Employee(int id, const string& name, const string& bio, int manager_id)
        : id(id), manager_id(manager_id) {
        strncpy(this->name, name.c_str(), 200);  // Copy the name, ensure it fits in the array
        this->name[200] = '\0';  // Null terminate the string
        strncpy(this->bio, bio.c_str(), 500);  // Copy the biography, ensure it fits in the array
        this->bio[500] = '\0';  // Null terminate the string
    }

    // Function to write the Employee object to a binary output stream, i.e., EmployeeRelation.dat file
    void write_into_data_file(ostream& out) const {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));  // Write the integer ID
        out.write(name, sizeof(name));  // Write the fixed length name

        /***TO_DO***/ // do the same thing for bio and manager-id
        out.write(reinterpret_cast<const char*>(&manager_id), sizeof(manager_id)); 
        out.write(bio, sizeof(bio));  // Write the fixed length bio
    }

    // Read data from a binary input stream, i.e., EmployeeRelation.dat file to populate an Employee object
    void read_from_data_file(istream& in) {
        in.read(reinterpret_cast<char*>(&id), sizeof(id));  // Read the integer ID
        in.read(name, sizeof(name));  // Read the fixed length name

        /***TO_DO***/ // do the same thing for bio and manager-id
        in.read(reinterpret_cast<char*>(&manager_id), sizeof(manager_id));  // Read the integer ID
        in.read(bio, sizeof(bio));  // Read the fixed length bio

    }

    // Print the Employee object to standard output
    void print() const {
        cout << "ID: " << id << ", Name: " << name << ", Bio: " << bio << ", Manager ID: " << manager_id << endl;
    }
};

class StorageManager {
private:
    string filename;  // Name of the file (EmployeeRelation.dat) where we will store the Employee records 
    fstream data_file; // fstream to handle both input and output binary file operations

public:
    // Constructor that opens a data file for binary input/output; truncates any existing data file
    StorageManager(const string& filename) : filename(filename) {
        data_file.open(filename, ios::binary | ios::out | ios::in | ios::trunc);
        if (!data_file.is_open()) {  // Check if the data_file was successfully opened
            cerr << "Failed to open data_file: " << filename << endl;
            exit(EXIT_FAILURE);  // Exit if the data_file cannot be opened
        }
    }

    // Destructor closes the data file if it is still open
    ~StorageManager() {
        if (data_file.is_open()) {
            data_file.close();
        }
    }

    // Reads data from a CSV file and writes it to a binary data file as Employee objects
    void createFromFile(const string& csvFilename) {
        ifstream csvFile(csvFilename);  // Open the Employee.csv file for reading
        
        string line, name, bio;
        int id, manager_id;

        // Read each line from the CSV file, parse it, and create Employee objects
        while (getline(csvFile, line)) {
            
            /***TO_DO***/ 
            // Parse id, name, bio and manager-id from line, to create the Employee object below
            // read data of line into ss as stringstream
            stringstream ss(line); 

            //parse data as string in format - id, name, bio and manager-id 
            string str_id, name, bio, str_man_id;
            long long id, manager_id;
            getline(ss, str_id, ',');
            getline(ss, name, ',');
            getline(ss, bio, ',');
            getline(ss, str_man_id, ',');

            //convert string into int for id and manager id
            id = stoll(str_id);
            manager_id = stoll(str_man_id);

            //check sizes of name and bio
            if (name.size() > 200 || bio.size() > 500) {
                throw runtime_error("Field size exceeded");
            }

            Employee emp(id, name, bio, manager_id);  //create Employee objects

            emp.write_into_data_file(data_file); // Write the Employee object, i.e., the row you read to the .dat data_file
        }
        csvFile.close();  // Close the CSV file
    }

    // Searches for an Employee by ID in the binary data_file and prints if found
    void findAndPrintEmployee(int searchId) {
        data_file.seekg(0, ios::beg);  // Rewind the data_file to the beginning for reading

        Employee emp;

        /*** TO_DO ***/
        // Use [emp.read_from_data_file(data_file)] to read lines from the datafile 
        // until you find the id you are looking for or reach the end-of-file (eof) 

        //data_file.open(filename, ios::binary | ios::out | ios::in | ios::trunc);
        int found = 0;
        while(!data_file.eof()){
            
            emp.read_from_data_file(data_file);
            
            if(searchId = emp.id){
                cout << "============Employee information============\nId: " << emp.id << endl;
                cout << "Name: " << emp.name << endl;
                cout << "Manager ID: " << emp.manager_id << endl;
                cout << "Bio:\n" << emp.bio << endl << endl; 
                found = 1;
                break;
            }
        }
        // Print not found message if no match
        if(!found){
            cout << "*******************************************" << endl;
            cout << "\tEmployee not found" << endl;
            cout << "*******************************************" << endl;
        }
    }
};
