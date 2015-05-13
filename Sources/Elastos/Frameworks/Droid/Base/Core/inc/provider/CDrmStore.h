
#ifndef __CDRMSTORE_H__
#define __CDRMSTORE_H__

#include "_CDrmStore.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CDrmStore)
{
public:
    /**
     * Utility function for inserting a file into the DRM content provider.
     *
     * @param cr The content resolver to use
     * @param file The file to insert
     * @param title The title for the content (or null)
     * @return uri to the DRM record or null
     */
    CARAPI AddDrmFile(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IFile* file,
        /* [in] */ const String& title,
        /* [out] */ IIntent** record);

    /**
     * Utility function for inserting a file stream into the DRM content provider.
     *
     * @param cr The content resolver to use
     * @param fis The FileInputStream to insert
     * @param title The title for the content (or null)
     * @return uri to the DRM record or null
     */
    CARAPI AddDrmFileEx(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IFileInputStream* fis,
        /* [in] */ const String& title,
        /* [out] */ IIntent** record);

    /**
     * Utility function to enforce any permissions required to access DRM
     * content.
     *
     * @param context A context used for checking calling permission.
     */
    CARAPI EnforceAccessDrmPermission(
        /* [in] */ IContext* context);

private:
    static const CString TAG;

    /**
     * This is in the Manifest class of the drm provider, but that isn't visible
     * in the framework.
     */
    static const String ACCESS_DRM_PERMISSION;

};

}
}
}

#endif //__CDRMSTORE_H__