#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc < 2){
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    
    struct stat st;
    int res = fstat(fd, &st);
    
    fprintf(stdout, "%i\n", st.st_ino);
    
    return 0;
}
