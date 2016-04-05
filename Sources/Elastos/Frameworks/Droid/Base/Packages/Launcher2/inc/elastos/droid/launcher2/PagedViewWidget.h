#ifndef  __ELASTOS_DROID_LAUNCHER2_PAGEDVIEWWIDGET_H__
#define  __ELASTOS_DROID_LAUNCHER2_PAGEDVIEWWIDGET_H__

#include "_Launcher2.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/LinearLayout.h"
#include "elastos/droid/os/Runnable.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.AppWidget.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.Utility.h"
#include "Elastos.Droid.View.h"

using Elastos::Droid::App::IFastBitmapDrawable;
using Elastos::Droid::AppWidget::IAppWidgetProviderInfo;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Widget::LinearLayout;
using Elastos::Droid::Os::Runnable;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

class PagedViewWidget
    : public LinearLayout
    , public IPagedViewWidget
{
private:
    class CheckForShortPress
        : public Runnable
    {
    public:
        CheckForShortPress(
            /* [in] */ PagedViewWidget* host);

        CARAPI Run();

    private:
        PagedViewWidget* mHost;
    };

public:
    CAR_INTERFACE_DECL();

    PagedViewWidget();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    static CARAPI SetDeletePreviewsWhenDetachedFromWindow(
        /* [in] */ Boolean value);

    static CARAPI SetRecyclePreviewsWhenDetachedFromWindow(
        /* [in] */ Boolean value);

    CARAPI ApplyFromAppWidgetProviderInfo(
        /* [in] */ IAppWidgetProviderInfo* info,
        /* [in] */ Int32 maxWidth,
        /* [in] */ ArrayOf<Int32>* cellSpan,
        /* [in] */ IWidgetPreviewLoader* loader);

    CARAPI ApplyFromResolveInfo(
        /* [in] */ IPackageManager* pm,
        /* [in] */ IResolveInfo* info,
        /* [in] */ IWidgetPreviewLoader* loader);

    CARAPI GetPreviewSize(
        /* [out, callee] */ ArrayOf<Int32>** size);

    CARAPI ApplyPreview(
        /* [in] */ IFastBitmapDrawable* preview,
        /* [in] */ Int32 index);

    CARAPI SetShortPressListener(
        /* [in] */ IPagedViewWidgetShortPressListener* listener);

    static CARAPI ResetShortPressTarget();

    //@Override
    CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* res);

protected:
    //@Override
    CARAPI OnFinishInflate();

    //@Override
    CARAPI OnDetachedFromWindow();

private:
    CARAPI_(void) MyCheckForShortPress();

    /**
     * Remove the longpress detection timer.
     */
    CARAPI_(void) RemoveShortPressCallback();

    CARAPI_(void) CleanUpShortPress();

protected:
    static const String TAG;

    AutoPtr<PagedViewWidget::CheckForShortPress> mPendingCheckForShortPress;
    AutoPtr<IPagedViewWidgetShortPressListener> mShortPressListener;
    Boolean mShortPressTriggered;
    static AutoPtr<IPagedViewWidget> sShortpressTarget;
    Boolean mIsAppWidget;

private:
    static Boolean sDeletePreviewsWhenDetachedFromWindow;
    static Boolean sRecyclePreviewsWhenDetachedFromWindow;

    String mDimensionsFormatString;

    AutoPtr<IRect> mOriginalImagePadding;
    AutoPtr<IInterface> mInfo;
    AutoPtr<IWidgetPreviewLoader> mWidgetPreviewLoader;
};

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_LAUNCHER2_PAGEDVIEWWIDGET_H__