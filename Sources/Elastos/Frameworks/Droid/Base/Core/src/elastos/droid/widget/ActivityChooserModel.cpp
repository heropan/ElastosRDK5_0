#include "elastos/droid/widget/ActivityChooserModel.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/utility/Xml.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Math::IBigDecimal;
using Elastos::Math::CBigDecimal;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::IO::IFileOutputStream;
using Elastos::Core::EIID_IComparable;

using Elastos::IO::IFileInputStream;
using Org::Xmlpull::V1::IXmlSerializer;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Widget::IActivitySorter;
using Elastos::Droid::Database::EIID_IObservable;
using Elastos::Droid::Database::EIID_IDataSetObservable;

namespace Elastos {
namespace Droid {
namespace Widget {

const Boolean ActivityChooserModel::DEBUG = FALSE;
const String ActivityChooserModel::TAG("ActivityChooserModel");
const String ActivityChooserModel::TAG_HISTORICAL_RECORDS("historical-records");
const String ActivityChooserModel::TAG_HISTORICAL_RECORD("historical-record");
const String ActivityChooserModel::ATTRIBUTE_ACTIVITY("activity");
const String ActivityChooserModel::ATTRIBUTE_TIME("time");
const String ActivityChooserModel::ATTRIBUTE_WEIGHT("weight");
const String ActivityChooserModel::DEFAULT_HISTORY_FILE_NAME("activity_choser_model_history.xml");
const Int32 ActivityChooserModel::DEFAULT_HISTORY_MAX_LENGTH = 50;
const Int32 ActivityChooserModel::DEFAULT_ACTIVITY_INFLATION = 5;
const Float ActivityChooserModel::DEFAULT_HISTORICAL_RECORD_WEIGHT = 1.0f;
const String ActivityChooserModel::HISTORY_FILE_EXTENSION(".xml");
const Int32 ActivityChooserModel::INVALID_INDEX = -1;
Mutex ActivityChooserModel::sRegistryLock;
HashMap<String, AutoPtr<IActivityChooserModel> > ActivityChooserModel::sDataModelRegistry;
const Float ActivityChooserModel::DefaultSorter::WEIGHT_DECAY_COEFFICIENT = 0.95f;

CAR_INTERFACE_IMPL(ActivityChooserModel::HistoricalRecord, IHistoricalRecord)
CAR_INTERFACE_IMPL_2(ActivityChooserModel::ActivityResolveInfo, IActivityResolveInfo, IComparable)
CAR_INTERFACE_IMPL(ActivityChooserModel::DefaultSorter, IActivitySorter)

ActivityChooserModel::HistoricalRecord::HistoricalRecord(
    /* [in] */ const String& activityName,
    /* [in] */ Int64 time,
    /* [in] */ Float weight)
{
    CComponentName::UnflattenFromString(activityName, (IComponentName**)&mActivity);
    mTime = time;
    mWeight = weight;
}

ActivityChooserModel::HistoricalRecord::HistoricalRecord(
    /* [in] */ IComponentName* activityName,
    /* [in] */ Int64 time,
    /* [in] */ Float weight)
{
    mActivity = activityName;
    mTime = time;
    mWeight = weight;
}

ECode ActivityChooserModel::HistoricalRecord::GetHashCode(
    /* [out] */ Int32* code)
{
    Int32 prime = 31;
    Int32 result = 1;
    Int32 activityHash = 0;
    if(mActivity != NULL)
        mActivity->GetHashCode(&activityHash);
    result = prime * result + activityHash;
    UInt64 tmp = *((UInt64*)(&mTime));
    result = prime * result + (Int32) (mTime ^ (tmp >> 32));
    result = prime * result + FloatToIntBits(mWeight);
    *code = result;
    return NOERROR;
}

ECode ActivityChooserModel::HistoricalRecord::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* res)
{
    if(obj == this) {
        return TRUE;
    }
    if (obj == NULL) {
        return FALSE;
    }
    if (IHistoricalRecord::Probe(obj) == NULL) {
        return FALSE;
    }
    HistoricalRecord* other = (HistoricalRecord*) obj;
    Boolean activityEquals;
    if (mActivity == NULL) {
        if (other->mActivity != NULL) {
            return FALSE;
        }
    } else if (mActivity->Equals(other->mActivity, &activityEquals), !activityEquals) {
        return FALSE;
    }
    if (mTime != other->mTime) {
        return FALSE;
    }

    if (FloatToIntBits(mWeight) != FloatToIntBits(other->mWeight)) {
        return FALSE;
    }
    return TRUE;
}

ECode ActivityChooserModel::HistoricalRecord::ToString(
    /* [out] */ String* str)
{
    StringBuilder builder;
    builder += "[";
    builder += "; activity:";
    String strBuf;
    mActivity->ToString(&strBuf);
    builder += strBuf;
    builder += "; time:";
    builder += mTime;
    builder += "; weight:";
    AutoPtr<IBigDecimal> bigDecimal;
    CBigDecimal::New(mWeight, (IBigDecimal**)&bigDecimal);
    bigDecimal->ToString(&strBuf);
    builder += strBuf;
    builder += "]";
    *str = builder.ToString();
    return NOERROR;
}

ActivityChooserModel::ActivityResolveInfo::ActivityResolveInfo(
    /* [in] */ IResolveInfo* resolveInfo,
    /* [in] */ ActivityChooserModel* host)
{
    mResolveInfo = resolveInfo;
    mHost = host;
}

ECode ActivityChooserModel::ActivityResolveInfo::GetHashCode(
    /* [out] */ Int32* hash)
{
    *hash = 31 + FloatToIntBits(mWeight);
    return NOERROR;
}

ECode ActivityChooserModel::ActivityResolveInfo::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* rst)
{
    if (obj == this) {
        return TRUE;
    }
    if (obj == NULL) {
        return FALSE;
    }
    if (IActivityResolveInfo::Probe(obj) == NULL) {
        return FALSE;
    }
    ActivityResolveInfo* other = (ActivityResolveInfo*)(IActivityResolveInfo::Probe(obj));
    if (FloatToIntBits(mWeight) != FloatToIntBits(other->mWeight)) {
        return FALSE;
    }
    return TRUE;
}

ECode ActivityChooserModel::ActivityResolveInfo::CompareTo(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32* rst)
{
    AutoPtr<IActivityResolveInfo> other = IActivityResolveInfo::Probe(obj);
    assert(other != NULL);
    ActivityResolveInfo* pTmp = (ActivityResolveInfo*)other.Get();
    *rst = Elastos::Core::Math::Compare(pTmp->mWeight, mWeight);
    return NOERROR;
}

ECode ActivityChooserModel::ActivityResolveInfo::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

ActivityChooserModel::DefaultSorter::DefaultSorter(
    /* [in] */ ActivityChooserModel* host)
{
    mHost = host;
}

ECode ActivityChooserModel::DefaultSorter::Sort(
    /* [in] */ IIntent* intent,
    /* [in] */ IObjectContainer* activities,
    /* [in] */ IObjectContainer* historicalRecords)
{
    mPackageNameToActivityMap.Clear();
    AutoPtr<IObjectEnumerator> em;
    activities->GetObjectEnumerator((IObjectEnumerator**)&em);
    Int32 count;
    activities->GetObjectCount(&count);
    Boolean hasNext;
    AutoPtr<ArrayOf<IActivityResolveInfo*> > infoArr = ArrayOf<IActivityResolveInfo*>::Alloc(count);
    Int32 i = 0;
    while (em->MoveNext(&hasNext), hasNext)
    {
        AutoPtr<IInterface> cur;
        em->Current((IInterface**)&cur);
        AutoPtr<IActivityResolveInfo> activityTemp = IActivityResolveInfo::Probe(cur);
        assert(activityTemp != NULL);
        AutoPtr<ActivityResolveInfo> activity = (ActivityResolveInfo*)activityTemp.Get();
        AutoPtr<IActivityInfo> aInfo;
        activity->mResolveInfo->GetActivityInfo((IActivityInfo**)&aInfo);
        String packageName;
        aInfo->GetPackageName(&packageName);
        mPackageNameToActivityMap[packageName] = activity;
        infoArr->Set(i++, activityTemp);
        activities->Remove(cur);
    }

    historicalRecords->GetObjectCount(&count);
    AutoPtr<ArrayOf<IHistoricalRecord*> > recordArr = ArrayOf<IHistoricalRecord*>::Alloc(count);
    em = NULL;
    historicalRecords->GetObjectEnumerator((IObjectEnumerator**)&em);
    i = 0;
    while (em->MoveNext(&hasNext), hasNext)
    {
        AutoPtr<IInterface> cur;
        em->Current((IInterface**)&cur);
        AutoPtr<IHistoricalRecord> record = IHistoricalRecord::Probe(cur);
        recordArr->Set(i++, record);
    }
    Int32 lastShareIndex = count - 1;
    Float nextRecordWeight = 1;
    for (i = lastShareIndex; i >= 0; i--)
    {
        AutoPtr<IHistoricalRecord> recordTmp = (*recordArr)[i];
        AutoPtr<HistoricalRecord> record = (HistoricalRecord*)recordTmp.Get();
        String packageName;
        record->mActivity->GetPackageName(&packageName);
        HashMap<String, AutoPtr<ActivityResolveInfo> >::Iterator it =
            mPackageNameToActivityMap.Find(packageName);
        if(it != mPackageNameToActivityMap.End() && it->mSecond != NULL)
        {
            AutoPtr<ActivityResolveInfo> activity = it->mSecond;
            it->mSecond->mWeight += record->mWeight * nextRecordWeight;
            nextRecordWeight = nextRecordWeight * WEIGHT_DECAY_COEFFICIENT;
        }
    }

    Int32 size = infoArr->GetLength() - 1;
    for(i = 0; i < size; i++)
    {
        for(Int32 j = 0; j < size -i; j++)
        {
            Int32 compare;
            (*infoArr)[j]->CompareTo((*infoArr)[j+1], &compare);
            if(compare > 0)
            {
                AutoPtr<IActivityResolveInfo> temp = (*infoArr)[j];
                infoArr->Set(j, (*infoArr)[j+1]);
                infoArr->Set(j+1, temp);
            }
        }
    }
    for(i = 0; i < infoArr->GetLength(); i++)
    {
        activities->Add((*infoArr)[i]);
    }
    return NOERROR;
}

ActivityChooserModel::PersistHistoryAsyncTask::PersistHistoryAsyncTask(
    /* [in] */ ActivityChooserModel* host)
{
    mHost = host;
}

AutoPtr<IInterface> ActivityChooserModel::PersistHistoryAsyncTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* args)
{
    AutoPtr<IObjectContainer> historicalRecords = IObjectContainer::Probe((*args)[0]);
    assert(historicalRecords != NULL);
    String historyFileName;
    AutoPtr<ICharSequence> csq = ICharSequence::Probe((*args)[1]);
    csq->ToString(&historyFileName);
    AutoPtr<IFileOutputStream> fos;
    AutoPtr<IObjectEnumerator> em;
    Boolean hasNext;

    // try {
    ECode ec = mHost->mContext->OpenFileOutput(historyFileName, IContext::MODE_PRIVATE, (IFileOutputStream**)&fos);
    if(FAILED(ec))
    {
        return NULL;
    }
    // } catch (FileNotFoundException fnfe) {
        // Log.e(LOG_TAG, "Error writing historical recrod file: " + hostoryFileName, fnfe);
        // return null;
    // }

    assert("TODO" && 0);
    AutoPtr<IXmlSerializer> serializer;// = Xml::NewSerializer();

    // try {
        if (FAILED(serializer->SetOutput(fos, String(NULL)))) {
            goto finally;
        }

        if (FAILED(serializer->StartDocument(String("UTF-8"), TRUE))) {
            goto finally;
        }
        if (FAILED(serializer->WriteStartTag(String(NULL), TAG_HISTORICAL_RECORDS))) {
            goto finally;
        }

        historicalRecords->GetObjectEnumerator((IObjectEnumerator**)&em);
        while (em->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> cur;
            em->Current((IInterface**)&cur);
            AutoPtr<IHistoricalRecord> recordTmp = IHistoricalRecord::Probe(cur);
            AutoPtr<HistoricalRecord> record = (HistoricalRecord*)recordTmp.Get();
            historicalRecords->Remove(cur);
            if (FAILED(serializer->WriteStartTag(String(NULL), TAG_HISTORICAL_RECORD))) {
                goto finally;
            }
            String str;
            record->mActivity->FlattenToString(&str);
            if (FAILED(serializer->WriteAttribute(String(NULL), ATTRIBUTE_ACTIVITY, str))) {
                goto finally;
            }
            String time, weight;
            time = StringUtils::ToString(record->mTime);
            weight = StringUtils::FloatToString(record->mWeight);
            if (FAILED(serializer->WriteAttribute(String(NULL), ATTRIBUTE_TIME, time))) {
                goto finally;
            }
            if (FAILED(serializer->WriteAttribute(String(NULL), ATTRIBUTE_WEIGHT, weight))) {
                goto finally;
            }
            if (FAILED(serializer->WriteEndTag(String(NULL), TAG_HISTORICAL_RECORD))) {
                goto finally;
            }
            // if (DEBUG) {
            //     Log.i(LOG_TAG, "Wrote " + record.toString());
            // }
        }

        if (FAILED(serializer->WriteEndTag(String(NULL), TAG_HISTORICAL_RECORDS))) {
            goto finally;
        }
        if (FAILED(serializer->EndDocument())) {
            goto finally;
        }

    //     if (DEBUG) {
    //         Log.i(LOG_TAG, "Wrote " + recordCount + " historical records.");
    //     }
    // } catch (IllegalArgumentException iae) {
    //     Log.e(LOG_TAG, "Error writing historical recrod file: " + mHistoryFileName, iae);
    // } catch (IllegalStateException ise) {
    //     Log.e(LOG_TAG, "Error writing historical recrod file: " + mHistoryFileName, ise);
    // } catch (IOException ioe) {
    //     Log.e(LOG_TAG, "Error writing historical recrod file: " + mHistoryFileName, ioe);
    // } finally {
    finally:
        mHost->mCanReadHistoricalData = TRUE;
        if (fos != NULL) {
            // try {
                fos->Close();
            // } catch (IOException e) {
                /* ignore */
            // }
        }
    // }
    return NULL;
}

ActivityChooserModel::DataModelPackageMonitor::DataModelPackageMonitor(
    /* [in] */ ActivityChooserModel* host)
{
    mHost = host;
}

ECode ActivityChooserModel::DataModelPackageMonitor::OnSomePackagesChanged()
{
    mHost->mReloadActivities = TRUE;
    return NOERROR;
}

PInterface ActivityChooserModel::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    } else if (riid == EIID_IObservable) {
        return (IObservable*)this;
    } else if (riid == EIID_IDataSetObservable) {
        return (IDataSetObservable*)this;
    } else if (riid == EIID_IActivityChooserModel) {
        return (IActivityChooserModel*)this;
    }
    return NULL;
}

UInt32 ActivityChooserModel::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ActivityChooserModel::Release()
{
    return ElRefBase::Release();
}

ECode ActivityChooserModel::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    if (object == (IInterface*)(IActivityChooserModel*)this)
    {
        *iid = EIID_IActivityChooserModel;
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

AutoPtr<IActivityChooserModel> ActivityChooserModel::Get(
    /* [in] */ IContext* context,
    /* [in] */ const String& historyFileName)
{
    AutoLock lock(sRegistryLock);
    HashMap<String, AutoPtr<IActivityChooserModel> >::Iterator it =
        sDataModelRegistry.Find(historyFileName);
    AutoPtr<IActivityChooserModel> dataModel;
    if(it != sDataModelRegistry.End() && it->mSecond != NULL)
    {
        dataModel = it->mSecond;
    } else {
        dataModel = new ActivityChooserModel(context, historyFileName);
        sDataModelRegistry[historyFileName] = dataModel;
    }
    return dataModel;
}

ActivityChooserModel::~ActivityChooserModel()
{
    mPackageMonitor->Unregister();
}

ECode ActivityChooserModel::SetIntent(
    /* [in] */ IIntent* intent)
{
    AutoLock lock(mInstanceLock);
    if (intent == mIntent) {
        return NOERROR;
    }
    mIntent = intent;
    mReloadActivities = TRUE;
    EnsureConsistentState();
    return NOERROR;
}

ECode ActivityChooserModel::GetIntent(
    /* [out] */ IIntent** intent)
{
    AutoLock lock(mInstanceLock);
    *intent = mIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode ActivityChooserModel::GetActivityCount(
    /* [out] */ Int32* rst)
{
    AutoLock lock(mInstanceLock);
    EnsureConsistentState();
    *rst = mActivities.GetSize();
    return NOERROR;
}

ECode ActivityChooserModel::GetActivity(
    /* [in] */ Int32 index,
    /* [out] */ IResolveInfo** activity)
{
    AutoLock lock(mInstanceLock);
    EnsureConsistentState();
    *activity = mActivities[index]->mResolveInfo;
    REFCOUNT_ADD(*activity);
    return NOERROR;
}

ECode ActivityChooserModel::GetActivityIndex(
    /* [in] */ IResolveInfo* activity,
    /* [out] */ Int32* rst)
{
    AutoLock lock(mInstanceLock);
    EnsureConsistentState();
    List<AutoPtr<ActivityResolveInfo> >::Iterator it =
        mActivities.Begin();
    Int32 i = 0 ;
    for (; it != mActivities.End(); it++, i++)
    {
        AutoPtr<ActivityResolveInfo> currentActivity = *it;
        if (activity == currentActivity->mResolveInfo)
        {
            *rst = i;
            return NOERROR;
        }
    }
    *rst = INVALID_INDEX;
    return NOERROR;
}

ECode ActivityChooserModel::ChooseActivity(
    /* [in] */ Int32 index,
    /* [out] */ IIntent** intent)
{
    AutoLock lock(mInstanceLock);
    if (mIntent == NULL) {
        *intent = NULL;
        return NOERROR;
    }

    EnsureConsistentState();

    AutoPtr<ActivityResolveInfo> chosenActivity = mActivities[index];

    AutoPtr<IComponentName> chosenName;
    AutoPtr<IActivityInfo> aInfo;
    chosenActivity->mResolveInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    String packageName, name;
    aInfo->GetPackageName(&packageName);
    aInfo->GetName(&name);
    CComponentName::New(packageName, name, (IComponentName**)&chosenName);
    AutoPtr<IIntent> choiceIntent;
    CIntent::New(mIntent, (IIntent**)&choiceIntent);
    choiceIntent->SetComponent(chosenName);

    if (mActivityChoserModelPolicy != NULL)
    {
        AutoPtr<IIntent> choiceIntentCopy;
        CIntent::New(choiceIntent, (IIntent**)&choiceIntent);
        Boolean handled;
        mActivityChoserModelPolicy->OnChooseActivity(this, choiceIntentCopy, &handled);
        if (handled) {
            *intent = NULL;
            return NOERROR;
        }
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    AutoPtr<HistoricalRecord> historicalRecord =
        new HistoricalRecord(chosenName, millis, DEFAULT_HISTORICAL_RECORD_WEIGHT);
    AddHisoricalRecord(historicalRecord);
    *intent = choiceIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode ActivityChooserModel::SetOnChooseActivityListener(
    /* [in] */ IOnChooseActivityListener* listener)
{
    AutoLock lock(mInstanceLock);
    mActivityChoserModelPolicy = listener;
    return NOERROR;
}

ECode ActivityChooserModel::GetDefaultActivity(
    /* [out] */ IResolveInfo** rInfo)
{
    AutoLock lock(mInstanceLock);
    EnsureConsistentState();
    if (!mActivities.IsEmpty()) {
        *rInfo = mActivities[0]->mResolveInfo;
        REFCOUNT_ADD(*rInfo);
        return NOERROR;
    }
    *rInfo = NULL;
    return NOERROR;
}

ECode ActivityChooserModel::SetDefaultActivity(
    /* [in] */ Int32 index)
{
    AutoLock lock(mInstanceLock);
    EnsureConsistentState();
    AutoPtr<ActivityResolveInfo> newDefaultActivity = mActivities[index];
    AutoPtr<ActivityResolveInfo> oldDefaultActivity = mActivities[0];

    Float weight;
    if (oldDefaultActivity != NULL) {
        // Add a record with weight enough to boost the chosen at the top.
        weight = oldDefaultActivity->mWeight - newDefaultActivity->mWeight
            + DEFAULT_ACTIVITY_INFLATION;
    } else {
        weight = DEFAULT_HISTORICAL_RECORD_WEIGHT;
    }

    AutoPtr<IComponentName> defaultName;
    AutoPtr<IActivityInfo> aInfo;
    newDefaultActivity->mResolveInfo->GetActivityInfo((IActivityInfo**)&aInfo);
    String packageName, name;
    aInfo->GetPackageName(&packageName);
    aInfo->GetName(&name);
    CComponentName::New(packageName, name, (IComponentName**)&defaultName);
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    AutoPtr<HistoricalRecord> historicalRecord = new HistoricalRecord(defaultName, millis, weight);
    AddHisoricalRecord(historicalRecord);
    return NOERROR;
}

ECode ActivityChooserModel::SetActivitySorter(
    /* [in] */ IActivitySorter* activitySorter)
{
    AutoLock lock(mInstanceLock);
    if (activitySorter == mActivitySorter) {
        return NOERROR;
    }
    mActivitySorter = (DefaultSorter*)activitySorter;
    if (SortActivitiesIfNeeded()) {
        NotifyChanged();
    }
    return NOERROR;
}

ECode ActivityChooserModel::SetHistoryMaxSize(
    /* [in] */ Int32 historyMaxSize)
{
    AutoLock lock(mInstanceLock);
    if (mHistoryMaxSize == historyMaxSize) {
        return NOERROR;
    }
    mHistoryMaxSize = historyMaxSize;
    PruneExcessiveHistoricalRecordsIfNeeded();
    if (SortActivitiesIfNeeded()) {
        NotifyChanged();
    }
    return NOERROR;
}

ECode ActivityChooserModel::GetHistoryMaxSize(
    /* [out] */ Int32* size)
{
    AutoLock lock(mInstanceLock);
    *size = mHistoryMaxSize;
    return NOERROR;
}

ECode ActivityChooserModel::GetHistorySize(
        /*[out] */ Int32* size)
{
    AutoLock lock(mInstanceLock);
    return mHistoryMaxSize;
}

ECode ActivityChooserModel::RegisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::RegisterObserver(observer);
}

ECode ActivityChooserModel::UnregisterObserver(
    /* [in] */ IInterface* observer)
{
    return DataSetObservable::UnregisterObserver(observer);
}

ECode ActivityChooserModel::UnregisterAll()
{
    return DataSetObservable::UnregisterAll();
}

ECode ActivityChooserModel::NotifyChanged()
{
    return DataSetObservable::NotifyChanged();
}

ECode ActivityChooserModel::NotifyInvalidated()
{
    return DataSetObservable::NotifyInvalidated();
}

ActivityChooserModel::ActivityChooserModel(
    /* [in] */ IContext* context,
    /* [in] */ const String& historyFileName)
{
    mPackageMonitor = new DataModelPackageMonitor(this);
    mActivitySorter = new DefaultSorter(this);
    mHistoryMaxSize = DEFAULT_HISTORY_MAX_LENGTH;
    mCanReadHistoricalData = TRUE;
    mReadShareHistoryCalled = FALSE;
    mHistoricalRecordsChanged = TRUE;
    mReloadActivities = FALSE;

    context->GetApplicationContext((IContext**)&mContext);
    if (!TextUtils::IsEmpty(historyFileName) && !historyFileName.EndWith(HISTORY_FILE_EXTENSION)) {
        mHistoryFileName = historyFileName + HISTORY_FILE_EXTENSION;
    } else {
        mHistoryFileName = historyFileName;
    }
    mPackageMonitor->Register(mContext, NULL, TRUE);
}

ECode ActivityChooserModel::PersistHistoricalDataIfNeeded()
{
    if (!mReadShareHistoryCalled) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mHistoricalRecordsChanged) {
        return NOERROR;
    }
    mHistoricalRecordsChanged = FALSE;
    if (!TextUtils::IsEmpty(mHistoryFileName)) {
        AutoPtr<IObjectContainer> param1;
        CObjectContainer::New((IObjectContainer**)&param1);
        List<AutoPtr<HistoricalRecord> >::Iterator it = mHistoricalRecords.Begin();
        for (; it != mHistoricalRecords.End(); it++) {
            param1->Add(*it);
        }
        AutoPtr<ICharSequence> param2;
        CStringWrapper::New(mHistoryFileName, (ICharSequence**)&param2);
        AutoPtr<ArrayOf<IInterface*> > params = ArrayOf<IInterface*>::Alloc(2);
        params->Set(0, param1);
        params->Set(1, param2);
        AutoPtr<PersistHistoryAsyncTask> task = new PersistHistoryAsyncTask(this);
        task->ExecuteOnExecutor(AsyncTask::SERIAL_EXECUTOR, params);
    }
    return NOERROR;
}

ECode ActivityChooserModel::EnsureConsistentState()
{
    Boolean stateChanged = LoadActivitiesIfNeeded();
    stateChanged |= ReadHistoricalDataIfNeeded();
    PruneExcessiveHistoricalRecordsIfNeeded();
    if (stateChanged) {
        SortActivitiesIfNeeded();
        NotifyChanged();
    }
    return NOERROR;
}

Boolean ActivityChooserModel::SortActivitiesIfNeeded()
{
    if (mActivitySorter != NULL && mIntent != NULL
        && !mActivities.IsEmpty() && !mHistoricalRecords.IsEmpty()) {
        AutoPtr<IObjectContainer> activities = TransfromList(mActivities);
        AutoPtr<IObjectContainer> historicalRecords = TransfromList(mHistoricalRecords);
        mActivitySorter->Sort(mIntent, activities, historicalRecords);
        mActivities = TransfromContainer<ActivityResolveInfo, IActivityResolveInfo>(activities.Get());
        mHistoricalRecords = TransfromContainer<HistoricalRecord, IHistoricalRecord>(historicalRecords.Get());
        return TRUE;
    }
    return FALSE;
}

Boolean ActivityChooserModel::LoadActivitiesIfNeeded()
{
    if (mReloadActivities && mIntent != NULL) {
        mReloadActivities = FALSE;
        mActivities.Clear();
        AutoPtr<IPackageManager> pmgr;
        mContext->GetPackageManager((IPackageManager**)&pmgr);
        AutoPtr<IObjectContainer> container;
        pmgr->QueryIntentActivities(mIntent, 0, (IObjectContainer**)&container);
        AutoPtr<IObjectEnumerator> em;
        container->GetObjectEnumerator((IObjectEnumerator**)&em);
        Boolean hasNext;
        while(em->MoveNext(&hasNext), hasNext)
        {
            AutoPtr<IInterface> tmp;
            em->Current((IInterface**)&tmp);
            AutoPtr<IResolveInfo> rInfo = IResolveInfo::Probe(tmp);
            assert(rInfo != NULL);
            AutoPtr<ActivityResolveInfo> arInfo = new ActivityResolveInfo(rInfo, this);
            mActivities.PushBack(arInfo);
        }
        return TRUE;
    }
    return FALSE;
}

Boolean ActivityChooserModel::ReadHistoricalDataIfNeeded()
{
    if (mCanReadHistoricalData && mHistoricalRecordsChanged &&
            !TextUtils::IsEmpty(mHistoryFileName))
    {
        mCanReadHistoricalData = FALSE;
        mReadShareHistoryCalled = TRUE;
        ReadHistoricalDataImpl();
        return TRUE;
    }
    return FALSE;
}

Boolean ActivityChooserModel::AddHisoricalRecord(
    /* [in] */ IHistoricalRecord* historicalRecord)
{
    // final boolean added = mHistoricalRecords.add(historicalRecord);
    // if (added) {
        mHistoricalRecords.PushBack((HistoricalRecord*)historicalRecord);
        mHistoricalRecordsChanged = TRUE;
        PruneExcessiveHistoricalRecordsIfNeeded();
        PersistHistoricalDataIfNeeded();
        SortActivitiesIfNeeded();
        NotifyChanged();
    // }
    return TRUE;
}

ECode ActivityChooserModel::PruneExcessiveHistoricalRecordsIfNeeded()
{
    Int32 pruneCount = mHistoricalRecords.GetSize() - mHistoryMaxSize;
    if (pruneCount <= 0)
    {
        return NOERROR;
    }
    mHistoricalRecordsChanged = TRUE;
    List<AutoPtr<HistoricalRecord> >::Iterator it = mHistoricalRecords.Begin();
    for (Int32 i = 0; i < pruneCount; i++)
    {
        List<AutoPtr<HistoricalRecord> >::Iterator oldIt = it++;
        mHistoricalRecords.Erase(oldIt);
    }
    return NOERROR;
}

ECode ActivityChooserModel::ReadHistoricalDataImpl()
{
    AutoPtr<IFileInputStream> fis;
    FAIL_RETURN(mContext->OpenFileInput(mHistoryFileName, (IFileInputStream**)&fis));
    AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
    String nodeName;
    String activity;
    String parseTmp;
    String name;
    Int64 time;
    Int32 type;

    if (FAILED(parser->SetInput(fis, String(NULL)))) {
        goto finally;
    }
    type = IXmlPullParser::START_DOCUMENT;
    while (type != IXmlPullParser::END_DOCUMENT && type != IXmlPullParser::START_TAG)
    {
        if (FAILED(parser->Next(&type))) {
            goto finally;
        }
    }
    parser->GetName(&name);
    if (!TAG_HISTORICAL_RECORDS.Equals(name)) {
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    mHistoricalRecords.Clear();
    while(TRUE) {
        if (FAILED(parser->Next(&type))) {
            goto finally;
        }
        if (type == IXmlPullParser::END_DOCUMENT) {
            break;
        }
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        parser->GetName(&nodeName);
        if (!TAG_HISTORICAL_RECORD.Equals(nodeName)) {
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        parser->GetAttributeValue(String(NULL), ATTRIBUTE_ACTIVITY, &activity);
        parser->GetAttributeValue(String(NULL), ATTRIBUTE_TIME, &parseTmp);
        time = StringUtils::ParseInt64(parseTmp);
        parser->GetAttributeValue(String(NULL), ATTRIBUTE_WEIGHT, &parseTmp);
        Float weight = StringUtils::ParseFloat(parseTmp);
        AutoPtr<HistoricalRecord> readRecord = new HistoricalRecord(activity, time, weight);
        mHistoricalRecords.PushBack(readRecord);
    }
    finally:
        if (fis != NULL) {
            fis->Close();
        }
        return NOERROR;
}

template<typename T>
AutoPtr<IObjectContainer> ActivityChooserModel::TransfromList(
        /* [in] */ List<AutoPtr<T> >& list)
{
    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    typename List<AutoPtr<T> >::Iterator it = list.Begin();
    for(; it != list.End(); it++)
    {
        container->Add(*it);
    }
    return container;
}

template<typename T, typename IT>
List<AutoPtr<T> > ActivityChooserModel::TransfromContainer(
        /* [in] */ IObjectContainer* container)
{
    List<AutoPtr<T> > list;
    AutoPtr<IObjectEnumerator> em;
    container->GetObjectEnumerator((IObjectEnumerator**)&em);
    Boolean next = FALSE;
    while(em->MoveNext(&next), next)
    {
        AutoPtr<IInterface> temp;
        em->Current((IInterface**)&temp);
        AutoPtr<T> obj = (T*)(IT::Probe(temp));
        list.PushBack(obj);
    }
    return list;
}

Int32 ActivityChooserModel::FloatToIntBits(
    /* [in] */ Float value)
{
    Int32 intBit;
    Boolean isNan = Elastos::Core::Math::IsNaN(value);
    if(isNan) {
        intBit = 0x7fc00000;
    }
    else {
        intBit = *((Int32*)(&value));
    }
    return intBit;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
