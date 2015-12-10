#include "elastos/droid/widget/RemoteViewsAdapter.h"
#include "elastos/droid/widget/RemoteViews.h"
#include "elastos/droid/widget/CRemoteViewsAdapterServiceConnection.h"
#include "elastos/droid/view/View.h"
#include "elastos/droid/view/ViewGroup.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/os/CUserHandle.h"
#include "elastos/droid/os/Looper.h"
#include "elastos/droid/content/CIntentFilterComparison.h"
#include "elastos/droid/R.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::R;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Looper;
using Elastos::Core::StringUtils;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger32;
using Elastos::Droid::View::View;
using Elastos::Droid::View::ViewGroup;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::Content::CIntentFilterComparison;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Widget::Internal::IIRemoteViewsFactory;

namespace Elastos {
namespace Droid {
namespace Widget {

/*--------------------------------RemoteViewsCacheKey--------------------------------*/
RemoteViewsCacheKey::RemoteViewsCacheKey(
    /* [in] */ IIntentFilterComparison* filter,
    /* [in] */ Int32 widgetId,
    /* [in] */ Int32 userId)
{
    mFilter = filter;
    mWidgetId = widgetId;
    mUserId = userId;
}

/*--------------------------------RemoteViewsFrameLayout--------------------------------*/

RemoteViewsFrameLayout::RemoteViewsFrameLayout()
{}

ECode RemoteViewsFrameLayout::constrctor(
    /* [in] */ IContext* context)
{
    return FrameLayout::constrctor(context);
}

ECode RemoteViewsFrameLayout::OnRemoteViewsLoaded(
    /* [in] */ IRemoteViews* view,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    // try {
        // Remove all the children of this layout first
        RemoveAllViews();
        AutoPtr<IView> v;
        AutoPtr<IContext> ctx;
        GetContext((IContext**)&ctx);
        ECode pe = view->Apply(ctx, this, handler, (IView**)&v);
        if(!FAILED(pe) && v != NULL)
            AddView(v);
    // } catch (Exception e) {
        // Log.e(TAG, "Failed to apply RemoteViews.");
    // }
        return pe;
}

/*--------------------------------RemoteViewsFrameLayoutRefSet--------------------------------*/
RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::RemoteViewsFrameLayoutRefSet(
    /* [in] */ RemoteViewsAdapter* host) : mHost(host)
{}

RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::~RemoteViewsFrameLayoutRefSet()
{
    mViewToLinkedList.Clear();
    mReferences.Clear();
}

ECode RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::Add(
    /* [in] */ Int32 position,
    /* [in] */ RemoteViewsFrameLayout* layout)
{
    AutoPtr<RemoteViewsFrameLayoutList> refs;

    // Create the list if necessary
    if (mReferences.Find(position) != mReferences.End()) {
        refs = mReferences[position];
    } else {
        refs = new List<AutoPtr<RemoteViewsFrameLayout> >();
        mReferences[position] = refs;
    }
    mViewToLinkedList[layout] = refs;

    // Add the references to the list
    refs->PushBack(layout);
    return NOERROR;
}

ECode RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::NotifyOnRemoteViewsLoaded(
    /* [in] */ Int32 position,
    /* [in] */ IRemoteViews* view)
{
    if (view == NULL)
        return NOERROR;

    if (mReferences.Find(position) != mReferences.End()) {
        // Notify all the references for that position of the newly loaded RemoteViews
        AutoPtr<RemoteViewsFrameLayoutList> refs = mReferences[position];
        RemoteViewsFrameLayoutListIterator it = refs->Begin();
        while (it != refs->End()) {
            AutoPtr<RemoteViewsFrameLayout> ref = *it;
            ref->OnRemoteViewsLoaded(view, mHost->mRemoteViewsOnClickHandler);
            RemoteViewsFrameLayoutMapIterator refIt = mViewToLinkedList.Find(ref);
            if (refIt != mViewToLinkedList.End()) {
                mViewToLinkedList.Erase(refIt);
            }
            it++;
        }
        refs->Clear();

        // Remove this set from the original mapping
        mReferences.Erase(position);
    }
    return NOERROR;
}

ECode RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::RemoveView(
    /* [in] */ RemoteViewsFrameLayout* rvfl)
{
    if (mViewToLinkedList.Find(rvfl) != mViewToLinkedList.End()) {
        mViewToLinkedList[rvfl]->Remove(rvfl);
        mViewToLinkedList.Erase(rvfl);
    }
    return NOERROR;
}

ECode RemoteViewsAdapter::RemoteViewsFrameLayoutRefSet::Clear()
{
    mReferences.Clear();
    mViewToLinkedList.Clear();
    return NOERROR;
}

/*--------------------------------RemoteViewsMetaData--------------------------------*/
RemoteViewsAdapter::RemoteViewsMetaData::RemoteViewsMetaData()
    : mCount(0)
    , mViewTypeCount(0)
    , mHasStableIds(FALSE)
    , mFirstViewHeight(0)
{
    Reset();
}

ECode RemoteViewsAdapter::RemoteViewsMetaData::Set(
    /* [in] */ RemoteViewsMetaData* d)
{
    AutoLock lock(mLock);
    mCount = d->mCount;
    mViewTypeCount = d->mViewTypeCount;
    mHasStableIds = d->mHasStableIds;
    SetLoadingViewTemplates(d->mUserLoadingView, d->mFirstView);
    return NOERROR;
}

ECode RemoteViewsAdapter::RemoteViewsMetaData::Reset()
{
    // by default there is at least one dummy view type
    mViewTypeCount = 1;
    mHasStableIds = TRUE;
    mUserLoadingView = NULL;
    mFirstView = NULL;
    mFirstViewHeight = 0;
    mTypeIdIndexMap.Clear();
    return NOERROR;
}

ECode RemoteViewsAdapter::RemoteViewsMetaData::SetLoadingViewTemplates(
    /* [in] */ IRemoteViews* loadingView,
    /* [in] */ IRemoteViews* firstView)
{
    mUserLoadingView = loadingView;
    if (firstView != NULL) {
        mFirstView = firstView;
        mFirstViewHeight = -1;
    }
    return NOERROR;
}

Int32 RemoteViewsAdapter::RemoteViewsMetaData::GetMappedViewType(
    /* [in] */ Int32 typeId)
{
    if (mTypeIdIndexMap.Find(typeId) != mTypeIdIndexMap.End()) {
        return mTypeIdIndexMap[typeId];
    } else {
        // We +1 because the loading view always has view type id of 0
        Int32 incrementalTypeId = mTypeIdIndexMap.GetSize() + 1;
        mTypeIdIndexMap[typeId] = incrementalTypeId;
        return incrementalTypeId;
    }
}

Boolean RemoteViewsAdapter::RemoteViewsMetaData::IsViewTypeInRange(
    /* [in] */ Int32 typeId)
{
    Int32 mappedType = GetMappedViewType(typeId);
    if (mappedType >= mFirstViewHeight) {
        return FALSE;
    } else {
        return TRUE;
    }
}

AutoPtr<RemoteViewsFrameLayout> RemoteViewsAdapter::RemoteViewsMetaData::CreateLoadingView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Object& lock,
    /* [in] */ ILayoutInflater* layoutInflater,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    // Create and return a new FrameLayout, and setup the references for this position
    AutoPtr<IContext> context;
    parent->GetContext((IContext**)&context);
    AutoPtr<RemoteViewsFrameLayout> layout = new RemoteViewsFrameLayout(context);

    // Create a new loading view
    {
        AutoLock alock(lock);
        Boolean customLoadingViewAvailable = FALSE;

        if (mUserLoadingView != NULL) {
            // Try to inflate user-specified loading view
            // try {
                AutoPtr<IView> loadingView;
                mUserLoadingView->Apply(context, parent, handler, (IView**)&loadingView);
                AutoPtr<IInteger32> integer;
                CInteger32::New(0, (IInteger32**)&integer);
                loadingView->SetTagInternal(R::id::rowTypeId, integer);
                layout->AddView(loadingView);
                customLoadingViewAvailable = TRUE;
            // } catch (Exception e) {
            //     Log.w(TAG, "Error inflating custom loading view, using default loading" +
            //             "view instead", e);
            // }
        }
        if (!customLoadingViewAvailable) {
            // A default loading view
            // Use the size of the first row as a guide for the size of the loading view
            if (mFirstViewHeight < 0) {
                // try {
                    AutoPtr<IView> firstView;
                    ECode pe = mFirstView->Apply(context, parent, handler, (IView**)&firstView);
                    if(FAILED(pe))
                    {
                        AutoPtr<IResources> rs;
                        context->GetResources((IResources**)&rs);
                        AutoPtr<IDisplayMetrics> metrics;
                        rs->GetDisplayMetrics((IDisplayMetrics**)&metrics);
                        Float density;
                        metrics->GetDensity(&density);
                        mFirstViewHeight = Elastos::Core::Math::Round(sDefaultLoadingViewHeight * density);
                        mFirstView = NULL;
                    } else {
                        firstView->Measure(
                                Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(0, Elastos::Droid::View::View::MeasureSpec::UNSPECIFIED),
                                Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(0, Elastos::Droid::View::View::MeasureSpec::UNSPECIFIED));
                        firstView->GetMeasuredHeight(&mFirstViewHeight);
                        mFirstView = NULL;
                    }
                // } catch (Exception e) {
                //     float density = context.getResources().getDisplayMetrics().density;
                //     mFirstViewHeight = (int)
                //             Math.round(sDefaultLoadingViewHeight * density);
                //     mFirstView = null;
                //     Log.w(TAG, "Error inflating first RemoteViews" + e);
                // }
            }

            // Compose the loading view text
            AutoPtr<IView> vTemp;
            layoutInflater->Inflate(R::layout::remote_views_adapter_default_loading_view,
                    layout, FALSE, (IView**)&vTemp);
            AutoPtr<ITextView> loadingTextView = ITextView::Probe(vTemp);
            loadingTextView->SetHeight(mFirstViewHeight);
            AutoPtr<IInteger32> integer;
            CInteger32::New(0, (IInteger32**)&integer);
            loadingTextView->SetTag(integer);

            layout->AddView(loadingTextView);
        }
    }

    return layout;
}

/*--------------------------------RemoteViewsIndexMetaData--------------------------------*/
RemoteViewsAdapter::RemoteViewsIndexMetaData::RemoteViewsIndexMetaData(
    /* [in] */ IRemoteViews* v,
    /* [in] */ Int64 itemId)
{
    Set(v, itemId);
}

ECode RemoteViewsAdapter::RemoteViewsIndexMetaData::Set(
    /* [in] */ IRemoteViews* v,
    /* [in] */ Int64 id)
{
    mItemId = id;
    if(v != NULL) {
        v->GetLayoutId(&mTypeId);
    } else {
        mTypeId = 0;
    }
    return NOERROR;
}

/*--------------------------------FixedSizeRemoteViewsCache--------------------------------*/
const String RemoteViewsAdapter::FixedSizeRemoteViewsCache::TAG("RemoteViewsAdapter::FixedSizeRemoteViewsCache");
const Float RemoteViewsAdapter::FixedSizeRemoteViewsCache::sMaxCountSlackPercent;
const Int32 RemoteViewsAdapter::FixedSizeRemoteViewsCache::sMaxMemoryLimitInBytes;

RemoteViewsAdapter::FixedSizeRemoteViewsCache::FixedSizeRemoteViewsCache(
    /* [in] */ Int32 maxCacheSize)
    : mLastRequestedIndex(-1)
    , mPreloadLowerBound(0)
    , mPreloadUpperBound(-1)
    , mMaxCount(maxCacheSize)
    , mMaxCountSlack(Elastos::Core::Math::Round(sMaxCountSlackPercent * (mMaxCount / 2)))
{
    mMetaData = new RemoteViewsMetaData();
    mTemporaryMetaData = new RemoteViewsMetaData();
}

ECode RemoteViewsAdapter::FixedSizeRemoteViewsCache::Insert(
    /* [in] */ Int32 position,
    /* [in] */ IRemoteViews* v,
    /* [in] */ Int64 itemId,
    /* [in] */ List<Int32> visibleWindow)
{
    // Trim the cache if we go beyond the count
    if (mIndexRemoteViews.GetSize() >= mMaxCount) {
        mIndexRemoteViews.Erase(GetFarthestPositionFrom(position, visibleWindow));
    }

    // Trim the cache if we go beyond the available memory size constraints
    Int32 pruneFromPosition = (mLastRequestedIndex > -1) ? mLastRequestedIndex : position;
    while (GetRemoteViewsBitmapMemoryUsage() >= sMaxMemoryLimitInBytes) {
        // Note: This is currently the most naive mechanism for deciding what to prune when
        // we hit the memory limit.  In the future, we may want to calculate which index to
        // remove based on both its position as well as it's current memory usage, as well
        // as whether it was directly requested vs. whether it was preloaded by our caching
        // mechanism.
        mIndexRemoteViews.Erase(GetFarthestPositionFrom(pruneFromPosition, visibleWindow));
    }

    // Update the metadata cache
    if (mIndexMetaData.Find(position) != mIndexMetaData.End()) {
        AutoPtr<RemoteViewsIndexMetaData> metaData = mIndexMetaData[position];
        metaData->Set(v, itemId);
    } else {
        AutoPtr<RemoteViewsIndexMetaData> metaData = new RemoteViewsIndexMetaData(v, itemId);
        mIndexMetaData[position] =  metaData;
    }
    mIndexRemoteViews[position] =  v;
    return NOERROR;
}

AutoPtr<RemoteViewsAdapter::RemoteViewsMetaData> RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetMetaData()
{
    return mMetaData;
}

AutoPtr<RemoteViewsAdapter::RemoteViewsMetaData> RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetTemporaryMetaData()
{
    return mTemporaryMetaData;
}

AutoPtr<IRemoteViews> RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetRemoteViewsAt(
    /* [in] */ Int32 position)
{
    if (mIndexRemoteViews.Find(position) != mIndexRemoteViews.End()) {
        return mIndexRemoteViews[position];
    }
    return NULL;
}

AutoPtr<RemoteViewsAdapter::RemoteViewsIndexMetaData> RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetMetaDataAt(
    /* [in] */ Int32 position)
{
    if (mIndexMetaData.Find(position) != mIndexMetaData.End()) {
        return mIndexMetaData[position];
    }
    return NULL;
}

ECode RemoteViewsAdapter::FixedSizeRemoteViewsCache::CommitTemporaryMetaData()
{
    AutoLock lock(mTemporaryMetaDataLock);
    AutoLock lock(mMetaDataLock);
    mMetaData->Set(mTemporaryMetaData);
    return NOERROR;
}

Int32 RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetRemoteViewsBitmapMemoryUsage()
{
    // Calculate the memory usage of all the RemoteViews bitmaps being cached
    Int32 mem = 0;
    HashMap<Int32, AutoPtr<IRemoteViews> >::Iterator it = mIndexRemoteViews.Begin();
    for (; it != mIndexRemoteViews.End(); it++) {
        AutoPtr<IRemoteViews> v = it->mSecond;
        if (v != NULL) {
            Int32 usage;
            v->EstimateMemoryUsage(&usage);
            mem += usage;
        }
    }
    return mem;
}

Int32 RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetFarthestPositionFrom(
    /* [in] */ Int32 pos,
    /* [in] */ List<Int32> visibleWindow)
{
    // Find the index farthest away and remove that
    Int32 maxDist = 0;
    Int32 maxDistIndex = -1;
    Int32 maxDistNotVisible = 0;
    Int32 maxDistIndexNotVisible = -1;
    HashMap<Int32, AutoPtr<IRemoteViews> >::Iterator it = mIndexRemoteViews.Begin();
    for (; it != mIndexRemoteViews.End(); it++) {
        Int32 dist = Elastos::Core::Math::Abs(it->mFirst-pos);
        List<Int32>::ConstIterator cIterator = Find(visibleWindow.Begin(), visibleWindow.End(), it->mFirst);
        if (dist > maxDistNotVisible && cIterator == visibleWindow.End()) {
            // maxDistNotVisible/maxDistIndexNotVisible will store the index of the
            // farthest non-visible position
            maxDistIndexNotVisible = it->mFirst;
            maxDistNotVisible = dist;
        }
        if (dist >= maxDist) {
            // maxDist/maxDistIndex will store the index of the farthest position
            // regardless of whether it is visible or not
            maxDistIndex = it->mFirst;
            maxDist = dist;
        }
    }
    if (maxDistIndexNotVisible > -1) {
        return maxDistIndexNotVisible;
    }
    return maxDistIndex;
}

ECode RemoteViewsAdapter::FixedSizeRemoteViewsCache::QueueRequestedPositionToLoad(
    /* [in] */ Int32 position)
{
    mLastRequestedIndex = position;
    {
        AutoLock lock(mLoadIndicesLock);
        mRequestedIndices.Insert(position);
        mLoadIndices.Insert(position);
    }
    return NOERROR;
}

Boolean RemoteViewsAdapter::FixedSizeRemoteViewsCache::QueuePositionsToBePreloadedFromRequestedPosition(
    /* [in] */ Int32 position)
{
    // Check if we need to preload any items
    if (mPreloadLowerBound <= position && position <= mPreloadUpperBound) {
        Int32 center = (mPreloadUpperBound + mPreloadLowerBound) / 2;
        if (Elastos::Core::Math::Abs(position - center) < mMaxCountSlack) {
            return FALSE;
        }
    }

    Int32 count = 0;
    {
        AutoLock lock(mMetaDataLock);
        count = mMetaData->mCount;
    }
    {
        AutoLock lock(mLoadIndicesLock);
        mLoadIndices.Clear();

        // Add all the requested indices
        mLoadIndices.Insert(mRequestedIndices.Begin(), mRequestedIndices.End());

        // Add all the preload indices
        Int32 halfMaxCount = mMaxCount / 2;
        mPreloadLowerBound = position - halfMaxCount;
        mPreloadUpperBound = position + halfMaxCount;
        Int32 effectiveLowerBound = Elastos::Core::Math::Max(0, mPreloadLowerBound);
        Int32 effectiveUpperBound = Elastos::Core::Math::Min(mPreloadUpperBound, count - 1);
        for (Int32 i = effectiveLowerBound; i <= effectiveUpperBound; ++i) {
            mLoadIndices.Insert(i);
        }

        // But remove all the indices that have already been loaded and are cached
        HashMap<Int32, AutoPtr<IRemoteViews> >::Iterator it = mIndexRemoteViews.Begin();
        for (; it != mIndexRemoteViews.End(); it++)
        {
            mLoadIndices.Erase(it->mFirst);
        }
    }
    return TRUE;
}

AutoPtr<ArrayOf<Int32> > RemoteViewsAdapter::FixedSizeRemoteViewsCache::GetNextIndexToLoad()
{
    // We try and prioritize items that have been requested directly, instead
    // of items that are loaded as a result of the caching mechanism
     {
        AutoLock lock(mLoadIndicesLock);
        // Prioritize requested indices to be loaded first
        AutoPtr<ArrayOf<Int32> > rst = ArrayOf<Int32>::Alloc(2);
        if (!mRequestedIndices.IsEmpty()) {
            HashSet<Int32>::Iterator it = mRequestedIndices.Begin();
            Int32 i = *it;
            mRequestedIndices.Erase(i);
            mLoadIndices.Erase(i);
            (*rst)[0] = i;
            (*rst)[1] = 1;
            return rst;
        }

        // Otherwise, preload other indices as necessary
        if (!mLoadIndices.IsEmpty()) {
            HashSet<Int32>::Iterator it = mLoadIndices.Begin();
            Int32 i = *it;
            mLoadIndices.Erase(i);
            (*rst)[0] = i;
            (*rst)[1] = 0;
            return rst;
        }

        (*rst)[0] = -1;
        (*rst)[1] = 0;
        return rst;
    }
}

Boolean RemoteViewsAdapter::FixedSizeRemoteViewsCache::ContainsRemoteViewAt(
    /* [in] */ Int32 position)
{
    return mIndexRemoteViews.Find(position) != mIndexRemoteViews.End();
}

Boolean RemoteViewsAdapter::FixedSizeRemoteViewsCache::ContainsMetaDataAt(
    /* [in] */ Int32 position)
{
    return mIndexMetaData.Find(position) != mIndexMetaData.End();
}

ECode RemoteViewsAdapter::FixedSizeRemoteViewsCache::Reset()
{
    // Note: We do not try and reset the meta data, since that information is still used by
    // collection views to validate it's own contents (and will be re-requested if the data
    // is invalidated through the notifyDataSetChanged() flow).

    mPreloadLowerBound = 0;
    mPreloadUpperBound = -1;
    mLastRequestedIndex = -1;
    mIndexRemoteViews.Clear();
    mIndexMetaData.Clear();
    {
        AutoLock lock(mLoadIndicesLock);
        mRequestedIndices.Clear();
        mLoadIndices.Clear();
    }
    return NOERROR;
}

/*--------------------------------InnerRunnable--------------------------------*/
RemoteViewsAdapter::InnerRunnable::InnerRunnable(
    /* [in] */ RemoteViewsCacheKey* key) : mKey(key)
{}

ECode RemoteViewsAdapter::InnerRunnable::Run()
{
    AutoLock lock(sCacheLock);
    if (sCachedRemoteViewsCaches.Find(mKey) != sCachedRemoteViewsCaches.End()) {
        sCachedRemoteViewsCaches.Erase(mKey);
    }
    if (sRemoteViewsCacheRemoveRunnables.Find(mKey) != sRemoteViewsCacheRemoveRunnables.End()) {
        sRemoteViewsCacheRemoveRunnables.Erase(mKey);
    }
    return NOERROR;
}
/*--------------------------------InnerRunnableEx--------------------------------*/
RemoteViewsAdapter::InnerRunnableEx::InnerRunnable(
    /* [in] */ RemoteViewsAdapter* host) : mHost(host)
{}

ECode RemoteViewsAdapter::InnerRunnableEx::Run()
{
    Boolean connected;
    if (mHost->mServiceConnection->IsConnected(&connected), connected) {
        // Get the next index to load
        Int32 position = -1;
        {
            AutoLock lock(mHost->mCacheLock);
            AutoPtr<ArrayOf<Int32> > res = mHost->mCache->GetNextIndexToLoad();
            position = (*res)[0];
        }
        if (position > -1) {
            // Load the item, and notify any existing RemoteViewsFrameLayouts
            mHost->UpdateRemoteViews(position, TRUE);

            // Queue up for the next one to load
            mHost->LoadNextIndexInBackground();
        } else {
            // No more items to load, so queue unbind
            mHost->EnqueueDeferredUnbindServiceMessage();
        }
    }
    return NOERROR;
}

/*--------------------------------InnerRunnableEx2--------------------------------*/
RemoteViewsAdapter::InnerRunnableEx2::InnerRunnable(
    /* [in] */ RemoteViewsAdapter* host) : mHost(host)
{}

ECode RemoteViewsAdapter::InnerRunnableEx2::Run()
{
    {
        AutoLock(mHost->mCacheLock);
        mHost->mCache->CommitTemporaryMetaData();
    }

    mHost->SuperNotifyDataSetChanged();
    mHost->EnqueueDeferredUnbindServiceMessage();
    return NOERROR;
}

/*--------------------------------InnerRunnableEx3--------------------------------*/
RemoteViewsAdapter::InnerRunnableEx3::InnerRunnable(
    /* [in] */ RemoteViewsAdapter* host) : mHost(host)
{}

ECode RemoteViewsAdapter::InnerRunnableEx3::Run()
{
    mHost->SuperNotifyDataSetChanged();
    return NOERROR;
}

/*--------------------------------InnerRunnableEx4--------------------------------*/
RemoteViewsAdapter::InnerRunnableEx4::InnerRunnable(
            /* [in] */ RemoteViewsFrameLayoutRefSet* refSet,
            /* [in] */ Int32 position,
            /* [in] */ IRemoteViews* rv)
    : mRefSet(refSet)
    , mPosition(position)
    , mRv(rv)
{}

ECode RemoteViewsAdapter::InnerRunnableEx4::Run()
{
    mRefSet->NotifyOnRemoteViewsLoaded(mPosition, mRv);
    return NOERROR;
}

/*--------------------------------InnerRunnableEx5--------------------------------*/
RemoteViewsAdapter::InnerRunnableEx5::InnerRunnable(
    /* [in] */ RemoteViewsAdapter* host) : mHost(host)
{}

ECode RemoteViewsAdapter::InnerRunnableEx5::Run()
{
    mHost->OnNotifyDataSetChanged();
    return NOERROR;
}
/*--------------------------------RemoteViewsAdapter--------------------------------*/

const String RemoteViewsAdapter::TAG("RemoteViewsAdapter");

    // The max number of items in the cache
const Int32 RemoteViewsAdapter::sDefaultCacheSize;
    // The delay (in millis) to wait until attempting to unbind from a service after a request.
    // This ensures that we don't stay continually bound to the service and that it can be destroyed
    // if we need the memory elsewhere in the system.
const Int32 RemoteViewsAdapter::sUnbindServiceDelay;

    // Default height for the default loading view, in case we cannot get inflate the first view
const Int32 RemoteViewsAdapter::sDefaultLoadingViewHeight;

    // Type defs for controlling different messages across the main and worker message queues
const Int32 RemoteViewsAdapter::RemoteViewsAdapter::sDefaultMessageType;
const Int32 RemoteViewsAdapter::sUnbindServiceMessageType;

    // We cache the FixedSizeRemoteViewsCaches across orientation. These are the related data
    // structures;
HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<RemoteViewsAdapter::FixedSizeRemoteViewsCache> > RemoteViewsAdapter::sCachedRemoteViewsCaches;
HashMap<AutoPtr<RemoteViewsCacheKey>, AutoPtr<IRunnable> > RemoteViewsAdapter::sRemoteViewsCacheRemoveRunnables;
Mutex RemoteViewsAdapter::sCacheLock;

AutoPtr<IHandlerThread> RemoteViewsAdapter::sCacheRemovalThread = NULL;
AutoPtr<IHandler> RemoteViewsAdapter::sCacheRemovalQueue = NULL;

RemoteViewsAdapter::RemoteViewsAdapter()
    : mAppWidgetId(0)
    , mVisibleWindowLowerBound(0)
    , mVisibleWindowUpperBound(0)
    , mNotifyDataSetChangedAfterOnServiceConnected(FALSE)
    , mDataReady(FALSE)
    , mUserId(0)
{}

RemoteViewsAdapter::RemoteViewsAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [in] */ IRemoteAdapterConnectionCallback* callback)
    : mAppWidgetId(0)
    , mVisibleWindowLowerBound(0)
    , mVisibleWindowUpperBound(0)
    , mNotifyDataSetChangedAfterOnServiceConnected(FALSE)
    , mDataReady(FALSE)
    , mUserId(0)
{
    ASSERT_SUCCEEDED(Init(context, intent, callback));
}

RemoteViewsAdapter::~RemoteViewsAdapter()
{
    if (mWorkerThread)
    {
        Boolean rst;
        mWorkerThread->Quit(&rst);
    }
}

Boolean RemoteViewsAdapter::IsDataReady()
{
    return mDataReady;
}

ECode RemoteViewsAdapter::SetRemoteViewsOnClickHandler(
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    mRemoteViewsOnClickHandler = handler;
    return NOERROR;
}

ECode RemoteViewsAdapter::SaveRemoteViewsCache()
{
    AutoPtr<IIntentFilterComparison> comparison;
    CIntentFilterComparison::New(mIntent, (IIntentFilterComparison**)&comparison);
    AutoPtr<RemoteViewsCacheKey> key = new RemoteViewsCacheKey(comparison, mAppWidgetId, mUserId);
    {
        // If we already have a remove runnable posted for this key, remove it.
        AutoLock lock(sCacheLock);
        RunIterator it = sRemoteViewsCacheRemoveRunnables.Find(key);
        if (it != sRemoteViewsCacheRemoveRunnables.End()) {
            AutoPtr<IRunnable> r = it->mSecond;
            sCacheRemovalQueue->RemoveCallbacks(r);
            sRemoteViewsCacheRemoveRunnables.Erase(key);
        }

        Int32 metaDataCount = 0;
        Int32 numRemoteViewsCached = 0;
        {
            AutoLock lock(mCache->mMetaDataLock);
            metaDataCount = mCache->mMetaData->mCount;
        }
        {
            AutoLock lock(mCacheLock);
            numRemoteViewsCached = mCache->mIndexRemoteViews.GetSize();
        }
        if (metaDataCount > 0 && numRemoteViewsCached > 0) {
            sCachedRemoteViewsCaches[key] = mCache;
        }

        AutoPtr<IRunnable> r = new InnerRunnable(key);
        sRemoteViewsCacheRemoveRunnables[key] = r;
        Boolean result;
        sCacheRemovalQueue->PostDelayed(r, REMOTE_VIEWS_CACHE_DURATION, &result);
    }
    return NOERROR;
}

AutoPtr<IIntent> RemoteViewsAdapter::GetRemoteViewsServiceIntent()
{
    return mIntent;
}

Int32 RemoteViewsAdapter::GetCount()
{
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    {
        AutoLock lock(mCache->mMetaDataLock);
        return metaData->mCount;
    }
}
AutoPtr<IInterface> RemoteViewsAdapter::GetItem(
    /* [in] */ Int32 position)
{
    // Disallow arbitrary object to be associated with an item for the time being
    return NULL;
}

Int64 RemoteViewsAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    AutoLock lock(mCacheLock);
    if (mCache->ContainsMetaDataAt(position)) {
        return mCache->GetMetaDataAt(position)->mItemId;
    }
    else {
        return 0;
    }
}

Int32 RemoteViewsAdapter::GetItemViewType(
    /* [in] */ Int32 position)
{
    Int32 typeId = 0;
    {
        AutoLock lock(mCacheLock);
        if(mCache->ContainsMetaDataAt(position)) {
            typeId = mCache->GetMetaDataAt(position)->mTypeId;
        }
        else {
            return 0;
        }
    }
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    {
        AutoLock lock(mCache->mMetaDataLock);
        return metaData->GetMappedViewType(typeId);
    }
}

ECode RemoteViewsAdapter::SetVisibleRangeHint(
    /* [in] */ Int32 lowerBound,
    /* [in] */ Int32 upperBound)
{
    mVisibleWindowLowerBound = lowerBound;
    mVisibleWindowUpperBound = upperBound;
    return NOERROR;
}

AutoPtr<IView> RemoteViewsAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    Boolean isInCache = mCache->ContainsRemoteViewAt(position);
    Boolean isConnected;
    mServiceConnection->IsConnected(&isConnected);
    Boolean hasNewItems = FALSE;

    if (convertView != NULL && convertView->Probe(EIID_RemoteViewsFrameLayout)) {
        mRequestedViews->RemoveView(reinterpret_cast<RemoteViewsFrameLayout*>(convertView->Probe(EIID_RemoteViewsFrameLayout)));
    }

    if (!isInCache && !isConnected) {
        // Requesting bind service will trigger a super.notifyDataSetChanged(), which will
        // in turn trigger another request to getView()
        RequestBindService();
    } else {
        // Queue up other indices to be preloaded based on this position
        hasNewItems = mCache->QueuePositionsToBePreloadedFromRequestedPosition(position);
    }

    if (isInCache) {
        AutoPtr<IView> convertViewChild;
        Int32 convertViewTypeId = 0;
        AutoPtr<RemoteViewsFrameLayout> layout;

        if (convertView->Probe(EIID_RemoteViewsFrameLayout)) {
            layout = reinterpret_cast<RemoteViewsFrameLayout*>(convertView->Probe(EIID_RemoteViewsFrameLayout));
            layout->GetChildAt(0, (IView**)&convertViewChild);
            convertViewTypeId = GetConvertViewTypeId(convertViewChild);
        }

        // Second, we try and retrieve the RemoteViews from the cache, returning a loading
        // view and queueing it to be loaded if it has not already been loaded.
        AutoPtr<IContext> context;
        parent->GetContext((IContext**)&context);
        AutoPtr<IRemoteViews> rv = mCache->GetRemoteViewsAt(position);
        AutoPtr<RemoteViewsIndexMetaData> indexMetaData = mCache->GetMetaDataAt(position);
        Int32 typeId = indexMetaData->mTypeId;

        // try {
            // Reuse the convert view where possible
            if (layout != NULL) {
                if (convertViewTypeId == typeId) {
                    ECode pe = rv->Reapply(context, convertViewChild, mRemoteViewsOnClickHandler);
                    if(FAILED(pe))
                    {
                        SLOGGERW(TAG, String("Error inflating RemoteViews at position: ") + StringUtils::Int32ToString(position) + ", using" +
                            "loading view instead");

                        AutoPtr<RemoteViewsFrameLayout> loadingView = NULL;
                        AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
                        {
                            AutoLock lock(mCache->mMetaDataLock);
                            loadingView = metaData->CreateLoadingView(position, convertView, parent,
                                    mCache->mMetaDataLock, mLayoutInflater, mRemoteViewsOnClickHandler);
                        }
                        if (hasNewItems) LoadNextIndexInBackground();
                        return loadingView;
                    }
                    if (hasNewItems) LoadNextIndexInBackground();
                    return layout;
                }
                layout->RemoveAllViews();
            } else {
                layout = new RemoteViewsFrameLayout(context);
            }

            // Otherwise, create a new view to be returned
            AutoPtr<IView> newView;
            rv->Apply(context, parent, mRemoteViewsOnClickHandler, (IView**)&newView);
            AutoPtr<IInteger32> integer;
            CInteger32::New(typeId, (IInteger32**)&integer);
            newView->SetTagInternal(R::id::rowTypeId, integer);
            layout->AddView(newView);
            if (hasNewItems) LoadNextIndexInBackground();
            return layout;

        // } catch (Exception e){
        //     // We have to make sure that we successfully inflated the RemoteViews, if not
        //     // we return the loading view instead.
        //     Log.w(TAG, "Error inflating RemoteViews at position: " + position + ", using" +
        //             "loading view instead" + e);

        //     RemoteViewsFrameLayout loadingView = null;
        //     final RemoteViewsMetaData metaData = mCache.getMetaData();
        //     synchronized(metaData) {
        //         loadingView = metaData.createLoadingView(position, convertView, parent,
        //                 mCache, mLayoutInflater, mRemoteViewsOnClickHandler);
        //     }
        //     return loadingView;
        // } finally {
            // if (hasNewItems) loadNextIndexInBackground();
        // }
    } else {
        // If the cache does not have the RemoteViews at this position, then create a
        // loading view and queue the actual position to be loaded in the background
        AutoPtr<RemoteViewsFrameLayout> loadingView = NULL;
        AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
        {
            AutoLock lock(mCache->mMetaDataLock);
            loadingView = metaData->CreateLoadingView(position, convertView, parent,
                    mCacheLock, mLayoutInflater, mRemoteViewsOnClickHandler);
        }

        mRequestedViews->Add(position, loadingView);
        mCache->QueueRequestedPositionToLoad(position);
        LoadNextIndexInBackground();

        return loadingView;
    }
}

Int32 RemoteViewsAdapter::GetViewTypeCount()
{
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    {
        AutoLock lock(mCache->mMetaDataLock);
        return metaData->mViewTypeCount;
    }
}

Boolean RemoteViewsAdapter::HasStableIds()
{
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    {
        AutoLock lock(mCache->mMetaDataLock);
        return metaData->mHasStableIds;
    }
}

Boolean RemoteViewsAdapter::IsEmpty()
{
    return GetCount() <= 0;;
}

ECode RemoteViewsAdapter::NotifyDataSetChanged()
{
    // Dequeue any unbind messages
    mMainQueue->RemoveMessages(sUnbindServiceMessageType);

    // If we are not connected, queue up the notifyDataSetChanged to be handled when we do
    // connect
    Boolean connected;
    if (!(mServiceConnection->IsConnected(&connected), connected)) {
        if (mNotifyDataSetChangedAfterOnServiceConnected) {
            return NOERROR;
        }

        mNotifyDataSetChangedAfterOnServiceConnected = TRUE;
        RequestBindService();
        return NOERROR;
    }

    AutoPtr<IRunnable> r = new InnerRunnable(this);
    Boolean rst;
    return mWorkerQueue->Post(r, &rst);
}

ECode RemoteViewsAdapter::SuperNotifyDataSetChanged()
{
    return BaseAdapter::NotifyDataSetChanged();
}

ECode RemoteViewsAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [in] */ IRemoteAdapterConnectionCallback* callback)
{
    mContext = context;
    mIntent = intent;
    intent->GetInt32Extra(RemoteViews::EXTRA_REMOTEADAPTER_APPWIDGET_ID, -1, &mAppWidgetId);
    LayoutInflater::From(context, (ILayoutInflater**)&mLayoutInflater);
    if (mIntent == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mRequestedViews = new RemoteViewsFrameLayoutRefSet(this);

    if (Process::MyUid() == IProcess::SYSTEM_UID) {
        // mUserId = new LockPatternUtils(context).getCurrentUser();
    } else {
        mUserId = UserHandle::GetMyUserId();
    }
    // Strip the previously injected app widget id from service intent
    Boolean hasExtra;
    if (intent->HasExtra(RemoteViews::EXTRA_REMOTEADAPTER_APPWIDGET_ID, &hasExtra), hasExtra) {
        intent->RemoveExtra(RemoteViews::EXTRA_REMOTEADAPTER_APPWIDGET_ID);
    }

    // Initialize the worker thread
    CHandlerThread::New(String("RemoteViewsCache-loader"), (IHandlerThread**)&mWorkerThread);
    mWorkerThread->Start();
    AutoPtr<ILooper> looper;
    mWorkerThread->GetLooper((ILooper**)&looper);
    CHandler::New(looper, (IHandler**)&mWorkerQueue);
    CHandler::New(Looper::GetMyLooper(), (IHandler**)&mMainQueue);

    if (sCacheRemovalThread == NULL) {
        CHandlerThread::New(String("RemoteViewsAdapter-cachePruner"), (IHandlerThread**)&sCacheRemovalThread);
        sCacheRemovalThread->Start();
        AutoPtr<ILooper> sLooper;
        sCacheRemovalThread->GetLooper((ILooper**)&sLooper);
        CHandler::New(sLooper, (IHandler**)&sCacheRemovalQueue);
    }

    // Initialize the cache and the service connection on startup
    mCallback = callback;//new WeakReference<RemoteAdapterConnectionCallback>(callback);
    CRemoteViewsAdapterServiceConnection::New(THIS_PROBE(IRemoteViewsAdapter), (IRemoteViewsAdapterServiceConnection**)&mServiceConnection);

    AutoPtr<IIntentFilterComparison> comparison;
    CIntentFilterComparison::New(mIntent, (IIntentFilterComparison**)&comparison);
    AutoPtr<RemoteViewsCacheKey> key = new RemoteViewsCacheKey(comparison, mAppWidgetId, mUserId);

    {
        AutoLock lock(sCacheLock);
        CacheIterator it = sCachedRemoteViewsCaches.Find(key);
        if (it != sCachedRemoteViewsCaches.End()) {

            mCache = it->mSecond;
            {
                AutoLock lock(mCache->mMetaDataLock);
                if (mCache->mMetaData->mCount > 0) {
                    // As a precautionary measure, we verify that the meta data indicates a
                    // non-zero count before declaring that data is ready.
                    mDataReady = TRUE;
                }
            }
        } else {
            mCache = new FixedSizeRemoteViewsCache(sDefaultCacheSize);
        }
        if (!mDataReady) {
            RequestBindService();
        }
    }
    return NOERROR;
}

ECode RemoteViewsAdapter::LoadNextIndexInBackground()
{
    AutoPtr<IRunnable> r = new InnerRunnable(this);
    Boolean result;
    mWorkerQueue->Post(r, &result);
    return NOERROR;
}

ECode RemoteViewsAdapter::ProcessException(
    /* [in] */ const String& method)
{
    SLOGGERD("RemoteViewsAdapter", String("Error in ") + method );//+ ": " + e.getMessage());

    // If we encounter a crash when updating, we should reset the metadata & cache and trigger
    // a notifyDataSetChanged to update the widget accordingly
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    {
        AutoLock lock(mCache->mMetaDataLock);
        metaData->Reset();
    }
    {
        AutoLock lock(mCacheLock);
        mCache->Reset();
    }
    AutoPtr<IRunnable> r = new InnerRunnable(this);
    Boolean rst;
    mMainQueue->Post(r, &rst);
    return NOERROR;
}

ECode RemoteViewsAdapter::UpdateTemporaryMetaData()
{
    AutoPtr<IIRemoteViewsFactory> factory;
    mServiceConnection->GetRemoteViewsFactory((IIRemoteViewsFactory**)&factory);

    // try {
        // get the properties/first view (so that we can use it to
        // measure our dummy views)
        Boolean hasStableIds;
        ECode pe = factory->HasStableIds(&hasStableIds);
        if (FAILED(pe))
        {
            ProcessException(String("updateMetaData"));
        }
        Int32 viewTypeCount;
        pe = factory->GetViewTypeCount(&viewTypeCount);
        if (FAILED(pe))
        {
            ProcessException(String("updateMetaData"));
        }
        Int32 count;
        pe = factory->GetCount(&count);
        if (FAILED(pe))
        {
            ProcessException(String("updateMetaData"));
        }
        AutoPtr<IRemoteViews> loadingView;
        AutoPtr<IParcelable> parcelable;
        pe = factory->GetLoadingView((IParcelable**)&parcelable);
        if (FAILED(pe))
        {
            ProcessException(String("updateMetaData"));
        }
        loadingView = IRemoteViews::Probe(parcelable);
        AutoPtr<IRemoteViews> firstView = NULL;
        if ((count > 0) && (loadingView == NULL)) {
            parcelable = NULL;
            pe = factory->GetViewAt(0, (IParcelable**)&parcelable);
            if (FAILED(pe))
            {
                ProcessException(String("updateMetaData"));
            }
            firstView = IRemoteViews::Probe(parcelable);
        }
        AutoPtr<RemoteViewsMetaData> tmpMetaData = mCache->GetTemporaryMetaData();
        {
            AutoLock lock(mCache->mTemporaryMetaDataLock);
            tmpMetaData->mHasStableIds = hasStableIds;
            // We +1 because the base view type is the loading view
            tmpMetaData->mViewTypeCount = viewTypeCount + 1;
            tmpMetaData->mCount = count;
            tmpMetaData->SetLoadingViewTemplates(loadingView, firstView);
        }
    // } catch(RemoteException e) {
    //     processException("updateMetaData", e);
    // } catch(RuntimeException e) {
    //     processException("updateMetaData", e);
    // }
    return NOERROR;
}

ECode RemoteViewsAdapter::UpdateRemoteViews(
    /* [in] */ Int32 position,
    /* [in] */ Boolean notifyWhenLoaded)
{
    AutoPtr<IIRemoteViewsFactory> factory;
    mServiceConnection->GetRemoteViewsFactory((IIRemoteViewsFactory**)&factory);

    // Load the item information from the remote service
    AutoPtr<IRemoteViews> remoteViews;
    Int64 itemId = 0;
    // try {
        AutoPtr<IParcelable> parcelable;
        ECode pe = factory->GetViewAt(position, (IParcelable**)&parcelable);
        if(FAILED(pe))
        {
            SLOGGERD(TAG, String("Error in updateRemoteViews(") + StringUtils::Int32ToString(position));
            return NOERROR;
        }
        remoteViews = IRemoteViews::Probe(parcelable);
        AutoPtr<IUserHandle> handle;
        CUserHandle::New(mUserId, (IUserHandle**)&handle);
        remoteViews->SetUser(handle);
        pe =factory->GetItemId(position, &itemId);
        if(FAILED(pe))
        {
            SLOGGERD(TAG, String("Error in updateRemoteViews(") + StringUtils::Int32ToString(position));
            return NOERROR;
        }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Error in updateRemoteViews(" + position + "): " + e.getMessage());

    //     // Return early to prevent additional work in re-centering the view cache, and
    //     // swapping from the loading view
    //     return;
    // } catch (RuntimeException e) {
    //     Log.e(TAG, "Error in updateRemoteViews(" + position + "): " + e.getMessage());
    //     return;
    // }

    if (remoteViews == NULL) {
        // If a null view was returned, we break early to prevent it from getting
        // into our cache and causing problems later. The effect is that the child  at this
        // position will remain as a loading view until it is updated.
        SLOGGERD(TAG, String("Error in updateRemoteViews(") + StringUtils::Int32ToString(position) + "): " + " null RemoteViews " +
                "returned from RemoteViewsFactory.");
        return NOERROR;
    }

    Int32 layoutId;
    remoteViews->GetLayoutId(&layoutId);
    AutoPtr<RemoteViewsMetaData> metaData = mCache->GetMetaData();
    Boolean viewTypeInRange;
    Int32 cacheCount;
    {
        AutoLock lock(mCache->mMetaDataLock);
        viewTypeInRange = metaData->IsViewTypeInRange(layoutId);
        cacheCount = mCache->mMetaData->mCount;
    }
    {
        AutoLock lock(mCacheLock);
        if (viewTypeInRange) {
            List<Int32> visibleWindow = GetVisibleWindow(mVisibleWindowLowerBound,
                    mVisibleWindowUpperBound, cacheCount);
            // Cache the RemoteViews we loaded
            mCache->Insert(position, remoteViews, itemId, visibleWindow);

            // Notify all the views that we have previously returned for this index that
            // there is new data for it.
            AutoPtr<IRemoteViews> rv = remoteViews;
            if (notifyWhenLoaded) {
                AutoPtr<IRunnable> r = new InnerRunnable(mRequestedViews, position, rv);
                Boolean rst;
                mMainQueue->Post(r, &rst);
            }
        } else {
            // We need to log an error here, as the the view type count specified by the
            // factory is less than the number of view types returned. We don't return this
            // view to the AdapterView, as this will cause an exception in the hosting process,
            // which contains the associated AdapterView.
            SLOGGERE(TAG, String("Error: widget's RemoteViewsFactory returns more view types than ") +
                    " indicated by getViewTypeCount() ");
        }
    }
    return NOERROR;
}

Int32 RemoteViewsAdapter::GetConvertViewTypeId(
    /* [in] */ IView* convertView)
{
    Int32 typeId = -1;
    if (convertView != NULL) {
        AutoPtr<IInterface> tag;
        convertView->GetTag(R::id::rowTypeId, (IInterface**)&tag);
        if (tag != NULL) {
             IInteger32::Probe(tag)->GetValue(&typeId);
        }
    }
    return typeId;
}

ECode RemoteViewsAdapter::OnNotifyDataSetChanged()
{
    // Complete the actual notifyDataSetChanged() call initiated earlier
    AutoPtr<IIRemoteViewsFactory> factory;
    mServiceConnection->GetRemoteViewsFactory((IIRemoteViewsFactory**)&factory);
    // try {
        FAIL_RETURN(factory->OnDataSetChanged());
    // } catch (RemoteException e) {
    //     Log.e(TAG, "Error in updateNotifyDataSetChanged(): " + e.getMessage());

    //     // Return early to prevent from further being notified (since nothing has
    //     // changed)
    //     return;
    // } catch (RuntimeException e) {
    //     Log.e(TAG, "Error in updateNotifyDataSetChanged(): " + e.getMessage());
    //     return;
    // }

    // Flush the cache so that we can reload new items from the service
    {
        AutoLock lock(mCacheLock);
        mCache->Reset();
    }

    // Re-request the new metadata (only after the notification to the factory)
    UpdateTemporaryMetaData();
    Int32 newCount;
    List<Int32> visibleWindow;
    {
        AutoLock lock(mCache->mTemporaryMetaDataLock);
        newCount = mCache->GetTemporaryMetaData()->mCount;
        visibleWindow = GetVisibleWindow(mVisibleWindowLowerBound,
                mVisibleWindowUpperBound, newCount);
    }

    // Pre-load (our best guess of) the views which are currently visible in the AdapterView.
    // This mitigates flashing and flickering of loading views when a widget notifies that
    // its data has changed.
    List<Int32>::Iterator it = visibleWindow.Begin();
    for (; it != visibleWindow.End(); it++) {
        // Because temporary meta data is only ever modified from this thread (ie.
        // mWorkerThread), it is safe to assume that count is a valid representation.
        if (*it < newCount) {
            UpdateRemoteViews(*it, FALSE);
        }
    }

    // Propagate the notification back to the base adapter
    AutoPtr<IRunnable> r = new InnerRunnable(this);
    Boolean rst;
    mMainQueue->Post(r, &rst);

    // Reset the notify flagflag
    mNotifyDataSetChangedAfterOnServiceConnected = FALSE;
    return NOERROR;
}

List<Int32> RemoteViewsAdapter::GetVisibleWindow(
    /* [in] */ Int32 lower,
    /* [in] */ Int32 upper,
    /* [in] */ Int32 count)
{
    List<Int32> window;

    // In the case that the window is invalid or uninitialized, return an empty window.
    if ((lower == 0 && upper == 0) || lower < 0 || upper < 0) {
        return window;
    }

    if (lower <= upper) {
        for (Int32 i = lower;  i <= upper; i++){
            window.PushBack(i);
        }
    }
    else {
        // If the upper bound is less than the lower bound it means that the visible window
        // wraps around.
        for (Int32 i = lower; i < count; i++) {
            window.PushBack(i);
        }
        for (Int32 i = 0; i <= upper; i++) {
            window.PushBack(i);
        }
    }
    return window;
}

Boolean RemoteViewsAdapter::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    Boolean result = false;
    switch (what) {
    case sUnbindServiceMessageType: {
        Boolean result;
        if (mServiceConnection->IsConnected(&result), result) {
            mServiceConnection->Unbind(mContext, mAppWidgetId, mIntent);
        }
        result = true;
        break;
    }
    default:
        break;
    }
    return result;
}

ECode RemoteViewsAdapter::EnqueueDeferredUnbindServiceMessage()
{
    // Remove any existing deferred-unbind messages
    mMainQueue->RemoveMessages(sUnbindServiceMessageType);
    Boolean result;
    return mMainQueue->SendEmptyMessageDelayed(sUnbindServiceMessageType,
        sUnbindServiceDelay, &result);
}

Boolean RemoteViewsAdapter::RequestBindService()
{
    // Try binding the service (which will start it if it's not already running)
    Boolean connected;
    if (!(mServiceConnection->IsConnected(&connected), connected)) {
        mServiceConnection->Bind(mContext, mAppWidgetId, mIntent);
    }

    // Remove any existing deferred-unbind messages
    mMainQueue->RemoveMessages(sUnbindServiceMessageType);
    mServiceConnection->IsConnected(&connected);
    return connected;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
