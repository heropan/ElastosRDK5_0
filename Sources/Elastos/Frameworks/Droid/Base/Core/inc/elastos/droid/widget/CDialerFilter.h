#ifndef __ELASTOS_DROID_WIDGET_CDIALERFILTER_H__
#define __ELASTOS_DROID_WIDGET_CDIALERFILTER_H__

#include "_Elastos_Droid_Widget_CDialerFilter.h"
#include "widget/DialerFilter.h"




using Elastos::Core::ICharSequence;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CDialerFilter), public DialerFilter
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
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI IsQwertyKeyboard(
        /* [out] */ Boolean* res);

    CARAPI GetMode(
        /* [out] */ Int32* mode);

    CARAPI SetMode(
        /* [in] */ Int32 newMode);

    CARAPI GetLetters(
        /* [out] */ ICharSequence** letters);

    CARAPI GetDigits(
        /* [out] */ ICharSequence** digits);

    CARAPI GetFilterText(
        /* [out] */ ICharSequence** filterText);

    CARAPI Append(
        /* [in] */ const String& text);

    CARAPI ClearText();

    CARAPI SetLettersWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI SetDigitsWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI SetFilterWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI RemoveFilterWatcher(
        /* [in] */  ITextWatcher* watcher);

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

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CDIALERFILTER_H__
