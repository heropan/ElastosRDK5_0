//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CSTRUCTINFO_H__
#define __CSTRUCTINFO_H__

#include "CEntryList.h"

class CStructInfo
    : public ElLightRefBase
    , public IStructInfo
{
public:
    CStructInfo();

    virtual ~CStructInfo();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI InitStatic(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ StructDirEntry *pStructDirEntry);

    CARAPI InitDynamic(
        /* [in] */ const String& name,
        /* [in] */ ArrayOf<String>* fieldNames,
        /* [in] */ ArrayOf<IDataTypeInfo*>* fieldTypeInfos);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetSize(
        /* [out] */ MemorySize * pSize);

    CARAPI GetDataType(
        /* [out] */ CarDataType * pDataType);

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

    CARAPI GetFieldCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllFieldInfos(
        /* [out] */ ArrayOf<IFieldInfo *> * pFieldInfos);

    CARAPI GetFieldInfo(
        /* [in] */ const String& name,
        /* [out] */ IFieldInfo ** ppFieldInfo);

    CARAPI CreateVariable(
        /* [out] */ IVariableOfStruct ** ppVariable);

    CARAPI CreateVariableBox(
        /* [in] */ PVoid variableDescriptor,
        /* [out] */ IVariableOfStruct ** ppVariable);

    CARAPI InitFieldInfos();

    CARAPI InitFieldElement();

    CARAPI CreateVarBox(
        /* [in] */ PVoid pVarPtr,
        /* [out] */ IVariableOfStruct ** ppVariable);

    CARAPI GetMaxAlignSize(
        /* [out] */ MemorySize * pAlignSize);

public:
    AutoPtr< ArrayOf<String> >         m_pFieldNames;
    AutoPtr< ArrayOf<IDataTypeInfo*> > m_pFieldTypeInfos;
    StructFieldDesc                   *m_pStructFieldDesc;
    UInt32                             m_uSize;

private:
    AutoPtr<CClsModule>                m_pCClsModule;
    StructDirEntry                    *m_pStructDirEntry;

    ArrayOf<IFieldInfo *>             *m_pFieldInfos;
    String                             m_pName;
};

#endif // __CSTRUCTINFO_H__
