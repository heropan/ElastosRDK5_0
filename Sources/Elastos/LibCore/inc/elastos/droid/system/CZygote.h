
#ifndef __CZYGOTE_H__
#define __CZYGOTE_H__

#include "_Elastos_Droid_System_CZygote.h"
#include "core/Singleton.h"

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace System {

CarClass(CZygote)
    , public Singleton
    , public IZygote
{
public:
    /**
     * Forks a new VM instance.  The current VM must have been started
     * with the -Xzygote flag. <b>NOTE: new instance keeps all
     * root capabilities. The new process is expected to call capset()</b>.
     *
     * @param uid the UNIX uid that the new process should setuid() to after
     * fork()ing and and before spawning any threads.
     * @param gid the UNIX gid that the new process should setgid() to after
     * fork()ing and and before spawning any threads.
     * @param gids null-ok; a list of UNIX gids that the new process should
     * setgroups() to after fork and before spawning any threads.
     * @param debugFlags bit flags that enable debugging features.
     * @param rlimits null-ok an array of rlimit tuples, with the second
     * dimension having a length of 3 and representing
     * (resource, rlim_cur, rlim_max). These are set via the posix
     * setrlimit(2) call.
     * @param seInfo null-ok a string specifying SEAndroid information for
     * the new process.
     * @param niceName null-ok a string specifying the process name.
     *
     * @return 0 if this is the child, pid of the child
     * if this is the parent, or -1 on error.
     */
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI ForkAndSpecialize(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
        /* [in] */ Int32 mountExternal,
        /* [in] */ const String& seInfo,
        /* [in] */ const String& niceName,
        /* [out] */ Int32* pid);

    static CARAPI_(Int32) NativeForkAndSpecialize(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
        /* [in] */ Int32 mountExternal,
        /* [in] */ const String& seInfo,
        /* [in] */ const String& niceName);

    /**
     * Special method to start the system server process. In addition to the
     * common actions performed in forkAndSpecialize, the pid of the child
     * process is recorded such that the death of the child process will cause
     * zygote to exit.
     *
     * @param uid the UNIX uid that the new process should setuid() to after
     * fork()ing and and before spawning any threads.
     * @param gid the UNIX gid that the new process should setgid() to after
     * fork()ing and and before spawning any threads.
     * @param gids null-ok; a list of UNIX gids that the new process should
     * setgroups() to after fork and before spawning any threads.
     * @param debugFlags bit flags that enable debugging features.
     * @param rlimits null-ok an array of rlimit tuples, with the second
     * dimension having a length of 3 and representing
     * (resource, rlim_cur, rlim_max). These are set via the posix
     * setrlimit(2) call.
     * @param permittedCapabilities argument for setcap()
     * @param effectiveCapabilities argument for setcap()
     *
     * @return 0 if this is the child, pid of the child
     * if this is the parent, or -1 on error.
     */
    CARAPI ForkSystemServer(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
        /* [in] */ Int64 permittedCapabilities,
        /* [in] */ Int64 effectiveCapabilities,
        /* [out] */ Int32* pid);

    static CARAPI_(Int32) NativeForkSystemServer(
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid,
        /* [in] */ ArrayOf<Int32>* gids,
        /* [in] */ Int32 debugFlags,
        /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
        /* [in] */ Int64 permittedCapabilities,
        /* [in] */ Int64 effectiveCapabilities);

    CARAPI GetSystemInSafeMode(
        /* [out] */ Boolean* mode);

private:
    static CARAPI_(void) PreFork();

    /**
     * We must not fork until we're single-threaded again. Wait until /proc shows we're
     * down to just one thread.
     */
    static CARAPI_(void) WaitUntilAllThreadsStopped();

    static CARAPI_(void) PostFork();

public:
    static Boolean sSystemInSafeMode;// = false;

private:
    static const Boolean DEBUG;
};

} // namespace System
} // namespace Droid
} // namespace Elastos

#endif //__CZYGOTE_H__
