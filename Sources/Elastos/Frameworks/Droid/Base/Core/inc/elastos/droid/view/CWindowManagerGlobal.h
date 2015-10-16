
#ifndef __ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__
#define __ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__

#include "_Elastos_Droid_View_CWindowManagerGlobal.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace View {

class ViewRootImpl;

CarClass(CWindowManagerGlobal)
{
private:
    class SystemPropertyUpdaterRunnable : public Runnable
    {
    public:
        SystemPropertyUpdaterRunnable(
            /* [in] */ CWindowManagerGlobal* owner);

        CARAPI Run();

    private:
        CWindowManagerGlobal* mOwner;
    };

public:
    CWindowManagerGlobal();

    static AutoPtr<IWindowManagerGlobal> GetInstance();

    static AutoPtr<IIWindowManager> GetWindowManagerService();

    static AutoPtr<IWindowSession> GetWindowSession(
        /* [in] */ ILooper* mainLooper);

    static AutoPtr<IWindowSession> PeekWindowSession();

    CARAPI GetWindowManagerService(
        /* [out] */ IIWindowManager** windowManager);

    CARAPI GetWindowSession(
        /* [in] */ ILooper* mainLooper,
        /* [out] */ IWindowSession** windowSession);

    CARAPI PeekWindowSession(
        /* [out] */ IWindowSession** windowSession);

    CARAPI AddView(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params,
        /* [in] */ IDisplay* display,
        /* [in] */ IWindow* parentWindow);

    CARAPI UpdateViewLayout(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI RemoveView(
        /* [in] */ IView* view,
        /* [in] */ Boolean immediate);

    CARAPI CloseAll(
        /* [in] */ IBinder* token,
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI SetStoppedState(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean stopped);

    CARAPI ReportNewConfiguration(
        /* [in] */ IConfiguration* config);

private:
    AutoPtr<IView> RemoveViewLocked(
        /* [in] */ Int32 index,
        /* [in] */ Boolean immediate);

    ECode FindViewLocked(
        /* [in] */ IView* view,
        /* [in] */ Boolean required,
        /* [out] */ Int32* result);

    CARAPI StartTrimMemory(
        /* [in] */ Int32 level);

    CARAPI EndTrimMemory();

    CARAPI TrimLocalMemory();

    CARAPI DumpGfxInfo(
        /* [in] */ IFileDescriptor* fd);

    static String GetWindowName(
        /* [in] */ ViewRootImpl* root);

private:
    static const char* TAG;

    static AutoPtr<IWindowManagerGlobal> sDefaultWindowManager;
    static Object sDefaultWindowManagerLock;
    static AutoPtr<IIWindowManager> sWindowManagerService;
    static AutoPtr<IWindowSession> sWindowSession;

    Object mLock;

    AutoPtr<ArrayOf<IView*> > mViews;
    AutoPtr<ArrayOf<ViewRootImpl*> > mRoots;
    AutoPtr<ArrayOf<IWindowManagerLayoutParams*> > mParams;
    Boolean mNeedsEglTerminate;

    AutoPtr<IRunnable> mSystemPropertyUpdater;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__
