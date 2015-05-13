
#ifndef __NATIVEUTIL_H__
#define __NATIVEUTIL_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Elastos::Droid::Content::IContext;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Webkit {

class NativeUtil
{
public:
    static CARAPI_(void) SetContext(
        /* [in] */ IContext* context);

    static CARAPI_(AutoPtr<IContext>) GetContext();

    /**
     * Called by JNI. Gets the application's database directory, excluding the trailing slash.
     * @return String The application's database directory
     */
    static CARAPI GetDatabaseDirectory(
        /* [out] */ String* directory);

    /**
     * Called by JNI. Gets the application's cache directory, excluding the trailing slash.
     * @return String The application's cache directory
     */
    static CARAPI GetCacheDirectory(
        /* [out] */ String* directory);

    /**
     * Called by JNI. Gets the application's package name.
     * @return String The application's package name
     */
    static CARAPI GetPackageName(
        /* [out] */ String* name);

    static CARAPI CanSatisfyMemoryAllocation(
        /* [in] */ Int64 bytesRequested,
        /* [out] */ Boolean* result);

    /**
     * Called by JNI. Calculates the size of an input stream by reading it.
     * @return long The size of the stream
     */
    static CARAPI_(Int64) ContentUrlSize(
        /* [in] */ const String& url);

    /**
     * Called by JNI.
     *
     * @return  Opened input stream to content
     * TODO: Make all content loading use this instead of BrowserFrame.java
     */
    static CARAPI_(AutoPtr<IInputStream>) ContentUrlStream(
        /* [in] */ const String& url);

    static CARAPI GetAutofillQueryUrl(
        /* [out] */ String* url);

private:
    NativeUtil() {} // Utility class, do not instantiate.

    static CARAPI CheckInitialized();

private:
    static const String LOGTAG;
    // Used by the Chromium HTTP stack.
    static String sDatabaseDirectory;
    static String sCacheDirectory;
    static AutoPtr<IContext> sContext;
    static const String ANDROID_CONTENT;
    static Mutex sLock;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__NATIVEUTIL_H__
