#ifndef __ELASTOS_DROID_WIDGET_ACTIVITYCHOOSERMODEL_H__
#define __ELASTOS_DROID_WIDGET_ACTIVITYCHOOSERMODEL_H__

#include "database/DataSetObservable.h"
#include "content/PackageMonitor.h"
#include "os/AsyncTask.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::IComparable;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Internal::Content::PackageMonitor;
using Elastos::Droid::Database::DataSetObservable;

namespace Elastos {
namespace Droid {
namespace Widget {

class ActivityChooserModel
    : public DataSetObservable
    , public IActivityChooserModel
    , public ElRefBase
{
public:
    class HistoricalRecord
        : public IHistoricalRecord
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        HistoricalRecord(
            /* [in] */ const String& activityName,
            /* [in] */ Int64 time,
            /* [in] */ Float weight);

        HistoricalRecord(
            /* [in] */ IComponentName* activityName,
            /* [in] */ Int64 time,
            /* [in] */ Float weight);

        CARAPI GetHashCode(
            /* [out] */ Int32* code);

        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* res);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        AutoPtr<IComponentName> mActivity;
        Int64 mTime;
        Float mWeight;
    };

    class ActivityResolveInfo
        : public IActivityResolveInfo
        , public ElRefBase
    {
        friend class ActivityChooserModel;
    public:
        CAR_INTERFACE_DECL()

        ActivityResolveInfo(
            /* [in] */ IResolveInfo* resolveInfo,
            /* [in] */ ActivityChooserModel* host);

        CARAPI GetHashCode(
            /* [out] */ Int32* hash);

        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* rst);

        CARAPI CompareTo(
            /* [in] */ IInterface* other,
            /* [out] */ Int32* rst);

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        ActivityChooserModel* mHost;
        AutoPtr<IResolveInfo> mResolveInfo;
        Float mWeight;
    };

    class DefaultSorter
        : public IActivitySorter
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        DefaultSorter(
            /* [in] */ ActivityChooserModel* host);

        CARAPI Sort(
            /* [in] */ IIntent* intent,
            /* [in] */ IObjectContainer* activities,
            /* [in] */ IObjectContainer* historicalRecords);
    private:
        ActivityChooserModel* mHost;
        HashMap<String, AutoPtr<ActivityResolveInfo> > mPackageNameToActivityMap;
        static const Float WEIGHT_DECAY_COEFFICIENT;
    };

    class PersistHistoryAsyncTask
        : public AsyncTask
    {
    public:
        PersistHistoryAsyncTask(
            /* [in] */ ActivityChooserModel* host);

        CARAPI_(AutoPtr<IInterface>) DoInBackground(
            /* [in] */ ArrayOf<IInterface*>* args);
    private:
        ActivityChooserModel* mHost;
    };

    class DataModelPackageMonitor
        : public PackageMonitor
    {
    public:
        DataModelPackageMonitor(
            /* [in] */ ActivityChooserModel* host);

        CARAPI OnSomePackagesChanged();
    private:
        ActivityChooserModel* mHost;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* object,
        /* [out] */ InterfaceID* iid);

    static CARAPI_(AutoPtr<IActivityChooserModel>) Get(
        /* [in] */ IContext* context,
        /* [in] */ const String& historyFileName);

    virtual ~ActivityChooserModel();

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI GetActivityCount(
        /* [out] */ Int32* rst);

    CARAPI GetActivity(
        /* [in] */ Int32 index,
        /* [out] */ IResolveInfo** activity);

    CARAPI GetActivityIndex(
        /* [in] */ IResolveInfo* activity,
        /* [out] */ Int32* rst);

    CARAPI ChooseActivity(
        /* [in] */ Int32 index,
        /* [out] */ IIntent** intent);

    CARAPI SetOnChooseActivityListener(
        /* [in] */ IOnChooseActivityListener* listener);

    CARAPI GetDefaultActivity(
        /* [out] */ IResolveInfo** activity);

    CARAPI SetDefaultActivity(
        /* [in] */ Int32 index);

    CARAPI SetActivitySorter(
        /* [in] */ IActivitySorter* activitySorter);

    CARAPI SetHistoryMaxSize(
        /* [in] */ Int32 historyMaxSize);

    CARAPI GetHistoryMaxSize(
        /*[out] */ Int32* size);

    CARAPI GetHistorySize(
        /*[out] */ Int32* size);

    CARAPI RegisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterObserver(
        /* [in] */ IInterface* observer);

    CARAPI UnregisterAll();

    CARAPI NotifyChanged();

    CARAPI NotifyInvalidated();

private:
    ActivityChooserModel(
        /* [in] */ IContext* context,
        /* [in] */ const String& historyFileName);

    CARAPI PersistHistoricalDataIfNeeded();

    CARAPI EnsureConsistentState();

    CARAPI_(Boolean) SortActivitiesIfNeeded();

    CARAPI_(Boolean) LoadActivitiesIfNeeded();

    CARAPI_(Boolean) ReadHistoricalDataIfNeeded();

    CARAPI_(Boolean) AddHisoricalRecord(
        /* [in] */ IHistoricalRecord* historicalRecord);

    CARAPI PruneExcessiveHistoricalRecordsIfNeeded();

    CARAPI ReadHistoricalDataImpl();

    template<typename T>
        AutoPtr<IObjectContainer> TransfromList(
            /* [in] */ List<AutoPtr<T> >& list);

    template<typename T, typename IT>
        List<AutoPtr<T> > TransfromContainer(
            /* [in] */ IObjectContainer* list);

    static CARAPI_(Int32) FloatToIntBits(
        /* [in] */ Float value);

private:
    /**
     * Flag for selecting debug mode.
     */

    static const Boolean DEBUG;
    /**
     * Tag used for logging.
     */

    static const String TAG;
    /**
     * The root tag in the history file.
     */

    static const String TAG_HISTORICAL_RECORDS;
    /**
     * The tag for a record in the history file.
     */

    static const String TAG_HISTORICAL_RECORD;
    /**
     * Attribute for the activity.
     */

    static const String ATTRIBUTE_ACTIVITY;
    /**
     * Attribute for the choice time.
     */

    static const String ATTRIBUTE_TIME;
    /**
     * Attribute for the choice weight.
     */

    static const String ATTRIBUTE_WEIGHT;
    /**
     * The default name of the choice history file.
     */

    static const String DEFAULT_HISTORY_FILE_NAME;
    /**
     * The default maximal length of the choice history.
     */

    static const Int32 DEFAULT_HISTORY_MAX_LENGTH;
    /**
     * The amount with which to inflate a chosen activity when set as default.
     */

    static const Int32 DEFAULT_ACTIVITY_INFLATION;
    /**
     * Default weight for a choice record.
     */

    static const Float DEFAULT_HISTORICAL_RECORD_WEIGHT;
    /**
     * The extension of the history file.
     */

    static const String HISTORY_FILE_EXTENSION;
    /**
     * An invalid item index.
     */

    static const Int32 INVALID_INDEX;
    /**
     * Lock to guard the model registry.
     */

    static Object sRegistryLock;
    /**
     * This the registry for data models.
     */

    static HashMap<String, AutoPtr<IActivityChooserModel> > sDataModelRegistry;

    /**
     * Lock for synchronizing on this instance.
     */

    Object mInstanceLock;
    /**
     * List of activities that can handle the current intent.
     */

    List<AutoPtr<ActivityResolveInfo> > mActivities;
    /**
     * List with historical choice records.
     */

    List<AutoPtr<HistoricalRecord> > mHistoricalRecords;
    /**
     * Monitor for added and removed packages.
     */

    AutoPtr<PackageMonitor> mPackageMonitor;
    /**
     * Context for accessing resources.
     */

    AutoPtr<IContext> mContext;
    /**
     * The name of the history file that backs this model.
     */

    String mHistoryFileName;
    /**
     * The intent for which a activity is being chosen.
     */

    AutoPtr<IIntent> mIntent;
    /**
     * The sorter for ordering activities based on intent and past choices.
     */

    AutoPtr<DefaultSorter> mActivitySorter;
    /**
     * The maximal length of the choice history.
     */

    Int32 mHistoryMaxSize;
    /**
     * Flag whether choice history can be read. In general many clients can
     * share the same data model and {@link #readHistoricalDataIfNeeded()} may be called
     * by arbitrary of them any number of times. Therefore, this class guarantees
     * that the very first read succeeds and subsequent reads can be performed
     * only after a call to {@link #persistHistoricalDataIfNeeded()} followed by change
     * of the share records.
     */

     Boolean mCanReadHistoricalData;
    /**
     * Flag whether the choice history was read. This is used to enforce that
     * before calling {@link #persistHistoricalDataIfNeeded()} a call to
     * {@link #persistHistoricalDataIfNeeded()} has been made. This aims to avoid a
     * scenario in which a choice history file exits, it is not read yet and
     * it is overwritten. Note that always all historical records are read in
     * full and the file is rewritten. This is necessary since we need to
     * purge old records that are outside of the sliding window of past choices.
     */

    Boolean mReadShareHistoryCalled;
    /**
     * Flag whether the choice records have changed. In general many clients can
     * share the same data model and {@link #persistHistoricalDataIfNeeded()} may be called
     * by arbitrary of them any number of times. Therefore, this class guarantees
     * that choice history will be persisted only if it has changed.
     */

    Boolean mHistoricalRecordsChanged;
    /**
     * Flag whether to reload the activities for the current intent.
     */

    Boolean mReloadActivities;
    /**
     * Policy for controlling how the model handles chosen activities.
     */

    AutoPtr<IOnChooseActivityListener> mActivityChoserModelPolicy;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
