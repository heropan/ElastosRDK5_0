#include "ext/frameworkext.h"
#include "app/CMediaRouteChooserDialogFragment.h"
#include "text/TextUtils.h"
#include <elastos/Math.h>

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::App::EIID_IDialog;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::ICheckable;
using Elastos::Droid::Widget::EIID_ICheckable;
using Elastos::Droid::Widget::EIID_IListView;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Media::EIID_IRouteInfo;
using Elastos::Droid::Media::EIID_IRouteCategory;
using Elastos::Droid::Graphics::Drawable::IDrawableConstantState;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class ConvertViewOnClickListener
    : public ElRefBase
    , public IViewOnClickListener
{
public:
    ConvertViewOnClickListener(
        /* [in] */ IListView* list,
        /* [in] */ IView* view,
        /* [in] */ MediaRouteChooserDialogFragment::ViewHolder* vh)
        : mList(list)
        , mView(view)
        , mViewHolder(vh)
    {}

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    ECode GetInterfaceID(
        /* [in] */ IInterface *object,
        /* [out] */ InterfaceID *iid)
    {
        VALIDATE_NOT_NULL(iid);
        if (object == (IInterface*)(IViewOnClickListener *)this) {
            *iid = EIID_IViewOnClickListener ;
        }
        else {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        return NOERROR;
    }

    PInterface Probe(
        /* [in]  */ REIID riid)
    {
        if ( riid == EIID_IInterface)
            return (IInterface*)(IViewOnClickListener*)this;
        else if ( riid == EIID_IViewOnClickListener )
            return (IViewOnClickListener *)this;
        return NULL;
    }

    ECode OnClick(
        /* [in] */ IView* v)
    {
        Boolean result;
        mList->PerformItemClick(mView, mViewHolder->mPosition, 0, &result);
        return NOERROR;
    }
private:
    AutoPtr<IListView> mList;
    AutoPtr<IView> mView;
    AutoPtr<MediaRouteChooserDialogFragment::ViewHolder> mViewHolder;
};


const String MediaRouteChooserDialogFragment::TAG = String("MediaRouteChooserDialogFragment");

AutoPtr<ArrayOf<Int32> > MediaRouteChooserDialogFragment::ITEM_LAYOUTS =
        ArrayOf<Int32>::Alloc(5);

ECode MediaRouteChooserDialogFragment::Init()
{
    MediaRouteChooserDialogFragment::
    ITEM_LAYOUTS->Set(0, 0/*R.layout.media_route_list_item_top_header*/);
    MediaRouteChooserDialogFragment::
    ITEM_LAYOUTS->Set(1, 0/*R.layout.media_route_list_item_section_header*/);
    MediaRouteChooserDialogFragment::
    ITEM_LAYOUTS->Set(2, 0/*R.layout.media_route_list_item*/);
    MediaRouteChooserDialogFragment::
    ITEM_LAYOUTS->Set(3, 0/*R.layout.media_route_list_item_checkable*/);
    MediaRouteChooserDialogFragment::
    ITEM_LAYOUTS->Set(4, 0/*R.layout.media_route_list_item_collapse_group*/);
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::SetLauncherListener(
    /* [in] */ ILauncherListener* listener)
{
    mLauncherListener = listener;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::OnAttach(
    /* [in] */ IActivity* activity)
{
    mRouter = NULL;
    mDisplayService = NULL;
    DialogFragment::OnAttach(activity);
    activity->GetSystemService(IContext::MEDIA_ROUTER_SERVICE,
            (IInterface**)(IMediaRouter**)&mRouter);
    activity->GetSystemService(IContext::DISPLAY_SERVICE,
            (IInterface**)(IDisplayManager**)&mDisplayService);
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::OnDetach()
{
    DialogFragment::OnDetach();
    if (mLauncherListener != NULL) {
        mLauncherListener->OnDetached((IMediaRouteChooserDialogFragment*)this->Probe(EIID_IMediaRouteChooserDialogFragment));
    }
    if (mAdapter != NULL) {
        mAdapter = NULL;
    }
    mInflater = NULL;
//    mRouter->RemoveCallback(mCallback);
    mRouter = NULL;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::SetExtendedSettingsClickListener(
        /* [in] */ IViewOnClickListener* listener)
{
    mExtendedSettingsListener = listener;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::SetRouteTypes(
        /* [in] */ Int32 types)
{
    mRouteTypes = types;
    if ((mRouteTypes & IMediaRouter::ROUTE_TYPE_LIVE_VIDEO) != 0 && mDisplayService == NULL) {
        AutoPtr<IActivity> activity;
        GetActivity((IActivity**)&activity);
        if (activity != NULL) {
            activity->GetSystemService(
                    IContext::DISPLAY_SERVICE,
                    (IInterface**)(IDisplayManager**)&mDisplayService);
        }
    } else {
        mDisplayService = NULL;
    }
    return NOERROR;
}

void MediaRouteChooserDialogFragment::UpdateVolume()
{
    if (mRouter == NULL) return;

    AutoPtr<IRouteInfo> selectedRoute;
    mRouter->GetSelectedRoute(mRouteTypes, (IRouteInfo**)&selectedRoute);
    mVolumeIcon->SetImageResource(
            0/*selectedRoute == null || selectedRoute.getPlaybackType() == RouteInfo.PLAYBACK_TYPE_LOCAL ?
            R.drawable.ic_audio_vol : R.drawable.ic_media_route_on_holo_dark*/);

    mIgnoreSliderVolumeChanges = TRUE;

    Int32 volumeHandling;
    if (selectedRoute == NULL ||
            (selectedRoute->GetVolumeHandling(&volumeHandling), volumeHandling == IRouteInfo::PLAYBACK_VOLUME_FIXED)) {
        // Disable the slider and show it at max volume.
        mVolumeSlider->SetMax(1);
        mVolumeSlider->SetProgress(1);
        mVolumeSlider->SetEnabled(FALSE);
    } else {
        mVolumeSlider->SetEnabled(TRUE);
        Int32 maxVolume;
        selectedRoute->GetVolumeMax(&maxVolume);
        mVolumeSlider->SetMax(maxVolume);
        Int32 volume;
        selectedRoute->GetVolume(&volume);
        mVolumeSlider->SetProgress(volume);
    }

    mIgnoreSliderVolumeChanges = FALSE;
}

void MediaRouteChooserDialogFragment::ChangeVolume(
        /* [in] */ Int32 newValue)
{
    if (mIgnoreSliderVolumeChanges) return;

    AutoPtr<IRouteInfo> selectedRoute;
    mRouter->GetSelectedRoute(mRouteTypes, (IRouteInfo**)&selectedRoute);
    Int32 volumeHandling;
    selectedRoute->GetVolumeHandling(&volumeHandling);
    if (volumeHandling == IRouteInfo::PLAYBACK_VOLUME_VARIABLE) {
        Int32 maxVolume;
        selectedRoute->GetVolumeMax(&maxVolume);
        newValue = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(newValue, maxVolume));
        selectedRoute->RequestSetVolume(newValue);
    }
}

ECode MediaRouteChooserDialogFragment::OnCreateView(
        /* [in] */ ILayoutInflater* inflater,
        /* [in] */ IViewGroup* container,
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IView** view)
{
    mInflater = inflater;
    AutoPtr<IView> layout;
    inflater->InflateEx2(0/*R.layout.media_route_chooser_layout*/,
            container, FALSE, (IView**)&layout);

    layout->FindViewById(0/*R.id.volume_icon*/, (IView**)(IImageView**)&mVolumeIcon);
    layout->FindViewById(0/*R.id.volume_slider*/, (IView**)(ISeekBar**)&mVolumeSlider);
    UpdateVolume();
    AutoPtr<VolumeSliderChangeListener> listener = new VolumeSliderChangeListener(this);
    mVolumeSlider->SetOnSeekBarChangeListener((ISeekBarOnSeekBarChangeListener*)
            listener->Probe(EIID_ISeekBarOnSeekBarChangeListener));

    if (mExtendedSettingsListener != NULL) {
        AutoPtr<IView> extendedSettingsButton;
        layout->FindViewById(0/*R.id.extended_settings*/, (IView**)&extendedSettingsButton);
        extendedSettingsButton->SetVisibility(IView::VISIBLE);
        extendedSettingsButton->SetOnClickListener(mExtendedSettingsListener);
    }

    AutoPtr<IListView> list;
    layout->FindViewById(0/*R.id.list*/, (IView**)(IListView**)&list);
    list->SetItemsCanFocus(TRUE);
    mAdapter = new RouteAdapter(this);
    list->SetAdapter((IAdapter*)(IBaseAdapter*)mAdapter->Probe(EIID_IBaseAdapter));
    list->SetOnItemClickListener(mAdapter);

    mListView = list;
//    mRouter->AddCallback(mRouteTypes, mCallback);

    mAdapter->ScrollToSelectedItem();

    *view = layout;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::OnCreateDialog(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IDialog** dialog)
{
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    Int32 theme;
    GetTheme(&theme);
    *dialog = new RouteChooserDialog(activity, theme, this);
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::OnResume()
{
    DialogFragment::OnResume();
    if (mDisplayService != NULL) {
        mDisplayService->ScanWifiDisplays();
    }
    return NOERROR;
}

IFRAGMENT_METHODS_IMPL(CMediaRouteChooserDialogFragment, MediaRouteChooserDialogFragment);
IDIALOGFRAGMENT_METHODS_IMPL(CMediaRouteChooserDialogFragment, MediaRouteChooserDialogFragment);

ECode CMediaRouteChooserDialogFragment::constructor()
{
    SetStyle(IDialogFragment::STYLE_NO_TITLE, 0/*R.style.Theme_DeviceDefault_Dialog*/);
    return NOERROR;
}

UInt32 CMediaRouteChooserDialogFragment::AddRef()
{
    return _CMediaRouteChooserDialogFragment::AddRef();
}

UInt32 CMediaRouteChooserDialogFragment::Release()
{
    return _CMediaRouteChooserDialogFragment::Release();
}

PInterface CMediaRouteChooserDialogFragment::Probe(
/* [in] */ REIID riid)
{
    return _CMediaRouteChooserDialogFragment::Probe(riid);
}

ECode CMediaRouteChooserDialogFragment::SetLauncherListener(
    /* [in] */ ILauncherListener* listener)
{
    return MediaRouteChooserDialogFragment::SetLauncherListener(listener);
}

ECode CMediaRouteChooserDialogFragment::SetExtendedSettingsClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return MediaRouteChooserDialogFragment::SetExtendedSettingsClickListener(listener);
}

ECode CMediaRouteChooserDialogFragment::SetRouteTypes(
    /* [in] */ Int32 types)
{
    return MediaRouteChooserDialogFragment::SetRouteTypes(types);
}


CAR_INTERFACE_IMPL(MediaRouteChooserDialogFragment::RouteAdapter, IAdapterViewOnItemClickListener)
IBASEADAPTER_METHODS_IMPL(MediaRouteChooserDialogFragment::RouteAdapter, BaseAdapter)

const Int32 MediaRouteChooserDialogFragment::RouteAdapter::VIEW_TOP_HEADER;
const Int32 MediaRouteChooserDialogFragment::RouteAdapter::VIEW_SECTION_HEADER;
const Int32 MediaRouteChooserDialogFragment::RouteAdapter::VIEW_ROUTE;
const Int32 MediaRouteChooserDialogFragment::RouteAdapter::VIEW_GROUPING_ROUTE;
const Int32 MediaRouteChooserDialogFragment::RouteAdapter::VIEW_GROUPING_DONE;

MediaRouteChooserDialogFragment::RouteAdapter::RouteAdapter(
    /* [in] */ MediaRouteChooserDialogFragment* host)
    : mSelectedItemPosition(-1)
    , mCategoryEditingGroups(NULL)
    , mEditingGroup(NULL)
    , mIgnoreUpdates(FALSE)
    , mHost(host)
{
    Update();
}

MediaRouteChooserDialogFragment::RouteAdapter::~RouteAdapter()
{
    mItems.Clear();
    mCatRouteList.Clear();
    mSortRouteList.Clear();
}

void MediaRouteChooserDialogFragment::RouteAdapter::Update()
{
    /*
     * This is kind of wacky, but our data sets are going to be
     * fairly small on average. Ideally we should be able to do some of this stuff
     * in-place instead.
     *
     * Basic idea: each entry in mItems represents an item in the list for quick access.
     * Entries can be a RouteCategory (section header), a RouteInfo with a category of
     * mCategoryEditingGroups (a flattened RouteInfo pulled out of its group, allowing
     * the user to change the group),
     */
    if (mIgnoreUpdates) return;

    mItems.Clear();

    AutoPtr<IRouteInfo> selectedRoute;
    mHost->mRouter->GetSelectedRoute(mHost->mRouteTypes, (IRouteInfo**)&selectedRoute);
    mSelectedItemPosition = -1;

    Int32 catCount;
    mHost->mRouter->GetCategoryCount(&catCount);
    for (Int32 i = 0; i < catCount; i++) {
        AutoPtr<IRouteCategory> cat;
        mHost->mRouter->GetCategoryAt(i, (IRouteCategory**)&cat);
        AutoPtr< List<AutoPtr<IRouteInfo> > > routes;
//        cat->GetRoutes(mCatRouteList, (List<AutoPtr<IRouteInfo> >**)&routes);

        Boolean issystem;
        cat->IsSystem(&issystem);
        if (!issystem) {
            mItems.PushBack(cat);
        }

        if (cat == mCategoryEditingGroups) {
            AddGroupEditingCategoryRoutes(routes);
        } else {
            AddSelectableRoutes(selectedRoute, routes);
        }

        routes = NULL;
    }

    NotifyDataSetChanged();
    if (mHost->mListView != NULL && mSelectedItemPosition >= 0) {
        mHost->mListView->SetItemChecked(mSelectedItemPosition, TRUE);
    }
}

void MediaRouteChooserDialogFragment::RouteAdapter::ScrollToEditingGroup()
{
    if (mCategoryEditingGroups == NULL || mHost->mListView == NULL) return;

    Int32 pos = 0;
    Int32 bound = 0;
    List<AutoPtr<IInterface> >::Iterator it;
    Int32 i = 0;
    for (it = mItems.Begin(); it != mItems.End(); ++it) {
        AutoPtr<IInterface> item = *it;
        if (item != NULL && item == mCategoryEditingGroups) {
            bound = i;
        }
        if (item == NULL) {
            pos = i;
            break; // this is always below the category header; we can stop here.
        }
        ++i;
    }

    mHost->mListView->SmoothScrollToPositionEx(pos, bound);
}

void MediaRouteChooserDialogFragment::RouteAdapter::ScrollToSelectedItem()
{
    if (mHost->mListView == NULL || mSelectedItemPosition < 0) return;

    mHost->mListView->SmoothScrollToPosition(mSelectedItemPosition);
}

void MediaRouteChooserDialogFragment::RouteAdapter::AddSelectableRoutes(
    /* [in] */ IRouteInfo* selectedRoute,
    /* [in] */ List<AutoPtr<IRouteInfo> >* from)
{
    List<AutoPtr<IRouteInfo> >::Iterator it;
    for (it = from->Begin(); it != from->End(); ++it) {
        AutoPtr<IRouteInfo> info = *it;
        if (info.Get() == selectedRoute) {
            mSelectedItemPosition = mItems.GetSize();
        }
        mItems.PushBack(info);
    }
}

void MediaRouteChooserDialogFragment::RouteAdapter::AddGroupEditingCategoryRoutes(
    /* [in] */ List<AutoPtr<IRouteInfo> >* from)
{
    // Unpack groups and flatten for presentation
    // mSortRouteList will always be empty here.
    List<AutoPtr<IRouteInfo> >::Iterator it;
    for (it = from->Begin(); it != from->End(); ++it) {
        AutoPtr<IRouteInfo> route = *it;
        AutoPtr<IRouteGroup> group;
        route->GetGroup((IRouteGroup**)&group);
        if (((IRouteInfo*)&group) == route) {
            // This is a group, unpack it.
            Int32 groupCount;
            group->GetRouteCount(&groupCount);
            for (Int32 j = 0; j < groupCount; j++) {
                AutoPtr<IRouteInfo> innerRoute;
                group->GetRouteAt(j, (IRouteInfo**)&innerRoute);
                mSortRouteList.PushBack(innerRoute);
            }
        } else {
            mSortRouteList.PushBack(route);
        }
    }
    // Sort by name. This will keep the route positions relatively stable even though they
    // will be repeatedly added and removed.
//    Collections.sort(mSortRouteList, mComparator);

    mItems.Assign(mSortRouteList.Begin(), mSortRouteList.End());
    mSortRouteList.Clear();

    mItems.PushBack(NULL); // Sentinel reserving space for the "done" button.
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = GetCount();
    return NOERROR;
}

Int32 MediaRouteChooserDialogFragment::RouteAdapter::GetCount()
{
    return mItems.GetSize();
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = 5;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* type)
{
    AutoPtr<IInterface> item;
    GetItem(position, (IInterface**)&item);
    if (item->Probe(EIID_IRouteCategory)) {
        *type = position == 0 ? VIEW_TOP_HEADER : VIEW_SECTION_HEADER;
        return NOERROR;
    } else if (item == NULL) {
        *type = VIEW_GROUPING_DONE;
        return NOERROR;
    } else {
        AutoPtr<IRouteInfo> info = (IRouteInfo*)item->Probe(EIID_IRouteInfo);
        AutoPtr<IRouteCategory> rCategory;
        info->GetCategory((IRouteCategory**)&rCategory);
        if (rCategory == mCategoryEditingGroups) {
            *type = VIEW_GROUPING_ROUTE;
            return NOERROR;
        }
        *type = VIEW_ROUTE;
        return NOERROR;
    }
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    *enabled = FALSE;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    Int32 type;
    GetItemViewType(position, &type);
    switch (type) {
        case VIEW_ROUTE:
            return (IRouteInfo::Probe(mItems[position].Get()))->IsEnabled(enabled);
        case VIEW_GROUPING_ROUTE:
        case VIEW_GROUPING_DONE:
            *enabled = TRUE;
            return NOERROR;
        default:
            *enabled = FALSE;
            return NOERROR;
    }
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    AutoPtr<IInterface> temp = GetItem(position);
    *item = temp;
    INTERFACE_ADDREF(*item);
    return NOERROR;
}

AutoPtr<IInterface> MediaRouteChooserDialogFragment::RouteAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return mItems[position];
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    *id = GetItemId(position);
    return NOERROR;
}

Int64 MediaRouteChooserDialogFragment::RouteAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return (Int64)position;
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    Int32 viewType;
    GetItemViewType(position, &viewType);

    AutoPtr<ViewHolder> holder;
    if (convertView == NULL) {
        mHost->mInflater->InflateEx2((*ITEM_LAYOUTS)[viewType], parent, FALSE, &convertView);
        holder = new ViewHolder();
        holder->mPosition = position;
        AutoPtr<IView> tempView;
        assert(0 && "TODO: set id!");
        convertView->FindViewById(0/*R.id.text1*/, (IView**)&tempView);
        holder->mText1 = ITextView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.text2*/, (IView**)&tempView);
        holder->mText2 = ITextView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.icon*/, (IView**)&tempView);
        holder->mIcon = IImageView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.check*/, (IView**)&tempView);
        holder->mCheck = ICheckBox::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.expand_button*/, (IView**)&tempView);
        holder->mExpandGroupButton = IImageButton::Probe(tempView);
        tempView = NULL;
        if (holder->mExpandGroupButton != NULL) {
            holder->mExpandGroupListener = new ExpandGroupListener(this);
            holder->mExpandGroupButton->SetOnClickListener(holder->mExpandGroupListener);
        }

        AutoPtr<IView> fview = convertView;
        AutoPtr<IListView> list = IListView::Probe(parent);;
        AutoPtr<ViewHolder> fholder = holder;
        AutoPtr<ConvertViewOnClickListener> convertOnClickListener =
                new ConvertViewOnClickListener(list, fview, fholder);
        convertView->SetOnClickListener(convertOnClickListener.Get());
        convertView->SetTag(holder);
    }
    else {
        AutoPtr<IInterface> obj;
        convertView->GetTag((IInterface**)&obj);
        holder = (ViewHolder*)obj.Get();
        holder->mPosition = position;
    }

    switch (viewType) {
        case VIEW_ROUTE:
        case VIEW_GROUPING_ROUTE:
            BindItemView(position, holder);
            break;
        case VIEW_SECTION_HEADER:
        case VIEW_TOP_HEADER:
            BindHeaderView(position, holder);
            break;
    }

    convertView->SetActivated(position == mSelectedItemPosition);
    Boolean enabled;
    IsEnabled(position, &enabled);
    convertView->SetEnabled(enabled);

    *view = convertView;
    return NOERROR;
}

AutoPtr<IView> MediaRouteChooserDialogFragment::RouteAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    Int32 viewType;
    GetItemViewType(position, &viewType);

    AutoPtr<ViewHolder> holder;
    if (convertView == NULL) {
        mHost->mInflater->InflateEx2((*ITEM_LAYOUTS)[viewType], parent, FALSE, (IView**)&convertView);
        holder = new ViewHolder();
        holder->mPosition = position;
        AutoPtr<IView> tempView;
        assert(0 && "TODO: set id!");
        convertView->FindViewById(0/*R.id.text1*/, (IView**)&tempView);
        holder->mText1 = ITextView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.text2*/, (IView**)&tempView);
        holder->mText2 = ITextView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.icon*/, (IView**)&tempView);
        holder->mIcon = IImageView::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.check*/, (IView**)&tempView);
        holder->mCheck = ICheckBox::Probe(tempView);
        tempView = NULL;
        convertView->FindViewById(0/*R.id.expand_button*/, (IView**)&tempView);
        holder->mExpandGroupButton = IImageButton::Probe(tempView);
        tempView = NULL;
        if (holder->mExpandGroupButton != NULL) {
            holder->mExpandGroupListener = new ExpandGroupListener(this);
            holder->mExpandGroupButton->SetOnClickListener(holder->mExpandGroupListener);
        }

        AutoPtr<IView> fview = convertView;
        AutoPtr<IListView> list = IListView::Probe(parent);
        AutoPtr<ViewHolder> fholder = holder;
        AutoPtr<ConvertViewOnClickListener> convertOnClickListener =
            new ConvertViewOnClickListener(list, fview, fholder);
        convertView->SetOnClickListener(convertOnClickListener);
        convertView->SetTag(holder);
    }
    else {
        AutoPtr<IInterface> obj;
        convertView->GetTag((IInterface**)&obj);
        holder = (ViewHolder*)obj.Get();
        holder->mPosition = position;
    }

    switch (viewType) {
        case VIEW_ROUTE:
        case VIEW_GROUPING_ROUTE:
            BindItemView(position, holder);
            break;
        case VIEW_SECTION_HEADER:
        case VIEW_TOP_HEADER:
            BindHeaderView(position, holder);
            break;
    }

    convertView->SetActivated(position == mSelectedItemPosition);
    Boolean enabled;
    IsEnabled(position, &enabled);
    convertView->SetEnabled(enabled);

    return convertView;
}

void MediaRouteChooserDialogFragment::RouteAdapter::BindItemView(
    /* [in] */ Int32 position,
    /* [in] */ ViewHolder* holder)
{
    AutoPtr<IRouteInfo> info = IRouteInfo::Probe(mItems[position].Get());
    AutoPtr<IActivity> activity;
    mHost->GetActivity((IActivity**)&activity);
    AutoPtr<ICharSequence> name;
    info->GetNameEx(activity, (ICharSequence**)&name);
    holder->mText1->SetText(name);
    AutoPtr<ICharSequence> status;
    info->GetStatus((ICharSequence**)&status);
    if (TextUtils::IsEmpty(status)) {
        holder->mText2->SetVisibility(IView::GONE);
    } else {
        holder->mText2->SetVisibility(IView::VISIBLE);
        holder->mText2->SetText(status);
    }
    AutoPtr<IDrawable> icon;
    info->GetIconDrawable((IDrawable**)&icon);
    if (icon != NULL) {
        // Make sure we have a fresh drawable where it doesn't matter if we mutate it
        AutoPtr<IResources> resources;
        mHost->GetResources((IResources**)&resources);
        AutoPtr<IDrawableConstantState> state;
        AutoPtr<IDrawable> oldIcon = icon;
        oldIcon->GetConstantState((IDrawableConstantState**)&state);
        icon = NULL;
        state->NewDrawableEx(resources, (IDrawable**)&icon);
    }
    holder->mIcon->SetImageDrawable(icon);
    holder->mIcon->SetVisibility(icon != NULL ? IView::VISIBLE : IView::GONE);

    AutoPtr<IRouteCategory> cat;
    info->GetCategory((IRouteCategory**)&cat);
    Boolean canGroup = FALSE;
    if (cat == mCategoryEditingGroups) {
        AutoPtr<IRouteGroup> group;
        info->GetGroup((IRouteGroup**)&group);
        Int32 count;
        group->GetRouteCount(&count);
        (holder->mCheck)->SetEnabled(count > 1);
        ((ICheckable*)holder->mCheck->Probe(EIID_ICheckable))->SetChecked(group == mEditingGroup);
    } else {
        Boolean result;
        cat->IsGroupable(&result);
        if (result) {
            AutoPtr<IRouteGroup> group = (IRouteGroup*) info.Get();
            Int32 rcount;
            group->GetRouteCount(&rcount);
            Int32 type;
            GetItemViewType(position - 1, &type);
            Int32 vtype;
            GetItemViewType(position + 1, &vtype);
            Int32 count;
            GetCount(&count);
            canGroup = rcount > 1 || type == VIEW_ROUTE ||
                    (position < count - 1 && vtype == VIEW_ROUTE);
        }
    }

    if (holder->mExpandGroupButton != NULL) {
        holder->mExpandGroupButton->SetVisibility(canGroup ? IView::VISIBLE : IView::GONE);
        holder->mExpandGroupListener->mPosition = position;
    }
}

void MediaRouteChooserDialogFragment::RouteAdapter::BindHeaderView(
    /* [in] */ Int32 position,
    /* [in] */ ViewHolder* holder)
{
    AutoPtr<IRouteCategory> cat = IRouteCategory::Probe(mItems[position].Get());
    AutoPtr<IActivity> activity;
    mHost->GetActivity((IActivity**)&activity);
    AutoPtr<ICharSequence> name;
    cat->GetNameEx(activity, (ICharSequence**)&name);
    holder->mText1->SetText(name);
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Int32 type;
    GetItemViewType(position, &type);
    if (type == VIEW_SECTION_HEADER || type == VIEW_TOP_HEADER) {
        return NOERROR;
    } else if (type == VIEW_GROUPING_DONE) {
        FinishGrouping();
        return NOERROR;
    } else {
        AutoPtr<IInterface> item;
        GetItem(position, (IInterface**)&item);
        if (!(item->Probe(EIID_IRouteInfo))) {
            // Oops. Stale event running around? Skip it.
            return NOERROR;
        }

        AutoPtr<IRouteInfo> route = (IRouteInfo*) item->Probe(EIID_IRouteInfo);
        if (type == VIEW_ROUTE) {
            mHost->mRouter->SelectRouteInt(mHost->mRouteTypes, route);
            mHost->Dismiss();
        } else if (type == VIEW_GROUPING_ROUTE) {
            AutoPtr<ICheckable> c = (ICheckable*) view->Probe(EIID_ICheckable);
            Boolean wasChecked;
            c->IsChecked(&wasChecked);

            mIgnoreUpdates = TRUE;
            AutoPtr<IRouteGroup> oldGroup;
            route->GetGroup((IRouteGroup**)&oldGroup);
            Int32 routeCount;
            mEditingGroup->GetRouteCount(&routeCount);
            if (!wasChecked && oldGroup != mEditingGroup) {
                // Assumption: in a groupable category oldGroup will never be null.
                AutoPtr<IRouteInfo> routeInfo;
                mHost->mRouter->GetSelectedRoute(mHost->mRouteTypes, (IRouteInfo**)&routeInfo);
                if (((IRouteInfo*)&oldGroup) == routeInfo) {
                    // Old group was selected but is now empty. Select the group
                    // we're manipulating since that's where the last route went.
                    mHost->mRouter->SelectRouteInt(mHost->mRouteTypes, (IRouteInfo*)&mEditingGroup);
                }
                oldGroup->RemoveRoute(route);
                mEditingGroup->AddRoute(route);
                c->SetChecked(TRUE);
            } else if (wasChecked && routeCount > 1) {
                mEditingGroup->RemoveRoute(route);

                // In a groupable category this will add
                // the route into its own new group.
                mHost->mRouter->AddRouteInt(route);
            }
            mIgnoreUpdates = FALSE;
            Update();
        }
    }
    return NOERROR;
}

Boolean MediaRouteChooserDialogFragment::RouteAdapter::IsGrouping()
{
    return mCategoryEditingGroups != NULL;
}

void MediaRouteChooserDialogFragment::RouteAdapter::FinishGrouping()
{
    mCategoryEditingGroups = NULL;
    mEditingGroup = NULL;
    AutoPtr<IDialog> dialog;
    mHost->GetDialog((IDialog**)&dialog);
    dialog->SetCanceledOnTouchOutside(TRUE);
    Update();
    ScrollToSelectedItem();
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return BaseAdapter::RegisterDataSetObserver(observer);
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return BaseAdapter::UnregisterDataSetObserver(observer);
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    *hasStableIds = BaseAdapter::HasStableIds();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::RouteAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    *isEmpty = BaseAdapter::IsEmpty();
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(MediaRouteChooserDialogFragment::RouteAdapter::ExpandGroupListener,
    IViewOnClickListener, IBaseAdapter)

ECode MediaRouteChooserDialogFragment::RouteAdapter::ExpandGroupListener::OnClick(
    /* [in] */ IView* v)
{
    // Assumption: this is only available for the user to click if we're presenting
    // a groupable category, where every top-level route in the category is a group.
    AutoPtr<IRouteGroup> group = (IRouteGroup*) mAHost->GetItem(mPosition).Get();
    mAHost->mEditingGroup = group;
    mAHost->mCategoryEditingGroups = NULL;
    ((IRouteInfo*)&group)->GetCategory((IRouteCategory**)&mAHost->mCategoryEditingGroups);
    AutoPtr<IDialog> dialog;
    mAHost->mHost->GetDialog((IDialog**)&dialog);
    dialog->SetCanceledOnTouchOutside(FALSE);
    mAHost->mEditingGroup = NULL;
    mAHost->mHost->mRouter->SelectRouteInt(mAHost->mHost->mRouteTypes, ((IRouteInfo*)&(mAHost->mEditingGroup)));
    mAHost->Update();
    mAHost->ScrollToEditingGroup();
    return NOERROR;
}

CAR_INTERFACE_IMPL(MediaRouteChooserDialogFragment::ViewHolder, IInterface)

UInt32 MediaRouteChooserDialogFragment::MediaRouterCallback::AddRef()
{
    return ElRefBase::AddRef();
}
UInt32 MediaRouteChooserDialogFragment::MediaRouterCallback::Release()
{
    return ElRefBase::Release();
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteSelected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IRouteInfo* info)
{
    mHost->mAdapter->Update();
    mHost->UpdateVolume();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteUnselected(
    /* [in] */ IMediaRouter* router,
    /* [in] */ Int32 type,
    /* [in] */ IRouteInfo* info)
{
    mHost->mAdapter->Update();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteAdded(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info)
{
    mHost->mAdapter->Update();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteRemoved(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info)
{
    if (info == ((IRouteInfo*)&(mHost->mAdapter->mEditingGroup))) {
        mHost->mAdapter->FinishGrouping();
    }
    mHost->mAdapter->Update();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info)
{
    mHost->mAdapter->NotifyDataSetChanged();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteGrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info,
    /* [in] */ IRouteGroup* group,
    /* [in] */ Int32 index)
{
    mHost->mAdapter->Update();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteUngrouped(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info,
    /* [in] */ IRouteGroup* group)
{
    mHost->mAdapter->Update();
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::MediaRouterCallback::OnRouteVolumeChanged(
    /* [in] */ IMediaRouter* router,
    /* [in] */ IRouteInfo* info)
{
    if (!mHost->mIgnoreCallbackVolumeChanges) {
        mHost->UpdateVolume();
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(MediaRouteChooserDialogFragment::RouteComparator, IComparator)

ECode MediaRouteChooserDialogFragment::RouteComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    AutoPtr<IActivity> activity;
    mHost->GetActivity((IActivity**)&activity);
    AutoPtr<ICharSequence> lname;
    ((IRouteInfo*)lhs)->GetNameEx(activity, (ICharSequence**)&lname);
    AutoPtr<ICharSequence> rname;
    ((IRouteInfo*)rhs)->GetNameEx(activity, (ICharSequence**)&rname);
    String lnamestring, rnamestring;
    lname->ToString(&lnamestring);
    rname->ToString(&rnamestring);
    *result = lnamestring.Equals(rnamestring);
    return NOERROR;
}

MediaRouteChooserDialogFragment::_RouteChooserDialog::_RouteChooserDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ MediaRouteChooserDialogFragment* host)
    : Dialog(context, theme)
{}

ECode MediaRouteChooserDialogFragment::_RouteChooserDialog::OnBackPressed()
{
    if (mHost->mAdapter != NULL && mHost->mAdapter->IsGrouping()) {
        mHost->mAdapter->FinishGrouping();
    } else {
        Dialog::OnBackPressed();
    }
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::_RouteChooserDialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* keydown)
{
    Boolean enabled;
    mHost->mVolumeSlider->IsEnabled(&enabled);
    AutoPtr<IRouteInfo> rInfo;
    mHost->mRouter->GetSelectedRoute(mHost->mRouteTypes, (IRouteInfo**)&rInfo);
    if (keyCode == IKeyEvent::KEYCODE_VOLUME_DOWN && enabled) {
        rInfo->RequestUpdateVolume(-1);
        *keydown = TRUE;
        return NOERROR;
    } else if (keyCode == IKeyEvent::KEYCODE_VOLUME_UP && enabled) {
        rInfo->RequestUpdateVolume(1);
        *keydown = TRUE;
        return NOERROR;
    } else {
        *keydown = Dialog::OnKeyDown(keyCode, event);
        return NOERROR;
    }
}

ECode MediaRouteChooserDialogFragment::_RouteChooserDialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* keyup)
{
    Boolean enabled;
    mHost->mVolumeSlider->IsEnabled(&enabled);
    if (keyCode == IKeyEvent::KEYCODE_VOLUME_DOWN && enabled) {
        *keyup = TRUE;
        return NOERROR;
    } else if (keyCode == IKeyEvent::KEYCODE_VOLUME_UP && enabled) {
        *keyup = TRUE;
        return NOERROR;
    } else {
        *keyup = Dialog::OnKeyUp(keyCode, event);
        return NOERROR;
    }
}

CAR_INTERFACE_IMPL(MediaRouteChooserDialogFragment::RouteChooserDialog, IDialog)
IDIALOG_METHODS_IMPL(MediaRouteChooserDialogFragment::RouteChooserDialog,
        MediaRouteChooserDialogFragment::_RouteChooserDialog)


CAR_INTERFACE_IMPL(MediaRouteChooserDialogFragment::VolumeSliderChangeListener, ISeekBarOnSeekBarChangeListener)

ECode MediaRouteChooserDialogFragment::VolumeSliderChangeListener::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    mHost->ChangeVolume(progress);
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::VolumeSliderChangeListener::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mHost->mIgnoreCallbackVolumeChanges = TRUE;
    return NOERROR;
}

ECode MediaRouteChooserDialogFragment::VolumeSliderChangeListener::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mHost->mIgnoreCallbackVolumeChanges = FALSE;
    mHost->UpdateVolume();
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
