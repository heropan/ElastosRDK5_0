// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mOwner = owner;
    */
}

Boolean ViewPositionObserver::InnerViewTreeObserverOnPreDrawListener::OnPreDraw()
{
    /*
    updatePosition();
    return true;
    */
    return FALSE;
}

//=====================================================================
//                         ViewPositionObserver
//=====================================================================
ViewPositionObserver::ViewPositionObserver(
    /* [in] */ IView* view)
{
    /*
    mView = view;
    mListeners = new ArrayList<Listener>();
    updatePosition();
    mPreDrawListener = // [wuweizuo auto add note here: new InnerViewTreeObserverOnPreDrawListener(this)] new ViewTreeObserver.OnPreDrawListener() {
        @Override
        public boolean onPreDraw() {
            updatePosition();
            return true;
        }
    };
    */
}

Int32 ViewPositionObserver::GetPositionX()
{
    /*
    // The stored position may be out-of-date. Get the real current position.
    updatePosition();
    return mPosition[0];
    */
    return 0;
}

Int32 ViewPositionObserver::GetPositionY()
{
    /*
    // The stored position may be out-of-date. Get the real current position.
    updatePosition();
    return mPosition[1];
    */
    return 0;
}

ECode ViewPositionObserver::AddListener(
    /* [in] */ Listener* listener)
{
    VALIDATE_NOT_NULL(listener);
    /*
    if (mListeners.contains(listener)) return;


    if (mListeners.isEmpty()) {
        mView.getViewTreeObserver().addOnPreDrawListener(mPreDrawListener);
        updatePosition();
    }


    mListeners.add(listener);
    */
    return NOERROR;
}

ECode ViewPositionObserver::RemoveListener(
    /* [in] */ Listener* listener)
{
    VALIDATE_NOT_NULL(listener);
    /*
    if (!mListeners.contains(listener)) return;


    mListeners.remove(listener);


    if (mListeners.isEmpty()) {
        mView.getViewTreeObserver().removeOnPreDrawListener(mPreDrawListener);
    }
    */
    return NOERROR;
}

ECode ViewPositionObserver::NotifyListeners()
{
    /*
    for (int i = 0; i < mListeners.size(); i++) {
        mListeners.get(i).onPositionChanged(mPosition[0], mPosition[1]);
    }
    */
    return NOERROR;
}

ECode ViewPositionObserver::UpdatePosition()
{
    /*
    int previousPositionX = mPosition[0];
    int previousPositionY = mPosition[1];
    mView.getLocationInWindow(mPosition);
    if (mPosition[0] != previousPositionX || mPosition[1] != previousPositionY) {
        notifyListeners();
    }
    */
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


