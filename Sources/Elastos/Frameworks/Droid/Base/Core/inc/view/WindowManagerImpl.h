
#ifndef __WINDOWMANAGERIMPL_H__
#define __WINDOWMANAGERIMPL_H__

#include "ext/frameworkdef.h"
#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace View {

class WindowManagerImpl : public ElRefBase, public IWindowManager
{
public:
    WindowManagerImpl(
        /* [in] */ IDisplay* display);

private:
    WindowManagerImpl(
        /* [in] */ IDisplay* display,
        /* [in] */ IWindow* parentWindow);

public:
    CAR_INTERFACE_DECL();

    CARAPI_(AutoPtr<WindowManagerImpl>) CreateLocalWindowManager(
        /* [in] */ IWindow* parentWindow);

    CARAPI_(AutoPtr<WindowManagerImpl>) CreatePresentationWindowManager(
        /* [in] */ IDisplay* display);

    CARAPI AddViewEx5(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI UpdateViewLayout(
        /* [in] */ IView* view,
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI RemoveView(
        /* [in] */ IView* view);

    CARAPI RemoveViewImmediate(
        /* [in] */ IView* view);

    CARAPI GetDefaultDisplay(
        /* [out] */ IDisplay** display);

private:
    AutoPtr<IWindowManagerGlobal> mGlobal;
    AutoPtr<IDisplay> mDisplay;
    IWindow* mParentWindow;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__WINDOWMANAGERIMPL_H__
