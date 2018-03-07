#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <openssl/sha.h>

#define MAX_WORD 128
#define MAX_DICTS 10

void *checkHash(void *ptr);

struct arg_struct {
    char *hash;
    int dictionary;
};

int main(int argc, char **argv) {
    char* hash_input = argv[1];
    
    int thread_num = 2;
    pthread_t *thread_array = malloc(thread_num * sizeof(pthread_t));
    int *thread_id_array = malloc(thread_num * sizeof(int));
    
    for(int i = 0; i < thread_num; i++) {
        struct arg_struct *args = malloc(sizeof (*args));
        args->hash = hash_input;
        args->dictionary = i;
        thread_id_array[i] = pthread_create(&thread_array[i], NULL, checkHash, (void *)args);
    }
    
    for(int i = 0; i < thread_num; i++)
        pthread_join(thread_array[i], NULL);
    
    exit(EXIT_SUCCESS);
}

void *checkHash(void *ptr) {
    struct arg_struct *args = ptr;
    FILE *fp;
    
    char *path = "dicts/";
    char *number = malloc(MAX_DICTS * sizeof(*number));
    sprintf(number, "%d", args->dictionary);
    char *extension = ".txt";
    char *complete_name = malloc(MAX_WORD * sizeof(char));
    
    strcat(complete_name, path);
    strcat(complete_name, number);
    strcat(complete_name, extension);
    
    fp = fopen(complete_name, "r");
    
    if(fp) {
        char *line = malloc(MAX_WORD * sizeof(*line));
        while ( fgets ( line, sizeof line, fp ) != NULL ) {
            line[strcspn(line, "\n")] = 0;
            char *target_hash = args->hash;
            unsigned char hash_try[SHA512_DIGEST_LENGTH];
            char *new_hash = malloc(SHA512_DIGEST_LENGTH);
            SHA512(line, strlen(line), hash_try);
            for(int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
                char *tmp = malloc(MAX_WORD * sizeof(*tmp));
                sprintf(tmp, "%02x", hash_try[i]);
                strcat(new_hash, tmp);
            }
            if(!strcmp(target_hash, new_hash)){
                printf("%s\n", line);
            }
        }
        fclose ( fp );
    } else {
        printf("ERROR");
    }
    pthread_exit(NULL);
}
