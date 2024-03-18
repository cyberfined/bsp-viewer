#include <winlib/error.h>
#include <winlib/string.h>
#include <winlib/offsetof.h>
#include <winlib/linux/platform.h>

#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define ENOTBLK         15
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define ETXTBSY         26
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         35
#define ENAMETOOLONG    36
#define ENOLCK          37
#define ENOSYS          38
#define ENOTEMPTY       39
#define ELOOP           40
#define EWOULDBLOCK     EAGAIN
#define ENOMSG          42
#define EIDRM           43
#define ECHRNG          44
#define EL2NSYNC        45
#define EL3HLT          46
#define EL3RST          47
#define ELNRNG          48
#define EUNATCH         49
#define ENOCSI          50
#define EL2HLT          51
#define EBADE           52
#define EBADR           53
#define EXFULL          54
#define ENOANO          55
#define EBADRQC         56
#define EBADSLT         57
#define EDEADLOCK       EDEADLK
#define EBFONT          59
#define ENOSTR          60
#define ENODATA         61
#define ETIME           62
#define ENOSR           63
#define ENONET          64
#define ENOPKG          65
#define EREMOTE         66
#define ENOLINK         67
#define EADV            68
#define ESRMNT          69
#define ECOMM           70
#define EPROTO          71
#define EMULTIHOP       72
#define EDOTDOT         73
#define EBADMSG         74
#define EOVERFLOW       75
#define ENOTUNIQ        76
#define EBADFD          77
#define EREMCHG         78
#define ELIBACC         79
#define ELIBBAD         80
#define ELIBSCN         81
#define ELIBMAX         82
#define ELIBEXEC        83
#define EILSEQ          84
#define ERESTART        85
#define ESTRPIPE        86
#define EUSERS          87
#define ENOTSOCK        88
#define EDESTADDRREQ    89
#define EMSGSIZE        90
#define EPROTOTYPE      91
#define ENOPROTOOPT     92
#define EPROTONOSUPPORT 93
#define ESOCKTNOSUPPORT 94
#define EOPNOTSUPP      95
#define EPFNOSUPPORT    96
#define EAFNOSUPPORT    97
#define EADDRINUSE      98
#define EADDRNOTAVAIL   99
#define ENETDOWN        100
#define ENETUNREACH     101
#define ENETRESET       102
#define ECONNABORTED    103
#define ECONNRESET      104
#define ENOBUFS         105
#define EISCONN         106
#define ENOTCONN        107
#define ESHUTDOWN       108
#define ETOOMANYREFS    109
#define ETIMEDOUT       110
#define ECONNREFUSED    111
#define EHOSTDOWN       112
#define EHOSTUNREACH    113
#define EALREADY        114
#define EINPROGRESS     115
#define ESTALE          116
#define EUCLEAN         117
#define ENOTNAM         118
#define ENAVAIL         119
#define EISNAM          120
#define EREMOTEIO       121
#define EDQUOT          122
#define ENOMEDIUM       123
#define EMEDIUMTYPE     124
#define ECANCELED       125
#define ENOKEY          126
#define EKEYEXPIRED     127
#define EKEYREVOKED     128
#define EKEYREJECTED    129
#define EOWNERDEAD      130
#define ENOTRECOVERABLE 131
#define ERFKILL         132
#define EHWPOISON       133
#define ENOTSUP         EOPNOTSUPP

#define ERRORS \
    ERR(0,            "No error information") \
    ERR(EILSEQ,       "Illegal byte sequence") \
    ERR(EDOM,         "Domain error") \
    ERR(ERANGE,       "Result not representable") \
    ERR(ENOTTY,       "Not a tty") \
    ERR(EACCES,       "Permission denied") \
    ERR(EPERM,        "Operation not permitted") \
    ERR(ENOENT,       "No such file or directory") \
    ERR(ESRCH,        "No such process") \
    ERR(EEXIST,       "File exists") \
    ERR(EOVERFLOW,    "Value too large for data type") \
    ERR(ENOSPC,       "No space left on device") \
    ERR(ENOMEM,       "Out of memory") \
    ERR(EBUSY,        "Resource busy") \
    ERR(EINTR,        "Interrupted system call") \
    ERR(EAGAIN,       "Resource temporarily unavailable") \
    ERR(ESPIPE,       "Invalid seek") \
    ERR(EXDEV,        "Cross-device link") \
    ERR(EROFS,        "Read-only file system") \
    ERR(ENOTEMPTY,    "Directory not empty") \
    ERR(ECONNRESET,   "Connection reset by peer") \
    ERR(ETIMEDOUT,    "Operation timed out") \
    ERR(ECONNREFUSED, "Connection refused") \
    ERR(EHOSTDOWN,    "Host is down") \
    ERR(EHOSTUNREACH, "Host is unreachable") \
    ERR(EADDRINUSE,   "Address in use") \
    ERR(EPIPE,        "Broken pipe") \
    ERR(EIO,          "I/O error") \
    ERR(ENXIO,        "No such device or address") \
    ERR(ENOTBLK,      "Block device required") \
    ERR(ENODEV,       "No such device") \
    ERR(ENOTDIR,      "Not a directory") \
    ERR(EISDIR,       "Is a directory") \
    ERR(ETXTBSY,      "Text file busy") \
    ERR(ENOEXEC,      "Exec format error") \
    ERR(EINVAL,       "Invalid argument") \
    ERR(E2BIG,        "Argument list too long") \
    ERR(ELOOP,        "Symbolic link loop") \
    ERR(ENAMETOOLONG, "Filename too long") \
    ERR(ENFILE,       "Too many open files in system") \
    ERR(EMFILE,       "No file descriptors available") \
    ERR(EBADF,        "Bad file descriptor") \
    ERR(ECHILD,       "No child process") \
    ERR(EFAULT,       "Bad address") \
    ERR(EFBIG,        "File too large") \
    ERR(EMLINK,       "Too many links") \
    ERR(ENOLCK,       "No locks available") \
    ERR(EDEADLK,      "Resource deadlock would occur") \
    ERR(ENOTRECOVERABLE, "State not recoverable") \
    ERR(EOWNERDEAD,   "Previous owner died") \
    ERR(ECANCELED,    "Operation canceled") \
    ERR(ENOSYS,       "Function not implemented") \
    ERR(ENOMSG,       "No message of desired type") \
    ERR(EIDRM,        "Identifier removed") \
    ERR(ENOSTR,       "Device not a stream") \
    ERR(ENODATA,      "No data available") \
    ERR(ETIME,        "Device timeout") \
    ERR(ENOSR,        "Out of streams resources") \
    ERR(ENOLINK,      "Link has been severed") \
    ERR(EPROTO,       "Protocol error") \
    ERR(EBADMSG,      "Bad message") \
    ERR(EBADFD,       "File descriptor in bad state") \
    ERR(ENOTSOCK,     "Not a socket") \
    ERR(EDESTADDRREQ, "Destination address required") \
    ERR(EMSGSIZE,     "Message too large") \
    ERR(EPROTOTYPE,   "Protocol wrong type for socket") \
    ERR(ENOPROTOOPT,  "Protocol not available") \
    ERR(EPROTONOSUPPORT,"Protocol not supported") \
    ERR(ESOCKTNOSUPPORT,"Socket type not supported") \
    ERR(ENOTSUP,      "Not supported") \
    ERR(EPFNOSUPPORT, "Protocol family not supported") \
    ERR(EAFNOSUPPORT, "Address family not supported by protocol") \
    ERR(EADDRNOTAVAIL,"Address not available") \
    ERR(ENETDOWN,     "Network is down") \
    ERR(ENETUNREACH,  "Network unreachable") \
    ERR(ENETRESET,    "Connection reset by network") \
    ERR(ECONNABORTED, "Connection aborted") \
    ERR(ENOBUFS,      "No buffer space available") \
    ERR(EISCONN,      "Socket is connected") \
    ERR(ENOTCONN,     "Socket not connected") \
    ERR(ESHUTDOWN,    "Cannot send after socket shutdown") \
    ERR(EALREADY,     "Operation already in progress") \
    ERR(EINPROGRESS,  "Operation in progress") \
    ERR(ESTALE,       "Stale file handle") \
    ERR(EREMOTEIO,    "Remote I/O error") \
    ERR(EDQUOT,       "Quota exceeded") \
    ERR(ENOMEDIUM,    "No medium found") \
    ERR(EMEDIUMTYPE,  "Wrong medium type") \
    ERR(EMULTIHOP,    "Multihop attempted") \
    ERR(ENOKEY,       "Required key not available") \
    ERR(EKEYEXPIRED,  "Key has expired") \
    ERR(EKEYREVOKED,  "Key has been revoked") \
    ERR(EKEYREJECTED, "Key was rejected by service")

static struct messages_t {
#define ERR(code, msg) char str##code[sizeof(msg)];
ERRORS
#undef ERR
} errors = {
#define ERR(code, msg) msg,
ERRORS
#undef ERR
};

static unsigned short indices[] = {
#define ERR(code, msg) \
    [code] = offsetof(struct messages_t, str##code),
ERRORS
#undef ERR
};

char* os_error(uintptr_t error) {
    if(error >= sizeof(indices) / sizeof(*indices))
        error = 0;

    unsigned short idx = indices[error];
    return ((char*)&errors) + idx;
}
