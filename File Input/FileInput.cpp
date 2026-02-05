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

using namespace std;

int main() {
    string filename = "words.txt";
    
    // Step 1: Create and write to the file
    cout << "Enter words (type them and press Enter when done):" << endl;
    cout << "> ";
    
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error: Could not create file!" << endl;
        return 1;
    }
    
    string input;
    getline(cin, input);
    outFile << input;
    outFile.close();
    
    cout << "\nFile saved successfully!" << endl;
    cout << "-----------------------------------" << endl;
    
    // Step 2: Read from the file and display words
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Could not open file for reading!" << endl;
        return 1;
    }
    
    cout << "Words from file: ";
    string word;
    int wordCount = 0;
    
    // Read each word separated by space
    while (inFile >> word) {
        cout << word << " ";
        wordCount++;
    }
    
    cout << endl;
    cout << "Number of words found: " << wordCount << endl;
    
    inFile.close();
    
    return 0;
}
