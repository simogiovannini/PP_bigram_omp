#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <omp.h>
#include <chrono>

using namespace std;

string readInput(const string& path);
map<string, int> computeSequentialNgrams(string data, int ngram_length, int start, int end);
map<string, int> computeParallelNgrams(string data, int ngram_length);

int main() {
    string corpus = readInput("../corpus.txt");
    cout << "Corpus length: "  << corpus.length() << endl;

    std::chrono::high_resolution_clock::time_point beg, end;
    long long int duration;

    int min_ngram_length = 2;
    int max_ngram_length = 8;
    int n_attempts = 10;

    long long int seq_times[max_ngram_length - min_ngram_length + 1];
    long long int par_times[max_ngram_length - min_ngram_length + 1];

    for (int j = min_ngram_length; j <= max_ngram_length; j++) {
        cout << endl << "Length: " << j << endl;
        beg = std::chrono::high_resolution_clock::now();
        for (int p = 0; p < n_attempts; p++) {
            map<string, int> seq_ngrams = computeSequentialNgrams(corpus, j, 0, corpus.length() - 1);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count();
        cout << "Seq time: " << duration / n_attempts << endl;
        seq_times[j - min_ngram_length] = duration / n_attempts;

        beg = std::chrono::high_resolution_clock::now();
        for (int p = 0; p < n_attempts; p++) {
            map<string, int> par_ngrams = computeParallelNgrams(corpus, j);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count();
        cout << "Par time: " << duration / n_attempts << endl;
        par_times[j - min_ngram_length] = duration / n_attempts;
    }
    return 0;
}

string readInput(const string& path) {
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

        if(thread_id > 0) {
            start = start - ngram_length + 1;
        }

        map<string, int> local_ngrams;

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