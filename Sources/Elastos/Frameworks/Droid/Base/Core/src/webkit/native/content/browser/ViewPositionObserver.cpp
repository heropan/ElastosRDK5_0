// wuweizuo automatic build .cpp file from .java file.

#include "ViewPositionObserver.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//     ViewPositionObserver::InnerViewTreeObserverOnPreDrawListener
//=====================================================================
ViewPositionObserver::InnerViewTreeObserverOnPreDrawListener::InnerViewTreeObserverOnPreDrawListener(
    /* [in] */ ViewPositionObserver* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

Boolean ViewPositionObserver::InnerViewTreeObserverOnPreDrawListener::OnPreDraw()
{
    // ==================before translated======================
    // updatePosition();
    // return true;
    assert(0);
    return FALSE;
}

//=====================================================================
//                         ViewPositionObserver
//=====================================================================
ViewPositionObserver::ViewPositionObserver(
    /* [in] */ IView* view)
{
    // ==================before translated======================
    // mView = view;
    // mListeners = new ArrayList<Listener>();
    // updatePosition();
    // mPreDrawListener = new ViewTreeObserver.OnPreDrawListener() {
    //     @Override
    //     public boolean onPreDraw() {
    //         updatePosition();
    //         return true;
    //     }
    // };
}

Int32 ViewPositionObserver::GetPositionX()
{
    // ==================before translated======================
    // // The stored position may be out-of-date. Get the real current position.
    // updatePosition();
    // return mPosition[0];
    assert(0);
    return 0;
}

Int32 ViewPositionObserver::GetPositionY()
{
    // ==================before translated======================
    // // The stored position may be out-of-date. Get the real current position.
    // updatePosition();
    // return mPosition[1];
    assert(0);
    return 0;
}

ECode ViewPositionObserver::AddListener(
    /* [in] */ Listener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // if (mListeners.contains(listener)) return;
    //
    // if (mListeners.isEmpty()) {
    //     mView.getViewTreeObserver().addOnPreDrawListener(mPreDrawListener);
    //     updatePosition();
    // }
    //
    // mListeners.add(listener);
    assert(0);
    return NOERROR;
}

ECode ViewPositionObserver::RemoveListener(
    /* [in] */ Listener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // if (!mListeners.contains(listener)) return;
    //
    // mListeners.remove(listener);
    //
    // if (mListeners.isEmpty()) {
    //     mView.getViewTreeObserver().removeOnPreDrawListener(mPreDrawListener);
    // }
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Int32> > ViewPositionObserver::MiddleInitMposition()
{
    // ==================before translated======================
    // int[] result = new int[2];
    assert(0);
    AutoPtr< ArrayOf<Int32> > empty;
    return empty;
}

ECode ViewPositionObserver::NotifyListeners()
{
    // ==================before translated======================
    // for (int i = 0; i < mListeners.size(); i++) {
    //     mListeners.get(i).onPositionChanged(mPosition[0], mPosition[1]);
    // }
    assert(0);
    return NOERROR;
}

ECode ViewPositionObserver::UpdatePosition()
{
    // ==================before translated======================
    // int previousPositionX = mPosition[0];
    // int previousPositionY = mPosition[1];
    // mView.getLocationInWindow(mPosition);
    // if (mPosition[0] != previousPositionX || mPosition[1] != previousPositionY) {
    //     notifyListeners();
    // }
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


