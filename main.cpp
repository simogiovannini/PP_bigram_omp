#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <omp.h>
#include <chrono>

using namespace std;

string readInput(string path);
map<string, int> computeSequentialNgrams(string data, int ngram_length, int start, int end);
map<string, int> computeParallelNgrams(string data, int ngram_length);

int main() {
    string corpus = readInput("../corpus.txt");
    cout << "Corpus length: "  << corpus.length() << endl;
    int ngram_length = 2;

    auto beg = chrono::high_resolution_clock::now();
    map<string, int> seq_ngrams = computeSequentialNgrams(corpus, ngram_length, 0, corpus.length() - 1);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - beg);

    cout << "Number of ngrams of size " << ngram_length << ": " << seq_ngrams.size() << endl;
    cout << "Sequential Elapsed Time: " << duration.count() << endl;

    beg = chrono::high_resolution_clock::now();
    map<string, int> par_ngrams = computeParallelNgrams(corpus, ngram_length);
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end - beg);

    cout << endl << "Number of ngrams of size " << ngram_length << ": " << par_ngrams.size() << endl;
    cout << "Parallel Elapsed Time: " << duration.count() << endl;

    return 0;
}

string readInput(string path) {
    ifstream file(path);
    string str;
    string data;
    while (getline(file, str)) {
        data += str;
    }
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
    return data;
}

map<string, int> computeSequentialNgrams(string data, int ngram_length, int start, int end) {
    map<string, int> ngrams;
    for (int i = start; i <= end - ngram_length + 1; i++) {
        string ngram = data.substr(i, ngram_length);
        ngrams[ngram]++;
    }
    return ngrams;
}

map<string, int> computeParallelNgrams(string data, int ngram_length) {
    map<string, int> ngrams;
    int n_threads = omp_get_max_threads();

    int batch_size = data.length() / n_threads;

#pragma omp parallel default(none) shared(ngram_length, batch_size, data, ngrams)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id * batch_size;
        int end = start + batch_size - 1;

        if(thread_id == omp_get_max_threads() - 1) {
            end = data.length() - 1;
        }

        map<string, int> local_ngrams;

        if(thread_id > 0) {
            for(int i = start - ngram_length + 1; i < start; i++) {
                string ngram = data.substr(i, ngram_length);
                local_ngrams[ngram]++;
            }
        }

        for (int i = start; i <= end - ngram_length + 1; i++) {
            string ngram = data.substr(i, ngram_length);
            local_ngrams[ngram]++;
        }

        for (auto const &pair: local_ngrams) {
        #pragma omp critical
            {
                ngrams[pair.first] += pair.second;
            }
        }

    }
    return ngrams;
}