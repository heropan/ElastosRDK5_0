
#ifndef __COSCONSTANTS_H__
#define __COSCONSTANTS_H__

#include "_COsConstants.h"
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;

namespace Libcore {
namespace IO {

CarClass(COsConstants)
{
public:
    CARAPI _S_ISBLK(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISCHR(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISDIR(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISFIFO(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISREG(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISLNK(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _S_ISSOCK(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI _WEXITSTATUS(
        /* [in] */ Int32 status,
        /* [out] */ Int32* result);

    CARAPI _WCOREDUMP(
        /* [in] */ Int32 status,
        /* [out] */ Boolean* result);

    CARAPI _WTERMSIG(
        /* [in] */ Int32 status,
        /* [out] */ Int32* result);

    CARAPI _WSTOPSIG(
        /* [in] */ Int32 status,
        /* [out] */ Int32* result);

    CARAPI _WIFEXITED(
        /* [in] */ Int32 status,
        /* [out] */ Boolean* result);

    CARAPI _WIFSTOPPED(
        /* [in] */ Int32 status,
        /* [out] */ Boolean* result);

    CARAPI _WIFSIGNALED(
        /* [in] */ Int32 status,
        /* [out] */ Boolean* result);

    CARAPI GaiName(
        /* [in] */ Int32 error,
        /* [out] */ String* name);

    CARAPI ErrnoName(
        /* [in] */ Int32 errnum,
        /* [out] */ String* mame);

    CARAPI GetOsConstant(
        /* [in] */ const String& name,
        /* [out] */ Int32* value);

    static CARAPI_(Boolean) _S_ISBLK(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISCHR(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISDIR(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISFIFO(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISREG(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISLNK(
        /* [in] */ Int32 mode);

    static CARAPI_(Boolean) _S_ISSOCK(
        /* [in] */ Int32 mode);

    static CARAPI_(Int32) _WEXITSTATUS(
        /* [in] */ Int32 status);

    static CARAPI_(Boolean) _WCOREDUMP(
        /* [in] */ Int32 status);

    static CARAPI_(Int32) _WTERMSIG(
        /* [in] */ Int32 status);

    static CARAPI_(Int32) _WSTOPSIG(
        /* [in] */ Int32 status);

    static CARAPI_(Boolean) _WIFEXITED(
        /* [in] */ Int32 status);

    static CARAPI_(Boolean) _WIFSTOPPED(
        /* [in] */ Int32 status);

    static CARAPI_(Boolean) _WIFSIGNALED(
        /* [in] */ Int32 status);

    static CARAPI_(String) GaiName(
        /* [in] */ Int32 error);

    static CARAPI_(String) ErrnoName(
        /* [in] */ Int32 errnum);

private:
    static CARAPI_(void) Init();

public:
    static const Int32 sAF_INET;
    static const Int32 sAF_INET6;
    static const Int32 sAF_UNIX;
    static const Int32 sAF_UNSPEC;
    static const Int32 sAI_ADDRCONFIG;
    static const Int32 sAI_ALL;
    static const Int32 sAI_CANONNAME;
    static const Int32 sAI_NUMERICHOST;
    static const Int32 sAI_NUMERICSERV;
    static const Int32 sAI_PASSIVE;
    static const Int32 sAI_V4MAPPED;
    static const Int32 sE2BIG;
    static const Int32 sEACCES;
    static const Int32 sEADDRINUSE;
    static const Int32 sEADDRNOTAVAIL;
    static const Int32 sEAFNOSUPPORT;
    static const Int32 sEAGAIN;
    static const Int32 sEAI_AGAIN;
    static const Int32 sEAI_BADFLAGS;
    static const Int32 sEAI_FAIL;
    static const Int32 sEAI_FAMILY;
    static const Int32 sEAI_MEMORY;
    static const Int32 sEAI_NODATA;
    static const Int32 sEAI_NONAME;
    static const Int32 sEAI_OVERFLOW;
    static const Int32 sEAI_SERVICE;
    static const Int32 sEAI_SOCKTYPE;
    static const Int32 sEAI_SYSTEM;
    static const Int32 sEALREADY;
    static const Int32 sEBADF;
    static const Int32 sEBADMSG;
    static const Int32 sEBUSY;
    static const Int32 sECANCELED;
    static const Int32 sECHILD;
    static const Int32 sECONNABORTED;
    static const Int32 sECONNREFUSED;
    static const Int32 sECONNRESET;
    static const Int32 sEDEADLK;
    static const Int32 sEDESTADDRREQ;
    static const Int32 sEDOM;
    static const Int32 sEDQUOT;
    static const Int32 sEEXIST;
    static const Int32 sEFAULT;
    static const Int32 sEFBIG;
    static const Int32 sEHOSTUNREACH;
    static const Int32 sEIDRM;
    static const Int32 sEILSEQ;
    static const Int32 sEINPROGRESS;
    static const Int32 sEINTR;
    static const Int32 sEINVAL;
    static const Int32 sEIO;
    static const Int32 sEISCONN;
    static const Int32 sEISDIR;
    static const Int32 sELOOP;
    static const Int32 sEMFILE;
    static const Int32 sEMLINK;
    static const Int32 sEMSGSIZE;
    static const Int32 sEMULTIHOP;
    static const Int32 sENAMETOOLONG;
    static const Int32 sENETDOWN;
    static const Int32 sENETRESET;
    static const Int32 sENETUNREACH;
    static const Int32 sENFILE;
    static const Int32 sENOBUFS;
    static const Int32 sENODATA;
    static const Int32 sENODEV;
    static const Int32 sENOENT;
    static const Int32 sENOEXEC;
    static const Int32 sENOLCK;
    static const Int32 sENOLINK;
    static const Int32 sENOMEM;
    static const Int32 sENOMSG;
    static const Int32 sENOPROTOOPT;
    static const Int32 sENOSPC;
    static const Int32 sENOSR;
    static const Int32 sENOSTR;
    static const Int32 sENOSYS;
    static const Int32 sENOTCONN;
    static const Int32 sENOTDIR;
    static const Int32 sENOTEMPTY;
    static const Int32 sENOTSOCK;
    static const Int32 sENOTSUP;
    static const Int32 sENOTTY;
    static const Int32 sENXIO;
    static const Int32 sEOPNOTSUPP;
    static const Int32 sEOVERFLOW;
    static const Int32 sEPERM;
    static const Int32 sEPIPE;
    static const Int32 sEPROTO;
    static const Int32 sEPROTONOSUPPORT;
    static const Int32 sEPROTOTYPE;
    static const Int32 sERANGE;
    static const Int32 sEROFS;
    static const Int32 sESPIPE;
    static const Int32 sESRCH;
    static const Int32 sESTALE;
    static const Int32 sETIME;
    static const Int32 sETIMEDOUT;
    static const Int32 sETXTBSY;
    static const Int32 sEWOULDBLOCK;
    static const Int32 sEXDEV;
    static const Int32 sEXIT_FAILURE;
    static const Int32 sEXIT_SUCCESS;
    static const Int32 sFD_CLOEXEC;
    static const Int32 sFIONREAD;
    static const Int32 sF_DUPFD;
    static const Int32 sF_GETFD;
    static const Int32 sF_GETFL;
    static const Int32 sF_GETLK;
    static const Int32 sF_GETLK64;
    static const Int32 sF_GETOWN;
    static const Int32 sF_OK;
    static const Int32 sF_RDLCK;
    static const Int32 sF_SETFD;
    static const Int32 sF_SETFL;
    static const Int32 sF_SETLK;
    static const Int32 sF_SETLK64;
    static const Int32 sF_SETLKW;
    static const Int32 sF_SETLKW64;
    static const Int32 sF_SETOWN;
    static const Int32 sF_UNLCK;
    static const Int32 sF_WRLCK;
    static const Int32 sIFF_ALLMULTI;
    static const Int32 sIFF_AUTOMEDIA;
    static const Int32 sIFF_BROADCAST;
    static const Int32 sIFF_DEBUG;
    static const Int32 sIFF_DYNAMIC;
    static const Int32 sIFF_LOOPBACK;
    static const Int32 sIFF_MASTER;
    static const Int32 sIFF_MULTICAST;
    static const Int32 sIFF_NOARP;
    static const Int32 sIFF_NOTRAILERS;
    static const Int32 sIFF_POINTOPOINT;
    static const Int32 sIFF_PORTSEL;
    static const Int32 sIFF_PROMISC;
    static const Int32 sIFF_RUNNING;
    static const Int32 sIFF_SLAVE;
    static const Int32 sIFF_UP;
    static const Int32 sIPPROTO_ICMP;
    static const Int32 sIPPROTO_IP;
    static const Int32 sIPPROTO_IPV6;
    static const Int32 sIPPROTO_RAW;
    static const Int32 sIPPROTO_TCP;
    static const Int32 sIPPROTO_UDP;
    static const Int32 sIPV6_CHECKSUM;
    static const Int32 sIPV6_MULTICAST_HOPS;
    static const Int32 sIPV6_MULTICAST_IF;
    static const Int32 sIPV6_MULTICAST_LOOP;
    static const Int32 sIPV6_RECVDSTOPTS;
    static const Int32 sIPV6_RECVHOPLIMIT;
    static const Int32 sIPV6_RECVHOPOPTS;
    static const Int32 sIPV6_RECVPKTINFO;
    static const Int32 sIPV6_RECVRTHDR;
    static const Int32 sIPV6_RECVTCLASS;
    static const Int32 sIPV6_TCLASS;
    static const Int32 sIPV6_UNICAST_HOPS;
    static const Int32 sIPV6_V6ONLY;
    static const Int32 sIP_MULTICAST_IF;
    static const Int32 sIP_MULTICAST_LOOP;
    static const Int32 sIP_MULTICAST_TTL;
    static const Int32 sIP_TOS;
    static const Int32 sIP_TTL;
    static const Int32 sMAP_FIXED;
    static const Int32 sMAP_PRIVATE;
    static const Int32 sMAP_SHARED;
    static const Int32 sMCAST_JOIN_GROUP;
    static const Int32 sMCAST_LEAVE_GROUP;
    static const Int32 sMCL_CURRENT;
    static const Int32 sMCL_FUTURE;
    static const Int32 sMSG_CTRUNC;
    static const Int32 sMSG_DONTROUTE;
    static const Int32 sMSG_EOR;
    static const Int32 sMSG_OOB;
    static const Int32 sMSG_PEEK;
    static const Int32 sMSG_TRUNC;
    static const Int32 sMSG_WAITALL;
    static const Int32 sMS_ASYNC;
    static const Int32 sMS_INVALIDATE;
    static const Int32 sMS_SYNC;
    static const Int32 sNI_DGRAM;
    static const Int32 sNI_NAMEREQD;
    static const Int32 sNI_NOFQDN;
    static const Int32 sNI_NUMERICHOST;
    static const Int32 sNI_NUMERICSERV;
    static const Int32 sO_ACCMODE;
    static const Int32 sO_APPEND;
    static const Int32 sO_CREAT;
    static const Int32 sO_EXCL;
    static const Int32 sO_NOCTTY;
    static const Int32 sO_NOFOLLOW;
    static const Int32 sO_NONBLOCK;
    static const Int32 sO_RDONLY;
    static const Int32 sO_RDWR;
    static const Int32 sO_SYNC;
    static const Int32 sO_TRUNC;
    static const Int32 sO_WRONLY;
    static const Int32 sPOLLERR;
    static const Int32 sPOLLHUP;
    static const Int32 sPOLLIN;
    static const Int32 sPOLLNVAL;
    static const Int32 sPOLLOUT;
    static const Int32 sPOLLPRI;
    static const Int32 sPOLLRDBAND;
    static const Int32 sPOLLRDNORM;
    static const Int32 sPOLLWRBAND;
    static const Int32 sPOLLWRNORM;
    static const Int32 sPROT_EXEC;
    static const Int32 sPROT_NONE;
    static const Int32 sPROT_READ;
    static const Int32 sPROT_WRITE;
    static const Int32 sR_OK;
    static const Int32 sSEEK_CUR;
    static const Int32 sSEEK_END;
    static const Int32 sSEEK_SET;
    static const Int32 sSHUT_RD;
    static const Int32 sSHUT_RDWR;
    static const Int32 sSHUT_WR;
    static const Int32 sSIGABRT;
    static const Int32 sSIGALRM;
    static const Int32 sSIGBUS;
    static const Int32 sSIGCHLD;
    static const Int32 sSIGCONT;
    static const Int32 sSIGFPE;
    static const Int32 sSIGHUP;
    static const Int32 sSIGILL;
    static const Int32 sSIGINT;
    static const Int32 sSIGIO;
    static const Int32 sSIGKILL;
    static const Int32 sSIGPIPE;
    static const Int32 sSIGPROF;
    static const Int32 sSIGPWR;
    static const Int32 sSIGQUIT;
    static const Int32 sSIGRTMAX;
    static const Int32 sSIGRTMIN;
    static const Int32 sSIGSEGV;
    static const Int32 sSIGSTKFLT;
    static const Int32 sSIGSTOP;
    static const Int32 sSIGSYS;
    static const Int32 sSIGTERM;
    static const Int32 sSIGTRAP;
    static const Int32 sSIGTSTP;
    static const Int32 sSIGTTIN;
    static const Int32 sSIGTTOU;
    static const Int32 sSIGURG;
    static const Int32 sSIGUSR1;
    static const Int32 sSIGUSR2;
    static const Int32 sSIGVTALRM;
    static const Int32 sSIGWINCH;
    static const Int32 sSIGXCPU;
    static const Int32 sSIGXFSZ;
    static const Int32 sSIOCGIFADDR;
    static const Int32 sSIOCGIFBRDADDR;
    static const Int32 sSIOCGIFDSTADDR;
    static const Int32 sSIOCGIFNETMASK;
    static const Int32 sSOCK_DGRAM;
    static const Int32 sSOCK_RAW;
    static const Int32 sSOCK_SEQPACKET;
    static const Int32 sSOCK_STREAM;
    static const Int32 sSOL_SOCKET;
    static const Int32 sSO_BINDTODEVICE;
    static const Int32 sSO_BROADCAST;
    static const Int32 sSO_DEBUG;
    static const Int32 sSO_DONTROUTE;
    static const Int32 sSO_ERROR;
    static const Int32 sSO_KEEPALIVE;
    static const Int32 sSO_LINGER;
    static const Int32 sSO_OOBINLINE;
    static const Int32 sSO_RCVBUF;
    static const Int32 sSO_RCVLOWAT;
    static const Int32 sSO_RCVTIMEO;
    static const Int32 sSO_REUSEADDR;
    static const Int32 sSO_SNDBUF;
    static const Int32 sSO_SNDLOWAT;
    static const Int32 sSO_SNDTIMEO;
    static const Int32 sSO_TYPE;
    static const Int32 sSTDERR_FILENO;
    static const Int32 sSTDIN_FILENO;
    static const Int32 sSTDOUT_FILENO;
    static const Int32 sS_IFBLK;
    static const Int32 sS_IFCHR;
    static const Int32 sS_IFDIR;
    static const Int32 sS_IFIFO;
    static const Int32 sS_IFLNK;
    static const Int32 sS_IFMT;
    static const Int32 sS_IFREG;
    static const Int32 sS_IFSOCK;
    static const Int32 sS_IRGRP;
    static const Int32 sS_IROTH;
    static const Int32 sS_IRUSR;
    static const Int32 sS_IRWXG;
    static const Int32 sS_IRWXO;
    static const Int32 sS_IRWXU;
    static const Int32 sS_ISGID;
    static const Int32 sS_ISUID;
    static const Int32 sS_ISVTX;
    static const Int32 sS_IWGRP;
    static const Int32 sS_IWOTH;
    static const Int32 sS_IWUSR;
    static const Int32 sS_IXGRP;
    static const Int32 sS_IXOTH;
    static const Int32 sS_IXUSR;
    static const Int32 sTCP_NODELAY;
    static const Int32 sWCONTINUED;
    static const Int32 sWEXITED;
    static const Int32 sWNOHANG;
    static const Int32 sWNOWAIT;
    static const Int32 sWSTOPPED;
    static const Int32 sWUNTRACED;
    static const Int32 sW_OK;
    static const Int32 sX_OK;
    static const Int32 s_SC_2_CHAR_TERM;
    static const Int32 s_SC_2_C_BIND;
    static const Int32 s_SC_2_C_DEV;
    static const Int32 s_SC_2_C_VERSION;
    static const Int32 s_SC_2_FORT_DEV;
    static const Int32 s_SC_2_FORT_RUN;
    static const Int32 s_SC_2_LOCALEDEF;
    static const Int32 s_SC_2_SW_DEV;
    static const Int32 s_SC_2_UPE;
    static const Int32 s_SC_2_VERSION;
    static const Int32 s_SC_AIO_LISTIO_MAX;
    static const Int32 s_SC_AIO_MAX;
    static const Int32 s_SC_AIO_PRIO_DELTA_MAX;
    static const Int32 s_SC_ARG_MAX;
    static const Int32 s_SC_ASYNCHRONOUS_IO;
    static const Int32 s_SC_ATEXIT_MAX;
    static const Int32 s_SC_AVPHYS_PAGES;
    static const Int32 s_SC_BC_BASE_MAX;
    static const Int32 s_SC_BC_DIM_MAX;
    static const Int32 s_SC_BC_SCALE_MAX;
    static const Int32 s_SC_BC_STRING_MAX;
    static const Int32 s_SC_CHILD_MAX;
    static const Int32 s_SC_CLK_TCK;
    static const Int32 s_SC_COLL_WEIGHTS_MAX;
    static const Int32 s_SC_DELAYTIMER_MAX;
    static const Int32 s_SC_EXPR_NEST_MAX;
    static const Int32 s_SC_FSYNC;
    static const Int32 s_SC_GETGR_R_SIZE_MAX;
    static const Int32 s_SC_GETPW_R_SIZE_MAX;
    static const Int32 s_SC_IOV_MAX;
    static const Int32 s_SC_JOB_CONTROL;
    static const Int32 s_SC_LINE_MAX;
    static const Int32 s_SC_LOGIN_NAME_MAX;
    static const Int32 s_SC_MAPPED_FILES;
    static const Int32 s_SC_MEMLOCK;
    static const Int32 s_SC_MEMLOCK_RANGE;
    static const Int32 s_SC_MEMORY_PROTECTION;
    static const Int32 s_SC_MESSAGE_PASSING;
    static const Int32 s_SC_MQ_OPEN_MAX;
    static const Int32 s_SC_MQ_PRIO_MAX;
    static const Int32 s_SC_NGROUPS_MAX;
    static const Int32 s_SC_NPROCESSORS_CONF;
    static const Int32 s_SC_NPROCESSORS_ONLN;
    static const Int32 s_SC_OPEN_MAX;
    static const Int32 s_SC_PAGESIZE;
    static const Int32 s_SC_PAGE_SIZE;
    static const Int32 s_SC_PASS_MAX;
    static const Int32 s_SC_PHYS_PAGES;
    static const Int32 s_SC_PRIORITIZED_IO;
    static const Int32 s_SC_PRIORITY_SCHEDULING;
    static const Int32 s_SC_REALTIME_SIGNALS;
    static const Int32 s_SC_RE_DUP_MAX;
    static const Int32 s_SC_RTSIG_MAX;
    static const Int32 s_SC_SAVED_IDS;
    static const Int32 s_SC_SEMAPHORES;
    static const Int32 s_SC_SEM_NSEMS_MAX;
    static const Int32 s_SC_SEM_VALUE_MAX;
    static const Int32 s_SC_SHARED_MEMORY_OBJECTS;
    static const Int32 s_SC_SIGQUEUE_MAX;
    static const Int32 s_SC_STREAM_MAX;
    static const Int32 s_SC_SYNCHRONIZED_IO;
    static const Int32 s_SC_THREADS;
    static const Int32 s_SC_THREAD_ATTR_STACKADDR;
    static const Int32 s_SC_THREAD_ATTR_STACKSIZE;
    static const Int32 s_SC_THREAD_DESTRUCTOR_ITERATIONS;
    static const Int32 s_SC_THREAD_KEYS_MAX;
    static const Int32 s_SC_THREAD_PRIORITY_SCHEDULING;
    static const Int32 s_SC_THREAD_PRIO_INHERIT;
    static const Int32 s_SC_THREAD_PRIO_PROTECT;
    static const Int32 s_SC_THREAD_SAFE_FUNCTIONS;
    static const Int32 s_SC_THREAD_STACK_MIN;
    static const Int32 s_SC_THREAD_THREADS_MAX;
    static const Int32 s_SC_TIMERS;
    static const Int32 s_SC_TIMER_MAX;
    static const Int32 s_SC_TTY_NAME_MAX;
    static const Int32 s_SC_TZNAME_MAX;
    static const Int32 s_SC_VERSION;
    static const Int32 s_SC_XBS5_ILP32_OFF32;
    static const Int32 s_SC_XBS5_ILP32_OFFBIG;
    static const Int32 s_SC_XBS5_LP64_OFF64;
    static const Int32 s_SC_XBS5_LPBIG_OFFBIG;
    static const Int32 s_SC_XOPEN_CRYPT;
    static const Int32 s_SC_XOPEN_ENH_I18N;
    static const Int32 s_SC_XOPEN_LEGACY;
    static const Int32 s_SC_XOPEN_REALTIME;
    static const Int32 s_SC_XOPEN_REALTIME_THREADS;
    static const Int32 s_SC_XOPEN_SHM;
    static const Int32 s_SC_XOPEN_UNIX;
    static const Int32 s_SC_XOPEN_VERSION;
    static const Int32 s_SC_XOPEN_XCU_VERSION;

private:
    static Boolean sInitlized;
    static HashMap<String, Int32> sConstants;
};

} // namespace IO
} // namespace Libcore

#endif // __COSCONSTANTS_H__
