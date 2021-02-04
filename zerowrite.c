#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, const char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "usage : %s filename buffer_size [nb]\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];
    size_t buffer_size = atoll(argv[2]);
    long long nb = 1;

    if (argc > 3) {
        nb = atoll(argv[3]);
    }

    int flags = O_RDWR | O_CREAT | O_TRUNC;
    int perms =  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = open(argv[1], flags, perms);


    size_t offset = 0;
    for (long long i = 0; i < nb; i ++) {
        ftruncate(fd, offset + buffer_size);

        char *fd_content = mmap(NULL, buffer_size, PROT_WRITE, MAP_SHARED, fd, offset);
        memset(fd_content, offset, buffer_size);
        munmap(fd_content, buffer_size);

        offset += buffer_size;
    }

    close(fd);
}
