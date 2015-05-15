//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CStructInfo.h"
#include "CFieldInfo.h"
#include "CVariableOfStruct.h"
#include "CCarArrayInfo.h"
#include "CCppVectorInfo.h"

CStructInfo::CStructInfo()
{
    m_pStructFieldDesc = NULL;
    m_uSize = 0;
}

CStructInfo::~CStructInfo()
{
    if (m_pStructFieldDesc) delete [] m_pStructFieldDesc;

    if (m_pFieldInfos) {
        for (Int32 i = 0; i < m_pFieldInfos->GetLength(); i++) {
            if ((*m_pFieldInfos)[i]) delete (*m_pFieldInfos)[i];
        }
        ArrayOf<IFieldInfo *>::Free(m_pFieldInfos);
    }
}

UInt32 CStructInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CStructInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_Struct);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        if (m_pCClsModule == NULL) {
            g_objInfoList.DetachStructInfo(this);
        }
        else {
            g_objInfoList.RemoveStructInfo(m_pStructDirEntry);
        }
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_Struct);
    assert(nRef >= 0);
    return nRef;
}

PInterface CStructInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IStructInfo) {
        return (IStructInfo *)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CStructInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CStructInfo::InitStatic(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ StructDirEntry *pStructDirEntry)
{
    m_pCClsModule = pCClsModule;

	Int32 nBase = m_pCClsModule->m_nBase;
    m_pStructDirEntry = pStructDirEntry;
    m_pName = adjustNameAddr(nBase, m_pStructDirEntry->pszName);
	StructDescriptor *pDesc = adjustStructDescAddr(nBase, pStructDirEntry->pDesc);
    StructElement *pElem = NULL;

    ECode ec = NOERROR;
    Int32 i = 0;
    m_pFieldTypeInfos = ArrayOf<IDataTypeInfo*>::Alloc(pDesc->cElems);
    if (!m_pFieldTypeInfos) {
        ec = E_OUT_OF_MEMORY;
        goto EExit;
    }

    m_pFieldNames = ArrayOf<String>::Alloc(pDesc->cElems);
    if (!m_pFieldNames) {
        ec = E_OUT_OF_MEMORY;
        goto EExit;
    }

    for (i = 0; i < pDesc->cElems; i++) {
        pElem = getStructElementAddr(nBase, pDesc->ppElems, i);
        (*m_pFieldNames)[i] = adjustNameAddr(nBase, pElem->pszName);

        AutoPtr<IDataTypeInfo> dataTypeInfo;
        ec = g_objInfoList.AcquireDataTypeInfo(m_pCClsModule,
            &pElem->type,
            (IDataTypeInfo**)&dataTypeInfo, TRUE);
        if (FAILED(ec)) goto EExit;
        m_pFieldTypeInfos->Set(i, dataTypeInfo);
    }

    ec = InitFieldElement();
    if (FAILED(ec)) goto EExit;

    ec = InitFieldInfos();
    if (FAILED(ec)) goto EExit;

    return NOERROR;

EExit:
    m_pFieldNames = NULL;
    m_pFieldTypeInfos = NULL;

    return ec;
}

ECode CStructInfo::InitDynamic(
    /* [in] */ const String& name,
    /* [in] */ ArrayOf<String>* fieldNames,
    /* [in] */ ArrayOf<IDataTypeInfo*>* fieldTypeInfos)
{
    if (fieldNames == NULL || fieldTypeInfos == NULL) {
        return E_INVALID_ARGUMENT;
    }

    m_pName = name;
    m_pFieldNames = fieldNames;
    m_pFieldTypeInfos = fieldTypeInfos;

    ECode ec = InitFieldElement();
    if (FAILED(ec)) goto EExit;

    ec = InitFieldInfos();
    if (FAILED(ec)) goto EExit;

    return NOERROR;

EExit:
    m_pFieldNames = NULL;
    m_pFieldTypeInfos = NULL;

    return ec;
}

ECode CStructInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = m_pName;
    return NOERROR;
}

ECode CStructInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = m_uSize;
    return NOERROR;
}

ECode CStructInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = CarDataType_Struct;
    return NOERROR;
}

ECode CStructInfo::GetModuleInfo(
    /* [out] */ IModuleInfo ** ppModuleInfo)
{
    if (m_pCClsModule) {
        return m_pCClsModule->GetModuleInfo(ppModuleInfo);
    }
    else {
        return E_INVALID_OPERATION;
    }
}

ECode CStructInfo::GetFieldCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pFieldNames->GetLength();

    return NOERROR;
}

ECode CStructInfo::GetAllFieldInfos(
    /* [out] */ ArrayOf<IFieldInfo *> * pFieldInfos)
{
    if (!pFieldInfos || !pFieldInfos->GetLength()) {
        return E_INVALID_ARGUMENT;
    }

    pFieldInfos->Copy(m_pFieldInfos);

    return NOERROR;
}

ECode CStructInfo::GetFieldInfo(
    /* [in] */ CString name,
    /* [out] */ IFieldInfo ** ppFieldInfo)
{
    if (name.IsNull() || !ppFieldInfo) {
        return E_INVALID_ARGUMENT;
    }

    for (Int32 i = 0; i < m_pFieldNames->GetLength(); i++) {
        if ((*m_pFieldNames)[i].Equals(name)) {
            *ppFieldInfo = (IFieldInfo *)(*m_pFieldInfos)[i];
            (*ppFieldInfo)->AddRef();
            return NOERROR;
        }
    }

    return E_DOES_NOT_EXIST;
}

ECode CStructInfo::GetMaxAlignSize(
    /* [out] */ MemorySize * pAlignSize)
{
    Int32 size = 0, align = 1;

    CarDataType dataType;
    for (Int32 i = 0; i < m_pFieldTypeInfos->GetLength(); i++) {
        ((IDataTypeInfo *)(*m_pFieldTypeInfos)[i])->GetDataType(&dataType);
        if (dataType == CarDataType_Struct) {
            ((CStructInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&size);
        }
        else if (dataType == CarDataType_ArrayOf
                || dataType == CarDataType_BufferOf
                || dataType == CarDataType_MemoryBuf) {
            ((CCarArrayInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&size);
        }
        else if (dataType == CarDataType_CppVector) {
            ((CCppVectorInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&size);
        }
        else {
            ((IDataTypeInfo *)(*m_pFieldTypeInfos)[i])->GetSize(&size);
        }

        if (size > align) align = size;
    }

    if (align > ALIGN_BOUND) align = ALIGN_BOUND;

    *pAlignSize = align;

    return NOERROR;
}

ECode CStructInfo::InitFieldInfos()
{
    Int32 i = 0, count = m_pFieldTypeInfos->GetLength();
    m_pFieldInfos = ArrayOf<IFieldInfo *>::Alloc(count);
    if (m_pFieldInfos == NULL) {
        return E_OUT_OF_MEMORY;
    }

    for (i = 0; i < count; i++) {
        // do not use m_pFieldInfos->Set(i, xxx), because it will call
        // xxx->AddRef()
        (*m_pFieldInfos)[i] = new CFieldInfo(this, (*m_pFieldNames)[i],
            (IDataTypeInfo*)(*m_pFieldTypeInfos)[i]);
        if (!(*m_pFieldInfos)[i]) goto EExit;
    }

    return NOERROR;

EExit:
    for (i = 0; i < count; i++) {
        if ((*m_pFieldInfos)[i]) delete (*m_pFieldInfos)[i];
    }
    ArrayOf<IFieldInfo *>::Free(m_pFieldInfos);
    m_pFieldInfos = NULL;

    return NOERROR;
}

CAR_INLINE UInt32 AdjuctElemOffset(UInt32 uOffset, UInt32 uElemSize, UInt32 uAlign)
{
    if (uElemSize > uAlign) uElemSize = uAlign;
    UInt32 uMode = uOffset % uElemSize;
    if (uMode != 0) {
        uOffset = uOffset - uMode + uElemSize;
    }

    return uOffset;
}

ECode CStructInfo::InitFieldElement()
{
    Int32 count = m_pFieldTypeInfos->GetLength();
    m_pStructFieldDesc = new StructFieldDesc[count];
    if (!m_pStructFieldDesc) {
        return E_OUT_OF_MEMORY;
    }

    UInt32 uOffset = 0;
    ECode ec = NOERROR;

    CarDataType dataType;
    Int32 align, elemtAlign, elemtSize;
    ec = GetMaxAlignSize(&align);
    if (FAILED(ec)) return ec;

    for (int i = 0; i < count; i++) {
        ((IDataTypeInfo *)(*m_pFieldTypeInfos)[i])->GetDataType(&dataType);
        if (dataType == CarDataType_Struct) {
            ((CStructInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&elemtAlign);
            ((CStructInfo *)(*m_pFieldTypeInfos)[i])->GetSize(&elemtSize);
        }
        else if (dataType == CarDataType_ArrayOf
                || dataType == CarDataType_BufferOf
                || dataType == CarDataType_MemoryBuf) {
            ((CCarArrayInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&elemtAlign);
            ((CCarArrayInfo *)(*m_pFieldTypeInfos)[i])->GetSize(&elemtSize);
        }
        else if (dataType == CarDataType_CppVector) {
            ((CCppVectorInfo *)(*m_pFieldTypeInfos)[i]) \
                ->GetMaxAlignSize(&elemtAlign);
            ((CCppVectorInfo *)(*m_pFieldTypeInfos)[i])->GetSize(&elemtSize);
        }
        else {
            ((IDataTypeInfo *)(*m_pFieldTypeInfos)[i])->GetSize(&elemtSize);
            elemtAlign = align;
        }

        m_pStructFieldDesc[i].type = dataType;
        m_pStructFieldDesc[i].size = elemtSize;
        //Adjuct the Element Size
        uOffset = AdjuctElemOffset(uOffset, elemtSize, elemtAlign);

        m_pStructFieldDesc[i].pos = uOffset;
        uOffset += m_pStructFieldDesc[i].size;

        m_pStructFieldDesc[i].pszName = (char *)(*m_pFieldNames)[i].string();
    }

    m_uSize = AdjuctElemOffset(uOffset, align, align);

    return NOERROR;
}

ECode CStructInfo::CreateVarBox(
    /* [in] */ PVoid variableDescriptor,
    /* [out] */ IVariableOfStruct ** ppVariable)
{
    AutoPtr<CVariableOfStruct> pStructBox = new CVariableOfStruct();
    if (pStructBox == NULL) {
        return E_OUT_OF_MEMORY;
    }

    ECode ec = pStructBox->Init(this, variableDescriptor);
    if (FAILED(ec)) {
        return ec;
    }

    *ppVariable = (IVariableOfStruct *)pStructBox;
    (*ppVariable)->AddRef();

    return NOERROR;
}

ECode CStructInfo::CreateVariable(
    /* [out] */ IVariableOfStruct ** ppVariable)
{
    if (!ppVariable) {
        return E_INVALID_ARGUMENT;
    }

    return CreateVarBox(NULL, ppVariable);
}

ECode CStructInfo::CreateVariableBox(
    /* [in] */ PVoid variableDescriptor,
    /* [out] */ IVariableOfStruct ** ppVariable)
{
    if (!variableDescriptor || !ppVariable) {
        return E_INVALID_ARGUMENT;
    }

    return CreateVarBox(variableDescriptor, ppVariable);
}
