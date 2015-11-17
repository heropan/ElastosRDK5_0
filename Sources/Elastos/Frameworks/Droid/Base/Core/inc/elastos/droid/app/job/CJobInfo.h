
#ifndef __ELASTOS_DROID_APP_JOB_CJOBINFO_H__
#define __ELASTOS_DROID_APP_JOB_CJOBINFO_H__

#include "_Elastos_Droid_App_Job_CJobInfo.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::IParcelable;
using Elastos::Droid::Os::IPersistableBundle;

namespace Elastos {
namespace Droid {
namespace App {
namespace Job {

/**
 * Container of data passed to the {@link android.app.job.JobScheduler} fully encapsulating the
 * parameters required to schedule work against the calling application. These are constructed
 * using the {@link JobInfo.Builder}.
 * You must specify at least one sort of constraInt32 on the JobInfo object that you are creating.
 * The goal here is to provide the scheduler with high-level semantics about the work you want to
 * accomplish. Doing otherwise with throw an exception in your app.
 */
CarClass(CJobInfo)
    , public Object
    , public IJobInfo
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /** Default. */
    public static final Int32 NETWORK_TYPE_NONE = 0;
    /** This job requires network connectivity. */
    public static final Int32 NETWORK_TYPE_ANY = 1;
    /** This job requires network connectivity that is unmetered. */
    public static final Int32 NETWORK_TYPE_UNMETERED = 2;

    /**
     * Amount of backoff a job has initially by default, in milliseconds.
     */
    public static final Int64 DEFAULT_INITIAL_BACKOFF_MILLIS = 30000L;  // 30 seconds.

    /**
     * Maximum backoff we allow for a job, in milliseconds.
     */
    public static final Int64 MAX_BACKOFF_DELAY_MILLIS = 5 * 60 * 60 * 1000;  // 5 hours.

    /**
     * Linearly back-off a failed job. See
     * {@link android.app.job.JobInfo.Builder#setBackoffCriteria(Int64, Int32)}
     * retry_time(current_time, num_failures) =
     *     current_time + initial_backoff_millis * num_failures, num_failures >= 1
     */
    public static final Int32 BACKOFF_POLICY_LINEAR = 0;

    /**
     * Exponentially back-off a failed job. See
     * {@link android.app.job.JobInfo.Builder#setBackoffCriteria(Int64, Int32)}
     *
     * retry_time(current_time, num_failures) =
     *     current_time + initial_backoff_millis * 2 ^ (num_failures - 1), num_failures >= 1
     */
    public static final Int32 BACKOFF_POLICY_EXPONENTIAL = 1;

    /**
     * Default type of backoff.
     * @hide
     */
    public static final Int32 DEFAULT_BACKOFF_POLICY = BACKOFF_POLICY_EXPONENTIAL;

    /**
     * Unique job id associated with this class. This is assigned to your job by the scheduler.
     */
    public Int32 GetId() {
        return jobId;
    }

    /**
     * Bundle of extras which are returned to your application at execution time.
     */
    public IPersistableBundle* GetExtras() {
        return extras;
    }

    /**
     * Name of the service endpoInt32 that will be called back Int32o by the JobScheduler.
     */
    public IComponentName* GetService() {
        return service;
    }

    /**
     * Whether this job needs the device to be plugged in.
     */
    public Boolean IsRequireCharging() {
        return requireCharging;
    }

    /**
     * Whether this job needs the device to be in an Idle maInt32enance window.
     */
    public Boolean IsRequireDeviceIdle() {
        return requireDeviceIdle;
    }

    /**
     * One of {@link android.app.job.JobInfo#NETWORK_TYPE_ANY},
     * {@link android.app.job.JobInfo#NETWORK_TYPE_NONE}, or
     * {@link android.app.job.JobInfo#NETWORK_TYPE_UNMETERED}.
     */
    public Int32 GetNetworkType() {
        return networkType;
    }

    /**
     * Set for a job that does not recur periodically, to specify a delay after which the job
     * will be eligible for execution. This value is not set if the job recurs periodically.
     */
    public Int64 GetMinLatencyMillis() {
        return minLatencyMillis;
    }

    /**
     * See {@link Builder#setOverrideDeadline(Int64)}. This value is not set if the job recurs
     * periodically.
     */
    public Int64 GetMaxExecutionDelayMillis() {
        return maxExecutionDelayMillis;
    }

    /**
     * Track whether this job will repeat with a given period.
     */
    public Boolean IsPeriodic() {
        return isPeriodic;
    }

    /**
     * @return Whether or not this job should be persisted across device reboots.
     */
    public Boolean IsPersisted() {
        return isPersisted;
    }

    /**
     * Set to the Int32erval between occurrences of this job. This value is <b>not</b> set if the
     * job does not recur periodically.
     */
    public Int64 GetIntervalMillis() {
        return Int32ervalMillis;
    }

    /**
     * The amount of time the JobScheduler will wait before rescheduling a failed job. This value
     * will be increased depending on the backoff policy specified at job creation time. Defaults
     * to 5 seconds.
     */
    public Int64 GetInitialBackoffMillis() {
        return initialBackoffMillis;
    }

    /**
     * One of either {@link android.app.job.JobInfo#BACKOFF_POLICY_EXPONENTIAL}, or
     * {@link android.app.job.JobInfo#BACKOFF_POLICY_LINEAR}, depending on which criteria you set
     * when creating this job.
     */
    public Int32 GetBackoffPolicy() {
        return backoffPolicy;
    }

    /**
     * User can specify an early constraInt32 of 0L, which is valid, so we keep track of whether the
     * function was called at all.
     * @hide
     */
    public Boolean hasEarlyConstraInt32() {
        return hasEarlyConstraInt32;
    }

    /**
     * User can specify a late constraInt32 of 0L, which is valid, so we keep track of whether the
     * function was called at all.
     * @hide
     */
    public Boolean hasLateConstraInt32() {
        return hasLateConstraInt32;
    }

    private JobInfo(Parcel in) {
        jobId = in->ReadInt32();
        extras = in->ReadPersistableBundle();
        service = in->ReadParcelable(null);
        requireCharging = in->ReadInt32() == 1;
        requireDeviceIdle = in->ReadInt32() == 1;
        networkType = in->ReadInt32();
        minLatencyMillis = in->ReadInt64();
        maxExecutionDelayMillis = in->ReadInt64();
        isPeriodic = in->ReadInt32() == 1;
        isPersisted = in->ReadInt32() == 1;
        Int32ervalMillis = in->ReadInt64();
        initialBackoffMillis = in->ReadInt64();
        backoffPolicy = in->ReadInt32();
        hasEarlyConstraInt32 = in->ReadInt32() == 1;
        hasLateConstraInt32 = in->ReadInt32() == 1;
    }

    private JobInfo(JobInfo.Builder b) {
        jobId = b.mJobId;
        extras = b.mExtras;
        service = b.mJobService;
        requireCharging = b.mRequiresCharging;
        requireDeviceIdle = b.mRequiresDeviceIdle;
        networkType = b.mNetworkType;
        minLatencyMillis = b.mMinLatencyMillis;
        maxExecutionDelayMillis = b.mMaxExecutionDelayMillis;
        isPeriodic = b.mIsPeriodic;
        isPersisted = b.mIsPersisted;
        Int32ervalMillis = b.mIntervalMillis;
        initialBackoffMillis = b.mInitialBackoffMillis;
        backoffPolicy = b.mBackoffPolicy;
        hasEarlyConstraInt32 = b.mHasEarlyConstraInt32;
        hasLateConstraInt32 = b.mHasLateConstraInt32;
    }

    @Override
    public Int32 describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel out, Int32 flags) {
        out->WriteInt32(jobId);
        out->WritePersistableBundle(extras);
        out->WriteParcelable(service, flags);
        out->WriteInt32(requireCharging ? 1 : 0);
        out->WriteInt32(requireDeviceIdle ? 1 : 0);
        out->WriteInt32(networkType);
        out->WriteInt64(minLatencyMillis);
        out->WriteInt64(maxExecutionDelayMillis);
        out->WriteInt32(isPeriodic ? 1 : 0);
        out->WriteInt32(isPersisted ? 1 : 0);
        out->WriteInt64(Int32ervalMillis);
        out->WriteInt64(initialBackoffMillis);
        out->WriteInt32(backoffPolicy);
        out->WriteInt32(hasEarlyConstraInt32 ? 1 : 0);
        out->WriteInt32(hasLateConstraInt32 ? 1 : 0);
    }

    public static final Creator<JobInfo> CREATOR = new Creator<JobInfo>() {
        @Override
        public JobInfo createFromParcel(Parcel in) {
            return new JobInfo(in);
        }

        @Override
        public JobInfo[] newArray(Int32 size) {
            return new JobInfo[size];
        }
    };

    @Override
    public String toString() {
        return "(job:" + jobId + "/" + service.flattenToShortString() + ")";
    }

private:

    Int32 jobId;
    IPersistableBundle* extras;
    IComponentName* service;
    Boolean requireCharging;
    Boolean requireDeviceIdle;
    Boolean hasEarlyConstraInt32;
    Boolean hasLateConstraInt32;
    Int32 networkType;
    Int64 minLatencyMillis;
    Int64 maxExecutionDelayMillis;
    Boolean isPeriodic;
    Boolean isPersisted;
    Int64 Int32ervalMillis;
    Int64 initialBackoffMillis;
    Int32 backoffPolicy;
};

/** Builder class for constructing {@link JobInfo} objects. */
class JobInfoBuilder
    : public Object
    , public IJobInfoBuilder
{
public:
    CAR_INTERFACE_DECL()

    /**
     * @param jobId Application-provided id for this job. Subsequent calls to cancel, or
     *               jobs created with the same jobId, will update the pre-existing job with
     *               the same id.
     * @param jobService The endpoInt32 that you implement that will receive the callback from the
     *            JobScheduler.
     */
    public Builder(Int32 jobId, IComponentName* jobService) {
        mJobService = jobService;
        mJobId = jobId;
    }

    /**
     * Set optional extras. This is persisted, so we only allow primitive types.
     * @param extras Bundle containing extras you want the scheduler to hold on to for you.
     */
    public CARAPI SetExtras(IPersistableBundle* extras) {
        mExtras = extras;
        return NOERROR;
    }

    /**
     * Set some description of the kind of network type your job needs to have.
     * Not calling this function means the network is not necessary, as the default is
     * {@link #NETWORK_TYPE_NONE}.
     * Bear in mind that calling this function defines network as a strict requirement for your
     * job. If the network requested is not available your job will never run. See
     * {@link #setOverrideDeadline(Int64)} to change this behaviour.
     */
    public CARAPI SetRequiredNetworkType(Int32 networkType) {
        mNetworkType = networkType;
        return NOERROR;
    }

    /**
     * Specify that to run this job, the device needs to be plugged in. This defaults to
     * false.
     * @param requiresCharging Whether or not the device is plugged in.
     */
    public CARAPI SetRequiresCharging(Boolean requiresCharging) {
        mRequiresCharging = requiresCharging;
        return NOERROR;
    }

    /**
     * Specify that to run, the job needs the device to be in idle mode. This defaults to
     * false.
     * <p>Idle mode is a loose definition provided by the system, which means that the device
     * is not in use, and has not been in use for some time. As such, it is a good time to
     * perform resource heavy jobs. Bear in mind that battery usage will still be attributed
     * to your application, and surfaced to the user in battery stats.</p>
     * @param requiresDeviceIdle Whether or not the device need be within an idle maInt32enance
     *                           window.
     */
    public CARAPI SetRequiresDeviceIdle(Boolean requiresDeviceIdle) {
        mRequiresDeviceIdle = requiresDeviceIdle;
        return NOERROR;
    }

    /**
     * Specify that this job should recur with the provided Int32erval, not more than once per
     * period. You have no control over when within this Int32erval this job will be executed,
     * only the guarantee that it will be executed at most once within this Int32erval.
     * Setting this function on the builder with {@link #setMinimumLatency(Int64)} or
     * {@link #setOverrideDeadline(Int64)} will result in an error.
     * @param Int32ervalMillis Millisecond Int32erval for which this job will repeat.
     */
    public CARAPI SetPeriodic(Int64 Int32ervalMillis) {
        mIsPeriodic = true;
        mIntervalMillis = Int32ervalMillis;
        mHasEarlyConstraInt32 = mHasLateConstraInt32 = true;
        return NOERROR;
    }

    /**
     * Specify that this job should be delayed by the provided amount of time.
     * Because it doesn't make sense setting this property on a periodic job, doing so will
     * throw an {@link java.lang.IllegalArgumentException} when
     * {@link android.app.job.JobInfo.Builder#build()} is called.
     * @param minLatencyMillis Milliseconds before which this job will not be considered for
     *                         execution.
     */
    public CARAPI SetMinimumLatency(Int64 minLatencyMillis) {
        mMinLatencyMillis = minLatencyMillis;
        mHasEarlyConstraInt32 = true;
        return NOERROR;
    }

    /**
     * Set deadline which is the maximum scheduling latency. The job will be run by this
     * deadline even if other requirements are not met. Because it doesn't make sense setting
     * this property on a periodic job, doing so will throw an
     * {@link java.lang.IllegalArgumentException} when
     * {@link android.app.job.JobInfo.Builder#build()} is called.
     */
    public CARAPI SetOverrideDeadline(Int64 maxExecutionDelayMillis) {
        mMaxExecutionDelayMillis = maxExecutionDelayMillis;
        mHasLateConstraInt32 = true;
        return NOERROR;
    }

    /**
     * Set up the back-off/retry policy.
     * This defaults to some respectable values: {30 seconds, Exponential}. We cap back-off at
     * 5hrs.
     * Note that trying to set a backoff criteria for a job with
     * {@link #setRequiresDeviceIdle(Boolean)} will throw an exception when you call build().
     * This is because back-off typically does not make sense for these types of jobs. See
     * {@link android.app.job.JobService#jobFinished(android.app.job.JobParameters, Boolean)}
     * for more description of the return value for the case of a job executing while in idle
     * mode.
     * @param initialBackoffMillis Millisecond time Int32erval to wait initially when job has
     *                             failed.
     * @param backoffPolicy is one of {@link #BACKOFF_POLICY_LINEAR} or
     * {@link #BACKOFF_POLICY_EXPONENTIAL}
     */
    public CARAPI SetBackoffCriteria(Int64 initialBackoffMillis, Int32 backoffPolicy) {
        mBackoffPolicySet = true;
        mInitialBackoffMillis = initialBackoffMillis;
        mBackoffPolicy = backoffPolicy;
        return NOERROR;
    }

    /**
     * Set whether or not to persist this job across device reboots. This will only have an
     * effect if your application holds the permission
     * {@link android.Manifest.permission#RECEIVE_BOOT_COMPLETED}. Otherwise an exception will
     * be thrown.
     * @param isPersisted True to indicate that the job will be written to disk and loaded at
     *                    boot.
     */
    public CARAPI SetPersisted(Boolean isPersisted) {
        mIsPersisted = isPersisted;
        return NOERROR;
    }

    /**
     * @return The job object to hand to the JobScheduler. This object is immutable.
     */
    public JobInfo build() {
        // Allow jobs with no constraInt32s - What am I, a database?
        if (!mHasEarlyConstraInt32 && !mHasLateConstraInt32 && !mRequiresCharging &&
                !mRequiresDeviceIdle && mNetworkType == NETWORK_TYPE_NONE) {
            throw new IllegalArgumentException("You're trying to build a job with no " +
                    "constraInt32s, this is not allowed.");
        }
        mExtras = new IPersistableBundle*(mExtras);  // Make our own copy.
        // Check that a deadline was not set on a periodic job.
        if (mIsPeriodic && (mMaxExecutionDelayMillis != 0L)) {
            throw new IllegalArgumentException("Can't call setOverrideDeadline() on a " +
                    "periodic job.");
        }
        if (mIsPeriodic && (mMinLatencyMillis != 0L)) {
            throw new IllegalArgumentException("Can't call setMinimumLatency() on a " +
                    "periodic job");
        }
        if (mBackoffPolicySet && mRequiresDeviceIdle) {
            throw new IllegalArgumentException("An idle mode job will not respect any" +
                    " back-off policy, so calling setBackoffCriteria with" +
                    " setRequiresDeviceIdle is an error.");
        }
        return new JobInfo(this);
    }

private:
    Int32 mJobId;
    IPersistableBundle* mExtras = IPersistableBundle*.EMPTY;
    IComponentName* mJobService;
    // Requirements.
    Boolean mRequiresCharging;
    Boolean mRequiresDeviceIdle;
    Int32 mNetworkType;
    Boolean mIsPersisted;
    // One-off parameters.
    Int64 mMinLatencyMillis;
    Int64 mMaxExecutionDelayMillis;
    // Periodic parameters.
    Boolean mIsPeriodic;
    Boolean mHasEarlyConstraInt32;
    Boolean mHasLateConstraInt32;
    Int64 mIntervalMillis;
    // Back-off parameters.
    Int64 mInitialBackoffMillis = DEFAULT_INITIAL_BACKOFF_MILLIS;
    Int32 mBackoffPolicy = DEFAULT_BACKOFF_POLICY;
    /** Easy way to track whether the client has tried to set a back-off policy. */
    Boolean mBackoffPolicySet = false;
};

} // namespace Job
} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_JOB_CJOBINFO_H__