
#include "app/ListActivity.h"
#include "R.h"
#ifdef DROID_CORE
#include "os/CHandler.h"
#endif

using Elastos::Droid::Os::CHandler;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace App {

//====================================================
// ListActivity::MyListener
//====================================================

ListActivity::MyListener::MyListener(
    /* [in] */ ListActivity* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(ListActivity::MyListener, IAdapterViewOnItemClickListener)

ECode ListActivity::MyListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return mHost->OnListItemClick(IListView::Probe(parent), v, position, id);
}


//====================================================
// ListActivity::RequestRunnable
//====================================================

ListActivity::RequestRunnable::RequestRunnable(
    /* [in] */ ListActivity* host)
    : mHost(host)
{}

ECode ListActivity::RequestRunnable::Run()
{
    return (IViewParent::Probe(mHost->mList))->FocusableViewAvailable(mHost->mList);
}


//====================================================
// ListActivity
//====================================================

ListActivity::ListActivity()
    : mFinishedStart(FALSE)
{
    CHandler::New((IHandler**)&mHandler);
    mRequestFocus = new RequestRunnable(this);
    mOnClickListener = new MyListener(this);
}

ListActivity::~ListActivity()
{}

PInterface ListActivity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IListActivity) {
        return (IListActivity*)this;
    }
    return Activity::Probe(riid);
}

UInt32 ListActivity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ListActivity::Release()
{
    return ElRefBase::Release();
}

ECode ListActivity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface *)(IListActivity *)this) {
        *pIID = EIID_IListActivity;
        return NOERROR;
    }
    return ListActivity::GetInterfaceID(pObject, pIID);
}

ECode ListActivity::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    return Activity::GetBaseContext(ctx);
}

ECode ListActivity::ApplyOverrideConfiguration(
    /* [in] */ IConfiguration* overrideConfiguration)
{
    return Activity::ApplyOverrideConfiguration(overrideConfiguration);
}

ECode ListActivity::OnContentChanged()
{
    Activity::OnContentChanged();
    AutoPtr<IView> emptyView = Activity::FindViewById(R::id::empty);
    mList = IListView::Probe(Activity::FindViewById(R::id::list));
    if (mList == NULL) {
        // throw new RuntimeException(
        //         "Your content must have a ListView whose id attribute is " +
        //         "'android.R.id.list'");
        return E_RUNTIME_EXCEPTION;
    }
    if (emptyView != NULL) {
        mList->SetEmptyView(emptyView);
    }
    mList->SetOnItemClickListener(mOnClickListener);
    if (mFinishedStart) {
        SetListAdapter(mAdapter);
    }

    Boolean res = FALSE;
    mHandler->Post(mRequestFocus, &res);
    mFinishedStart = TRUE;
    return NOERROR;
}

ECode ListActivity::SetListAdapter(
    /* [in] */ IListAdapter* adapter)
{
    Mutex::Autolock lock(mLock);
    EnsureList();
    mAdapter = adapter;
    return mList->SetAdapter(adapter);
}

ECode ListActivity::SetSelection(
    /* [in] */ Int32 position)
{
    return mList->SetSelection(position);
}

ECode ListActivity::GetSelectedItemPosition(
    /* [our] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos)
    return mList->GetSelectedItemPosition(pos);
}

ECode ListActivity::GetSelectedItemId(
    /* [our] */ Int64* id)
{
    VALIDATE_NOT_NULL(id)
    return mList->GetSelectedItemId(id);
}

ECode ListActivity::GetListView(
    /* [out] */ IListView** listView)
{
    VALIDATE_NOT_NULL(listView)
    EnsureList();
    *listView = mList;
    INTERFACE_ADDREF(*listView)
    return NOERROR;
}

ECode ListActivity::GetListAdapter(
    /* [out] */ IListAdapter** listAdapter)
{
    VALIDATE_NOT_NULL(listAdapter)
    *listAdapter = mAdapter;
    INTERFACE_ADDREF(*listAdapter)
    return NOERROR;
}

ECode ListActivity::OnListItemClick(
    /* [in] */ IListView* l,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return NOERROR;
}

ECode ListActivity::OnRestoreInstanceState(
    /* [in] */ IBundle* state)
{
    EnsureList();
    return Activity::OnRestoreInstanceState(state);
}

ECode ListActivity::OnDestroy()
{
    mHandler->RemoveCallbacks(mRequestFocus);
    return Activity::OnDestroy();
}

void ListActivity::EnsureList()
{
    if (mList != NULL) {
        return;
    }
    Activity::SetContentView(R::layout::list_content_simple);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
