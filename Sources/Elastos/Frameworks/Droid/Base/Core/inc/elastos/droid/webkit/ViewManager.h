
#ifndef __ELASTOS_DROID_WEBKIT_VIEWMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_VIEWMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::IRunnable;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ISurfaceView;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

class ViewManager : public ElRefBase
{
public:
    class ChildView
        : public ElRefBase
        , public IInterface
    {
    private:
        class AttachViewRunnable : public Runnable
        {
        public:
            AttachViewRunnable(
                /* [in] */ ChildView* owner);

            CARAPI Run();

        private:
            AutoPtr<ChildView> mOwner;
        };

        class RemoveViewRunnable : public Runnable
        {
        public:
            RemoveViewRunnable(
                /* [in] */ ChildView* owner);

            CARAPI Run();

        private:
            AutoPtr<ChildView> mOwner;
        };

    public:
        ChildView(
            /* [in] */ ViewManager* owner);

        CAR_INTERFACE_DECL()

        CARAPI_(void) SetBounds(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI_(void) AttachView(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height);

        CARAPI_(void) RemoveView();

    private:
        CARAPI_(void) AttachViewOnUIThread();

        CARAPI_(void) RemoveViewOnUIThread();

    public:
        Int32 mX;
        Int32 mY;
        Int32 mWidth;
        Int32 mHeight;
        AutoPtr<IView> mView; // generic view to show
        AutoPtr<ViewManager> mOwner;
    };

private:
    class RequestLayoutRunnable : public Runnable
    {
    public:
        RequestLayoutRunnable(
            /* [in] */ ISurfaceView* v);

        CARAPI Run();

    private:
        AutoPtr<ISurfaceView> mView;
    };

    class PostResetStateAllRunnable : public Runnable
    {
    public:
        PostResetStateAllRunnable(
            /* [in] */ ViewManager* owner);

        CARAPI Run();

    private:
        AutoPtr<ViewManager> mOwner;
    };

    class PostReadyToDrawAllRunnable : public Runnable
    {
    public:
        PostReadyToDrawAllRunnable(
            /* [in] */ ViewManager* owner);

        CARAPI Run();

    private:
        AutoPtr<ViewManager> mOwner;
    };

public:
    ViewManager(
        /* [in] */ CWebViewClassic* w);

    CARAPI_(AutoPtr<ChildView>) CreateView();

    CARAPI_(void) StartZoom();

    CARAPI_(void) EndZoom();

    CARAPI_(void) ScaleAll();

    CARAPI_(void) HideAll();

    CARAPI_(void) ShowAll();

    CARAPI_(void) PostResetStateAll();

    CARAPI_(void) PostReadyToDrawAll();

    CARAPI_(AutoPtr<ChildView>) HitTest(
        /* [in] */ Int32 contentX,
        /* [in] */ Int32 contentY);

private:
    /**
     * This should only be called from the UI thread.
     */
    CARAPI_(void) RequestLayout(
        /* [in] */ ChildView* v);

private:
    AutoPtr<CWebViewClassic> mWebView;
    List< AutoPtr<ChildView> > mChildren;
    Boolean mHidden;
    Boolean mReadyToDraw;
    Boolean mZoomInProgress;

    // Threshold at which a surface is prevented from further increasing in size
    Int32 MAX_SURFACE_AREA;
    // GPU Limit (hard coded for now)
    static const Int32 MAX_SURFACE_DIMENSION;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_VIEWMANAGER_H__
