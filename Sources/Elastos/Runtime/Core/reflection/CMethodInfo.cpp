//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CMethodInfo.h"
#include "CParamInfo.h"
#include "CArgumentList.h"
#include "CCallbackArgumentList.h"

#define INVALID_PARAM_COUNT 0xFFFFFFFF

EXTERN_C int invoke(void* pFunc, int *pParam, int nSize);

struct VTable
{
    void* methods[1];
};

struct VObject
{
    VTable* vtab;
};

CMethodInfo::CMethodInfo(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ MethodDescriptor *pMethodDescriptor,
    /* [in] */ UInt32 uIndex)
{
    m_pMethodDescriptor = pMethodDescriptor;
    mIndex = uIndex;
    m_pCClsModule = pCClsModule;
    m_pClsMod = m_pCClsModule->mClsMod;
    m_pParamElem = NULL;
    m_pParameterInfos = NULL;
    m_nBase = m_pCClsModule->mBase;
}

CMethodInfo::~CMethodInfo()
{
    Int32 count = m_pMethodDescriptor->cParams;
    if (m_pParameterInfos) {
        for (Int32 i = 0; i < count; i++) {
            if ((*m_pParameterInfos)[i]) {
                delete (*m_pParameterInfos)[i];
                (*m_pParameterInfos)[i] = NULL;
            }
        }
        ArrayOf<IParamInfo *>::Free(m_pParameterInfos);
    }

    if (m_pParamElem) {
        delete [] m_pParamElem;
    }
}

UInt32 CMethodInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CMethodInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_Method);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.RemoveMethodInfo(m_pMethodDescriptor, mIndex);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_Method);
    assert(nRef >= 0);
    return nRef;
}

PInterface CMethodInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IMethodInfo) {
        return (IMethodInfo *)this;
    }
    else if (riid == EIID_IFunctionInfo) {
        return (IFunctionInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CMethodInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CMethodInfo::Init()
{
    ECode ec = InitParmElement();
    if (FAILED(ec)) return ec;

    return InitParamInfos();
}

ECode CMethodInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = adjustNameAddr(m_nBase, m_pMethodDescriptor->pszName);

    return NOERROR;
}

ECode CMethodInfo::GetParamCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pMethodDescriptor->cParams;
    return NOERROR;
}

ECode CMethodInfo::GetAllParamInfos(
    /* [out] */ ArrayOf<IParamInfo *> * pParamInfos)
{
    if (!pParamInfos || !pParamInfos->GetLength()) {
        return E_INVALID_ARGUMENT;
    }

    pParamInfos->Copy(m_pParameterInfos);

    return NOERROR;
}

ECode CMethodInfo::GetParamInfoByIndex(
    /* [in] */ Int32 index,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    if (!ppParamInfo || index < 0) {
        return E_INVALID_ARGUMENT;
    }

    if (index >= m_pParameterInfos->GetLength()) {
        return E_DOES_NOT_EXIST;
    }

    *ppParamInfo = (IParamInfo *)(*m_pParameterInfos)[index];
    (*ppParamInfo)->AddRef();

    return NOERROR;
}

ECode CMethodInfo::GetParamInfoByName(
    /* [in] */ const String& name,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    if (name.IsNull() || !ppParamInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pMethodDescriptor->cParams) {
        return E_DOES_NOT_EXIST;
    }

    ParamDescriptor* pParam = NULL;
    for (Int32 i = 0; i < m_pMethodDescriptor->cParams; i++) {
        pParam = getParamDescAddr(m_nBase, m_pMethodDescriptor->ppParams, i);
        if (name.Equals(adjustNameAddr(m_nBase, pParam->pszName))) {
            return GetParamInfoByIndex(i, ppParamInfo);
        }
    }

    return E_DOES_NOT_EXIST;
}

ECode CMethodInfo::SetParamElem(
    /* [in] */ ParamDescriptor *pParamDescriptor,
    /* [in] */ ParmElement *pParmElement)
{
    TypeDescriptor *pTypeDesc = &pParamDescriptor->type;
    pParmElement->pointer = pTypeDesc->nPointer;

    ECode ec = NOERROR;
    if (pTypeDesc->type == Type_alias) {
        ec = m_pCClsModule->AliasToOriginal(pTypeDesc, &pTypeDesc);
        if (FAILED(ec)) {
            return ec;
        }
        pParmElement->pointer += pTypeDesc->nPointer;
    }

    CarDataType type = GetCarDataType(pTypeDesc->type);
    //Set type and IO attrib
    if (pParamDescriptor->dwAttribs & ParamAttrib_in) {
        if (type == CarDataType_Interface) {
            if (pParmElement->pointer > 1) {
                type = CarDataType_LocalPtr;
            }
        }
        else if (pParmElement->pointer > 0) {
            type = CarDataType_LocalPtr;
        }
        pParmElement->attrib = ParamIOAttribute_In;
    }
    else {
        pParmElement->attrib = ParamIOAttribute_CallerAllocOut;

        if (pParmElement->pointer > 2) {
            type = CarDataType_LocalPtr;
            pParmElement->pointer -= 1;
        }
        else if (pParmElement->pointer == 2 && type != CarDataType_Interface) {
            if (type == CarDataType_String) {
                pParmElement->attrib = ParamIOAttribute_CalleeAllocOut;
            }
            else {
                type = CarDataType_LocalPtr;
                pParmElement->pointer = 1;
            }
        }
        else if (pParamDescriptor->dwAttribs & ParamAttrib_callee) {
            pParmElement->attrib = ParamIOAttribute_CalleeAllocOut;
            pParmElement->pointer = 2;
        }
    }
    pParmElement->type = type;

    //Set size
    if (pTypeDesc->type == Type_struct
        || pTypeDesc->type == Type_EMuid
        || pTypeDesc->type == Type_EGuid
        || pTypeDesc->type == Type_ArrayOf
        || pTypeDesc->type == Type_Array) {
        pParmElement->size = sizeof(PVoid);
    }
    else {
        pParmElement->size = GetDataTypeSize(m_pCClsModule, pTypeDesc);
    }

#if defined(_arm) && defined(__GNUC__) && (__GNUC__ >= 4)
    if ((pTypeDesc->type == Type_Double || pTypeDesc->type == Type_Int64)
        && pParmElement->attrib != ParamIOAttribute_CallerAllocOut) {
        m_uParamBufSize = ROUND8(m_uParamBufSize);
    }
#endif

    pParmElement->pos = m_uParamBufSize;
    m_uParamBufSize += ROUND4(pParmElement->size);

    return NOERROR;
}

ECode CMethodInfo::InitParmElement()
{
    Int32 count = m_pMethodDescriptor->cParams;
    m_pParamElem = new ParmElement[count];
    if (!m_pParamElem) {
        return E_OUT_OF_MEMORY;
    }
    memset(m_pParamElem, 0, sizeof(m_pParamElem) * count);

    m_uParamBufSize = 4; //For this pointer
    ECode ec = NOERROR;
    Int32 i = 0;
    for (i = 0; i < count; i++) {
        ec = SetParamElem(
                getParamDescAddr(m_nBase, m_pMethodDescriptor->ppParams, i),
                &m_pParamElem[i]);
        if (FAILED(ec)) goto EExit;
    }

    return NOERROR;

EExit:
    delete [] m_pParamElem;
    m_pParamElem = NULL;

    return ec;
}

ECode CMethodInfo::InitParamInfos()
{
    Int32 i = 0, count = m_pMethodDescriptor->cParams;
    m_pParameterInfos = ArrayOf<IParamInfo *>::Alloc(count);
    if (m_pParameterInfos == NULL) {
        return E_OUT_OF_MEMORY;
    }

    for (i = 0; i < count; i++) {
        // do not use m_pParameterInfos->Set(i, xxx), because it will
        // call xxx->AddRef()
        (*m_pParameterInfos)[i] = new CParamInfo(m_pCClsModule, this,
                &m_pParamElem[i],
                getParamDescAddr(m_nBase, m_pMethodDescriptor->ppParams, i),
                i);
        if (!(*m_pParameterInfos)[i]) goto EExit;
    }

    return NOERROR;

EExit:
    for (i = 0; i < count; i++) {
        if ((*m_pParameterInfos)[i]) {
            delete (*m_pParameterInfos)[i];
            (*m_pParameterInfos)[i] = NULL;
        }
    }
    ArrayOf<IParamInfo *>::Free(m_pParameterInfos);
    m_pParameterInfos = NULL;

    return NOERROR;
}

ECode CMethodInfo::CreateFunctionArgumentList(
        /* [in] */ IFunctionInfo *pFunctionInfo,
        /* [in] */ Boolean bMethodInfo,
        /* [out] */ IArgumentList ** ppArgumentList)
{
    if (!ppArgumentList) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pMethodDescriptor->cParams) {
        return E_INVALID_OPERATION;
    }

    AutoPtr<CArgumentList> pArgumentList = new CArgumentList();
    if (pArgumentList == NULL) {
        return E_OUT_OF_MEMORY;
    }

#if defined(_arm) && defined(__GNUC__) && (__GNUC__ >= 4)
    m_uParamBufSize = ROUND8(m_uParamBufSize);
#endif

    ECode ec = pArgumentList->Init(pFunctionInfo, m_pParamElem,
                    m_pMethodDescriptor->cParams, m_uParamBufSize, bMethodInfo);
    if FAILED(ec) {
        return ec;
    }

    *ppArgumentList = pArgumentList;
    (*ppArgumentList)->AddRef();

    return NOERROR;
}

ECode CMethodInfo::CreateCBArgumentList(
    /* [in] */ ICallbackMethodInfo *pCallbackMethodInfo,
    /* [out] */ ICallbackArgumentList ** ppCBArgumentList)
{
    AutoPtr<CCallbackArgumentList> pCBArgumentList = new CCallbackArgumentList();
    if (pCBArgumentList == NULL) {
        return E_OUT_OF_MEMORY;
    }

/*
The GCC4 is 64-bit types (like long long) alignment and m_pHandlerThis(4-Byte)
 is picked out when creating ParamBuf, so we must recompute the position.
*/
#if defined(_arm) && defined(__GNUC__) && (__GNUC__ >= 4)
    int position = 4;
    for (int i = 0; i < m_pMethodDescriptor->cParams; i++) {
        if (m_pParamElem[i].type == CarDataType_Double ||
                m_pParamElem[i].type == CarDataType_Int64) {
            m_pParamElem[i].pos = ROUND8(position + 4) - 4;
        }
        else {
            position = ROUND4(position);
            m_pParamElem[i].pos = position;
        }

        position += m_pParamElem[i].size;
    }
#endif

    ECode ec = pCBArgumentList->Init(pCallbackMethodInfo, m_pParamElem,
                        m_pMethodDescriptor->cParams, m_uParamBufSize);
    if FAILED(ec) {
        return ec;
    }

    *ppCBArgumentList = pCBArgumentList;
    (*ppCBArgumentList)->AddRef();

    return NOERROR;
}

ECode CMethodInfo::CreateArgumentList(
    /* [out] */ IArgumentList ** ppArgumentList)
{
    return CreateFunctionArgumentList(this, TRUE, ppArgumentList);
}

ECode CMethodInfo::Invoke(
    /* [in] */ PInterface target,
    /* [in] */ IArgumentList * pArgumentList)
{
    if (!target) {
        return E_INVALID_ARGUMENT;
    }

    int *pParamBuf = NULL;
    if (!pArgumentList) {
        if (m_pMethodDescriptor->cParams) return E_INVALID_ARGUMENT;
        pParamBuf = (int *)alloca(4);
        if (!pParamBuf) return E_OUT_OF_MEMORY;
        m_uParamBufSize = 4;
    }
    else {
        pParamBuf = (int *)((CArgumentList *)pArgumentList)->mParamBuf;
    }

    InterfaceDirEntry* pIFDir = getInterfaceDirAddr(m_nBase,
            m_pClsMod->ppInterfaceDir, INTERFACE_INDEX(mIndex));
    EIID iid = adjustInterfaceDescAddr(m_nBase, pIFDir->pDesc)->iid;
    PInterface pVObject;
    pVObject = target->Probe(iid);
    if (!pVObject) {
        return E_NO_INTERFACE;
    }

    //push this pointer to buf
    *(PInterface *)pParamBuf = pVObject;

    VObject* vobj = reinterpret_cast<VObject*>(pVObject);
    void* pMethodAddr = vobj->vtab->methods[METHOD_INDEX(mIndex)];
    return (ECode)invoke(pMethodAddr, pParamBuf,  m_uParamBufSize);
}
