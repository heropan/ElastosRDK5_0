
#include "elastos/droid/preference/PreferenceActivity.h"
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/utility/XmlUtils.h"
#include "elastos/droid/app/Fragment.h"
#include "elastos/droid/text/TextUtils.h"
#ifdef DROID_CORE
#include "elastos/droid/preference/CPreferenceManager.h"
#include "elastos/droid/preference/CPreferenceActivityHeader.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CIntentHelper.h"
#include "elastos/droid/os/CBundle.h"
// #include "elastos/droid/widget/CFrameLayoutLayoutParams.h"
#endif
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Core::CString;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::App::Fragment;
using Elastos::Droid::App::IFragmentManager;
using Elastos::Droid::App::IFragmentTransaction;
using Elastos::Droid::App::EIID_IActivity;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentHelper;
using Elastos::Droid::Content::IIntentHelper;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::IViewGroupLayoutParams;
// using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
using Elastos::Droid::Widget::IAbsListView;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Org::Xmlpull::V1::IXmlPullParser;

namespace Elastos {
namespace Droid {
namespace Preference {
const String PreferenceActivity::TAG("PreferenceActivity");
const String PreferenceActivity::HEADERS_TAG(":android:headers");
const String PreferenceActivity::CUR_HEADER_TAG(":android:cur_header");
const String PreferenceActivity::PREFERENCES_TAG(":android:preferences");
const String PreferenceActivity::BACK_STACK_PREFS(":android:prefs");
const String PreferenceActivity::EXTRA_PREFS_SHOW_BUTTON_BAR("extra_prefs_show_button_bar");
const String PreferenceActivity::EXTRA_PREFS_SHOW_SKIP("extra_prefs_show_skip");
const String PreferenceActivity::EXTRA_PREFS_SET_NEXT_TEXT("extra_prefs_set_next_text");
const String PreferenceActivity::EXTRA_PREFS_SET_BACK_TEXT("extra_prefs_set_back_text");
const Int32 PreferenceActivity::FIRST_REQUEST_CODE;
const Int32 PreferenceActivity::MSG_BIND_PREFERENCES;
const Int32 PreferenceActivity::MSG_BUILD_HEADERS;

//====================================================
// PreferenceActivity::MyHandler
//====================================================
onBuildHeaders(List<Header> target)
ECode PreferenceActivity::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case MSG_BIND_PREFERENCES:
            mHost->BindPreferences();
            break;
        case MSG_BUILD_HEADERS:
            List<AutoPtr<IPreferenceActivityHeader> > oldHeaders(mHost->mHeaders);
            mHost->mHeaders.Clear();
            mHost->OnBuildHeaders(mHeaders);
            AutoPtr<IBaseAdapter> baseAdapter = IBaseAdapter::Probe(mAdapter);
            if (baseAdapter != NULL) {
                baseAdapter->NotifyDataSetChanged();
            }
            AutoPtr<IPreferenceActivityHeader> header;
            mHost->OnGetNewHeader((IPreferenceActivityHeader**)&header);
            String fragment;
            if (header != NULL && (header->GetFragment(&fragment), !fragment.IsNull())) {
                AutoPtr<IPreferenceActivityHeader> mappedHeader = mHost->FindBestMatchingHeader(header, oldHeaders);
                if (mappedHeader == NULL || mHost->mCurHeader != mappedHeader) {
                    mHost->SwitchToHeader(header);
                }
            }
            else if (mHost->mCurHeader != NULL) {
                AutoPtr<IPreferenceActivityHeader> mappedHeader = mHost->FindBestMatchingHeader(mHost->mCurHeader, mHost->mHeaders);
                if (mappedHeader != NULL) {
                    mHost->SetSelectedHeader(mappedHeader);
                }
            }
            break;
    }

    return NOERROR;
}

//====================================================
// PreferenceActivity::HeaderAdapter
//====================================================
PreferenceActivity::HeaderAdapter::HeaderAdapter(
    /* [in] */ IContext* context,
    /* [in] */ List<AutoPtr<IPreferenceActivityHeader> > * objects,
    /* [in] */ Int32 layoutResId,
    /* [in] */ Boolean removeIconBehavior)
        : ArrayAdapter(context, 0, objects)
        , mLayoutResId(layoutResId)
        , mRemoveIconIfEmpty(removeIconBehavior)
{
    AutoPtr<IInterface> object;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&object);
    mInflater = ILayoutInflater::Probe(object);
}

PreferenceActivity::HeaderAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    AutoPtr<HeaderViewHolder> holder;
    AutoPtr<IView> lView;

    if (convertView == NULL) {
        mInflater->Inflate(mLayoutResId, FALSE, (IView**)&lView);
        holder = new HeaderViewHolder();
        AutoPtr<IView> tempView;
        lView->FindViewById(R::id::icon, (IView**)&tempView);
        holder->mIcon = IImageView::Probe(tempView);
        tempView = NULL;
        lView->FindViewById(R::id::title, (IView**)&tempView);
        holder->mTitle = ITextView::Probe(tempView);
        tempView = NULL;
        lView->FindViewById(R::id::summary, (IView**)&tempView);
        holder->mSummary = ITextView::Probe(tempView);
        tempView = NULL;
        lView->SetTag(holder);
    }
    else {
        lView = convertView;
        AutoPtr<IInterface> inface;
        lView->GetTag((IInterface**)&inface);
        holder = (HeaderViewHolder*)inface.Get();
    }

    // All view fields must be updated every time, because the view may be recycled
    AutoPtr<IInterface> obj;
    GetItem(position, (IInterface**)&obj);
    AutoPtr<IPreferenceActivityHeader> header = IPreferenceActivityHeader::Probe(obj);
    Int32 iconRes;
    header->GetIconRes(&iconRes);
    if (mRemoveIconIfEmpty) {
        Int32 iconRes = 0;
        header->GetIconRes(&iconRes);
        if (iconRes == 0) {
            holder->icon->SetVisibility(IView::GONE);
        } else {
            holder->icon->SetVisibility(IView::VISIBLE);
            holder->icon->SetImageResource(iconRes);
        }
    } else {
        holder->icon->SetImageResource(iconRes);
    }

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resource;
    context->GetResources((IResources**)&resource);
    AutoPtr<ICharSequence> title;
    header->GetTitle(resource, (ICharSequence**)&title);
    holder->mTitle->SetText(title);
    AutoPtr<ICharSequence> summary;
    header->GetSummary(resource, (ICharSequence**)&summary);\
    if (!TextUtils::IsEmpty(summary)) {
        holder->mSummary->SetVisibility(IView::VISIBLE);
        holder->mSummary->SetText(summary);
    }
    else {
        holder->mSummary->SetVisibility(IView::GONE);
    }

    *view = lView;
    REFCOUNT_ADD(*view)

    return NOERROR;
}


//====================================================
// PreferenceActivity::BackButtonListener
//====================================================

CAR_INTERFACE_IMPL(PreferenceActivity::BackButtonListener, Object, IViewOnClickListener);

PreferenceActivity::BackButtonListener::BackButtonListener(
    /* [in] */ PreferenceActivity* parent)
    : mParent(parent)
{}

ECode PreferenceActivity::BackButtonListener::OnClick(
    /* [in] */ IView* v)
{
    mParent->SetResult(IActivity::RESULT_CANCELED);
    return mParent->Finish();
}


//====================================================
// PreferenceActivity::SkipButtonListener
//====================================================

CAR_INTERFACE_IMPL(PreferenceActivity::SkipButtonListener, Object, IViewOnClickListener);

PreferenceActivity::SkipButtonListener::SkipButtonListener(
    /* [in] */ PreferenceActivity* parent)
    : mParent(parent)
{}

ECode PreferenceActivity::SkipButtonListener::OnClick(
    /* [in] */ IView* v)
{
    mParent->SetResult(IActivity::RESULT_OK);
    return mParent->Finish();
}


//====================================================
// PreferenceActivity::NextButtonListener
//====================================================

CAR_INTERFACE_IMPL(PreferenceActivity::NextButtonListener, Object, IViewOnClickListener)

PreferenceActivity::NextButtonListener::NextButtonListener(
    /* [in] */ PreferenceActivity* parent)
    : mParent(parent)
{}

ECode PreferenceActivity::NextButtonListener::OnClick(
    /* [in] */ IView* v)
{
    mParent->SetResult(IActivity::RESULT_OK);
    return mParent->Finish();
}


//====================================================
// PreferenceActivity
//====================================================
CAR_INTERFACE_IMPL_3(PreferenceActivity, ListActivity, IPreferenceActivity, IPreferenceManagerOnPreferenceTreeClickListener, IPreferenceFragmentOnPreferenceStartFragmentCallback)

PreferenceActivity::PreferenceActivity()
    : mSinglePane(FALSE)
    , mPreferenceHeaderItemResId(0)
    , mPreferenceHeaderRemoveEmptyIcon(FALSE)
{
    mHandler = new MyHandler(this);
}

PreferenceActivity::~PreferenceActivity()
{}

ECode PreferenceActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    ListActivity::OnCreate(savedInstanceState);

    // Theming for the PreferenceActivity layout and for the Preference Header(s) layout
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PreferenceActivity),
            ARRAY_SIZE(R::styleable::PreferenceActivity));
    AutoPtr<ITypedArray> sa;
    Activity::ObtainStyledAttributes(NULL, attrIds,
        R::attr::preferenceActivityStyle, 0, (ITypedArray**)&sa);

    Int32 layoutResId;
    sa->GetResourceId(
            R::styleable::PreferenceActivity_layout,
            R::layout::preference_list_content, &layoutResId);

    sa->GetResourceId(
            R::styleable::PreferenceActivity_headerLayout,
            R::layout::preference_header_item, &mPreferenceHeaderItemResId);
    sa->GetBoolean(
            R::styleable::PreferenceActivity_headerRemoveIconIfEmpty,
            FALSE, &mPreferenceHeaderRemoveEmptyIcon);

    sa->Recycle();

    Activity::SetContentView(layoutResId);

    AutoPtr<IView> view = Activity::FindViewById(R::id::list_footer);
    mListFooter = IFrameLayout::Probe(view);
    view = Activity::FindViewById(R::id::prefs_frame);
    mPrefsContainer = IViewGroup::Probe(view);
    Boolean hidingHeaders, isMultiPane;
    OnIsHidingHeaders(&hidingHeaders);
    OnIsMultiPane(&isMultiPane);
    mSinglePane = hidingHeaders || !isMultiPane;
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    String initialFragment;
    intent->GetStringExtra(EXTRA_SHOW_FRAGMENT, &initialFragment);
    AutoPtr<IBundle> initialArguments;
    intent->GetBundleExtra(EXTRA_SHOW_FRAGMENT_ARGUMENTS, (IBundle**)&initialArguments);
    Int32 initialTitle;
    intent->GetInt32Extra(EXTRA_SHOW_FRAGMENT_TITLE, 0, &initialTitle);
    Int32 initialShortTitle;
    intent->GetInt32Extra(EXTRA_SHOW_FRAGMENT_SHORT_TITLE, 0, &initialShortTitle);

    if (savedInstanceState != NULL) {
        // We are restarting from a previous saved state; used that to
        // initialize, instead of starting fresh.
        AutoPtr<IObjectContainer> container;
        savedInstanceState->GetParcelableArrayList(HEADERS_TAG, (IObjectContainer**)&container);
        if (container != NULL) {
            AutoPtr<IObjectEnumerator> objEnumerator;
            container->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator);
            Boolean hasNext;
            while (objEnumerator->MoveNext(&hasNext), hasNext) {
                AutoPtr<IPreferenceActivityHeader> header;
                objEnumerator->Current((IInterface**)&header);
                mHeaders.PushBack(header);
            }
            Int32 curHeader;
            savedInstanceState->GetInt32(CUR_HEADER_TAG, (Int32) HEADER_ID_UNDEFINED, &curHeader);
            if (curHeader >= 0 && curHeader < (Int32)mHeaders.GetSize()) {
                SetSelectedHeader(mHeaders[curHeader]);
            }
        }
    }
    else {
        if (!initialFragment.IsNull() && mSinglePane) {
            // If we are just showing a fragment, we want to run in
            // new fragment mode, but don't need to compute and show
            // the headers.
            SwitchToHeader(initialFragment, initialArguments);
            if (initialTitle != 0) {
                AutoPtr<ICharSequence> initialTitleStr;
                GetText(initialTitle, (ICharSequence**)&initialTitleStr);
                AutoPtr<ICharSequence> initialShortTitleStr;
                if(initialShortTitle != 0) {
                    GetText(initialShortTitle, (ICharSequence**)&initialShortTitleStr);
                }
                ShowBreadCrumbs(initialTitleStr, initialShortTitleStr);
            }

        }
        else {
            // We need to try to build the headers.
            AutoPtr<IObjectContainer> container;
            CObjectContainer::New((IObjectContainer**)&container);
            List<AutoPtr<IPreferenceActivityHeader> >::Iterator it = mHeaders.Begin();
            for (; it != mHeaders.End(); ++it) {
                container->Add(*it);
            }
            OnBuildHeaders(container);

            // If there are headers, then at this point we need to show
            // them and, depending on the screen, we may also show in-line
            // the currently selected preference fragment.
            if (mHeaders.Begin() != mHeaders.End()) {
                if (!mSinglePane) {
                    if (initialFragment.IsNull()) {
                        AutoPtr<IPreferenceActivityHeader> h;
                        OnGetInitialHeader((IPreferenceActivityHeader**)&h);
                        SwitchToHeader(h);
                    }
                    else {
                        SwitchToHeader(initialFragment, initialArguments);
                    }
                }
            }
        }
    }

    // The default configuration is to only show the list view.  Adjust
    // visibility for other configurations.
    if (!initialFragment.IsNull() && mSinglePane) {
        // Single pane, showing just a prefs fragment.
        Activity::FindViewById(R::id::headers)->SetVisibility(IView::GONE);
        mPrefsContainer->SetVisibility(IView::VISIBLE);
        if (initialTitle != 0) {
            AutoPtr<ICharSequence> initialTitleStr;
            GetText(initialTitle, (ICharSequence**)&initialTitleStr);
            AutoPtr<ICharSequence> initialShortTitleStr;
            if (initialShortTitle != 0) {
                GetText(initialShortTitle, (ICharSequence**)&initialShortTitleStr);
            }
            ShowBreadCrumbs(initialTitleStr, initialShortTitleStr);
        }
    }
    else if (mHeaders.GetSize() > 0) {
        setListAdapter(new HeaderAdapter(this, mHeaders, mPreferenceHeaderItemResId,
                mPreferenceHeaderRemoveEmptyIcon));
        AutoPtr<HeaderAdapter> ha = new HeaderAdapter(IContext::Probe(this), mHeaders,
             mPreferenceHeaderItemResId,mPreferenceHeaderRemoveEmptyIcon);
        ListActivity::SetListAdapter(IListAdapter::Probe(ha));
        if (!mSinglePane) {
            // Multi-pane.
            AutoPtr<IListView> listView;
            GetListView((IListView**)&listView);
            listView->SetChoiceMode(IAbsListView::CHOICE_MODE_SINGLE);
            if (mCurHeader != NULL) {
                SetSelectedHeader(mCurHeader);
            }
            mPrefsContainer->SetVisibility(IView::VISIBLE);
        }
    }
    else {
        // If there are no headers, we are in the old "just show a screen
        // of preferences" mode.
        SetContentView(R::layout::preference_list_content_single);
        view = Activity::FindViewById(R::id::list_footer);
        mListFooter = IFrameLayout::Probe(view);
        view = Activity::FindViewById(R::id::prefs);
        mPrefsContainer = IViewGroup::Probe(view);
        CPreferenceManager::New(THIS_PROBE(IActivity), FIRST_REQUEST_CODE, (IPreferenceManager**)&mPreferenceManager);
        AutoPtr<IPreferenceManagerOnPreferenceTreeClickListener> listener;
        listener = (IPreferenceManagerOnPreferenceTreeClickListener*)this->Probe(EIID_IPreferenceManagerOnPreferenceTreeClickListener);
        mPreferenceManager->SetOnPreferenceTreeClickListener(listener);
    }

    // see if we should show Back/Next buttons
    intent = NULL;
    GetIntent((IIntent**)&intent);
    Boolean isShow;
    if (intent->GetBooleanExtra(EXTRA_PREFS_SHOW_BUTTON_BAR, FALSE, &isShow), isShow) {
        Activity::FindViewById(R::id::button_bar)->SetVisibility(IView::VISIBLE);
        view = Activity::FindViewById(R::id::back_button);
        AutoPtr<IButton> backButton = IButton::Probe(view);
        AutoPtr<IViewOnClickListener> backButtonListener = new BackButtonListener(this);
        backButton->SetOnClickListener(backButtonListener);

        view = Activity::FindViewById(R::id::skip_button);
        AutoPtr<IButton> skipButton = IButton::Probe(view);
        AutoPtr<IViewOnClickListener> skipButtonListener = new SkipButtonListener(this);
        skipButton->SetOnClickListener(skipButtonListener);

        view = Activity::FindViewById(R::id::next_button);
        mNextButton = IButton::Probe(view);
        AutoPtr<IViewOnClickListener> nextButtonListener = new NextButtonListener(this);
        mNextButton->SetOnClickListener(nextButtonListener);

        // set our various button parameters
        Boolean setNextText;
        if (intent->HasExtra(EXTRA_PREFS_SET_NEXT_TEXT, &setNextText), setNextText) {
            String buttonText;
            intent->GetStringExtra(EXTRA_PREFS_SET_NEXT_TEXT, &buttonText);
            if (TextUtils::IsEmpty(buttonText)) {
                mNextButton->SetVisibility(IView::GONE);
            }
            else {
                AutoPtr<ICharSequence> cButtonText;
                CString::New(buttonText, (ICharSequence**)&cButtonText);
                mNextButton->SetText(cButtonText);
            }
        }
        Boolean setBackText;
        if (intent->HasExtra(EXTRA_PREFS_SET_NEXT_TEXT, &setBackText), setBackText) {
            String buttonText;
            intent->GetStringExtra(EXTRA_PREFS_SET_BACK_TEXT, &buttonText);
            if (TextUtils::IsEmpty(buttonText)) {
                backButton->SetVisibility(IView::GONE);
            }
            else {
                AutoPtr<ICharSequence> cButtonText;
                CString::New(buttonText, (ICharSequence**)&cButtonText);
                backButton->SetText(cButtonText);
            }
        }
        Boolean showSkip;
        if (intent->GetBooleanExtra(EXTRA_PREFS_SHOW_SKIP, FALSE, &showSkip), showSkip) {
            skipButton->SetVisibility(IView::VISIBLE);
        }
    }

    return NOERROR;
}

ECode PreferenceActivity::HasHeaders(
    /* [out] */ Boolean* hasHeaders)
{
    VALIDATE_NOT_NULL(hasHeaders)
    AutoPtr<IListView> lw;
    GetListView((IListView**)&lw);
    Int32 visibility;
    lw->GetVisibility(&visibility);
    *hasHeaders = visibility == IView::VISIBLE && mPreferenceManager == NULL;
    return NOERROR;
}

List<AutoPtr<IPreferenceActivityHeader> >& PreferenceActivity::GetHeaders()
{
    return mHeaders;
}

ECode PreferenceActivity::IsMultiPane(
    /* [out] */ Boolean* isMultiPane)
{
    VALIDATE_NOT_NULL(isMultiPane)
    Boolean hasHeaders;
    HasHeaders(&hasHeaders);
    if (!hasHeaders) {
        *isMultiPane = hasHeaders;
    }
    else {
        Int32 visiblity;
        mPrefsContainer->GetVisibility(&visiblity);
        *isMultiPane = visiblity == IView::VISIBLE;
    }
    return NOERROR;
}

ECode PreferenceActivity::OnIsMultiPane(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IResources> resource;
    GetResources((IResources**)&resource);
    return resource->GetBoolean(R::bool_::preferences_prefer_dual_pane, result);
}

ECode PreferenceActivity::OnIsHidingHeaders(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    return intent->GetBooleanExtra(EXTRA_NO_HEADERS, FALSE, result);
}

ECode PreferenceActivity::OnGetInitialHeader(
    /* [out] */ IPreferenceActivityHeader** header)
{
    List<AutoPtr<IPreferenceActivityHeader>::Iterator it;
    for(it = mHeaders.Begin(); it != mHeaders.End(); ++it) {
        AutoPtr<IPreferenceActivityHeader> h = *it;
        String fragment;
        h->GetFragment(&fragment);
        if (!fragment.IsNull()) {
            *header = h;
            REFCOUNT_ADD(*header)
            return NOERROR;
        }
    }
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode PreferenceActivity::OnGetNewHeader(
    /* [out] */ IPreferenceActivityHeader** header)
{
    VALIDATE_NOT_NULL(header)
    *header = NULL;
    return NOERROR;
}

ECode PreferenceActivity::OnBuildHeaders(
    /* [in] */ IObjectContainer* target)
{
    // Should be overloaded by subclasses
    return NOERROR;
}

ECode PreferenceActivity::InvalidateHeaders()
{
    Boolean result;
    if (mHandler->HasMessages(MSG_BUILD_HEADERS, &result), !result) {
        return mHandler->SendEmptyMessage(MSG_BUILD_HEADERS, &result);
    }
    return NOERROR;
}

ECode PreferenceActivity::LoadHeadersFromResource(
    /* [in] */ Int32 resid,
    /* [in] */ IObjectContainer* target)
{
    AutoPtr<IXmlResourceParser> parser;
    // try {
    AutoPtr<IResources> resource;
    GetResources((IResources**)&resource);
    resource->GetXml(resid, (IXmlResourceParser**)&parser);
    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    while (parser->Next(&type),
            (type != IXmlPullParser::END_DOCUMENT && type != IXmlPullParser::START_TAG)) {
        // Parse next until start tag is found
    }

    String nodeName;
    parser->GetName(&nodeName);
    if (!nodeName.Equals("preference-headers")) {
        String description;
        parser->GetPositionDescription(&description);
        Logger::E("PreferenceActivity", "XML document must start with <preference-headers> tag; found %s at %s"
                , nodeName.string(), description.string());
        if (parser != NULL) parser->Close();
        return E_RUNTIME_EXCEPTION;
    }

    AutoPtr<IBundle> curBundle;

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 depth;
    while ((parser->Next(&type), (type != IXmlPullParser::END_DOCUMENT
           && type != IXmlPullParser::END_TAG)) || (parser->GetDepth(&depth), depth > outerDepth)) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        parser->GetName(&nodeName);
        if (nodeName.Equals("header")) {
            AutoPtr<IPreferenceActivityHeader> header;
            CPreferenceActivityHeader::New((IPreferenceActivityHeader**)&header);

            Int32 size = ARRAY_SIZE(R::styleable::PreferenceHeader);
            AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::PreferenceHeader, size);
            AutoPtr<ITypedArray> sa;
            ObtainStyledAttributes(attrs, layout, (ITypedArray**)&sa);
            Int32 id;
            sa->GetResourceId(R::styleable::PreferenceHeader_id, HEADER_ID_UNDEFINED, &id);
            header->SetId(id);

            AutoPtr<ITypedValue> tv;
            sa->PeekValue(R::styleable::PreferenceHeader_title, (ITypedValue**)&tv);
            Int32 type;
            if (tv != NULL && (tv->GetType(&type), type == ITypedValue::TYPE_STRING)) {
                Int32 resourceId;
                tv->GetResourceId(&resourceId);
                if (resourceId != 0) {
                    header->SetTitleRes(resourceId);
                }
                else {
                    AutoPtr<ICharSequence> string;
                    tv->GetString((ICharSequence**)&string);
                    header->SetTitle(string);
                }
            }

            tv = NULL;
            sa->PeekValue(R::styleable::PreferenceHeader_summary, (ITypedValue**)&tv);
            if (tv != NULL && (tv->GetType(&type), type == ITypedValue::TYPE_STRING)) {
                Int32 resourceId;
                tv->GetResourceId(&resourceId);
                if (resourceId != 0) {
                    header->SetSummaryRes(resourceId);
                }
                else {
                    AutoPtr<ICharSequence> string;
                    tv->GetString((ICharSequence**)&string);
                    header->SetSummary(string);
                }
            }

            tv = NULL;
            sa->PeekValue(R::styleable::PreferenceHeader_breadCrumbTitle, (ITypedValue**)&tv);
            if (tv != NULL && (tv->GetType(&type), type == ITypedValue::TYPE_STRING)) {
                Int32 resourceId;
                tv->GetResourceId(&resourceId);
                if (resourceId != 0) {
                    header->SetBreadCrumbTitleRes(resourceId);
                }
                else {
                    AutoPtr<ICharSequence> string;
                    tv->GetString((ICharSequence**)&string);
                    header->SetBreadCrumbTitle(string);
                }
            }

            tv = NULL;
            sa->PeekValue(R::styleable::PreferenceHeader_breadCrumbShortTitle, (ITypedValue**)&tv);
            if (tv != NULL && (tv->GetType(&type), type == ITypedValue::TYPE_STRING)) {
                Int32 resourceId;
                tv->GetResourceId(&resourceId);
                if (resourceId != 0) {
                    header->SetBreadCrumbShortTitleRes(resourceId);
                }
                else {
                    AutoPtr<ICharSequence> string;
                    tv->GetString((ICharSequence**)&string);
                    header->SetBreadCrumbShortTitle(string);
                }
            }

            Int32 resourceId;
            sa->GetResourceId(R::styleable::PreferenceHeader_icon, 0, &resourceId);
            header->SetIconRes(resourceId);
            String fragment;
            sa->GetString(R::styleable::PreferenceHeader_fragment, &fragment);
            header->SetFragment(fragment);
            sa->Recycle();

            if (curBundle == NULL) {
                CBundle::New((IBundle**)&curBundle);
            }

            Int32 innerDepth;
            parser->GetDepth(&innerDepth);
            while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT
                   && type != IXmlPullParser::END_TAG) || (parser->GetDepth(&depth), depth > innerDepth)) {
                if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
                    continue;
                }

                String innerNodeName;
                parser->GetName(&innerNodeName);
                if (innerNodeName.Equals("extra")) {
                    resource->ParseBundleExtra(String("extra"), attrs, curBundle);
                    XmlUtils::SkipCurrentTag(parser);

                }
                else if (innerNodeName.Equals("intent")) {
                    AutoPtr<IIntentHelper> helper;
                    CIntentHelper::AcquireSingleton((IIntentHelper**)&helper);
                    AutoPtr<IIntent> intent;
                    helper->ParseIntent(resource, parser, attrs, (IIntent**)&intent);
                    header->SetIntent(intent);
                }
                else {
                    XmlUtils::SkipCurrentTag(parser);
                }
            }

            if (curBundle->GetSize(&size), size > 0) {
                header->SetFragmentArguments(curBundle);
                curBundle = NULL;
            }

            target->Add(header);
        }
        else {
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    return NOERROR;
    // } catch (XmlPullParserException e) {
    //     throw new RuntimeException("Error parsing headers", e);
    // } catch (IOException e) {
    //     throw new RuntimeException("Error parsing headers", e);
    // } finally {
    //     if (parser != null) parser.close();
    // }
}

ECode PreferenceActivity::IsValidFragment(
    /* [in] */ String fragmentName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IApplicationInfo> info;
    Activity::GetApplicationInfo((IApplicationInfo**)&info);
    Int32 targetSdkVersion;
    info->GetTargetSdkVersion(&targetSdkVersion);
    if (targetSdkVersion  >= android.os.Build.VERSION_CODES.KITKAT) {
        // throw new RuntimeException(
        //         "Subclasses of PreferenceActivity must override isValidFragment(String)"
        //         + " to verify that the Fragment class is valid! " + this.getClass().getName()
        //         + " has not checked if fragment " + fragmentName + " is valid.");
        return E_RUNTIME_EXCEPTION;
    } else {
        *result= TRUE;
        return NOERROR;
    }
}

ECode PreferenceActivity::SetListFooter(
    /* [in] */ IView* view)
{
    mListFooter->RemoveAllViews();
    // AutoPtr<IViewGroupLayoutParams> layoutParams;
    // CFrameLayoutLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
    //         IViewGroupLayoutParams::WRAP_CONTENT, (IFrameLayoutLayoutParams**)&layoutParams);
    // return mListFooter->AddView(view, layoutParams);
    return NOERROR;
}

ECode PreferenceActivity::OnStop()
{
    ListActivity::OnStop();
    if (mPreferenceManager != NULL) {
        mPreferenceManager->DispatchActivityStop();
    }
    return NOERROR;
}

ECode PreferenceActivity::OnDestroy()
{
    mHandler->RemoveMessages(MSG_BIND_PREFERENCES);
    mHandler->RemoveMessages(MSG_BUILD_HEADERS);
    ListActivity::OnDestroy();
    if (mPreferenceManager != NULL) {
        mPreferenceManager->DispatchActivityDestroy();
    }
    return NOERROR;
}

ECode PreferenceActivity::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    ListActivity::OnSaveInstanceState(outState);

    if (mHeaders.Begin() != mHeaders.End()) {
        AutoPtr<IObjectContainer> container;
        CObjectContainer::New((IObjectContainer**)&container);
        List<AutoPtr<IPreferenceActivityHeader> >::Iterator iter = mHeaders.Begin();
        for (; iter != mHeaders.End(); ++iter) {
            container->Add(*iter);
        }
        outState->PutParcelableArrayList(HEADERS_TAG, container);
        if (mCurHeader != NULL) {
            Int32 index = 0;
            for (iter = mHeaders.Begin(); iter != mHeaders.End(); ++index, ++iter) {
                if (mCurHeader == *iter) break;
            }
            if (iter != mHeaders.End()) {
                outState->PutInt32(CUR_HEADER_TAG, index);
            }
        }
    }

    if (mPreferenceManager != NULL) {
        AutoPtr<IPreferenceScreen> preferenceScreen;
        GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
        if (preferenceScreen != NULL) {
            AutoPtr<IBundle> container;
            CBundle::New((IBundle**)&container);
            IPreference::Probe(preferenceScreen)->SaveHierarchyState(container);
            outState->PutBundle(PREFERENCES_TAG, container);
        }
    }

    return NOERROR;
}

ECode PreferenceActivity::OnRestoreInstanceState(
    /* [in] */ IBundle* outState)
{
    if (mPreferenceManager != NULL) {
        AutoPtr<IBundle> container;
        outState->GetBundle(PREFERENCES_TAG, (IBundle**)&container);
        if (container != NULL) {
            AutoPtr<IPreferenceScreen> preferenceScreen;
            GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
            if (preferenceScreen != NULL) {
                IPreference::Probe(preferenceScreen)->RestoreHierarchyState(container);
                mSavedInstanceState = outState;
                return NOERROR;
            }
        }
    }

    // Only call this if we didn't save the instance state for later.
    // If we did save it, it will be restored when we bind the adapter.
    return ListActivity::OnRestoreInstanceState(outState);
}

ECode PreferenceActivity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    ListActivity::OnActivityResult(requestCode, resultCode, data);
    if (mPreferenceManager != NULL) {
        mPreferenceManager->DispatchActivityResult(requestCode, resultCode, data);
    }
    return NOERROR;
}

ECode PreferenceActivity::OnContentChanged()
{
    ListActivity::OnContentChanged();
    if (mPreferenceManager != NULL) {
        PostBindPreferences();
    }
    return NOERROR;
}

ECode PreferenceActivity::OnListItemClick(
    /* [in] */ IListView* l,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean isResumed = FALSE;
    Activity::IsResumed(&isResumed);
    if (!isResumed) {
        return E_NULL_POINTER;
    }
    ListActivity::OnListItemClick(l, v, position, id);
    if (mAdapter != NULL) {
        AutoPtr<IInterface> item;
        mAdapter->GetItem(position, (IInterface**)&item);
        AutoPtr<IPreferenceActivityHeader> header = IPreferenceActivityHeader::Probe(item);
        if (header) {
            OnHeaderClick(header, position);
        }
    }
    return NOERROR;
}

ECode PreferenceActivity::OnHeaderClick(
    /* [in] */ IPreferenceActivityHeader* header,
    /* [in] */ Int32 position)
{
    String fragment;
    header->GetFragment(&fragment);
    AutoPtr<IIntent> intent;
    if (!fragment.IsNull()) {
        if (mSinglePane) {
            Int32 titleRes;
            header->GetBreadCrumbTitleRes(&titleRes);
            Int32 shortTitleRes;
            header->GetBreadCrumbShortTitleRes(&shortTitleRes);
            if (titleRes == 0) {
                header->GetTitleRes(&titleRes);
                shortTitleRes = 0;
            }
            AutoPtr<IBundle> fragmentArguments;
            header->GetFragmentArguments((IBundle**)&fragmentArguments);
            StartWithFragment(fragment, fragmentArguments, NULL, 0, titleRes, shortTitleRes);
        }
        else {
            SwitchToHeader(header);
        }
    }
    else if (header->GetIntent((IIntent**)&intent), intent != NULL) {
        StartActivity(intent);
    }

    return NOERROR;
}

ECode PreferenceActivity::OnBuildStartFragmentIntent(
    /* [in] */ const String& fragmentName,
    /* [in] */ IBundle* args,
    /* [in] */ Int32 titleRes,
    /* [in] */ Int32 shortTitleRes,
    /* [out] */ IIntent** _intent)
{
    VALIDATE_NOT_NULL(_intent)

    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_MAIN, (IIntent**)&intent);

    ClassID id;
    GetClassID(&id);
    intent->SetClass(THIS_PROBE(IContext), id);
    intent->PutStringExtra(EXTRA_SHOW_FRAGMENT, fragmentName);
    intent->PutBundleExtra(EXTRA_SHOW_FRAGMENT_ARGUMENTS, args);
    intent->PutInt32Extra(EXTRA_SHOW_FRAGMENT_TITLE, titleRes);
    intent->PutInt32Extra(EXTRA_SHOW_FRAGMENT_SHORT_TITLE, shortTitleRes);
    intent->PutBooleanExtra(EXTRA_NO_HEADERS, TRUE);

    *_intent = intent;
    REFCOUNT_ADD(*_intent)
    return NOERROR;
}

ECode PreferenceActivity::StartWithFragment(
    /* [in] */ const String& fragmentName,
    /* [in] */ IBundle* args,
    /* [in] */ IFragment* resultTo,
    /* [in] */ Int32 resultRequestCode)
{
    return StartWithFragment(fragmentName, args, resultTo, resultRequestCode, 0, 0);;
}

ECode PreferenceActivity::StartWithFragment(
    /* [in] */ const String& fragmentName,
    /* [in] */ IBundle* args,
    /* [in] */ IFragment* resultTo,
    /* [in] */ Int32 resultRequestCode,
    /* [in] */ Int32 titleRes,
    /* [in] */ Int32 shortTitleRes)
{
    AutoPtr<IIntent> intent;
    OnBuildStartFragmentIntent(fragmentName, args, titleRes, shortTitleRes, (IIntent**)&intent);
    if (resultTo == NULL) {
        return StartActivity(intent);
    }
    else {
        return resultTo->StartActivityForResult(intent, resultRequestCode);
    }
}

ECode PreferenceActivity::ShowBreadCrumbs(
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* shortTitle)
{
    if (mFragmentBreadCrumbs == NULL) {
        AutoPtr<IView> crumbs = Activity::FindViewById(R::id::title);
        // For screens with a different kind of title, don't create breadcrumbs.
        mFragmentBreadCrumbs = IFragmentBreadCrumbs::Probe(crumbs);
        if (mFragmentBreadCrumbs == NULL) {
            SetTitle(title);
            return E_NULL_POINTER;
        }
        if (mFragmentBreadCrumbs == NULL) {
            if (title != NULL) {
                SetTitle(title);
            }
            return E_NULL_POINTER;
        }
        if (mSinglePane) {
            mFragmentBreadCrumbs->SetVisibility(IView::GONE);
            // Hide the breadcrumb section completely for single-pane
            AutoPtr<IView> bcSection = Activity::FindViewById(R::id::breadcrumb_section);
            if (bcSection != NULL) {
                bcSection->SetVisibility(IView::GONE);
            }
            SetTitle(title);
        }
        mFragmentBreadCrumbs->SetMaxVisible(2);
        mFragmentBreadCrumbs->SetActivity(THIS_PROBE(IActivity));
    }
    Boolean res = FALSE;
    mFragmentBreadCrumbs->GetVisibility(&res);
    if (res != IView::VISIBLE) {
        SetTitle(title);
    } else {
        mFragmentBreadCrumbs->SetTitle(title, shortTitle);
        mFragmentBreadCrumbs->SetParentTitle(NULL, NULL, NULL);
    }
    return NOERROR;
}

ECode PreferenceActivity::SetParentTitle(
    /* [in] */ ICharSequence* title,
    /* [in] */ ICharSequence* shortTitle,
    /* [in] */ IViewOnClickListener* listener)
{
    if (mFragmentBreadCrumbs != NULL) {
        return mFragmentBreadCrumbs->SetParentTitle(title, shortTitle, listener);
    }
    return NOERROR;
}

void PreferenceActivity::SetSelectedHeader(
    /* [in] */ IPreferenceActivityHeader* header)
{
    mCurHeader = header;
    AutoPtr<IPreferenceActivityHeader> temp(header);
    Int32 index = 0;
    List<AutoPtr<IPreferenceActivityHeader> >::Iterator itr = mHeaders.Begin();
    for (; itr != mHeaders.End(); ++index, ++itr) {
        if (header == *itr) break;
    }
    AutoPtr<IListView> listView;
    GetListView((IListView**)&listView);
    if (itr != mHeaders.End()) {
        listView->SetItemChecked(index, TRUE);
    }
    else {
        listView->ClearChoices();
    }
    ShowBreadCrumbs(header);
}

void PreferenceActivity::ShowBreadCrumbs(
    /* [in] */ IPreferenceActivityHeader* header)
{
    if (header != NULL) {
        AutoPtr<IResources> resource;
        GetResources((IResources**)&resource);
        AutoPtr<ICharSequence> title;
        header->GetBreadCrumbTitle(resource, (ICharSequence**)&title);
        if (title == NULL) {
            header->GetTitle(resource, (ICharSequence**)&title);
        }
        if (title == NULL) {
            GetTitle((ICharSequence**)&title);
        }
        AutoPtr<ICharSequence> breadCrumbShortTitle;
        header->GetBreadCrumbShortTitle(resource, (ICharSequence**)&breadCrumbShortTitle);
        ShowBreadCrumbs(title, breadCrumbShortTitle);
    }
    else {
        AutoPtr<ICharSequence> title;
        GetTitle((ICharSequence**)&title);
        ShowBreadCrumbs(title, NULL);
    }
}

void PreferenceActivity::SwitchToHeaderInner(
    /* [in] */ const String& fragmentName,
    /* [in] */ IBundle* args)
{
    AutoPtr<IFragmentManager> manager;
    GetFragmentManager((IFragmentManager**)&manager);
    manager->PopBackStack(BACK_STACK_PREFS, IFragmentManager::POP_BACK_STACK_INCLUSIVE);

    Boolean result = FALSE;
    IsValidFragment(fragmentName, &result);
    if (!result) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IFragment> f;
    Fragment::Instantiate(THIS_PROBE(IActivity), fragmentName, args, (IFragment**)&f);
    AutoPtr<IFragmentTransaction> transaction;
    manager->BeginTransaction((IFragmentTransaction**)&transaction);
    transaction->SetTransition(IFragmentTransaction::TRANSIT_FRAGMENT_FADE);
    transaction->Replace(R::id::prefs, f);
    Int32 res;
    transaction->CommitAllowingStateLoss(&res);
}

ECode PreferenceActivity::SwitchToHeader(
    /* [in] */ const String& fragmentName,
    /* [in] */ IBundle* args)
{
    AutoPtr<IPreferenceActivityHeader> selectedHeader = NULL;
    List<AutoPtr<IPreferenceActivityHeader> >::Iterator it;
    for (it = mHeaders.Begin(); it != mHeaders.End(); ++it) {
        String fragment;
        it->GetFragment(&fragment);
        if (fragmentName.Equals(fragment)) {
            selectedHeader = *it;
            break;
        }
    }
    SetSelectedHeader(selectedHeader);
    SwitchToHeaderInner(fragmentName, args);
    return NOERROR;
}

ECode PreferenceActivity::SwitchToHeader(
    /* [in] */ IPreferenceActivityHeader* header)
{
    if (mCurHeader.Get() == header) {
        // This is the header we are currently displaying.  Just make sure
        // to pop the stack up to its root state.
        AutoPtr<IFragmentManager> manager;
        GetFragmentManager((IFragmentManager**)&manager);
        manager->PopBackStack(BACK_STACK_PREFS, IFragmentManager::POP_BACK_STACK_INCLUSIVE);
    }
    else {
        String fragment;
        header->GetFragment(&fragment);
        AutoPtr<IBundle> fragmentArguments;
        header->GetFragmentArguments((IBundle**)&fragmentArguments);
        if (fragment.IsNull()) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        SwitchToHeaderInner(fragment, fragmentArguments);
        SetSelectedHeader(header);
    }

    return NOERROR;
}

AutoPtr<IPreferenceActivityHeader> PreferenceActivity::FindBestMatchingHeader(
    /* [in] */ IPreferenceActivityHeader* cur,
    /* [in] */ List<AutoPtr<IPreferenceActivityHeader> >& from)
{
    List<AutoPtr<IPreferenceActivityHeader> > matches;
    List<AutoPtr<IPreferenceActivityHeader> >::Iterator it = from.Begin();
    for (; it != from.End(); ++it) {
        AutoPtr<IPreferenceActivityHeader> lOh = *it;
        Int64 curId;
        cur->GetId(&curId);
        Int64 ohId;
        lOh->GetId(&ohId);
        if (cur == lOh || (curId != HEADER_ID_UNDEFINED && curId == ohId)) {
            // Must be this one.
            matches.Clear();
            matches.PushBack(lOh);
            break;
        }
        String curFragment;
        AutoPtr<IIntent> curIntent;
        AutoPtr<ICharSequence> curTitle;
        if (cur->GetFragment(&curFragment), !curFragment.IsNull()) {
            String ohFragment;
            lOh->GetFragment(&ohFragment);
            if (curFragment.Equals(ohFragment)) {
                matches.PushBack(lOh);
            }
        }
        else if (cur->GetIntent((IIntent**)&curIntent), curIntent != NULL) {
            AutoPtr<IIntent> ohIntent;
            lOh->GetIntent((IIntent**)&ohIntent);
            AutoPtr<IObject> obj = IObject::Probe(curIntent);
            Boolean result;
            if (obj->Equals(ohIntent, &result), result) {
                matches.PushBack(lOh);
            }
        }
        else if (cur->GetTitle((ICharSequence**)&curTitle), curTitle != NULL) {
            AutoPtr<ICharSequence> ohTitle;
            lOh->GetTitle((ICharSequence**)&ohTitle);
            AutoPtr<IObject> obj = IObject::Probe(curTitle);
            Boolean result;
            if (obj->Equals(ohTitle, &result), result) {
                matches.PushBack(lOh);
            }
        }
    }
    it = matches.Begin();
    if (it != matches.End()) {
        ++it;
        if (it == matches.End()) {
            return *(matches.Begin());
        }
        else {
            for (it = matches.Begin(); it != matches.End(); ++it) {
                AutoPtr<IPreferenceActivityHeader> oh = *it;
                AutoPtr<IBundle> curFragmentArguments;
                cur->GetFragmentArguments((IBundle**)&curFragmentArguments);
                if (curFragmentArguments != NULL) {
                    AutoPtr<IBundle> ohFragmentArguments;
                    oh->GetFragmentArguments((IBundle**)&ohFragmentArguments);
                    AutoPtr<IObject> obj = IObject::Probe(curFragmentArguments);
                    Boolean result;
                    if (obj->Equals(ohFragmentArguments, &result), result) {
                        return oh;
                    }
                }
                AutoPtr<IBundle> curExtras;
                cur->GetExtras((IBundle**)&curExtras);
                if (curExtras != NULL) {
                    AutoPtr<IBundle> ohExtras;
                    oh->GetExtras((IBundle**)&ohExtras);
                    AutoPtr<IObject> obj = IObject::Probe(curExtras);
                    Boolean result;
                    if (obj->Equals(ohExtras, &result), result) {
                        return oh;
                    }
                }
                AutoPtr<ICharSequence> curTitle;
                cur->GetTitle((ICharSequence**)&curTitle);
                if (curTitle != NULL) {
                    AutoPtr<ICharSequence> ohTitle;
                    oh->GetTitle((ICharSequence**)&ohTitle);
                    AutoPtr<IObject> obj = IObject::Probe(curTitle);
                    Boolean result;
                    if (obj->Equals(ohTitle, &result), result) {
                        return oh;
                    }
                }
            }
        }
    }

    return NULL;
}

ECode PreferenceActivity::StartPreferenceFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Boolean push)
{
    AutoPtr<IFragmentManager> manager;
    GetFragmentManager((IFragmentManager**)&manager);
    AutoPtr<IFragmentTransaction> transaction;
    manager->BeginTransaction((IFragmentTransaction**)&transaction);
    transaction->Replace(R::id::prefs, fragment);
    if (push) {
        transaction->SetTransition(IFragmentTransaction::TRANSIT_FRAGMENT_OPEN);
        transaction->AddToBackStack(BACK_STACK_PREFS);
    }
    else {
        transaction->SetTransition(IFragmentTransaction::TRANSIT_FRAGMENT_FADE);
    }
    Int32 res;
    transaction->CommitAllowingStateLoss(&res);

    return NOERROR;
}

ECode PreferenceActivity::StartPreferencePanel(
    /* [in] */ const String& fragmentClass,
    /* [in] */ IBundle* args,
    /* [in] */ Int32 titleRes,
    /* [in] */ ICharSequence* titleText,
    /* [in] */ IFragment* resultTo,
    /* [in] */ Int32 resultRequestCode)
{
    if (mSinglePane) {
        StartWithFragment(fragmentClass, args, resultTo, resultRequestCode, titleRes, 0);
    }
    else {
        AutoPtr<IFragment> f;
        Fragment::Instantiate(THIS_PROBE(IActivity), fragmentClass, args, (IFragment**)&f);
        if (resultTo != NULL) {
            f->SetTargetFragment(resultTo, resultRequestCode);
        }
        AutoPtr<IFragmentManager> manager;
        GetFragmentManager((IFragmentManager**)&manager);
        AutoPtr<IFragmentTransaction> transaction;
        manager->BeginTransaction((IFragmentTransaction**)&transaction);
        transaction->Replace(R::id::prefs, f);
        if (titleRes != 0) {
            transaction->SetBreadCrumbTitle(titleRes);
        }
        else if (titleText != NULL) {
            transaction->SetBreadCrumbTitle(titleText);
        }
        transaction->SetTransition(IFragmentTransaction::TRANSIT_FRAGMENT_OPEN);
        transaction->AddToBackStack(BACK_STACK_PREFS);
        Int32 res;
        transaction->CommitAllowingStateLoss(&res);
    }

    return NOERROR;
}

ECode PreferenceActivity::FinishPreferencePanel(
    /* [in] */ IFragment* caller,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    if (mSinglePane) {
        SetResult(resultCode, resultData);
        Finish();
    }
    else {
        // XXX be smarter about popping the stack.
        OnBackPressed();
        if (caller != NULL) {
            AutoPtr<IFragment> targetFragment;
            caller->GetTargetFragment((IFragment**)&targetFragment);
            if (targetFragment != NULL) {
                Int32 targetRequestCode;
                caller->GetTargetRequestCode(&targetRequestCode);
                targetFragment->OnActivityResult(targetRequestCode, resultCode, resultData);
            }
        }
    }

    return NOERROR;
}

ECode PreferenceActivity::OnPreferenceStartFragment(
    /* [in] */ IPreferenceFragment* caller,
    /* [in] */ IPreference* pref,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    String fragment;
    pref->GetFragment(&fragment);
    AutoPtr<IBundle> extras;
    pref->GetExtras((IBundle**)&extras);
    Int32 titleRes;
    pref->GetTitleRes(&titleRes);
    AutoPtr<ICharSequence> title;
    pref->GetTitle((ICharSequence**)&title);
    StartPreferencePanel(fragment, extras, titleRes, title, NULL, 0);
    *result = TRUE;
    return NOERROR;
}

void PreferenceActivity::PostBindPreferences()
{
    Boolean hasMessages;
    if (mHandler->HasMessages(MSG_BIND_PREFERENCES, &hasMessages), !hasMessages) {
        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_BIND_PREFERENCES, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

void PreferenceActivity::BindPreferences()
{
    AutoPtr<IPreferenceScreen> preferenceScreen;
    GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
    if (preferenceScreen != NULL) {
        AutoPtr<IListView> listView;
        GetListView((IListView**)&listView);
        preferenceScreen->Bind(listView);
        if (mSavedInstanceState != NULL) {
            ListActivity::OnRestoreInstanceState(mSavedInstanceState);
            mSavedInstanceState = NULL;
        }
    }
}

ECode PreferenceActivity::GetPreferenceManager(
    /* [out] */ IPreferenceManager** manager)
{
    VALIDATE_NOT_NULL(manager)
    *manager = mPreferenceManager;
    REFCOUNT_ADD(*manager)
    return NOERROR;
}

ECode PreferenceActivity::RequirePreferenceManager()
{
    if (mPreferenceManager == NULL) {
        if (mAdapter == NULL) {
            Logger::E("PreferenceActivity", "This should be called after super.onCreate.");
            return E_RUNTIME_EXCEPTION;
        }

        Logger::E("PreferenceActivity", "Modern two-pane PreferenceActivity requires use of a PreferenceFragment");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode PreferenceActivity::SetPreferenceScreen(
    /* [in] */ IPreferenceScreen* preferenceScreen)
{
    FAIL_RETURN(RequirePreferenceManager())

    Boolean result;
    if ((mPreferenceManager->SetPreferences(preferenceScreen, &result), result) && preferenceScreen != NULL) {
        PostBindPreferences();
        AutoPtr<IPreferenceScreen> preferenceScreen;
        GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
        AutoPtr<ICharSequence> title;
        IPreference::Probe(preferenceScreen)->GetTitle((ICharSequence**)&title);
        // Set the title of the activity
        if (title != NULL) {
            SetTitle(title);
        }
    }
    return NOERROR;
}

ECode PreferenceActivity::GetPreferenceScreen(
    /* [out] */ IPreferenceScreen** screen)
{
    VALIDATE_NOT_NULL(screen)
    if (mPreferenceManager != NULL) {
        AutoPtr<IPreferenceScreen> s;
        mPreferenceManager->GetPreferenceScreen((IPreferenceScreen**)&s);
        *screen = s;
        REFCOUNT_ADD(*screen)
        return NOERROR;
    }
    *screen = NULL;
    return NOERROR;
}

ECode PreferenceActivity::AddPreferencesFromIntent(
    /* [in] */ IIntent* intent)
{
    FAIL_RETURN(RequirePreferenceManager())

    AutoPtr<IPreferenceScreen> preferenceScreen;
    GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
    AutoPtr<IPreferenceScreen> preScreen;
    mPreferenceManager->InflateFromIntent(intent, preferenceScreen, (IPreferenceScreen**)&preScreen);
    SetPreferenceScreen(preScreen);

    return NOERROR;
}

ECode PreferenceActivity::AddPreferencesFromResource(
    /* [in] */ Int32 preferencesResId)
{
    FAIL_RETURN(RequirePreferenceManager())

    AutoPtr<IPreferenceScreen> preferenceScreen;
    GetPreferenceScreen((IPreferenceScreen**)&preferenceScreen);
    AutoPtr<IPreferenceScreen> preScreen;
    mPreferenceManager->InflateFromResource(THIS_PROBE(IContext), preferencesResId, preferenceScreen, (IPreferenceScreen**)&preScreen);
    SetPreferenceScreen(preScreen);
    return NOERROR;
}

ECode PreferenceActivity::OnPreferenceTreeClick(
    /* [in] */ IPreferenceScreen* preferenceScreen,
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode PreferenceActivity::FindPreference(
    /* [in] */ const String& key,
    /* [out] */ IPreference** preference)
{
    VALIDATE_NOT_NULL(preference)
    if (mPreferenceManager == NULL) {
        *preference = NULL;
        return NOERROR;
    }

    AutoPtr<ICharSequence> cs;
    CString::New(key, (ICharSequence**)&cs);
    return mPreferenceManager->FindPreference(cs, preference);
}

ECode PreferenceActivity::OnNewIntent(
    /* [in] */ IIntent* intent)
{
    if (mPreferenceManager != NULL) {
        mPreferenceManager->DispatchNewIntent(intent);
    }

    return NOERROR;
}

Boolean PreferenceActivity::HasNextButton()
{
    return mNextButton != NULL;
}

AutoPtr<IButton> PreferenceActivity::GetNextButton()
{
    return mNextButton;
}

} // Preference
} // Droid
} // Elastos
