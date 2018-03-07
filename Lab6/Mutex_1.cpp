#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

#define N 100 /* tamaño del búfer */
#define WORD_SIZE 4 /* tamaño de cada palabra a generar */

using namespace std;

void *productor(void *ptr);
void *consumidor(void *ptr);
void generateCombs(string prefix, int min, int max);
vector<char> fillAlphabet();
string sha256(const string str);
int contador = 0;

pthread_mutex_t el_mutex;
pthread_cond_t condc,condp;
vector<string> bufer(N, ""); /* búfer utilizado entre productor y consumidor */

vector<char> root = fillAlphabet();
vector<int> pos(WORD_SIZE, 0);
string combo(WORD_SIZE, root[0]);

int main(int argc, char **argv) {
    
    string *target_hash;
    if(argc > 1) {
        target_hash = new string (argv[1]);
    } else {
        cout << "Uso:\n./(...) + HASH\n";
        return -1;
    }
    
    pthread_t pro, con;
    pthread_mutex_init(&el_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumidor, (void *) target_hash);
    pthread_create(&pro, 0, productor, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&el_mutex);
}

void *productor(void *ptr) /* produce datos */ {
    int i;
    while (true) {
        i = (i+1)%N;
        pthread_mutex_lock(&el_mutex); /* obtiene acceso exclusivo al búfer */
        while (bufer[i]!="") pthread_cond_wait(&condp, &el_mutex);
        while (contador >= N) pthread_cond_wait(&condp, &el_mutex);
        
        bufer[i] = combo; /* coloca elemento en el búfer */
        contador++;
        //cout<<"coloca: "<< bufer[i] << endl;
        
        pthread_cond_signal(&condc); /* despierta al consumidor */
        pthread_mutex_unlock(&el_mutex); /* libera el acceso al búfer */
        
        int place = WORD_SIZE-1;
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
            exit(0);
    }
    pthread_exit(0);
}

void *consumidor(void *ptr) /* consume datos */ {
    int i;
    while(true) {
        i = (i+1)%N;
        pthread_mutex_lock(&el_mutex); /* obtiene acceso exclusivo al búfer */
        while (bufer[i]=="") pthread_cond_wait(&condc, &el_mutex);
        while (contador < 0) pthread_cond_wait(&condc, &el_mutex);
        string curr_word = bufer[i];
        string target_hash = *((string *) ptr);
        string curr_hash = sha256(curr_word);
        cout << curr_hash << endl;
        if(target_hash == curr_hash) {
            cout << "Found word: " << curr_word << "\a\n";
            exit(0);
        }
        
        //cout<< "saca "<<bufer[i]<<endl;
        bufer[i] = ""; /* saca elemento del búfer */
        contador--;
        pthread_cond_signal(&condp); /* despierta al productor */
        pthread_mutex_unlock(&el_mutex); /* libera el acceso al búfer */
    }
    pthread_exit(0);
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

vector<char> fillAlphabet() {
    vector<char> res;
    for(char letter = 48; letter < 123; letter++) {
        if(letter == 58) letter = 97;
        res.push_back(letter);
    }
    return res;
}

string sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

//
// Compilar con:
// g++ -lcrypto
//
// Para producir SHA256 en terminal:
// echo -n "palabra" | openssl dgst -sha256
//
// Referencias:
// https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
// https://ideone.com/Hgm5iL
//
