
#include <pthread.h>
#include <dlfcn.h>
#include <stdio.h>
#include <elastos.h>
#include <clsinfo.h>
#include <cutils/log.h>

_ELASTOS_NAMESPACE_USING

void RelocateUnalignedPtr(void* ptr, size_t offset);

void RelocateModuleInfo(
    /* [in] */ CIModuleInfo* srcModInfo,
    /* [out] */ CIModuleInfo* destModInfo);

static CIModuleInfoNode* sModuleInfoList = NULL;

static pthread_mutex_t sModuleInfoLock;

#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
int _DumpGUID(REIID riid)
{
    ALOGD("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
        riid.Data1, riid.Data2, riid.Data3,
        riid.Data4[0], riid.Data4[1], riid.Data4[2],
        riid.Data4[3], riid.Data4[4], riid.Data4[5],
        riid.Data4[6], riid.Data4[7]);
    return printf("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
        riid.Data1, riid.Data2, riid.Data3,
        riid.Data4[0], riid.Data4[1], riid.Data4[2],
        riid.Data4[3], riid.Data4[4], riid.Data4[5],
        riid.Data4[6], riid.Data4[7]);
}

int _DumpCLSID(RClassID rclsid)
{
    _DumpGUID((REIID)rclsid);
    ALOGD("%s\n", rclsid.pUunm);
    return printf("%s\n", rclsid.pUunm);
}
#endif // _DEBUG || _MARSHAL_DEBUG

ECode InitMIL()
{
    pthread_mutexattr_t recursiveAttr;
    pthread_mutexattr_init(&recursiveAttr);
    pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);
    if (pthread_mutex_init(&sModuleInfoLock, &recursiveAttr)) {
        return E_FAIL;
    }
    pthread_mutexattr_destroy(&recursiveAttr);

    return NOERROR;
}

void UninitMIL()
{
    pthread_mutex_destroy(&sModuleInfoLock);
}

ECode RegisterModuleInfo(
    /* [in] */ CIModuleInfo* srcModuleInfo)
{
    CIModuleInfoNode* curNode;

    assert (NULL != srcModuleInfo);

    // TODO: compare component uuid, need car support
    // prevent from registering same ClassInfo again
    //
    pthread_mutex_lock(&sModuleInfoLock);
    if (srcModuleInfo->classNum != 0) {
        CIClassInfo* classInfo = (CIClassInfo *)((UInt32)srcModuleInfo->classes \
                + (UInt32)srcModuleInfo);
        curNode = sModuleInfoList;
        while (curNode != NULL) {
            if (curNode->m_pModInfo->classNum != 0 \
                && classInfo[0].clsid == \
                curNode->m_pModInfo->classes[0].clsid) {
                break;
            }
            curNode = curNode->m_pNext;
        }
    }
    else if (srcModuleInfo->interfaceNum != 0) {
        CIInterfaceInfo* interfaceInfo = (CIInterfaceInfo *)((UInt32)srcModuleInfo->interfaces \
                + (UInt32)srcModuleInfo);
        curNode = sModuleInfoList;
        while (curNode != NULL) {
            if (curNode->m_pModInfo->interfaceNum != 0 \
                && interfaceInfo[0].iid == \
                curNode->m_pModInfo->interfaces[0].iid) {
                break;
            }
            curNode = curNode->m_pNext;
        }
    }
    else {
        pthread_mutex_unlock(&sModuleInfoLock);
        // No class info need to register
        return NOERROR;
    }

    if (curNode != NULL) {
        pthread_mutex_unlock(&sModuleInfoLock);
        return NOERROR;
    }

    CIModuleInfo* moduleInfo = (CIModuleInfo *)malloc(srcModuleInfo->totalSize);
    if (NULL == moduleInfo) {
        pthread_mutex_unlock(&sModuleInfoLock);
        return E_OUT_OF_MEMORY;
    }
    RelocateModuleInfo(srcModuleInfo, moduleInfo);

    CIModuleInfoNode* newNode = (CIModuleInfoNode *)malloc(sizeof(CIModuleInfoNode));
    if (newNode == NULL) {
        free(moduleInfo);
        pthread_mutex_unlock(&sModuleInfoLock);
        return E_OUT_OF_MEMORY;
    }
    newNode->m_pModInfo = (CIModuleInfo *)moduleInfo;
    newNode->m_pNext = NULL;
    if (sModuleInfoList == NULL) {
        sModuleInfoList = newNode;
    }
    else {
        newNode->m_pNext = sModuleInfoList->m_pNext;
        sModuleInfoList->m_pNext = newNode;
    }
    pthread_mutex_unlock(&sModuleInfoLock);

    return NOERROR;
}

ECode UnregisterModuleInfo(
    /* [in] */ CIModuleInfo* moduleInfo)
{
    assert(moduleInfo != NULL);

    pthread_mutex_lock(&sModuleInfoLock);
    CIModuleInfoNode* preNode = NULL;
    CIModuleInfoNode* curNode = sModuleInfoList;
    while (curNode != NULL) {
        if ((*curNode).m_pModInfo == moduleInfo) {
            break;
        }
        preNode = curNode;
        curNode = curNode->m_pNext;
    }

    if (curNode != NULL) {
        if (preNode == NULL) {
            sModuleInfoList = curNode->m_pNext;
        }
        else {
            preNode->m_pNext = curNode->m_pNext;
        }
        pthread_mutex_unlock(&sModuleInfoLock);
        free(curNode);

        return NOERROR;
    }

    pthread_mutex_unlock(&sModuleInfoLock);
    return E_DOES_NOT_EXIST;
}

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo** classInfo)
{
    assert(classInfo != NULL);

    pthread_mutex_lock(&sModuleInfoLock);
    CIModuleInfoNode* curNode = sModuleInfoList;
    while (curNode != NULL) {
        CIModuleInfo* modInfo = curNode->m_pModInfo;
        for (Int32 m = 0; m < modInfo->classNum; m++) {
            if (modInfo->classes[m].clsid == rclsid) {
                *classInfo = &(modInfo->classes[m]);
                pthread_mutex_unlock(&sModuleInfoLock);
                return NOERROR;
            }
        }
        curNode = curNode->m_pNext;
    }
    pthread_mutex_unlock(&sModuleInfoLock);

    return E_DOES_NOT_EXIST;
}

ECode LookupModuleInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIModuleInfo** moduleInfo)
{
    assert(moduleInfo != NULL);

    pthread_mutex_lock(&sModuleInfoLock);
    CIModuleInfoNode* curNode = sModuleInfoList;
    while (curNode != NULL) {
        CIModuleInfo* modInfo = curNode->m_pModInfo;
        for (Int32 m = 0; m < modInfo->classNum; m++) {
            if (modInfo->classes[m].clsid == rclsid) {
                *moduleInfo = modInfo;
                pthread_mutex_unlock(&sModuleInfoLock);
                return NOERROR;
            }
        }
        curNode = curNode->m_pNext;
    }
    pthread_mutex_unlock(&sModuleInfoLock);

    return E_DOES_NOT_EXIST;
}

// TODO:
void *GetUnalignedPtr(void* ptr)
{
#ifdef _x86
    return *(void **)ptr;
#else
    union
    {
        void    *p;
        byte_t  bytes[4];
    } u;
    u.bytes[0] = ((byte_t *)ptr)[0];
    u.bytes[1] = ((byte_t *)ptr)[1];
    u.bytes[2] = ((byte_t *)ptr)[2];
    u.bytes[3] = ((byte_t *)ptr)[3];

    return u.p;
#endif
}

ECode AcquireClassInfo(
    /* [in] */ const ClassID& classId,
    /* [out] */ CIClassInfo** clsInfo)
{
    typedef ECode (STDCALL *CarDllGetClassObject_t)(
            REMuid clsid, REIID riid, IInterface** clsObj);

    if (clsInfo == NULL) return E_INVALID_ARGUMENT;

    char path[260];
    strcpy(path, classId.pUunm);
    void* module = dlopen(path, RTLD_NOW);

    strcpy(path, "DllGetClassObject");
    CarDllGetClassObject_t func = (CarDllGetClassObject_t)dlsym(module, path);

    CIModuleInfo* modInfo;
    func(ECLSID_ClassInfo, EIID_IInterface, (IInterface**)&modInfo);

    ECode ec = RegisterModuleInfo(modInfo);
    if (FAILED(ec)) {
#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
        _DumpCLSID(classId);
        ALOGD("Failed to RegisterModuleInfo in %s", classId.pUunm);
#endif
        return ec;
    }

    ec = LookupModuleInfo(classId.clsid, &modInfo);
    if (FAILED(ec)) {
#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
        _DumpCLSID(classId);
        ALOGD("Failed to LookupModuleInfo in %s", classId.pUunm);
#endif
        return ec;
    }

    for (Int32 m = 0; m < modInfo->classNum; m++) {
        if (modInfo->classes[m].clsid == classId.clsid) {
            *clsInfo = &(modInfo->classes[m]);
            return NOERROR;
        }
    }

#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
    _DumpCLSID(classId);
    ALOGD("Failed to AcquireClassInfo in %s", classId.pUunm);
#endif
    return E_DOES_NOT_EXIST;
}
