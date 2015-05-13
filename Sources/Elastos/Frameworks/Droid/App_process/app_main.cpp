
#include <ext/frameworkdef.h>
#include <DroidRuntime.h>
#include <cutils/process_name.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {

class AppRuntime : public DroidRuntime
{
public:
    AppRuntime() {}

    CARAPI_(void) OnStarted();

public:
    String mModuleName;
    String mClassName;
    AutoPtr<ArrayOf<String> > mArgs;
};

void AppRuntime::OnStarted()
{
    android::ProcessState::self()->startThreadPool();
    DroidRuntime::GetRuntime()->CallMain(mModuleName, mClassName, mArgs);
    android::IPCThreadState::self()->stopProcess();
}

} // namespace Droid
} // namespace Elastos

/*
 * sets argv0 to as much of newArgv0 as will fit
 */
static void setArgv0(const char *argv0, const char *newArgv0)
{
    strlcpy(const_cast<char *>(argv0), newArgv0, strlen(argv0));
}

int main(int argc, char* argv[])
{
    // These are global variables in ProcessState.cpp
    android::mArgC = argc;
    android::mArgV = argv;

    android::mArgLen = 0;
    for (int i=0; i<argc; i++) {
        android::mArgLen += strlen(argv[i]) + 1;
    }
    android::mArgLen--;

    Elastos::Droid::AppRuntime runtime;
    const char* argv0 = argv[0];

    // Process command line arguments
    // ignore argv[0]
    argc--;
    argv++;

    int i = 0;

    // Parse runtime arguments.  Stop at first unrecognized option.
    bool zygote = false;
    bool startSystemServer = false;
    bool application = false;
    const char* parentDir = NULL;
    const char* niceName = NULL;
    const char* className = NULL;
    while (i < argc) {
        const char* arg = argv[i++];
        if (!parentDir) {
            parentDir = arg;
        }
        else if (strcmp(arg, "--zygote") == 0) {
            zygote = true;
            niceName = "elzygote";
        }
        else if (strcmp(arg, "--start-system-server") == 0) {
            startSystemServer = true;
        }
        else if (strcmp(arg, "--application") == 0) {
            application = true;
        }
        else if (strncmp(arg, "--nice-name=", 12) == 0) {
            niceName = arg + 12;
        }
        else {
            className = arg;
            break;
        }
    }

    if (niceName && *niceName) {
        setArgv0(argv0, niceName);
        set_process_name(niceName);
    }

    if (zygote) {
        runtime.Start("Elastos.Droid.Core.eco", "CZygoteInit",
                startSystemServer ? "start-system-server" : "");
    }
    else if (className) {
// Remainder of args get passed to startup class main()
//        runtime.mClassName = className;
//        runtime.mArgC = argc - i;
//        runtime.mArgV = argv + i;
//runtime.start("com.android.internal.os.RuntimeInit",
//                application ? "application" : "tool");
        AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(argc - i);
        ASSERT_TRUE(args != NULL);
        for (Int32 index = 0; index < argc - i; ++index) {
            (*args)[index] = argv[i + index];
        }

        if (0 == strcmp(className, "CMonkey")) {
            runtime.mModuleName = "Elastos.Droid.Commands.Monkey.eco";
        } else if (0 == strcmp(className, "CAm")) {
            runtime.mModuleName = "Elastos.Droid.Commands.Am.eco";
        }

        runtime.mClassName = className;
        runtime.mArgs = args;
        runtime.Start("Elastos.Droid.Core.eco", "CRuntimeInit",
            application ? "application" : "tool");
    }
    else {
//        fprintf(stderr, "Error: no class name or --zygote supplied.\n");
//        app_usage();
//        LOG_ALWAYS_FATAL("app_process: no class name or --zygote supplied.");
//        return 10;
    }
}
