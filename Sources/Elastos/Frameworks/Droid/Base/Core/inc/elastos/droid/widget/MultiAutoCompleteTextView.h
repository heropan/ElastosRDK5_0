
#ifndef __ELASTOS_DROID_WIDGET_MULTIAUTOCOMPLETETEXTVIEW_H__
#define __ELASTOS_DROID_WIDGET_MULTIAUTOCOMPLETETEXTVIEW_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/AutoCompleteTextView.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;

namespace Elastos{
namespace Droid{
namespace Widget{

class MultiAutoCompleteTextView : public AutoCompleteTextView
{
public:
    class CommaTokenizer
        : public ITokenizer
        , public ElRefBase
    {
    public:
        CommaTokenizer();

        CARAPI FindTokenStart(
            /* [in] */ ICharSequence* text,
            /* [in] */ Int32 cursor,
            /* [out] */ Int32* start);

        CARAPI FindTokenEnd(
            /* [in] */ ICharSequence* text,
            /* [in] */ Int32 cursor,
            /* [out] */ Int32* end);

        CARAPI TerminateToken(
            /* [in] */ ICharSequence* text,
            /* [out] */ ICharSequence** res);
    };

public:
    MultiAutoCompleteTextView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::autoCompleteTextViewStyle);

    virtual CARAPI SetTokenizer(
        /* [in] */ ITokenizer* t);

    CARAPI_(Boolean) EnoughToFilter();

    CARAPI PerformValidation();

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    MultiAutoCompleteTextView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = R::attr::autoCompleteTextViewStyle);

    CARAPI_(void) PerformFiltering(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 keyCode);

    CARAPI_(void) PerformFiltering(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 keyCode);

    CARAPI_(void) ReplaceText(
        /* [in] */ ICharSequence* text);

private:
    CARAPI_(void) FinishInit();



private:
    AutoPtr<ITokenizer> mTokenizer;
    static const String MULTIAUTOCOMPLETETEXTVIEW_NAME;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
