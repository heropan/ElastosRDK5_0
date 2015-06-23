
#include "ext/frameworkext.h"
#include "media/drm/mobile1/CDrmRightsManager.h"
#include "media/drm/mobile1/CDrmRights.h"
#include "media/drm/mobile1/CDrmRawContent.h"
#include <objmng/svc_drm.h>
#include <elastos/utility/etl/List.h>
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

const Int32 CDrmRightsManager::DRM_MIMETYPE_RIGHTS_XML;
const Int32 CDrmRightsManager::DRM_MIMETYPE_RIGHTS_WBXML;
const Int32 CDrmRightsManager::DRM_MIMETYPE_MESSAGE;
const Int32 CDrmRightsManager::JNI_DRM_SUCCESS;
const Int32 CDrmRightsManager::JNI_DRM_FAILURE;
AutoPtr<IDrmRightsManager> CDrmRightsManager::sSingleton;

/**
 * Define the mime type of DRM data.
 */
#define TYPE_DRM_MESSAGE            0x48    /**< The mime type is "application/vnd.oma.drm.message" */
#define TYPE_DRM_RIGHTS_XML         0x4a    /**< The mime type is "application/vnd.oma.drm.rights+xml" */
#define TYPE_DRM_RIGHTS_WBXML       0x4b    /**< The mime type is "application/vnd.oma.drm.rights+wbxml" */

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

Int32 ReadInputStreamData(
    /* [in] */ Int32 handle,
    /* [in] */ uint8_t* buf,
    /* [in] */ Int32 bufLen)
{
    AutoPtr<IInputStream> inputStream;
    Int32 len;
    DrmData* p;

    p = (DrmData *)handle;

    if (NULL == p || NULL == buf || bufLen <- 0)
        return 0;

    inputStream = p->pInData;
    len = p->len;

    if (p->len <= 0 || inputStream == NULL)
        return 0;

    AutoPtr<ArrayOf<Byte> > tmp = ArrayOf<Byte>::Alloc(bufLen);
    inputStream->ReadBytes(tmp, 0, bufLen, &len);
    memcpy(buf, tmp->GetPayload(), len);

    if (-1 == len)
        return -1;

    return len;
}

static Int32 SetRightsFields(
    /* [in] */ IDrmRights* rights,
    /* [in] */ T_DRM_Rights_Info* pRoInfo)
{
    if (NULL == rights){
        return CDrmRightsManager::JNI_DRM_FAILURE;
    }

    /* set roId field */
    //field = GetFieldID(env, clazz, "roId", "Ljava/lang/String;");
    //str = NewStringUTF(env, (char *)pRoInfo->roId);
    //SetObjectField(rights, field, str);


    return CDrmRightsManager::JNI_DRM_SUCCESS;
}

Int32 CDrmRightsManager::NativeInstallDrmRights(
    /* [in] */ IInputStream* data,
    /* [in] */ Int32 len,
    /* [in] */ Int32 mimeType,
    /* [in] */ IDrmRights* rights)
{
    T_DRM_Input_Data inData;
    DrmData* drmInData;
    T_DRM_Rights_Info rightsInfo;

    switch (mimeType) {
    case DRM_MIMETYPE_RIGHTS_XML:
        mimeType = TYPE_DRM_RIGHTS_XML;
        break;
    case DRM_MIMETYPE_RIGHTS_WBXML:
        mimeType = TYPE_DRM_RIGHTS_WBXML;
        break;
    case DRM_MIMETYPE_MESSAGE:
        mimeType = TYPE_DRM_MESSAGE;
        break;
    default:
        return JNI_DRM_FAILURE;
    }

    drmInData = NewItem();
    if (NULL == drmInData){
        return JNI_DRM_FAILURE;
    }

    drmInData->pInData = data;
    drmInData->len = len;

    inData.inputHandle = (int32_t)drmInData;
    inData.mimeType = mimeType;
    inData.getInputDataLength = GetInputStreamDataLength;
    inData.readInputData = ReadInputStreamData;

    memset(&rightsInfo, 0, sizeof(T_DRM_Rights_Info));
    if (DRM_FAILURE == SVC_drm_installRights(inData, &rightsInfo))
        return JNI_DRM_FAILURE;

    FreeItem(drmInData);

    return SetRightsFields(rights, &rightsInfo);
}

Int32 CDrmRightsManager::NativeQueryRights(
    /* [in] */ IDrmRawContent* rawContent,
    /* [in] */ IDrmRights* rights)
{
    Int32 id;
    T_DRM_Rights_Info rightsInfo;

    rawContent->GetId(&id);

    memset(&rightsInfo, 0, sizeof(T_DRM_Rights_Info));
    if (DRM_SUCCESS != SVC_drm_getRightsInfo(id, &rightsInfo))
        return JNI_DRM_FAILURE;

    return SetRightsFields(rights, &rightsInfo);
}

Int32 CDrmRightsManager::NativeGetNumOfRights()
{
    T_DRM_Rights_Info_Node *pRightsList;
    T_DRM_Rights_Info_Node *pCurNode;
    int32_t num = 0;

    if (DRM_FAILURE == SVC_drm_viewAllRights(&pRightsList))
        return JNI_DRM_FAILURE;

    pCurNode = pRightsList;
    while (pCurNode != NULL) {
        num++;
        pCurNode = pCurNode->next;
    }

    SVC_drm_freeRightsInfoList(pRightsList);

    return num;
}

Int32 CDrmRightsManager::NativeGetRightsList(
    /* [in] */ ArrayOf<IDrmRights*>* rightsArray,
    /* [in] */ Int32 numRights)
{
    T_DRM_Rights_Info_Node *pRightsList;
    T_DRM_Rights_Info_Node *pCurNode;
    int32_t index;

    if (DRM_FAILURE == SVC_drm_viewAllRights(&pRightsList))
        return JNI_DRM_FAILURE;

    pCurNode = pRightsList;
    for (index = 0; NULL != pCurNode; index++) {
        IDrmRights* mRights;
        mRights = (*rightsArray)[index];
        if (NULL == mRights)
            break;

        if (JNI_DRM_FAILURE == SetRightsFields(mRights, &(pCurNode->roInfo)))
            break;

        pCurNode = pCurNode->next;
    }

    SVC_drm_freeRightsInfoList(pRightsList);

    return index;
}

Int32 CDrmRightsManager::NativeDeleteRights(
    /* [in] */ IDrmRights* rights)
{
    if(rights == NULL) return JNI_DRM_FAILURE;

    String roId;
    rights->GetRoId(&roId);
    if(roId.IsNull()) return JNI_DRM_FAILURE;

    if (0 == strcmp("ForwardLock", roId.string()))
        return JNI_DRM_SUCCESS;

    if (DRM_SUCCESS != SVC_drm_deleteRights((uint8_t*)(roId.string()))) {
        return JNI_DRM_FAILURE;
    }

    return JNI_DRM_SUCCESS;
}

ECode CDrmRightsManager::GetInstance(
    /* [out] */ IDrmRightsManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    if (sSingleton == NULL) {
        CDrmRightsManager::New((IDrmRightsManager**)&sSingleton);
    }

    *manager = sSingleton;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode CDrmRightsManager::InstallRights(
    /* [in] */ IInputStream* rightsData,
    /* [in] */ Int32 length,
    /* [in] */ const String& mimeTypeStr,
    /* [out] */ IDrmRights** rights)
{
    VALIDATE_NOT_NULL(rights);

    Mutex::Autolock lock(&_m_syncLock);

    Int32 mMimeType = 0;

    if (mimeTypeStr.Equals(IDrmRightsManager::DRM_MIMETYPE_RIGHTS_XML_STRING)){
        mMimeType = DRM_MIMETYPE_RIGHTS_XML;
    }
    else if (mimeTypeStr.Equals(IDrmRightsManager::DRM_MIMETYPE_RIGHTS_WBXML_STRING)){
        mMimeType = DRM_MIMETYPE_RIGHTS_WBXML;
    }
    else if (mimeTypeStr.Equals(IDrmRawContent::DRM_MIMETYPE_MESSAGE_STRING)){
        mMimeType = DRM_MIMETYPE_MESSAGE;
    }
    else{
        //throw new IllegalArgumentException("mimeType must be DRM_MIMETYPE_RIGHTS_XML or DRM_MIMETYPE_RIGHTS_WBXML or DRM_MIMETYPE_MESSAGE");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (length <= 0){
        *rights = NULL;
        return NOERROR;
    }

    AutoPtr<IDrmRights> rightsObj;
    CDrmRights::New((IDrmRights**)&rightsObj);

    /* call native method to install this rights object. */
    Int32 res = NativeInstallDrmRights(rightsData, length, mMimeType, rightsObj);

    if (JNI_DRM_FAILURE == res){
        //throw new DrmException("nativeInstallDrmRights() returned JNI_DRM_FAILURE");
        assert(0);
        return E_RUNTIME_EXCEPTION;
    }

    *rights = rightsObj;
    REFCOUNT_ADD(*rights);
    return NOERROR;
}

ECode CDrmRightsManager::QueryRights(
    /* [in] */ IDrmRawContent * content,
    /* [out] */ IDrmRights ** rights)
{
    VALIDATE_NOT_NULL(rights);

    Mutex::Autolock lock(&_m_syncLock);

    AutoPtr<IDrmRights> rightsObj;
    CDrmRights::New((IDrmRights**)&rightsObj);

    /* call native method to query the rights */
    Int32 res = NativeQueryRights(content, rightsObj);

    if (JNI_DRM_FAILURE == res){
        *rights = NULL;
        return NOERROR;
    }

    *rights = rightsObj;
    REFCOUNT_ADD(*rights);
    return NOERROR;
}

ECode CDrmRightsManager::GetRightsList(
    /* [out] */ IObjectContainer** rightsList)
{
    VALIDATE_NOT_NULL(rightsList);

    Mutex::Autolock lock(&_m_syncLock);

    /* call native method to get how many rights object in current agent */
    Int32 mNum = NativeGetNumOfRights();

    if (JNI_DRM_FAILURE == mNum) {
        *rightsList = NULL;
        return NOERROR;
    }

    AutoPtr<IObjectContainer> list;
    CObjectContainer::New((IObjectContainer**)&list);

    if (mNum > 0) {
        AutoPtr<ArrayOf<IDrmRights*> > rightsArray = ArrayOf<IDrmRights*>::Alloc(mNum);
        Int32 i;

        for (i = 0; i < mNum; i++) {
            AutoPtr<IDrmRights> dr;
            CDrmRights::New((IDrmRights**)&dr);
            rightsArray->Set(i, dr);
        }

        /* call native method to get all the rights information */
        mNum = NativeGetRightsList(rightsArray, mNum);

        if (JNI_DRM_FAILURE == mNum) {
            *rightsList = NULL;
            return NOERROR;
        }

        /* add all rights informations to ArrayList */
        for (i = 0; i < mNum; i++) {
            list->Add((*rightsArray)[i]);
        }
    }

    *rightsList = list;
    REFCOUNT_ADD(*rightsList);
    return NOERROR;
}

ECode CDrmRightsManager::DeleteRights(
    /* [in] */ IDrmRights* rights)
{
    Mutex::Autolock lock(&_m_syncLock);

    /* call native method to delete the specified rights object */
    NativeDeleteRights(rights);

    return NOERROR;
}

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos
