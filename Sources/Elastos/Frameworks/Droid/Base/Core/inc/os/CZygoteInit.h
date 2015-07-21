
#ifndef __ELASTOS_DROID_INTZERNAL_CZYGOTEINIT_H__
#define __ELASTOS_DROID_INTZERNAL_CZYGOTEINIT_H__

#include "_Elastos_Droid_Internal_Os_CZygoteInit.h"
#include "os/Runnable.h"
#include "os/ZygoteConnection.h"

using Elastos::Core::IRunnable;
using Elastos::Droid::Net::ILocalServerSocket;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

CarClass(CZygoteInit)
    , public Object
    , public IZygoteInit
{
public:
    /**
     * Helper exception class which holds a method and arguments and
     * can call them. This is used as part of a trampoline to get rid of
     * the initial process setup stack frames.
     */
    class MethodAndArgsCaller
        : public Runnable
    {
    public:
        MethodAndArgsCaller(
            /* [in] */ IInterface* object,
            /* [in] */ IMethodInfo* method,
            /* [in] */ ArrayOf<String>* args);

        CARAPI Run();

    private:
        AutoPtr<IInterface> mObject;
        AutoPtr<IMethodInfo> mMethod;
        AutoPtr< ArrayOf<String> > mArgs;
    };

public:
    /**
     * Close and clean up zygote sockets. Called on shutdown and on the
     * child's exit path.
     */
    static CARAPI_(void) CloseServerSocket();

    /**
    * Creates a file descriptor from an int fd.
    */
    static CARAPI_(AutoPtr<IFileDescriptor>) CreateFileDescriptor(
        /* [in] */ Int32 fd);

    CARAPI Main(
        /* [in] */ const ArrayOf<String>& argv);

    /**
     * The Linux syscall "setreuid()"
     * @param ruid real uid
     * @param euid effective uid
     * @return 0 on success, non-zero errno on fail
     */
    static CARAPI_(Int32) Setreuid(
        /* [in] */ Int32 ruid,
        /* [in] */ Int32 euid);

    /**
     * The Linux syscall "setregid()"
     * @param rgid real gid
     * @param egid effective gid
     * @return 0 on success, non-zero errno on fail
     */
    static CARAPI_(Int32) Setregid(
        /* [in] */ Int32 rgid,
        /* [in] */ Int32 egid);

    /**
     * Invokes the linux syscall "setpgid"
     *
     * @param pid pid to change
     * @param pgid new process group of pid
     * @return 0 on success or non-zero errno on fail
     */
    static CARAPI_(Int32) Setpgid(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 pgid);

    /**
     * Invokes the linux syscall "getpgid"
     *
     * @param pid pid to query
     * @return pgid of pid in question
     * @throws IOException on error
     */
    static CARAPI Getpgid(
        /* [in] */ Int32 pid,
        /* [out] */ Int32* pgid);

    /**
     * Invokes the syscall dup2() to copy the specified descriptors into
     * stdin, stdout, and stderr. The existing stdio descriptors will be
     * closed and errors during close will be ignored. The specified
     * descriptors will also remain open at their original descriptor numbers,
     * so the caller may want to close the original descriptors.
     *
     * @param in new stdin
     * @param out new stdout
     * @param err new stderr
     * @throws IOException
     */
    static CARAPI_(void) ReopenStdio(
        /* [in] */ IFileDescriptor* in,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ IFileDescriptor* err);

    /**
     * Toggles the close-on-exec flag for the specified file descriptor.
     *
     * @param fd non-null; file descriptor
     * @param flag desired close-on-exec flag state
     * @throws IOException
     */
    static CARAPI SetCloseOnExec(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean flag);

    /**
     * Invokes select() on the provider array of file descriptors (selecting
     * for readability only). Array elements of null are ignored.
     *
     * @param fds non-null; array of readable file descriptors
     * @return index of descriptor that is now readable or -1 for empty array.
     * @throws IOException if an error occurs
     */
    static CARAPI SelectReadable(
        /* [in] */ ArrayOf<IFileDescriptor*>* fds,
        /* [out] */ Int32* index);

private:
    /**
     * Registers a server socket for zygote command connections
     *
     * @throws RuntimeException when open fails
     */
    static CARAPI RegisterZygoteSocket();

    /**
     * Waits for and accepts a single command connection. Throws
     * RuntimeException on failure.
     */
    static CARAPI AcceptCommandPeer(
        /* [out] */ ZygoteConnection** peer);

    /**
     * Finish remaining work for the newly forked system server process.
     */
    static CARAPI HandleSystemServerProcess(
        /* [in] */ ZygoteConnection::Arguments* parsedArgs,
        /* [out] */ IRunnable** task);

    /**
     * Prepare the arguments and fork for the system server process.
     */
    static CARAPI StartSystemServer(
        /* [out] */ IRunnable** task);

    /**
     * Runs the zygote in accept-and-fork mode. In this mode, each peer
     * gets its own zygote spawner process. This code is retained for
     * reference only.
     *
     * @throws MethodAndArgsCaller in a child process when a main() should
     * be executed.
     */
    static CARAPI RunForkMode();

    /**
     * Runs the zygote process's select loop. Accepts new connections as
     * they happen, and reads commands from connections one spawn-request's
     * worth at a time.
     *
     * @throws MethodAndArgsCaller in a child process when a main() should
     * be executed.
     */
    static CARAPI RunSelectLoopMode(
        /* [in] */ IRunnable** task);

private:
    static const String TAG;

    static AutoPtr<ILocalServerSocket> sServerSocket;

    /**
     * If true, zygote forks for each peer. If false, a select loop is used
     * inside a single process. The latter is preferred.
     */
    static const Boolean ZYGOTE_FORK_MODE = FALSE;
};

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CZYGOTEINIT_H__
