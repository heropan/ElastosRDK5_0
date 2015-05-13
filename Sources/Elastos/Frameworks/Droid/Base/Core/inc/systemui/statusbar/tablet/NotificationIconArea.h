#ifndef __NOTIFICATIONICONAREA_H_
#define __NOTIFICATIONICONAREA_H_

#include "widget/RelativeLayout.h"

using Elastos::Droid::Widget::RelativeLayout;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {


class NotificationIconArea
    : public RelativeLayout
{
public:
    NotificationIconArea();

    NotificationIconArea(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
private:
    AutoPtr<INotificationIconAreaIconLayout> mIconLayout;
};


}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__NOTIFICATIONICONAREA_H_
