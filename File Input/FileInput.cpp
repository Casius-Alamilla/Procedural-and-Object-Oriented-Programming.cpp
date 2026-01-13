#include <iostream>
#include <fstream>
#include <string>


int main() {
    std::ifstream inputFile("example.txt");
    
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
    }

    inputFile.close();
    return 0;
}




