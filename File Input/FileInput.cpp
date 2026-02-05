/*
1. Open a file called “words.txt” for input

2. Read in each word from the file. Each word is separated by a space. The file will have at
least one word. The program will not know how many words are in the file.

    a. As the program reads in each word, it will output the words to the console
        (separated by a space).
    b. On a separate line, output the number of words that were found in the file.
*/


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




