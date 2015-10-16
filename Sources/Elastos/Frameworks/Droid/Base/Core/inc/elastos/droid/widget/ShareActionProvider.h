
#ifndef __ELASTOS_DROID_WIDGET_SHAREACTIONPROVIDER_H__
#define __ELASTOS_DROID_WIDGET_SHAREACTIONPROVIDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/ActionProvider.h"

using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ISubMenu;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IOnMenuItemClickListener;
using Elastos::Droid::Widget::IOnChooseActivityListener;
using Elastos::Droid::Widget::IActivityChooserModel;
using Elastos::Droid::Widget::IOnShareTargetSelectedListener;
using Elastos::Droid::View::ActionProvider;

namespace Elastos {
namespace Droid {
namespace Widget {

class ShareActionProvider : public ActionProvider
{
private:
    class ShareMenuItemOnMenuItemClickListener
        : public IOnMenuItemClickListener
        , public ElRefBase
    {
    public:
        ShareMenuItemOnMenuItemClickListener(
            /* [in] */ ShareActionProvider* host);

        CAR_INTERFACE_DECL()

        CARAPI OnMenuItemClick(
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* isConsumed);

    private:
        ShareActionProvider* mHost;
    };

    class ShareAcitivityChooserModelPolicy
        : public IOnChooseActivityListener
        , public ElRefBase
    {
    public:
        ShareAcitivityChooserModelPolicy(
            /* [in] */ ShareActionProvider* host);

        CAR_INTERFACE_DECL()

        CARAPI OnChooseActivity(
            /* [in] */  IActivityChooserModel* host,
            /* [in] */  IIntent* intent,
            /* [out] */  Boolean* res);

    private:
        ShareActionProvider* mHost;
    };

public:
    ShareActionProvider(
        /* [in] */ IContext* context);

    CARAPI SetOnShareTargetSelectedListener(
        /* [in] */ IOnShareTargetSelectedListener* listener);

    CARAPI_(AutoPtr<IView>) OnCreateActionView();

    using ActionProvider::OnCreateActionView;

    CARAPI_(Boolean) HasSubMenu();

    CARAPI OnPrepareSubMenu(
        /* [in] */ ISubMenu* subMenu);

    CARAPI SetShareHistoryFileName(
        /* [in] */ const String& shareHistoryFile);

    CARAPI SetShareIntent(
        /* [in] */ IIntent* shareIntent);

    CARAPI SetActivityChooserPolicyIfNeeded();

protected:
    ShareActionProvider();

    CARAPI Init(
        /* [in] */ IContext* context);

private:
    static const Int32 DEFAULT_INITIAL_ACTIVITY_COUNT = 4;
    static const String DEFAULT_SHARE_HISTORY_FILE_NAME;

    Int32 mMaxShownActivityCount;
    AutoPtr<ShareMenuItemOnMenuItemClickListener> mOnMenuItemClickListener;

    AutoPtr<IContext> mContext;
    String mShareHistoryFileName;
    AutoPtr<IOnShareTargetSelectedListener> mOnShareTargetSelectedListener;
    AutoPtr<IOnChooseActivityListener> mOnChooseActivityListener;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_SHAREACTIONPROVIDER_H__
