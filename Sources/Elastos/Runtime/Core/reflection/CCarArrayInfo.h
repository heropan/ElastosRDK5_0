//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCARARRAYINFO_H__
#define __CCARARRAYINFO_H__

#include "CClsModule.h"

class CCarArrayInfo
    : public ElLightRefBase
    , public ICarArrayInfo
{
public:
    CCarArrayInfo(
        /* [in] */ CarDataType quintetType,
        /* [in] */ IDataTypeInfo *pElementTypeInfo,
        /* [in] */ CarDataType dataType = CarDataType_Struct);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetSize(
        /* [out] */ MemorySize * pSize);

    CARAPI GetDataType(
        /* [out] */ CarDataType * pDataType);

    CARAPI GetElementTypeInfo(
        /* [out] */ IDataTypeInfo ** ppElementTypeInfo);

    CARAPI CreateVariable(
        /* [in] */ Int32 capacity,
        /* [out] */ IVariableOfCarArray ** ppVariable);

    CARAPI CreateVariableBox(
        /* [in] */ PCarQuintet variableDescriptor,
        /* [out] */ IVariableOfCarArray ** ppVariable);

    CARAPI GetMaxAlignSize(
        /* [out] */ MemorySize * pAlignSize);

private:
    CarQuintetFlag         m_carQuintetFlag;

    AutoPtr<IDataTypeInfo> m_pElementTypeInfo;
    CarDataType            m_elementDataType;
    CarDataType            m_quintetType;
};

#endif // __CCARARRAYINFO_H__
