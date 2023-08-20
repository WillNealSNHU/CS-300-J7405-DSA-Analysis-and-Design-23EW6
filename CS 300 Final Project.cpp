// CS 300 Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// William Neal Final Project
// 
// 
// 
// what a mess, I decided to go with a hash table. So it is not the fastest, and there is 100% code bloat compared to other reference types I could have used (see my comments about an
// unordered map below.) I am sure you are asking why did I do this if I knew there was a better and faster way? Well because this one is harder, actually I think the hash table was 
// the hardest reference type for me to implement personally (I am just not as familiar). Hash Tables were also my least favorite type in this class, vectors were my favorite because 
// I know how to easily implement them. This is from experience in school and at work.
// But I thought this is the last code I am going to write in this class, lets see if I can pull this off. I want to improve, learn and grow. So I chose hash table to A. See if I could
// do it and B. learn something new. Again I know it might not be the most logical choice for this assignment. 
// You even say in the announcements hash tables arent a good idea, and have a diagram with all the data structures and sorting algos.
// But I took the risk and I feel like I learned something, hopefully that counts for something.
// In your words "Hash tables are generally not used for storing records (such as these for courses.) If you selected this option, 
// you could make it work. Keep in mind that although you can access an element by key, if there are collisions you cannot access it directly, 
// per se since you must use an alternate method (as we did in our programming assignments) to get the actual value and sorting the output of a hash 
// table requires extra work." Well it was extra work, but the program does run and the functionality is good. It was a pain but I got through it. 

// Also " For those of you who read this announcement, although the instructions state 0-n prerequisites (meaning there could be 20),
// I assure you I will only be testing it for no more than three (3) prerequisites for any given course. Therefore, you may code your program for a 
// max of three prerequisites if you chose not to create a dynamic list to store them. This will make your program a lot easier when it comes to handling
// the prerequisites. Your choice!  :-)  "

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <list>


// Here I am defining the Course class
class Course {
public:
    std::string number;
    std::string title;
    std::vector<std::string> prerequisites;
    // threw this vector in because I am taking a huge risk on making this work.

    Course() = default; // This is the added default constructor

    Course(std::string number, std::string title, std::vector<std::string> prerequisites) {
        this->number = number;
        this->title = title;
        this->prerequisites = prerequisites;
    }
};

// Honestly an unordered map would be better than a hash table, it would look like this:
//std::unordered_map<std::string, Course> courses;
// Would get better log times as well, search and load would be better, would not need the HashTable class, the insert or the getAllCourses.


class HashTable {
private:
    static const int tableSize = 15; // Normally I would not have the table size as a constant, but I already know how much data space I need. 
    std::list<Course> table[tableSize]; // I am going to use an list here for chaining

    int hash(std::string key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % tableSize;
    }
    // actual hashing with our key. 
public:
    void insert(Course course) {
        int index = hash(course.number);
        table[index].push_back(course);
    }
    //inserting our course

    Course* search(std::string courseNumber) {
        int index = hash(courseNumber);
        for (Course& course : table[index]) {
            if (course.number == courseNumber) {
                return &course;
            }
        }
        return nullptr;
    }
    // so that our search functionality below actually works.

    std::vector<Course> getAllCourses() {
        std::vector<Course> allCourses;
        for (int i = 0; i < tableSize; i++) {
            allCourses.insert(allCourses.end(), table[i].begin(), table[i].end());
        }
        return allCourses;
    }
    // you have to get all the courses, without this method everything will break
};

HashTable courses;

// So I can trim/parse the document
// is trimming necessary? probably not, but I don't take chances, I have had so many programs and scripts break because something wasnt trimmed
// I looked at the csv and I shouldnt have a problem, but honestly its just good standard practice when parsing
std::string trim(const std::string& str) {
    std::size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    std::size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

const std::string CSV_PATH = "ABCU_Advising_Program_Input.csv";
void LoadDataStructure() {
    std::string filename = CSV_PATH;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string courseNumber, courseTitle, prerequisite;
        std::vector<std::string> prerequisites;

        // Parse the line and create a Course object
        if (std::getline(ss, courseNumber, ',') && std::getline(ss, courseTitle, ',')) {
            // Trim whitespace
            courseNumber = trim(courseNumber);
            courseTitle = trim(courseTitle);

            // If there is a third value, it is a prerequisite
            while (std::getline(ss, prerequisite, ',')) {
                prerequisite = trim(prerequisite); // Trim any leading/trailing spaces
                prerequisites.push_back(prerequisite);
            }

            // Create a Course object
            Course course(courseNumber, courseTitle, prerequisites);

            // Add the Course object to the unordered_map with course number as key
            courses.insert(course);

        }
    }
}




// Function to print all the courses in the list
void PrintCourseList() {
    // Get all courses from the hash table, messy hash table. 
    std::vector<Course> allCourses = courses.getAllCourses();

    // Sort the courses by their course number
    std::sort(allCourses.begin(), allCourses.end(), [](const Course& a, const Course& b) {
        return a.number < b.number;
        });

    // As we say, PRINT LINE!
    for (const Course& course : allCourses) {
        std::cout << course.number << ": " << course.title << "\n";
    }
}


// Function to search and print
void SearchPrintCourse(std::string number) {
    Course* course = courses.search(number);
    if (course) {
        std::cout << "Title: " << course->title << "\n";
        std::cout << "These are the Prerequisites: ";
        for (const std::string& prerequisite : course->prerequisites) {
            std::cout << prerequisite << " ";
        }
        std::cout << "\n";
    }
    else {
        std::cout << "\n";
        std::cout << "You either did not load the file, or your course doesn't exist. Better luck next time ey? \n";
        std::cout << "\n";
    } // else so you arent wondering why nothing printed out
}

// this is my menu, there are many like it but this one is mine. Without my menu my program is nothing, without my program my menu is nothing.
int main() {
    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1. Load Data Structure\n";
        std::cout << "2. Search for a Course and see if it has Prerequisites\n";
        // changed this because it reads better, some courses have prereqs and others dont.
        std::cout << "3. Print Course\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "The CSV file has loaded :\n";
            LoadDataStructure();
        }
        else if (choice == 2) {
            std::cout << "Enter course number: ";
            std::string number;
            std::cin >> number;
            SearchPrintCourse(number);
        }
        else if (choice == 3) {
            PrintCourseList();
            std::cout << "\n";
        }
        else if (choice == 9) {
            break;
        }
    }
    return 0;
}

