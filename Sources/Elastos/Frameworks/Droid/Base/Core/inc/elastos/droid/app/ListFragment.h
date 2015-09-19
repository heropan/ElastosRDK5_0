
#ifndef __ELASTOS_DROID_APP_LISTGRAGMENT_H__
#define __ELASTOS_DROID_APP_LISTGRAGMENT_H__

#include "app/Fragment.h"
#include "os/Runnable.h"
#ifdef DROID_CORE
#include "os/CHandler.h"
#endif

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::App::Fragment;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::EIID_IListView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace App {

class ListFragment
    : public Fragment
    , public IListFragment
{
public:
    class _Runnable
        : public Runnable
    {
    public:
        _Runnable(
            /* [in] */ ListFragment* host)
            : mHost(host)
        {}

        CARAPI Run();

    private:
        ListFragment* mHost;
    };

    class _OnItemClickListener
        : public ElRefBase
        , public IAdapterViewOnItemClickListener
    {
    public:
        _OnItemClickListener(
            /* [in] */ ListFragment* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL();

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        ListFragment* mHost;
    };

public:
    ListFragment();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI OnCreateView(
        /* [in] */ ILayoutInflater* inflater,
        /* [in] */ IViewGroup* container,
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IView** view);

    CARAPI OnViewCreated(
        /* [in] */ IView* view,
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnDestroyView();

    CARAPI OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI SetListAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI SetSelection(
        /* [in] */ Int32 position);

    CARAPI GetSelectedItemPosition(
        /* [out] */ Int32* position);

    CARAPI GetSelectedItemId(
        /* [out] */ Int64* id);

    CARAPI GetListView(
        /* [out] */ IListView** listview);

    CARAPI SetEmptyText(
        /* [in] */ ICharSequence* text);

    CARAPI SetListShown(
        /* [in] */ Boolean shown);

    CARAPI SetListShownNoAnimation(
        /* [in] */ Boolean shown);

    CARAPI GetListAdapter(
        /* [out] */ IListAdapter** listadapter);

private:
    CARAPI_(void) SetListShown(
        /* [in] */ Boolean shown,
        /* [in] */ Boolean animate);

    CARAPI_(void) EnsureList();

public:
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<IListView> mList;
    AutoPtr<IView> mEmptyView;
    AutoPtr<ITextView> mStandardEmptyView;
    AutoPtr<IView> mProgressContainer;
    AutoPtr<IView> mListContainer;
    AutoPtr<ICharSequence> mEmptyText;
    Boolean mListShown;

private:
    AutoPtr<IHandler> mHandler;
    AutoPtr<_Runnable> mRequestFocus;
    AutoPtr<_OnItemClickListener> mOnClickListener;
};

} //namespace App
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_APP_LISTGRAGMENT_H__
