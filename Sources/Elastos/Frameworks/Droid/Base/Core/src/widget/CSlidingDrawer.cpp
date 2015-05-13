#include "CSlidingDrawer.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IVIEWGROUP_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IVIEWPARENT_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IVIEWMANAGER_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IDRAWABLECALLBACK_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IKEYEVENTCALLBACK_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSlidingDrawer, SlidingDrawer)

CSlidingDrawer::CSlidingDrawer()
{}

ECode CSlidingDrawer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SlidingDrawer::Init(context, attrs);
}

ECode CSlidingDrawer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SlidingDrawer::Init(context, attrs, defStyle);
}

PInterface CSlidingDrawer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CSlidingDrawer::Probe(riid);
}

ECode CSlidingDrawer::Toggle()
{
    return SlidingDrawer::Toggle();
}

ECode CSlidingDrawer::AnimateToggle()
{
    return SlidingDrawer::AnimateToggle();
}

ECode CSlidingDrawer::Open()
{
    return SlidingDrawer::Open();
}

ECode CSlidingDrawer::Close()
{
    return SlidingDrawer::Close();
}

ECode CSlidingDrawer::AnimateClose()
{
    return SlidingDrawer::AnimateClose();
}

ECode CSlidingDrawer::AnimateOpen()
{
    return SlidingDrawer::AnimateOpen();
}

ECode CSlidingDrawer::SetOnDrawerOpenListener(
    /* [in] */ IOnDrawerOpenListener* onDrawerOpenListener)
{
    return SlidingDrawer::SetOnDrawerOpenListener(onDrawerOpenListener);
}

ECode CSlidingDrawer::SetOnDrawerCloseListener(
    /* [in] */ IOnDrawerCloseListener* onDrawerCloseListener)
{
    return SlidingDrawer::SetOnDrawerCloseListener(onDrawerCloseListener);
}

ECode CSlidingDrawer::SetOnDrawerScrollListener(
    /* [in] */ IOnDrawerScrollListener* onDrawerScrollListener)
{
    return SlidingDrawer::SetOnDrawerScrollListener(onDrawerScrollListener);
}

ECode CSlidingDrawer::GetHandle(
    /* [out] */ IView** handle)
{
    AutoPtr<IView> temp = SlidingDrawer::GetHandle();
    *handle = temp;
    INTERFACE_ADDREF(*handle);
    return NOERROR;
}

ECode CSlidingDrawer::GetContent(
    /* [out] */ IView** content)
{
    AutoPtr<IView> temp = SlidingDrawer::GetContent();
    *content = temp;
    INTERFACE_ADDREF(*content);
    return NOERROR;
}

ECode CSlidingDrawer::Unlock()
{
    return SlidingDrawer::Unlock();
}

ECode CSlidingDrawer::Lock()
{
    return SlidingDrawer::Lock();
}

ECode CSlidingDrawer::IsOpened(
    /* [out] */ Boolean* opened)
{
    *opened = SlidingDrawer::IsOpened();
    return NOERROR;
}

ECode CSlidingDrawer::IsMoving(
    /* [out] */ Boolean* moving)
{
    *moving = SlidingDrawer::IsMoving();
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos