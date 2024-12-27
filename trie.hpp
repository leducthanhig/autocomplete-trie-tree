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
    if (!pRoot) return {};

    priority_queue<pair<TrieNode*, string>, vector<pair<TrieNode*, string>>, Comparator> pq;
    pq.push({ pRoot, prefix });

    TrieNode* curNode = nullptr;
    string word;
    vector<string> words;
    while (pq.size()) {
        curNode = pq.top().first;
        word = pq.top().second + ' ';
        pq.pop();

        for (int i = 0; i < 26; i++) {
            TrieNode* child = curNode->children[i];
            if (child) {
                word.back() = 'a' + i;
                if (child->weight == curNode->maxChildrenWeight && word.size() >= LENGTH_LEAST) {
                    words.push_back(word);
                    if (words.size() == limit) return words;
                }
                pq.push({ child, word });
            }
        }
    }
    return words;
}

vector<string> search(TrieNode* pRoot, const string& prefix, int limit = SEARCH_RESULT_LIMIT) {
    if (!pRoot) return {};
    
    TrieNode* curNode = pRoot;
    for (char ch: prefix) {
        ch -= 'a';
        if (!curNode->children[ch]) {
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