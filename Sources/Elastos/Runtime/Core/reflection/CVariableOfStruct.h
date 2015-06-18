//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CSTRUCTVARIABLE_H__
#define __CSTRUCTVARIABLE_H__

#include "refutil.h"

class CVariableOfStruct
    : public ElLightRefBase
    , public IVariableOfStruct
    , public IStructSetter
    , public IStructGetter
{
public:
    CVariableOfStruct();

    virtual ~CVariableOfStruct();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Init(
        /* [in] */ IStructInfo *pStructInfo,
        /* [in] */ PVoid pVarBuf);

    CARAPI GetTypeInfo(
        /* [out] */ IDataTypeInfo ** ppTypeInfo);

    CARAPI GetPayload(
        /* [out] */ PVoid * pPayload);

    CARAPI Rebox(
        /* [in] */ PVoid localVariablePtr);

    CARAPI GetSetter(
        /* [out] */ IStructSetter ** ppSetter);

    CARAPI GetGetter(
        /* [out] */ IStructGetter ** ppGetter);

//--------------Setter----------------------------------------------------------

    CARAPI ZeroAllFields();

    CARAPI SetInt16Field(
        /* [in] */ const String& name,
        /* [in] */ Int16 value);

    CARAPI SetInt32Field(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI SetInt64Field(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    CARAPI SetByteField(
        /* [in] */ const String& name,
        /* [in] */ Byte value);

    CARAPI SetFloatField(
        /* [in] */ const String& name,
        /* [in] */ Float value);

    CARAPI SetDoubleField(
        /* [in] */ const String& name,
        /* [in] */ Double value);

    CARAPI SetChar16Field(
        /* [in] */ const String& name,
        /* [in] */ Char16 value);

    CARAPI SetChar32Field(
        /* [in] */ const String& name,
        /* [in] */ Char32 value);

    CARAPI SetBooleanField(
        /* [in] */ const String& name,
        /* [in] */ Boolean value);

    CARAPI SetEMuidField(
        /* [in] */ const String& name,
        /* [in] */ EMuid * pValue);

    CARAPI SetEGuidField(
        /* [in] */ const String& name,
        /* [in] */ EGuid * pValue);

    CARAPI SetECodeField(
        /* [in] */ const String& name,
        /* [in] */ ECode value);

    CARAPI SetLocalPtrField(
        /* [in] */ const String& name,
        /* [in] */ LocalPtr value);

    CARAPI SetLocalTypeField(
        /* [in] */ const String& name,
        /* [in] */ PVoid value);

    CARAPI SetEnumField(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    CARAPI GetStructFieldSetter(
        /* [in] */ const String& name,
        /* [out] */ IStructSetter ** ppSetter);

    CARAPI GetCppVectorFieldSetter(
        /* [in] */ const String& name,
        /* [out] */ ICppVectorSetter ** ppSetter);

    CARAPI SetFieldValueByName(
        /* [in] */ const String& name,
        /* [in] */ void *pParam,
        /* [in] */ CarDataType type);

//--------------Getter----------------------------------------------------------

    CARAPI GetInt16Field(
        /* [in] */ const String& name,
        /* [out] */ Int16 * pValue);

    CARAPI GetInt32Field(
        /* [in] */ const String& name,
        /* [out] */ Int32 * pValue);

    CARAPI GetInt64Field(
        /* [in] */ const String& name,
        /* [out] */ Int64 * pValue);

    CARAPI GetByteField(
        /* [in] */ const String& name,
        /* [out] */ Byte * pValue);

    CARAPI GetFloatField(
        /* [in] */ const String& name,
        /* [out] */ Float * pValue);

    CARAPI GetDoubleField(
        /* [in] */ const String& name,
        /* [out] */ Double * pValue);

    CARAPI GetChar16Field(
        /* [in] */ const String& name,
        /* [out] */ Char16 * pValue);

    CARAPI GetChar32Field(
        /* [in] */ const String& name,
        /* [out] */ Char32 * pValue);

    CARAPI GetBooleanField(
        /* [in] */ const String& name,
        /* [out] */ Boolean * pValue);

    CARAPI GetEMuidField(
        /* [in] */ const String& name,
        /* [out] */ EMuid * pValue);

    CARAPI GetEGuidField(
        /* [in] */ const String& name,
        /* [out] */ EGuid * pValue);

    CARAPI GetECodeField(
        /* [in] */ const String& name,
        /* [out] */ ECode * pValue);

    CARAPI GetLocalPtrField(
        /* [in] */ const String& name,
        /* [out] */ LocalPtr * pValue);

    CARAPI GetLocalTypeField(
        /* [in] */ const String& name,
        /* [out] */ PVoid value);

    CARAPI GetEnumField(
        /* [in] */ const String& name,
        /* [out] */ Int32 * pValue);

    CARAPI GetStructFieldGetter(
        /* [in] */ const String& name,
        /* [out] */ IStructGetter ** ppGetter);

    CARAPI GetCppVectorFieldGetter(
        /* [in] */ const String& name,
        /* [out] */ ICppVectorGetter ** ppGetter);

    CARAPI GetFieldValueByName(
        /* [in] */ const String& name,
        /* [in] */ void *pParam,
        /* [in] */ CarDataType type);

    CARAPI GetIndexByName(
        /* [in] */ const String& name,
        /* [out] */ Int32 *pIndex);

    CARAPI AcquireCppVectorFieldSGetter(
        /* [in] */ const String& name,
        /* [in] */ Boolean bSetter,
        /* [out] */ IInterface ** ppSGetter);

private:
    AutoPtr<IStructInfo>  m_pStructInfo;
    StructFieldDesc      *m_pStructFieldDesc;
    PByte                 m_pVarBuf;
    Boolean               m_bAlloc;
    Int32                 m_iCount;
    UInt32                m_uVarSize;

    IInterface          **m_pCppVectorSGetters;
};

#endif // __CSTRUCTVARIABLE_H__
