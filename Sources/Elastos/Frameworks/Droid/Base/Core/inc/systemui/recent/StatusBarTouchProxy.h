
#ifndef __STATISBARTOUCHPROXY_H_
#define __STATISBARTOUCHPROXY_H_

#include "widget/FrameLayout.h"

using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Widget::FrameLayout;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

class StatusBarTouchProxy : public FrameLayout
{
public:
    StatusBarTouchProxy();

    CARAPI SetStatusBar(
        /* [in] */ IView* statusBar);

    CARAPI_(Boolean) OnTouchEvent (
        /* [in] */ IMotionEvent* event);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    AutoPtr<IView> mStatusBar;
};


}// namespace Recent
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__STATISBARTOUCHPROXY_H_
