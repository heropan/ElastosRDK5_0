
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_FACEUNLOCKVIEW_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_FACEUNLOCKVIEW_H__

#include "elastos/droid/widget/RelativeLayout.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

class FaceUnlockView : public RelativeLayout
{

public:
    FaceUnlockView();

    FaceUnlockView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

protected:

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    CARAPI_(Int32) ResolveMeasured(
        /* [in] */ Int32 measureSpec,
        /* [in] */ Int32 desired);

    static const String FaceUnlockView_TAG;
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
