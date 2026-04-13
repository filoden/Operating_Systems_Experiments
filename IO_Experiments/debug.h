#include <errno.h>
#include <stdio.h>

enum function_name {
    OPEN2,
    CLOSE2,
    FNCTL2,
    WRITE2,
    READ2,
    STAT2,
    LSEEK2
};

void perrno(int function){
    switch (function){
        case OPEN2:
            switch(errno){
                case EACCES:
                    fprintf(stderr, "OPEN2-EACCES: File/directory access not allowed.\n");
                    break;
                case ENOENT:
                    fprintf(stderr, "OPEN2-ENOENT: File/directory does not exist.\n");
                    break;
                case EEXIST:
                    fprintf(stderr, "OPEN2-EEXIST: File exists and O_CREAT|O_EXCL was used.\n");
                    break;
                case ENOTDIR:
                    fprintf(stderr, "OPEN2-ENOTDIR: A component of the path is not a directory.\n");
                    break;
                case EISDIR:
                    fprintf(stderr, "OPEN2-EISDIR: Path is a directory and cannot be opened with this mode.\n");
                    break;
                case EMFILE:
                    fprintf(stderr, "OPEN2-EMFILE: Per-process file descriptor limit reached.\n");
                    break;
                case ENFILE:
                    fprintf(stderr, "OPEN2-ENFILE: System-wide file descriptor limit reached.\n");
                    break;
                case ENOMEM:
                    fprintf(stderr, "OPEN2-ENOMEM: Insufficient kernel memory.\n");
                    break;
                case EROFS:
                    fprintf(stderr, "OPEN2-EROFS: Read-only file system.\n");
                    break;
                case ENAMETOOLONG:
                    fprintf(stderr, "OPEN2-ENAMETOOLONG: Path name too long.\n");
                    break;
                default:
                    fprintf(stderr, "OPEN2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case CLOSE2:
            switch(errno){
                case EBADF:
                    fprintf(stderr, "CLOSE2-EBADF: Invalid file descriptor.\n");
                    break;
                case EINTR:
                    fprintf(stderr, "CLOSE2-EINTR: Interrupted by a signal.\n");
                    break;
                case EIO:
                    fprintf(stderr, "CLOSE2-EIO: I/O error while closing.\n");
                    break;
                default:
                    fprintf(stderr, "CLOSE2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case FNCTL2:
            switch(errno){
                case EBADF:
                    fprintf(stderr, "FNCTL2-EBADF: Invalid file descriptor.\n");
                    break;
                case EINVAL:
                    fprintf(stderr, "FNCTL2-EINVAL: Invalid command or argument.\n");
                    break;
                case EACCES:
                    fprintf(stderr, "FNCTL2-EACCES: Permission denied.\n");
                    break;
                case EAGAIN:
                    fprintf(stderr, "FNCTL2-EAGAIN: Resource temporarily unavailable.\n");
                    break;
                case EFAULT:
                    fprintf(stderr, "FNCTL2-EFAULT: Bad address.\n");
                    break;
                default:
                    fprintf(stderr, "FNCTL2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case WRITE2:
            switch(errno){
                case EBADF:
                    fprintf(stderr, "WRITE2-EBADF: Invalid file descriptor.\n");
                    break;
                case EFAULT:
                    fprintf(stderr, "WRITE2-EFAULT: Bad address.\n");
                    break;
                case EFBIG:
                    fprintf(stderr, "WRITE2-EFBIG: File too large.\n");
                    break;
                case EINTR:
                    fprintf(stderr, "WRITE2-EINTR: Interrupted by a signal.\n");
                    break;
                case EINVAL:
                    fprintf(stderr, "WRITE2-EINVAL: Invalid argument.\n");
                    break;
                case EIO:
                    fprintf(stderr, "WRITE2-EIO: I/O error.\n");
                    break;
                case ENOSPC:
                    fprintf(stderr, "WRITE2-ENOSPC: No space left on device.\n");
                    break;
                case EPIPE:
                    fprintf(stderr, "WRITE2-EPIPE: Broken pipe.\n");
                    break;
                case EROFS:
                    fprintf(stderr, "WRITE2-EROFS: Read-only file system.\n");
                    break;
                default:
                    fprintf(stderr, "WRITE2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case READ2:
            switch(errno){
                case EBADF:
                    fprintf(stderr, "READ2-EBADF: Invalid file descriptor.\n");
                    break;
                case EFAULT:
                    fprintf(stderr, "READ2-EFAULT: Bad address.\n");
                    break;
                case EINTR:
                    fprintf(stderr, "READ2-EINTR: Interrupted by a signal.\n");
                    break;
                case EINVAL:
                    fprintf(stderr, "READ2-EINVAL: Invalid argument.\n");
                    break;
                case EIO:
                    fprintf(stderr, "READ2-EIO: I/O error.\n");
                    break;
                case ENOMEM:
                    fprintf(stderr, "READ2-ENOMEM: Insufficient memory.\n");
                    break;
                case EAGAIN:
                    fprintf(stderr, "READ2-EAGAIN: Resource temporarily unavailable.\n");
                    break;
                case EISDIR:
                    fprintf(stderr, "READ2-EISDIR: fd refers to directory");
                default:
                    fprintf(stderr, "READ2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case STAT2:
            switch(errno){
                case EACCES:
                    fprintf(stderr, "STAT3-EACCES: Search permission denied for a directory in path.\n");
                    break;
                case EBADF:
                    fprintf(stderr, "STAT3-EBADF: Bad file descriptor.\n");
                    break;
                case EFAULT:
                    fprintf(stderr, "STAT3-EFAULT: Bad address.\n");
                    break;
                case EINVAL:
                    fprintf(stderr, "STAT3-EINVAL: Invalid argument.\n");
                    break;
                case ELOOP:
                    fprintf(stderr, "STAT3-ELOOP: Too many symbolic links.\n");
                    break;
                case ENAMETOOLONG:
                    fprintf(stderr, "STAT3-ENAMETOOLONG: Path name too long.\n");
                    break;
                case ENOENT:
                    fprintf(stderr, "STAT3-ENOENT: File does not exist.\n");
                    break;
                case ENOMEM:
                    fprintf(stderr, "STAT3-ENOMEM: Out of memory.\n");
                    break;
                case ENOTDIR:
                    fprintf(stderr, "STAT3-ENOTDIR: A component is not a directory.\n");
                    break;
                case EOVERFLOW:
                    fprintf(stderr, "STAT3-EOVERFLOW: File size or inode cannot be represented.\n");
                    break;
                default:
                    fprintf(stderr, "STAT3-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        case LSEEK2:
            switch(errno){
                case EBADF:
                    fprintf(stderr, "LSEEK2-EBADF: Invalid file descriptor.\n");
                    break;
                case EINVAL:
                    fprintf(stderr, "LSEEK2-EINVAL: Invalid whence or resulting offset invalid.\n");
                    break;
                case EOVERFLOW:
                    fprintf(stderr, "LSEEK2-EOVERFLOW: Resulting offset cannot be represented.\n");
                    break;
                case ESPIPE:
                    fprintf(stderr, "LSEEK2-ESPIPE: File descriptor is a pipe or FIFO.\n");
                    break;
                default:
                    fprintf(stderr, "LSEEK2-ERRNO %d: Unhandled error.\n", errno);
                    break;
            }
            break;
        default:
            fprintf(stderr, "UNKNOWN-FUNCTION %d errno %d\n", function, errno);
            break;
    }
}