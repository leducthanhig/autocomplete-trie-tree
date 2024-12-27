#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "utils.hpp"

bool sortAcsendingByWord(const pair<string, uint64>& p1, const pair<string, uint64>& p2) {
    return p1.first < p2.first;
}

bool sortDescendingByWeight(const pair<string, uint64>& p1, const pair<string, uint64>& p2) {
    return p1.second > p2.second;
}

void quickSort(vector<pair<string, uint64>>* dict, int l, int r, bool (*comparator)(const pair<string, uint64>&, const pair<string, uint64>&)) {
    int i = l, j = r;
    auto m = dict->at(l + (r - l) / 2);

    while (i <= j) {
        while (comparator(dict->at(i), m)) i++;
        while (comparator(m, dict->at(j))) j--;
        if (i <= j) {
            swap(dict->at(i), dict->at(j));
            i++; j--;
        }
    }

    if (i < r) quickSort(dict, i, r, comparator);
    if (l < j) quickSort(dict, l, j, comparator);
}

vector<pair<string, uint64>>* createArray() {
    vector<pair<string, uint64>>* dict = new vector<pair<string, uint64>>();
    
    ifstream fi(DATA_FILE_PATH);

    string text;
    getline(fi, text);
    while (getline(fi, text, ',')) {
        dict->push_back({text, 0});
        getline(fi, text);
        dict->back().second = stoull(text);
    }
    quickSort(dict, 0, static_cast<int>(dict->size()) - 1, sortAcsendingByWord);

    fi.close();

    return dict;
}

vector<string> getAllWords(vector<pair<string, uint64>>* dict, int l, int r, int limit = SEARCH_RESULT_LIMIT) {
    vector<pair<string, uint64>>* subDict = new vector<pair<string, uint64>>(dict->begin() + l, dict->begin() + r + 1);
    quickSort(subDict, 0, static_cast<int>(subDict->size()) - 1, sortDescendingByWeight);
    
    vector<string> words;
    limit = min(static_cast<int>(subDict->size()), limit);
    int cnt = limit;
    
    for (auto& word: *subDict) {
        if (word.first.size() >= LENGTH_LEAST) {
            words.push_back(word.first);
            cnt--;
            if (!cnt) break;
        }
    }
    
    delete subDict; subDict = nullptr;
    return words;
}

vector<string> search(vector<pair<string, uint64>>* dict, const string& prefix, int limit = SEARCH_RESULT_LIMIT) {
    int l = 0, r = static_cast<int>(dict->size()) - 1;
    while (l < r) {
        int m = l + (r - l) / 2;
        if (prefix <= dict->at(m).first.substr(0, prefix.size())) r = m;
        else l = m + 1;
    }

    if (dict->at(l).first.substr(0, prefix.size()) != prefix) return {};

    int start = l + 1;
    r = static_cast<int>(dict->size()) - 1;
    while (l < r) {
        int m = l + (r - l + 1) / 2;
        if (dict->at(m).first.substr(0, prefix.size()) <= prefix) l = m;
        else r = m - 1;
    }

    if (r < start) return {};

    return getAllWords(dict, start, r, limit);
}

#endif