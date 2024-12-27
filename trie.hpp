#ifndef TRIE_HPP
#define TRIE_HPP

#include "utils.hpp"

struct TrieNode {
    TrieNode* children[26] = {nullptr};
    uint64 weight = 0, maxChildrenWeight = 0;
    int childrenCount = 0;
};

struct Comparator {
    bool operator()(const pair<TrieNode*, string>& p1, const pair<TrieNode*, string>& p2) {
        ++comparisons;
        return p1.first->maxChildrenWeight < p2.first->maxChildrenWeight;
    }
};

void insertWord(TrieNode* pRoot, const string& word, uint64 weight = 0) {
    if (!pRoot) return;
    
    TrieNode* cur = pRoot;
    for (char ch: word) {
        ch -= 'a';
        if (!cur->children[ch]) {
            cur->children[ch] = new TrieNode();
            cur->childrenCount++;
        }
        cur->maxChildrenWeight = max(cur->maxChildrenWeight, weight);
        cur = cur->children[ch];
    }
    cur->weight = weight;
}

void removeWord(TrieNode*& pRoot, const string& word, int idx) {
    if (!pRoot) return;
    
    if (idx < word.size()) {
        char ch = word[idx] - 'a';
        if (!pRoot->children[ch]) return;
        
        removeWord(pRoot->children[ch], word, idx + 1);

        if (!pRoot->children[ch]) {
            pRoot->childrenCount--;
        }
    }
    else {
        pRoot->weight = 0;
        
        if (!pRoot->childrenCount) {
            delete pRoot;
            pRoot = nullptr;
        }
    }
}

TrieNode* createTrie() {
    TrieNode* root = new TrieNode();
    
    ifstream fi(DATA_FILE_PATH);
    string word, freq;
    getline(fi, word);
    
    while (getline(fi, word, ',')) {
        getline(fi, freq);
        insertWord(root, word, stoull(freq));
    }
    
    fi.close();
    
    return root;
}

vector<string> getAllWords(TrieNode* pRoot, const string& prefix, int limit = SEARCH_RESULT_LIMIT) {
    if (++comparisons && !pRoot) return {};

    priority_queue<pair<TrieNode*, string>, vector<pair<TrieNode*, string>>, Comparator> pq;
    pq.push({ pRoot, prefix });

    TrieNode* curNode = nullptr;
    string word;
    vector<string> words;
    while (++comparisons && pq.size()) {
        curNode = pq.top().first;
        word = pq.top().second + ' ';
        pq.pop();

        for (int i = 0; ++comparisons && i < 26; i++) {
            TrieNode* child = curNode->children[i];
            if (++comparisons && child) {
                word.back() = 'a' + i;
                if (++comparisons && child->weight == curNode->maxChildrenWeight && ++comparisons && word.size() >= LENGTH_LEAST) {
                    words.push_back(word);
                    if (++comparisons && words.size() == limit) return words;
                }
                pq.push({ child, word });
            }
        }
    }
    return words;
}

vector<string> search(TrieNode* pRoot, const string& prefix, int limit = SEARCH_RESULT_LIMIT) {
    if (++comparisons && !pRoot) return {};
    
    TrieNode* curNode = pRoot;
    ++comparisons;
    for (char ch: prefix) {
        ++comparisons;
        ch -= 'a';
        if (++comparisons && !curNode->children[ch]) {
            return {};
        }
        curNode = curNode->children[ch];
    }
    
    return getAllWords(curNode, prefix, limit);
}

void release(TrieNode*& pRoot) {
    if (!pRoot) return;
    
    for (auto& child: pRoot->children) {
        release(child);
    }
    
    delete pRoot;
    pRoot = nullptr;
}

#endif