#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "freq_list.h"
#include "worker.h"

/* Complete this function for Task 1. Including fixing this comment.
*/
FreqRecord *get_word(char *word, Node *head, char **file_names) {
    Node* cur = head;
    int found = 0;
    if(cur != NULL){
        if(strcmp(cur->word, word) == 0){
            found = 1;
            break;
        }
        cur = cur->next;
    }

    int i = 0;
    while(file_names[i] != NULL){
        i++;
    }
    i--;
    if(!found){
        i = 0;
    }

    //store the last one
    FreqRecord* record = malloc(sizeof(FreqRecord)*(i+1));
    FreqRecord tail;
    tail.freq = 0;
    strcpy(tail.filename, '\0');
    record[i] = tail;

    for(int j = 0; j<i;j++){
        FreqRecord temp;
        temp.freq = cur.freq[j];
        strcpy(temp.filename, file_names[j]);
        record[j] = temp;
    }
    return record;
}

/* Print to standard output the frequency records for a word.
* Use this for your own testing and also for query.c
*/
void print_freq_records(FreqRecord *frp) {
    int i = 0;

    while (frp != NULL && frp[i].freq != 0) {
        printf("%d    %s\n", frp[i].freq, frp[i].filename);
        i++;
    }
}

/* Complete this function for Task 2 including writing a better comment.
*/
void run_worker(char *dirname, int in, int out) {
    return;
}