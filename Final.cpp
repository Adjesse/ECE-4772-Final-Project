using namespace std; 
#include <iostream>
#include <fstream>
#include <string>


int main() {

    std::ifstream inputFile("data.txt"); // Open the file

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::cout << line << std::endl; // Print each line to the console
        }

        inputFile.close(); // Close the file when you're done with it
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    return 0;
}

