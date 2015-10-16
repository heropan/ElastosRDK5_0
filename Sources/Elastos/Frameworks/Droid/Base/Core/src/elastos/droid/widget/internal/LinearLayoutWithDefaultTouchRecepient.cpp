
#include "elastos/droid/internal/LinearLayoutWithDefaultTouchRecepient.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

LinearLayoutWithDefaultTouchRecepient::LinearLayoutWithDefaultTouchRecepient()
{
    CRect::NewByFriend((CRect**)&mTempRect);
}

LinearLayoutWithDefaultTouchRecepient::LinearLayoutWithDefaultTouchRecepient(
    /* [in] */ IContext* context) : LinearLayout(context)
{
    CRect::NewByFriend((CRect**)&mTempRect);
}

LinearLayoutWithDefaultTouchRecepient::LinearLayoutWithDefaultTouchRecepient(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : LinearLayout(context, attrs)
{
    CRect::NewByFriend((CRect**)&mTempRect);
}

ECode LinearLayoutWithDefaultTouchRecepient::SetDefaultTouchRecepient(
    /* [in] */ IView* defaultTouchRecepient)
{
    mDefaultTouchRecepient = defaultTouchRecepient;
    return NOERROR;
}

Boolean LinearLayoutWithDefaultTouchRecepient::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    if (mDefaultTouchRecepient == NULL) {
        return LinearLayout::DispatchTouchEvent(ev);
    }

    if (LinearLayout::DispatchTouchEvent(ev)) {
        return TRUE;
    }
    mTempRect->Set(0, 0, 0, 0);
    OffsetRectIntoDescendantCoords(mDefaultTouchRecepient, mTempRect);

    Float x, y;
    ev->GetX(&x);
    ev->GetY(&y);

    ev->SetLocation(x + mTempRect->mLeft, y + mTempRect->mTop);
    Boolean result;
    mDefaultTouchRecepient->DispatchTouchEvent(ev, &result);
    return result;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
