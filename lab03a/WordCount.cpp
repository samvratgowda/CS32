#include "WordCount.h"
#include <cctype>

using namespace std;

WordCount::WordCount() {}

size_t WordCount::hash(std::string word) const {
        size_t accumulator = 0;
        for (size_t i = 0; i < word.size(); i++) {
                accumulator += word.at(i);
        }
        return accumulator % CAPACITY;
}

int WordCount::getTotalWords() const {
    int total = 0;
    for (size_t i = 0; i < CAPACITY; i++) {
        for (size_t j = 0; j < table[i].size(); j++) {
            total += table[i][j].second;
        }
    }
    return total;
}

int WordCount::getNumUniqueWords() const {
    int unique = 0;
    for (size_t i = 0; i < CAPACITY; i++) {
        unique += table[i].size();
    }
    return unique;
}

int WordCount::getWordCount(std::string word) const {
    string validWord = makeValidWord(word);
    if (validWord.empty()) {
        return 0;
    }
    size_t idx = hash(validWord);
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].first == validWord) {
            return table[idx][i].second;
        }
    }
    return 0;
}

int WordCount::incrWordCount(std::string word) {
    string validWord = makeValidWord(word);
    if (validWord.empty()) {
        return 0;
    }
    size_t idx = hash(validWord);
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].first == validWord) {
            table[idx][i].second = table[idx][i].second + 1;
            return table[idx][i].second;
        }
    }
    pair<string, int> newPair;
    newPair.first = validWord;
    newPair.second = 1;
    table[idx].push_back(newPair);
    return 1;
}

int WordCount::decrWordCount(std::string word) {
    string validWord = makeValidWord(word);
    if (validWord.empty()) {
        return -1;
    }
    size_t idx = hash(validWord);
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].first == validWord) {
            table[idx][i].second = table[idx][i].second - 1;
            if (table[idx][i].second == 0) {
                table[idx].erase(table[idx].begin() + i);
                return 0;
            }
            return table[idx][i].second;
        }
    }
    return -1;
}

bool WordCount::isWordChar(char c) {
    if (c >= 'a' && c <= 'z') {
        return true;
    }
    if (c >= 'A' && c <= 'Z') {
        return true;
    }
    return false;
}

std::string WordCount::makeValidWord(std::string word) {
    if (word.empty()) {
        return "";
    }
    string result = "";
    for (size_t i = 0; i < word.size(); i++) {
        char c = word[i];
        if (isWordChar(c)) {
            result += tolower(c);
        } else if (c == '\'' || c == '-') {
            if (!result.empty()) {
                result += c;
            }
        }
    }
    while (!result.empty()) {
        char lastChar = result[result.size() - 1];
        if (lastChar == '\'' || lastChar == '-') {
            result.erase(result.size() - 1);
        } else {
            break;
        }
    }
    return result;
}
