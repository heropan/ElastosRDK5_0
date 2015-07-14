
#include "CZygote.h"
#include "core/Thread.h"
#include "logging/Logger.h"
#include "CFile.h"
#include <Elastos.CoreLibrary.h>
#ifdef HAVE_SELINUX
#include <selinux/android.h>
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/personality.h>
#include <unistd.h>
#include <cutils/sched_policy.h>
#include <stdlib.h>

#if defined(HAVE_PRCTL)
# include <sys/prctl.h>
#endif

using Elastos::Core::Thread;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace System {

static pid_t sSystemServerPid;

static const String TAG("CZygote");
Boolean CZygote::sSystemInSafeMode = FALSE;
static const Boolean DEBUG = FALSE;

CAR_SINGLETON_IMPL(CZygote)

CAR_INTERFACE_IMPL(CZygote, Singleton, IZygote)

void CZygote::PreFork()
{
    // Daemons.stop();
    WaitUntilAllThreadsStopped();
}

void CZygote::WaitUntilAllThreadsStopped()
{
    AutoPtr<IFile> tasks;
    CFile::New(String("/proc/self/task"), (IFile**)&tasks);
    AutoPtr< ArrayOf<String> > files;
    tasks->List((ArrayOf<String>**)&files);
    while (files != NULL && files->GetLength() > 1) {
        // try {
        // Experimentally, booting and playing about with a stingray, I never saw us
        // go round this loop more than once with a 10ms sleep.
        Thread::Sleep(10);
        // } catch (InterruptedException ignored) {
        // }
        files = NULL;
        tasks->List((ArrayOf<String>**)&files);
    }
}

void CZygote::PostFork()
{
    // Daemons.start();
}

ECode CZygote::ForkAndSpecialize(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int32 mountExternal,
    /* [in] */ const String& seInfo,
    /* [in] */ const String& niceName,
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    PreFork();
    *pid = NativeForkAndSpecialize(
            uid, gid, gids, debugFlags, rlimits, mountExternal, seInfo, niceName);
    PostFork();
    return NOERROR;
}

ECode CZygote::ForkSystemServer(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int64 permittedCapabilities,
    /* [in] */ Int64 effectiveCapabilities,
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    PreFork();
    *pid = NativeForkSystemServer(
            uid, gid, gids, debugFlags, rlimits, permittedCapabilities, effectiveCapabilities);
    PostFork();
    return NOERROR;
}

enum {
    MOUNT_EXTERNAL_NONE = 0,
    MOUNT_EXTERNAL_SINGLEUSER = 1,
    MOUNT_EXTERNAL_MULTIUSER = 2,
    MOUNT_EXTERNAL_MULTIUSER_ALL = 3,
};

/*
 * This signal handler is for zygote mode, since the zygote
 * must reap its children
 */
static void sigchldHandler(int s)
{
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        /* Log process-death status that we care about.  In general it is not
           safe to call ALOG(...) from a signal handler because of possible
           reentrancy.  However, we know a priori that the current implementation
           of ALOG() is safe to call from a SIGCHLD handler in the zygote process.
           If the ALOG() implementation changes its locking strategy or its use
           of syscalls within the lazy-init critical section, its use here may
           become unsafe. */
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status)) {
                // Logger::D(TAG, "Process %d exited cleanly (%d)",
                //     (int)pid, WEXITSTATUS(status));
            }
            else {
                if (DEBUG) {
                    // Logger::V(TAG,
                    //     "Process %d exited cleanly (%d)",
                    //     (int)pid, WEXITSTATUS(status));
                }
            }
        }
        else if (WIFSIGNALED(status)) {
            if (WTERMSIG(status) != SIGKILL) {
                // Logger::D(TAG,
                //     "Process %d terminated by signal (%d)",
                //     (int)pid, WTERMSIG(status));
            }
            else {
                if (DEBUG) {
                    // Logger::V(TAG,
                    //     "Process %d terminated by signal (%d)",
                    //     (int)pid, WTERMSIG(status));
                }
            }
#ifdef WCOREDUMP
            if (WCOREDUMP(status)) {
                // Logger::V(TAG, "Process %d dumped core",
                //     (int)pid);
            }
#endif /* ifdef WCOREDUMP */
        }

        /*
         * If the just-crashed process is the system_server, bring down zygote
         * so that it is restarted by init and system server will be restarted
         * from there.
         */
        if (pid == sSystemServerPid) {
            // Logger::V(TAG,
            //     "Exit zygote because system server (%d) has terminated",
            //     (int)pid);
            kill(getpid(), SIGKILL);
        }
    }

    if (pid < 0) {
        // Logger::W(TAG,
        //     "Zygote SIGCHLD error in waitpid: %s", strerror(errno));
    }
}

/*
 * configure sigchld handler for the zygote process
 * This is configured very late, because earlier in the dalvik lifecycle
 * we can fork() and exec() for the verifier/optimizer, and we
 * want to waitpid() for those rather than have them be harvested immediately.
 *
 * This ends up being called repeatedly before each fork(), but there's
 * no real harm in that.
 */
static void SetSignalHandler()
{
   int err;
   struct sigaction sa;

   memset(&sa, 0, sizeof(sa));

   sa.sa_handler = sigchldHandler;

   err = sigaction(SIGCHLD, &sa, NULL);

   if (err < 0) {
       // Logger::W(TAG, "Error setting SIGCHLD handler: %s", strerror(errno));
   }
}

/*
 * Set the SIGCHLD handler back to default behavior in zygote children
 */
static void UnsetSignalHandler()
{
   int err;
   struct sigaction sa;

   memset(&sa, 0, sizeof(sa));

   sa.sa_handler = SIG_DFL;

   err = sigaction(SIGCHLD, &sa, NULL);

   if (err < 0) {
       // Logger::W(TAG, "Error unsetting SIGCHLD handler: %s", strerror(errno));
   }
}

/*
 * Calls POSIX setgroups() using the int[] object as an argument.
 * A NULL argument is tolerated.
 */

static int SetgroupsIntarray(
    /* [in] */ ArrayOf<Int32>* gidArray)
{
    gid_t *gids;
    Int32 i;

    PFL_EX("TODO:");

    // if (gidArray == NULL) {
    //    return 0;
    // }

    // Int32 len = gidArray->GetLength();
    // /* just in case gid_t and u4 are different... */
    // gids = (gid_t *)alloca(sizeof(gid_t) * len);

    // for (i = 0; i < len; i++) {
    //    gids[i] = (gid_t)((*gidArray)[i]);
    // }

    // gids define in Amlogic4_2_1/system/core/include/private/android_filesystem_config.h
    Int32 len = 32;
    gids = (gid_t *)alloca(sizeof(gid_t) * len);
    for (i = 0; i < 24; i++)
        gids[i] = 1000 + i;
    for (i = 0; i < 8; i++)
        gids[24 + i] = 3001 + i;

    return setgroups((size_t)len, gids);
}

/*
 * Sets the resource limits via setrlimit(2) for the values in the
 * two-dimensional array of integers that's passed in. The second dimension
 * contains a tuple of length 3: (resource, rlim_cur, rlim_max). NULL is
 * treated as an empty array.
 *
 * -1 is returned on error.
 */
static int SetrlimitsFromArray(
    /* [in] */ ArrayOf<Int32*>* rlimits)
{
//    u4 i;
//    struct rlimit rlim;
//
//    if (rlimits == NULL) {
//        return 0;
//    }
//
//    memset (&rlim, 0, sizeof(rlim));
//
//    ArrayObject** tuples = (ArrayObject **)(void *)rlimits->contents;
//
//    for (i = 0; i < rlimits->length; i++) {
//        ArrayObject * rlimit_tuple = tuples[i];
//        s4* contents = (s4 *)(void *)rlimit_tuple->contents;
//        int err;
//
//        if (rlimit_tuple->length != 3) {
//            ALOGE("rlimits array must have a second dimension of size 3");
//            return -1;
//        }
//
//        rlim.rlim_cur = contents[1];
//        rlim.rlim_max = contents[2];
//
//        err = setrlimit(contents[0], &rlim);
//
//        if (err < 0) {
//            return -1;
//        }
//    }
//
    return 0;
}

/*
 * Create a private mount namespace and bind mount appropriate emulated
 * storage for the given user.
 */
static int MountEmulatedStorage(
    /* [in] */ uid_t uid,
    /* [in] */ Int32 mountMode)
{
//    // See storage config details at http://source.android.com/tech/storage/
//    userid_t userid = multiuser_get_user_id(uid);
//
//    // Create a second private mount namespace for our process
//    if (unshare(CLONE_NEWNS) == -1) {
//        SLOGE("Failed to unshare(): %s", strerror(errno));
//        return -1;
//    }
//
//    // Create bind mounts to expose external storage
//    if (mountMode == MOUNT_EXTERNAL_MULTIUSER
//            || mountMode == MOUNT_EXTERNAL_MULTIUSER_ALL) {
//        // These paths must already be created by init.rc
//        const char* source = getenv("EMULATED_STORAGE_SOURCE");
//        const char* target = getenv("EMULATED_STORAGE_TARGET");
//        const char* legacy = getenv("EXTERNAL_STORAGE");
//        if (source == NULL || target == NULL || legacy == NULL) {
//            SLOGE("Storage environment undefined; unable to provide external storage");
//            return -1;
//        }
//
//        // Prepare source paths
//        char source_user[PATH_MAX];
//        char source_obb[PATH_MAX];
//        char target_user[PATH_MAX];
//
//        // /mnt/shell/emulated/0
//        snprintf(source_user, PATH_MAX, "%s/%d", source, userid);
//        // /mnt/shell/emulated/obb
//        snprintf(source_obb, PATH_MAX, "%s/obb", source);
//        // /storage/emulated/0
//        snprintf(target_user, PATH_MAX, "%s/%d", target, userid);
//
//        if (fs_prepare_dir(source_user, 0000, 0, 0) == -1
//                || fs_prepare_dir(source_obb, 0000, 0, 0) == -1
//                || fs_prepare_dir(target_user, 0000, 0, 0) == -1) {
//            return -1;
//        }
//
//        if (mountMode == MOUNT_EXTERNAL_MULTIUSER_ALL) {
//            // Mount entire external storage tree for all users
//            if (mount(source, target, NULL, MS_BIND, NULL) == -1) {
//                SLOGE("Failed to mount %s to %s: %s", source, target, strerror(errno));
//                return -1;
//            }
//        } else {
//            // Only mount user-specific external storage
//            if (mount(source_user, target_user, NULL, MS_BIND, NULL) == -1) {
//                SLOGE("Failed to mount %s to %s: %s", source_user, target_user, strerror(errno));
//                return -1;
//            }
//        }
//
//        // Now that user is mounted, prepare and mount OBB storage
//        // into place for current user
//        char target_android[PATH_MAX];
//        char target_obb[PATH_MAX];
//
//        // /storage/emulated/0/Android
//        snprintf(target_android, PATH_MAX, "%s/%d/Android", target, userid);
//        // /storage/emulated/0/Android/obb
//        snprintf(target_obb, PATH_MAX, "%s/%d/Android/obb", target, userid);
//
//        if (fs_prepare_dir(target_android, 0000, 0, 0) == -1
//                || fs_prepare_dir(target_obb, 0000, 0, 0) == -1
//                || fs_prepare_dir(legacy, 0000, 0, 0) == -1) {
//            return -1;
//        }
//        if (mount(source_obb, target_obb, NULL, MS_BIND, NULL) == -1) {
//            SLOGE("Failed to mount %s to %s: %s", source_obb, target_obb, strerror(errno));
//            return -1;
//        }
//
//        mountSecondaryStorage(mountMode == MOUNT_EXTERNAL_MULTIUSER_ALL ?
//                                    target : target_user,
//                              mountMode);
//
//        // Finally, mount user-specific path into place for legacy users
//        if (mount(target_user, legacy, NULL, MS_BIND | MS_REC, NULL) == -1) {
//            SLOGE("Failed to mount %s to %s: %s", target_user, legacy, strerror(errno));
//            return -1;
//        }
//
//    } else {
//        SLOGE("Mount mode %d unsupported", mountMode);
//        return -1;
//    }
//
    return 0;
}

/*
 * Enable/disable debug features requested by the caller.
 *
 * debugger
 *   If set, enable debugging; if not set, disable debugging.  This is
 *   easy to handle, because the JDWP thread isn't started until we call
 *   dvmInitAfterZygote().
 * checkjni
 *   If set, make sure "check JNI" is enabled.
 * assert
 *   If set, make sure assertions are enabled.  This gets fairly weird,
 *   because it affects the result of a method called by class initializers,
 *   and hence can't affect pre-loaded/initialized classes.
 * safemode
 *   If set, operates the VM in the safe mode. The definition of "safe mode" is
 *   implementation dependent and currently only the JIT compiler is disabled.
 *   This is easy to handle because the compiler thread and associated resources
 *   are not requested until we call dvmInitAfterZygote().
 */
static void EnableDebugFeatures(Int32 debugFlags)
{
//    ALOGV("debugFlags is 0x%02x", debugFlags);
//
//    gDvm.jdwpAllowed = ((debugFlags & DEBUG_ENABLE_DEBUGGER) != 0);
//
//    if ((debugFlags & DEBUG_ENABLE_CHECKJNI) != 0) {
//        /* turn it on if it's not already enabled */
//        dvmLateEnableCheckedJni();
//    }
//
//    if ((debugFlags & DEBUG_ENABLE_JNI_LOGGING) != 0) {
//        gDvmJni.logThirdPartyJni = true;
//    }
//
//    if ((debugFlags & DEBUG_ENABLE_ASSERT) != 0) {
//        /* turn it on if it's not already enabled */
//        dvmLateEnableAssertions();
//    }
//
//    if ((debugFlags & DEBUG_ENABLE_SAFEMODE) != 0) {
//#if defined(WITH_JIT)
//        /* turn off the jit if it is explicitly requested by the app */
//        if (gDvm.executionMode == kExecutionModeJit)
//            gDvm.executionMode = kExecutionModeInterpFast;
//#endif
//    }
//
//#ifdef HAVE_ANDROID_OS
//    if ((debugFlags & DEBUG_ENABLE_DEBUGGER) != 0) {
//        /* To let a non-privileged gdbserver attach to this
//         * process, we must set its dumpable bit flag. However
//         * we are not interested in generating a coredump in
//         * case of a crash, so also set the coredump size to 0
//         * to disable that
//         */
//        if (prctl(PR_SET_DUMPABLE, 1, 0, 0, 0) < 0) {
//            ALOGE("could not set dumpable bit flag for pid %d: %s",
//                 getpid(), strerror(errno));
//        } else {
//            struct rlimit rl;
//            rl.rlim_cur = 0;
//            rl.rlim_max = RLIM_INFINITY;
//            if (setrlimit(RLIMIT_CORE, &rl) < 0) {
//                ALOGE("could not disable core file generation for pid %d: %s",
//                    getpid(), strerror(errno));
//            }
//        }
//    }
//#endif
}

/*
 * Set Linux capability flags.
 *
 * Returns 0 on success, errno on failure.
 */
static int SetCapabilities(
    /* [in] */ Int64 permitted,
    /* [in] */ Int64 effective)
{
#ifdef HAVE_ANDROID_OS
    struct __user_cap_header_struct capheader;
    struct __user_cap_data_struct capdata;

    memset(&capheader, 0, sizeof(capheader));
    memset(&capdata, 0, sizeof(capdata));

    capheader.version = _LINUX_CAPABILITY_VERSION;
    capheader.pid = 0;

    capdata.effective = effective;
    capdata.permitted = permitted;

    // Logger::V(TAG, "CAPSET perm=%llx eff=%llx", permitted, effective);
    if (capset(&capheader, &capdata) != 0)
        return errno;
#endif /*HAVE_ANDROID_OS*/

    return 0;
}

#ifdef HAVE_SELINUX
/*
 * Set SELinux security context.
 *
 * Returns 0 on success, -1 on failure.
 */
static int SetSELinuxContext(
    /* [in] */ uid_t uid,
    /* [in] */ bool isSystemServer,
    /* [in] */ const char *seInfo,
    /* [in] */ const char *niceName)
{
#ifdef HAVE_ANDROID_OS
    return selinux_android_setcontext(uid, isSystemServer, seInfo, niceName);
#else
    return 0;
#endif
}
#endif

static pid_t ForkAndSpecializeCommon(
    /* [in] */ uid_t uid,
    /* [in] */ gid_t gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int32 mountMode,
    /* [in] */ const String& seInfoObj,
    /* [in] */ const String& niceNameObj,
    /* [in] */ Int64 permittedCapabilities,
    /* [in] */ Int64 effectiveCapabilities)
{
    pid_t pid;

#ifdef HAVE_SELINUX
    char *seInfo = NULL;
    char *niceName = NULL;

    if (!seInfoObj.IsNull()) {
        seInfo = (char*)malloc(seInfoObj.GetByteLength() + 1);
        strcpy(seInfo, seInfoObj.string());
    }
    if (!niceNameObj.IsNull()) {
        niceName = (char*)malloc(niceNameObj.GetByteLength() + 1);
        strcpy(niceName, niceNameObj.string());
    }

#endif

//    if (!gDvm.zygote) {
//        dvmThrowIllegalStateException(
//            "VM instance not started with -Xzygote");
//
//        return -1;
//    }
//
//    if (!dvmGcPreZygoteFork()) {
//        ALOGE("pre-fork heap failed");
//        dvmAbort();
//    }

    SetSignalHandler();

    // dvmDumpLoaderStats("zygote");
    pid = fork();

    if (pid == 0) {
        int err;
        /* The child process */

#ifdef HAVE_ANDROID_OS
        // extern int gMallocLeakZygoteChild;
        // gMallocLeakZygoteChild = 1;

        /* keep caps across UID change, unless we're staying root */
        if (uid != 0) {
            err = prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);

            if (err < 0) {
                // Logger::E(TAG, "cannot PR_SET_KEEPCAPS: %s", strerror(errno));
                // dvmAbort();
            }
        }

#endif /* HAVE_ANDROID_OS */

        if (mountMode != MOUNT_EXTERNAL_NONE) {
            err = MountEmulatedStorage(uid, mountMode);
            if (err < 0) {
                // Logger::E(TAG, "cannot mountExternalStorage(): %s", strerror(errno));

                if (errno == ENOTCONN || errno == EROFS) {
                    // When device is actively encrypting, we get ENOTCONN here
                    // since FUSE was mounted before the framework restarted.
                    // When encrypted device is booting, we get EROFS since
                    // FUSE hasn't been created yet by init.
                    // In either case, continue without external storage.
                }
                else {
                    // dvmAbort();
                }
            }
        }

        err = SetgroupsIntarray(gids);
        if (err < 0) {
            // Logger::E(TAG, "cannot setgroups(): %s", strerror(errno));
            // dvmAbort();
        }

        err = SetrlimitsFromArray(rlimits);
        if (err < 0) {
            // Logger::E(TAG, "cannot setrlimit(): %s", strerror(errno));
            // dvmAbort();
        }

        err = setgid(gid);
        if (err < 0) {
            // Logger::E(TAG, "cannot setgid(%d): %s", gid, strerror(errno));
            // dvmAbort();
        }

        err = setuid(uid);
        if (err < 0) {
            // Logger::E(TAG, "cannot setuid(%d): %s", uid, strerror(errno));
            // dvmAbort();
        }

        int current = personality(0xffffFFFF);
        int success = personality((ADDR_NO_RANDOMIZE | current));
        if (success == -1) {
            // Logger::W(TAG, "Personality switch failed. current=%d error=%d\n", current, errno);
        }

        err = SetCapabilities(permittedCapabilities, effectiveCapabilities);
        if (err != 0) {
            // Logger::E(TAG, "cannot set capabilities (%llx,%llx): %s",
            //     permittedCapabilities, effectiveCapabilities, strerror(err));
            // dvmAbort();
        }

        err = set_sched_policy(0, SP_DEFAULT);
        if (err < 0) {
            // Logger::E(TAG, "cannot set_sched_policy(0, SP_DEFAULT): %s", strerror(-err));
            // dvmAbort();
        }

#ifdef HAVE_SELINUX
        err = SetSELinuxContext(uid, true, seInfo, niceName);
        if (err < 0) {
            // Logger::E(TAG, "cannot set SELinux context: %s\n", strerror(errno));
            // dvmAbort();
        }
        // These free(3) calls are safe because we know we're only ever forking
        // a single-threaded process, so we know no other thread held the heap
        // lock when we forked.
        free(seInfo);
        free(niceName);
#endif

        /*
         * Our system thread ID has changed.  Get the new one.
         */
        // Thread* thread = dvmThreadSelf();
        // thread->systemTid = dvmGetSysThreadId();

        /* configure additional debug options */
        EnableDebugFeatures(debugFlags);

        UnsetSignalHandler();
        // gDvm.zygote = false;
        // if (!dvmInitAfterZygote()) {
        //     ALOGE("error in post-zygote initialization");
        //     dvmAbort();
        // }
    }
    else if (pid > 0) {
        /* the parent process */
#ifdef HAVE_SELINUX
        free(seInfo);
        free(niceName);
#endif
    }

    return pid;
}

static pid_t ForkAndSpecializeNormal(
    /* [in] */ uid_t uid,
    /* [in] */ gid_t gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int32 mountExternal,
    /* [in] */ const String& seInfo,
    /* [in] */ const String& niceName)
{
    return ForkAndSpecializeCommon(uid, gid, gids, debugFlags, rlimits,
            mountExternal, seInfo, niceName, 0, 0);
}

Int32 CZygote::NativeForkAndSpecialize(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int32 mountExternal,
    /* [in] */ const String& seInfo,
    /* [in] */ const String& niceName)
{
    pid_t pid = ForkAndSpecializeNormal((uid_t)uid, (gid_t)gid, gids, debugFlags, rlimits,
            mountExternal, seInfo, niceName);

    return (Int32)pid;
}

/*
 * Utility routine to fork zygote and specialize the child process.
 */
static pid_t ForkAndSpecializeSystemServer(
    /* [in] */ uid_t uid,
    /* [in] */ gid_t gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int64 permittedCapabilities,
    /* [in] */ Int64 effectiveCapabilities)
{
    return ForkAndSpecializeCommon(uid, gid, gids, debugFlags, rlimits, MOUNT_EXTERNAL_NONE,
            String(NULL), String(NULL), permittedCapabilities, effectiveCapabilities);
}

Int32 CZygote::NativeForkSystemServer(
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid,
    /* [in] */ ArrayOf<Int32>* gids,
    /* [in] */ Int32 debugFlags,
    /* [in] */ ArrayOf<Int32*>* rlimits, //int[][] rlimits,
    /* [in] */ Int64 permittedCapabilities,
    /* [in] */ Int64 effectiveCapabilities)
{
    pid_t pid;
    pid = ForkAndSpecializeSystemServer((uid_t)uid, (gid_t)gid, gids, debugFlags, rlimits,
            permittedCapabilities, effectiveCapabilities);

    /* The zygote process checks whether the child process has died or not. */
    if (pid > 0) {
        int status;

        // Logger::I(TAG, "System server process %d has been created", pid);
        sSystemServerPid = pid;
        /* There is a slight window that the system server process has crashed
         * but it went unnoticed because we haven't published its pid yet. So
         * we recheck here just to make sure that all is well.
         */
        if (waitpid(pid, &status, WNOHANG) == pid) {
            // Logger::E(TAG, "System server process %d has died. Restarting Zygote!", pid);
            kill(getpid(), SIGKILL);
        }
    }
    return pid;
}

ECode CZygote::GetSystemInSafeMode(
        /* [out] */ Boolean* mode)
{
    VALIDATE_NOT_NULL(mode);

    *mode = sSystemInSafeMode;
    return NOERROR;
}

} // namespace System
} // namespace Droid
} // namespace Elastos
