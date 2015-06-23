
#include "ext/frameworkext.h"
#include "CMtpDatabase.h"
#include <elastos/core/StringUtils.h>
#include "text/TextUtils.h"
#include "media/media/CMediaScanner.h"
#include "content/CIntent.h"
#include "content/CContentValues.h"
#include "media/mtp/CMtpPropertyList.h"
#include "media/mtp/CMtpPropertyGroup.h"
#include "MtpDatabase.h"
#include "MtpDataPacket.h"
#include "MtpObjectInfo.h"
#include "MtpProperty.h"
#include "MtpStringBuffer.h"
#include "MtpUtils.h"
#include "mtp.h"
#include "provider/CMediaStoreFiles.h"
#include "provider/CMediaStoreAudioPlaylists.h"

extern "C" {
#include "jhead.h"
}

using namespace android;
using Libcore::ICU::ILocale;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Core::ICharSequence;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::IStringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Media::CMediaScanner;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::ISharedPreferencesEditor;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Mtp::CMtpPropertyList;
using Elastos::Droid::Mtp::CMtpPropertyGroup;
using Elastos::Droid::Provider::IMediaStoreFilesFileColumns;
using Elastos::Droid::Provider::IMediaStoreFiles;
using Elastos::Droid::Provider::CMediaStoreFiles;
using Elastos::Droid::Provider::IMediaStore;
using Elastos::Droid::Provider::IMediaStoreAudioPlaylists;
using Elastos::Droid::Provider::CMediaStoreAudioPlaylists;
using Elastos::Droid::Provider::IMediaStoreMediaColumns;

namespace Elastos {
namespace Droid {
namespace Mtp {

class MyMtpDatabase : public MtpDatabase {
private:
    AutoPtr<CMtpDatabase>       mDatabase;
    AutoPtr<ArrayOf<Int32> >    mIntBuffer;
    AutoPtr<ArrayOf<Int64> >    mLongBuffer;
    AutoPtr<ArrayOf<Char32> >   mStringBuffer;

public:
                                    MyMtpDatabase(CMtpDatabase* client);
    virtual                         ~MyMtpDatabase();
    void                            cleanup();

    virtual MtpObjectHandle         beginSendObject(const char* path,
                                            MtpObjectFormat format,
                                            MtpObjectHandle parent,
                                            MtpStorageID storage,
                                            uint64_t size,
                                            time_t modified);

    virtual void                    endSendObject(const char* path,
                                            MtpObjectHandle handle,
                                            MtpObjectFormat format,
                                            bool succeeded);

    virtual MtpObjectHandleList*    getObjectList(MtpStorageID storageID,
                                    MtpObjectFormat format,
                                    MtpObjectHandle parent);

    virtual Int32                   getNumObjects(MtpStorageID storageID,
                                            MtpObjectFormat format,
                                            MtpObjectHandle parent);

    // callee should delete[] the results from these
    // results can be NULL
    virtual MtpObjectFormatList*    getSupportedPlaybackFormats();
    virtual MtpObjectFormatList*    getSupportedCaptureFormats();
    virtual MtpObjectPropertyList*  getSupportedObjectProperties(MtpObjectFormat format);
    virtual MtpDevicePropertyList*  getSupportedDeviceProperties();

    virtual MtpResponseCode         getObjectPropertyValue(MtpObjectHandle handle,
                                            MtpObjectProperty property,
                                            MtpDataPacket& packet);

    virtual MtpResponseCode         setObjectPropertyValue(MtpObjectHandle handle,
                                            MtpObjectProperty property,
                                            MtpDataPacket& packet);

    virtual MtpResponseCode         getDevicePropertyValue(MtpDeviceProperty property,
                                            MtpDataPacket& packet);

    virtual MtpResponseCode         setDevicePropertyValue(MtpDeviceProperty property,
                                            MtpDataPacket& packet);

    virtual MtpResponseCode         resetDeviceProperty(MtpDeviceProperty property);

    virtual MtpResponseCode         getObjectPropertyList(MtpObjectHandle handle,
                                            uint32_t format, uint32_t property,
                                            Int32 groupCode, Int32 depth,
                                            MtpDataPacket& packet);

    virtual MtpResponseCode         getObjectInfo(MtpObjectHandle handle,
                                            MtpObjectInfo& info);

    virtual void*                   getThumbnail(MtpObjectHandle handle, size_t& outThumbSize);

    virtual MtpResponseCode         getObjectFilePath(MtpObjectHandle handle,
                                            MtpString& outFilePath,
                                            int64_t& outFileLength,
                                            MtpObjectFormat& outFormat);
    virtual MtpResponseCode         deleteFile(MtpObjectHandle handle);

    Boolean                         getObjectPropertyInfo(MtpObjectProperty property, Int32& type);
    Boolean                         getDevicePropertyInfo(MtpDeviceProperty property, Int32& type);

    virtual MtpObjectHandleList*    getObjectReferences(MtpObjectHandle handle);

    virtual MtpResponseCode         setObjectReferences(MtpObjectHandle handle,
                                            MtpObjectHandleList* references);

    virtual MtpProperty*            getObjectPropertyDesc(MtpObjectProperty property,
                                            MtpObjectFormat format);

    virtual MtpProperty*            getDevicePropertyDesc(MtpDeviceProperty property);

    virtual void                    sessionStarted();

    virtual void                    sessionEnded();
};

// ----------------------------------------------------------------------------

// static void checkAndClearExceptionFromCallback(JNIEnv* env, const char* methodName) {
//     if (env->ExceptionCheck()) {
//         ALOGE("An exception was thrown by callback '%s'.", methodName);
//         LOGE_EX(env);
//         env->ExceptionClear();
//     }
// }

// ----------------------------------------------------------------------------

MyMtpDatabase::MyMtpDatabase(CMtpDatabase* client)
    : mDatabase(client)
{
    // create buffers for out arguments
    // we don't need to be thread-safe so this is OK
    mIntBuffer = ArrayOf<Int32>::Alloc(3);

    mLongBuffer = ArrayOf<Int64>::Alloc(2);

    mStringBuffer = ArrayOf<Char32>::Alloc(256);
}

void MyMtpDatabase::cleanup() {
}

MyMtpDatabase::~MyMtpDatabase() {
}

MtpObjectHandle MyMtpDatabase::beginSendObject(
    const char* path,
    MtpObjectFormat format,
    MtpObjectHandle parent,
    MtpStorageID storage,
    uint64_t size,
    time_t modified)
{
    MtpObjectHandle result = mDatabase->BeginSendObject(String(path), (Int32)format, (Int32)parent, (Int32)storage,
            (Int64)size, (Int64)modified);
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

void MyMtpDatabase::endSendObject(
    const char* path,
    MtpObjectHandle handle,
    MtpObjectFormat format,
    bool succeeded)
{
    mDatabase->EndSendObject(String(path), (Int32)handle, (Int32)format, (Boolean)succeeded);
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
}

MtpObjectHandleList* MyMtpDatabase::getObjectList(
    MtpStorageID storageID,
    MtpObjectFormat format,
    MtpObjectHandle parent)
{
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetObjectList((Int32)storageID, (Int32)format, (Int32)parent);
    if (!array)
        return NULL;
    MtpObjectHandleList* list = new MtpObjectHandleList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

Int32 MyMtpDatabase::getNumObjects(
    MtpStorageID storageID,
    MtpObjectFormat format,
    MtpObjectHandle parent)
{
    Int32 result = mDatabase->GetNumObjects((Int32)storageID, (Int32)format, (Int32)parent);
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpObjectFormatList* MyMtpDatabase::getSupportedPlaybackFormats() {
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetSupportedPlaybackFormats();
    if (!array)
        return NULL;
    MtpObjectFormatList* list = new MtpObjectFormatList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

MtpObjectFormatList* MyMtpDatabase::getSupportedCaptureFormats() {
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetSupportedCaptureFormats();
    if (!array)
        return NULL;
    MtpObjectFormatList* list = new MtpObjectFormatList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

MtpObjectPropertyList* MyMtpDatabase::getSupportedObjectProperties(MtpObjectFormat format)
{
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetSupportedObjectProperties((Int32)format);
    if (!array)
        return NULL;
    MtpObjectPropertyList* list = new MtpObjectPropertyList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

MtpDevicePropertyList* MyMtpDatabase::getSupportedDeviceProperties() {
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetSupportedDeviceProperties();
    if (!array)
        return NULL;
    MtpDevicePropertyList* list = new MtpDevicePropertyList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

MtpResponseCode MyMtpDatabase::getObjectPropertyValue(
    MtpObjectHandle handle,
    MtpObjectProperty property,
    MtpDataPacket& packet)
{
    AutoPtr<IMtpPropertyList> list = mDatabase->GetObjectPropertyList((Int64)handle, 0, (Int64)property, 0, 0);
    MtpResponseCode result = ((CMtpPropertyList*)list.Get())->mResult;
    Int32 count = ((CMtpPropertyList*)list.Get())->mCount;
    if (result == MTP_RESPONSE_OK && count != 1)
        result = MTP_RESPONSE_GENERAL_ERROR;

    if (result == MTP_RESPONSE_OK) {
        AutoPtr<ArrayOf<Int32> > objectHandlesArray = ((CMtpPropertyList*)list.Get())->mObjectHandles;
        AutoPtr<ArrayOf<Int32> > propertyCodesArray = ((CMtpPropertyList*)list.Get())->mPropertyCodes;
        AutoPtr<ArrayOf<Int32> > dataTypesArray = ((CMtpPropertyList*)list.Get())->mDataTypes;
        AutoPtr<ArrayOf<Int64> > longValuesArray = ((CMtpPropertyList*)list.Get())->mLongValues;
        AutoPtr<ArrayOf<String> > stringValuesArray = ((CMtpPropertyList*)list.Get())->mStringValues;

        Int32 type = (*dataTypesArray)[0];
        Int64 longValue = (longValuesArray ? (*longValuesArray)[0] : 0);

        // special case date properties, which are strings to MTP
        // but stored internally as a uint64
        if (property == MTP_PROPERTY_DATE_MODIFIED || property == MTP_PROPERTY_DATE_ADDED) {
            char    date[20];
            formatDateTime(longValue, date, sizeof(date));
            packet.putString(date);
            goto out;
        }
        // release date is stored internally as just the year
        if (property == MTP_PROPERTY_ORIGINAL_RELEASE_DATE) {
            char    date[20];
            snprintf(date, sizeof(date), "%04lld0101T000000", longValue);
            packet.putString(date);
            goto out;
        }

        switch (type) {
            case MTP_TYPE_INT8:
                packet.putInt8(longValue);
                break;
            case MTP_TYPE_UINT8:
                packet.putUInt8(longValue);
                break;
            case MTP_TYPE_INT16:
                packet.putInt16(longValue);
                break;
            case MTP_TYPE_UINT16:
                packet.putUInt16(longValue);
                break;
            case MTP_TYPE_INT32:
                packet.putInt32(longValue);
                break;
            case MTP_TYPE_UINT32:
                packet.putUInt32(longValue);
                break;
            case MTP_TYPE_INT64:
                packet.putInt64(longValue);
                break;
            case MTP_TYPE_UINT64:
                packet.putUInt64(longValue);
                break;
            case MTP_TYPE_INT128:
                packet.putInt128(longValue);
                break;
            case MTP_TYPE_UINT128:
                packet.putInt128(longValue);
                break;
            case MTP_TYPE_STR:
            {
                String stringValue = (*stringValuesArray)[0];
                if (stringValue) {
                    const char* str = stringValue.string();
                    if (str == NULL) {
                        return MTP_RESPONSE_GENERAL_ERROR;
                    }
                    packet.putString(str);
                } else {
                    packet.putEmptyString();
                }
                break;
             }
            default:
                // ALOGE("unsupported type in getObjectPropertyValue\n");
                result = MTP_RESPONSE_INVALID_OBJECT_PROP_FORMAT;
        }
out:
        ;
    }

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpResponseCode MyMtpDatabase::setObjectPropertyValue(
    MtpObjectHandle handle,
    MtpObjectProperty property,
    MtpDataPacket& packet)
{
    Int32 type;

    if (!getObjectPropertyInfo(property, type))
        return MTP_RESPONSE_OBJECT_PROP_NOT_SUPPORTED;

    Int64 longValue = 0;
    String stringValue;

    switch (type) {
        case MTP_TYPE_INT8:
            longValue = packet.getInt8();
            break;
        case MTP_TYPE_UINT8:
            longValue = packet.getUInt8();
            break;
        case MTP_TYPE_INT16:
            longValue = packet.getInt16();
            break;
        case MTP_TYPE_UINT16:
            longValue = packet.getUInt16();
            break;
        case MTP_TYPE_INT32:
            longValue = packet.getInt32();
            break;
        case MTP_TYPE_UINT32:
            longValue = packet.getUInt32();
            break;
        case MTP_TYPE_INT64:
            longValue = packet.getInt64();
            break;
        case MTP_TYPE_UINT64:
            longValue = packet.getUInt64();
            break;
        case MTP_TYPE_STR:
        {
            MtpStringBuffer buffer;
            packet.getString(buffer);
            stringValue = String((const char *)buffer);
            break;
         }
        default:
            // ALOGE("unsupported type in getObjectPropertyValue\n");
            return MTP_RESPONSE_INVALID_OBJECT_PROP_FORMAT;
    }

    Int32 result = mDatabase->SetObjectProperty((Int32)handle, (Int32)property, (Int64)longValue, stringValue);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpResponseCode MyMtpDatabase::getDevicePropertyValue(
    MtpDeviceProperty property,
    MtpDataPacket& packet)
{
    int type;

    if (!getDevicePropertyInfo(property, type))
        return MTP_RESPONSE_DEVICE_PROP_NOT_SUPPORTED;

    Int32 result = mDatabase->GetDeviceProperty((Int32)property, mLongBuffer, mStringBuffer);
    if (result != MTP_RESPONSE_OK) {
        // checkAndClearExceptionFromCallback(env, __FUNCTION__);
        return result;
    }

    Int64 longValue = (*mLongBuffer)[0];

    switch (type) {
        case MTP_TYPE_INT8:
            packet.putInt8(longValue);
            break;
        case MTP_TYPE_UINT8:
            packet.putUInt8(longValue);
            break;
        case MTP_TYPE_INT16:
            packet.putInt16(longValue);
            break;
        case MTP_TYPE_UINT16:
            packet.putUInt16(longValue);
            break;
        case MTP_TYPE_INT32:
            packet.putInt32(longValue);
            break;
        case MTP_TYPE_UINT32:
            packet.putUInt32(longValue);
            break;
        case MTP_TYPE_INT64:
            packet.putInt64(longValue);
            break;
        case MTP_TYPE_UINT64:
            packet.putUInt64(longValue);
            break;
        case MTP_TYPE_INT128:
            packet.putInt128(longValue);
            break;
        case MTP_TYPE_UINT128:
            packet.putInt128(longValue);
            break;
        case MTP_TYPE_STR:
        {
            String str = String(*mStringBuffer.Get());
            packet.putString(str.string());
            break;
         }
        default:
            // ALOGE("unsupported type in getDevicePropertyValue\n");
            return MTP_RESPONSE_INVALID_DEVICE_PROP_FORMAT;
    }

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return MTP_RESPONSE_OK;
}

MtpResponseCode MyMtpDatabase::setDevicePropertyValue(
    MtpDeviceProperty property,
    MtpDataPacket& packet)
{
    Int32 type;

    if (!getDevicePropertyInfo(property, type))
        return MTP_RESPONSE_DEVICE_PROP_NOT_SUPPORTED;

    Int64 longValue = 0;
    String stringValue;

    switch (type) {
        case MTP_TYPE_INT8:
            longValue = packet.getInt8();
            break;
        case MTP_TYPE_UINT8:
            longValue = packet.getUInt8();
            break;
        case MTP_TYPE_INT16:
            longValue = packet.getInt16();
            break;
        case MTP_TYPE_UINT16:
            longValue = packet.getUInt16();
            break;
        case MTP_TYPE_INT32:
            longValue = packet.getInt32();
            break;
        case MTP_TYPE_UINT32:
            longValue = packet.getUInt32();
            break;
        case MTP_TYPE_INT64:
            longValue = packet.getInt64();
            break;
        case MTP_TYPE_UINT64:
            longValue = packet.getUInt64();
            break;
        case MTP_TYPE_STR:
        {
            MtpStringBuffer buffer;
            packet.getString(buffer);
            stringValue = String((const char *)buffer);
            break;
         }
        default:
            // ALOGE("unsupported type in setDevicePropertyValue\n");
            return MTP_RESPONSE_INVALID_OBJECT_PROP_FORMAT;
    }

    Int32 result = mDatabase->SetDeviceProperty((Int32)property, longValue, stringValue);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpResponseCode MyMtpDatabase::resetDeviceProperty(
    MtpDeviceProperty property)
{
    return -1;
}

MtpResponseCode MyMtpDatabase::getObjectPropertyList(
    MtpObjectHandle handle,
    uint32_t format,
    uint32_t property,
    Int32 groupCode,
    Int32 depth,
    MtpDataPacket& packet)
{
    AutoPtr<IMtpPropertyList> list = mDatabase->GetObjectPropertyList((Int64)handle, (Int32)format, (Int64)property, (Int32)groupCode, (Int32)depth);
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    if (!list)
        return MTP_RESPONSE_GENERAL_ERROR;
    Int32 count = ((CMtpPropertyList*)list.Get())->mCount;
    MtpResponseCode result = ((CMtpPropertyList*)list.Get())->mResult;

    packet.putUInt32(count);
    if (count > 0) {
        AutoPtr<ArrayOf<Int32> > objectHandlesArray = ((CMtpPropertyList*)list.Get())->mObjectHandles;
        AutoPtr<ArrayOf<Int32> > propertyCodesArray = ((CMtpPropertyList*)list.Get())->mPropertyCodes;
        AutoPtr<ArrayOf<Int32> > dataTypesArray = ((CMtpPropertyList*)list.Get())->mDataTypes;
        AutoPtr<ArrayOf<Int64> > longValuesArray = ((CMtpPropertyList*)list.Get())->mLongValues;
        AutoPtr<ArrayOf<String> > stringValuesArray = ((CMtpPropertyList*)list.Get())->mStringValues;

        for (Int32 i = 0; i < count; i++) {
            packet.putUInt32((*objectHandlesArray)[i]);
            packet.putUInt16((*propertyCodesArray)[i]);
            Int32 type = (*dataTypesArray)[i];
            packet.putUInt16(type);

            switch (type) {
                case MTP_TYPE_INT8:
                    packet.putInt8((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_UINT8:
                    packet.putUInt8((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_INT16:
                    packet.putInt16((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_UINT16:
                    packet.putUInt16((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_INT32:
                    packet.putInt32((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_UINT32:
                    packet.putUInt32((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_INT64:
                    packet.putInt64((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_UINT64:
                    packet.putUInt64((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_INT128:
                    packet.putInt128((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_UINT128:
                    packet.putUInt128((*longValuesArray)[i]);
                    break;
                case MTP_TYPE_STR: {
                    String value = (*stringValuesArray)[i];
                    const char *valueStr = (value ? value.string() : NULL);
                    if (valueStr) {
                        packet.putString(valueStr);
                    } else {
                        packet.putEmptyString();
                    }
                    break;
                }
                default:
                    // ALOGE("bad or unsupported data type in MyMtpDatabase::getObjectPropertyList");
                    break;
            }
        }

    }

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpResponseCode MyMtpDatabase::getObjectInfo(
    MtpObjectHandle handle,
    MtpObjectInfo& info)
{
    char            date[20];
    MtpString       path;
    int64_t         length;
    MtpObjectFormat format;

    MtpResponseCode result = getObjectFilePath(handle, path, length, format);
    if (result != MTP_RESPONSE_OK) {
        return result;
    }
    info.mCompressedSize = (length > 0xFFFFFFFFLL ? 0xFFFFFFFF : (uint32_t)length);

    Boolean b = mDatabase->GetObjectInfo((Int32)handle, mIntBuffer, mStringBuffer, mLongBuffer);
    if(b) {
        return MTP_RESPONSE_INVALID_OBJECT_HANDLE;
    }

    info.mStorageID = (*mIntBuffer)[0];
    info.mFormat = (*mIntBuffer)[1];
    info.mParent = (*mIntBuffer)[2];

    info.mDateModified = (*mLongBuffer)[0];

   // info.mAssociationType = (format == MTP_FORMAT_ASSOCIATION ?
   //                         MTP_ASSOCIATION_TYPE_GENERIC_FOLDER :
   //                         MTP_ASSOCIATION_TYPE_UNDEFINED);
    info.mAssociationType = MTP_ASSOCIATION_TYPE_UNDEFINED;

    String str = String(*mStringBuffer.Get());
    MtpString temp(str.string());
    info.mName = strdup((const char *)temp);

    // read EXIF data for thumbnail information
    if (info.mFormat == MTP_FORMAT_EXIF_JPEG || info.mFormat == MTP_FORMAT_JFIF) {
        ResetJpgfile();
         // Start with an empty image information structure.
        memset(&ImageInfo, 0, sizeof(ImageInfo));
        ImageInfo.FlashUsed = -1;
        ImageInfo.MeteringMode = -1;
        ImageInfo.Whitebalance = -1;
        strncpy(ImageInfo.FileName, (const char *)path, PATH_MAX);
        if (ReadJpegFile((const char*)path, READ_METADATA)) {
            Section_t* section = FindSection(M_EXIF);
            if (section) {
                info.mThumbCompressedSize = ImageInfo.ThumbnailSize;
                info.mThumbFormat = MTP_FORMAT_EXIF_JPEG;
                info.mImagePixWidth = ImageInfo.Width;
                info.mImagePixHeight = ImageInfo.Height;
            }
        }
        DiscardData();
    }

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return MTP_RESPONSE_OK;
}

void* MyMtpDatabase::getThumbnail(
    MtpObjectHandle handle,
    size_t& outThumbSize)
{
    MtpString path;
    int64_t length;
    MtpObjectFormat format;
    void* result = NULL;
    outThumbSize = 0;

    if (getObjectFilePath(handle, path, length, format) == MTP_RESPONSE_OK
            && (format == MTP_FORMAT_EXIF_JPEG || format == MTP_FORMAT_JFIF)) {
        ResetJpgfile();
         // Start with an empty image information structure.
        memset(&ImageInfo, 0, sizeof(ImageInfo));
        ImageInfo.FlashUsed = -1;
        ImageInfo.MeteringMode = -1;
        ImageInfo.Whitebalance = -1;
        strncpy(ImageInfo.FileName, (const char *)path, PATH_MAX);
        if (ReadJpegFile((const char*)path, READ_METADATA)) {
            Section_t* section = FindSection(M_EXIF);
            if (section) {
                outThumbSize = ImageInfo.ThumbnailSize;
                result = malloc(outThumbSize);
                if (result)
                    memcpy(result, section->Data + ImageInfo.ThumbnailOffset + 8, outThumbSize);
            }
            DiscardData();
        }
    }

    return result;
}

MtpResponseCode MyMtpDatabase::getObjectFilePath(
    MtpObjectHandle handle,
    MtpString& outFilePath,
    int64_t& outFileLength,
    MtpObjectFormat& outFormat)
{
    Int32 result = mDatabase->GetObjectFilePath((Int32)handle, mStringBuffer, mLongBuffer);
    if (result != MTP_RESPONSE_OK) {
        // checkAndClearExceptionFromCallback(env, __FUNCTION__);
        return result;
    }

    String str = String(*mStringBuffer.Get());
    AutoPtr<ArrayOf<Char16> > array16s = str.GetChar16s();
    Char16* str16s = array16s.Get()->GetPayload();
    outFilePath.setTo(str16s, strlen16(str16s));

    outFileLength = (*mLongBuffer)[0];
    outFormat = (*mLongBuffer)[1];

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpResponseCode MyMtpDatabase::deleteFile(MtpObjectHandle handle) {
    MtpResponseCode result = mDatabase->DeleteFile((Int32)handle);
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

struct PropertyTableEntry {
    MtpObjectProperty   property;
    Int32               type;
};

static const PropertyTableEntry   kObjectPropertyTable[] = {
    {   MTP_PROPERTY_STORAGE_ID,        MTP_TYPE_UINT32     },
    {   MTP_PROPERTY_OBJECT_FORMAT,     MTP_TYPE_UINT16     },
    {   MTP_PROPERTY_PROTECTION_STATUS, MTP_TYPE_UINT16     },
    {   MTP_PROPERTY_OBJECT_SIZE,       MTP_TYPE_UINT64     },
    {   MTP_PROPERTY_OBJECT_FILE_NAME,  MTP_TYPE_STR        },
    {   MTP_PROPERTY_DATE_MODIFIED,     MTP_TYPE_STR        },
    {   MTP_PROPERTY_PARENT_OBJECT,     MTP_TYPE_UINT32     },
    {   MTP_PROPERTY_PERSISTENT_UID,    MTP_TYPE_UINT128    },
    {   MTP_PROPERTY_NAME,              MTP_TYPE_STR        },
    {   MTP_PROPERTY_DISPLAY_NAME,      MTP_TYPE_STR        },
    {   MTP_PROPERTY_DATE_ADDED,        MTP_TYPE_STR        },
    {   MTP_PROPERTY_ARTIST,            MTP_TYPE_STR        },
    {   MTP_PROPERTY_ALBUM_NAME,        MTP_TYPE_STR        },
    {   MTP_PROPERTY_ALBUM_ARTIST,      MTP_TYPE_STR        },
    {   MTP_PROPERTY_TRACK,             MTP_TYPE_UINT16     },
    {   MTP_PROPERTY_ORIGINAL_RELEASE_DATE, MTP_TYPE_STR    },
    {   MTP_PROPERTY_GENRE,             MTP_TYPE_STR        },
    {   MTP_PROPERTY_COMPOSER,          MTP_TYPE_STR        },
    {   MTP_PROPERTY_DURATION,          MTP_TYPE_UINT32     },
    {   MTP_PROPERTY_DESCRIPTION,       MTP_TYPE_STR        },
};

static const PropertyTableEntry   kDevicePropertyTable[] = {
    {   MTP_DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER,    MTP_TYPE_STR },
    {   MTP_DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME,       MTP_TYPE_STR },
    {   MTP_DEVICE_PROPERTY_IMAGE_SIZE,                 MTP_TYPE_STR },
};

Boolean MyMtpDatabase::getObjectPropertyInfo(
    MtpObjectProperty property,
    Int32& type)
{
    Int32 count = sizeof(kObjectPropertyTable) / sizeof(kObjectPropertyTable[0]);
    const PropertyTableEntry* entry = kObjectPropertyTable;
    for (Int32 i = 0; i < count; i++, entry++) {
        if (entry->property == property) {
            type = entry->type;
            return TRUE;
        }
    }
    return FALSE;
}

Boolean MyMtpDatabase::getDevicePropertyInfo(
    MtpDeviceProperty property,
    Int32& type)
{
    Int32 count = sizeof(kDevicePropertyTable) / sizeof(kDevicePropertyTable[0]);
    const PropertyTableEntry* entry = kDevicePropertyTable;
    for (Int32 i = 0; i < count; i++, entry++) {
        if (entry->property == property) {
            type = entry->type;
            return TRUE;
        }
    }
    return FALSE;
}

MtpObjectHandleList* MyMtpDatabase::getObjectReferences(
    MtpObjectHandle handle)
{
    AutoPtr<ArrayOf<Int32> > array = mDatabase->GetObjectReferences((Int32)handle);
    if (!array)
        return NULL;
    MtpObjectHandleList* list = new MtpObjectHandleList();
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; i++)
        list->push((*array)[i]);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return list;
}

MtpResponseCode MyMtpDatabase::setObjectReferences(
    MtpObjectHandle handle,
    MtpObjectHandleList* references)
{
    Int32 count = references->size();
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(count);
    if (!array) {
        // ALOGE("out of memory in setObjectReferences");
        return FALSE;
    }
     for (Int32 i = 0; i < count; i++)
        array->Set(i, (*references)[i]);
    MtpResponseCode result = mDatabase->SetObjectReferences((Int32)handle, array);

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

MtpProperty* MyMtpDatabase::getObjectPropertyDesc(
    MtpObjectProperty property,
    MtpObjectFormat format)
{
    MtpProperty* result = NULL;
    switch (property) {
        case MTP_PROPERTY_OBJECT_FORMAT:
            // use format as default value
            result = new MtpProperty(property, MTP_TYPE_UINT16, FALSE, format);
            break;
        case MTP_PROPERTY_PROTECTION_STATUS:
        case MTP_PROPERTY_TRACK:
            result = new MtpProperty(property, MTP_TYPE_UINT16);
            break;
        case MTP_PROPERTY_STORAGE_ID:
        case MTP_PROPERTY_PARENT_OBJECT:
        case MTP_PROPERTY_DURATION:
            result = new MtpProperty(property, MTP_TYPE_UINT32);
            break;
        case MTP_PROPERTY_OBJECT_SIZE:
            result = new MtpProperty(property, MTP_TYPE_UINT64);
            break;
        case MTP_PROPERTY_PERSISTENT_UID:
            result = new MtpProperty(property, MTP_TYPE_UINT128);
            break;
        case MTP_PROPERTY_NAME:
        case MTP_PROPERTY_DISPLAY_NAME:
        case MTP_PROPERTY_ARTIST:
        case MTP_PROPERTY_ALBUM_NAME:
        case MTP_PROPERTY_ALBUM_ARTIST:
        case MTP_PROPERTY_GENRE:
        case MTP_PROPERTY_COMPOSER:
        case MTP_PROPERTY_DESCRIPTION:
            result = new MtpProperty(property, MTP_TYPE_STR);
            break;
        case MTP_PROPERTY_DATE_MODIFIED:
        case MTP_PROPERTY_DATE_ADDED:
        case MTP_PROPERTY_ORIGINAL_RELEASE_DATE:
            result = new MtpProperty(property, MTP_TYPE_STR);
            result->setFormDateTime();
            break;
        case MTP_PROPERTY_OBJECT_FILE_NAME:
            // We allow renaming files and folders
            result = new MtpProperty(property, MTP_TYPE_STR, TRUE);
            break;
    }

    return result;
}

MtpProperty* MyMtpDatabase::getDevicePropertyDesc(
    MtpDeviceProperty property)
{
    MtpProperty* result = NULL;
    Boolean writable = FALSE;

    switch (property) {
        case MTP_DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER:
        case MTP_DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME:
            writable = TRUE;
            // fall through
        case MTP_DEVICE_PROPERTY_IMAGE_SIZE:
            result = new MtpProperty(property, MTP_TYPE_STR, writable);

            // get current value
            Int32 ret = mDatabase->GetDeviceProperty((Int32)property, mLongBuffer, mStringBuffer);
            if (ret == MTP_RESPONSE_OK) {
                String str = String(*mStringBuffer.Get());
                AutoPtr<ArrayOf<Char16> > array16s = str.GetChar16s();
                Char16* str16s = array16s.Get()->GetPayload();
                result->setCurrentValue(str16s);
                // for read-only properties it is safe to assume current value is default value
                if (!writable)
                    result->setDefaultValue(str16s);
            } else {
                // ALOGE("unable to read device property, response: %04X", ret);
            }
            break;
    }

    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
    return result;
}

void MyMtpDatabase::sessionStarted() {
    mDatabase->SessionStarted();
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
}

void MyMtpDatabase::sessionEnded() {
    mDatabase->SessionEnded();
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
}



const String CMtpDatabase::TAG("MtpDatabase");
const Int32 CMtpDatabase::DEVICE_PROPERTIES_DATABASE_VERSION = 1;

ArrayOf<String>* CMtpDatabase::ID_PROJECTION;
ArrayOf<String>* CMtpDatabase::PATH_PROJECTION;
ArrayOf<String>* CMtpDatabase::PATH_FORMAT_PROJECTION;
ArrayOf<String>* CMtpDatabase::OBJECT_INFO_PROJECTION;
ArrayOf<Int32>* CMtpDatabase::FILE_PROPERTIES;
ArrayOf<Int32>* CMtpDatabase::AUDIO_PROPERTIES;
ArrayOf<Int32>* CMtpDatabase::VIDEO_PROPERTIES;
ArrayOf<Int32>* CMtpDatabase::IMAGE_PROPERTIES;
ArrayOf<Int32>* CMtpDatabase::ALL_PROPERTIES;

const String CMtpDatabase::ID_WHERE = IMediaStoreFilesFileColumns::ID + "=?";
const String CMtpDatabase::PATH_WHERE = IMediaStoreFilesFileColumns::DATA + "=?";
const String CMtpDatabase::STORAGE_WHERE = IMediaStoreFilesFileColumns::STORAGE_ID + "=?";
const String CMtpDatabase::FORMAT_WHERE = IMediaStoreFilesFileColumns::FORMAT + "=?";
const String CMtpDatabase::PARENT_WHERE = IMediaStoreFilesFileColumns::PARENT + "=?";
const String CMtpDatabase::STORAGE_FORMAT_WHERE = CMtpDatabase::STORAGE_WHERE + " AND " + IMediaStoreFilesFileColumns::FORMAT + "=?";
const String CMtpDatabase::STORAGE_PARENT_WHERE = CMtpDatabase::STORAGE_WHERE + " AND " + IMediaStoreFilesFileColumns::PARENT + "=?";
const String CMtpDatabase::FORMAT_PARENT_WHERE = CMtpDatabase::FORMAT_WHERE + " AND " + IMediaStoreFilesFileColumns::PARENT + "=?";
const String CMtpDatabase::STORAGE_FORMAT_PARENT_WHERE = CMtpDatabase::STORAGE_FORMAT_WHERE + " AND " + IMediaStoreFilesFileColumns::PARENT + "=?";

CMtpDatabase::CMtpDatabase()
    : mMediaStoragePath(NULL)
    , mSubDirectoriesWhere(NULL)
    , mDatabaseModified(FALSE)
    , mNativeContext(0)
{
    ID_PROJECTION = ArrayOf<String>::Alloc(1);
    ID_PROJECTION->Set(0, IMediaStoreFilesFileColumns::ID);

    PATH_PROJECTION = ArrayOf<String>::Alloc(2);
    PATH_PROJECTION->Set(0, IMediaStoreFilesFileColumns::ID);
    PATH_PROJECTION->Set(1, IMediaStoreFilesFileColumns::DATA);

    PATH_FORMAT_PROJECTION = ArrayOf<String>::Alloc(3);
    PATH_FORMAT_PROJECTION->Set(0, IMediaStoreFilesFileColumns::ID);
    PATH_FORMAT_PROJECTION->Set(1, IMediaStoreFilesFileColumns::DATA);
    PATH_FORMAT_PROJECTION->Set(2, IMediaStoreFilesFileColumns::FORMAT);

    OBJECT_INFO_PROJECTION = ArrayOf<String>::Alloc(6);
    OBJECT_INFO_PROJECTION->Set(0, IMediaStoreFilesFileColumns::ID);
    OBJECT_INFO_PROJECTION->Set(1, IMediaStoreFilesFileColumns::STORAGE_ID);
    OBJECT_INFO_PROJECTION->Set(2, IMediaStoreFilesFileColumns::FORMAT);
    OBJECT_INFO_PROJECTION->Set(3, IMediaStoreFilesFileColumns::PARENT);
    OBJECT_INFO_PROJECTION->Set(4, IMediaStoreFilesFileColumns::DATA);
    OBJECT_INFO_PROJECTION->Set(5, IMediaStoreFilesFileColumns::DATE_MODIFIED);

    FILE_PROPERTIES = ArrayOf<Int32>::Alloc(10);
            // NOTE must match beginning of AUDIO_PROPERTIES, VIDEO_PROPERTIES
            // and IMAGE_PROPERTIES below
    FILE_PROPERTIES->Set(0, IMtpConstants::PROPERTY_STORAGE_ID);
    FILE_PROPERTIES->Set(1, IMtpConstants::PROPERTY_OBJECT_FORMAT);
    FILE_PROPERTIES->Set(2, IMtpConstants::PROPERTY_PROTECTION_STATUS);
    FILE_PROPERTIES->Set(3, IMtpConstants::PROPERTY_OBJECT_SIZE);
    FILE_PROPERTIES->Set(4, IMtpConstants::PROPERTY_OBJECT_FILE_NAME);
    FILE_PROPERTIES->Set(5, IMtpConstants::PROPERTY_DATE_MODIFIED);
    FILE_PROPERTIES->Set(6, IMtpConstants::PROPERTY_PARENT_OBJECT);
    FILE_PROPERTIES->Set(7, IMtpConstants::PROPERTY_PERSISTENT_UID);
    FILE_PROPERTIES->Set(8, IMtpConstants::PROPERTY_NAME);
    FILE_PROPERTIES->Set(9, IMtpConstants::PROPERTY_DATE_ADDED);

    AUDIO_PROPERTIES = ArrayOf<Int32>::Alloc(19);
            // NOTE must match FILE_PROPERTIES above
    AUDIO_PROPERTIES->Set(0, IMtpConstants::PROPERTY_STORAGE_ID);
    AUDIO_PROPERTIES->Set(1, IMtpConstants::PROPERTY_OBJECT_FORMAT);
    AUDIO_PROPERTIES->Set(2, IMtpConstants::PROPERTY_PROTECTION_STATUS);
    AUDIO_PROPERTIES->Set(3, IMtpConstants::PROPERTY_OBJECT_SIZE);
    AUDIO_PROPERTIES->Set(4, IMtpConstants::PROPERTY_OBJECT_FILE_NAME);
    AUDIO_PROPERTIES->Set(5, IMtpConstants::PROPERTY_DATE_MODIFIED);
    AUDIO_PROPERTIES->Set(6, IMtpConstants::PROPERTY_PARENT_OBJECT);
    AUDIO_PROPERTIES->Set(7, IMtpConstants::PROPERTY_PERSISTENT_UID);
    AUDIO_PROPERTIES->Set(8, IMtpConstants::PROPERTY_NAME);
    AUDIO_PROPERTIES->Set(9, IMtpConstants::PROPERTY_DISPLAY_NAME);
    AUDIO_PROPERTIES->Set(10, IMtpConstants::PROPERTY_DATE_ADDED);
            // audio specific properties
    AUDIO_PROPERTIES->Set(11, IMtpConstants::PROPERTY_ARTIST);
    AUDIO_PROPERTIES->Set(12, IMtpConstants::PROPERTY_ALBUM_NAME);
    AUDIO_PROPERTIES->Set(13, IMtpConstants::PROPERTY_ALBUM_ARTIST);
    AUDIO_PROPERTIES->Set(14, IMtpConstants::PROPERTY_TRACK);
    AUDIO_PROPERTIES->Set(15, IMtpConstants::PROPERTY_ORIGINAL_RELEASE_DATE);
    AUDIO_PROPERTIES->Set(16, IMtpConstants::PROPERTY_DURATION);
    AUDIO_PROPERTIES->Set(17, IMtpConstants::PROPERTY_GENRE);
    AUDIO_PROPERTIES->Set(18, IMtpConstants::PROPERTY_COMPOSER);

    VIDEO_PROPERTIES = ArrayOf<Int32>::Alloc(15);
            // NOTE must match FILE_PROPERTIES above
    VIDEO_PROPERTIES->Set(0, IMtpConstants::PROPERTY_STORAGE_ID);
    VIDEO_PROPERTIES->Set(1, IMtpConstants::PROPERTY_OBJECT_FORMAT);
    VIDEO_PROPERTIES->Set(2, IMtpConstants::PROPERTY_PROTECTION_STATUS);
    VIDEO_PROPERTIES->Set(3, IMtpConstants::PROPERTY_OBJECT_SIZE);
    VIDEO_PROPERTIES->Set(4, IMtpConstants::PROPERTY_OBJECT_FILE_NAME);
    VIDEO_PROPERTIES->Set(5, IMtpConstants::PROPERTY_DATE_MODIFIED);
    VIDEO_PROPERTIES->Set(6, IMtpConstants::PROPERTY_PARENT_OBJECT);
    VIDEO_PROPERTIES->Set(7, IMtpConstants::PROPERTY_PERSISTENT_UID);
    VIDEO_PROPERTIES->Set(8, IMtpConstants::PROPERTY_NAME);
    VIDEO_PROPERTIES->Set(9, IMtpConstants::PROPERTY_DISPLAY_NAME);
    VIDEO_PROPERTIES->Set(10, IMtpConstants::PROPERTY_DATE_ADDED);
            // video specific properties
    VIDEO_PROPERTIES->Set(11, IMtpConstants::PROPERTY_ARTIST);
    VIDEO_PROPERTIES->Set(12, IMtpConstants::PROPERTY_ALBUM_NAME);
    VIDEO_PROPERTIES->Set(13, IMtpConstants::PROPERTY_DURATION);
    VIDEO_PROPERTIES->Set(14, IMtpConstants::PROPERTY_DESCRIPTION);

    IMAGE_PROPERTIES = ArrayOf<Int32>::Alloc(12);
            // NOTE must match FILE_PROPERTIES above
    IMAGE_PROPERTIES->Set(0, IMtpConstants::PROPERTY_STORAGE_ID);
    IMAGE_PROPERTIES->Set(1, IMtpConstants::PROPERTY_OBJECT_FORMAT);
    IMAGE_PROPERTIES->Set(2, IMtpConstants::PROPERTY_PROTECTION_STATUS);
    IMAGE_PROPERTIES->Set(3, IMtpConstants::PROPERTY_OBJECT_SIZE);
    IMAGE_PROPERTIES->Set(4, IMtpConstants::PROPERTY_OBJECT_FILE_NAME);
    IMAGE_PROPERTIES->Set(5, IMtpConstants::PROPERTY_DATE_MODIFIED);
    IMAGE_PROPERTIES->Set(6, IMtpConstants::PROPERTY_PARENT_OBJECT);
    IMAGE_PROPERTIES->Set(7, IMtpConstants::PROPERTY_PERSISTENT_UID);
    IMAGE_PROPERTIES->Set(8, IMtpConstants::PROPERTY_NAME);
    IMAGE_PROPERTIES->Set(9, IMtpConstants::PROPERTY_DISPLAY_NAME);
    IMAGE_PROPERTIES->Set(10, IMtpConstants::PROPERTY_DATE_ADDED);
            // image specific properties
    IMAGE_PROPERTIES->Set(11, IMtpConstants::PROPERTY_DESCRIPTION);

    ALL_PROPERTIES = ArrayOf<Int32>::Alloc(25);
            // NOTE must match FILE_PROPERTIES above
    ALL_PROPERTIES->Set(0, IMtpConstants::PROPERTY_STORAGE_ID);
    ALL_PROPERTIES->Set(1, IMtpConstants::PROPERTY_OBJECT_FORMAT);
    ALL_PROPERTIES->Set(2, IMtpConstants::PROPERTY_PROTECTION_STATUS);
    ALL_PROPERTIES->Set(3, IMtpConstants::PROPERTY_OBJECT_SIZE);
    ALL_PROPERTIES->Set(4, IMtpConstants::PROPERTY_OBJECT_FILE_NAME);
    ALL_PROPERTIES->Set(5, IMtpConstants::PROPERTY_DATE_MODIFIED);
    ALL_PROPERTIES->Set(6, IMtpConstants::PROPERTY_PARENT_OBJECT);
    ALL_PROPERTIES->Set(7, IMtpConstants::PROPERTY_PERSISTENT_UID);
    ALL_PROPERTIES->Set(8, IMtpConstants::PROPERTY_NAME);
    ALL_PROPERTIES->Set(9, IMtpConstants::PROPERTY_DISPLAY_NAME);
    ALL_PROPERTIES->Set(10, IMtpConstants::PROPERTY_DATE_ADDED);
            // image specific properties
    ALL_PROPERTIES->Set(11, IMtpConstants::PROPERTY_DESCRIPTION);
            // audio specific properties
    ALL_PROPERTIES->Set(12, IMtpConstants::PROPERTY_ARTIST);
    ALL_PROPERTIES->Set(13, IMtpConstants::PROPERTY_ALBUM_NAME);
    ALL_PROPERTIES->Set(14, IMtpConstants::PROPERTY_ALBUM_ARTIST);
    ALL_PROPERTIES->Set(15, IMtpConstants::PROPERTY_TRACK);
    ALL_PROPERTIES->Set(16, IMtpConstants::PROPERTY_ORIGINAL_RELEASE_DATE);
    ALL_PROPERTIES->Set(17, IMtpConstants::PROPERTY_DURATION);
    ALL_PROPERTIES->Set(18, IMtpConstants::PROPERTY_GENRE);
    ALL_PROPERTIES->Set(19, IMtpConstants::PROPERTY_COMPOSER);
            // video specific properties
    ALL_PROPERTIES->Set(20, IMtpConstants::PROPERTY_ARTIST);
    ALL_PROPERTIES->Set(21, IMtpConstants::PROPERTY_ALBUM_NAME);
    ALL_PROPERTIES->Set(22, IMtpConstants::PROPERTY_DURATION);
    ALL_PROPERTIES->Set(23, IMtpConstants::PROPERTY_DESCRIPTION);
            // image specific properties
    ALL_PROPERTIES->Set(24, IMtpConstants::PROPERTY_DESCRIPTION);

}

ECode CMtpDatabase::constructor(
    /* [in] */ IContext* context,
    /* [in] */ const String& volumeName,
    /* [in] */ const String& storagePath,
    /* [in] */ ArrayOf<String>* subDirectories)
{
    NativeSetup();

    mContext = context;
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    cr->AcquireProvider(String("media"), (IIContentProvider**)&mMediaProvider);
    mVolumeName = volumeName;
    mMediaStoragePath = storagePath;
    AutoPtr<IMediaStoreFiles> files;
    CMediaStoreFiles::AcquireSingleton((IMediaStoreFiles**)&files);
    files->GetMtpObjectsUri(volumeName, (IUri**)&mObjectsUri);
    CMediaScanner::New(context, (IMediaScanner**)&mMediaScanner);

    mSubDirectories = subDirectories;
    if (subDirectories != NULL) {
        // Compute "where" string for restricting queries to subdirectories
        AutoPtr<IStringBuilder> builder = new StringBuilder();
        builder->AppendString(String("("));
        Int32 count = subDirectories->GetLength();
        for (Int32 i = 0; i < count; i++) {
            builder->AppendString(IMediaStoreFilesFileColumns::DATA + "=? OR "
                + IMediaStoreFilesFileColumns::DATA + " LIKE ?");
            if (i != count - 1) {
                builder->AppendString(String(" OR "));
            }
        }
        builder->AppendString(String(")"));
        builder->ToString(&mSubDirectoriesWhere);

        // Compute "where" arguments for restricting queries to subdirectories
        mSubDirectoriesWhereArgs = ArrayOf<String>::Alloc(count * 2);
        for (Int32 i = 0, j = 0; i < count; i++) {
            String path = (*subDirectories)[i];
            mSubDirectoriesWhereArgs->Set(j++, path);
            mSubDirectoriesWhereArgs->Set(j++, path + "/%");
        }
    }

    // Set locale to MediaScanner.
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    AutoPtr<ILocale> locale;
    config->GetLocale((ILocale**)&locale);
    if (locale != NULL) {
        String language;
        locale->GetLanguage(&language);
        String country;
        locale->GetCountry(&country);
        if (!language.IsNull()) {
            if (!country.IsNull()) {
                mMediaScanner->SetLocale(language + "_" + country);
            } else {
                mMediaScanner->SetLocale(language);
            }
        }
    }
    InitDeviceProperties(context);
    return NOERROR;
}

void CMtpDatabase::Finalize()
{
    NativeFinalize();
}

ECode CMtpDatabase::AddStorage(
    /* [in] */ IMtpStorage* storage)
{
    String path;
    storage->GetPath(&path);
    mStorageMap.Insert(HashMap<String, AutoPtr<IMtpStorage> >::ValueType(path, storage));
    return NOERROR;
}

ECode CMtpDatabase::RemoveStorage(
    /* [in] */ IMtpStorage* storage)
{
    String path;
    storage->GetPath(&path);
    mStorageMap.Erase(path);
    return NOERROR;
}

ECode CMtpDatabase::GetNativeContext(
    /* [out] */ Int32* nativeContext)
{
    VALIDATE_NOT_NULL(nativeContext);
    *nativeContext = mNativeContext;
    return NOERROR;
}

void CMtpDatabase::InitDeviceProperties(
    /* [in] */ IContext* context)
{
    const String devicePropertiesName = String("device-properties");
    context->GetSharedPreferences(devicePropertiesName, IContext::MODE_PRIVATE, (ISharedPreferences**)&mDeviceProperties);
    AutoPtr<IFile> databaseFile;
    context->GetDatabasePath(devicePropertiesName, (IFile**)&databaseFile);

    Boolean b;
    databaseFile->Exists(&b);
    if (b) {
        // for backward compatibility - read device properties from sqlite database
        // and migrate them to shared prefs
        AutoPtr<ISQLiteDatabase> db;
        AutoPtr<ICursor> c;
        //try {
            context->OpenOrCreateDatabase(String("device-properties"), IContext::MODE_PRIVATE, NULL, (ISQLiteDatabase**)&db);
            if (db != NULL) {
                AutoPtr<ArrayOf<String> > s = ArrayOf<String>::Alloc(3);
                s->Set(0, String("_id"));
                s->Set(1, String("code"));
                s->Set(2, String("value"));
                db->Query(String("properties"), s, String(NULL), NULL, String(NULL), String(NULL), String(NULL), (ICursor**)&c);
                if (c != NULL) {
                    AutoPtr<ISharedPreferencesEditor> e;
                    mDeviceProperties->Edit((ISharedPreferencesEditor**)&e);
                    Boolean n;
                    c->MoveToNext(&n);
                    while (n) {
                        String name;
                        c->GetString(1, &name);
                        String value;
                        c->GetString(2, &value);
                        e->PutString(name, value);
                    }
                    Boolean bc;
                    e->Commit(&bc);
                }
            }
        //} catch (Exception e) {
        //    Log->E(TAG, "failed to migrate device properties", e);
        //} finally {
            if (c != NULL) c->Close();
            if (db != NULL) db->Close();
        //}
        Boolean isDeleted;
        databaseFile->Delete(&isDeleted);
    }
}

// check to see if the path is contained in one of our storage subdirectories
// returns TRUE if we have no special subdirectories
Boolean CMtpDatabase::InStorageSubDirectory(
    /* [in] */ const String& path)
{
    if (mSubDirectories == NULL) return TRUE;
    if (path.IsNull()) return FALSE;

    Boolean allowed = FALSE;
    Int32 pathLength = path.GetLength();
    for (Int32 i = 0; i < mSubDirectories->GetLength() && !allowed; i++) {
        String subdir = (*mSubDirectories)[i];
        Int32 subdirLength = subdir.GetLength();
        if (subdirLength < pathLength &&
                path.GetChar(subdirLength) == '/' &&
                path.StartWith(subdir)) {
            allowed = TRUE;
        }
    }
    return allowed;
}

// check to see if the path matches one of our storage subdirectories
// returns TRUE if we have no special subdirectories
Boolean CMtpDatabase::IsStorageSubDirectory(
    /* [in] */ const String& path)
{
    if (mSubDirectories == NULL) return FALSE;
    for (Int32 i = 0; i < mSubDirectories->GetLength(); i++) {
        if (path.Equals((*mSubDirectories)[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

Int32 CMtpDatabase::BeginSendObject(
    /* [in] */ const String& path,
    /* [in] */ Int32 format,
    /* [in] */ Int32 parent,
    /* [in] */ Int32 storageId,
    /* [in] */ Int64 size,
    /* [in] */ Int64 modified)
{
    // if mSubDirectories is not null, do not allow copying files to any other locations
    if (!InStorageSubDirectory(path)) return -1;

    // make sure the object does not exist
    if (path.IsNull()) {
        AutoPtr<ICursor> c;
        //try {
        AutoPtr<ArrayOf<String> > s = ArrayOf<String>::Alloc(1);
        s->Set(0, path);
        mMediaProvider->Query(mObjectsUri, ID_PROJECTION, PATH_WHERE, s, String(NULL), NULL, (ICursor**)&c);
        Int32 count;
        if (c != NULL) {
            Int32 count;
            c->GetCount(&count);
            if (count > 0) {
                // Log->W(TAG, "file already exists in beginSendObject: " + path);
                return -1;
            }
        }
        //} catch (RemoteException e) {
        //    Log->E(TAG, "RemoteException in beginSendObject", e);
        //} finally {
        if (c != NULL) {
            c->Close();
        }
        //}
    }

    mDatabaseModified = TRUE;
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(path, (ICharSequence**)&csq);
    values->PutString(IMediaStoreFilesFileColumns::DATA, csq);
    AutoPtr<IInteger32> iFormat;
    CInteger32::New(format, (IInteger32**)&iFormat);
    values->PutInt32(IMediaStoreFilesFileColumns::FORMAT, iFormat);
    AutoPtr<IInteger32> iParent;
    CInteger32::New(parent, (IInteger32**)&iParent);
    values->PutInt32(IMediaStoreFilesFileColumns::PARENT, iParent);
    AutoPtr<IInteger32> iStorageId;
    CInteger32::New(storageId, (IInteger32**)&iStorageId);
    values->PutInt32(IMediaStoreFilesFileColumns::STORAGE_ID, iStorageId);
    AutoPtr<IInteger64> iSize;
    CInteger64::New(size, (IInteger64**)&iSize);
    values->PutInt64(IMediaStoreFilesFileColumns::SIZE, iSize);
    AutoPtr<IInteger64> iModified;
    CInteger64::New(modified, (IInteger64**)&iModified);
    values->PutInt64(IMediaStoreFilesFileColumns::DATE_MODIFIED, iModified);

    //try {
        AutoPtr<IUri> uri;
        mMediaProvider->Insert(mObjectsUri, values, (IUri**)&uri);
        if (uri != NULL) {
            AutoPtr<ArrayOf<String> > s;
            uri->GetPathSegments((ArrayOf<String>**)&s);
            return StringUtils::ParseInt32((*s)[2]);
        } else {
            return -1;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in beginSendObject", e);
        return -1;
    //}
}

void CMtpDatabase::EndSendObject(
    /* [in] */ const String& path,
    /* [in] */ Int32 handle,
    /* [in] */ Int32 format,
    /* [in] */ Boolean succeeded)
{
    if (succeeded) {
        // handle abstract playlists separately
        // they do not exist in the file system so don't use the media scanner here
        if (format == IMtpConstants::FORMAT_ABSTRACT_AV_PLAYLIST) {
            // extract name from path
            String name = path;
            Int32 lastSlash = name.LastIndexOf('/');
            if (lastSlash >= 0) {
                name = name.Substring(lastSlash + 1);
            }
            // strip trailing ".pla" from the name
            if (name.EndWith(".pla")) {
                name = name.Substring(0, name.GetLength() - 4);
            }

            AutoPtr<ISystem> system;
            Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

            AutoPtr<IContentValues> values;
            CContentValues::New(1, (IContentValues**)&values);
            AutoPtr<ICharSequence> csData;
            CStringWrapper::New(path, (ICharSequence**)&csData);
            values->PutString(IMediaStoreAudioPlaylists::DATA, csData);
            AutoPtr<ICharSequence> csName;
            CStringWrapper::New(name, (ICharSequence**)&csName);
            values->PutString(IMediaStoreAudioPlaylists::NAME, csName);
            AutoPtr<IInteger32> iFormat;
            CInteger32::New(format, (IInteger32**)&iFormat);
            values->PutInt32(IMediaStoreFilesFileColumns::FORMAT, iFormat);
            Int64 millis;
            system->GetCurrentTimeMillis(&millis);

            AutoPtr<IInteger64> iModified;
            CInteger64::New(millis / 1000, (IInteger64**)&iModified);
            values->PutInt64(IMediaStoreFilesFileColumns::DATE_MODIFIED, iModified);
            AutoPtr<IInteger32> iHandle;
            CInteger32::New(handle, (IInteger32**)&iHandle);
            values->PutInt32(IMediaStoreMediaColumns::MEDIA_SCANNER_NEW_OBJECT_ID, iHandle);

            //try {
                AutoPtr<IUri> uri;
                AutoPtr<IMediaStoreAudioPlaylists> list;
                CMediaStoreAudioPlaylists::AcquireSingleton((IMediaStoreAudioPlaylists**)&list);
                AutoPtr<IUri> extUri;
                list->GetEXTERNALCONTENTURI((IUri**)&extUri);
                mMediaProvider->Insert(extUri, values, (IUri**)&uri);
            //} catch (RemoteException e) {
            //    Log->E(TAG, "RemoteException in endSendObject", e);
            //}
        } else {
            mMediaScanner->ScanMtpFile(path, mVolumeName, handle, format);
        }
    } else {
        DeleteFile(handle);
    }
}

AutoPtr<ICursor> CMtpDatabase::CreateObjectQuery(
    /* [in] */ Int32 storageID,
    /* [in] */ Int32 format,
    /* [in] */ Int32 parent)
{
    String where;
    AutoPtr<ArrayOf<String> > whereArgs;

    if (storageID == 0xFFFFFFFF) {
        // query all stores
        if (format == 0) {
            // query all formats
            if (parent == 0) {
                // query all objects
                where = NULL;
                whereArgs = NULL;
            }
            else {
                if (parent == 0xFFFFFFFF) {
                    // all objects in root of store
                    parent = 0;
                }
                where = PARENT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(1);
                whereArgs->Set(0, StringUtils::Int32ToString(parent));
            }
        }
        else {
            // query specific format
            if (parent == 0) {
                // query all objects
                where = FORMAT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(1);
                whereArgs->Set(0, StringUtils::Int32ToString(format));
            }
            else {
                if (parent == 0xFFFFFFFF) {
                    // all objects in root of store
                    parent = 0;
                }
                where = FORMAT_PARENT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(2);
                whereArgs->Set(0, StringUtils::Int32ToString(format));
                whereArgs->Set(1, StringUtils::Int32ToString(parent));
            }
        }
    }
    else {
        // query specific store
        if (format == 0) {
            // query all formats
            if (parent == 0) {
                // query all objects
                where = STORAGE_WHERE;
                whereArgs = ArrayOf<String>::Alloc(1);
                whereArgs->Set(0, StringUtils::Int32ToString(storageID));
            }
            else {
                if (parent == 0xFFFFFFFF) {
                    // all objects in root of store
                    parent = 0;
                }
                where = STORAGE_PARENT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(2);
                whereArgs->Set(0, StringUtils::Int32ToString(storageID));
                whereArgs->Set(1, StringUtils::Int32ToString(parent));
            }
        }
        else {
            // query specific format
            if (parent == 0) {
                // query all objects
                where = STORAGE_FORMAT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(2);
                whereArgs->Set(0, StringUtils::Int32ToString(storageID));
                whereArgs->Set(1, StringUtils::Int32ToString(format));
            }
            else {
                if (parent == 0xFFFFFFFF) {
                    // all objects in root of store
                    parent = 0;
                }
                where = STORAGE_FORMAT_PARENT_WHERE;
                whereArgs = ArrayOf<String>::Alloc(3);
                whereArgs->Set(0, StringUtils::Int32ToString(storageID));
                whereArgs->Set(1, StringUtils::Int32ToString(format));
                whereArgs->Set(2, StringUtils::Int32ToString(parent));
            }
        }
    }

    // if we are restricting queries to mSubDirectories, we need to add the restriction
    // onto our "where" arguments
    if (!mSubDirectoriesWhere.IsNull()) {
        if (where.IsNull()) {
            where = mSubDirectoriesWhere;
            whereArgs = mSubDirectoriesWhereArgs;
        }
        else {
            where = where + " AND " + mSubDirectoriesWhere;

            // create new array to hold whereArgs and mSubDirectoriesWhereArgs
            AutoPtr<ArrayOf<String> > newWhereArgs = ArrayOf<String>::Alloc(whereArgs->GetLength() + mSubDirectoriesWhereArgs->GetLength());
            Int32 i, j;
            for (i = 0; i < whereArgs->GetLength(); i++) {
                newWhereArgs->Set(i, (*whereArgs)[i]);
            }
            for (j = 0; j < mSubDirectoriesWhereArgs->GetLength(); i++, j++) {
                newWhereArgs->Set(i, (*mSubDirectoriesWhereArgs)[j]);
            }
            whereArgs = newWhereArgs;
        }
    }

    AutoPtr<ICursor> cur;
    mMediaProvider->Query(mObjectsUri, ID_PROJECTION, where, whereArgs, String(NULL), NULL, (ICursor**)&cur);
    return cur;
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetObjectList(
    /* [in] */ Int32 storageID,
    /* [in] */ Int32 format,
    /* [in] */ Int32 parent)
{
    AutoPtr<ICursor> c;
    //try {
    c = CreateObjectQuery(storageID, format, parent);
    if (c == NULL) {
        return NULL;
    }
    Int32 count;
    c->GetCount(&count);
    if (count > 0) {
        AutoPtr<ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(count);
        for (Int32 i = 0; i < count; i++) {
            Boolean b;
            c->MoveToNext(&b);
            Int32 value;
            c->GetInt32(0, &value);
            result->Set(i, value);
        }
        return result;
    }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getObjectList", e);
    //} finally {
        if (c != NULL) {
            c->Close();
        }
    //}
    return NULL;
}

Int32 CMtpDatabase::GetNumObjects(
    /* [in] */ Int32 storageID,
    /* [in] */ Int32 format,
    /* [in] */ Int32 parent)
{
    AutoPtr<ICursor> c;
    //try {
        c = CreateObjectQuery(storageID, format, parent);
        if (c != NULL) {
            Int32 count;
            c->GetCount(&count);
            return count;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getNumObjects", e);
    //} finally {
        if (c != NULL) {
            c->Close();
        }
    //}
    return -1;
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetSupportedPlaybackFormats()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(26);
        // allow transfering arbitrary files
    array->Set(0, IMtpConstants::FORMAT_UNDEFINED);
    array->Set(1, IMtpConstants::FORMAT_ASSOCIATION);
    array->Set(2, IMtpConstants::FORMAT_TEXT);
    array->Set(3, IMtpConstants::FORMAT_HTML);
    array->Set(4, IMtpConstants::FORMAT_WAV);
    array->Set(5, IMtpConstants::FORMAT_MP3);
    array->Set(6, IMtpConstants::FORMAT_MPEG);
    array->Set(7, IMtpConstants::FORMAT_EXIF_JPEG);
    array->Set(8, IMtpConstants::FORMAT_TIFF_EP);
    array->Set(9, IMtpConstants::FORMAT_BMP);
    array->Set(10, IMtpConstants::FORMAT_GIF);
    array->Set(11, IMtpConstants::FORMAT_JFIF);
    array->Set(12, IMtpConstants::FORMAT_PNG);
    array->Set(13, IMtpConstants::FORMAT_TIFF);
    array->Set(14, IMtpConstants::FORMAT_WMA);
    array->Set(15, IMtpConstants::FORMAT_OGG);
    array->Set(16, IMtpConstants::FORMAT_AAC);
    array->Set(17, IMtpConstants::FORMAT_MP4_CONTAINER);
    array->Set(18, IMtpConstants::FORMAT_MP2);
    array->Set(19, IMtpConstants::FORMAT_3GP_CONTAINER);
    array->Set(20, IMtpConstants::FORMAT_ABSTRACT_AV_PLAYLIST);
    array->Set(21, IMtpConstants::FORMAT_WPL_PLAYLIST);
    array->Set(22, IMtpConstants::FORMAT_M3U_PLAYLIST);
    array->Set(23, IMtpConstants::FORMAT_PLS_PLAYLIST);
    array->Set(24, IMtpConstants::FORMAT_XML_DOCUMENT);
    array->Set(25, IMtpConstants::FORMAT_FLAC);
    return array;
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetSupportedCaptureFormats()
{
    // no capture formats yet
    return NULL;
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetSupportedObjectProperties(Int32 format)
{
    switch (format) {
        case IMtpConstants::FORMAT_MP3:
        case IMtpConstants::FORMAT_WAV:
        case IMtpConstants::FORMAT_WMA:
        case IMtpConstants::FORMAT_OGG:
        case IMtpConstants::FORMAT_AAC:
            return AUDIO_PROPERTIES;
        case IMtpConstants::FORMAT_MPEG:
        case IMtpConstants::FORMAT_3GP_CONTAINER:
        case IMtpConstants::FORMAT_WMV:
            return VIDEO_PROPERTIES;
        case IMtpConstants::FORMAT_EXIF_JPEG:
        case IMtpConstants::FORMAT_GIF:
        case IMtpConstants::FORMAT_PNG:
        case IMtpConstants::FORMAT_BMP:
            return IMAGE_PROPERTIES;
        case 0:
            return ALL_PROPERTIES;
        default:
            return FILE_PROPERTIES;
    }
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetSupportedDeviceProperties()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(3);
    array->Set(0, IMtpConstants::DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER);
    array->Set(1, IMtpConstants::DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME);
    array->Set(2, IMtpConstants::DEVICE_PROPERTY_IMAGE_SIZE);
    return array;
}

AutoPtr<IMtpPropertyList> CMtpDatabase::GetObjectPropertyList(
    /* [in] */ Int64 handle,
    /* [in] */ Int32 format,
    /* [in] */ Int64 property,
    /* [in] */ Int32 groupCode,
    /* [in] */ Int32 depth)
{
    // FIXME - implement group support
    if (groupCode != 0) {
        AutoPtr<IMtpPropertyList> list;
        CMtpPropertyList::New(0, IMtpConstants::RESPONSE_SPECIFICATION_BY_GROUP_UNSUPPORTED, (IMtpPropertyList**)&list);
        return list;
    }

    AutoPtr<IMtpPropertyGroup> propertyGroup;
    if (property == 0xFFFFFFFFL) {
        AutoPtr<IInteger32> iFormat;
        CInteger32::New(format, (IInteger32**)&iFormat);
        HashMap<AutoPtr<IInteger32>, AutoPtr<IMtpPropertyGroup> >::Iterator it = mPropertyGroupsByFormat.Find(iFormat);
        if (it == mPropertyGroupsByFormat.End()) {
            AutoPtr<ArrayOf<Int32> > propertyList = GetSupportedObjectProperties(format);
            CMtpPropertyGroup::New(this, mMediaProvider, mVolumeName, propertyList, (IMtpPropertyGroup**)&propertyGroup);
            mPropertyGroupsByFormat[iFormat] = propertyGroup;
        }
    } else {
        AutoPtr<IInteger32> iProperty;
        CInteger32::New(property, (IInteger32**)&iProperty);
        HashMap<AutoPtr<IInteger32>, AutoPtr<IMtpPropertyGroup> >::Iterator it = mPropertyGroupsByProperty.Find(iProperty);
        if (it == mPropertyGroupsByProperty.End()) {
            AutoPtr<ArrayOf<Int32> > propertyList = ArrayOf<Int32>::Alloc(1);
            propertyList->Set(0, (Int32)property);
            CMtpPropertyGroup::New(this, mMediaProvider, mVolumeName, propertyList, (IMtpPropertyGroup**)&propertyGroup);
            mPropertyGroupsByProperty[iProperty] = propertyGroup;
        }
    }

    AutoPtr<IMtpPropertyList> list;
    propertyGroup->GetPropertyList((Int32)handle, format, depth, (IMtpPropertyList**)&list);
    return list;
}

Int32 CMtpDatabase::RenameFile(
    /* [in] */ Int32 handle,
    /* [in] */ const String& newName)
{
    AutoPtr<ICursor> c;

    // first compute current path
    String path;
    AutoPtr<ArrayOf<String> > whereArgs = ArrayOf<String>::Alloc(1);
    whereArgs->Set(0, StringUtils::Int32ToString(handle));
    //try {
        mMediaProvider->Query(mObjectsUri, PATH_PROJECTION, ID_WHERE, whereArgs, String(NULL), NULL, (ICursor**)&c);
        Boolean b;
        if (c != NULL && (c->MoveToNext(&b), b)) {
            c->GetString(1, &path);
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getObjectFilePath", e);
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    //} finally {
        if (c != NULL) {
            c->Close();
        }
    //}
    if (path == NULL) {
        return IMtpConstants::RESPONSE_INVALID_OBJECT_HANDLE;
    }

    // do not allow renaming any of the special subdirectories
    if (IsStorageSubDirectory(path)) {
        return IMtpConstants::RESPONSE_OBJECT_WRITE_PROTECTED;
    }

    // now rename the file.  make sure this succeeds before updating database
    AutoPtr<IFile> oldFile;
    CFile::New(path, (IFile**)&oldFile);
    Int32 lastSlash = path.LastIndexOf('/');
    if (lastSlash <= 1) {
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    }
    String newPath = path.Substring(0, lastSlash + 1) + newName;
    AutoPtr<IFile> newFile;
    CFile::New(newPath, (IFile**)&newFile);
    Boolean success;
    oldFile->RenameTo(newFile, &success);
    if (!success) {
    //    Log->W(TAG, "renaming "+ path + " to " + newPath + " failed");
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    }

    // finally update database
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(newPath, (ICharSequence**)&csq);
    values->PutString(IMediaStoreFilesFileColumns::DATA, csq);
    Int32 updated = 0;
    //try {
        // note - we are relying on a special case in MediaProvider->Update() to update
        // the paths for all children in the case where this is a directory.
        mMediaProvider->Update(mObjectsUri, values, ID_WHERE, whereArgs, &updated);
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in mMediaProvider->Update", e);
    //}
    if (updated == 0) {
        //Log->E(TAG, "Unable to update path for " + path + " to " + newPath);
        // this shouldn't happen, but if it does we need to rename the file to its original name
        newFile->RenameTo(oldFile, &success);
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    }

    // check if nomedia status changed
    newFile->IsDirectory(&b);
    String name;
    oldFile->GetName(&name);
    if (b) {
        // for directories, check if renamed from something hidden to something non-hidden
        if (name.StartWith(".") && !newPath.StartWith(".")) {
            // directory was unhidden
            //try {
                AutoPtr<IBundle> bundle;
                mMediaProvider->Call(IMediaStore::UNHIDE_CALL, newPath, NULL, (IBundle**)&bundle);
            //} catch (RemoteException e) {
            //    Log->E(TAG, "failed to unhide/rescan for " + newPath);
            //}
        }
    } else {
        // for files, check if renamed from .nomedia to something else
        if (name.ToLowerCase(/*Locale->US*/).Equals(".nomedia")
                && !newPath.ToLowerCase(/*Locale->US*/).Equals(".nomedia")) {
            //try {
                String parent;
                oldFile->GetParent(&parent);
                AutoPtr<IBundle> bundle;
                mMediaProvider->Call(IMediaStore::UNHIDE_CALL, parent, NULL, (IBundle**)&bundle);
            //} catch (RemoteException e) {
            //    Log->E(TAG, "failed to unhide/rescan for " + newPath);
            //}
        }
    }

    return IMtpConstants::RESPONSE_OK;
}

Int32 CMtpDatabase::SetObjectProperty(
    /* [in] */ Int32 handle,
    /* [in] */ Int32 property,
    /* [in] */ Int64 intValue,
    /* [in] */ const String& stringValue)
{
    switch (property) {
        case IMtpConstants::PROPERTY_OBJECT_FILE_NAME:
            return RenameFile(handle, stringValue);

        default:
            return IMtpConstants::RESPONSE_OBJECT_PROP_NOT_SUPPORTED;
    }
}

Int32 CMtpDatabase::GetDeviceProperty(
    /* [in] */ Int32 property,
    /* [in] */ ArrayOf<Int64>* outIntValue,
    /* [in] */ ArrayOf<Char32>* outStringValue)
{
    AutoPtr<IDisplay> display;
    AutoPtr<IWindowManager> wm;
    String value;
    Int32 length;
    AutoPtr<ICharSequence> csq;
    Int32 width;
    Int32 height;
    String imageSize;
    switch (property) {
        case IMtpConstants::DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER:
        case IMtpConstants::DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME:
            // writable string properties kept in shared preferences
            mDeviceProperties->GetString(StringUtils::Int32ToString(property), String(""), &value);
            length = value.GetLength();
            if (length > 255) {
                length = 255;
            }
            CStringWrapper::New(value, (ICharSequence**)&csq);
            TextUtils::GetChars(csq, 0, length, outStringValue, 0);
            outStringValue->Set(length, 0);
            return IMtpConstants::RESPONSE_OK;

        case IMtpConstants::DEVICE_PROPERTY_IMAGE_SIZE:
            // use screen size as max image size
            mContext->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&wm);
            wm->GetDefaultDisplay((IDisplay**)&display);
            display->GetMaximumSizeDimension(&width);
            display->GetMaximumSizeDimension(&height);
            imageSize = StringUtils::Int32ToString(width) + "x" +  StringUtils::Int32ToString(height);
            CStringWrapper::New(imageSize, (ICharSequence**)&csq);
            TextUtils::GetChars(csq, 0, imageSize.GetLength(), outStringValue, 0);
            outStringValue->Set(imageSize.GetLength(), 0);
            return IMtpConstants::RESPONSE_OK;

        default:
            return IMtpConstants::RESPONSE_DEVICE_PROP_NOT_SUPPORTED;
    }
}

Int32 CMtpDatabase::SetDeviceProperty(
    /* [in] */ Int32 property,
    /* [in] */ Int64 intValue,
    /* [in] */ const String& stringValue)
{
    switch (property) {
        case IMtpConstants::DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER:
        case IMtpConstants::DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME:
            // writable string properties kept in shared prefs
            AutoPtr<ISharedPreferencesEditor> e;
            mDeviceProperties->Edit((ISharedPreferencesEditor**)&e);
            e->PutString(StringUtils::Int32ToString(property), stringValue);
            Boolean b;
            e->Commit(&b);
            return (b ? IMtpConstants::RESPONSE_OK
                    : IMtpConstants::RESPONSE_GENERAL_ERROR);
    }

    return IMtpConstants::RESPONSE_DEVICE_PROP_NOT_SUPPORTED;
}

Boolean CMtpDatabase::GetObjectInfo(
    /* [in] */ Int32 handle,
    /* [in] */ ArrayOf<Int32>* outStorageFormatParent,
    /* [in] */ ArrayOf<Char32>* outName,
    /* [in] */ ArrayOf<Int64>* outModified)
{
    AutoPtr<ICursor> c;
    //try {
        AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
        array->Set(0, StringUtils::Int32ToString(handle));
        mMediaProvider->Query(mObjectsUri, OBJECT_INFO_PROJECTION,
                        ID_WHERE, array, String(NULL), NULL, (ICursor**)&c);
        Boolean b;
        if (c != NULL && (c->MoveToNext(&b),b)) {
            Int32 c1, c2, c3;
            c->GetInt32(1, &c1);
            c->GetInt32(2, &c2);
            c->GetInt32(3, &c3);
            outStorageFormatParent->Set(0, c1);
            outStorageFormatParent->Set(1, c2);
            outStorageFormatParent->Set(2, c3);

            // extract name from path
            String path;
            c->GetString(4, &path);
            Int32 lastSlash = path.LastIndexOf('/');
            Int32 start = (lastSlash >= 0 ? lastSlash + 1 : 0);
            Int32 end = path.GetLength();
            if (end - start > 255) {
                end = start + 255;
            }
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(path, (ICharSequence**)&csq);
            TextUtils::GetChars(csq, start, end, outName, 0);
            outName->Set(end - start, 0);

            Int64 value;
            c->GetInt64(5, &value);
            outModified->Set(0, value);
            return TRUE;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getObjectInfo", e);
    //} finally {
        if (c != NULL) {
            c->Close();
        }
    //}
    return FALSE;
}

Int32 CMtpDatabase::GetObjectFilePath(
    /* [in] */ Int32 handle,
    /* [in] */ ArrayOf<Char32>* outFilePath,
    /* [in] */ ArrayOf<Int64>* outFileLengthFormat)
{
    if (handle == 0) {
        // special case root directory
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(mMediaStoragePath, (ICharSequence**)&csq);
        TextUtils::GetChars(csq, 0, mMediaStoragePath.GetLength(), outFilePath, 0);
        outFilePath->Set(mMediaStoragePath.GetLength(), 0);
        outFileLengthFormat->Set(0, 0);
        outFileLengthFormat->Set(1, IMtpConstants::FORMAT_ASSOCIATION);
        return IMtpConstants::RESPONSE_OK;
    }
    AutoPtr<ICursor> c;
    //try {
        AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
        array->Set(0, StringUtils::Int32ToString(handle));
        mMediaProvider->Query(mObjectsUri, PATH_FORMAT_PROJECTION,
                        ID_WHERE, array, String(NULL), NULL, (ICursor**)&c);
        Boolean b;
        if (c != NULL && (c->MoveToNext(&b),b)) {
            String path;
            c->GetString(1, &path);
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(path, (ICharSequence**)&csq);
            TextUtils::GetChars(csq, 0, path.GetLength(), outFilePath, 0);
            outFilePath->Set(path.GetLength(), 0);
            // File transfers from device to host will likely fail if the size is incorrect.
            // So to be safe, use the actual file size here.
            AutoPtr<IFile> file;
            CFile::New(path, (IFile**)&file);
            Int64 len, value;
            file->GetLength(&len);
            outFileLengthFormat->Set(0, len);
            c->GetInt64(2, &value);
            outFileLengthFormat->Set(1, value);
            return IMtpConstants::RESPONSE_OK;
        } else {
            return IMtpConstants::RESPONSE_INVALID_OBJECT_HANDLE;
        }
        assert(0);

    if (c != NULL) {
        c->Close();
    }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getObjectFilePath", e);
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    //} finally {

    //}
}

Int32 CMtpDatabase::DeleteFile(
    /* [in] */ Int32 handle)
{
    mDatabaseModified = TRUE;
    String path;
    Int32 format;

    AutoPtr<ICursor> c;
    //try {
        AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(1);
        array->Set(0, StringUtils::Int32ToString(handle));
        mMediaProvider->Query(mObjectsUri, PATH_FORMAT_PROJECTION,
                        ID_WHERE, array, String(NULL), NULL, (ICursor**)&c);
        Boolean b;
        if (c != NULL && (c->MoveToNext(&b),b)) {
            // don't convert to media path here, since we will be matching
            // against paths in the database matching /data/media
            c->GetString(1, &path);
            c->GetInt32(2, &format);
        }
        else {
            return IMtpConstants::RESPONSE_INVALID_OBJECT_HANDLE;
        }

        if (path == NULL || format == 0) {
            return IMtpConstants::RESPONSE_GENERAL_ERROR;
        }

        // do not allow deleting any of the special subdirectories
        if (IsStorageSubDirectory(path)) {
            return IMtpConstants::RESPONSE_OBJECT_WRITE_PROTECTED;
        }

        if (format == IMtpConstants::FORMAT_ASSOCIATION) {
            // recursive case - delete all children first
            AutoPtr<IUri> uri;
            AutoPtr<IMediaStoreFiles> files;
            CMediaStoreFiles::AcquireSingleton((IMediaStoreFiles**)&files);
            files->GetMtpObjectsUri(mVolumeName, (IUri**)&uri);
            Int32 count;
            Int32 len = path.GetLength() + 1;

            AutoPtr<ArrayOf<String> > ss;
            ss->Set(0, path + "/%");
            ss->Set(1, StringUtils::Int32ToString(len));
            ss->Set(2, path + "/%");
            mMediaProvider->Delete(uri,
                // the 'like' makes it use the index, the 'lower()' makes it correct
                // when the path contains sqlite wildcard characters
                String("_data LIKE ?1 AND lower(substr(_data,1,?2))=lower(?3)"), ss, &count);
        }

        AutoPtr<IUri> uri;
        AutoPtr<IMediaStoreFiles> files;
        CMediaStoreFiles::AcquireSingleton((IMediaStoreFiles**)&files);
        files->GetMtpObjectsUri(mVolumeName, handle, (IUri**)&uri);
        Int32 v;
        mMediaProvider->Delete(uri, String(NULL), NULL, &v);
        if (v > 0) {
            if (format != IMtpConstants::FORMAT_ASSOCIATION
                    && path.ToLowerCase(/*Locale->US*/).EndWith("/.nomedia")) {
                //try {
                    String parentPath = path.Substring(0, path.LastIndexOf("/"));
                    AutoPtr<IBundle> bundle;
                    mMediaProvider->Call(IMediaStore::UNHIDE_CALL, parentPath, NULL, (IBundle**)&bundle);
                //} catch (RemoteException e) {
                //    Log->E(TAG, "failed to unhide/rescan for " + path);
                //}
            }
            return IMtpConstants::RESPONSE_OK;
        } else {
            return IMtpConstants::RESPONSE_INVALID_OBJECT_HANDLE;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in deleteFile", e);
        return IMtpConstants::RESPONSE_GENERAL_ERROR;
    //} finally {
    //    if (c != NULL) {
    //        c->Close();
    //    }
    //}
}

AutoPtr<ArrayOf<Int32> > CMtpDatabase::GetObjectReferences(
    /* [in] */ Int32 handle)
{
    AutoPtr<IUri> uri;
    AutoPtr<IMediaStoreFiles> files;
    CMediaStoreFiles::AcquireSingleton((IMediaStoreFiles**)&files);
    files->GetMtpReferencesUri(mVolumeName, handle, (IUri**)&uri);
    AutoPtr<ICursor> c;
    //try {
        mMediaProvider->Query(uri, ID_PROJECTION, String(NULL), NULL, String(NULL), NULL, (ICursor**)&c);
        if (c == NULL) {
            return NULL;
        }
        Int32 count;
        c->GetCount(&count);
        if (count > 0) {
            AutoPtr<ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(count);
            for (Int32 i = 0; i < count; i++) {
                Boolean b;
                c->MoveToNext(&b);
                Int32 value;
                c->GetInt32(0, &value);
                result->Set(i, value);
            }
            return result;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in getObjectList", e);
    //} finally {
        if (c != NULL) {
            c->Close();
        }
    //}
    return NULL;
}

Int32 CMtpDatabase::SetObjectReferences(
    /* [in] */ Int32 handle,
    /* [in] */ ArrayOf<Int32>* references)
{
    mDatabaseModified = TRUE;
    AutoPtr<IUri> uri;
    AutoPtr<IMediaStoreFiles> files;
    CMediaStoreFiles::AcquireSingleton((IMediaStoreFiles**)&files);
    files->GetMtpReferencesUri(mVolumeName, handle, (IUri**)&uri);
    Int32 count = references->GetLength();
    AutoPtr<ArrayOf<IContentValues*> > valuesList =ArrayOf<IContentValues*>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IContentValues> values;
        CContentValues::New((IContentValues**)&values);
        AutoPtr<IInteger32> iVal;
        CInteger32::New((*references)[i], (IInteger32**)&iVal);
        values->PutInt32(IMediaStoreFilesFileColumns::ID, iVal);
        valuesList->Set(i, values);
    }
    //try {
        Int32 c;
        mMediaProvider->BulkInsert(uri, valuesList, &c);
        if (c > 0) {
            return IMtpConstants::RESPONSE_OK;
        }
    //} catch (RemoteException e) {
    //    Log->E(TAG, "RemoteException in setObjectReferences", e);
    //}
    return IMtpConstants::RESPONSE_GENERAL_ERROR;
}

void CMtpDatabase::SessionStarted()
{
    mDatabaseModified = FALSE;
}

void CMtpDatabase::SessionEnded()
{
    if (mDatabaseModified) {
        AutoPtr<IIntent> intent;
        CIntent::New(IMediaStore::ACTION_MTP_SESSION_END, (IIntent**)&intent);
        mContext->SendBroadcast(intent);
        mDatabaseModified = FALSE;
    }
}

void CMtpDatabase::NativeSetup()
{
    MyMtpDatabase* database = new MyMtpDatabase(this);
    mNativeContext = (Int32)database;
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
}

void CMtpDatabase::NativeFinalize()
{
    MyMtpDatabase* database = (MyMtpDatabase *)mNativeContext;
    database->cleanup();
    delete database;
    mNativeContext = 0;
    // checkAndClearExceptionFromCallback(env, __FUNCTION__);
}


} // namespace Mtp
} // namepsace Droid
} // namespace Elastos
