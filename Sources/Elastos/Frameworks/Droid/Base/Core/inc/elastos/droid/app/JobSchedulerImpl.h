

// in android.app so ContextImpl has package access
package android.app;

import android.app.job.JobInfo;
import android.app.job.JobScheduler;
import android.app.job.IJobScheduler;
import android.os.RemoteException;

import java.util.List;


/**
 * Concrete implementation of the JobScheduler interface
 * @hide
 */
public class JobSchedulerImpl extends JobScheduler {
    IJobScheduler mBinder;

    /* package */ JobSchedulerImpl(IJobScheduler binder) {
        mBinder = binder;
    }

    @Override
    public int schedule(JobInfo job) {
        try {
            return mBinder.schedule(job);
        } catch (RemoteException e) {
            return JobScheduler.RESULT_FAILURE;
        }
    }

    @Override
    public void cancel(int jobId) {
        try {
            mBinder.cancel(jobId);
        } catch (RemoteException e) {}

    }

    @Override
    public void cancelAll() {
        try {
            mBinder.cancelAll();
        } catch (RemoteException e) {}

    }

    @Override
    public List<JobInfo> getAllPendingJobs() {
        try {
            return mBinder.getAllPendingJobs();
        } catch (RemoteException e) {
            return null;
        }
    }
}
