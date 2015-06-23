
#include "media/drm/mobile1/CDrmRawContent.h"
#include "ext/frameworkext.h"
#include <objmng/svc_drm.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;
using Elastos::IO::EIID_IInputStream;
using Elastos::IO::CBufferedInputStream;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

const Int32 CDrmRawContent::JNI_DRM_SUCCESS;
const Int32 CDrmRawContent::JNI_DRM_FAILURE;
const Int32 CDrmRawContent::DRM_MIMETYPE_MESSAGE;
const Int32 CDrmRawContent::DRM_MIMETYPE_CONTENT;
const Int32 CDrmRawContent::JNI_DRM_EOF;
const Int32 CDrmRawContent::JNI_DRM_UNKNOWN_DATA_LEN;

/**
 * DRM data structure
 */
typedef struct _DrmData {
    /**
     * The id of the DRM content.
     */
    Int32 id;

    /**
     * The pointer of DRM raw content InputStream object.
     */
    AutoPtr<IInputStream> pInData;

    /**
     * The len of the InputStream object.
     */
    Int32 len;

    /**
     * The next DRM data.
     */
    struct _DrmData *next;
} DrmData;

static DrmData *drmTable = NULL; /** The table to hold all the DRM data. */

/**
 * Allocate a new item of DrmData.
 *
 * \return a pointer to a DrmData item if allocate successfully,
 *         otherwise return NULL
 */

static DrmData * NewItem(void)
{
    DrmData *d = (DrmData *)malloc(sizeof(DrmData));

    if (d != NULL) {
        d->id = -1;
        d->next = NULL;
    }

    return d;
}

/**
 * Free the memory of the specified DrmData item <code>d</code>.
 *
 * \param d - a pointer to DrmData
 */
static void FreeItem(
    /* [in] */ DrmData *d)
{
    assert(d != NULL);

    free(d);
}


/**
 * Insert a DrmData item with given <code>name</code> into the head of
 * the DrmData list.
 *
 * @param d - the pointer of the JNI interface
 * @param pInData - the pointer of the DRM content InputStream object.
 *
 * @return <code>JNI_DRM_SUCCESS</code> if insert successfully, otherwise
 *         return <code>JNI_DRM_FAILURE</code>
 */

static Int32 AddItem(
    /* [in] */ DrmData* d)
{
    if (NULL == d)
        return CDrmRawContent::JNI_DRM_FAILURE;

    if (NULL == drmTable) {
        drmTable = d;
        return CDrmRawContent::JNI_DRM_SUCCESS;
    }

    d->next = drmTable;
    drmTable = d;

    return CDrmRawContent::JNI_DRM_SUCCESS;
}

/**
 * Get the item from the DrmData list by the specified <code>
 * id</code>.
 *
 * @param p - the pointer of the DRM content InputStream object.
 *
 * @return a pointer to the DrmData item if find it successfuly,
 *         otherwise return NULL
 */

static DrmData* GetItem(
    /* [in] */ Int32 id)
{
    DrmData *d;

    if (NULL == drmTable)
        return NULL;

    for (d = drmTable; d != NULL; d = d->next) {
        if (id == d->id)
            return d;
    }

    return NULL;
}

/**
 * Remove the specified DrmData item <code>d</code>.
 *
 * @param p - the pointer of the DRM content InputStream object.
 *
 * @return <code>JNI_DRM_SUCCESS</code> if remove successfuly,
 *         otherwise return <code>JNI_DRM_FAILURE</code>
 */
static int32_t RemoveItem(int32_t id)
{
    DrmData *curItem, *preItem;

    if (NULL == drmTable)
        return CDrmRawContent::JNI_DRM_FAILURE;

    preItem = NULL;
    for (curItem = drmTable; curItem != NULL; curItem = curItem->next) {
        if (id == curItem->id) {
            if (curItem == drmTable)
                drmTable = curItem->next;
            else
                preItem->next = curItem->next;

            FreeItem(curItem);

            return CDrmRawContent::JNI_DRM_SUCCESS;
        }

        preItem = curItem;
    }

    return CDrmRawContent::JNI_DRM_FAILURE;
}

static Int32 GetInputStreamDataLength(
    /* [in] */ Int32 handle)
{
    AutoPtr<IInputStream> inputStream;
    Int32 len;
    DrmData* p;

    p = (DrmData *)handle;

    if (NULL == p){
        return 0;
    }

    len = p->len;
    inputStream = p->pInData;

    if (p->len <= 0 || inputStream == NULL){
        return 0;
    }

    Int32 number;
    inputStream->Available(&number);
    if (0 > number)
        return 0;

    return len;
}

extern Int32 ReadInputStreamData(
    /* [in] */ Int32 handle,
    /* [in] */ uint8_t* buf,
    /* [in] */ Int32 bufLen);

// native interface
Int32 CDrmRawContent::NativeConstructDrmContent(
    /* [in] */ IInputStream *data,
    /* [in] */ Int32 len,
    /* [in] */ Int32 mimeType)
{
    Int32 id;
    T_DRM_Input_Data inData;
    DrmData* drmInData;

    switch (mimeType) {
    case DRM_MIMETYPE_MESSAGE:
        mimeType = TYPE_DRM_MESSAGE;
        break;
    case DRM_MIMETYPE_CONTENT:
        mimeType = TYPE_DRM_CONTENT;
        break;
    default:
        return JNI_DRM_FAILURE;
    }

    drmInData = NewItem();
    if (NULL == drmInData)
        return JNI_DRM_FAILURE;

    drmInData->pInData = data;
    drmInData->len = len;

    if (JNI_DRM_FAILURE == AddItem(drmInData))
        return JNI_DRM_FAILURE;

    inData.inputHandle = (int32_t)drmInData;
    inData.mimeType = mimeType;
    inData.getInputDataLength = GetInputStreamDataLength;
    inData.readInputData = ReadInputStreamData;

    id = SVC_drm_openSession(inData);
    if (id < 0)
        return JNI_DRM_FAILURE;

    drmInData->id = id;

    return id;
}

String CDrmRawContent::NativeGetRightsAddress()
{
    uint8_t rightsIssuer[256] = {0};
    String str("");

    if (DRM_SUCCESS == SVC_drm_getRightsIssuer(mId, rightsIssuer)) {
        str = String((char*)rightsIssuer);
    }

    return str;
}

Int32 CDrmRawContent::NativeGetDeliveryMethod()
{
    Int32 res = SVC_drm_getDeliveryMethod(mId);

    switch (res) {
    case FORWARD_LOCK:
        return IDrmRawContent::DRM_FORWARD_LOCK;
    case COMBINED_DELIVERY:
        return IDrmRawContent::DRM_COMBINED_DELIVERY;
    case SEPARATE_DELIVERY:
        return IDrmRawContent::DRM_SEPARATE_DELIVERY;
    case SEPARATE_DELIVERY_FL:
        return IDrmRawContent::DRM_SEPARATE_DELIVERY_DM;
    default:
        return JNI_DRM_FAILURE;
    }
}

Int32 CDrmRawContent::NativeReadContent(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 bufOff,
    /* [in] */ Int32 len,
    /* [in] */ Int32 mediaOff)
{
    DrmData* p;

    if (NULL == buf) {
        return CDrmRawContent::JNI_DRM_FAILURE;
    }

    if (len < 0 || bufOff < 0 || len + bufOff > buf->GetLength()) {
        return CDrmRawContent::JNI_DRM_FAILURE;
    }

    if (mediaOff < 0 || len == 0)
        return CDrmRawContent::JNI_DRM_FAILURE;

    p = GetItem(mId);
    if (NULL == p)
        return CDrmRawContent::JNI_DRM_FAILURE;

    p->pInData = mInData;

    len = SVC_drm_getContent(mId, mediaOff, (uint8_t *)buf->GetPayload() + bufOff, len);

    if (DRM_MEDIA_EOF == len){
       return CDrmRawContent::JNI_DRM_EOF;
    }

    if (len <= 0){
        return CDrmRawContent::JNI_DRM_FAILURE;
    }

    return len;
}

String CDrmRawContent::NativeGetContentType()
{
    uint8_t contentType[64] = {0};
    String str("");

    if (DRM_SUCCESS == SVC_drm_getContentType(mId, contentType)){
        str = String((char*)contentType);
    }

    return str;
}

Int32 CDrmRawContent::NativeGetContentLength()
{
    Int32 len = SVC_drm_getContentLength(mId);

    if (DRM_UNKNOWN_DATA_LEN == len)
        return CDrmRawContent::JNI_DRM_UNKNOWN_DATA_LEN;

    if (0 > len)
        return CDrmRawContent::JNI_DRM_FAILURE;

    return len;
}

CDrmRawContent::CDrmRawContent()
    : mInData(NULL)
    , mInDataLen(0)
    , mId(0)
    , mRightsIssuer(NULL)
    , mMediaType(NULL)
    , mRawType(0)
{
}

CDrmRawContent::~CDrmRawContent()
{
    RemoveItem(mId);
    SVC_drm_closeSession(mId);
}

ECode CDrmRawContent::constructor(
    /* [in] */ IInputStream* inRawdata,
    /* [in] */ Int32 len,
    /* [in] */ const String& mimeTypeStr)
{
    Int32 mimeType;

    mId = -1;
    CBufferedInputStream::New(inRawdata,1024 ,(IBufferedInputStream**)&mInData);
    mInDataLen = len;

    if (mimeTypeStr == IDrmRawContent::DRM_MIMETYPE_MESSAGE_STRING){
        mimeType = DRM_MIMETYPE_MESSAGE;
    }
    else if (IDrmRawContent::DRM_MIMETYPE_CONTENT_STRING == mimeTypeStr){
        mimeType = DRM_MIMETYPE_CONTENT;
    }
    else
    //    throw new IllegalArgumentException("mimeType must be DRM_MIMETYPE_MESSAGE or DRM_MIMETYPE_CONTENT");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (len <= 0){
    //    throw new IllegalArgumentException("len must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    /* call native method to initialize this DRM content */
    mId = NativeConstructDrmContent(mInData, mInDataLen, mimeType);

    if (JNI_DRM_FAILURE == mId){
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    /* init the rights issuer field. */
    mRightsIssuer = NativeGetRightsAddress();

    /* init the raw content type. */
    mRawType = NativeGetDeliveryMethod();
    if (JNI_DRM_FAILURE == mRawType){
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    /* init the media content type. */
    mMediaType = NativeGetContentType();
    if (NULL == mMediaType){
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode CDrmRawContent::GetRightsAddress(
    /* [out] */ String* rightsIssuer)
{
    VALIDATE_NOT_NULL(rightsIssuer);
    *rightsIssuer = mRightsIssuer;

    return NOERROR;
}

ECode CDrmRawContent::GetRawType(
    /* [out] */ Int32* rawType)
{
    VALIDATE_NOT_NULL(rawType);
    *rawType = mRawType;

    return NOERROR;
}

ECode CDrmRawContent::GetContentInputStream(
    /* [in] */ IDrmRights* mRights,
    /* [out] */ IInputStream** rights)
{
    VALIDATE_NOT_NULL(mRights);
    if (NULL == mRights){
        return E_NULL_POINTER_EXCEPTION;
    }

    *rights = new DrmInputStream(mRights, this);
    REFCOUNT_ADD(*rights);

    return NOERROR;
}

ECode CDrmRawContent::GetContentType(
    /* [out] */ String* mediaType)
{
    VALIDATE_NOT_NULL(mediaType);
    *mediaType = mMediaType;

    return NOERROR;
}

ECode CDrmRawContent::GetContentLength(
    /* [in] */ IDrmRights* mRights,
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);

    if (NULL == mRights){
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 mediaLen = NativeGetContentLength();

    if (JNI_DRM_FAILURE == mediaLen){
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    if (JNI_DRM_UNKNOWN_DATA_LEN == mediaLen){
        return DRM_UNKNOWN_DATA_LEN;
    }

    if (JNI_DRM_UNKNOWN_DATA_LEN == mediaLen)
        *len = DRM_UNKNOWN_DATA_LEN;
    else
        *len = mediaLen;

    return NOERROR;
}

ECode CDrmRawContent::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

CDrmRawContent::DrmInputStream::DrmInputStream(
    /* [in] */ IDrmRights* rights,
    /* [in] */ CDrmRawContent* drmRawContent)
    : mIsClosed(FALSE)
    , mOffset(0)
    , mDrmRawContent(drmRawContent)
{
    mB = ArrayOf<Byte>::Alloc(1);
}

CDrmRawContent::DrmInputStream::~DrmInputStream()
{
}

PInterface CDrmRawContent::DrmInputStream::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IInputStream) {
        return (PInterface)(IInputStream*)this;
    }

    return NULL;
}

UInt32 CDrmRawContent::DrmInputStream::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CDrmRawContent::DrmInputStream::Release()
{
    return ElRefBase::Release();
}

ECode CDrmRawContent::DrmInputStream::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [in] */ InterfaceID* pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }
    if (pObject == (IInterface*)(IInputStream*)this) {
        *pIID = EIID_IInputStream;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    /* call native method to get this DRM decrypted media content length */
    Int32 len = mDrmRawContent->NativeGetContentLength();

    if (JNI_DRM_FAILURE == len){
        return E_IO_EXCEPTION;
    }

    /* if the length is unknown, just return 0 for available value */
    if (JNI_DRM_UNKNOWN_DATA_LEN == len){
        return 0;
    }

    Int32 availableLen = len - mOffset;
    if (availableLen < 0){
        return E_IO_EXCEPTION;
    }

    *number = availableLen;

    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 res;
    ReadBytes(mB, 0, 1, &res);

    if (-1 == res){
        *value = -1;
    }
    else {
        *value = (*mB)[0] & 0xff;
    }

    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::ReadBytes(
    /* [in] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* number)
{
    return ReadBytes(b, 0, b->GetLength(), number);
}

ECode CDrmRawContent::DrmInputStream::ReadBytes(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    if (NULL == b){
        return E_NULL_POINTER_EXCEPTION;
    }

    if (off < 0 || len < 0 || off + len > b->GetLength()){
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (TRUE == mIsClosed){
        return E_IO_EXCEPTION;
    }

    if (0 == len){
        return 0;
    }

    len = mDrmRawContent->NativeReadContent(b, off, len, mOffset);

    if (JNI_DRM_FAILURE == len){
        return E_IO_EXCEPTION;
    }
    else if (JNI_DRM_EOF == len){
        return -1;
    }

    mOffset += len;
    *number = len;

    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    *supported = FALSE;

    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode CDrmRawContent::DrmInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)
    *number = 0;

    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::Close()
{
    mIsClosed = TRUE;
    return NOERROR;
}

ECode CDrmRawContent::DrmInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = InputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos
