#ifndef __ELASTOS_DROID_WIDGET_TWOLINELISTITEM_H__
#define __ELASTOS_DROID_WIDGET_TWOLINELISTITEM_H__

#include "ext/frameworkext.h"
#include "widget/RelativeLayout.h"
#include <R.h>

namespace Elastos {
namespace Droid {
namespace Widget {

class TwoLineListItem : public RelativeLayout
{
public:
    TwoLineListItem(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    virtual CARAPI_(AutoPtr<ITextView>) GetText1();

    virtual CARAPI_(AutoPtr<ITextView>) GetText2();

    virtual CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);


protected:
    TwoLineListItem();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0);

    CARAPI OnFinishInflate();

private:
    AutoPtr<ITextView> mText1;
    AutoPtr<ITextView> mText2;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_TWOLINELISTITEM_H__


