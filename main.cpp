#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string readInput(string path);

int main() {
    string corpus = readInput("../corpus.txt");

    cout << corpus.length();

    int ngram_length = 2;

    return 0;
}

string readInput(string path) {
    ifstream file("../corpus.txt");
    string str;
    string data;
    while (getline(file, str)) {
        data += str;
        // file_contents.push_back('\n');
    }
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
    return data;
}