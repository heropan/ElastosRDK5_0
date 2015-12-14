#ifndef __ELASTOS_DROID_WIDGET_REMOTEVIEWSADAPTER_H__
#define __ELASTOS_DROID_WIDGET_REMOTEVIEWSADAPTER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/BaseAdapter.h"
#include "elastos/droid/widget/FrameLayout.h"
#include "elastos/droid/os/CHandlerThread.h"
#include "elastos/droid/os/Runnable.h"

#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::HashSet;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IHandlerCallback;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilterComparison;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;

namespace Elastos {
namespace Droid {
namespace Widget {

class RemoteViewsCacheKey
    : public Object
    , public IRemoteViewsCacheKey
{
public:
    RemoteViewsCacheKey(
        /* [in] */ IIntentFilterComparison* filter,
        /* [in] */ Int32 widgetId,
        /* [in] */ Int32 userId);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    AutoPtr<IIntentFilterComparison> mFilter;
    Int32 mWidgetId;
};

class RemoteViewsFrameLayout
    : public FrameLayout
    , public IRemoteViewsFrameLayout
{
public:
    CAR_INTERFCE_DECL()

    RemoteViewsFrameLayout();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI OnRemoteViewsLoaded(
        /* [in] */ IRemoteViews* view,
        /* [in] */ IRemoteViewsOnClickHandler* handler);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

_ETL_NAMESPACE_BEGIN
template<> struct Hash<AutoPtr<Elastos::Droid::Widget::RemoteViewsCacheKey> >
{
    size_t operator()(const AutoPtr<Elastos::Droid::Widget::RemoteViewsCacheKey> s) const
    {
        Int32 hashCode;
        s->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};
_ETL_NAMESPACE_END

#define HASH_FUNC_FOR_AUTOPTR_REMOTEVIEWFRAMELAYOUT
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Widget::RemoteViewsFrameLayout)
#endif

namespace Elastos {
namespace Droid {
namespace Widget {


class CRemoteViewsAdapterServiceConnection;
class RemoteViewsAdapter
    : public BaseAdapter
    , public IRemoteViewsAdapter
    , public IHandlerCallback
{
private:
    class RemoteViewsFrameLayoutRefSet
        : public Object
    {
    public:
        RemoteViewsFrameLayoutRefSet(
            /* [in] */ RemoteViewsAdapter* host);

        ~RemoteViewsFrameLayoutRefSet();

        CARAPI Add(
            /* [in] */ Int32 position,
            /* [in] */ RemoteViewsFrameLayout* layout);

        CARAPI NotifyOnRemoteViewsLoaded(
            /* [in] */ Int32 position,
            /* [in] */ IRemoteViews* view);

        CARAPI RemoveView(
            /* [in] */ RemoteViewsFrameLayout* rvfl);

        CARAPI Clear();

    private:
        typedef List<AutoPtr<RemoteViewsFrameLayout> > RemoteViewsFrameLayoutList;
        typedef typename RemoteViewsFrameLayoutList::Iterator RemoteViewsFrameLayoutListIterator;
        typedef HashMap<AutoPtr<RemoteViewsFrameLayout>, AutoPtr<RemoteViewsFrameLayoutList> > RemoteViewsFrameLayoutMap;
        typedef typename RemoteViewsFrameLayoutMap::Iterator RemoteViewsFrameLayoutMapIterator;

        RemoteViewsAdapter* mHost;
        HashMap<Int32, AutoPtr<RemoteViewsFrameLayoutList> > mReferences;
        RemoteViewsFrameLayoutMap mViewToLinkedList;
    };

    class RemoteViewsMetaData : public Object
    {
    public:
        RemoteViewsMetaData();

        CARAPI Set(
            /* [in] */ RemoteViewsMetaData* d);

        CARAPI Reset();

        CARAPI SetLoadingViewTemplates(
            /* [in] */ IRemoteViews* loadingView,
            /* [in] */ IRemoteViews* firstView);

        CARAPI_(Int32) GetMappedViewType(
            /* [in] */ Int32 typeId);

        CARAPI_(Boolean) IsViewTypeInRange(
            /* [in] */ Int32 typeId);

    private:
        CARAPI_(AutoPtr<RemoteViewsFrameLayout>) CreateLoadingView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [in] */ Object& lock,
            /* [in] */ ILayoutInflater* layoutInflater,
            /* [in] */ IRemoteViewsOnClickHandler* handler);
        friend class RemoteViewsAdapter;

    public:
        Int32 mCount;
        Int32 mViewTypeCount;
        Boolean mHasStableIds;
        // Used to determine how to construct loading views.  If a loading view is not specified
        // by the user, then we try and load the first view, and use its height as the height for
        // the default loading view.
        AutoPtr<IRemoteViews> mUserLoadingView;
        AutoPtr<IRemoteViews> mFirstView;
        Int32 mFirstViewHeight;

    private:
        Object mLock;
        HashMap<Int32, Int32> mTypeIdIndexMap;
    };

    class RemoteViewsIndexMetaData : public Object
    {
    public:
        RemoteViewsIndexMetaData(
            /* [in] */ IRemoteViews* v,
            /* [in] */ Int64 itemId);

        CARAPI Set(
            /* [in] */ IRemoteViews* v,
            /* [in] */ Int64 id);
    private:
        Int32 mTypeId;
        Int64 mItemId;
        friend class RemoteViewsAdapter;
    };

    class FixedSizeRemoteViewsCache : public Object
    {
    public:
        FixedSizeRemoteViewsCache(
            /* [in] */ Int32 maxCacheSize);

        CARAPI Insert(
            /* [in] */ Int32 position,
            /* [in] */ IRemoteViews* v,
            /* [in] */ Int64 itemId,
            /* [in] */ List<Int32> visibleWindow);

        CARAPI_(AutoPtr<RemoteViewsMetaData>) GetMetaData();

        CARAPI_(AutoPtr<RemoteViewsMetaData>) GetTemporaryMetaData();

        CARAPI_(AutoPtr<IRemoteViews>) GetRemoteViewsAt(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<RemoteViewsIndexMetaData>) GetMetaDataAt(
            /* [in] */ Int32 position);

        CARAPI CommitTemporaryMetaData();

        CARAPI_(Int32) GetRemoteViewsBitmapMemoryUsage();

        CARAPI_(Int32) GetFarthestPositionFrom(
            /* [in] */ Int32 pos,
            /* [in] */ List<Int32> visibleWindow);

        CARAPI QueueRequestedPositionToLoad(
            /* [in] */ Int32 position);

        CARAPI_(Boolean) QueuePositionsToBePreloadedFromRequestedPosition(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<ArrayOf<Int32> >) GetNextIndexToLoad();

        CARAPI_(Boolean) ContainsRemoteViewAt(
            /* [in] */ Int32 position);

        CARAPI_(Boolean) ContainsMetaDataAt(
            /* [in] */ Int32 position);

        CARAPI Reset();

    private:
        static const String TAG;
        static const Float sMaxCountSlackPercent = 0.75f;
        static const Int32 sMaxMemoryLimitInBytes = 2 * 1024 * 1024;
        AutoPtr<RemoteViewsMetaData> mMetaData;
        AutoPtr<RemoteViewsMetaData> mTemporaryMetaData;
        HashMap<Int32, AutoPtr<RemoteViewsIndexMetaData> > mIndexMetaData;
        HashMap<Int32, AutoPtr<IRemoteViews> > mIndexRemoteViews;
        HashSet<Int32> mRequestedIndices;
        Int32 mLastRequestedIndex;
        HashSet<Int32> mLoadIndices;
        Int32 mPreloadLowerBound;
        Int32 mPreloadUpperBound;
        Int32 mMaxCount;
        Int32 mMaxCountSlack;
        Object mTemporaryMetaDataLock;
        Object mMetaDataLock;
        Object mLoadIndicesLock;
        friend class RemoteViewsAdapter;
    };

    class InnerRunnable : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsCacheKey* host);

        CARAPI Run();
    private:
        AutoPtr<RemoteViewsCacheKey> mKey;
    };

    class InnerRunnableEx : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsAdapter* host);

        CARAPI Run();
    private:
        RemoteViewsAdapter* mHost;
    };

    class InnerRunnableEx2 : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsAdapter* host);

        CARAPI Run();
    private:
        RemoteViewsAdapter* mHost;
    };

    class InnerRunnableEx3 : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsAdapter* host);

        CARAPI Run();
    private:
        RemoteViewsAdapter* mHost;
    };

    class InnerRunnableEx4 : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsFrameLayoutRefSet* refSet,
            /* [in] */ Int32 position,
            /* [in] */ IRemoteViews* rv);

        CARAPI Run();
    private:
        AutoPtr<RemoteViewsFrameLayoutRefSet> mRefSet;
        Int32 mPosition;
        AutoPtr<IRemoteViews> mRv;
    };

    class InnerRunnableEx5 : public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ RemoteViewsAdapter* host);

        CARAPI Run();
    private:
        RemoteViewsAdapter* mHost;
    };

public:
    CAR_INTERFCE_DECL()

    RemoteViewsAdapter();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent,
        /* [in] */ IRemoteAdapterConnectionCallback* callback);

    ~RemoteViewsAdapter();

    CARAPI HandleMessage(
        /* [in] */ IMessage* msg,
        /* [out]*/ Boolean* result);

    CARAPI IsDataReady(
        /* [out]*/ Boolean* ready);

    CARAPI SetRemoteViewsOnClickHandler(
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    CARAPI SaveRemoteViewsCache();

    CARAPI GetRemoteViewsServiceIntent(
        /* [out] */ IIntent** intent);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetItem(IInterface
        /* [in] */ Int32 position,
        /* [out] */ IInterface** intent);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64 id);

    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32* count);

    CARAPI SetVisibleRangeHint(
        /* [in] */ Int32 lowerBound,
        /* [in] */ Int32 upperBound);

    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    CARAPI HasStableIds(
        /* [out]*/ Boolean* result);

    CARAPI IsEmpty(
        /* [out]*/ Boolean* result);

    CARAPI NotifyDataSetChanged();

    CARAPI SuperNotifyDataSetChanged();


private:
    CARAPI LoadNextIndexInBackground();

    CARAPI ProcessException(
        /* [in] */ const String& method);

    CARAPI UpdateTemporaryMetaData();

    CARAPI UpdateRemoteViews(
        /* [in] */ Int32 position,
        /* [in] */ Boolean notifyWhenLoaded);

    CARAPI_(Int32) GetConvertViewTypeId(
        /* [in] */ IView* convertView);

    CARAPI OnNotifyDataSetChanged();

    CARAPI_(List<Int32>) GetVisibleWindow(
        /* [in] */ Int32 lower,
        /* [in] */ Int32 upper,
        /* [in] */ Int32 count);

    CARAPI EnqueueDeferredUnbindServiceMessage();

    CARAPI_(Boolean) RequestBindService();

public:
    // Type defs for controlling different messages across the main and worker message queues
    static const Int32 sDefaultMessageType = 0;
    static const Int32 sUnbindServiceMessageType = 1;
private:

    static const String TAG;//= "RemoteViewsAdapter";
    static const String MULTI_USER_PERM;

    // The max number of items in the cache
    static const Int32 sDefaultCacheSize = 40;
    // The delay (in millis) to wait until attempting to unbind from a service after a request.
    // This ensures that we don't stay continually bound to the service and that it can be destroyed
    // if we need the memory elsewhere in the system.
    static const Int32 sUnbindServiceDelay = 5000;

    // Default height for the default loading view, in case we cannot get inflate the first view
    static const Int32 sDefaultLoadingViewHeight = 50;

    // We cache the FixedSizeRemoteViewsCaches across orientation. These are the related data
    // structures;
    static HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<FixedSizeRemoteViewsCache> > sCachedRemoteViewsCaches;
    static Object sCacheLock;

    static HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<IRunnable> > sRemoteViewsCacheRemoveRunnables;

    static AutoPtr<IHandlerThread> sCacheRemovalThread;
    static AutoPtr<IHandler> sCacheRemovalQueue;

    // We keep the cache around for a duration after onSaveInstanceState for use on re-inflation.
    // If a new RemoteViewsAdapter with the same intent / widget id isn't constructed within this
    // duration, the cache is dropped.
    static const Int32 REMOTE_VIEWS_CACHE_DURATION = 5000;
    AutoPtr<IContext> mContext;
    AutoPtr<IIntent> mIntent;
    Int32 mAppWidgetId;
    AutoPtr<ILayoutInflater> mLayoutInflater;
    AutoPtr<IRemoteViewsAdapterServiceConnection> mServiceConnection;
    IRemoteAdapterConnectionCallback* mCallback; // weakReference
    AutoPtr<IRemoteViewsOnClickHandler> mRemoteViewsOnClickHandler;
    AutoPtr<FixedSizeRemoteViewsCache> mCache;
    Object mCacheLock;
    Int32 mVisibleWindowLowerBound;
    Int32 mVisibleWindowUpperBound;

    // A flag to determine whether we should notify data set changed after we connect
    Boolean mNotifyDataSetChangedAfterOnServiceConnected;

    // The set of requested views that are to be notified when the associated RemoteViews are
    // loaded.
    AutoPtr<RemoteViewsFrameLayoutRefSet> mRequestedViews;

    AutoPtr<IHandlerThread> mWorkerThread;
    // items may be interrupted within the normally processed queues
    AutoPtr<IHandler> mWorkerQueue;
    AutoPtr<IHandler> mMainQueue;


    // Used to indicate to the AdapterView that it can use this Adapter immediately after
    // construction (happens when we have a cached FixedSizeRemoteViewsCache).
    Boolean mDataReady;// = false;

    typedef HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<IRunnable> >::Iterator RunIterator;
    typedef HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<FixedSizeRemoteViewsCache> >::Iterator CacheIterator;
protected:
    Int32 mUserId;
    friend class CRemoteViewsAdapterServiceConnection;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
