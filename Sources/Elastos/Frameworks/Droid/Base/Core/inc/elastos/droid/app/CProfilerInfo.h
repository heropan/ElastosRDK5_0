package android.app;

using Elastos::Droid::os.Parcel;
using Elastos::Droid::os.Parcelable;
using Elastos::Droid::os.ParcelFileDescriptor;

/**
 * System private API for passing profiler settings.
 *
 * {@hide}
 */
public class ProfilerInfo implements Parcelable {

    /* Name of profile output file. */
    public final String profileFile;

    /* File descriptor for profile output file, can be null. */
    public ParcelFileDescriptor profileFd;

    /* Indicates sample profiling when nonzero, interval in microseconds. */
    public final int samplingInterval;

    /* Automatically stop the profiler when the app goes idle. */
    public final boolean autoStopProfiler;

    public ProfilerInfo(String filename, ParcelFileDescriptor fd, int interval, boolean autoStop) {
        profileFile = filename;
        profileFd = fd;
        samplingInterval = interval;
        autoStopProfiler = autoStop;
    }

    public int describeContents() {
        if (profileFd != null) {
            return profileFd.describeContents();
        } else {
            return 0;
        }
    }

    public void writeToParcel(Parcel out, int flags) {
        out.writeString(profileFile);
        if (profileFd != null) {
            out.writeInt(1);
            profileFd.writeToParcel(out, flags);
        } else {
            out.writeInt(0);
        }
        out.writeInt(samplingInterval);
        out.writeInt(autoStopProfiler ? 1 : 0);
    }

    public static final Parcelable.Creator<ProfilerInfo> CREATOR =
            new Parcelable.Creator<ProfilerInfo>() {
        public ProfilerInfo createFromParcel(Parcel in) {
            return new ProfilerInfo(in);
        }

        public ProfilerInfo[] newArray(int size) {
            return new ProfilerInfo[size];
        }
    };

    private ProfilerInfo(Parcel in) {
        profileFile = in.readString();
        profileFd = in.readInt() != 0 ? ParcelFileDescriptor.CREATOR.createFromParcel(in) : null;
        samplingInterval = in.readInt();
        autoStopProfiler = in.readInt() != 0;
    }
}
