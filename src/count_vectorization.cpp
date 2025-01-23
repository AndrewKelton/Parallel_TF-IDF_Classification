#include "count_vectorization.h"

void count_words(const std::string& text, std::map<std::string, int>& global_count) {
    std::map<std::string, int> local_count;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word)
        local_count[word]++;

    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [word, count] : local_count)
        global_count[word] += count;
}