
#include "ext/frameworkdef.h"
#include "os/CZygoteInit.h"
#include "os/Process.h"
#include "os/RuntimeInit.h"
#include "net/CLocalServerSocket.h"
#include <Elastos.Droid.System.h>
#include <Elastos.Core.h>
#include <elastos/Logger.h>
#include <elastos/Vector.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Vector;
using Elastos::Droid::Net::CLocalServerSocket;
using Elastos::Droid::Os::Process;
using Elastos::Droid::System::CZygote;
using Elastos::Droid::System::IZygote;
using Elastos::IO::CFileDescriptor;
using Elastos::IO::IFileDescriptor;
using Elastos::Utility::Logging::Logger;
using Libcore::IO::CLibcore;
using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::IOs;
using Libcore::IO::IOsConstants;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

const String CZygoteInit::TAG("zygote");
AutoPtr<ILocalServerSocket> CZygoteInit::sServerSocket;
const Boolean CZygoteInit::ZYGOTE_FORK_MODE;

CZygoteInit::MethodAndArgsCaller::MethodAndArgsCaller(
    /* [in] */ IInterface* object,
    /* [in] */ IMethodInfo* method,
    /* [in] */ ArrayOf<String>* args)
    : mObject(object)
    , mMethod(method)
    , mArgs(args)
{}

PInterface CZygoteInit::MethodAndArgsCaller::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IRunnable*)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 CZygoteInit::MethodAndArgsCaller::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CZygoteInit::MethodAndArgsCaller::Release()
{
    return ElRefBase::Release();
}

ECode CZygoteInit::MethodAndArgsCaller::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CZygoteInit::MethodAndArgsCaller::Run()
{
    AutoPtr<IArgumentList> argumentList;
    ECode ec = mMethod->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Create \"Main\" method argument list failed!\n");
        return ec;
    }

    argumentList->SetInputArgumentOfCarArray(0, mArgs.Get());

    ec = mMethod->Invoke(mObject, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"Main\" method failed!\n");
        return ec;
    }
    return ec;
}


ECode CZygoteInit::RegisterZygoteSocket()
{
    if (sServerSocket == NULL) {
        AutoPtr<Elastos::Core::ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((Elastos::Core::ISystem**)&system);

        Int32 fd;
        // try {
        String env;
        system->GetEnv(String("ANDROID_SOCKET_elzygote"), &env);
        if (env.IsNull()) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        fd = StringUtils::ParseInt32(env);
        // } catch (RuntimeException ex) {
        //     throw new RuntimeException(
        //             ANDROID_SOCKET_ENV + " unset or invalid", ex);
        // }

        // try {
        //     sServerSocket = new LocalServerSocket(
        //             createFileDescriptor(fileDesc));
        ECode ec = CLocalServerSocket::New(CreateFileDescriptor(fd), (ILocalServerSocket**)&sServerSocket);
        if (FAILED(ec)) {
            printf("Error binding to local socket!\n");
        }
        // } catch (IOException ex) {
        //     throw new RuntimeException(
        //             "Error binding to local socket '" + fileDesc + "'", ex);
        // }
    }
    return NOERROR;
}

ECode CZygoteInit::AcceptCommandPeer(
    /* [out] */ ZygoteConnection** peer)
{
    // try {
    AutoPtr<ILocalSocket> socket;
    FAIL_RETURN(sServerSocket->Accept((ILocalSocket**)&socket));
    *peer = new ZygoteConnection(socket);
    INTERFACE_ADDREF(*peer);
    return NOERROR;
    // } catch (IOException ex) {
    //     throw new RuntimeException(
    //             "IOException during accept()", ex);
    // }
}

AutoPtr<IFileDescriptor> CZygoteInit::CreateFileDescriptor(
    /* [in] */ Int32 fd)
{
    AutoPtr<IFileDescriptor> fileDesc;
    CFileDescriptor::New((IFileDescriptor**)&fileDesc);
    fileDesc->SetDescriptor(fd);
    return fileDesc;
}

void CZygoteInit::CloseServerSocket()
{
//    try {
    if (sServerSocket != NULL) {
        sServerSocket->Close();
    }
//    } catch (IOException ex) {
//        Log.e(TAG, "Zygote:  error closing sockets", ex);
//    }
//
    sServerSocket = NULL;
}

ECode CZygoteInit::HandleSystemServerProcess(
    /* [in] */ ZygoteConnection::Arguments* parsedArgs,
    /* [out] */ IRunnable** task)
{
    CloseServerSocket();

    // set umask to 0077 so new files and directories will default to owner-only permissions.
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IOsConstants> osConsts;
    COsConstants::AcquireSingleton((IOsConstants**)&osConsts);
    Int32 v1, v2;
    osConsts->GetOsConstant(String("S_IRWXG"), &v1);
    osConsts->GetOsConstant(String("S_IRWXO"), &v2);
    Int32 result;
    os->Umask(v1 | v2, &result);

    if (!parsedArgs->mNiceName.IsNull()) {
        Process::SetArgV0(parsedArgs->mNiceName);
    }

   if (!parsedArgs->mInvokeWith.IsNull()) {
//        WrapperInit.execApplication(parsedArgs.invokeWith,
//                parsedArgs.niceName, parsedArgs.targetSdkVersion,
//                null, parsedArgs.remainingArgs);
        return NOERROR;
   }
   else {
        /*
         * Pass the remaining arguments to SystemServer.
         */
        return RuntimeInit::ZygoteInit(parsedArgs->mTargetSdkVersion, parsedArgs->mRemainingArgs, task);
   }
}

/**
 * Prepare the arguments and fork for the system server process.
 */
ECode CZygoteInit::StartSystemServer(
    /* [out] */ IRunnable** task)
{
    VALIDATE_NOT_NULL(task);
    *task = NULL;

//    /* Hardcoded command line to start the system server */
    ArrayOf_<String, 8> args;
    args[0] = "--setuid=1000";
    args[1] = "--setgid=1000";
    args[2] = "--setgroups=1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1018,3001,3002,3003,3006,3007";
    args[3] = "--capabilities=130104352,130104352";
    args[4] = "--runtime-init";
    args[5] = "--nice-name=elsystemserver";
    args[6] = "Elastos.Droid.Server.eco";
    args[7] = "CSystemServer";
    AutoPtr<ZygoteConnection::Arguments> parsedArgs;

    Int32 pid;

//    try {
    parsedArgs = new ZygoteConnection::Arguments(args);
    ZygoteConnection::ApplyDebuggerSystemProperty(parsedArgs);
    ZygoteConnection::ApplyInvokeWithSystemProperty(parsedArgs);

    /* Request to fork the system server process */
    AutoPtr<IZygote> zygote;
    CZygote::AcquireSingleton((IZygote**)&zygote);
    zygote->ForkSystemServer(
            parsedArgs->mUid, parsedArgs->mGid,
            parsedArgs->mGids,
            parsedArgs->mDebugFlags,
            NULL,
            parsedArgs->mPermittedCapabilities,
            parsedArgs->mEffectiveCapabilities,
            &pid);
//    } catch (IllegalArgumentException ex) {
//        throw new RuntimeException(ex);
//    }
//
    /* For child process */
    if (pid == 0) {
        HandleSystemServerProcess(parsedArgs, task);
    }

    return NOERROR;
}

ECode CZygoteInit::Main(
    /* [in] */ const ArrayOf<String>& argv)
{
    // try {
    // Start profiling the zygote initialization.
//    SamplingProfilerIntegration.start();
//
    RegisterZygoteSocket();
//    EventLog.writeEvent(LOG_BOOT_PROGRESS_PRELOAD_START,
//        SystemClock.uptimeMillis());
//    preload();
//    EventLog.writeEvent(LOG_BOOT_PROGRESS_PRELOAD_END,
//        SystemClock.uptimeMillis());
//
//    // Finish profiling the zygote initialization.
//    SamplingProfilerIntegration.writeZygoteSnapshot();
//
//    // Do an initial gc to clean up after startup
//    gc();
//
//    // If requested, start system server directly from Zygote
//    if (argv.length != 2) {
//        throw new RuntimeException(argv[0] + USAGE_STRING);
//    }

    AutoPtr<IRunnable> task;

    if (argv[1].Equals("start-system-server")) {
        FAIL_RETURN(StartSystemServer((IRunnable**)&task));
        if (task != NULL) goto RUN_TASK;
    }
    else if (!argv[1].Equals("")) {
        // throw new RuntimeException(argv[0] + USAGE_STRING);
        return E_RUNTIME_EXCEPTION;
    }

    Logger::I(TAG, "Accepting command socket connections");

    if (ZYGOTE_FORK_MODE) {
        RunForkMode();
    }
    else {
        task = NULL;
        RunSelectLoopMode((IRunnable**)&task);
        if (task != NULL) goto RUN_TASK;
    }

    CloseServerSocket();
    return NOERROR;
    // } catch (MethodAndArgsCaller caller) {
    //     caller.run();
    // } catch (RuntimeException ex) {
    //     Log.e(TAG, "Zygote died with exception", ex);
    //     closeServerSocket();
    //     throw ex;
    // }

RUN_TASK:
    /* For child process */
    return task->Run();
}

ECode CZygoteInit::RunForkMode()
{
    assert(0);
//    while (true) {
//        ZygoteConnection peer = acceptCommandPeer();
//
//        int pid;
//
//        pid = Zygote.fork();
//
//        if (pid == 0) {
//            // The child process should handle the peer requests
//
//            // The child does not accept any more connections
//            try {
//                sServerSocket.close();
//            } catch (IOException ex) {
//                Log.e(TAG, "Zygote Child: error closing sockets", ex);
//            } finally {
//                sServerSocket = null;
//            }
//
//            peer.run();
//            break;
//        } else if (pid > 0) {
//            peer.closeSocket();
//        } else {
//            throw new RuntimeException("Error invoking fork()");
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CZygoteInit::RunSelectLoopMode(
    /* [in] */ IRunnable** task)
{
    Vector< AutoPtr<IFileDescriptor> > fds;
    Vector< AutoPtr<ZygoteConnection> > peers;
    AutoPtr< ArrayOf<IFileDescriptor*> > fdArray = ArrayOf<IFileDescriptor*>::Alloc(4);

    AutoPtr<IFileDescriptor> fd;
    sServerSocket->GetFileDescriptor((IFileDescriptor**)&fd);
    fds.PushBack(fd);
    peers.PushBack(NULL);

    while (TRUE) {
        Int32 index;

        // try {
        Int32 count = fds.GetSize();
        if (count > fdArray->GetLength()) {
            fdArray = ArrayOf<IFileDescriptor*>::Alloc(count);
        }
        for (Int32 i = 0; i < count; i++) {
            fdArray->Set(i, fds[i]);
        }
        ECode ec = SelectReadable(fdArray, &index);
        if (ec == (ECode)E_IO_EXCEPTION) {
            return E_RUNTIME_EXCEPTION;
        }
        // } catch (IOException ex) {
        //     throw new RuntimeException("Error in select()", ex);
        // }

        if (index < 0) {
            // throw new RuntimeException("Error in select()");
            return E_RUNTIME_EXCEPTION;
        }
        else if (index == 0) {
            AutoPtr<ZygoteConnection> newPeer;
            FAIL_RETURN(AcceptCommandPeer((ZygoteConnection**)&newPeer));
            peers.PushBack(newPeer);
            AutoPtr<IFileDescriptor> fd;
            newPeer->GetFileDescriptor((IFileDescriptor**)&fd);
            fds.PushBack(fd);
        }
        else {
            Boolean done = peers[index]->RunOnce(task);
            if (*task != NULL) return NOERROR;

            if (done) {
                peers.Erase(peers.Begin() + index);
                fds.Erase(fds.Begin() + index);
            }
        }
    }
    return NOERROR;
}

Int32 CZygoteInit::Setreuid(
    /* [in] */ Int32 ruid,
    /* [in] */ Int32 euid)
{
    if (setreuid(ruid, euid) < 0) {
        return errno;
    }
    return 0;
}

Int32 CZygoteInit::Setregid(
    /* [in] */ Int32 rgid,
    /* [in] */ Int32 egid)
{
    if (setregid(rgid, egid) < 0) {
        return errno;
    }
    return 0;
}

Int32 CZygoteInit::Setpgid(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 pgid)
{
    if (setpgid(pid, pgid) < 0) {
        return errno;
    }
    return 0;
}

ECode CZygoteInit::Getpgid(
    /* [in] */ Int32 pid,
    /* [out] */ Int32* pgid)
{
    VALIDATE_NOT_NULL(pgid);
    pid_t ret;
    ret = getpgid(pid);

    if (ret < 0) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }

    *pgid = (Int32)ret;
    return NOERROR;
}

void CZygoteInit::ReopenStdio(
    /* [in] */ IFileDescriptor* infd,
    /* [in] */ IFileDescriptor* outfd,
    /* [in] */ IFileDescriptor* errfd)
{
    Int32 fd;
    int err;

    infd->GetDescriptor(&fd);

    do {
        err = dup2(fd, STDIN_FILENO);
    } while (err < 0 && errno == EINTR);

    outfd->GetDescriptor(&fd);

    do {
        err = dup2(fd, STDOUT_FILENO);
    } while (err < 0 && errno == EINTR);

    errfd->GetDescriptor(&fd);

    do {
        err = dup2(fd, STDERR_FILENO);
    } while (err < 0 && errno == EINTR);
}

ECode CZygoteInit::SetCloseOnExec(
    /* [in] */ IFileDescriptor* descriptor,
    /* [in] */ Boolean flag)
{
    Int32 fd;
    int err;
    int fdFlags;

    descriptor->GetDescriptor(&fd);

    fdFlags = fcntl(fd, F_GETFD);

    if (fdFlags < 0) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }

    if (flag) {
        fdFlags |= FD_CLOEXEC;
    }
    else {
        fdFlags &= ~FD_CLOEXEC;
    }

    err = fcntl(fd, F_SETFD, fdFlags);

    if (err < 0) {
        // jniThrowIOException(env, errno);
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CZygoteInit::SelectReadable(
    /* [in] */ ArrayOf<IFileDescriptor*>* fds,
    /* [out] */ Int32* index)
{
    if (fds == NULL) {
        // jniThrowNullPointerException(env, "fds == null");
        *index = -1;
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 length = fds->GetLength();
    fd_set fdset;

    FD_ZERO(&fdset);

    int nfds = 0;
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IFileDescriptor> fdObj = (*fds)[i];
        if (fdObj == NULL) {
            continue;
        }
        Int32 fd;
        fdObj->GetDescriptor(&fd);

        FD_SET(fd, &fdset);

        if (fd >= nfds) {
            nfds = fd + 1;
        }
    }

    int err;
    do {
        err = select(nfds, &fdset, NULL, NULL, NULL);
    } while (err < 0 && errno == EINTR);

    if (err < 0) {
        // jniThrowIOException(env, errno);
        *index = -1;
        return E_IO_EXCEPTION;
    }

    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IFileDescriptor> fdObj = (*fds)[i];
        if (fdObj == NULL) {
            continue;
        }
        Int32 fd;
        fdObj->GetDescriptor(&fd);
        if (FD_ISSET(fd, &fdset)) {
            *index = i;
            return NOERROR;
        }
    }
    *index = -1;
    return NOERROR;
}

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos
