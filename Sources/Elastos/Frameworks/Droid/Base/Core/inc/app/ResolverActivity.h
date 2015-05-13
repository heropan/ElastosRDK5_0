
#ifndef __RESOLVERACTIVITY_H__
#define __RESOLVERACTIVITY_H__

#include "app/AlertActivity.h"
#include "content/PackageMonitor.h"
#include "content/ContextMacro.h"
#include "widget/BaseAdapter.h"
#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Internal::App::IResolverActivity;
using Elastos::Droid::Internal::Content::PackageMonitor;
using Elastos::Droid::Internal::Content::EIID_IPackageMonitor;
using Elastos::Droid::Widget::IGridView;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemLongClickListener;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IResolveInfo;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class ResolverActivity
    : public AlertActivity
    , public IResolverActivity
{
public:
    friend class ResolveListAdapter;

    class ItemLongClickListener
        : public ElRefBase
        , public IAdapterViewOnItemLongClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        ItemLongClickListener(
            /* [in] */ ResolverActivity* host)
            : mHost(host)
        {}

        CARAPI OnItemLongClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id,
            /* [out] */ Boolean* clicked);

    private:
        ResolverActivity* mHost;
    };

private:
    class DisplayResolveInfo : public ElRefBase
    {
    public:
        DisplayResolveInfo(
            /* [in] */ IResolveInfo* pri,
            /* [in] */ ICharSequence* pLabel,
            /* [in] */ ICharSequence* pInfo,
            /* [in] */ IIntent* pOrigIntent)
            : mRi(pri)
            , mDisplayLabel(pLabel)
            , mDisplayIcon(NULL)
            , mExtendedInfo(pInfo)
            , mOrigIntent(pOrigIntent)
        {}

    public:
        AutoPtr<IResolveInfo> mRi;
        AutoPtr<ICharSequence> mDisplayLabel;
        AutoPtr<IDrawable> mDisplayIcon;
        AutoPtr<ICharSequence> mExtendedInfo;
        AutoPtr<IIntent> mOrigIntent;
    };

    class ResolveListAdapter
//        : public BaseAdapter
    {
    public:
        friend class ResolverActivity;

        ResolveListAdapter(
            /* [in] */ ResolverActivity* host,
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent,
            /* [in] */ ArrayOf<IIntent*>* initialIntents,
            /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
            /* [in] */ Int32 launchedFromUid);

        virtual ~ResolveListAdapter();

        CARAPI HandlePackagesChanged();

        CARAPI ResolveInfoForPosition(
            /* [in] */ Int32 position,
            /* [out] */ IResolveInfo** resInfo);

        CARAPI IntentForPosition(
            /* [in] */ Int32 position,
            /* [out] */ IIntent** bkIntent);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* id);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** bkView);

    private:
        CARAPI_(void) RebuildList();

        CARAPI_(void) ProcessGroup(
            /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ IResolveInfo* ro,
            /* [in] */ ICharSequence* roLabel);

        CARAPI_(void) BindView(
            /* [in] */ IView* view,
            /* [in] */ DisplayResolveInfo* info);

    private:
        ResolverActivity* mHost;
        AutoPtr<ArrayOf<IIntent*> > mInitialIntents;
        AutoPtr< List<AutoPtr<IResolveInfo> > > mBaseResolveList;
        AutoPtr<IIntent> mIntent;
        Int32 mLaunchedFromUid;
        AutoPtr<ILayoutInflater> mInflater;

        AutoPtr< List<AutoPtr<IResolveInfo> > > mCurrentResolveList;
        List<AutoPtr<DisplayResolveInfo> > mList;
    };

    class _PackageMonitor
        : public PackageMonitor
    {
    public:
        _PackageMonitor(
            /* [in] */ ResolverActivity* host)
            : mHost(host)
        {}

        CARAPI OnSomePackagesChanged();

    private:
        ResolverActivity* mHost;
    };

public:
    ResolverActivity();

    virtual ~ResolverActivity()
    {}

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState,
        /* [in] */ IIntent* intent,
        /* [in] */ ICharSequence* title,
        /* [in] */ ArrayOf<IIntent*>* initialIntents,
        /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
        /* [in] */ Boolean alwaysUseOption);

    CARAPI ResizeGrid();

    CARAPI GetIcon(
        /* [in] */ IResources* res,
        /* [in] */ Int32 resId,
        /* [out] */ IDrawable** drawable);

    CARAPI LoadIconForResolveInfo(
        /* [in] */ IResolveInfo* ri,
        /* [out] */ IDrawable** drawable);

    CARAPI OnRestart();

    CARAPI OnStop();

    CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnButtonClick(
        /* [in] */ IView* v);

    CARAPI StartSelected(
        /* [in] */ Int32 which,
        /* [in] */ Boolean always);

    CARAPI OnIntentSelected(
        /* [in] */ IResolveInfo* ri,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean alwaysCheck);

    CARAPI ShowAppDetails(
        /* [in] */ IResolveInfo* ri);

private:
    CARAPI_(AutoPtr<IIntent>) MakeMyIntent();

private:
    static const String TAG;

    Int32 mLaunchedFromUid;
    ResolveListAdapter* mAdapter;
    AutoPtr<IPackageManager> mPm;
    Boolean mAlwaysUseOption;
    Boolean mShowExtended;
    AutoPtr<IGridView> mGrid;
    AutoPtr<IButton> mAlwaysButton;
    AutoPtr<IButton> mOnceButton;
    Int32 mIconDpi;
    Int32 mIconSize;
    Int32 mMaxColumns;
    Int32 mLastSelected;

    Boolean mRegistered;

    AutoPtr<_PackageMonitor> mPackageMonitor;
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__RESOLVERACTIVITY_H__
