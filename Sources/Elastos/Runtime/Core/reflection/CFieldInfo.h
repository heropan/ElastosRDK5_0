//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CFIELDINFO_H__
#define __CFIELDINFO_H__

#include "refutil.h"

class CFieldInfo : public IFieldInfo
{
public:
    CFieldInfo(
        /* [in] */ IStructInfo * pStructInfo,
        /* [in] */ const String& name,
        /* [in] */ IDataTypeInfo *pTypeInfo);

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

private:
    IStructInfo    *m_pStructInfo;
    String          m_sName;
    IDataTypeInfo  *m_pTypeInfo;
};

#endif // __CFIELDINFO_H__
