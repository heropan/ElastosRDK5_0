
#include "provider/CDrmStore.h"


const CString CDrmStore::TAG = "CDrmStore";

const String CDrmStore::ACCESS_DRM_PERMISSION = String("android.permission.ACCESS_DRM");

ECode CDrmStore::AddDrmFile(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IFile* file,
    /* [in] */ const String& title,
    /* [out] */ IIntent** record)
{
//     FileInputStream fis = null;
//     Intent result = null;

//     try {
//         fis = new FileInputStream(file);
//         if (title == null) {
//             title = file.getName();
//             int lastDot = title.lastIndexOf('.');
//             if (lastDot > 0) {
//                 title = title.substring(0, lastDot);
//             }
//         }
//         result = addDrmFile(cr, fis, title);
//     } catch (Exception e) {
//         Log.e(TAG, "pushing file failed", e);
//     } finally {
//         try {
//             if (fis != null)
//                 fis.close();
//         } catch (IOException e) {
//             Log.e(TAG, "IOException in DrmStore.addDrmFile()", e);
//         }
//     }

//     return result;
}

ECode CDrmStore::AddDrmFile(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IFileInputStream* fis,
    /* [in] */ const String& title,
    /* [out] */ IIntent** record)
{
//     OutputStream os = null;
//     Intent result = null;

//     try {
//         DrmRawContent content = new DrmRawContent(fis, (int) fis.available(),
//                 DrmRawContent.DRM_MIMETYPE_MESSAGE_STRING);
//         String mimeType = content.getContentType();
//         long size = fis.getChannel().size();

//         DrmRightsManager manager = manager = DrmRightsManager.getInstance();
//         DrmRights rights = manager.queryRights(content);
//         InputStream stream = content.getContentInputStream(rights);

//         Uri contentUri = null;
//         if (mimeType.startsWith("audio/")) {
//             contentUri = DrmStore.Audio.CONTENT_URI;
//         } else if (mimeType.startsWith("image/")) {
//             contentUri = DrmStore.Images.CONTENT_URI;
//         } else {
//             Log.w(TAG, "unsupported mime type " + mimeType);
//         }

//         if (contentUri != null) {
//             ContentValues values = new ContentValues(3);
//             values.put(DrmStore.Columns.TITLE, title);
//             values.put(DrmStore.Columns.SIZE, size);
//             values.put(DrmStore.Columns.MIME_TYPE, mimeType);

//             Uri uri = cr.insert(contentUri, values);
//             if (uri != null) {
//                 os = cr.openOutputStream(uri);

//                 byte[] buffer = new byte[1000];
//                 int count;

//                 while ((count = stream.read(buffer)) != -1) {
//                     os.write(buffer, 0, count);
//                 }
//                 result = new Intent();
//                 result.setDataAndType(uri, mimeType);

//             }
//         }
//     } catch (Exception e) {
//         Log.e(TAG, "pushing file failed", e);
//     } finally {
//         try {
//             if (fis != null)
//                 fis.close();
//             if (os != null)
//                 os.close();
//         } catch (IOException e) {
//             Log.e(TAG, "IOException in DrmStore.addDrmFile()", e);
//         }
//     }

//     return result;
}

ECode CDrmStore::EnforceAccessDrmPermission(
    /* [in] */ IContext* context)
{
    Int32 perm;
    FAIL_RETURN(context->CheckCallingOrSelfPermission(ACCESS_DRM_PERMISSION, &perm));
    if (perm != IPackageManager::PERMISSION_GRANTED) {
        return E_SECURITY_EXCEPTION;
    }
    return NOERROR;
}