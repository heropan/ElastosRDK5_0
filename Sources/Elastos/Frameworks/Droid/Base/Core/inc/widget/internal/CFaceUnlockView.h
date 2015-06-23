#ifndef __CFACEUNLOCKVIEW_H__
#define __CFACEUNLOCKVIEW_H__

#include "_CFaceUnlockView.h"
#include "internal/FaceUnlockView.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CFaceUnlockView), public FaceUnlockView
{
public:

    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetIgnoreGravity(
        /* [in] */ Int32 viewId);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
