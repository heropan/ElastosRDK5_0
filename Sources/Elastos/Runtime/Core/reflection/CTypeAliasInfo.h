//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CTYPEALIASINFO_H__
#define __CTYPEALIASINFO_H__

#include "CClsModule.h"

class CTypeAliasInfo
    : public ElLightRefBase
    , public ITypeAliasInfo
{
public:
    CTypeAliasInfo(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ AliasDirEntry *pAliasDirEntry);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetTypeInfo(
        /* [out] */ IDataTypeInfo ** ppTypeInfo);

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

    CARAPI IsDummy(
        /* [out] */ Boolean * pIsDummy);

    CARAPI GetPtrLevel(
        /* [out] */ Int32 *pLevel);

private:
    AutoPtr<CClsModule>  m_pCClsModule;
    AliasDirEntry       *m_pAliasDirEntry;
};

#endif // __CTYPEALIASINFO_H__
