
#include "DroidRuntime.h"
#include <elastos/Logger.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdlib.h>

using Elastos::Utility::Logging::Logger;

#ifndef ASSERT_TRUE
#define ASSERT_TRUE(expr) \
    do { \
        Boolean assert_result_ = (Boolean)(expr); \
        assert(assert_result_); \
    } while(0);
#endif

extern AutoPtr<Elastos::Droid::DroidRuntime> sCurRuntime;

namespace Elastos {
namespace Droid {

static const String TAG("DroidRuntime");

static int hasDir(const char* dir)
{
    struct stat s;
    int res = stat(dir, &s);
    if (res == 0) {
        return S_ISDIR(s.st_mode);
    }
    return 0;
}

/*
 * We just want failed write() calls to just return with an error.
 */
static void BlockSigpipe()
{
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGPIPE);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
        Logger::W(TAG, "WARNING: SIGPIPE not blocked\n");
    }
}

DroidRuntime::DroidRuntime()
{
    sCurRuntime = this;
}

DroidRuntime::~DroidRuntime()
{}

ECode DroidRuntime::CallMain(
    /* [in] */ const String& moduleName,
    /* [in] */ const String& className,
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    AutoPtr<IMethodInfo> methodInfo;
    AutoPtr<IArgumentList> argumentList;

    ECode ec = _CReflector_AcquireModuleInfo(
            moduleName.string(), (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        Logger::E(TAG, "Acquire \"%s\" module info failed!\n", moduleName.string());
        return ec;
    }

    ec = moduleInfo->GetClassInfo(
            className.string(), (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        Logger::E(TAG, "Acquire \"%s\" class info failed!\n", className.string());
        return ec;
    }

    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        Logger::E(TAG, "Create object failed!\n");
        return ec;
    }

    ec = classInfo->GetMethodInfo(
            "Main", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        Logger::E(TAG, "Acquire \"Main\" method info failed!\n");
        return ec;
    }

    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        Logger::E(TAG, "Create \"Main\" method argument list failed!\n");
        return ec;
    }

    argumentList->SetInputArgumentOfCarArray(0, args);
    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        Logger::E(TAG, "Invoke \"Main\" method failed!\n");
        return ec;
    }
    return NOERROR;
}

/*
 * Configure signals.  We need to block SIGQUIT so that the signal only
 * reaches the dump-stack-trace thread.
 *
 * This can be disabled with the "-Xrs" flag.
 */
static void BlockSignals()
{
    sigset_t mask;
    int cc;

    sigemptyset(&mask);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGUSR1);      // used to initiate heap dump
//#if defined(WITH_JIT) && defined(WITH_JIT_TUNING)
//    sigaddset(&mask, SIGUSR2);      // used to investigate JIT internals
//#endif
    //sigaddset(&mask, SIGPIPE);
    cc = sigprocmask(SIG_BLOCK, &mask, NULL);
    assert(cc == 0);

//    if (false) {
//        /* TODO: save the old sigaction in a global */
//        struct sigaction sa;
//        memset(&sa, 0, sizeof(sa));
//        sa.sa_sigaction = busCatcher;
//        sa.sa_flags = SA_SIGINFO;
//        cc = sigaction(SIGBUS, &sa, NULL);
//        assert(cc == 0);
//    }
}

/*
 * Start the Android runtime.  This involves starting the virtual machine
 * and calling the "static void main(String[] args)" method in the class
 * named by "className".
 *
 * Passes the main function two arguments, the class name and the specified
 * options string.
 */
void DroidRuntime::Start(
    /* [in] */ const char* moduleName,
    /* [in] */ const char* className,
    /* [in] */ const char* options)
{
    Logger::D(TAG, "\n>>>>>> AndroidRuntime START %s - %s <<<<<<\n",
            moduleName != NULL ? moduleName : "(unknown)",
            className != NULL ? className : "(unknown)");

    BlockSigpipe();

    /*
     * 'startSystemServer == true' means runtime is obsolete and not run from
     * init.rc anymore, so we print out the boot start event here.
     */
    if (strcmp(options, "start-system-server") == 0) {
//        /* track our progress through the boot sequence */
//        const int LOG_BOOT_PROGRESS_START = 3000;
//        LOG_EVENT_LONG(LOG_BOOT_PROGRESS_START,
//                       ns2ms(systemTime(SYSTEM_TIME_MONOTONIC)));
    }

    const char* rootDir = getenv("ANDROID_ROOT");
    if (rootDir == NULL) {
        rootDir = "/system";
        if (!hasDir("/system")) {
            Logger::E(TAG, "No root directory specified, and /android does not exist.");
            return;
        }
        setenv("ANDROID_ROOT", rootDir, 1);
    }

    //const char* kernelHack = getenv("LD_ASSUME_KERNEL");
    //ALOGD("Found LD_ASSUME_KERNEL='%s'\n", kernelHack);

    // from startVm
    BlockSignals();

    /*
     * We want to call main() with a String array with arguments in it.
     * At present we have two arguments, the class name and an option string.
     * Create an array to hold them.
     */
    AutoPtr< ArrayOf<String> > args = ArrayOf<String>::Alloc(2);
    args->Set(0, String(className));
    args->Set(1, String(options));

    CallMain(String(moduleName), String(className), args.Get());

    Logger::D(TAG, "Shutting down\n");
}

AutoPtr<DroidRuntime> DroidRuntime::GetRuntime()
{
    return sCurRuntime;
}

} // namespace Droid
} // namespace Elastos
