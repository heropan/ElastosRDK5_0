

package android.app;

using Elastos::Droid::content.Intent;
using Elastos::Droid::content.pm.IPackageDeleteObserver2;

/** {@hide} */
public class PackageDeleteObserver {
    private final IPackageDeleteObserver2.Stub mBinder = new IPackageDeleteObserver2.Stub() {
        @Override
        public void onUserActionRequired(Intent intent) {
            PackageDeleteObserver.this.onUserActionRequired(intent);
        }

        @Override
        public void onPackageDeleted(String basePackageName, int returnCode, String msg) {
            PackageDeleteObserver.this.onPackageDeleted(basePackageName, returnCode, msg);
        }
    };

    /** {@hide} */
    public IPackageDeleteObserver2 getBinder() {
        return mBinder;
    }

    public void onUserActionRequired(Intent intent) {
    }

    public void onPackageDeleted(String basePackageName, int returnCode, String msg) {
    }
}
