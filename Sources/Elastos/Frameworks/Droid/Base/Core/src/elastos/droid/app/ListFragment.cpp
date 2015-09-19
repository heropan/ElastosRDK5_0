
#include "app/ListFragment.h"
#include "R.h"
#ifdef DROID_CORE
#include "view/animation/CAnimationUtils.h"
#endif

#include <elastos/utility/logging/Slogger.h>
using Elastos::Utility::Logging::Slogger;

using Elastos::Droid::R;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::IAnimationUtils;
using Elastos::Droid::View::Animation::CAnimationUtils;

namespace Elastos {
namespace Droid {
namespace App {

ECode ListFragment::_Runnable::Run()
{
    ((IViewParent*)mHost->mList->Probe(EIID_IViewParent))->FocusableViewAvailable(mHost->mList);
    return NOERROR;
}

CAR_INTERFACE_IMPL(ListFragment::_OnItemClickListener, IAdapterViewOnItemClickListener);

ECode ListFragment::_OnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return mHost->OnListItemClick((IListView*)parent, view, position, id);
}

ListFragment::ListFragment()
    : mAdapter(NULL)
    , mList(NULL)
    , mEmptyView(NULL)
    , mStandardEmptyView(NULL)
    , mProgressContainer(NULL)
    , mListContainer(NULL)
    , mEmptyText(NULL)
    , mListShown(FALSE)
    , mRequestFocus(new _Runnable(this))
    , mOnClickListener(new _OnItemClickListener(this))
{
    CHandler::New((IHandler**)&mHandler);
}

PInterface ListFragment::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IListFragment*)this;
    }
    else if (riid == EIID_IListFragment) {
        return (IListFragment*)this;
    }

    return Fragment::Probe(riid);
}

UInt32 ListFragment::AddRef()
{
    return Fragment::AddRef();
}

UInt32 ListFragment::Release()
{
    return Fragment::Release();
}

ECode ListFragment::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IListFragment *)this) {
        *pIID = EIID_IListFragment;
        return NOERROR;
    }

    return Fragment::GetInterfaceID(pObject, pIID);
}

ECode ListFragment::OnCreateView(
    /* [in] */ ILayoutInflater* inflater,
    /* [in] */ IViewGroup* container,
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IView** view)
{
    return inflater->Inflate(R::layout::list_content,
            container, FALSE, view);
}

ECode ListFragment::OnViewCreated(
    /* [in] */ IView* view,
    /* [in] */ IBundle* savedInstanceState)
{
    Fragment::OnViewCreated(view, savedInstanceState);
    EnsureList();
    return NOERROR;
}

ECode ListFragment::OnDestroyView()
{
    mHandler->RemoveCallbacks(mRequestFocus);
    mList = NULL;
    mListShown = FALSE;
    mEmptyView = mProgressContainer = mListContainer = NULL;
    mStandardEmptyView = NULL;
    Fragment::OnDestroyView();
    return NOERROR;
}

ECode ListFragment::OnListItemClick(
    /* [in] */ IListView* l,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return NOERROR;
}

ECode ListFragment::SetListAdapter(
    /* [in] */ IListAdapter* adapter)
{
    Boolean hadAdapter = mAdapter != NULL;
    mAdapter = adapter;
    if (mList != NULL) {
        mList->SetAdapter(adapter);
        if (!mListShown && !hadAdapter) {
            // The list was hidden, and previously didn't have an
            // adapter.  It is now time to show it.
            AutoPtr<IView> view;
            GetView((IView**)&view);
            AutoPtr<IBinder> token;
            view->GetWindowToken((IBinder**)&token);
            SetListShown(TRUE, token != NULL);
        }
    }
    return NOERROR;
}

ECode ListFragment::SetSelection(
    /* [in] */ Int32 position)
{
    EnsureList();
    mList->SetSelection(position);
    return NOERROR;
}

ECode ListFragment::GetSelectedItemPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    EnsureList();
    mList->GetSelectedItemPosition(position);
    return NOERROR;
}

ECode ListFragment::GetSelectedItemId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    EnsureList();
    mList->GetSelectedItemId(id);
    return NOERROR;
}

ECode ListFragment::GetListView(
    /* [out] */ IListView** listview)
{
    VALIDATE_NOT_NULL(listview);

    EnsureList();
    *listview = mList;
    REFCOUNT_ADD(*listview);
    return NOERROR;
}

ECode ListFragment::SetEmptyText(
    /* [in] */ ICharSequence* text)
{
    EnsureList();
    if (mStandardEmptyView == NULL) {
        // throw new IllegalStateException("Can't be used with a custom content view");
        Slogger::E("ListFragment", "Can't be used with a custom content view");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mStandardEmptyView->SetText(text);
    if (mEmptyText == NULL) {
        mList->SetEmptyView(mStandardEmptyView);
    }
    mEmptyText = text;
    return NOERROR;
}

ECode ListFragment::SetListShown(
    /* [in] */ Boolean shown)
{
    SetListShown(shown, TRUE);
    return NOERROR;
}

ECode ListFragment::SetListShownNoAnimation(
    /* [in] */ Boolean shown)
{
    SetListShown(shown, FALSE);
    return NOERROR;
}

void ListFragment::SetListShown(
    /* [in] */ Boolean shown,
    /* [in] */ Boolean animate)
{
    EnsureList();
    if (mProgressContainer == NULL) {
        // throw new IllegalStateException("Can't be used with a custom content view");
        Slogger::E("ListFragment", "Can't be used with a custom content view");
        return;
    }
    if (mListShown == shown) {
        return;
    }
    AutoPtr<IAnimationUtils> aUtils;
    CAnimationUtils::AcquireSingleton((IAnimationUtils**)&aUtils);
    mListShown = shown;
    if (shown) {
        if (animate) {
            AutoPtr<IAnimation> aOut, aIn;
            aUtils->LoadAnimation(mActivity, R::anim::fade_out, (IAnimation**)&aOut);
            aUtils->LoadAnimation(mActivity, R::anim::fade_in, (IAnimation**)&aIn);
            mProgressContainer->StartAnimation(aOut);
            mListContainer->StartAnimation(aIn);
        } else {
            mProgressContainer->ClearAnimation();
            mListContainer->ClearAnimation();
        }
        mProgressContainer->SetVisibility(IView::GONE);
        mListContainer->SetVisibility(IView::VISIBLE);
    } else {
        if (animate) {
            AutoPtr<IAnimation> aOut, aIn;
            aUtils->LoadAnimation(mActivity, R::anim::fade_out, (IAnimation**)&aOut);
            aUtils->LoadAnimation(mActivity, R::anim::fade_in, (IAnimation**)&aIn);
            mProgressContainer->StartAnimation(aIn);
            mListContainer->StartAnimation(aOut);
        } else {
            mProgressContainer->ClearAnimation();
            mListContainer->ClearAnimation();
        }
        mProgressContainer->SetVisibility(IView::VISIBLE);
        mListContainer->SetVisibility(IView::GONE);
    }
}

ECode ListFragment::GetListAdapter(
    /* [out] */ IListAdapter** listadapter)
{
    VALIDATE_NOT_NULL(listadapter);

    *listadapter = mAdapter;
    REFCOUNT_ADD(*listadapter);
    return NOERROR;
}

void ListFragment::EnsureList()
{
    if (mList != NULL) {
        return;
    }
    AutoPtr<IView> root;
    GetView((IView**)&root);
    if (root == NULL) {
        // throw new IllegalStateException("Content view not yet created");
        Slogger::E("ListFragment", "Content view not yet created");
        return;
    }
    if (root->Probe(EIID_IListView)) {
        mList = (IListView*)root->Probe(EIID_IListView);
    } else {
        mStandardEmptyView = NULL;
        root->FindViewById(
                R::id::internalEmpty,
                (IView**)(ITextView**)&mStandardEmptyView);
        if (mStandardEmptyView == NULL) {
            mEmptyView = NULL;
            root->FindViewById(R::id::empty, (IView**)&mEmptyView);
        } else {
            mStandardEmptyView->SetVisibility(IView::GONE);
        }
        root->FindViewById(R::id::progressContainer,
                (IView**)&mProgressContainer);
        root->FindViewById(R::id::listContainer,
                (IView**)&mListContainer);
        AutoPtr<IView> rawListView;
        root->FindViewById(R::id::list, (IView**)&rawListView);
        if (!(rawListView->Probe(EIID_IListView))) {
            // throw new RuntimeException(
            //         "Content has view with id attribute 'android.R.id.list' "
            //         + "that is not a ListView class");
            Slogger::E("ListFragment", "Content has view with id attribute 'android.R.id.list' that is not a ListView class");
            return;
        }
        mList = (IListView*)rawListView->Probe(EIID_IListView);
        if (mList == NULL) {
            // throw new RuntimeException(
            //         "Your content must have a ListView whose id attribute is " +
            //         "'android.R.id.list'");
            Slogger::E("ListFragment", "Your content must have a ListView whose id attribute is 'android.R.id.list'");
            return;
        }
        if (mEmptyView != NULL) {
            mList->SetEmptyView(mEmptyView);
        } else if (mEmptyText != NULL) {
            mStandardEmptyView->SetText(mEmptyText);
            mList->SetEmptyView(mStandardEmptyView);
        }
    }
    mListShown = TRUE;
    mList->SetOnItemClickListener(mOnClickListener);
    if (mAdapter != NULL) {
        AutoPtr<IListAdapter> adapter = mAdapter;
        mAdapter = NULL;
        SetListAdapter(adapter);
    } else {
        // We are starting without an adapter, so assume we won't
        // have our data right away and start with the progress indicator.
        if (mProgressContainer != NULL) {
            SetListShown(FALSE, FALSE);
        }
    }
    Boolean result;
    mHandler->Post(mRequestFocus.Get(), &result);
}

} //namespace App
} //namespace Droid
} //namespace Elastos
