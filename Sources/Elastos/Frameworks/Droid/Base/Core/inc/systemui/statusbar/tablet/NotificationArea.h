#ifndef __NOTIFICATIONAREA_H_
#define __NOTIFICATIONAREA_H_

#include "widget/LinearLayout.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Widget::LinearLayout;


namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {


class NotificationArea : public LinearLayout
{
public:
    NotificationArea();

    NotificationArea(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(Boolean) OnRequestSendAccessibilityEvent(
        /* [in] */ IView* child,
        /* [in] */ IAccessibilityEvent* event);
};


}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__NOTIFICATIONAREA_H_
