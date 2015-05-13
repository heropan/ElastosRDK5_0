#ifndef __PANELBACKGROUNDVIEW_H_
#define __PANELBACKGROUNDVIEW_H_

#include "view/View.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

class PanelBackgroundView : public Elastos::Droid::View::View
{
public:
    PanelBackgroundView();

    PanelBackgroundView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__PANELBACKGROUNDVIEW_H_
