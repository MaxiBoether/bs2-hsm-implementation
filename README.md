# bs2-hsm-implementation
a simple simulation for a hierarchical storage management implementation

The idea is to put our libhsm before the libc (using `LD_PRELOAD`). We cannot intercept direct syscall invocations from binaries (as part of this exercise).

## Notes

We differentiate between FD (or `file*`) creating calls and IO-related calls. For now our library should deal with the following calls:

### FDs
    open
    openat
    fopen
    open64
    close
    fclose
    0,1,2
    dup
    dup2
    pipe
    (fcntl)

### IO
    read
    fread
    write
    fwrite
    fread_unlocked
    fwrite_unlocked
    stat
    lstat
    fstat

So, how does our `libhsm` call the `libc` (if we overwrite open, we cannot call open again, as this would lead to recursion). For this, we use the `dlsym` interface. Remember `int open(const char* name)` has the pointer type `int (*p) (const char*)`. So we have:

    int (*p)(const char*) = dlsym(...)
    p(...) //call the libc implementation


