
#ifndef __ELASTOS_DROID_SERVER_WM_PROCESSLIST_H__
#define __ELASTOS_DROID_SERVER_WM_PROCESSLIST_H__

#include <elastos.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {
    class CWindowManagerService;
}}}}

using Elastos::Droid::Server::Wm::CWindowManagerService;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

class ProcessList : public Object
{
public:
    ProcessList();

    CARAPI ApplyDisplaySize(
        /* [in] */ CWindowManagerService* wm);

    CARAPI_(Int64) GetMemLevel(
        /* [in] */ Int32 adjustment);

    CARAPI_(String) ToString();

private:
    CARAPI UpdateOomLevels(
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 displayHeight,
        /* [in] */ Boolean write);

    CARAPI WriteFile(
        /* [in] */ const String& path,
        /* [in] */ const String& data);

public:
    // The minimum time we allow between crashes, for us to consider this
    // application to be bad and stop and its services and reject broadcasts.
    static const Int32 MIN_CRASH_INTERVAL = 60*1000;

    // OOM adjustments for processes in various states:

    // This is a process only hosting activities that are not visible,
    // so it can be killed without any disruption.
    static const Int32 HIDDEN_APP_MAX_ADJ = 15;
    static const Int32 HIDDEN_APP_MIN_ADJ = 9;

    // The B list of SERVICE_ADJ -- these are the old and decrepit
    // services that aren't as shiny and interesting as the ones in the A list.
    static const Int32 SERVICE_B_ADJ = 8;

    // This is the process of the previous application that the user was in.
    // This process is kept above other things, because it is very common to
    // switch back to the previous app.  This is important both for recent
    // task switch (toggling between the two top recent apps) as well as normal
    // UI flow such as clicking on a URI in the e-mail app to view in the browser,
    // and then pressing back to return to e-mail.
    static const Int32 PREVIOUS_APP_ADJ = 7;

    // This is a process holding the home application -- we want to try
    // avoiding killing it, even if it would normally be in the background,
    // because the user interacts with it so much.
    static const Int32 HOME_APP_ADJ = 6;

    // This is a process holding an application service -- killing it will not
    // have much of an impact as far as the user is concerned.
    static const Int32 SERVICE_ADJ = 5;

    // This is a process currently hosting a backup operation.  Killing it
    // is not entirely fatal but is generally a bad idea.
    static const Int32 BACKUP_APP_ADJ = 4;

    // This is a process with a heavy-weight application.  It is in the
    // background, but we want to try to avoid killing it.  Value set in
    // system/rootdir/init.rc on startup.
    static const Int32 HEAVY_WEIGHT_APP_ADJ = 3;

    // This is a process only hosting components that are perceptible to the
    // user, and we really want to avoid killing them, but they are not
    // immediately visible. An example is background music playback.
    static const Int32 PERCEPTIBLE_APP_ADJ = 2;

    // This is a process only hosting activities that are visible to the
    // user, so we'd prefer they don't disappear.
    static const Int32 VISIBLE_APP_ADJ = 1;

    // This is the process running the current foreground app.  We'd really
    // rather not kill it!
    static const Int32 FOREGROUND_APP_ADJ = 0;

    // This is a system persistent process, such as telephony.  Definitely
    // don't want to kill it, but doing so is not completely fatal.
    static const Int32 PERSISTENT_PROC_ADJ = -12;

    // The system process runs at the default adjustment.
    static const Int32 SYSTEM_ADJ = -16;

    // Memory pages are 4K.
    static const Int32 MEMORY_PAGE_SIZE = 4 * 1024;

    // The minimum number of hidden apps we want to be able to keep around,
    // without empty apps being able to push them out of memory.
    static const Int32 MIN_HIDDEN_APPS = 2;

    // The maximum number of hidden processes we will keep around before
    // killing them; this is just a control to not let us go too crazy with
    // keeping around processes on devices with large amounts of RAM.
    static const Int32 MAX_HIDDEN_APPS = 24;

    // We allow empty processes to stick around for at most 30 minutes.
    static const Int64 MAX_EMPTY_TIME = 30 * 60 * 1000;

    // The number of hidden at which we don't consider it necessary to do
    // memory trimming.
    static const Int32 TRIM_HIDDEN_APPS = 3;

    // The number of empty apps at which we don't consider it necessary to do
    // memory trimming.
    static const Int32 TRIM_EMPTY_APPS = 3;

    // Threshold of number of hidden+empty where we consider memory critical.
    static const Int32 TRIM_CRITICAL_THRESHOLD = 3;

    // Threshold of number of hidden+empty where we consider memory critical.
    static const Int32 TRIM_LOW_THRESHOLD = 5;

    // We put empty content processes after any hidden processes that have
    // been idle for less than 15 seconds.
    static const Int64 CONTENT_APP_IDLE_OFFSET = 15 * 1000;

    // We put empty content processes after any hidden processes that have
    // been idle for less than 120 seconds.
    static const Int64 EMPTY_APP_IDLE_OFFSET = 120 * 1000;

private:
    // These are the various interesting memory levels that we will give to
    // the OOM killer.  Note that the OOM killer only supports 6 slots, so we
    // can't give it a different value for every possible kind of process.
    AutoPtr< ArrayOf<Int32> > mOomAdj;

    // These are the low-end OOM level limits.  This is appropriate for an
    // HVGA or smaller phone with less than 512MB.  Values are in KB.
    AutoPtr< ArrayOf<Int64> > mOomMinFreeLow;

    // These are the high-end OOM level limits.  This is appropriate for a
    // 1280x800 or larger screen with around 1GB RAM.  Values are in KB.
    AutoPtr< ArrayOf<Int64> > mOomMinFreeHigh;

    // The actual OOM killer memory levels we are using.
    AutoPtr< ArrayOf<Int64> > mOomMinFree;

    Int64 mTotalMemMb;

    Boolean mHaveDisplaySize;

    String mStringName;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_WM_PROCESSLIST_H__
