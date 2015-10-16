
#ifndef __ELASTOS_DROID_VIEW_CVIEWSTUB_H__
#define  __ELASTOS_DROID_VIEW_CVIEWSTUB_H__

#include "_Elastos_Droid_View_CViewStub.h"
#include "elastos/droid/view/ViewStub.h"


namespace Elastos {
namespace Droid {
namespace View {

CarClass(CViewStub), public ViewStub
{
public:
    IVIEW_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutResource);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInflatedId(
        /* [out] */ Int32* id);

    CARAPI SetInflatedId(
        /* [in] */ Int32 inflatedId);

    CARAPI GetLayoutResource(
        /* [out] */ Int32* resource);

    CARAPI SetLayoutResource(
        /* [in] */ Int32 layoutResource);

    CARAPI SetLayoutInflater(
        /* [in] */ ILayoutInflater* inflater);

    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater);

    CARAPI Inflate(
        /* [out] */ IView** retView);

    CARAPI SetOnInflateListener(
        /* [in] */ IOnInflateListener* inflateListener);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_CVIEWSTUB_H__
