#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CGallery.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CGallery, Gallery)
IVIEWGROUP_METHODS_IMPL(CGallery, Gallery)
IVIEWPARENT_METHODS_IMPL(CGallery, Gallery)
IVIEWMANAGER_METHODS_IMPL(CGallery, Gallery)
IDRAWABLECALLBACK_METHODS_IMPL(CGallery, Gallery)
IKEYEVENTCALLBACK_METHODS_IMPL(CGallery, Gallery)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CGallery, Gallery)
IADAPTERVIEW_METHODS_IMPL(CGallery, Gallery)

PInterface CGallery::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CGallery::Probe(riid);
}

ECode CGallery::constructor(
    /* [in] */ IContext* context)
{
    return Gallery::Init(context);
}

ECode CGallery::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Gallery::Init(context, attrs);
}

ECode CGallery::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Gallery::Init(context, attrs, defStyle);
}

ECode CGallery::SetCallbackDuringFling(
    /* [in] */ Boolean shouldCallback)
{
    return Gallery::SetCallbackDuringFling(shouldCallback);
}

ECode CGallery::SetCallbackOnUnselectedItemClick(
    /* [in] */ Boolean shouldCallback)
{
    return Gallery::SetCallbackOnUnselectedItemClick(shouldCallback);
}

ECode CGallery::SetAnimationDuration(
    /* [in] */ Int32 animationDurationMillis)
{
    return Gallery::SetAnimationDuration(animationDurationMillis);
}

ECode CGallery::SetSpacing(
    /* [in] */ Int32 spacing)
{
    return Gallery::SetSpacing(spacing);
}

ECode CGallery::SetUnselectedAlpha(
    /* [in] */ Float unselectedAlpha)
{
    return Gallery::SetUnselectedAlpha(unselectedAlpha);
}

ECode CGallery::SetGravity(
    /* [in] */ Int32 gravity)
{
    return Gallery::SetGravity(gravity);
}

CARAPI CGallery::SetSelection(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    return Gallery::SetSelection(position, animate);
}

CARAPI CGallery::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = Gallery::PointToPosition(x, y);
    return NOERROR;
}

// Interface IOnGestureListener

ECode CGallery::OnDown(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnDown(e);
    return NOERROR;
}
ECode CGallery::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
    return Gallery::OnShowPress(e);
}

ECode CGallery::OnSingleTapUp(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnDown(e);
    return NOERROR;
}

ECode CGallery::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnScroll(e1, e2, distanceX, distanceY);
    return NOERROR;
}

ECode CGallery::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    return Gallery::OnLongPress(e);
}

ECode CGallery::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Gallery::OnFling(e1, e2, velocityX, velocityY);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
