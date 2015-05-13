
#ifndef __TICKERVIEW_H_
#define __TICKERVIEW_H_

#include "widget/TextSwitcher.h"

using Elastos::Droid::Widget::TextSwitcher;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Phone {

class TickerView : public TextSwitcher
{
public:
    TickerView();

    TickerView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    CARAPI SetTicker(
        /* [in] */ ITicker* t);

private:
    AutoPtr<ITicker> mTicker;
};

}// namespace Phone
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__TICKERVIEW_H_
