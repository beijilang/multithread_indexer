#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

#include "freq_list.h"
#include "indexer.h"

char *remove_punc(char *);

/* Returns a pointer to a linked list of nodes where each node
* contains a word and the count of the number of occurrences of the word.
*/
Node *index_file(Node *head, char *fname, char **filenames) {
    char line[MAXLINE];
    char *marker, *token;
    int countlines = 0;
    FILE *fp;

    if ((fp = fopen(fname, "r")) == NULL) {
        perror(fname);
        exit(1);
    }

    while ((fgets(line, MAXLINE, fp)) != NULL) {
        countlines++;
        if ((countlines % 1000) == 0) {
            printf("processed %d lines from %s (words%d)\n", countlines, fname, num_words);
        }

        if (strlen(line) > 0 ) {
            line[strlen(line) - 1] = '\0';
        } else {
            continue;
        }

        if (strlen(line) == 0) {
            continue;
        }

        marker = line;
        while ((token = strsep(&marker, " \t")) != NULL) {
            if (strlen(token) == 0) {
                continue;
            }

            token = remove_punc(token);
            if ((strlen(token) <= 3) || isdigit(*token)) {
                free(token);
                continue;
            }

            if (*token != '\0') {
                head = add_word(head, filenames, token, fname);
            }
            free(token);
        }
    }
    return head;
}
void print_freq_records(FreqRecord *frp) {
		int i = 0;
		while(frp != NULL && frp[i].freq != 0) {
			printf("%d    %s\n", frp[i].freq, frp[i].filename);
			i++;
		}
	}

int main(int argc, char **argv) {
    Node *head = NULL;
    char **filenames = init_filenames();
    char ch;
    char *indexfile = "index";
    char *namefile = "filenames";
    char *listfile = "index";
    char dirname[PATHLENGTH] = ".";
    char path[PATHLENGTH];

    while ((ch = getopt(argc, argv, "i:n:d:")) != -1) {
        switch (ch) {
        case 'i':
            indexfile = optarg;
            break;
        case 'n':
            namefile = optarg;
            break;
        case 'd':
            strncpy(dirname, optarg, PATHLENGTH);
            dirname[PATHLENGTH - 1] = '\0';
            break;
        default:
            fprintf(stderr, "Usage: indexer [-i FILE] [-n FILE] [-d DIRECTORY_NAME]\n");
            exit(1);
        }
    }

    DIR *dirp;
    if ((dirp = opendir(dirname)) == NULL) {
        perror("opendir");
        exit(1);
    }

    struct dirent *dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 ||
            strcmp(dp->d_name, ".svn") == 0 || strcmp(dp->d_name, ".git") == 0) {
                continue;
        }

        path[0] = '\0';
        strncpy(path, dirname, PATHLENGTH);
        strncat(path, "/", PATHLENGTH-strlen(path));
        strncat(path, dp->d_name, PATHLENGTH-strlen(path));
        path[PATHLENGTH - 1] = '\0';
        printf("Indexing: %s\n", path);
        head = index_file(head, path, filenames);
    }

    if (closedir(dirp) < 0)
        perror("closedir");

    write_list(namefile, indexfile, head, filenames);
    char **read_filenames = init_filenames();
    read_list(listfile, namefile, &head, read_filenames);
    FreqRecord* record = get_word("Banada",head,read_filenames);
    print_freq_records(record);
    return 0;
}
