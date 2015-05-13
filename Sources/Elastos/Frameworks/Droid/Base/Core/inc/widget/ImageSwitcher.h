#ifndef __IMAGESWITCHER_H_
#define __IMAGESWITCHER_H_

#include "widget/ViewSwitcher.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

namespace Elastos {
namespace Droid {
namespace Widget {

class ImageSwitcher : public ViewSwitcher
{
public:
    ImageSwitcher(
        /* [in] */ IContext* context);

    ImageSwitcher(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetImageResource(
        /* [in] */ Int32 resid);

    CARAPI SetImageURI(
        /* [in] */ IUri* uri);

    CARAPI SetImageDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

protected:
    ImageSwitcher();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

private:
    const static String IMAGESWITCHER_NAME;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__IMAGESWITCHER_H_