//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CEnumInfo.h"
#include "CEnumItemInfo.h"

CEnumInfo::CEnumInfo()
    : m_pEnumDirEntry(NULL)
    , m_pItemInfos(NULL)
{}

CEnumInfo::~CEnumInfo()
{
    if (m_pItemInfos) {
        for (Int32 i = 0; i < m_pItemInfos->GetLength(); i++) {
            if ((*m_pItemInfos)[i]) {
                delete (*m_pItemInfos)[i];
                (*m_pItemInfos)[i] = NULL;
            }
        }
        ArrayOf<IEnumItemInfo *>::Free(m_pItemInfos);
    }
}

UInt32 CEnumInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CEnumInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_Enum);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        if (!m_pCClsModule) {
            g_objInfoList.DetachEnumInfo(this);
        }
        else {
            g_objInfoList.RemoveEnumInfo(m_pEnumDirEntry);
        }
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_Enum);
    assert(nRef >= 0);
    return nRef;
}

PInterface CEnumInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IEnumInfo) {
        return (IEnumInfo *)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CEnumInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnumInfo::InitStatic(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ EnumDirEntry *pEnumDirEntry)
{
    m_pCClsModule = pCClsModule;

    Int32 nBase = m_pCClsModule->mBase;
    m_pEnumDirEntry = pEnumDirEntry;
    m_pName = adjustNameAddr(nBase, m_pEnumDirEntry->pszName);
    m_pNamespace = adjustNameAddr(nBase, m_pEnumDirEntry->pszNameSpace);
    EnumDescriptor* pDesc = adjustEnumDescAddr(nBase, pEnumDirEntry->pDesc);

    m_pItemValues = ArrayOf<Int32>::Alloc(pDesc->cElems);
    if (!m_pItemValues) {
        return E_OUT_OF_MEMORY;
    }

    m_pItemNames = ArrayOf<String>::Alloc(pDesc->cElems);
    if (!m_pItemNames) {
        m_pItemValues = NULL;
        return E_OUT_OF_MEMORY;
    }

    EnumElement* pElem = NULL;
    for (Int32 i = 0; i < pDesc->cElems; i++) {
        pElem = getEnumElementAddr(nBase, pDesc->ppElems, i);
        (*m_pItemValues)[i] = pElem->nValue;
        (*m_pItemNames)[i] = adjustNameAddr(nBase, pElem->pszName);
    }

    ECode ec = InitItemInfos();
    if (FAILED(ec)) {
        m_pItemNames = NULL;
        m_pItemValues = NULL;
    }

    return NOERROR;
}

ECode CEnumInfo::InitDynamic(
    /* [in] */ const String& fullName,
    /* [in] */ ArrayOf<String>* itemNames,
    /* [in] */ ArrayOf<Int32>* itemValues)
{
    if (itemNames == NULL || itemValues == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 index = fullName.LastIndexOf('.');
    m_pName = index > 0 ? fullName.Substring(index + 1) : fullName;
    m_pNamespace = index > 0 ? fullName.Substring(0, index - 1) : String("");
    m_pItemNames = itemNames;
    m_pItemValues = itemValues;

    ECode ec = InitItemInfos();
    if (FAILED(ec)) {
        m_pItemNames = NULL;
        m_pItemValues = NULL;
        return ec;
    }

    return NOERROR;
}

ECode CEnumInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = sizeof(Int32);
    return NOERROR;
}

ECode CEnumInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = CarDataType_Enum;
    return NOERROR;
}

ECode CEnumInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = m_pName;
    return NOERROR;
}

ECode CEnumInfo::GetNamespace(
    /* [out] */ String* pNamespace)
{
    if (pNamespace == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pNamespace = m_pNamespace;
    return NOERROR;
}

ECode CEnumInfo::GetModuleInfo(
    /* [out] */ IModuleInfo ** ppModuleInfo)
{
    if (m_pCClsModule) {
        return m_pCClsModule->GetModuleInfo(ppModuleInfo);
    }
    else {
        return E_INVALID_OPERATION;
    }
}

ECode CEnumInfo::GetItemCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pItemValues->GetLength();
    return NOERROR;
}

ECode CEnumInfo::InitItemInfos()
{
    Int32 count = m_pItemValues->GetLength();
    m_pItemInfos = ArrayOf<IEnumItemInfo *>::Alloc(count);
    if (m_pItemInfos == NULL) {
        return E_OUT_OF_MEMORY;
    }

    for (Int32 i = 0; i < count; i++) {
        // do not use m_pItemInfos->Set(i, xxx), because it will
        // call xxx->AddRef();
        (*m_pItemInfos)[i] = new CEnumItemInfo(this,
                (*m_pItemNames)[i], (*m_pItemValues)[i]);
        if (!(*m_pItemInfos)[i]) goto EExit;
    }

    return NOERROR;

EExit:
    for (Int32 i = 0; i < count; i++) {
        if ((*m_pItemInfos)[i]) {
            delete (*m_pItemInfos)[i];
            (*m_pItemInfos)[i] = NULL;
        }
    }
    ArrayOf<IEnumItemInfo *>::Free(m_pItemInfos);
    m_pItemInfos = NULL;

    return NOERROR;
}

ECode CEnumInfo::GetAllItemInfos(
    /* [out] */ ArrayOf<IEnumItemInfo *> * pItemInfos)
{
    if (!pItemInfos) {
        return E_INVALID_ARGUMENT;
    }

    pItemInfos->Copy(m_pItemInfos);

    return NOERROR;
}

ECode CEnumInfo::GetItemInfo(
    /* [in] */ const String& name,
    /* [out] */ IEnumItemInfo ** ppEnumItemInfo)
{
    if (name.IsNull() || !ppEnumItemInfo) {
        return E_INVALID_ARGUMENT;
    }

    for (Int32 i = 0; i < m_pItemInfos->GetLength(); i++) {
        if (name.Equals((*m_pItemNames)[i].string())) {
            *ppEnumItemInfo = (IEnumItemInfo *)(*m_pItemInfos)[i];
            (*ppEnumItemInfo)->AddRef();
            return NOERROR;
        }
    }

    return E_DOES_NOT_EXIST;
}
