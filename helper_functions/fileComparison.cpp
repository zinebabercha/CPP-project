#include <iostream>
#include <fstream>

using namespace std;

bool areFilesSame(const string& file1, const string& file2) {
    ifstream f1(file1, ifstream::binary | ifstream::ate);
    ifstream f2(file2, ifstream::binary | ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; // File open failed
    }

    if (f1.tellg() != f2.tellg()) {
        return false; // Size mismatch
    }

    // Reset and compare contents
    f1.seekg(0, ifstream::beg);
    f2.seekg(0, ifstream::beg);
    return equal(istreambuf_iterator<char>(f1),
                 istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(f2));
}

int main() {
    string file1 = "C:\\Users\\Pc\\Downloads\\cpp\\Part1 and 2.pdf";
    string file2 = "C:\\Users\\Pc\\Downloads\\cpp\\Part1 and 2.pdf";

    if (areFilesSame(file1, file2)) {
        cout << "Files are the same." << endl;
    } else {
        cout << "Files are different." << endl;
    }

    return 0;
}
