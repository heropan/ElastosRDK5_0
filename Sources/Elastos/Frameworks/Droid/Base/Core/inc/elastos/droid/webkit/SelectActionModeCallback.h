
#ifndef __ELASTOS_DROID_WEBKIT_SELECTACTIONMODECALLBACK_H__
#define __ELASTOS_DROID_WEBKIT_SELECTACTIONMODECALLBACK_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IActionModeCallback;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

class SelectActionModeCallback
    : public ElRefBase
    , public IActionModeCallback
{
public:
    SelectActionModeCallback();

    CAR_INTERFACE_DECL();

    virtual CARAPI_(void) SetWebView(
        /* [in] */ CWebViewClassic* webView);

    virtual CARAPI_(void) SetTextSelected(
        /* [in] */ Boolean isTextSelected);

    virtual CARAPI_(void) Finish();

    // ActionMode.Callback implementation

    CARAPI OnCreateActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnPrepareActionMode(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* result);

    CARAPI OnActionItemClicked(
        /* [in] */ IActionMode* mode,
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result);

    CARAPI OnDestroyActionMode(
        /* [in] */ IActionMode* mode);

private:
    CARAPI_(void) SetMenuVisibility(
        /* [in] */ IMenu* menu,
        /* [in] */ Boolean visible,
        /* [in] */ Int32 resourceId);

private:
    AutoPtr<CWebViewClassic> mWebView;
    AutoPtr<IActionMode> mActionMode;
    Boolean mIsTextSelected;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_SELECTACTIONMODECALLBACK_H__
