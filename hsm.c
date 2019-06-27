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


char in[] = "<stdin>";
char out[] = "<stdout>";
char err[] = "<stderr>";

char *files[32] = { in, out, err };
static int(*libc_open)(const char*, int, ...) = NULL;
static int(*libc_close)(int) = NULL;

int open(const char *pathname, int flags, ...) {
    if (libc_open == NULL)
        libc_open = dlsym(RTLD_NEXT, "open");
    
    int res = libc_open(pathname, flags); // we ignore extra parameters
    if (res >= 0 && res < 32)             // todo: flexibility for > 32
        files[res] = strdup(pathname);
    
    fprintf(stderr, "\nopen called: %s => %i\n", pathname, res);

    return res;
}


int close(int fd) {
    if (libc_close == NULL)
        libc_close = dlsym(RTLD_NEXT, "close");
    
    fprintf(stderr, "\nclose called: %u => %s\n", fd, files[fd]);

    int res = libc_close(fd); 
    if (res >= 0) {
        if (files[res] != in && files[res] != out && files[res] != err)
            free(files[fd]);
        files[fd] = NULL;
    }
        

    return res;
}


int fclose (FILE *stream) {
    static int(*libc_fclose)(FILE*) = NULL;
    if (libc_fclose == NULL)
        libc_fclose = dlsym(RTLD_NEXT, "fclose");
    
    int fd = fileno(stream);
    int res = libc_fclose(stream); 
    fprintf(stderr, "\nfclose called: %u => %s\n", fd, files[fd]);

    if (res >= 0) {
        if (files[res] != in && files[res] != out && files[res] != err)
            free(files[fd]);
        files[fd] = NULL;
    }

    return res;
}


ssize_t read(int fd, void *buf, size_t count) {
    static int(*libc_read)(int, void*, size_t) = NULL;
    if (libc_read == NULL)
        libc_read = dlsym(RTLD_NEXT, "read");

    if (!strcmp(files[fd], "file")) {
        // we do hsm
        struct stat st;
        fstat(fd, &st);

        // don't do this at home
        char buf2[1024] = { 0 };
        sprintf(buf2, "/tmp/%lu", st.st_ino); //unter /tmp/<inodenummer> speichern wir den tatsächlichen Inhalt

        int fd2 = libc_open(buf2, O_RDONLY);
        ssize_t res = libc_read(fd2, buf, count);
        fprintf(stderr, "hsm read %zu from %i (%s::%s) -> %zi :: %.20s\n", count, fd, files[fd], buf2, res,(char*)buf);
        libc_close(fd2);

        return res;
    } else {
        // don't do hsm
        ssize_t res = libc_read(fd, buf, count);
        fprintf(stderr, "read %zu from %i (%s) -> %zi :: %.20s\n", count, fd, files[fd], res, (char*)buf);
        return res;
    }
}

/*
ssize_t write(int fd, const void *buf, size_t count) {

}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE* stream) {
    return write(fileno(stream), ptr, size * nmemb);
}
*/
