#ifndef __ELASTOS_DROID_SERVER_BOOT_RECEIVER_H__
#define __ELASTOS_DROID_SERVER_BOOT_RECEIVER_H__

#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/os/FileObserver.h"
#include <elastos/core/Thread.h>

using Elastos::Droid::Os::FileObserver;
using Elastos::Droid::Os::IFileObserver;
using Elastos::Droid::Os::IDropBoxManager;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Core::Thread;
using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Server {

/**
 * Performs a number of miscellaneous, non-system-critical actions
 * after the system has finished booting.
 */
class BootReceiver
    : public BroadcastReceiver
{
private:
    class MyThread
        : public Thread
    {
    public:
        MyThread(
            /* [in] */ BootReceiver* host，
            /* [in] */ IContext* ctx);

        //@Override
        CARAPI Run();
    private:
        BootReceiver* mHost;
        AutoPtr<IContext> mContext;
    };

    class TombstoneObserver
        : public FileObserver
    {
    public:
        TombstoneObserver(
            /* [in] */ const String& path,
            /* [in] */ Int32 mask,
            /* [in] */ IDropBoxManager* mgr,
            /* [in] */ ISharedPreferences* pref,
            /* [in] */ const String& headers,
            /* [in] */ BootReceiver* host);

        //@Override
        CARAPI OnEvent(
            /* [in] */ Int32 event,
            /* [in] */ const String& path);
    private:
        BootReceiver* mHost;
        AutoPtr<IDropBoxManager> mDropBoxManager;
        AutoPtr<ISharedPreferences> mSharedPreferences;
        String mHeaders;
    };
public:

    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

private:

    CARAPI RemoveOldUpdatePackages(
        /* [in] */ IContext* context);

    CARAPI LogBootEvents(
        /* [in] */ IContext* ctx);

    static CARAPI AddFileToDropBox(
        /* [in] */ IDropBoxManager* db,
        /* [in] */ ISharedPreferences* prefs,
        /* [in] */ const String& headers,
        /* [in] */ const String& footers,
        /* [in] */ const String& filename,
        /* [in] */ Int32 maxSize,
        /* [in] */ const String& tag);

    static CARAPI AddFileWithFootersToDropBox(
        /* [in] */ IDropBoxManager* db,
        /* [in] */ ISharedPreferences* prefs,
        /* [in] */ const String& headers,
        /* [in] */ const String& footers,
        /* [in] */ const String& filename,
        /* [in] */ Int32 maxSize,
        /* [in] */ const String& tag);

    static CARAPI AddAuditErrorsToDropBox(
        /* [in] */ IDropBoxManager* db,
        /* [in] */ ISharedPreferences* prefs,
        /* [in] */ const String& headers,
        /* [in] */ Int32 maxSize,
        /* [in] */ const String& tag);

    static CARAPI AddFsckErrorsToDropBox(
        /* [in] */ IDropBoxManager* db,
        /* [in] */ ISharedPreferences* prefs,
        /* [in] */ const String& headers,
        /* [in] */ Int32 maxSize,
        /* [in] */ const String& tag);

private:
    friend class TombstoneObserver;
    friend class MyThread;

    static const String TAG;

    // Maximum size of a logged event (files get truncated if they're longer).
    // Give userdebug builds a larger max to capture extra debug, esp. for last_kmsg.
    static const Int32 LOG_SIZE;

    static const AutoPtr<IFile> TOMBSTONE_DIR;

    // The pre-froyo package and class of the system updater, which
    // ran in the system process.  We need to remove its packages here
    // in order to clean up after a pre-froyo-to-froyo update.
    static const String OLD_UPDATER_PACKAGE;
    static const String OLD_UPDATER_CLASS;

    // Keep a reference to the observer so the finalizer doesn't disable it.
    static AutoPtr<IFileObserver> sTombstoneObserver;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_BOOT_RECEIVER_H__