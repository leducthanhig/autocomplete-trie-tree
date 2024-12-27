#include "trie.hpp"

int main() {
    system("cls");
    cout << "Importing dictionary...\n";
    TrieNode* root = createTrie();
    
    char cmd;
    string word;
    vector<string> words;
    uint64 weight;
    while (1) {
        cout << "\nType 'a', 'r', or 's' and 'e' to 'add', 'remove', or 'search' and 'exit'.\n";
        
        cout << "\n> ";
        cin >> cmd;
        if (cmd == 'e') break;
        cin >> word;
            
        switch (cmd) {
        case 'a':
            cout << "Enter the weight of your word, if you don't have one, just enter '-1'.\n";
            cin >> weight;
            insertWord(root, word, weight);
            cout << "\nSuccessfully added new word!\n";
            break;
        case 'r':
            removeWord(root, word, 0);
            cout << "\nSuccessfully removed specified word!\n";
            break;
        case 's':
            words = search(root, word, SEARCH_RESULT_LIMIT);
            cout << "\nFound " << words.size() << " result(s)\n\n";
            for (string word: words) {
                cout << word << endl;
            }
            words.clear();
            break;
        case 'e':
            cout << "\nGood bye!\n";
            break;
        default:
            cout << "\nInvalid mode!\n";
            break;
        }
    }
    
    cout << "\nReleasing memory...\n";
    release(root);

    return 0;
}