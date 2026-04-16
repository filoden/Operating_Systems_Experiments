#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> // 
#include <sys/stat.h>
#include "debug.h"
#include <time.h>

#define BLOCK_SIZE 4096

int creation_mode;
char* filename = NULL;
int size;
int samples;


void file_creation(){
    // first we try to open the given file name
    int fd = open(filename, O_RDWR | O_TRUNC); // open in rw and trunc to len 0

    // if open fails, determine cause
    if (fd < 0){

        if (errno != ENOENT){
            perrno(OPEN2);
            // open fail for unexpected reason
            exit(1);
        }

        else{ // if cause is no file entry, create file
            // access priviledge: UG = RW, O = R
            mode_t access_mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
            fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, access_mode);
        }
    }
    // post - we should now have an open file of length 0
    for (int i = 0; i<size; i++){
        char buf[BLOCK_SIZE];
        memset(buf,0,sizeof(buf));
        int err = write(fd, buf, sizeof(buf));
        if (err != BLOCK_SIZE){
            fprintf(stderr, "Failure writing 0 to file: i = %d\n", i);
        }
    }
    fsync(fd);
    close(fd);
}

int compar(const void *a, const void *b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    if (aa < bb){
        return -1;
    }
    else if (aa > bb){
        return 1;
    }
    else{
        return 0;
    }
}

void file_randio(){

    // Create/Open file:

    int fd = open(filename, O_RDWR); // open in rw 

    // if open fails, determine cause
    if (fd < 0){
        perrno(OPEN2);
        // open fail for unexpected reason
        exit(1);
        }
    fsync(fd);

    // get file info
    struct stat* info = malloc(sizeof(struct stat));
    int err = stat(filename, info);
    if (err != 0){ // abort on failure
        perrno(STAT2);
        exit(1);
    }
    int file_size = info->st_size;
    free(info);
    int io_size = size; // max io size in bytes
    int max_add = file_size - io_size + 1; // max file add in bytes to prevent reading invalid data
    int max_block = max_add / BLOCK_SIZE - 1; // total number of valid blocks which can be read minus 1
    char* io_buf = malloc(io_size);
    struct timespec time[2];
    int latency[samples];

    for (int i = 0; i < samples; i++){
        // set random address which aligns to block increments
        long add = random() % max_block;
        add = add*BLOCK_SIZE;

        clock_gettime(_POSIX_MONOTONIC_CLOCK, &time[0]);
        int num_offset = lseek(fd,add,SEEK_SET); // set cursor to add bytes
        int num_read = read(fd, io_buf, io_size);
        clock_gettime(_POSIX_MONOTONIC_CLOCK, &time[1]);
        
        latency[i] = (time[1].tv_sec - time[0].tv_sec)*(1000000) + (time[1].tv_nsec - time[0].tv_nsec)/(1000);

        // exit on failure
        if (num_offset != add){
            fprintf(stderr, "lseek error num_offset = %d .\n", num_offset);
            perrno(LSEEK2);
            exit(1);
        }
        if (num_read == -1){
            fprintf(stderr, "read error num_read = %d .\n", num_read);
            perrno(READ2);
            exit(1);
        }


    }
    free(io_buf);
    qsort(latency, samples, sizeof(latency[0]), compar);
    float median;
    if (samples % 2){ // if not even
        median = latency[(samples / 2)];
    }
    else{ // if even
        median = (latency[(samples / 2)] + latency[(samples / 2 - 1)]) / 2;
    }
    printf("%0.2f\n", median);
}

int main(int argc, char* argv[]){
    filename = malloc(strlen(argv[2])+1);
    int IO_mode = atoi(argv[1]);
    strcpy(filename, argv[2]);
    size = atoi(argv[3]);

    switch (IO_mode){
        case (1):
            if (argc != 4){
                fprintf(stderr, "Error, Incorrect number of input arguments for file creation\n");
                exit(1);
            }
            file_creation();
        break;
        case (2):
            if (argc != 5){
                fprintf(stderr, "Error, Incorrect number of input arguments for rand IO\n");
                exit(1);
            }
            samples = atoi(argv[4]);
            file_randio();
        break;

    }



    free(filename);
    return 0;
}

// Anki for main input arguments
