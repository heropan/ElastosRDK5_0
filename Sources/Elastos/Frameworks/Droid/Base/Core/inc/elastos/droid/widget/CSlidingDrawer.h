#ifndef __ELASTOS_DROID_WIDGET_CSLIDINGDRAWER_H__
#define __ELASTOS_DROID_WIDGET_CSLIDINGDRAWER_H__

#include "_Elastos_Droid_Widget_CSlidingDrawer.h"

#include "elastos/droid/widget/SlidingDrawer.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSlidingDrawer) , public SlidingDrawer
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CSlidingDrawer();

    CARAPI constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    CARAPI Toggle();

    CARAPI AnimateToggle();

    CARAPI Open();

    CARAPI Close();

    CARAPI AnimateClose();

    CARAPI AnimateOpen();

    CARAPI SetOnDrawerOpenListener(
        /* [in] */ IOnDrawerOpenListener* onDrawerOpenListener);

    CARAPI SetOnDrawerCloseListener(
        /* [in] */ IOnDrawerCloseListener* onDrawerCloseListener);

    CARAPI SetOnDrawerScrollListener(
        /* [in] */ IOnDrawerScrollListener* onDrawerScrollListener);

    CARAPI GetHandle(
        /* [out] */ IView** handle);

    CARAPI GetContent(
        /* [out] */ IView** content);

    CARAPI Unlock();

    CARAPI Lock();

    CARAPI IsOpened(
        /* [out] */ Boolean* opened);

    CARAPI IsMoving(
        /* [out] */ Boolean* moving);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos
#endif
