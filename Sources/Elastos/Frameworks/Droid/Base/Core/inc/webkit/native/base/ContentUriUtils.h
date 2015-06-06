
#ifndef __CONTENTURIUTILS_H__
#define __CONTENTURIUTILS_H__

// import android.content.ContentResolver;
// import android.content.Context;
// import android.database.Cursor;
// import android.net.Uri;
// import android.os.ParcelFileDescriptor;
// import android.util.Log;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * This class provides methods to access content URI schemes.
 */
class ContentUriUtils
{
public:
    /**
     * Opens the content URI for reading, and returns the file descriptor to
     * the caller. The caller is responsible for closing the file desciptor.
     *
     * @param context {@link Context} in interest
     * @param uriString the content URI to open
     * @returns file desciptor upon sucess, or -1 otherwise.
     */
    //@CalledByNative
    static CARAPI_(Int32) OpenContentUriForRead(
        /* [in] */ IContext* context,
        /* [in] */ String uriString);

    /**
     * Check whether a content URI exists.
     *
     * @param context {@link Context} in interest.
     * @param uriString the content URI to query.
     * @returns true if the uri exists, or false otherwise.
     */
    //@CalledByNative
    static CARAPI_(Boolean) ContentUriExists(
        /* [in] */ IContext* context,
        /* [in] */ String uriString);

    /**
     * Method to resolve the display name of a content URI.
     *
     * @param uri the content URI to be resolved.
     * @param contentResolver the content resolver to query.
     * @param columnField the column field to query.
     * @returns the display name of the @code uri if present in the database
     *  or an empty string otherwise.
     */
    static CARAPI_(String) GetDisplayName(
        /* [in] */ IUri* uri,
        /* [in] */ IContentResolver* contentResolver,
        /* [in] */ String columnField);

private:
    // Prevent instantiation.
    ContentUriUtils();

    static const String TAG;

    /**
     * Helper method to open a content URI and return the ParcelFileDescriptor.
     *
     * @param context {@link Context} in interest.
     * @param uriString the content URI to open.
     * @returns ParcelFileDescriptor of the content URI, or NULL if the file does not exist.
     */
    static CARAPI_(AutoPtr<IParcelFileDescriptor>) GetParcelFileDescriptor(
        /* [in] */ IContext* context,
        /* [in] */ String uriString);
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CONTENTURIUTILS_H__
