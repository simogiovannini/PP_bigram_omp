#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

string readInput(string path);
map<string, int> computeNgrams(string data, int size, int start, int end);

int main() {
    string corpus = readInput("../corpus.txt");
    cout << "Corpus length: "  << corpus.length() << endl;
    int ngram_length = 2;

    map<string, int> seq_ngrams = computeNgrams(corpus, ngram_length, 0, corpus.length() - 1);
    cout << "Number of ngrams of size " << ngram_length << ": " << seq_ngrams.size() << endl;
    return 0;
}

string readInput(string path) {
    ifstream file(path);
    string str;
    string data;
    while (getline(file, str)) {
        data += str;
        // data.push_back('\n');
    }
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
    return data;
}

map<string, int> computeNgrams(string data, int size, int start, int end) {
    string ngram;
    int batch_size = end - start + 1;
    if (batch_size % size != 0) {
        int leftovers_size = batch_size - (batch_size / size) * size;
        end -= leftovers_size;
    }
    map<string, int> ngrams;
    for (int i = start; i <= end - size + 1; i++) {
        string ngram = data.substr(i, size);
        ngrams[ngram]++;
    }
    return ngrams;
}