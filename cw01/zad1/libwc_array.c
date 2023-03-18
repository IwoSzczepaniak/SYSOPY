#ifndef WC_ARRAY_H
#define WC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256

typedef struct wc_array {
    size_t max_size;    // maximum number of blocks that can be stored
    size_t curr_size;   // current number of blocks stored
    char** blocks;      // array of pointers to blocks
} wc_array_t;

wc_array_t* wc_array_create(size_t max_size) {
    wc_array_t* arr = malloc(sizeof(wc_array_t));
    arr->max_size = max_size;
    arr->curr_size = 0;
    arr->blocks = calloc(max_size, sizeof(char*));
    return arr;
}

char* read_file(const char* filename) {
    char* buffer = NULL;
    size_t length;
    FILE* fp = fopen(filename, "r");

    if (fp) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buffer = (char*)calloc(length + 1, sizeof(char));

        if (buffer) {
            fread(buffer, 1, length, fp);
            buffer[length] = '\0';
        }

        fclose(fp);
    }

    return buffer;
}



int wc_array_count(wc_array_t* arr, const char* filename) {
    char command[MAX_FILENAME_LENGTH + 32];

    
    snprintf(command, sizeof(command), "wc %s > /tmp/wc_array.tmp", filename);
    int ret = system(command);
    if (ret != 0) {
        return -1;
    }

    char* block = read_file("/tmp/wc_array.tmp");

    if (block == NULL)
    {
        printf("Failed to read file.\n");
        return -1;
    }

    
    if(arr->curr_size < arr->max_size){
        arr->blocks[arr->curr_size] = block;
        arr->curr_size++;
    }
    else
    {
        printf("Not enough memory");
        return -1;
    }
 
 
    remove("/tmp/wc_array.tmp");   
    return 0;
}

char* wc_array_get(wc_array_t* arr, size_t index) {
    if (index >= arr->curr_size || index < 0) {
        return "Error with index";
    }
    return arr->blocks[index];
    
}


int wc_array_remove(wc_array_t* arr, size_t index) {
    if (index >= arr->curr_size || index < 0) {
        return -1;
    }
    free(arr->blocks[index]);

    return 0;
}

void wc_array_free(wc_array_t* arr) {
    if (arr) {
        free(arr->blocks);
    }
}

#endif /* WC_ARRAY_H */
