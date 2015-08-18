
#ifndef __ELASTOS_DROID_MTP_CMTPDATABASE_H__
#define __ELASTOS_DROID_MTP_CMTPDATABASE_H__

#include "_Elastos_Droid_Mtp_CMtpDatabase.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::IInteger32;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIContentProvider;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Media::IMediaScanner;
using Elastos::Droid::Mtp::IMtpPropertyGroup;

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpDatabase)
{
public:
    CMtpDatabase();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ const String& volumeName,
        /* [in] */ const String& storagePath,
        /* [in] */ ArrayOf<String>* subDirectories);

    CARAPI AddStorage(
        /* [in] */ IMtpStorage* storage);

    CARAPI RemoveStorage(
        /* [in] */ IMtpStorage* storage);

    CARAPI GetNativeContext(
        /* [out] */ Int32* nativeContext);

protected:
    CARAPI_(void) Finalize();

public:
    CARAPI_(void) InitDeviceProperties(
        /* [in] */ IContext* context);

    CARAPI_(Boolean) InStorageSubDirectory(
        /* [in] */ const String& path);

    CARAPI_(Boolean) IsStorageSubDirectory(
        /* [in] */ const String& path);

    CARAPI_(Int32) BeginSendObject(
        /* [in] */ const String& path,
        /* [in] */ Int32 format,
        /* [in] */ Int32 parent,
        /* [in] */ Int32 storageId,
        /* [in] */ Int64 size,
        /* [in] */ Int64 modified);

    CARAPI_(void) EndSendObject(
        /* [in] */ const String& path,
        /* [in] */ Int32 handle,
        /* [in] */ Int32 format,
        /* [in] */ Boolean succeeded);

    CARAPI_(AutoPtr<ICursor>) CreateObjectQuery(
        /* [in] */ Int32 storageID,
        /* [in] */ Int32 format,
        /* [in] */ Int32 parent);

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetObjectList(
        /* [in] */ Int32 storageID,
        /* [in] */ Int32 format,
        /* [in] */ Int32 parent);

    CARAPI_(Int32) GetNumObjects(
        /* [in] */ Int32 storageID,
        /* [in] */ Int32 format,
        /* [in] */ Int32 parent);

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetSupportedPlaybackFormats();

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetSupportedCaptureFormats();

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetSupportedObjectProperties(
        /* [in] */ Int32 format);

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetSupportedDeviceProperties();

    CARAPI_(AutoPtr<IMtpPropertyList>) GetObjectPropertyList(
        /* [in] */ Int64 handle,
        /* [in] */ Int32 format,
        /* [in] */ Int64 property,
        /* [in] */ Int32 groupCode,
        /* [in] */ Int32 depth);

    CARAPI_(Int32) RenameFile(
        /* [in] */ Int32 handle,
        /* [in] */ const String& newName);

    CARAPI_(Int32) SetObjectProperty(
        /* [in] */ Int32 handle,
        /* [in] */ Int32 property,
        /* [in] */ Int64 intValue,
        /* [in] */ const String& stringValue);

    CARAPI_(Int32) GetDeviceProperty(
        /* [in] */ Int32 property,
        /* [in] */ ArrayOf<Int64>* outIntValue,
        /* [in] */ ArrayOf<Char32>* outStringValue);

    CARAPI_(Int32) SetDeviceProperty(
        /* [in] */ Int32 property,
        /* [in] */ Int64 intValue,
        /* [in] */ const String& stringValue);

    CARAPI_(Boolean) GetObjectInfo(
        /* [in] */ Int32 handle,
        /* [in] */ ArrayOf<Int32>* outStorageFormatParent,
        /* [in] */ ArrayOf<Char32>* outName,
        /* [in] */ ArrayOf<Int64>* outModified);

    CARAPI_(Int32) GetObjectFilePath(
        /* [in] */ Int32 handle,
        /* [in] */ ArrayOf<Char32>* outFilePath,
        /* [in] */ ArrayOf<Int64>* outFileLengthFormat);

    CARAPI_(Int32) DeleteFile(
        /* [in] */ Int32 handle);

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetObjectReferences(
        /* [in] */ Int32 handle);

    CARAPI_(Int32) SetObjectReferences(
        /* [in] */ Int32 handle,
        /* [in] */ ArrayOf<Int32>* references);

    CARAPI_(void) SessionStarted();

    CARAPI_(void) SessionEnded();

    CARAPI_(void) NativeSetup();

    CARAPI_(void) NativeFinalize();

private:
    static const String TAG;

    AutoPtr<IContext> mContext;
    AutoPtr<IIContentProvider> mMediaProvider;
    String mVolumeName;
    AutoPtr<IUri> mObjectsUri;
    // path to primary storage
    String mMediaStoragePath;
    // if not null, restrict all queries to these subdirectories
    AutoPtr<ArrayOf<String> > mSubDirectories;
    // where clause for restricting queries to files in mSubDirectories
    String mSubDirectoriesWhere;
    // where arguments for restricting queries to files in mSubDirectories
    AutoPtr<ArrayOf<String> > mSubDirectoriesWhereArgs;

    HashMap<String, AutoPtr<IMtpStorage> > mStorageMap;

    // cached property groups for single properties
    HashMap<AutoPtr<IInteger32>, AutoPtr<IMtpPropertyGroup> > mPropertyGroupsByProperty;

    // cached property groups for all properties for a given format
    HashMap<AutoPtr<IInteger32>, AutoPtr<IMtpPropertyGroup> > mPropertyGroupsByFormat;

    // true if the database has been modified in the current MTP session
    Boolean mDatabaseModified;

    // SharedPreferences for writable MTP device properties
    AutoPtr<ISharedPreferences> mDeviceProperties;

    static const Int32 DEVICE_PROPERTIES_DATABASE_VERSION;

    static ArrayOf<String>* ID_PROJECTION;
    static ArrayOf<String>* PATH_PROJECTION;
    static ArrayOf<String>* PATH_FORMAT_PROJECTION;
    static ArrayOf<String>* OBJECT_INFO_PROJECTION;

    static ArrayOf<Int32>* FILE_PROPERTIES;
    static ArrayOf<Int32>* AUDIO_PROPERTIES;
    static ArrayOf<Int32>* VIDEO_PROPERTIES;
    static ArrayOf<Int32>* IMAGE_PROPERTIES;
    static ArrayOf<Int32>* ALL_PROPERTIES;

    static const String ID_WHERE;
    static const String PATH_WHERE;

    static const String STORAGE_WHERE;
    static const String FORMAT_WHERE;
    static const String PARENT_WHERE;
    static const String STORAGE_FORMAT_WHERE;
    static const String STORAGE_PARENT_WHERE;
    static const String FORMAT_PARENT_WHERE;
    static const String STORAGE_FORMAT_PARENT_WHERE;

    AutoPtr<IMediaScanner> mMediaScanner;

    // used by the JNI code
    Int32 mNativeContext;
};

} // namespace Mtp
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MTP_CMTPDATABASE_H__
