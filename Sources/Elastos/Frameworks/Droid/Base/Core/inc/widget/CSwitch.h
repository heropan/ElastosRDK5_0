#ifndef __CSWITCH_H_
#define __CSWITCH_H_

#include "_CSwitch.h"
#include "widget/Switch.h"




namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CSwitch), public Switch
{
public:
    IVIEW_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ICOMPOUNDBUTTON_METHODS_DECL()
    ICHECKABLE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    CARAPI SetSwitchTextAppearance(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resid);

    CARAPI SetSwitchTypeface(
        /* [in] */ ITypeface* tf,
        /* [in] */ Int32 style);

    CARAPI SetSwitchTypeface(
        /* [in] */ ITypeface* tf);

    CARAPI SetSwitchPadding(
        /* [in] */ Int32 pixels);

    CARAPI GetSwitchPadding(
        /* [out] */ Int32* padding);

    CARAPI SetSwitchMinWidth(
        /* [in] */ Int32 pixels);

    CARAPI GetSwitchMinWidth(
        /* [out] */ Int32* width);

    CARAPI SetThumbTextPadding(
        /* [in] */ Int32 pixels);

    CARAPI GetThumbTextPadding(
        /* [out] */ Int32* padding);

    CARAPI SetTrackDrawable(
        /* [in] */ IDrawable* track);

    CARAPI SetTrackResource(
        /* [in] */ Int32 resId);

    CARAPI GetTrackDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI SetThumbDrawable(
        /* [in] */ IDrawable* thumb);

    CARAPI SetThumbResource(
        /* [in] */ Int32 resId);

    CARAPI GetThumbDrawable(
        /* [out] */ IDrawable** drawable);

    CARAPI GetTextOn(
        /* [out] */ ICharSequence** c);

    CARAPI SetTextOn(
        /* [in] */ ICharSequence* textOn);

    CARAPI GetTextOff(
        /* [out] */ ICharSequence** c);

    CARAPI SetTextOff(
        /* [in] */ ICharSequence* textOff);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    CARAPI GetButtonDrawable(
        /* [out] */ IDrawable** d);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CSWITCH_H_
