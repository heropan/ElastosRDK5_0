//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CENUMINFO_H__
#define __CENUMINFO_H__

#include "CEntryList.h"

class CEnumInfo
    : public ElLightRefBase
    , public IEnumInfo
{
public:
    CEnumInfo();

    virtual ~CEnumInfo();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI InitStatic(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ EnumDirEntry *pEnumDirEntry);

    CARAPI InitDynamic(
        /* [in] */ const String& fullName,
        /* [in] */ ArrayOf<String>* itemNames,
        /* [in] */ ArrayOf<Int32>* itemValues);

    CARAPI GetSize(
        /* [out] */ MemorySize * pSize);

    CARAPI GetDataType(
        /* [out] */ CarDataType * pDataType);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetNamespace(
        /* [out] */ String * pNamespace);

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

    CARAPI GetItemCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllItemInfos(
        /* [out] */ ArrayOf<IEnumItemInfo *> * pItemInfos);

    CARAPI GetItemInfo(
        /* [in] */ const String& name,
        /* [out] */ IEnumItemInfo ** ppEnumItemInfo);

    CARAPI InitItemInfos();

public:
    AutoPtr< ArrayOf<String> > m_pItemNames;
    AutoPtr< ArrayOf<Int32> >  m_pItemValues;

private:
    AutoPtr<CClsModule>        m_pCClsModule;
    EnumDirEntry              *m_pEnumDirEntry;
    ArrayOf<IEnumItemInfo *>  *m_pItemInfos;

    String                     m_pName;
    String                     m_pNamespace;
};

#endif // __CENUMINFO_H__
