#include <iostream>
#include <string>
#include <vector>

using namespace std;

void generateCombs(string prefix, int min, int max);
void generateCombs2(vector<char> root, int k);

int main() {
    //generateCombs("", 2, 4);
    vector<char> pool;
    pool.push_back('a');
    pool.push_back('b');
    pool.push_back('c');
    generateCombs2(pool, 3);
    return 1;
}

void generateCombs(string prefix, int min, int max) {
    if(min == 0) {
        cout << prefix << endl;
    }
    
    if(max == 0) {
        cout << prefix << endl;
        return;
    }
    
    for(char letter = 48; letter < 123; letter++) {
        if(letter == 58) letter = 97;
        string newPrefix = prefix + letter;
        generateCombs(newPrefix, min-1, max-1);
    }
}

void generateCombs2(vector<char> root, int k) {
    vector<int> pos(k, 0);
    string combo(k, root[0]);
    
    while(true) {
        cout << combo << endl;
        
        int place = k-1;
        while (place >= 0) {
            if(++pos[place] == root.size()) {
                pos[place] = 0;
                combo[place] = root[0];
                place--;
            } else {
                combo[place] = root[pos[place]];
                break;
            }
        }
        if (place < 0)
            break;
    }
}
