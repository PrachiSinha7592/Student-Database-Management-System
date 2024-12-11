#include <iostream>
#include <mysql.h> 
#include <iomanip>

using namespace std;

class StudentDatabase {
private:
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

public:
    StudentDatabase() {
        conn = mysql_init(NULL);
        if (!mysql_real_connect(conn, "localhost", "root", "DB_Password", "Student_Database", 3306, NULL, 0)) {
            cerr << "Error connecting to MySQL database: " << mysql_error(conn) << endl;
            exit(1);
        }
        while (!AdminLogin());
    }

    ~StudentDatabase() {
        // Close the connection to the MySQL database
        mysql_close(conn);
    }

    void executeQuery(const string& query) {
        if (mysql_query(conn, query.c_str()) != 0) {
            cerr << "Error: " << mysql_error(conn) << endl;
        }
    }

    bool AdminLogin() {
        int password;
        cout << "Enter admin password: ";
        cin >> password;

        if (password == 752054) {
            cout << "Admin login successful!" << endl;
            return true;
        } else {
            cout << "Incorrect password. Please try again." << endl;
            return false;
        }
    }

    void AddStudent() {
        string Name, Regd_Number, Roll_Number, Branch, Semester;

        cout << "Enter registration number: ";
        cin >> Regd_Number;

        cout << "Enter student name: ";
        cin >> Name;

        cout << "Enter roll number: ";
        cin >> Roll_Number;

        cout << "Enter branch: ";
        cin >> Branch;

        cout << "Enter semester: ";
        cin >> Semester;

        string query = "INSERT INTO students (Regd_No, S_Name, Roll_No, Branch, Semester) VALUES ('" +
                       Regd_Number + "', '" + Name + "', '" + Roll_Number + "', '" + Branch + "', '" + Semester + "')";
        executeQuery(query);

        cout << "Student added successfully." << endl;
    }

    void DisplayAllStudents() {
        string query = "SELECT * FROM students";
        executeQuery(query);

        res = mysql_store_result(conn);
        if (res != NULL) {
            cout << "--------------------------------- STUDENTS INFORMATION ---------------------------------" << endl;
            cout << setw(15) << "Registration No." << setw(15) << "Name" << setw(15) << "Roll Number" << setw(15) << "Branch"
                 << setw(10) << "Semester" << endl;
            cout << "----------------------------------------------------------------------------------------" << endl;

            while ((row = mysql_fetch_row(res))) {
                cout << setw(15) << row[0] << setw(15) << row[1] << setw(15) << row[2] << setw(15) << row[3] << setw(10)
                     << row[4] << endl;
            }
            cout << "----------------------------------------------------------------------------------------" << endl;
            mysql_free_result(res);
        }
    }

    void UpdateStudent() {
        string Regd_Number;
        int updateOption;

        cout << "Enter registration number of the student to update: ";
        cin >> Regd_Number;

        cout << "Select attribute to update:" << endl;
        cout << "1. Update Name" << endl;
        cout << "2. Update Roll Number" << endl;
        cout << "3. Update Branch" << endl;
        cout << "4. Update Semester" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> updateOption;

        string columnName, newValue;

        switch (updateOption) {
            case 1:
                columnName = "S_Name";
                cout << "Enter new name: ";
                cin >> newValue;
                break;
            case 2:
                columnName = "Roll_No";
                cout << "Enter new roll number: ";
                cin >> newValue;
                break;
            case 3:
                columnName = "Branch";
                cout << "Enter new branch: ";
                cin >> newValue;
                break;
            case 4:
                columnName = "Semester";
                cout << "Enter new semester: ";
                cin >> newValue;
                break;
            default:
                cout << "Invalid option" << endl;
                return;
        }

        string query = "UPDATE students SET " + columnName + " = '" + newValue + "' WHERE Regd_No = '" + Regd_Number + "'";
        executeQuery(query);

        cout << "Student " << columnName << " updated successfully." << endl;
    }

    void DisplayStudentByRegistrationNumber() {
        string Regd_Number;

        cout << "Enter registration number of the student to display: ";
        cin >> Regd_Number;

        string query = "SELECT * FROM students WHERE Regd_No = '" + Regd_Number + "'";
        executeQuery(query);

        res = mysql_store_result(conn);
        if (res != NULL) {
            row = mysql_fetch_row(res);
            if (row != NULL) {
                cout << "--------------------------------- STUDENT INFORMATION ---------------------------------" << endl;
                cout << setw(15) << "Registration No." << setw(15) << "Name" << setw(15) << "Roll Number" << setw(15) << "Branch"
                     << setw(10) << "Semester" << endl;
                cout << "----------------------------------------------------------------------------------------" << endl;
                cout << setw(15) << row[0] << setw(15) << row[1] << setw(15) << row[2] << setw(15) << row[3] << setw(10)
                     << row[4] << endl;
                cout << "---------------------------------------------------------------------------------------" << endl;
            } else {
                cout << "Student not found." << endl;
            }
            mysql_free_result(res);
        }
    }

    void DeleteStudent() {
        string Regd_Number;

        cout << "Enter registration number of the student to delete: ";
        cin >> Regd_Number;

        string query = "DELETE FROM students WHERE Regd_No = '" + Regd_Number + "'";
        executeQuery(query);

        cout << "Student deleted successfully." << endl;
    }

    void MainMenu() {
        int option;
        cout << "======================= STUDENT DATABASE MANAGEMENT SYSTEM ======================= " << endl;
        do {
            cout << endl;
            cout << "1. Add Student" << endl;
            cout << "2. Display All Students" << endl;
            cout << "3. Update Student" << endl;
            cout << "4. Display Student by Registration Number" << endl;
            cout << "5. Delete Student" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> option;

            switch (option) {
                case 1:
                    AddStudent();
                    break;
                case 2:
                    DisplayAllStudents();
                    break;
                case 3:
                    UpdateStudent();
                    break;
                case 4:
                    DisplayStudentByRegistrationNumber();
                    break;
                case 5:
                    DeleteStudent();
                    break;
                case 0:
                    cout << "Exiting Student Database Management System..." << endl;
                    break;
                default:
                    cout << "Invalid option" << endl;
            }
        } while (option != 0);
    }
};

int main() {
    StudentDatabase studentDB;
    studentDB.MainMenu();

    return 0;
}

