//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CEntryList.h"

#include "CConstantInfo.h"
#include "CConstructorInfo.h"
#include "CEnumItemInfo.h"
#include "CParamInfo.h"
#include "CFieldInfo.h"
#include "CClassInfo.h"

CEntryList::CEntryList(
    /* [in] */ EntryType type,
    /* [in] */ void* desc,
    /* [in] */ UInt32 totalCount,
    /* [in] */ CClsModule* clsModule,
    /* [in] */ IFIndexEntry* ifList,
    /* [in] */ UInt32 listCount,
    /* [in] */ CClassInfo* clsInfo)
{
    mClsModule = clsModule;
    mClsMod = mClsModule->mClsMod;

    mType = type;
    mDesc = desc;
    mTotalCount = totalCount;

    mObjElement = NULL;
    mIFList = ifList;
    mListCount = listCount;
    mClsInfo = clsInfo;

    mBase = mClsModule->mBase;
}

CEntryList::~CEntryList()
{
    if (mObjElement) {
        for (UInt32 i = 0; i < mTotalCount; i++) {
            if (mObjElement[i].mObject) {
                mObjElement[i].mObject->Release();
            }
        }
        delete[] mObjElement;
    }
}

UInt32 CEntryList::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CEntryList::Release()
{
    return ElLightRefBase::Release();
}

ECode CEntryList::InitElemList()
{
    if (mTotalCount == 0) {
        return E_DOES_NOT_EXIST;
    }

    if (mObjElement) {
        return NOERROR;
    }

    mObjElement = new ObjElement[mTotalCount];
    if (mObjElement == NULL) {
        return E_OUT_OF_MEMORY;
    }
    memset(mObjElement, 0, sizeof(ObjElement) * mTotalCount);

    UInt32 index = 0, i = 0, j = 0, n = 0;
    if (mType == EntryType_TypeAliase) {
        AliasDirEntry* aliasDir = NULL;
        mObjElement[i].mIndex = 0;
        for (i = 0; i < mClsMod->cAliases; i++) {
            if (!IsSysAlaisType(mClsModule, i)) {
                aliasDir = getAliasDirAddr(mBase, mClsMod->ppAliasDir, i);
                mObjElement[j].mIndex = i;
                mObjElement[j].mName = adjustNameAddr(mBase, aliasDir->pszName);
                if (!mHTIndexs.Put(mObjElement[j].mName, j)) {
                    return E_OUT_OF_MEMORY;
                }
                j++;
            }
        }
        return NOERROR;
    }
    else if (mType == EntryType_ClassInterface) {
        for (i = 0; i < mListCount; i++) {
            mObjElement[i].mIndex = mIFList[i].mIndex;
            mObjElement[i].mObject = NULL;
            mObjElement[i].mDesc = NULL;
            mObjElement[i].mName = mIFList[i].mName;
            mObjElement[i].mNamespaceOrSignature = mIFList[i].mNameSpace;
            String strKey;
            if (mObjElement[i].mNamespaceOrSignature != NULL &&
                mObjElement[i].mNamespaceOrSignature[0] != '\0') {
                strKey = String(mObjElement[i].mNamespaceOrSignature) + String(".") + String(mObjElement[i].mName);
            }
            else {
                strKey = mObjElement[i].mName;
            }
            if (!mHTIndexs.Put(const_cast<char*>(strKey.string()), i)) {
                return E_OUT_OF_MEMORY;
            }
        }
        return NOERROR;
    }
    else if (mType == EntryType_Method || mType == EntryType_Constructor
            || mType == EntryType_CBMethod) {

        n = 0;
        for (i = 0; i < mListCount; i++) {
            for (j = 0; j < mIFList[i].mDesc->cMethods; j++, n++) {
                if (n == mTotalCount) return E_INVALID_ARGUMENT;

                mObjElement[n].mIndex = MK_METHOD_INDEX(mIFList[i].mIndex,
                        mIFList[i].mBeginNo + j);
                mObjElement[n].mObject = NULL;
                mObjElement[n].mDesc = getMethodDescAddr(mBase,
                        mIFList[i].mDesc->ppMethods, j);
                mObjElement[n].mName = adjustNameAddr(mBase,
                        ((MethodDescriptor *)mObjElement[n].mDesc)->pszName);
                mObjElement[n].mNamespaceOrSignature = adjustNameAddr(mBase,
                        ((MethodDescriptor *)mObjElement[n].mDesc)->pszSignature);
                String strKey = String(mObjElement[n].mName) + String(mObjElement[n].mNamespaceOrSignature);
                if (!mHTIndexs.Put(const_cast<char*>(strKey.string()), n)) {
                    return E_OUT_OF_MEMORY;
                }
            }
        }
        return NOERROR;
    }

    ClassDirEntry*      classDir = NULL;
    InterfaceDirEntry*  ifDir = NULL;
    StructDirEntry*     structDir = NULL;
    EnumDirEntry*       enumDir = NULL;
    ConstDirEntry*      constDir = NULL;
    //No EntryType_TypeAliase
    for (i = 0; i < mTotalCount; i++) {
        mObjElement[i].mIndex = i;
        mObjElement[i].mObject = NULL;
        mObjElement[i].mDesc = NULL;

        switch (mType) {
            case EntryType_Aspect:
                index = adjustIndexsAddr(mBase,
                        ((ClassDescriptor *)mDesc)->pAspectIndexs)[i];
                classDir = getClassDirAddr(mBase, mClsMod->ppClassDir, index);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        classDir->pszName);
                mObjElement[i].mNamespaceOrSignature = NULL;
                break;
            case EntryType_Aggregatee:
                index = adjustIndexsAddr(mBase,
                        ((ClassDescriptor *)mDesc)->pAggrIndexs)[i];
                classDir = getClassDirAddr(mBase, mClsMod->ppClassDir, index);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        classDir->pszName);
                mObjElement[i].mNamespaceOrSignature = NULL;
                break;
            case EntryType_Class:
                classDir = getClassDirAddr(mBase, mClsMod->ppClassDir, i);
                mObjElement[i].mName  = adjustNameAddr(mBase,
                        classDir->pszName);
                mObjElement[i].mNamespaceOrSignature = adjustNameAddr(mBase,
                        classDir->pszNameSpace);
                break;
            case EntryType_ClassInterface:
                index = mObjElement[i].mIndex;
                ifDir = getInterfaceDirAddr(mBase,
                        mClsMod->ppInterfaceDir, index);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        ifDir->pszName);
                mObjElement[i].mNamespaceOrSignature = adjustNameAddr(mBase,
                        ifDir->pszNameSpace);
                break;
            case EntryType_Interface:
                ifDir = getInterfaceDirAddr(mBase, mClsMod->ppInterfaceDir, i);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        ifDir->pszName);
                mObjElement[i].mNamespaceOrSignature = adjustNameAddr(mBase,
                        ifDir->pszNameSpace);
                break;
            case EntryType_Struct:
                structDir = getStructDirAddr(mBase, mClsMod->ppStructDir, i);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        structDir->pszName);
                mObjElement[i].mNamespaceOrSignature = NULL;
                break;
            case EntryType_Enum:
                enumDir = getEnumDirAddr(mBase, mClsMod->ppEnumDir, i);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        enumDir->pszName);
                mObjElement[i].mNamespaceOrSignature = adjustNameAddr(mBase,
                        enumDir->pszNameSpace);
                break;
            case EntryType_Constant:
                constDir = getConstDirAddr(mBase, mClsMod->ppConstDir, i);
                mObjElement[i].mName = adjustNameAddr(mBase,
                        constDir->pszName);
                mObjElement[i].mNamespaceOrSignature = NULL;
                break;
            default:
                return E_INVALID_OPERATION;
        }

        String strKey;
        if (mObjElement[i].mNamespaceOrSignature != NULL) {
            strKey = String(mObjElement[i].mNamespaceOrSignature) + String(".") + String(mObjElement[i].mName);
        }
        else {
            strKey = mObjElement[i].mName;
        }
        if (!mHTIndexs.Put(const_cast<char*>(strKey.string()), i)) {
            return E_OUT_OF_MEMORY;
        }
    }
    return NOERROR;
}

ECode CEntryList::AcquireObjByName(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    if (!object || name.IsNull()) {
        return E_INVALID_ARGUMENT;
    }

    g_objInfoList.LockHashTable(mType);
    ECode ec = InitElemList();
    g_objInfoList.UnlockHashTable(mType);
    if (FAILED(ec)) {
        return ec;
    }

    UInt32* index = mHTIndexs.Get((PVoid)name.string());
    if (index == NULL) {
        return E_DOES_NOT_EXIST;
    }
    else {
        return AcquireObjByIndex(*index, object);
    }
}

ECode CEntryList::AcquireObjByIndex(
    /* [in] */ UInt32 index,
    /* [out] */ IInterface ** object)
{
    if (!object) {
        return E_INVALID_ARGUMENT;
    }

    if (!mTotalCount || (index & 0x0000FFFF) >= mTotalCount) {
        return E_DOES_NOT_EXIST;
    }

    g_objInfoList.LockHashTable(mType);
    ECode ec = InitElemList();
    g_objInfoList.UnlockHashTable(mType);
    if (FAILED(ec)) {
        return ec;
    }

    UInt32 i = 0;
    if ((mType == EntryType_Method || mType == EntryType_Constructor
            || mType == EntryType_CBMethod) && (index & 0xFFFF0000)) {
        //Method's Index
        //Change to the array's index
        for (i = 0; i < mTotalCount; i++) {
            if (mObjElement[i].mIndex == index) {
                index = i;
                break;
            }
        }
        if (index != i) return E_INVALID_ARGUMENT;
    }

    UInt32 sIndex = 0;

    switch (mType) {
        case EntryType_Aspect:
            sIndex = adjustIndexsAddr(mBase,
                    ((ClassDescriptor *)mDesc)->pAspectIndexs)[index];
            ec = g_objInfoList.AcquireClassInfo(mClsModule,
                    getClassDirAddr(mBase, mClsMod->ppClassDir, sIndex),
                    &mObjElement[index].mObject);
            break;

        case EntryType_Aggregatee:
            sIndex = adjustIndexsAddr(mBase,
                    ((ClassDescriptor *)mDesc)->pAggrIndexs)[index];
            ec = g_objInfoList.AcquireClassInfo(mClsModule,
                    getClassDirAddr(mBase, mClsMod->ppClassDir, sIndex),
                    &mObjElement[index].mObject);
            break;

        case EntryType_Class:
            ec = g_objInfoList.AcquireClassInfo(mClsModule,
                    getClassDirAddr(mBase, mClsMod->ppClassDir, index),
                    &mObjElement[index].mObject);
            break;

        case EntryType_ClassInterface:
            sIndex = mObjElement[index].mIndex;
            ec = g_objInfoList.AcquireInterfaceInfo(mClsModule, sIndex,
                    &mObjElement[index].mObject);
            break;

        case EntryType_Interface:
            ec = g_objInfoList.AcquireInterfaceInfo(mClsModule, index,
                    &mObjElement[index].mObject);
            break;

        case EntryType_Struct:
            ec = g_objInfoList.AcquireStaticStructInfo(mClsModule,
                    getStructDirAddr(mBase, mClsMod->ppStructDir, index),
                    &mObjElement[index].mObject);
            break;

        case EntryType_Enum:
            ec = g_objInfoList.AcquireStaticEnumInfo(mClsModule,
                    getEnumDirAddr(mBase, mClsMod->ppEnumDir, index),
                    &mObjElement[index].mObject);
            break;

        case EntryType_TypeAliase:
            sIndex = mObjElement[index].mIndex;
            ec = g_objInfoList.AcquireTypeAliasInfo(mClsModule,
                    getAliasDirAddr(mBase, mClsMod->ppAliasDir, sIndex),
                    &mObjElement[index].mObject);
            break;

        case EntryType_Constant:
            ec = g_objInfoList.AcquireConstantInfo(mClsModule,
                    getConstDirAddr(mBase, mClsMod->ppConstDir, index),
                    &mObjElement[index].mObject);
            break;

        case EntryType_Method:
            ec = g_objInfoList.AcquireMethodInfo(mClsModule,
                    (MethodDescriptor *)mObjElement[index].mDesc,
                    mObjElement[index].mIndex,
                    &mObjElement[index].mObject);
            break;

        case EntryType_Constructor:
            ec = g_objInfoList.AcquireConstructorInfo(mClsModule,
                    (MethodDescriptor *)mObjElement[index].mDesc,
                    mObjElement[index].mIndex,
                    &mClsInfo->mClsId,
                    &mObjElement[index].mObject);
            break;

        case EntryType_CBMethod:
            ec = g_objInfoList.AcquireCBMethodInfoInfo(mClsModule,
                    mClsInfo->mCBMethodDesc[index].mEventNum,
                    mClsInfo->mCBMethodDesc[index].mDesc,
                    mClsInfo->mCBMethodDesc[index].mIndex,
                    &mObjElement[index].mObject);
            break;

        default:
            ec = E_INVALID_OPERATION;
    }

    if (FAILED(ec)) {
        return ec;
    }

    *object = mObjElement[index].mObject;
    (*object)->AddRef();
    return NOERROR;
}

ECode CEntryList::GetAllObjInfos(
    /* [out] */ ArrayOf<IInterface *>* objInfos)
{
    if (!objInfos) {
        return E_INVALID_ARGUMENT;
    }

    Int32 capacity = objInfos->GetLength();
    if (capacity == 0) {
        return E_INVALID_ARGUMENT;
    }

    if (!mTotalCount) {
        return NOERROR;
    }

    Int32 count = capacity < (int)mTotalCount ? capacity : mTotalCount;
    ECode ec = NOERROR;
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IInterface> object;
        ec = AcquireObjByIndex(i, (IInterface**)&object);
        if (FAILED(ec)) return ec;
        objInfos->Set(i, object);
    }

    return NOERROR;
}
