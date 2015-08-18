
#ifndef __ELASTOS_DROID_INTERNAL_APP_CMEDIAROUTECHOOSERDIALOGFRAGMENT_H__
#define __ELASTOS_DROID_INTERNAL_APP_CMEDIAROUTECHOOSERDIALOGFRAGMENT_H__

#include "_Elastos_Droid_Internal_App_CMediaRouteChooserDialogFragment.h"

#include "widget/BaseAdapter.h"

#include "app/DialogFragment.h"

#include "app/Dialog.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::IComparator;
using Elastos::Core::EIID_IComparator;
using Elastos::Droid::App::Dialog;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::DialogFragment;
using Elastos::Droid::App::ILoaderManager;
using Elastos::Droid::App::ILoaderManagerImpl;
using Elastos::Droid::App::IFragmentManagerImpl;
using Elastos::Droid::App::IFragmentSavedState;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::Media::IRouteInfo;
using Elastos::Droid::Media::IRouteGroup;
using Elastos::Droid::Media::IMediaRouter;
using Elastos::Droid::Media::IRouteCategory;
using Elastos::Droid::Widget::ISeekBar;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::ICheckBox;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::IImageButton;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::ISeekBarOnSeekBarChangeListener;
using Elastos::Droid::Widget::EIID_ISeekBarOnSeekBarChangeListener;
using Elastos::Droid::Hardware::Display::IDisplayManager;


namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class MediaRouteChooserDialogFragment
    : public DialogFragment
{
friend class ConvertViewOnClickListener;
friend class MediaRouterCallback;
friend class RouteChooserDialog;

class ViewHolder;

private:
    class RouteAdapter
        : public ElRefBase
        , public BaseAdapter
        , public IBaseAdapter
        , public IAdapterViewOnItemClickListener
    {
    friend class MediaRouterCallback;
    friend class MediaRouteChooserDialogFragment;

    public:
        class ExpandGroupListener
            : public ElRefBase
            , public IViewOnClickListener
        {
        public:
            CAR_INTERFACE_DECL()

            ExpandGroupListener(
                /* [in] */ RouteAdapter* aHost)
                : mPosition(0)
                , mAHost(aHost)
            {}

            CARAPI OnClick(
                /* [in] */ IView* v);

        public:
            Int32 mPosition;
            AutoPtr<RouteAdapter> mAHost;
        };

    public:
        CAR_INTERFACE_DECL()
        IBASEADAPTER_METHODS_DECL()

        RouteAdapter(
            /* [in] */ MediaRouteChooserDialogFragment* host);

        ~RouteAdapter();

        CARAPI_(void) Update();

        CARAPI_(void) ScrollToEditingGroup();

        CARAPI_(void) ScrollToSelectedItem();

        CARAPI_(void) AddSelectableRoutes(
            /* [in] */ IRouteInfo* selectedRoute,
            /* [in] */ List<AutoPtr<IRouteInfo> >* from);

        CARAPI_(void) AddGroupEditingCategoryRoutes(
            /* [in] */ List<AutoPtr<IRouteInfo> >* from);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI_(Int32) GetCount();

        CARAPI GetViewTypeCount(
            /* [out] */ Int32* count);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32* type);

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* enabled);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI_(AutoPtr<IInterface>) GetItem(
            /* [in] */ Int32 position);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* id);

        CARAPI_(Int64) GetItemId(
            /* [in] */ Int32 position);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

        CARAPI_(void) BindItemView(
            /* [in] */ Int32 position,
            /* [in] */ ViewHolder* holder);

        CARAPI_(void) BindHeaderView(
            /* [in] */ Int32 position,
            /* [in] */ ViewHolder* holder);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI_(Boolean) IsGrouping();

        CARAPI_(void) FinishGrouping();

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI HasStableIds(
            /* [out] */ Boolean* hasStableIds);

        CARAPI IsEmpty(
            /* [out] */ Boolean* isEmpty);


    private:
        static const Int32 VIEW_TOP_HEADER = 0;
        static const Int32 VIEW_SECTION_HEADER = 1;
        static const Int32 VIEW_ROUTE = 2;
        static const Int32 VIEW_GROUPING_ROUTE = 3;
        static const Int32 VIEW_GROUPING_DONE = 4;

        Int32 mSelectedItemPosition;
        List<AutoPtr<IInterface> > mItems;

        AutoPtr<IRouteCategory> mCategoryEditingGroups;
        AutoPtr<IRouteGroup> mEditingGroup;

        // Temporary lists for manipulation
        List<AutoPtr<IRouteInfo> > mCatRouteList;
        List<AutoPtr<IRouteInfo> > mSortRouteList;

        Boolean mIgnoreUpdates;

        MediaRouteChooserDialogFragment* mHost;
    };

    class ViewHolder
        : public IInterface
        , public ElRefBase
    {
    public:
        ViewHolder()
            : mText1(NULL)
            , mText2(NULL)
            , mIcon(NULL)
            , mExpandGroupButton(NULL)
            , mExpandGroupListener(NULL)
            , mPosition(0)
            , mCheck(NULL)
        {}

        CAR_INTERFACE_DECL()

    public:
        AutoPtr<ITextView> mText1;
        AutoPtr<ITextView> mText2;
        AutoPtr<IImageView> mIcon;
        AutoPtr<IImageButton> mExpandGroupButton;
        AutoPtr<RouteAdapter::ExpandGroupListener> mExpandGroupListener;
        Int32 mPosition;
        AutoPtr<ICheckBox> mCheck;
    };

public:
    class MediaRouterCallback
        : public ElRefBase
        //extends MediaRouter.Callback
    {
    public:
        CARAPI_(UInt32) AddRef();
        CARAPI_(UInt32) Release();

        MediaRouterCallback(
            /* [in] */ MediaRouteChooserDialogFragment* host)
            : mHost(host)
        {}

        CARAPI OnRouteSelected(
            /* [in] */ IMediaRouter* router,
            /* [in] */ Int32 type,
            /* [in] */ IRouteInfo* info);

        CARAPI OnRouteUnselected(
            /* [in] */ IMediaRouter* router,
            /* [in] */ Int32 type,
            /* [in] */ IRouteInfo* info);

        CARAPI OnRouteAdded(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info);

        CARAPI OnRouteRemoved(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info);

        CARAPI OnRouteChanged(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info);

        CARAPI OnRouteGrouped(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info,
            /* [in] */ IRouteGroup* group,
            /* [in] */ Int32 index);

        CARAPI OnRouteUngrouped(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info,
            /* [in] */ IRouteGroup* group);

        CARAPI OnRouteVolumeChanged(
            /* [in] */ IMediaRouter* router,
            /* [in] */ IRouteInfo* info);

    private:
        MediaRouteChooserDialogFragment* mHost;
    };

    class RouteComparator
        : public ElRefBase
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        RouteComparator(
            /* [in] */ MediaRouteChooserDialogFragment* host)
            : mHost(host)
        {}

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);

    private:
        MediaRouteChooserDialogFragment* mHost;
    };

    class _RouteChooserDialog
        : public Dialog
    {
    public:
        _RouteChooserDialog(
            /* [in] */ IContext* context,
            /* [in] */ Int32 theme,
            /* [in] */ MediaRouteChooserDialogFragment* host);

        CARAPI OnBackPressed();

        CARAPI OnKeyDown(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* keydown);

        CARAPI OnKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* keyup);

    private:
        MediaRouteChooserDialogFragment* mHost;
    };

    class RouteChooserDialog
        : public ElRefBase
        , _RouteChooserDialog
        , public IDialog
    {
    public:
        CAR_INTERFACE_DECL()
        IDIALOG_METHODS_DECL()

        RouteChooserDialog(
            /* [in] */ IContext* context,
            /* [in] */ Int32 theme,
            /* [in] */ MediaRouteChooserDialogFragment* host)
            : _RouteChooserDialog(context, theme, host)
        {}
    };

    class VolumeSliderChangeListener
        : public ElRefBase
        , public ISeekBarOnSeekBarChangeListener
    {
    public:
        CAR_INTERFACE_DECL()

        VolumeSliderChangeListener(
            /* [in] */ MediaRouteChooserDialogFragment* host)
            : mHost(host)
        {}

        CARAPI OnProgressChanged(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

        CARAPI OnStartTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

        CARAPI OnStopTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

    private:
        MediaRouteChooserDialogFragment* mHost;
    };

public:
    MediaRouteChooserDialogFragment()
        : mRouter(NULL)
        , mDisplayService(NULL)
        , mComparator(new RouteComparator(this))
        , mCallback(new MediaRouterCallback(this))
        , mRouteTypes(0)
        , mInflater(NULL)
        , mLauncherListener(NULL)
        , mExtendedSettingsListener(NULL)
        , mAdapter(NULL)
        , mListView(NULL)
        , mVolumeSlider(NULL)
        , mVolumeIcon(NULL)
        , mIgnoreSliderVolumeChanges(FALSE)
        , mIgnoreCallbackVolumeChanges(FALSE)
    {}

    static CARAPI Init();

    CARAPI SetLauncherListener(
        /* [in] */ ILauncherListener* listener);

    CARAPI OnAttach(
        /* [in] */ IActivity* activity);

    CARAPI OnDetach();

    CARAPI SetExtendedSettingsClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI SetRouteTypes(
        /* [in] */ Int32 types);

    CARAPI_(void) UpdateVolume();

    CARAPI_(void) ChangeVolume(
        /* [in] */ Int32 newValue);

    CARAPI OnCreateView(
        /* [in] */ ILayoutInflater* inflater,
        /* [in] */ IViewGroup* container,
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IView** view);

    CARAPI OnCreateDialog(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IDialog** dialog);

    CARAPI OnResume();

public:
    AutoPtr<IMediaRouter> mRouter;
    AutoPtr<IDisplayManager> mDisplayService;

    AutoPtr<RouteComparator> mComparator;
    AutoPtr<MediaRouterCallback> mCallback;

private:
    static const String TAG;
    static AutoPtr<ArrayOf<Int32> > ITEM_LAYOUTS;
    Int32 mRouteTypes;

    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<ILauncherListener> mLauncherListener;
    AutoPtr<IViewOnClickListener> mExtendedSettingsListener;
    AutoPtr<RouteAdapter> mAdapter;
    AutoPtr<IListView> mListView;
    AutoPtr<ISeekBar> mVolumeSlider;
    AutoPtr<IImageView> mVolumeIcon;
    Boolean mIgnoreSliderVolumeChanges;
    Boolean mIgnoreCallbackVolumeChanges;
};

CarClass(CMediaRouteChooserDialogFragment) , public MediaRouteChooserDialogFragment
{
public:
    CMediaRouteChooserDialogFragment()
        : MediaRouteChooserDialogFragment()
    {}

    IFRAGMENT_METHODS_DECL();
    IDIALOGFRAGMENT_METHODS_DECL();

    CARAPI constructor();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetLauncherListener(
        /* [in] */ ILauncherListener* listener);

    CARAPI SetExtendedSettingsClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI SetRouteTypes(
        /* [in] */ Int32 types);

};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CMEDIAROUTECHOOSERDIALOGFRAGMENT_H__
