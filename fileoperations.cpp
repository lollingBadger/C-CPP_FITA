#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    fstream outFile;
    outFile.open("example.txt", ios::in | ios::out | ios::trunc);

    if (!outFile)
    {
        cout << "File could not be opened!" << endl;
        return 1;
    }

    // Write to file
    outFile << "Hello, this is a file operation example explained to Jinesh.\n";
    outFile << "C++ makes file handling easy.\n";

    // Move read pointer to beginning
    outFile.seekg(0);

    string line;
    cout << "Contents of the file:\n";

    // Read from file
    while (getline(outFile, line))
    {
        cout << line << endl;
    }

    outFile.close();


    fstream inFile("example.txt", ios::in);
    if (!inFile)
    {
        cout << "File could not be opened for reading!" << endl;
        return 1;
    }  
    cout << "\nReading the file again to confirm contents:\n";
    while (getline(inFile, line))
    {
        cout << line << endl;
    }       
    inFile.close();
    return 0;
}
