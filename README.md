# bs2-hsm-implementation
a simple simulation for a hierarchical storage management implementation

The idea is to put our libhsm before the libc (using LD\_PRELOAD). We cannot intercept direct syscall invocations from binaries (as part of this exercise).
