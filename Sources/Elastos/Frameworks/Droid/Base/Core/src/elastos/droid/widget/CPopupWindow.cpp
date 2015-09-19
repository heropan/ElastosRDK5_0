#include "widget/CPopupWindow.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IPOPUPWINDOW_METHODS_IMPL(CPopupWindow, PopupWindow)

ECode CPopupWindow::constructor()
{
    return Init();
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* context)
{
    return Init(context);
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs);
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Init(context, attrs, defStyle);
}

ECode CPopupWindow::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
{
    return Init(context, attrs, defStyle, defStyleRes);
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView)
{
    return Init(contentView);
}

ECode CPopupWindow::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Init(NULL, width, height);
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return Init(contentView, width, height);
}

ECode CPopupWindow::constructor(
    /* [in] */ IView* contentView,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean focusable)
{
    return Init(contentView, width, height, focusable);
}

//
//CARAPI_(PInterface) Probe(
//    /* [in] */ REIID riid)
//{
//    return NOERROR;
//}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

