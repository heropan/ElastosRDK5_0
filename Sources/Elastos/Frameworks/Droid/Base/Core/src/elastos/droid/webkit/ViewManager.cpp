
#include "elastos/droid/webkit/ViewManager.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/widget/CAbsoluteLayoutLayoutParams.h"
#include <elastos/core/Math.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Droid::Widget::IAbsoluteLayoutLayoutParams;
using Elastos::Droid::Widget::CAbsoluteLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//           ViewManager::ChildView::AttachViewRunnable
//===============================================================
ViewManager::ChildView::AttachViewRunnable::AttachViewRunnable(
    /* [in] */ ChildView* owner)
    : mOwner(owner)
{}

ECode ViewManager::ChildView::AttachViewRunnable::Run()
{
    // This method may be called multiple times. If the view is
    // already attached, just set the new LayoutParams,
    // otherwise attach the view and add it to the list of
    // children.
    mOwner->mOwner->RequestLayout(mOwner);

    AutoPtr<IViewParent> viewParent;
    mOwner->mView->GetParent((IViewParent**)&viewParent);
    if (viewParent == NULL) {
        mOwner->AttachViewOnUIThread();
    }
    return NOERROR;
}


//===============================================================
//           ViewManager::ChildView::RemoveViewRunnable
//===============================================================
ViewManager::ChildView::RemoveViewRunnable::RemoveViewRunnable(
    /* [in] */ ChildView* owner)
    : mOwner(owner)
{}

ECode ViewManager::ChildView::RemoveViewRunnable::Run()
{
    mOwner->RemoveViewOnUIThread();
    return NOERROR;
}


//===============================================================
//                    ViewManager::ChildView
//===============================================================
ViewManager::ChildView::ChildView(
    /* [in] */ ViewManager* owner)
   : mX(0)
   , mY(0)
   , mWidth(0)
   , mHeight(0)
   , mOwner(owner)
{}

CAR_INTERFACE_IMPL(ViewManager::ChildView, IInterface)

void ViewManager::ChildView::SetBounds(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
}

void ViewManager::ChildView::AttachView(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mView == NULL) {
        return;
    }
    SetBounds(x, y, width, height);

    AutoPtr<IRunnable> runnable = new AttachViewRunnable(this);
    Boolean result = FALSE;
    mOwner->mWebView->mPrivateHandler->Post(runnable, &result);
}

void ViewManager::ChildView::AttachViewOnUIThread()
{
    AutoPtr<IWebView> webView;
    mOwner->mWebView->GetWebView((IWebView**)&webView);
    webView->AddView(mView);
    mOwner->mChildren.PushBack(this);
    if (!mOwner->mReadyToDraw) {
        mView->SetVisibility(IView::GONE);
    }
}

void ViewManager::ChildView::RemoveView()
{
    if (mView == NULL) {
        return;
    }
    AutoPtr<IRunnable> runnable = new RemoveViewRunnable(this);
    Boolean result = FALSE;
    mOwner->mWebView->mPrivateHandler->Post(runnable, &result);
}

void ViewManager::ChildView::RemoveViewOnUIThread()
{
    AutoPtr<IWebView> webView;
    mOwner->mWebView->GetWebView((IWebView**)&webView);
    IViewManager::Probe(webView)->RemoveView(mView);
    mOwner->mChildren.Remove(this);
}


//===============================================================
//             ViewManager::RequestLayoutRunnable
//===============================================================
ViewManager::RequestLayoutRunnable::RequestLayoutRunnable(
    /* [in] */ ISurfaceView* v)
    : mView(v)
{}

ECode ViewManager::RequestLayoutRunnable::Run()
{
    mView->SetVisibility(IView::VISIBLE);
    return NOERROR;
}


//===============================================================
//             ViewManager::PostResetStateAllRunnable
//===============================================================
ViewManager::PostResetStateAllRunnable::PostResetStateAllRunnable(
    /* [in] */ ViewManager* owner)
    : mOwner(owner)
{}

ECode ViewManager::PostResetStateAllRunnable::Run()
{
    mOwner->mReadyToDraw = FALSE;
    return NOERROR;
}


//===============================================================
//             ViewManager::PostReadyToDrawAllRunnable
//===============================================================
ViewManager::PostReadyToDrawAllRunnable::PostReadyToDrawAllRunnable(
    /* [in] */ ViewManager* owner)
    : mOwner(owner)
{}

ECode ViewManager::PostReadyToDrawAllRunnable::Run()
{
    mOwner->mReadyToDraw = TRUE;
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mOwner->mChildren.Begin(); it != mOwner->mChildren.End(); ++it) {
        (*it)->mView->SetVisibility(IView::VISIBLE);
    }
    return NOERROR;
}


//===============================================================
//                         ViewManager
//===============================================================
// GPU Limit (hard coded for now)
const Int32 ViewManager::MAX_SURFACE_DIMENSION = 2048;

ViewManager::ViewManager(
    /* [in] */ CWebViewClassic* w)
    : mWebView(w)
    , mHidden(FALSE)
    , mReadyToDraw(FALSE)
    , mZoomInProgress(FALSE)
{
    AutoPtr<IWebView> webView;
    w->GetWebView((IWebView**)&webView);
    AutoPtr<IResources> res;
    webView->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Int32 widthPixels, heightPixels;
    metrics->GetWidthPixels(&widthPixels);
    metrics->GetHeightPixels(&heightPixels);
    Int32 pixelArea = widthPixels * heightPixels;
    /* set the threshold to be 275% larger than the screen size. The
       percentage is simply an estimation and is not based on anything but
       basic trial-and-error tests run on multiple devices.
     */
    MAX_SURFACE_AREA = (Int32)(pixelArea * 2.75);
}

AutoPtr<ViewManager::ChildView> ViewManager::CreateView()
{
    AutoPtr<ViewManager::ChildView> v = new ChildView(this);
    return v;
}

void ViewManager::RequestLayout(
        /* [in] */ ChildView* v)
{
    Int32 width = mWebView->ContentToViewDimension(v->mWidth);
    Int32 height = mWebView->ContentToViewDimension(v->mHeight);
    Int32 x = mWebView->ContentToViewX(v->mX);
    Int32 y = mWebView->ContentToViewY(v->mY);

    AutoPtr<IAbsoluteLayoutLayoutParams> lp;
    AutoPtr<IViewGroupLayoutParams> layoutParams;
    v->mView->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);

    if (IAbsoluteLayoutLayoutParams::Probe(layoutParams)) {
        lp = IAbsoluteLayoutLayoutParams::Probe(layoutParams);
        lp->SetWidth(width);
        lp->SetHeight(height);
        lp->SetX(x);
        lp->SetY(y);
    }
    else {
        CAbsoluteLayoutLayoutParams::New(width, height, x, y, (IAbsoluteLayoutLayoutParams**)&lp);
    }

    // apply the layout to the view
    v->mView->SetLayoutParams(lp);

    if (ISurfaceView::Probe(v->mView)) {
        const AutoPtr<ISurfaceView> sView = ISurfaceView::Probe(v->mView);

        Boolean result;
        if ((sView->IsFixedSize(&result), result) && mZoomInProgress) {
            /* If we're already fixed, and we're in a zoom, then do nothing
               about the size. Just wait until we get called at the end of
               the zoom session (with mZoomInProgress false) and we'll
               fixup our size then.
             */
            return;
        }

        /* Compute proportional fixed width/height if necessary.
         *
         * NOTE: plugins (e.g. Flash) must not explicitly fix the size of
         * their surface. The logic below will result in unexpected behavior
         * for the plugin if they attempt to fix the size of the surface.
         */
        Int32 fixedW = width;
        Int32 fixedH = height;
        if (fixedW > MAX_SURFACE_DIMENSION || fixedH > MAX_SURFACE_DIMENSION) {
            if (v->mWidth > v->mHeight) {
                fixedW = MAX_SURFACE_DIMENSION;
                fixedH = v->mHeight * MAX_SURFACE_DIMENSION / v->mWidth;
            }
            else {
                fixedH = MAX_SURFACE_DIMENSION;
                fixedW = v->mWidth * MAX_SURFACE_DIMENSION / v->mHeight;
            }
        }
        if (fixedW * fixedH > MAX_SURFACE_AREA) {
            Float area = MAX_SURFACE_AREA;
            if (v->mWidth > v->mHeight) {
                fixedW = (Int32)Elastos::Core::Math::Sqrt(area * v->mWidth / v->mHeight);
                fixedH = v->mHeight * fixedW / v->mWidth;
            }
            else {
                fixedH = (Int32)Elastos::Core::Math::Sqrt(area * v->mHeight / v->mWidth);
                fixedW = v->mWidth * fixedH / v->mHeight;
            }
        }

        if (fixedW != width || fixedH != height) {
            // if we get here, either our dimensions or area (or both)
            // exeeded our max, so we had to compute fixedW and fixedH
            AutoPtr<ISurfaceHolder> holder;
            sView->GetHolder((ISurfaceHolder**)&holder);
            holder->SetFixedSize(fixedW, fixedH);
        }
        else if ((sView->IsFixedSize(&result), !result) && mZoomInProgress) {
            // just freeze where we were (view size) until we're done with
            // the zoom progress
            Int32 width, height;
            sView->GetWidth(&width);
            sView->GetHeight(&height);
            AutoPtr<ISurfaceHolder> holder;
            sView->GetHolder((ISurfaceHolder**)&holder);
            holder->SetFixedSize(width, height);
        }
        else if ((sView->IsFixedSize(&result), result) && !mZoomInProgress) {
            /* The changing of visibility is a hack to get around a bug in
             * the framework that causes the surface to revert to the size
             * it was prior to being fixed before it redraws using the
             * values currently in its layout.
             *
             * The surface is destroyed when it is set to invisible and then
             * recreated at the new dimensions when it is made visible. The
             * same destroy/create step occurs without the change in
             * visibility, but then exhibits the behavior described in the
             * previous paragraph.
             */
            Int32 visibility;
            if (sView->GetVisibility(&visibility), visibility == IView::VISIBLE) {
                sView->SetVisibility(IView::INVISIBLE);
                AutoPtr<ISurfaceHolder> holder;
                sView->GetHolder((ISurfaceHolder**)&holder);
                holder->SetSizeFromLayout();
                // setLayoutParams() only requests the layout. If we set it
                // to VISIBLE now, it will use the old dimension to set the
                // size. Post a message to ensure that it shows the new size.
                AutoPtr<IRunnable> r = new RequestLayoutRunnable(sView);
                Boolean result;
                mWebView->mPrivateHandler->Post(r, &result);
            }
            else {
                AutoPtr<ISurfaceHolder> holder;
                sView->GetHolder((ISurfaceHolder**)&holder);
                holder->SetSizeFromLayout();
            }
        }
    }
}

void ViewManager::StartZoom()
{
    mZoomInProgress = TRUE;
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        RequestLayout(*it);
    }
}

void ViewManager::EndZoom()
{
    mZoomInProgress = FALSE;
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        RequestLayout(*it);
    }
}

void ViewManager::ScaleAll()
{
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        RequestLayout(*it);
    }
}

void ViewManager::HideAll()
{
    if (mHidden) {
        return;
    }
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        (*it)->mView->SetVisibility(IView::GONE);
    }
    mHidden = TRUE;
}

void ViewManager::ShowAll()
{
    if (!mHidden) {
        return;
    }
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        (*it)->mView->SetVisibility(IView::VISIBLE);
    }
    mHidden = FALSE;
}

void ViewManager::PostResetStateAll()
{
    AutoPtr<IRunnable> runnable = new PostResetStateAllRunnable(this);
    Boolean result;
    mWebView->mPrivateHandler->Post(runnable, &result);
}

void ViewManager::PostReadyToDrawAll()
{
    AutoPtr<IRunnable> runnable = new PostReadyToDrawAllRunnable(this);
    Boolean result;
    mWebView->mPrivateHandler->Post(runnable, &result);
}

AutoPtr<ViewManager::ChildView> ViewManager::HitTest(
    /* [in] */ Int32 contentX,
    /* [in] */ Int32 contentY)
{
    if (mHidden) {
        return NULL;
    }
    List< AutoPtr<ChildView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        ChildView* v = *it;
        Int32 visibility;
        v->mView->GetVisibility(&visibility);
        if (visibility == IView::VISIBLE) {
            if (contentX >= v->mX && contentX < (v->mX + v->mWidth)
                    && contentY >= v->mY && contentY < (v->mY + v->mHeight)) {
                return v;
            }
        }
    }
    return NULL;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
