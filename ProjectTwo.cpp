/*
Derek Castro
04/20/2025
CS300 Project Two
Designing a cpp file to read a course file using a menu display interface that prompts the user.

*/
/*
UPDATED 04/27: For automatically loading file
- Updated loadData to direct file path. Line 206 must be changed to direct path for proper data loading.
- The program loads this file automatically (no user input required).
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Course structure to store course data
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Binary Search Tree node structure
struct Node {
    Course course;
    Node* left;
    Node* right;
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;
    // Helper functions for BST 
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
public:
    BinarySearchTree() : root(nullptr) {}
    // Main BST interface
    void Insert(Course course);
    Course Search(string courseNumber);
    void PrintSorted();
    bool IsEmpty() { return root == nullptr; }
};

// Insert new course into BST
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

// Recursive helper 
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

// Search for course in BST
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;
    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return Course(); // Return empty course if not found
}

// Print courses in sorted order
void BinarySearchTree::PrintSorted() {
    inOrder(root);
}

// In-order traversal for printing
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << endl;
        inOrder(node->right);
    }
}

// FUNCTION main()
// Load data from file
void loadData(BinarySearchTree* bst, string filename) {
    ifstream file(filename); // OPEN the file300
    if (!file.is_open()) { // IF file cannot be opened
        cout << "Error: Unable to open the file." << endl; // PRINT "Error: Unable to open the file."
        return; // EXIT PROGRAM
    }

    string line;
    while (getline(file, line)) { // WHILE there are more lines in file
        if (line.empty()) { // IF line is empty or invalid
            continue; // CONTINUE to next line
        }

        vector<string> tokens;
        size_t start = 0;
        size_t end = line.find(',');

        // First token (course number)
        if (end == string::npos) {
            cout << "Error: Invalid format - missing course title." << endl;
            continue;
        }
        tokens.push_back(line.substr(start, end - start));

        // Second token (course title)
        start = end + 1;
        end = line.find(',', start);
        if (end == string::npos) {  // No prerequisites case
            tokens.push_back(line.substr(start));
        }
        else {
            tokens.push_back(line.substr(start, end - start));
            start = end + 1;

            // Handle prerequisites
            while (start < line.size()) {
                end = line.find(',', start);
                if (end == string::npos) {
                    end = line.size();
                }
                string prereq = line.substr(start, end - start);
                if (!prereq.empty()) {  // Only add non-empty prerequisites
                    tokens.push_back(prereq);
                }
                start = end + 1;
            }
        }

        Course course; // CREATE a new course object
        course.courseNumber = tokens[0]; // SET course number to first token
        course.title = tokens[1]; // SET course name to second token

        // Add prerequisites if they exist
        for (size_t i = 2; i < tokens.size(); ++i) { // FOR each prerequisite
            course.prerequisites.push_back(tokens[i]); // APPEND token to course's prerequisites list
        }

        bst->Insert(course); // INSERT course into BST
    }
    file.close(); // CLOSE file
}

// Display main menu options
void displayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

int main() {
    BinarySearchTree bst;
    int choice = 0;
    string filename, courseNumber;

    cout << "Welcome to the course planner." << endl;

    // Menu System 
    while (choice != 9) {
        displayMenu();
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1: // Load default course file
            filename = "C:\\Users\\lolgi\\source\\repos\\ProjectTwoCS300\\x64\\Debug\\CS 300 ABCU_Advising_Program_Input.csv"; // Replace current file path with direct path where .csv file is in your system
            loadData(&bst, filename);
			cout << "Data loaded successfully." << endl; // PRINT "Data loaded successfully."
            break;
        case 2: // ELSE IF choice == 2
            if (bst.IsEmpty()) {
                cout << "No courses loaded. Please load data first." << endl;
            }
            else {
                bst.PrintSorted(); // CALL printSortedCourses()
            }
            break;
        case 3: // ELSE IF choice == 3
            if (bst.IsEmpty()) {
                cout << "No courses loaded. Please load data first." << endl;
                break;
            }
            cout << "What course do you want to know about? "; // PROMPT "Enter course number:"
            cin >> courseNumber; // GET searchTerm
            {
                Course course = bst.Search(courseNumber); // CALL printCourseDetails(searchTerm)
                if (course.courseNumber.empty()) {
                    cout << "Course not found." << endl; // PRINT "Course not found."
                }
                else {
                    cout << course.courseNumber << ", " << course.title << endl; // PRINT course.title
                    if (!course.prerequisites.empty()) { // IF course.prerequisites is not empty
                        cout << "Prerequisites: "; // PRINT course.prerequisites
                        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                            cout << course.prerequisites[i];
                            if (i < course.prerequisites.size() - 1) cout << ", ";
                        }
                        cout << endl;
                    }
                }
            }
            break;
        case 9: // ELSE IF choice == 9
            cout << "Thank you for using the course planner!" << endl;
            return 0; // EXIT PROGRAM
        default: // ELSE
            cout << choice << " is not a valid option." << endl; // PRINT "Invalid choice."
            break;
        }
    }

    return 0;
}