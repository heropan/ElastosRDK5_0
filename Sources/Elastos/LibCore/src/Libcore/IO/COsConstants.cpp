
#include "cmdef.h"
#include "COsConstants.h"

#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace Libcore {
namespace IO {

const Int32 COsConstants::sAF_INET = AF_INET;
const Int32 COsConstants::sAF_INET6 = AF_INET6;
const Int32 COsConstants::sAF_UNIX = AF_UNIX;
const Int32 COsConstants::sAF_UNSPEC = AF_UNSPEC;
const Int32 COsConstants::sAI_ADDRCONFIG = AI_ADDRCONFIG;
const Int32 COsConstants::sAI_ALL = AI_ALL;
const Int32 COsConstants::sAI_CANONNAME = AI_CANONNAME;
const Int32 COsConstants::sAI_NUMERICHOST = AI_NUMERICHOST;
const Int32 COsConstants::sAI_NUMERICSERV = AI_NUMERICSERV;
const Int32 COsConstants::sAI_PASSIVE = AI_PASSIVE;
const Int32 COsConstants::sAI_V4MAPPED = AI_V4MAPPED;
const Int32 COsConstants::sE2BIG = E2BIG;
const Int32 COsConstants::sEACCES = EACCES;
const Int32 COsConstants::sEADDRINUSE = EADDRINUSE;
const Int32 COsConstants::sEADDRNOTAVAIL = EADDRNOTAVAIL;
const Int32 COsConstants::sEAFNOSUPPORT = EAFNOSUPPORT;
const Int32 COsConstants::sEAGAIN = EAGAIN;
const Int32 COsConstants::sEAI_AGAIN = EAI_AGAIN;
const Int32 COsConstants::sEAI_BADFLAGS = EAI_BADFLAGS;
const Int32 COsConstants::sEAI_FAIL = EAI_FAIL;
const Int32 COsConstants::sEAI_FAMILY = EAI_FAMILY;
const Int32 COsConstants::sEAI_MEMORY = EAI_MEMORY;
const Int32 COsConstants::sEAI_NODATA = EAI_NODATA;
const Int32 COsConstants::sEAI_NONAME = EAI_NONAME;
const Int32 COsConstants::sEAI_OVERFLOW = EAI_OVERFLOW;
const Int32 COsConstants::sEAI_SERVICE = EAI_SERVICE;
const Int32 COsConstants::sEAI_SOCKTYPE = EAI_SOCKTYPE;
const Int32 COsConstants::sEAI_SYSTEM = EAI_SYSTEM;
const Int32 COsConstants::sEALREADY = EALREADY;
const Int32 COsConstants::sEBADF = EBADF;
const Int32 COsConstants::sEBADMSG = EBADMSG;
const Int32 COsConstants::sEBUSY = EBUSY;
const Int32 COsConstants::sECANCELED = ECANCELED;
const Int32 COsConstants::sECHILD = ECHILD;
const Int32 COsConstants::sECONNABORTED = ECONNABORTED;
const Int32 COsConstants::sECONNREFUSED = ECONNREFUSED;
const Int32 COsConstants::sECONNRESET = ECONNRESET;
const Int32 COsConstants::sEDEADLK = EDEADLK;
const Int32 COsConstants::sEDESTADDRREQ = EDESTADDRREQ;
const Int32 COsConstants::sEDOM = EDOM;
const Int32 COsConstants::sEDQUOT = EDQUOT;
const Int32 COsConstants::sEEXIST = EEXIST;
const Int32 COsConstants::sEFAULT = EFAULT;
const Int32 COsConstants::sEFBIG = EFBIG;
const Int32 COsConstants::sEHOSTUNREACH = EHOSTUNREACH;
const Int32 COsConstants::sEIDRM = EIDRM;
const Int32 COsConstants::sEILSEQ = EILSEQ;
const Int32 COsConstants::sEINPROGRESS = EINPROGRESS;
const Int32 COsConstants::sEINTR = EINTR;
const Int32 COsConstants::sEINVAL = EINVAL;
const Int32 COsConstants::sEIO = EIO;
const Int32 COsConstants::sEISCONN = EISCONN;
const Int32 COsConstants::sEISDIR = EISDIR;
const Int32 COsConstants::sELOOP = ELOOP;
const Int32 COsConstants::sEMFILE = EMFILE;
const Int32 COsConstants::sEMLINK = EMLINK;
const Int32 COsConstants::sEMSGSIZE = EMSGSIZE;
const Int32 COsConstants::sEMULTIHOP = EMULTIHOP;
const Int32 COsConstants::sENAMETOOLONG = ENAMETOOLONG;
const Int32 COsConstants::sENETDOWN = ENETDOWN;
const Int32 COsConstants::sENETRESET = ENETRESET;
const Int32 COsConstants::sENETUNREACH = ENETUNREACH;
const Int32 COsConstants::sENFILE = ENFILE;
const Int32 COsConstants::sENOBUFS = ENOBUFS;
const Int32 COsConstants::sENODATA = ENODATA;
const Int32 COsConstants::sENODEV = ENODEV;
const Int32 COsConstants::sENOENT = ENOENT;
const Int32 COsConstants::sENOEXEC = ENOEXEC;
const Int32 COsConstants::sENOLCK = ENOLCK;
const Int32 COsConstants::sENOLINK = ENOLINK;
const Int32 COsConstants::sENOMEM = ENOMEM;
const Int32 COsConstants::sENOMSG = ENOMSG;
const Int32 COsConstants::sENOPROTOOPT = ENOPROTOOPT;
const Int32 COsConstants::sENOSPC = ENOSPC;
const Int32 COsConstants::sENOSR = ENOSR;
const Int32 COsConstants::sENOSTR = ENOSTR;
const Int32 COsConstants::sENOSYS = ENOSYS;
const Int32 COsConstants::sENOTCONN = ENOTCONN;
const Int32 COsConstants::sENOTDIR = ENOTDIR;
const Int32 COsConstants::sENOTEMPTY = ENOTEMPTY;
const Int32 COsConstants::sENOTSOCK = ENOTSOCK;
const Int32 COsConstants::sENOTSUP = ENOTSUP;
const Int32 COsConstants::sENOTTY = ENOTTY;
const Int32 COsConstants::sENXIO = ENXIO;
const Int32 COsConstants::sEOPNOTSUPP = EOPNOTSUPP;
const Int32 COsConstants::sEOVERFLOW = EOVERFLOW;
const Int32 COsConstants::sEPERM = EPERM;
const Int32 COsConstants::sEPIPE = EPIPE;
const Int32 COsConstants::sEPROTO = EPROTO;
const Int32 COsConstants::sEPROTONOSUPPORT = EPROTONOSUPPORT;
const Int32 COsConstants::sEPROTOTYPE = EPROTOTYPE;
const Int32 COsConstants::sERANGE = ERANGE;
const Int32 COsConstants::sEROFS = EROFS;
const Int32 COsConstants::sESPIPE = ESPIPE;
const Int32 COsConstants::sESRCH = ESRCH;
const Int32 COsConstants::sESTALE = ESTALE;
const Int32 COsConstants::sETIME = ETIME;
const Int32 COsConstants::sETIMEDOUT = ETIMEDOUT;
const Int32 COsConstants::sETXTBSY = ETXTBSY;
const Int32 COsConstants::sEWOULDBLOCK = EWOULDBLOCK;
const Int32 COsConstants::sEXDEV = EXDEV;
const Int32 COsConstants::sEXIT_FAILURE = EXIT_FAILURE;
const Int32 COsConstants::sEXIT_SUCCESS = EXIT_SUCCESS;
const Int32 COsConstants::sFD_CLOEXEC = FD_CLOEXEC;
const Int32 COsConstants::sFIONREAD = FIONREAD;
const Int32 COsConstants::sF_DUPFD = F_DUPFD;
const Int32 COsConstants::sF_GETFD = F_GETFD;
const Int32 COsConstants::sF_GETFL = F_GETFL;
const Int32 COsConstants::sF_GETLK = F_GETLK;
const Int32 COsConstants::sF_GETLK64 = F_GETLK64;
const Int32 COsConstants::sF_GETOWN = F_GETOWN;
const Int32 COsConstants::sF_OK = F_OK;
const Int32 COsConstants::sF_RDLCK = F_RDLCK;
const Int32 COsConstants::sF_SETFD = F_SETFD;
const Int32 COsConstants::sF_SETFL = F_SETFL;
const Int32 COsConstants::sF_SETLK = F_SETLK;
const Int32 COsConstants::sF_SETLK64 = F_SETLK64;
const Int32 COsConstants::sF_SETLKW = F_SETLKW;
const Int32 COsConstants::sF_SETLKW64 = F_SETLKW64;
const Int32 COsConstants::sF_SETOWN = F_SETOWN;
const Int32 COsConstants::sF_UNLCK = F_UNLCK;
const Int32 COsConstants::sF_WRLCK = F_WRLCK;
const Int32 COsConstants::sIFF_ALLMULTI = IFF_ALLMULTI;
const Int32 COsConstants::sIFF_AUTOMEDIA = IFF_AUTOMEDIA;
const Int32 COsConstants::sIFF_BROADCAST = IFF_BROADCAST;
const Int32 COsConstants::sIFF_DEBUG = IFF_DEBUG;
const Int32 COsConstants::sIFF_DYNAMIC = IFF_DYNAMIC;
const Int32 COsConstants::sIFF_LOOPBACK = IFF_LOOPBACK;
const Int32 COsConstants::sIFF_MASTER = IFF_MASTER;
const Int32 COsConstants::sIFF_MULTICAST = IFF_MULTICAST;
const Int32 COsConstants::sIFF_NOARP = IFF_NOARP;
const Int32 COsConstants::sIFF_NOTRAILERS = IFF_NOTRAILERS;
const Int32 COsConstants::sIFF_POINTOPOINT = IFF_POINTOPOINT;
const Int32 COsConstants::sIFF_PORTSEL = IFF_PORTSEL;
const Int32 COsConstants::sIFF_PROMISC = IFF_PROMISC;
const Int32 COsConstants::sIFF_RUNNING = IFF_RUNNING;
const Int32 COsConstants::sIFF_SLAVE = IFF_SLAVE;
const Int32 COsConstants::sIFF_UP = IFF_UP;
const Int32 COsConstants::sIPPROTO_ICMP = IPPROTO_ICMP;
const Int32 COsConstants::sIPPROTO_IP = IPPROTO_IP;
const Int32 COsConstants::sIPPROTO_IPV6 = IPPROTO_IPV6;
const Int32 COsConstants::sIPPROTO_RAW = IPPROTO_RAW;
const Int32 COsConstants::sIPPROTO_TCP = IPPROTO_TCP;
const Int32 COsConstants::sIPPROTO_UDP = IPPROTO_UDP;
const Int32 COsConstants::sIPV6_CHECKSUM = IPV6_CHECKSUM;
const Int32 COsConstants::sIPV6_MULTICAST_HOPS = IPV6_MULTICAST_HOPS;
const Int32 COsConstants::sIPV6_MULTICAST_IF = IPV6_MULTICAST_IF;
const Int32 COsConstants::sIPV6_MULTICAST_LOOP = IPV6_MULTICAST_LOOP;
const Int32 COsConstants::sIPV6_RECVDSTOPTS = IPV6_RECVDSTOPTS;
const Int32 COsConstants::sIPV6_RECVHOPLIMIT = IPV6_RECVHOPLIMIT;
const Int32 COsConstants::sIPV6_RECVHOPOPTS = IPV6_RECVHOPOPTS;
const Int32 COsConstants::sIPV6_RECVPKTINFO = IPV6_RECVPKTINFO;
const Int32 COsConstants::sIPV6_RECVRTHDR = IPV6_RECVRTHDR;
const Int32 COsConstants::sIPV6_RECVTCLASS = IPV6_RECVTCLASS;
const Int32 COsConstants::sIPV6_TCLASS = IPV6_TCLASS;
const Int32 COsConstants::sIPV6_UNICAST_HOPS = IPV6_UNICAST_HOPS;
const Int32 COsConstants::sIPV6_V6ONLY = IPV6_V6ONLY;
const Int32 COsConstants::sIP_MULTICAST_IF = IP_MULTICAST_IF;
const Int32 COsConstants::sIP_MULTICAST_LOOP = IP_MULTICAST_LOOP;
const Int32 COsConstants::sIP_MULTICAST_TTL = IP_MULTICAST_TTL;
const Int32 COsConstants::sIP_TOS = IP_TOS;
const Int32 COsConstants::sIP_TTL = IP_TTL;
const Int32 COsConstants::sMAP_FIXED = MAP_FIXED;
const Int32 COsConstants::sMAP_PRIVATE = MAP_PRIVATE;
const Int32 COsConstants::sMAP_SHARED = MAP_SHARED;
const Int32 COsConstants::sMCAST_JOIN_GROUP = MCAST_JOIN_GROUP;
const Int32 COsConstants::sMCAST_LEAVE_GROUP = MCAST_LEAVE_GROUP;
const Int32 COsConstants::sMCL_CURRENT = MCL_CURRENT;
const Int32 COsConstants::sMCL_FUTURE = MCL_FUTURE;
const Int32 COsConstants::sMSG_CTRUNC = MSG_CTRUNC;
const Int32 COsConstants::sMSG_DONTROUTE = MSG_DONTROUTE;
const Int32 COsConstants::sMSG_EOR = MSG_EOR;
const Int32 COsConstants::sMSG_OOB = MSG_OOB;
const Int32 COsConstants::sMSG_PEEK = MSG_PEEK;
const Int32 COsConstants::sMSG_TRUNC = MSG_TRUNC;
const Int32 COsConstants::sMSG_WAITALL = MSG_WAITALL;
const Int32 COsConstants::sMS_ASYNC = MS_ASYNC;
const Int32 COsConstants::sMS_INVALIDATE = MS_INVALIDATE;
const Int32 COsConstants::sMS_SYNC = MS_SYNC;
const Int32 COsConstants::sNI_DGRAM = NI_DGRAM;
const Int32 COsConstants::sNI_NAMEREQD = NI_NAMEREQD;
const Int32 COsConstants::sNI_NOFQDN = NI_NOFQDN;
const Int32 COsConstants::sNI_NUMERICHOST = NI_NUMERICHOST;
const Int32 COsConstants::sNI_NUMERICSERV = NI_NUMERICSERV;
const Int32 COsConstants::sO_ACCMODE = O_ACCMODE;
const Int32 COsConstants::sO_APPEND = O_APPEND;
const Int32 COsConstants::sO_CREAT = O_CREAT;
const Int32 COsConstants::sO_EXCL = O_EXCL;
const Int32 COsConstants::sO_NOCTTY = O_NOCTTY;
const Int32 COsConstants::sO_NOFOLLOW = O_NOFOLLOW;
const Int32 COsConstants::sO_NONBLOCK = O_NONBLOCK;
const Int32 COsConstants::sO_RDONLY = O_RDONLY;
const Int32 COsConstants::sO_RDWR = O_RDWR;
const Int32 COsConstants::sO_SYNC = O_SYNC;
const Int32 COsConstants::sO_TRUNC = O_TRUNC;
const Int32 COsConstants::sO_WRONLY = O_WRONLY;
const Int32 COsConstants::sPOLLERR = POLLERR;
const Int32 COsConstants::sPOLLHUP = POLLHUP;
const Int32 COsConstants::sPOLLIN = POLLIN;
const Int32 COsConstants::sPOLLNVAL = POLLNVAL;
const Int32 COsConstants::sPOLLOUT = POLLOUT;
const Int32 COsConstants::sPOLLPRI = POLLPRI;
const Int32 COsConstants::sPOLLRDBAND = POLLRDBAND;
const Int32 COsConstants::sPOLLRDNORM = POLLRDNORM;
const Int32 COsConstants::sPOLLWRBAND = POLLWRBAND;
const Int32 COsConstants::sPOLLWRNORM = POLLWRNORM;
const Int32 COsConstants::sPROT_EXEC = PROT_EXEC;
const Int32 COsConstants::sPROT_NONE = PROT_NONE;
const Int32 COsConstants::sPROT_READ = PROT_READ;
const Int32 COsConstants::sPROT_WRITE = PROT_WRITE;
const Int32 COsConstants::sR_OK = R_OK;
const Int32 COsConstants::sSEEK_CUR = SEEK_CUR;
const Int32 COsConstants::sSEEK_END = SEEK_END;
const Int32 COsConstants::sSEEK_SET = SEEK_SET;
const Int32 COsConstants::sSHUT_RD = SHUT_RD;
const Int32 COsConstants::sSHUT_RDWR = SHUT_RDWR;
const Int32 COsConstants::sSHUT_WR = SHUT_WR;
const Int32 COsConstants::sSIGABRT = SIGABRT;
const Int32 COsConstants::sSIGALRM = SIGALRM;
const Int32 COsConstants::sSIGBUS = SIGBUS;
const Int32 COsConstants::sSIGCHLD = SIGCHLD;
const Int32 COsConstants::sSIGCONT = SIGCONT;
const Int32 COsConstants::sSIGFPE = SIGFPE;
const Int32 COsConstants::sSIGHUP = SIGHUP;
const Int32 COsConstants::sSIGILL = SIGILL;
const Int32 COsConstants::sSIGINT = SIGINT;
const Int32 COsConstants::sSIGIO = SIGIO;
const Int32 COsConstants::sSIGKILL = SIGKILL;
const Int32 COsConstants::sSIGPIPE = SIGPIPE;
const Int32 COsConstants::sSIGPROF = SIGPROF;
const Int32 COsConstants::sSIGPWR = SIGPWR;
const Int32 COsConstants::sSIGQUIT = SIGQUIT;
const Int32 COsConstants::sSIGRTMAX = SIGRTMAX;
const Int32 COsConstants::sSIGRTMIN = SIGRTMIN;
const Int32 COsConstants::sSIGSEGV = SIGSEGV;
const Int32 COsConstants::sSIGSTKFLT = SIGSTKFLT;
const Int32 COsConstants::sSIGSTOP = SIGSTOP;
const Int32 COsConstants::sSIGSYS = SIGSYS;
const Int32 COsConstants::sSIGTERM = SIGTERM;
const Int32 COsConstants::sSIGTRAP = SIGTRAP;
const Int32 COsConstants::sSIGTSTP = SIGTSTP;
const Int32 COsConstants::sSIGTTIN = SIGTTIN;
const Int32 COsConstants::sSIGTTOU = SIGTTOU;
const Int32 COsConstants::sSIGURG = SIGURG;
const Int32 COsConstants::sSIGUSR1 = SIGUSR1;
const Int32 COsConstants::sSIGUSR2 = SIGUSR2;
const Int32 COsConstants::sSIGVTALRM = SIGVTALRM;
const Int32 COsConstants::sSIGWINCH = SIGWINCH;
const Int32 COsConstants::sSIGXCPU = SIGXCPU;
const Int32 COsConstants::sSIGXFSZ = SIGXFSZ;
const Int32 COsConstants::sSIOCGIFADDR = SIOCGIFADDR;
const Int32 COsConstants::sSIOCGIFBRDADDR = SIOCGIFBRDADDR;
const Int32 COsConstants::sSIOCGIFDSTADDR = SIOCGIFDSTADDR;
const Int32 COsConstants::sSIOCGIFNETMASK = SIOCGIFNETMASK;
const Int32 COsConstants::sSOCK_DGRAM = SOCK_DGRAM;
const Int32 COsConstants::sSOCK_RAW = SOCK_RAW;
const Int32 COsConstants::sSOCK_SEQPACKET = SOCK_SEQPACKET;
const Int32 COsConstants::sSOCK_STREAM = SOCK_STREAM;
const Int32 COsConstants::sSOL_SOCKET = SOL_SOCKET;
const Int32 COsConstants::sSO_BINDTODEVICE = SO_BINDTODEVICE;
const Int32 COsConstants::sSO_BROADCAST = SO_BROADCAST;
const Int32 COsConstants::sSO_DEBUG = SO_DEBUG;
const Int32 COsConstants::sSO_DONTROUTE = SO_DONTROUTE;
const Int32 COsConstants::sSO_ERROR = SO_ERROR;
const Int32 COsConstants::sSO_KEEPALIVE = SO_KEEPALIVE;
const Int32 COsConstants::sSO_LINGER = SO_LINGER;
const Int32 COsConstants::sSO_OOBINLINE = SO_OOBINLINE;
const Int32 COsConstants::sSO_RCVBUF = SO_RCVBUF;
const Int32 COsConstants::sSO_RCVLOWAT = SO_RCVLOWAT;
const Int32 COsConstants::sSO_RCVTIMEO = SO_RCVTIMEO;
const Int32 COsConstants::sSO_REUSEADDR = SO_REUSEADDR;
const Int32 COsConstants::sSO_SNDBUF = SO_SNDBUF;
const Int32 COsConstants::sSO_SNDLOWAT = SO_SNDLOWAT;
const Int32 COsConstants::sSO_SNDTIMEO = SO_SNDTIMEO;
const Int32 COsConstants::sSO_TYPE = SO_TYPE;
const Int32 COsConstants::sSTDERR_FILENO = STDERR_FILENO;
const Int32 COsConstants::sSTDIN_FILENO = STDIN_FILENO;
const Int32 COsConstants::sSTDOUT_FILENO = STDOUT_FILENO;
const Int32 COsConstants::sS_IFBLK = S_IFBLK;
const Int32 COsConstants::sS_IFCHR = S_IFCHR;
const Int32 COsConstants::sS_IFDIR = S_IFDIR;
const Int32 COsConstants::sS_IFIFO = S_IFIFO;
const Int32 COsConstants::sS_IFLNK = S_IFLNK;
const Int32 COsConstants::sS_IFMT = S_IFMT;
const Int32 COsConstants::sS_IFREG = S_IFREG;
const Int32 COsConstants::sS_IFSOCK = S_IFSOCK;
const Int32 COsConstants::sS_IRGRP = S_IRGRP;
const Int32 COsConstants::sS_IROTH = S_IROTH;
const Int32 COsConstants::sS_IRUSR = S_IRUSR;
const Int32 COsConstants::sS_IRWXG = S_IRWXG;
const Int32 COsConstants::sS_IRWXO = S_IRWXO;
const Int32 COsConstants::sS_IRWXU = S_IRWXU;
const Int32 COsConstants::sS_ISGID = S_ISGID;
const Int32 COsConstants::sS_ISUID = S_ISUID;
const Int32 COsConstants::sS_ISVTX = S_ISVTX;
const Int32 COsConstants::sS_IWGRP = S_IWGRP;
const Int32 COsConstants::sS_IWOTH = S_IWOTH;
const Int32 COsConstants::sS_IWUSR = S_IWUSR;
const Int32 COsConstants::sS_IXGRP = S_IXGRP;
const Int32 COsConstants::sS_IXOTH = S_IXOTH;
const Int32 COsConstants::sS_IXUSR = S_IXUSR;
const Int32 COsConstants::sTCP_NODELAY = TCP_NODELAY;
const Int32 COsConstants::sWCONTINUED = WCONTINUED;
const Int32 COsConstants::sWEXITED = WEXITED;
const Int32 COsConstants::sWNOHANG = WNOHANG;
const Int32 COsConstants::sWNOWAIT = WNOWAIT;
const Int32 COsConstants::sWSTOPPED = WSTOPPED;
const Int32 COsConstants::sWUNTRACED = WUNTRACED;
const Int32 COsConstants::sW_OK = W_OK;
const Int32 COsConstants::sX_OK = X_OK;
const Int32 COsConstants::s_SC_2_CHAR_TERM = _SC_2_CHAR_TERM;
const Int32 COsConstants::s_SC_2_C_BIND = _SC_2_C_BIND;
const Int32 COsConstants::s_SC_2_C_DEV = _SC_2_C_DEV;
const Int32 COsConstants::s_SC_2_C_VERSION = _SC_2_C_VERSION;
const Int32 COsConstants::s_SC_2_FORT_DEV = _SC_2_FORT_DEV;
const Int32 COsConstants::s_SC_2_FORT_RUN = _SC_2_FORT_RUN;
const Int32 COsConstants::s_SC_2_LOCALEDEF = _SC_2_LOCALEDEF;
const Int32 COsConstants::s_SC_2_SW_DEV = _SC_2_SW_DEV;
const Int32 COsConstants::s_SC_2_UPE = _SC_2_UPE;
const Int32 COsConstants::s_SC_2_VERSION = _SC_2_VERSION;
const Int32 COsConstants::s_SC_AIO_LISTIO_MAX = _SC_AIO_LISTIO_MAX;
const Int32 COsConstants::s_SC_AIO_MAX = _SC_AIO_MAX;
const Int32 COsConstants::s_SC_AIO_PRIO_DELTA_MAX = _SC_AIO_PRIO_DELTA_MAX;
const Int32 COsConstants::s_SC_ARG_MAX = _SC_ARG_MAX;
const Int32 COsConstants::s_SC_ASYNCHRONOUS_IO = _SC_ASYNCHRONOUS_IO;
const Int32 COsConstants::s_SC_ATEXIT_MAX = _SC_ATEXIT_MAX;
const Int32 COsConstants::s_SC_AVPHYS_PAGES = _SC_AVPHYS_PAGES;
const Int32 COsConstants::s_SC_BC_BASE_MAX = _SC_BC_BASE_MAX;
const Int32 COsConstants::s_SC_BC_DIM_MAX = _SC_BC_DIM_MAX;
const Int32 COsConstants::s_SC_BC_SCALE_MAX = _SC_BC_SCALE_MAX;
const Int32 COsConstants::s_SC_BC_STRING_MAX = _SC_BC_STRING_MAX;
const Int32 COsConstants::s_SC_CHILD_MAX = _SC_CHILD_MAX;
const Int32 COsConstants::s_SC_CLK_TCK = _SC_CLK_TCK;
const Int32 COsConstants::s_SC_COLL_WEIGHTS_MAX = _SC_COLL_WEIGHTS_MAX;
const Int32 COsConstants::s_SC_DELAYTIMER_MAX = _SC_DELAYTIMER_MAX;
const Int32 COsConstants::s_SC_EXPR_NEST_MAX = _SC_EXPR_NEST_MAX;
const Int32 COsConstants::s_SC_FSYNC = _SC_FSYNC;
const Int32 COsConstants::s_SC_GETGR_R_SIZE_MAX = _SC_GETGR_R_SIZE_MAX;
const Int32 COsConstants::s_SC_GETPW_R_SIZE_MAX = _SC_GETPW_R_SIZE_MAX;
const Int32 COsConstants::s_SC_IOV_MAX = _SC_IOV_MAX;
const Int32 COsConstants::s_SC_JOB_CONTROL = _SC_JOB_CONTROL;
const Int32 COsConstants::s_SC_LINE_MAX = _SC_LINE_MAX;
const Int32 COsConstants::s_SC_LOGIN_NAME_MAX = _SC_LOGIN_NAME_MAX;
const Int32 COsConstants::s_SC_MAPPED_FILES = _SC_MAPPED_FILES;
const Int32 COsConstants::s_SC_MEMLOCK = _SC_MEMLOCK;
const Int32 COsConstants::s_SC_MEMLOCK_RANGE = _SC_MEMLOCK_RANGE;
const Int32 COsConstants::s_SC_MEMORY_PROTECTION = _SC_MEMORY_PROTECTION;
const Int32 COsConstants::s_SC_MESSAGE_PASSING = _SC_MESSAGE_PASSING;
const Int32 COsConstants::s_SC_MQ_OPEN_MAX = _SC_MQ_OPEN_MAX;
const Int32 COsConstants::s_SC_MQ_PRIO_MAX = _SC_MQ_PRIO_MAX;
const Int32 COsConstants::s_SC_NGROUPS_MAX = _SC_NGROUPS_MAX;
const Int32 COsConstants::s_SC_NPROCESSORS_CONF = _SC_NPROCESSORS_CONF;
const Int32 COsConstants::s_SC_NPROCESSORS_ONLN = _SC_NPROCESSORS_ONLN;
const Int32 COsConstants::s_SC_OPEN_MAX = _SC_OPEN_MAX;
const Int32 COsConstants::s_SC_PAGESIZE = _SC_PAGESIZE;
const Int32 COsConstants::s_SC_PAGE_SIZE = _SC_PAGE_SIZE;
const Int32 COsConstants::s_SC_PASS_MAX = _SC_PASS_MAX;
const Int32 COsConstants::s_SC_PHYS_PAGES = _SC_PHYS_PAGES;
const Int32 COsConstants::s_SC_PRIORITIZED_IO = _SC_PRIORITIZED_IO;
const Int32 COsConstants::s_SC_PRIORITY_SCHEDULING = _SC_PRIORITY_SCHEDULING;
const Int32 COsConstants::s_SC_REALTIME_SIGNALS = _SC_REALTIME_SIGNALS;
const Int32 COsConstants::s_SC_RE_DUP_MAX = _SC_RE_DUP_MAX;
const Int32 COsConstants::s_SC_RTSIG_MAX = _SC_RTSIG_MAX;
const Int32 COsConstants::s_SC_SAVED_IDS = _SC_SAVED_IDS;
const Int32 COsConstants::s_SC_SEMAPHORES = _SC_SEMAPHORES;
const Int32 COsConstants::s_SC_SEM_NSEMS_MAX = _SC_SEM_NSEMS_MAX;
const Int32 COsConstants::s_SC_SEM_VALUE_MAX = _SC_SEM_VALUE_MAX;
const Int32 COsConstants::s_SC_SHARED_MEMORY_OBJECTS = _SC_SHARED_MEMORY_OBJECTS;
const Int32 COsConstants::s_SC_SIGQUEUE_MAX = _SC_SIGQUEUE_MAX;
const Int32 COsConstants::s_SC_STREAM_MAX = _SC_STREAM_MAX;
const Int32 COsConstants::s_SC_SYNCHRONIZED_IO = _SC_SYNCHRONIZED_IO;
const Int32 COsConstants::s_SC_THREADS = _SC_THREADS;
const Int32 COsConstants::s_SC_THREAD_ATTR_STACKADDR = _SC_THREAD_ATTR_STACKADDR;
const Int32 COsConstants::s_SC_THREAD_ATTR_STACKSIZE = _SC_THREAD_ATTR_STACKSIZE;
const Int32 COsConstants::s_SC_THREAD_DESTRUCTOR_ITERATIONS = _SC_THREAD_DESTRUCTOR_ITERATIONS;
const Int32 COsConstants::s_SC_THREAD_KEYS_MAX = _SC_THREAD_KEYS_MAX;
const Int32 COsConstants::s_SC_THREAD_PRIORITY_SCHEDULING = _SC_THREAD_PRIORITY_SCHEDULING;
const Int32 COsConstants::s_SC_THREAD_PRIO_INHERIT = _SC_THREAD_PRIO_INHERIT;
const Int32 COsConstants::s_SC_THREAD_PRIO_PROTECT = _SC_THREAD_PRIO_PROTECT;
const Int32 COsConstants::s_SC_THREAD_SAFE_FUNCTIONS = _SC_THREAD_SAFE_FUNCTIONS;
const Int32 COsConstants::s_SC_THREAD_STACK_MIN = _SC_THREAD_STACK_MIN;
const Int32 COsConstants::s_SC_THREAD_THREADS_MAX = _SC_THREAD_THREADS_MAX;
const Int32 COsConstants::s_SC_TIMERS = _SC_TIMERS;
const Int32 COsConstants::s_SC_TIMER_MAX = _SC_TIMER_MAX;
const Int32 COsConstants::s_SC_TTY_NAME_MAX = _SC_TTY_NAME_MAX;
const Int32 COsConstants::s_SC_TZNAME_MAX = _SC_TZNAME_MAX;
const Int32 COsConstants::s_SC_VERSION = _SC_VERSION;
const Int32 COsConstants::s_SC_XBS5_ILP32_OFF32 = _SC_XBS5_ILP32_OFF32;
const Int32 COsConstants::s_SC_XBS5_ILP32_OFFBIG = _SC_XBS5_ILP32_OFFBIG;
const Int32 COsConstants::s_SC_XBS5_LP64_OFF64 = _SC_XBS5_LP64_OFF64;
const Int32 COsConstants::s_SC_XBS5_LPBIG_OFFBIG = _SC_XBS5_LPBIG_OFFBIG;
const Int32 COsConstants::s_SC_XOPEN_CRYPT = _SC_XOPEN_CRYPT;
const Int32 COsConstants::s_SC_XOPEN_ENH_I18N = _SC_XOPEN_ENH_I18N;
const Int32 COsConstants::s_SC_XOPEN_LEGACY = _SC_XOPEN_LEGACY;
const Int32 COsConstants::s_SC_XOPEN_REALTIME = _SC_XOPEN_REALTIME;
const Int32 COsConstants::s_SC_XOPEN_REALTIME_THREADS = _SC_XOPEN_REALTIME_THREADS;
const Int32 COsConstants::s_SC_XOPEN_SHM = _SC_XOPEN_SHM;
const Int32 COsConstants::s_SC_XOPEN_UNIX = _SC_XOPEN_UNIX;
const Int32 COsConstants::s_SC_XOPEN_VERSION = _SC_XOPEN_VERSION;
const Int32 COsConstants::s_SC_XOPEN_XCU_VERSION = _SC_XOPEN_XCU_VERSION;

Boolean COsConstants::sInitlized = FALSE;
HashMap<String, Int32> COsConstants::sConstants(47);

Boolean COsConstants::_S_ISBLK(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFBLK;
}

Boolean COsConstants::_S_ISCHR(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFCHR;
}

Boolean COsConstants::_S_ISDIR(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFDIR;
}

Boolean COsConstants::_S_ISFIFO(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFIFO;
}

Boolean COsConstants::_S_ISREG(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFREG;
}

Boolean COsConstants::_S_ISLNK(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFLNK;
}

Boolean COsConstants::_S_ISSOCK(
    /* [in] */ Int32 mode)
{
    return (mode & sS_IFMT) == sS_IFSOCK;
}

Int32 COsConstants::_WEXITSTATUS(
    /* [in] */ Int32 status)
{
    return (status & 0xff00) >> 8;
}

Boolean COsConstants::_WCOREDUMP(
    /* [in] */ Int32 status)
{
    return (status & 0x80) != 0;
}

Int32 COsConstants::_WTERMSIG(
    /* [in] */ Int32 status)
{
    return status & 0x7f;
}

Int32 COsConstants::_WSTOPSIG(
    /* [in] */ Int32 status)
{
    return WEXITSTATUS(status);
}

Boolean COsConstants::_WIFEXITED(
    /* [in] */ Int32 status)
{
    return (WTERMSIG(status) == 0);
}

Boolean COsConstants::_WIFSTOPPED(
    /* [in] */ Int32 status)
{
    return (WTERMSIG(status) == 0x7f);
}

Boolean COsConstants::_WIFSIGNALED(
    /* [in] */ Int32 status)
{
    return (WTERMSIG(status + 1) >= 2);
}

String COsConstants::GaiName(
    /* [in] */ Int32 error)
{
    if (error == sEAI_AGAIN) {
        return String("EAI_AGAIN");
    }
    if (error == sEAI_BADFLAGS) {
        return String("EAI_BADFLAGS");
    }
    if (error == sEAI_FAIL) {
        return String("EAI_FAIL");
    }
    if (error == sEAI_FAMILY) {
        return String("EAI_FAMILY");
    }
    if (error == sEAI_MEMORY) {
        return String("EAI_MEMORY");
    }
    if (error == sEAI_NODATA) {
        return String("EAI_NODATA");
    }
    if (error == sEAI_NONAME) {
        return String("EAI_NONAME");
    }
    if (error == sEAI_OVERFLOW) {
        return String("EAI_OVERFLOW");
    }
    if (error == sEAI_SERVICE) {
        return String("EAI_SERVICE");
    }
    if (error == sEAI_SOCKTYPE) {
        return String("EAI_SOCKTYPE");
    }
    if (error == sEAI_SYSTEM) {
        return String("EAI_SYSTEM");
    }
    return String(NULL);
}

String COsConstants::ErrnoName(
    /* [in] */ Int32 errnum)
{
    if (errnum == sE2BIG) {
        return String("E2BIG");
    }
    if (errnum == sEACCES) {
        return String("EACCES");
    }
    if (errnum == sEADDRINUSE) {
        return String("EADDRINUSE");
    }
    if (errnum == sEADDRNOTAVAIL) {
        return String("EADDRNOTAVAIL");
    }
    if (errnum == sEAFNOSUPPORT) {
        return String("EAFNOSUPPORT");
    }
    if (errnum == sEAGAIN) {
        return String("EAGAIN");
    }
    if (errnum == sEALREADY) {
        return String("EALREADY");
    }
    if (errnum == sEBADF) {
        return String("EBADF");
    }
    if (errnum == sEBADMSG) {
        return String("EBADMSG");
    }
    if (errnum == sEBUSY) {
        return String("EBUSY");
    }
    if (errnum == sECANCELED) {
        return String("ECANCELED");
    }
    if (errnum == sECHILD) {
        return String("ECHILD");
    }
    if (errnum == sECONNABORTED) {
        return String("ECONNABORTED");
    }
    if (errnum == sECONNREFUSED) {
        return String("ECONNREFUSED");
    }
    if (errnum == sECONNRESET) {
        return String("ECONNRESET");
    }
    if (errnum == sEDEADLK) {
        return String("EDEADLK");
    }
    if (errnum == sEDESTADDRREQ) {
        return String("EDESTADDRREQ");
    }
    if (errnum == sEDOM) {
        return String("EDOM");
    }
    if (errnum == sEDQUOT) {
        return String("EDQUOT");
    }
    if (errnum == sEEXIST) {
        return String("EEXIST");
    }
    if (errnum == sEFAULT) {
        return String("EFAULT");
    }
    if (errnum == sEFBIG) {
        return String("EFBIG");
    }
    if (errnum == sEHOSTUNREACH) {
        return String("EHOSTUNREACH");
    }
    if (errnum == sEIDRM) {
        return String("EIDRM");
    }
    if (errnum == sEILSEQ) {
        return String("EILSEQ");
    }
    if (errnum == sEINPROGRESS) {
        return String("EINPROGRESS");
    }
    if (errnum == sEINTR) {
        return String("EINTR");
    }
    if (errnum == sEINVAL) {
        return String("EINVAL");
    }
    if (errnum == sEIO) {
        return String("EIO");
    }
    if (errnum == sEISCONN) {
        return String("EISCONN");
    }
    if (errnum == sEISDIR) {
        return String("EISDIR");
    }
    if (errnum == sELOOP) {
        return String("ELOOP");
    }
    if (errnum == sEMFILE) {
        return String("EMFILE");
    }
    if (errnum == sEMLINK) {
        return String("EMLINK");
    }
    if (errnum == sEMSGSIZE) {
        return String("EMSGSIZE");
    }
    if (errnum == sEMULTIHOP) {
        return String("EMULTIHOP");
    }
    if (errnum == sENAMETOOLONG) {
        return String("ENAMETOOLONG");
    }
    if (errnum == sENETDOWN) {
        return String("ENETDOWN");
    }
    if (errnum == sENETRESET) {
        return String("ENETRESET");
    }
    if (errnum == sENETUNREACH) {
        return String("ENETUNREACH");
    }
    if (errnum == sENFILE) {
        return String("ENFILE");
    }
    if (errnum == sENOBUFS) {
        return String("ENOBUFS");
    }
    if (errnum == sENODATA) {
        return String("ENODATA");
    }
    if (errnum == sENODEV) {
        return String("ENODEV");
    }
    if (errnum == sENOENT) {
        return String("ENOENT");
    }
    if (errnum == sENOEXEC) {
        return String("ENOEXEC");
    }
    if (errnum == sENOLCK) {
        return String("ENOLCK");
    }
    if (errnum == sENOLINK) {
        return String("ENOLINK");
    }
    if (errnum == sENOMEM) {
        return String("ENOMEM");
    }
    if (errnum == sENOMSG) {
        return String("ENOMSG");
    }
    if (errnum == sENOPROTOOPT) {
        return String("ENOPROTOOPT");
    }
    if (errnum == sENOSPC) {
        return String("ENOSPC");
    }
    if (errnum == sENOSR) {
        return String("ENOSR");
    }
    if (errnum == sENOSTR) {
        return String("ENOSTR");
    }
    if (errnum == sENOSYS) {
        return String("ENOSYS");
    }
    if (errnum == sENOTCONN) {
        return String("ENOTCONN");
    }
    if (errnum == sENOTDIR) {
        return String("ENOTDIR");
    }
    if (errnum == sENOTEMPTY) {
        return String("ENOTEMPTY");
    }
    if (errnum == sENOTSOCK) {
        return String("ENOTSOCK");
    }
    if (errnum == sENOTSUP) {
        return String("ENOTSUP");
    }
    if (errnum == sENOTTY) {
        return String("ENOTTY");
    }
    if (errnum == sENXIO) {
        return String("ENXIO");
    }
    if (errnum == sEOPNOTSUPP) {
        return String("EOPNOTSUPP");
    }
    if (errnum == sEOVERFLOW) {
        return String("EOVERFLOW");
    }
    if (errnum == sEPERM) {
        return String("EPERM");
    }
    if (errnum == sEPIPE) {
        return String("EPIPE");
    }
    if (errnum == sEPROTO) {
        return String("EPROTO");
    }
    if (errnum == sEPROTONOSUPPORT) {
        return String("EPROTONOSUPPORT");
    }
    if (errnum == sEPROTOTYPE) {
        return String("EPROTOTYPE");
    }
    if (errnum == sERANGE) {
        return String("ERANGE");
    }
    if (errnum == sEROFS) {
        return String("EROFS");
    }
    if (errnum == sESPIPE) {
        return String("ESPIPE");
    }
    if (errnum == sESRCH) {
        return String("ESRCH");
    }
    if (errnum == sESTALE) {
        return String("ESTALE");
    }
    if (errnum == sETIME) {
        return String("ETIME");
    }
    if (errnum == sETIMEDOUT) {
        return String("ETIMEDOUT");
    }
    if (errnum == sETXTBSY) {
        return String("ETXTBSY");
    }
    if (errnum == sEWOULDBLOCK) {
        return String("EWOULDBLOCK");
    }
    if (errnum == sEXDEV) {
        return String("EXDEV");
    }
    return String(NULL);
}

ECode COsConstants::_S_ISBLK(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISBLK(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISCHR(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISCHR(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISDIR(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISDIR(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISFIFO(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISFIFO(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISREG(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISREG(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISLNK(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISLNK(mode);
    return NOERROR;
}

ECode COsConstants::_S_ISSOCK(
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _S_ISSOCK(mode);
    return NOERROR;
}

ECode COsConstants::_WEXITSTATUS(
    /* [in] */ Int32 status,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WEXITSTATUS(status);
    return NOERROR;
}

ECode COsConstants::_WCOREDUMP(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WCOREDUMP(status);
    return NOERROR;
}

ECode COsConstants::_WTERMSIG(
    /* [in] */ Int32 status,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WTERMSIG(status);
    return NOERROR;
}

ECode COsConstants::_WSTOPSIG(
    /* [in] */ Int32 status,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WSTOPSIG(status);
    return NOERROR;
}

ECode COsConstants::_WIFEXITED(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WIFEXITED(status);
    return NOERROR;
}

ECode COsConstants::_WIFSTOPPED(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WIFSTOPPED(status);
    return NOERROR;
}

ECode COsConstants::_WIFSIGNALED(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = _WIFSIGNALED(status);
    return NOERROR;
}

ECode COsConstants::GaiName(
    /* [in] */ Int32 error,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = GaiName(error);
    return NOERROR;
}

ECode COsConstants::ErrnoName(
    /* [in] */ Int32 errnum,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = ErrnoName(errnum);
    return NOERROR;
}

ECode COsConstants::GetOsConstant(
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (!sInitlized) {
        Init();
        sInitlized = TRUE;
    }
    HashMap<String, Int32>::Iterator it = sConstants.Find(name);
    if (it != sConstants.End()) {
        *value = it->mSecond;
        return NOERROR;
    }
    *value = -1;
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

void COsConstants::Init()
{
    sConstants[String("AF_INET")] = AF_INET;
    sConstants[String("AF_INET6")] = AF_INET6;
    sConstants[String("AF_UNIX")] = AF_UNIX;
    sConstants[String("AF_UNSPEC")] = AF_UNSPEC;
    sConstants[String("AI_ADDRCONFIG")] = AI_ADDRCONFIG;
    sConstants[String("AI_ALL")] = AI_ALL;
    sConstants[String("AI_CANONNAME")] = AI_CANONNAME;
    sConstants[String("AI_NUMERICHOST")] = AI_NUMERICHOST;
    sConstants[String("AI_NUMERICSERV")] = AI_NUMERICSERV;
    sConstants[String("AI_PASSIVE")] = AI_PASSIVE;
    sConstants[String("AI_V4MAPPED")] = AI_V4MAPPED;
    sConstants[String("E2BIG")] = E2BIG;
    sConstants[String("EACCES")] = EACCES;
    sConstants[String("EADDRINUSE")] = EADDRINUSE;
    sConstants[String("EADDRNOTAVAIL")] = EADDRNOTAVAIL;
    sConstants[String("EAFNOSUPPORT")] = EAFNOSUPPORT;
    sConstants[String("EAGAIN")] = EAGAIN;
    sConstants[String("EAI_AGAIN")] = EAI_AGAIN;
    sConstants[String("EAI_BADFLAGS")] = EAI_BADFLAGS;
    sConstants[String("EAI_FAIL")] = EAI_FAIL;
    sConstants[String("EAI_FAMILY")] = EAI_FAMILY;
    sConstants[String("EAI_MEMORY")] = EAI_MEMORY;
    sConstants[String("EAI_NODATA")] = EAI_NODATA;
    sConstants[String("EAI_NONAME")] = EAI_NONAME;
    sConstants[String("EAI_OVERFLOW")] = EAI_OVERFLOW;
    sConstants[String("EAI_SERVICE")] = EAI_SERVICE;
    sConstants[String("EAI_SOCKTYPE")] = EAI_SOCKTYPE;
    sConstants[String("EAI_SYSTEM")] = EAI_SYSTEM;
    sConstants[String("EALREADY")] = EALREADY;
    sConstants[String("EBADF")] = EBADF;
    sConstants[String("EBADMSG")] = EBADMSG;
    sConstants[String("EBUSY")] = EBUSY;
    sConstants[String("ECANCELED")] = ECANCELED;
    sConstants[String("ECHILD")] = ECHILD;
    sConstants[String("ECONNABORTED")] = ECONNABORTED;
    sConstants[String("ECONNREFUSED")] = ECONNREFUSED;
    sConstants[String("ECONNRESET")] = ECONNRESET;
    sConstants[String("EDEADLK")] = EDEADLK;
    sConstants[String("EDESTADDRREQ")] = EDESTADDRREQ;
    sConstants[String("EDOM")] = EDOM;
    sConstants[String("EDQUOT")] = EDQUOT;
    sConstants[String("EEXIST")] = EEXIST;
    sConstants[String("EFAULT")] = EFAULT;
    sConstants[String("EFBIG")] = EFBIG;
    sConstants[String("EHOSTUNREACH")] = EHOSTUNREACH;
    sConstants[String("EIDRM")] = EIDRM;
    sConstants[String("EILSEQ")] = EILSEQ;
    sConstants[String("EINPROGRESS")] = EINPROGRESS;
    sConstants[String("EINTR")] = EINTR;
    sConstants[String("EINVAL")] = EINVAL;
    sConstants[String("EIO")] = EIO;
    sConstants[String("EISCONN")] = EISCONN;
    sConstants[String("EISDIR")] = EISDIR;
    sConstants[String("ELOOP")] = ELOOP;
    sConstants[String("EMFILE")] = EMFILE;
    sConstants[String("EMLINK")] = EMLINK;
    sConstants[String("EMSGSIZE")] = EMSGSIZE;
    sConstants[String("EMULTIHOP")] = EMULTIHOP;
    sConstants[String("ENAMETOOLONG")] = ENAMETOOLONG;
    sConstants[String("ENETDOWN")] = ENETDOWN;
    sConstants[String("ENETRESET")] = ENETRESET;
    sConstants[String("ENETUNREACH")] = ENETUNREACH;
    sConstants[String("ENFILE")] = ENFILE;
    sConstants[String("ENOBUFS")] = ENOBUFS;
    sConstants[String("ENODATA")] = ENODATA;
    sConstants[String("ENODEV")] = ENODEV;
    sConstants[String("ENOENT")] = ENOENT;
    sConstants[String("ENOEXEC")] = ENOEXEC;
    sConstants[String("ENOLCK")] = ENOLCK;
    sConstants[String("ENOLINK")] = ENOLINK;
    sConstants[String("ENOMEM")] = ENOMEM;
    sConstants[String("ENOMSG")] = ENOMSG;
    sConstants[String("ENOPROTOOPT")] = ENOPROTOOPT;
    sConstants[String("ENOSPC")] = ENOSPC;
    sConstants[String("ENOSR")] = ENOSR;
    sConstants[String("ENOSTR")] = ENOSTR;
    sConstants[String("ENOSYS")] = ENOSYS;
    sConstants[String("ENOTCONN")] = ENOTCONN;
    sConstants[String("ENOTDIR")] = ENOTDIR;
    sConstants[String("ENOTEMPTY")] = ENOTEMPTY;
    sConstants[String("ENOTSOCK")] = ENOTSOCK;
    sConstants[String("ENOTSUP")] = ENOTSUP;
    sConstants[String("ENOTTY")] = ENOTTY;
    sConstants[String("ENXIO")] = ENXIO;
    sConstants[String("EOPNOTSUPP")] = EOPNOTSUPP;
    sConstants[String("EOVERFLOW")] = EOVERFLOW;
    sConstants[String("EPERM")] = EPERM;
    sConstants[String("EPIPE")] = EPIPE;
    sConstants[String("EPROTO")] = EPROTO;
    sConstants[String("EPROTONOSUPPORT")] = EPROTONOSUPPORT;
    sConstants[String("EPROTOTYPE")] = EPROTOTYPE;
    sConstants[String("ERANGE")] = ERANGE;
    sConstants[String("EROFS")] = EROFS;
    sConstants[String("ESPIPE")] = ESPIPE;
    sConstants[String("ESRCH")] = ESRCH;
    sConstants[String("ESTALE")] = ESTALE;
    sConstants[String("ETIME")] = ETIME;
    sConstants[String("ETIMEDOUT")] = ETIMEDOUT;
    sConstants[String("ETXTBSY")] = ETXTBSY;
    sConstants[String("EWOULDBLOCK")] = EWOULDBLOCK;
    sConstants[String("EXDEV")] = EXDEV;
    sConstants[String("EXIT_FAILURE")] = EXIT_FAILURE;
    sConstants[String("EXIT_SUCCESS")] = EXIT_SUCCESS;
    sConstants[String("FD_CLOEXEC")] = FD_CLOEXEC;
    sConstants[String("FIONREAD")] = FIONREAD;
    sConstants[String("F_DUPFD")] = F_DUPFD;
    sConstants[String("F_GETFD")] = F_GETFD;
    sConstants[String("F_GETFL")] = F_GETFL;
    sConstants[String("F_GETLK")] = F_GETLK;
    sConstants[String("F_GETLK64")] = F_GETLK64;
    sConstants[String("F_GETOWN")] = F_GETOWN;
    sConstants[String("F_OK")] = F_OK;
    sConstants[String("F_RDLCK")] = F_RDLCK;
    sConstants[String("F_SETFD")] = F_SETFD;
    sConstants[String("F_SETFL")] = F_SETFL;
    sConstants[String("F_SETLK")] = F_SETLK;
    sConstants[String("F_SETLK64")] = F_SETLK64;
    sConstants[String("F_SETLKW")] = F_SETLKW;
    sConstants[String("F_SETLKW64")] = F_SETLKW64;
    sConstants[String("F_SETOWN")] = F_SETOWN;
    sConstants[String("F_UNLCK")] = F_UNLCK;
    sConstants[String("F_WRLCKv")] = F_WRLCK;
    sConstants[String("IFF_ALLMULTI")] = IFF_ALLMULTI;
    sConstants[String("IFF_AUTOMEDIA")] = IFF_AUTOMEDIA;
    sConstants[String("IFF_BROADCAST")] = IFF_BROADCAST;
    sConstants[String("IFF_DEBUG")] = IFF_DEBUG;
    sConstants[String("IFF_DYNAMIC")] = IFF_DYNAMIC;
    sConstants[String("IFF_LOOPBACK")] = IFF_LOOPBACK;
    sConstants[String("IFF_MASTER")] = IFF_MASTER;
    sConstants[String("IFF_MULTICAST")] = IFF_MULTICAST;
    sConstants[String("IFF_NOARP")] = IFF_NOARP;
    sConstants[String("IFF_NOTRAILERS")] = IFF_NOTRAILERS;
    sConstants[String("IFF_POINTOPOINT")] = IFF_POINTOPOINT;
    sConstants[String("IFF_PORTSEL")] = IFF_PORTSEL;
    sConstants[String("IFF_PROMISC")] = IFF_PROMISC;
    sConstants[String("IFF_RUNNING")] = IFF_RUNNING;
    sConstants[String("IFF_SLAVE")] = IFF_SLAVE;
    sConstants[String("IFF_UP")] = IFF_UP;
    sConstants[String("IPPROTO_ICMP")] = IPPROTO_ICMP;
    sConstants[String("IPPROTO_IP")] = IPPROTO_IP;
    sConstants[String("IPPROTO_IPV6")] = IPPROTO_IPV6;
    sConstants[String("IPPROTO_RAW")] = IPPROTO_RAW;
    sConstants[String("IPPROTO_TCP")] = IPPROTO_TCP;
    sConstants[String("IPPROTO_UDP")] = IPPROTO_UDP;
    sConstants[String("IPV6_CHECKSUM")] = IPV6_CHECKSUM;
    sConstants[String("IPV6_MULTICAST_HOPS")] = IPV6_MULTICAST_HOPS;
    sConstants[String("IPV6_MULTICAST_IF")] = IPV6_MULTICAST_IF;
    sConstants[String("IPV6_MULTICAST_LOOP")] = IPV6_MULTICAST_LOOP;
    sConstants[String("IPV6_RECVDSTOPTS")] = IPV6_RECVDSTOPTS;
    sConstants[String("IPV6_RECVHOPLIMIT")] = IPV6_RECVHOPLIMIT;
    sConstants[String("IPV6_RECVHOPOPTS")] = IPV6_RECVHOPOPTS;
    sConstants[String("IPV6_RECVPKTINFO")] = IPV6_RECVPKTINFO;
    sConstants[String("IPV6_RECVRTHDR")] = IPV6_RECVRTHDR;
    sConstants[String("IPV6_RECVTCLASS")] = IPV6_RECVTCLASS;
    sConstants[String("IPV6_TCLASS")] = IPV6_TCLASS;
    sConstants[String("IPV6_UNICAST_HOPS")] = IPV6_UNICAST_HOPS;
    sConstants[String("IPV6_V6ONLY")] = IPV6_V6ONLY;
    sConstants[String("IP_MULTICAST_IF")] = IP_MULTICAST_IF;
    sConstants[String("IP_MULTICAST_LOOP")] = IP_MULTICAST_LOOP;
    sConstants[String("IP_MULTICAST_TTL")] = IP_MULTICAST_TTL;
    sConstants[String("IP_TOS")] = IP_TOS;
    sConstants[String("IP_TTL")] = IP_TTL;
    sConstants[String("MAP_FIXED")] = MAP_FIXED;
    sConstants[String("MAP_PRIVATE")] = MAP_PRIVATE;
    sConstants[String("MAP_SHARED")] = MAP_SHARED;
    sConstants[String("MCAST_JOIN_GROUP")] = MCAST_JOIN_GROUP;
    sConstants[String("MCAST_LEAVE_GROUP")] = MCAST_LEAVE_GROUP;
    sConstants[String("MCL_CURRENT")] = MCL_CURRENT;
    sConstants[String("MCL_FUTURE")] = MCL_FUTURE;
    sConstants[String("MSG_CTRUNC")] = MSG_CTRUNC;
    sConstants[String("MSG_DONTROUTE")] = MSG_DONTROUTE;
    sConstants[String("MSG_EOR")] = MSG_EOR;
    sConstants[String("MSG_OOB")] = MSG_OOB;
    sConstants[String("MSG_PEEK")] = MSG_PEEK;
    sConstants[String("MSG_TRUNC")] = MSG_TRUNC;
    sConstants[String("MSG_WAITALL")] = MSG_WAITALL;
    sConstants[String("MS_ASYNC")] = MS_ASYNC;
    sConstants[String("MS_INVALIDATE")] = MS_INVALIDATE;
    sConstants[String("MS_SYNC")] = MS_SYNC;
    sConstants[String("NI_DGRAM")] = NI_DGRAM;
    sConstants[String("NI_NAMEREQD")] = NI_NAMEREQD;
    sConstants[String("NI_NOFQDN")] = NI_NOFQDN;
    sConstants[String("NI_NUMERICHOST")] = NI_NUMERICHOST;
    sConstants[String("NI_NUMERICSERV")] = NI_NUMERICSERV;
    sConstants[String("O_ACCMODE")] = O_ACCMODE;
    sConstants[String("O_APPEND")] = O_APPEND;
    sConstants[String("O_CREAT")] = O_CREAT;
    sConstants[String("O_EXCL")] = O_EXCL;
    sConstants[String("O_NOCTTY")] = O_NOCTTY;
    sConstants[String("O_NOFOLLOW")] = O_NOFOLLOW;
    sConstants[String("O_NONBLOCK")] = O_NONBLOCK;
    sConstants[String("O_RDONLY")] = O_RDONLY;
    sConstants[String("O_RDWR")] = O_RDWR;
    sConstants[String("O_SYNC")] = O_SYNC;
    sConstants[String("O_TRUNC")] = O_TRUNC;
    sConstants[String("O_WRONLY")] = O_WRONLY;
    sConstants[String("POLLERR")] = POLLERR;
    sConstants[String("POLLHUP")] = POLLHUP;
    sConstants[String("POLLIN")] = POLLIN;
    sConstants[String("POLLNVAL")] = POLLNVAL;
    sConstants[String("POLLOUT")] = POLLOUT;
    sConstants[String("POLLPRI")] = POLLPRI;
    sConstants[String("POLLRDBAND")] = POLLRDBAND;
    sConstants[String("POLLRDNORM")] = POLLRDNORM;
    sConstants[String("POLLWRBAND")] = POLLWRBAND;
    sConstants[String("POLLWRNORM")] = POLLWRNORM;
    sConstants[String("PROT_EXEC")] = PROT_EXEC;
    sConstants[String("PROT_NONE")] = PROT_NONE;
    sConstants[String("PROT_READ")] = PROT_READ;
    sConstants[String("PROT_WRITE")] = PROT_WRITE;
    sConstants[String("R_OK")] = R_OK;
    sConstants[String("SEEK_CUR")] = SEEK_CUR;
    sConstants[String("SEEK_END")] = SEEK_END;
    sConstants[String("SEEK_SET")] = SEEK_SET;
    sConstants[String("SHUT_RD")] = SHUT_RD;
    sConstants[String("SHUT_RDWR")] = SHUT_RDWR;
    sConstants[String("SHUT_WR")] = SHUT_WR;
    sConstants[String("SIGABRT")] = SIGABRT;
    sConstants[String("SIGALRM")] = SIGALRM;
    sConstants[String("SIGBUS")] = SIGBUS;
    sConstants[String("SIGCHLD")] = SIGCHLD;
    sConstants[String("SIGCONT")] = SIGCONT;
    sConstants[String("SIGFPE")] = SIGFPE;
    sConstants[String("SIGHUP")] = SIGHUP;
    sConstants[String("SIGILLv")] = SIGILL;
    sConstants[String("SIGINT")] = SIGINT;
    sConstants[String("SIGIO")] = SIGIO;
    sConstants[String("SIGKILL")] = SIGKILL;
    sConstants[String("SIGPIPE")] = SIGPIPE;
    sConstants[String("SIGPROF")] = SIGPROF;
    sConstants[String("SIGPWR")] = SIGPWR;
    sConstants[String("SIGQUIT")] = SIGQUIT;
    sConstants[String("SIGRTMAX")] = SIGRTMAX;
    sConstants[String("SIGRTMIN")] = SIGRTMIN;
    sConstants[String("SIGSEGV")] = SIGSEGV;
    sConstants[String("SIGSTKFLT")] = SIGSTKFLT;
    sConstants[String("SIGSTOP")] = SIGSTOP;
    sConstants[String("SIGSYS")] = SIGSYS;
    sConstants[String("SIGTERM")] = SIGTERM;
    sConstants[String("SIGTRAP")] = SIGTRAP;
    sConstants[String("SIGTSTP")] = SIGTSTP;
    sConstants[String("SIGTTIN")] = SIGTTIN;
    sConstants[String("SIGTTOU")] = SIGTTOU;
    sConstants[String("SIGURG")] = SIGURG;
    sConstants[String("SIGUSR1")] = SIGUSR1;
    sConstants[String("SIGUSR2")] = SIGUSR2;
    sConstants[String("SIGVTALRM")] = SIGVTALRM;
    sConstants[String("SIGWINCH")] = SIGWINCH;
    sConstants[String("SIGXCPU")] = SIGXCPU;
    sConstants[String("SIGXFSZ")] = SIGXFSZ;
    sConstants[String("SIOCGIFADDR")] = SIOCGIFADDR;
    sConstants[String("SIOCGIFBRDADDR")] = SIOCGIFBRDADDR;
    sConstants[String("SIOCGIFDSTADDR")] = SIOCGIFDSTADDR;
    sConstants[String("SIOCGIFNETMASK")] = SIOCGIFNETMASK;
    sConstants[String("SOCK_DGRAM")] = SOCK_DGRAM;
    sConstants[String("SOCK_RAW")] = SOCK_RAW;
    sConstants[String("SOCK_SEQPACKET")] = SOCK_SEQPACKET;
    sConstants[String("SOCK_STREAM")] = SOCK_STREAM;
    sConstants[String("SOL_SOCKET")] = SOL_SOCKET;
    sConstants[String("SO_BINDTODEVICE")] = SO_BINDTODEVICE;
    sConstants[String("SO_BROADCAST")] = SO_BROADCAST;
    sConstants[String("SO_DEBUG")] = SO_DEBUG;
    sConstants[String("SO_DONTROUTE")] = SO_DONTROUTE;
    sConstants[String("SO_ERROR")] = SO_ERROR;
    sConstants[String("SO_KEEPALIVE")] = SO_KEEPALIVE;
    sConstants[String("SO_LINGER")] = SO_LINGER;
    sConstants[String("SO_OOBINLINE")] = SO_OOBINLINE;
    sConstants[String("SO_RCVBUF")] = SO_RCVBUF;
    sConstants[String("SO_RCVLOWAT")] = SO_RCVLOWAT;
    sConstants[String("SO_RCVTIMEO")] = SO_RCVTIMEO;
    sConstants[String("SO_REUSEADDR")] = SO_REUSEADDR;
    sConstants[String("SO_SNDBUF")] = SO_SNDBUF;
    sConstants[String("SO_SNDLOWAT")] = SO_SNDLOWAT;
    sConstants[String("SO_SNDTIMEO")] = SO_SNDTIMEO;
    sConstants[String("SO_TYPE")] = SO_TYPE;
    sConstants[String("STDERR_FILENO")] = STDERR_FILENO;
    sConstants[String("STDIN_FILENO")] = STDIN_FILENO;
    sConstants[String("STDOUT_FILENO")] = STDOUT_FILENO;
    sConstants[String("S_IFBLK")] = S_IFBLK;
    sConstants[String("S_IFCHR")] = S_IFCHR;
    sConstants[String("S_IFDIR")] = S_IFDIR;
    sConstants[String("S_IFIFO")] = S_IFIFO;
    sConstants[String("S_IFLNK")] = S_IFLNK;
    sConstants[String("S_IFMT")] = S_IFMT;
    sConstants[String("S_IFREG")] = S_IFREG;
    sConstants[String("S_IFSOCK")] = S_IFSOCK;
    sConstants[String("S_IRGRP")] = S_IRGRP;
    sConstants[String("S_IROTH")] = S_IROTH;
    sConstants[String("S_IRUSR")] = S_IRUSR;
    sConstants[String("S_IRWXG")] = S_IRWXG;
    sConstants[String("S_IRWXO")] = S_IRWXO;
    sConstants[String("S_IRWXU")] = S_IRWXU;
    sConstants[String("S_ISGID")] = S_ISGID;
    sConstants[String("S_ISUID")] = S_ISUID;
    sConstants[String("S_ISVTX")] = S_ISVTX;
    sConstants[String("S_IWGRP")] = S_IWGRP;
    sConstants[String("S_IWOTH")] = S_IWOTH;
    sConstants[String("S_IWUSR")] = S_IWUSR;
    sConstants[String("S_IXGRP")] = S_IXGRP;
    sConstants[String("S_IXOTH")] = S_IXOTH;
    sConstants[String("S_IXUSR")] = S_IXUSR;
    sConstants[String("TCP_NODELAY")] = TCP_NODELAY;
    sConstants[String("WCONTINUED")] = WCONTINUED;
    sConstants[String("WEXITED")] = WEXITED;
    sConstants[String("WNOHANG")] = WNOHANG;
    sConstants[String("WNOWAIT")] = WNOWAIT;
    sConstants[String("WSTOPPED")] = WSTOPPED;
    sConstants[String("WUNTRACED")] = WUNTRACED;
    sConstants[String("W_OK")] = W_OK;
    sConstants[String("X_OK")] = X_OK;
    sConstants[String("_SC_2_CHAR_TERM")] = _SC_2_CHAR_TERM;
    sConstants[String("_SC_2_C_BIND")] = _SC_2_C_BIND;
    sConstants[String("_SC_2_C_DEV")] = _SC_2_C_DEV;
    sConstants[String("_SC_2_C_VERSION")] = _SC_2_C_VERSION;
    sConstants[String("_SC_2_FORT_DEV")] = _SC_2_FORT_DEV;
    sConstants[String("_SC_2_FORT_RUN")] = _SC_2_FORT_RUN;
    sConstants[String("_SC_2_LOCALEDEF")] = _SC_2_LOCALEDEF;
    sConstants[String("_SC_2_SW_DEV")] = _SC_2_SW_DEV;
    sConstants[String("_SC_2_UPE")] = _SC_2_UPE;
    sConstants[String("_SC_2_VERSION")] = _SC_2_VERSION;
    sConstants[String("_SC_AIO_LISTIO_MAX")] = _SC_AIO_LISTIO_MAX;
    sConstants[String("_SC_AIO_MAX")] = _SC_AIO_MAX;
    sConstants[String("_SC_AIO_PRIO_DELTA_MAX")] = _SC_AIO_PRIO_DELTA_MAX;
    sConstants[String("_SC_ARG_MAX")] = _SC_ARG_MAX;
    sConstants[String("_SC_ASYNCHRONOUS_IO")] = _SC_ASYNCHRONOUS_IO;
    sConstants[String("_SC_ATEXIT_MAX")] = _SC_ATEXIT_MAX;
    sConstants[String("_SC_AVPHYS_PAGES")] = _SC_AVPHYS_PAGES;
    sConstants[String("_SC_BC_BASE_MAX")] = _SC_BC_BASE_MAX;
    sConstants[String("_SC_BC_DIM_MAX")] = _SC_BC_DIM_MAX;
    sConstants[String("_SC_BC_SCALE_MAX")] = _SC_BC_SCALE_MAX;
    sConstants[String("_SC_BC_STRING_MAX")] = _SC_BC_STRING_MAX;
    sConstants[String("_SC_CHILD_MAX")] = _SC_CHILD_MAX;
    sConstants[String("_SC_CLK_TCK")] = _SC_CLK_TCK;
    sConstants[String("_SC_COLL_WEIGHTS_MAX")] = _SC_COLL_WEIGHTS_MAX;
    sConstants[String("_SC_DELAYTIMER_MAX")] = _SC_DELAYTIMER_MAX;
    sConstants[String("_SC_EXPR_NEST_MAX")] = _SC_EXPR_NEST_MAX;
    sConstants[String("_SC_FSYNC")] = _SC_FSYNC;
    sConstants[String("_SC_GETGR_R_SIZE_MAX")] = _SC_GETGR_R_SIZE_MAX;
    sConstants[String("_SC_GETPW_R_SIZE_MAX")] = _SC_GETPW_R_SIZE_MAX;
    sConstants[String("_SC_IOV_MAX")] = _SC_IOV_MAX;
    sConstants[String("_SC_JOB_CONTROL")] = _SC_JOB_CONTROL;
    sConstants[String("_SC_LINE_MAX")] = _SC_LINE_MAX;
    sConstants[String("_SC_LOGIN_NAME_MAX")] = _SC_LOGIN_NAME_MAX;
    sConstants[String("_SC_MAPPED_FILES")] = _SC_MAPPED_FILES;
    sConstants[String("_SC_MEMLOCK")] = _SC_MEMLOCK;
    sConstants[String("_SC_MEMLOCK_RANGE")] = _SC_MEMLOCK_RANGE;
    sConstants[String("_SC_MEMORY_PROTECTION")] = _SC_MEMORY_PROTECTION;
    sConstants[String("_SC_MESSAGE_PASSING")] = _SC_MESSAGE_PASSING;
    sConstants[String("_SC_MQ_OPEN_MAX")] = _SC_MQ_OPEN_MAX;
    sConstants[String("_SC_MQ_PRIO_MAX")] = _SC_MQ_PRIO_MAX;
    sConstants[String("_SC_NGROUPS_MAX")] = _SC_NGROUPS_MAX;
    sConstants[String("_SC_NPROCESSORS_CONF")] = _SC_NPROCESSORS_CONF;
    sConstants[String("_SC_NPROCESSORS_ONLN")] = _SC_NPROCESSORS_ONLN;
    sConstants[String("_SC_OPEN_MAX")] = _SC_OPEN_MAX;
    sConstants[String("_SC_PAGESIZE")] = _SC_PAGESIZE;
    sConstants[String("_SC_PAGE_SIZE")] = _SC_PAGE_SIZE;
    sConstants[String("_SC_PASS_MAX")] = _SC_PASS_MAX;
    sConstants[String("_SC_PHYS_PAGES")] = _SC_PHYS_PAGES;
    sConstants[String("_SC_PRIORITIZED_IO")] = _SC_PRIORITIZED_IO;
    sConstants[String("_SC_PRIORITY_SCHEDULING")] = _SC_PRIORITY_SCHEDULING;
    sConstants[String("_SC_REALTIME_SIGNALS")] = _SC_REALTIME_SIGNALS;
    sConstants[String("_SC_RE_DUP_MAX")] = _SC_RE_DUP_MAX;
    sConstants[String("_SC_RTSIG_MAX")] = _SC_RTSIG_MAX;
    sConstants[String("_SC_SAVED_IDS")] = _SC_SAVED_IDS;
    sConstants[String("_SC_SEMAPHORES")] = _SC_SEMAPHORES;
    sConstants[String("_SC_SEM_NSEMS_MAX")] = _SC_SEM_NSEMS_MAX;
    sConstants[String("_SC_SEM_VALUE_MAX")] = _SC_SEM_VALUE_MAX;
    sConstants[String("_SC_SHARED_MEMORY_OBJECTS")] = _SC_SHARED_MEMORY_OBJECTS;
    sConstants[String("_SC_SIGQUEUE_MAX")] = _SC_SIGQUEUE_MAX;
    sConstants[String("_SC_STREAM_MAX")] = _SC_STREAM_MAX;
    sConstants[String("_SC_SYNCHRONIZED_IO")] = _SC_SYNCHRONIZED_IO;
    sConstants[String("_SC_THREADS")] = _SC_THREADS;
    sConstants[String("_SC_THREAD_ATTR_STACKADDR")] = _SC_THREAD_ATTR_STACKADDR;
    sConstants[String("_SC_THREAD_ATTR_STACKSIZE")] = _SC_THREAD_ATTR_STACKSIZE;
    sConstants[String("_SC_THREAD_DESTRUCTOR_ITERATIONS")] = _SC_THREAD_DESTRUCTOR_ITERATIONS;
    sConstants[String("_SC_THREAD_KEYS_MAX")] = _SC_THREAD_KEYS_MAX;
    sConstants[String("_SC_THREAD_PRIORITY_SCHEDULING")] = _SC_THREAD_PRIORITY_SCHEDULING;
    sConstants[String("_SC_THREAD_PRIO_INHERIT")] = _SC_THREAD_PRIO_INHERIT;
    sConstants[String("_SC_THREAD_PRIO_PROTECT")] = _SC_THREAD_PRIO_PROTECT;
    sConstants[String("_SC_THREAD_SAFE_FUNCTIONS")] = _SC_THREAD_SAFE_FUNCTIONS;
    sConstants[String("_SC_THREAD_STACK_MIN")] = _SC_THREAD_STACK_MIN;
    sConstants[String("_SC_THREAD_THREADS_MAX")] = _SC_THREAD_THREADS_MAX;
    sConstants[String("_SC_TIMERS")] = _SC_TIMERS;
    sConstants[String("_SC_TIMER_MAX")] = _SC_TIMER_MAX;
    sConstants[String("_SC_TTY_NAME_MAX")] = _SC_TTY_NAME_MAX;
    sConstants[String("_SC_TZNAME_MAX")] = _SC_TZNAME_MAX;
    sConstants[String("_SC_VERSION")] = _SC_VERSION;
    sConstants[String("_SC_XBS5_ILP32_OFF32")] = _SC_XBS5_ILP32_OFF32;
    sConstants[String("_SC_XBS5_ILP32_OFFBIG")] = _SC_XBS5_ILP32_OFFBIG;
    sConstants[String("_SC_XBS5_LP64_OFF64")] = _SC_XBS5_LP64_OFF64;
    sConstants[String("_SC_XBS5_LPBIG_OFFBIG")] = _SC_XBS5_LPBIG_OFFBIG;
    sConstants[String("_SC_XOPEN_CRYPT")] = _SC_XOPEN_CRYPT;
    sConstants[String("_SC_XOPEN_ENH_I18N")] = _SC_XOPEN_ENH_I18N;
    sConstants[String("_SC_XOPEN_LEGACY")] = _SC_XOPEN_LEGACY;
    sConstants[String("_SC_XOPEN_REALTIME")] = _SC_XOPEN_REALTIME;
    sConstants[String("_SC_XOPEN_REALTIME_THREADS")] = _SC_XOPEN_REALTIME_THREADS;
    sConstants[String("_SC_XOPEN_SHM")] = _SC_XOPEN_SHM;
    sConstants[String("_SC_XOPEN_UNIX")] = _SC_XOPEN_UNIX;
    sConstants[String("_SC_XOPEN_VERSION")] = _SC_XOPEN_VERSION;
    sConstants[String("_SC_XOPEN_XCU_VERSION")] = _SC_XOPEN_XCU_VERSION;
}

} // namespace IO
} // namespace Libcore
