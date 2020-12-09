#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "timespecsub.h"

int debug = 0;

int usage()
{
    char msg[] = "Usage: decode-latency [-d] [-h] filename\n"
                 "-d: debug\n"
                 "-h: help\n";
    fprintf(stderr, "%s", msg);

    return 0;
}

int main(int argc, char *argv[])
{
    int c;
    while ( (c = getopt(argc, argv, "dh")) != -1) {
        switch (c) {
            case 'd':
                debug = 1;
                break;
            case 'h':
                usage();
                exit(0);
            default:
                usage();
                exit(1);
        }
    }

    argc -= optind;
    argv += optind;

    if (argc != 1) {
        usage();
        exit(1);
    }

    FILE *fp = fopen(argv[0], "r");
    if (fp == NULL) {
        err(1, "fopen");
    }

    unsigned char buf[sizeof(unsigned long) + 2*sizeof(struct timespec)];

    int n_data = 0;
    for ( ; ; ) {
        int n = fread(buf, 1 /* byte */, sizeof(buf), fp);
        if (n == 0) {
            if (feof(fp)) {
                break;
            }
            else {
                err(1, "fread");
            }
        }
        else if (n != sizeof(buf)) {
            errx(0, "short read: %d bytes", n);
        }
    
        n_data ++;
        struct timespec start;

        unsigned long *data_size_p = (unsigned long *) &buf[0];
        unsigned long data_size = *data_size_p;
        
        struct timespec *ts_sender_p = (struct timespec *) &buf[sizeof(unsigned long)];
        struct timespec *ts_logger_p = (struct timespec *) &buf[sizeof(unsigned long) + sizeof(struct timespec)];

        struct timespec ts_sender = *ts_sender_p;
        struct timespec ts_logger = *ts_logger_p;
        
        if (n_data == 1) {
            start = ts_sender;
        }

        struct timespec diff, elapsed;
        timespecsub(&ts_logger, &ts_sender, &diff);
        timespecsub(&ts_sender, &start, &elapsed);
        printf("%ld.%09ld %ld.%09ld %.3f kB\n",
            elapsed.tv_sec, elapsed.tv_nsec, diff.tv_sec, diff.tv_nsec, data_size/1024.0);
        
    }


    return 0;
}
