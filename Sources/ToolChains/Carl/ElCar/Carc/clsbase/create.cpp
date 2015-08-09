//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <clsbase.h>

int IsValidCLS(CLSModule *pModule, int nSize, const char *pszName)
{
    if (pModule->mCLSModuleVersion != CLSMODULE_VERSION) {
        ExtraMessage("CLSModule version is incompatable", pszName, "should be recompiled and relinked");
        _ReturnError(CLSError_CLSModuleVersion);
    }
    if (memcmp(pModule->mMagic, MAGIC_STRING, MAGIC_STRING_LENGTH)) {
        _ReturnError(CLSError_FormatMagic);
    }
    if (nSize < (int)sizeof(CLSModule) || pModule->mSize < nSize) {
        _ReturnError (CLSError_FormatSize);
    }
    _ReturnError (CLS_NoError);
}

ClassInterface *NewClassInterface(USHORT index)
{
    ClassInterface *pClassInterface;

    pClassInterface = new ClassInterface;
    if (!pClassInterface) return NULL;
    memset(pClassInterface, 0, sizeof(ClassInterface));
    pClassInterface->mIndex = index;

    return pClassInterface;
}

void DeleteClassInterface(ClassInterface *pClassInterface)
{
    assert(pClassInterface != NULL);

    delete pClassInterface;
}

ClassDirEntry *NewClassDirEntry(const char *pszName, const char *pszNamespace)
{
    ClassDirEntry *pClass;

    assert(pszName != NULL);

    pClass = new ClassDirEntry;
    if (!pClass) return NULL;
    memset(pClass, 0, sizeof(ClassDirEntry));

    pClass->mDesc = new ClassDescriptor;
    if (!pClass->mDesc) goto ErrorExit;
    memset(pClass->mDesc, 0, sizeof(ClassDescriptor));

    pClass->mDesc->ppInterfaces = \
        new ClassInterface *[c_nMaxClassInterfaces];
    if (!pClass->mDesc->ppInterfaces) goto ErrorExit;

    pClass->mDesc->pAggrIndexs = new USHORT[c_nMaxClassAspects];
    if (!pClass->mDesc->pAggrIndexs) goto ErrorExit;

    pClass->mDesc->pAspectIndexs = new USHORT[c_nMaxClassAspects];
    if (!pClass->mDesc->pAspectIndexs) goto ErrorExit;

    pClass->mDesc->pClassIndexs = new USHORT[c_nMaxAggrClassesOfAspect];
    if (!pClass->mDesc->pClassIndexs) goto ErrorExit;

    pClass->mName = new char[strlen(pszName) + 1];
    if (!pClass->mName) goto ErrorExit;
    strcpy(pClass->mName, pszName);
    if (pszNamespace != NULL) {
        pClass->mNameSpace = new char[strlen(pszNamespace) + 1];
        if (!pClass->mNameSpace) goto ErrorExit;
        strcpy(pClass->mNameSpace, pszNamespace);
    }

    return pClass;

ErrorExit:
    if (pClass->mDesc) {
        if (pClass->mDesc->pClassIndexs)
            delete [] pClass->mDesc->pClassIndexs;
        if (pClass->mDesc->pAspectIndexs)
            delete [] pClass->mDesc->pAspectIndexs;
        if (pClass->mDesc->pAggrIndexs)
            delete [] pClass->mDesc->pAggrIndexs;
        if (pClass->mDesc->ppInterfaces)
            delete [] pClass->mDesc->ppInterfaces;
        delete pClass->mDesc;
    }
    delete pClass;
    return NULL;
}

void DeleteClassDirEntry(ClassDirEntry *pClass)
{
    assert(pClass != NULL);
    assert(pClass->mDesc != NULL);
    assert(pClass->mDesc->ppInterfaces != NULL);
    assert(pClass->mDesc->pAggrIndexs != NULL);
    assert(pClass->mDesc->pAspectIndexs != NULL);

    for (int n = 0; n < pClass->mDesc->mInterfaceCount; n++) {
        DeleteClassInterface(pClass->mDesc->ppInterfaces[n]);
    }
    delete [] pClass->mDesc->ppInterfaces;
    delete [] pClass->mDesc->pAggrIndexs;
    delete [] pClass->mDesc->pAspectIndexs;
    delete pClass->mDesc;
    if (pClass->mName) delete pClass->mName;
    if (pClass->mNameSpace) delete pClass->mNameSpace;
    delete pClass;
}

InterfaceConstDescriptor *NewInterfaceConstDirEntry(const char *pszName)
{
    InterfaceConstDescriptor *pConst;

    assert(pszName != NULL);

    pConst = new InterfaceConstDescriptor;
    if (!pConst) return NULL;
    memset(pConst, 0, sizeof(InterfaceConstDescriptor));

    pConst->mName = new char[strlen(pszName) + 1];
    if (!pConst->mName) {
        delete pConst;
        return NULL;
    }
    strcpy(pConst->mName, pszName);
    return pConst;
}

ParamDescriptor *NewParam(const char *pszName)
{
    ParamDescriptor *pParam;

    assert(pszName != NULL);

    pParam = new ParamDescriptor;
    if (!pParam) return NULL;
    memset(pParam, 0, sizeof(ParamDescriptor));

    pParam->mName = new char[strlen(pszName) + 1];
    if (!pParam->mName) {
        delete pParam;
        return NULL;
    }
    strcpy(pParam->mName, pszName);
    return pParam;
}

void DeleteParam(ParamDescriptor *pParam)
{
    assert(pParam != NULL);
    assert(pParam->mName != NULL);

    if (pParam->type.mNestedType) delete pParam->type.mNestedType;
    delete pParam->mName;
    delete pParam;
}

MethodDescriptor *NewMethod(const char *pszName)
{
    MethodDescriptor *pMethod;

    assert(pszName != NULL);

    pMethod = new MethodDescriptor;
    if (!pMethod) return NULL;
    memset(pMethod, 0, sizeof(MethodDescriptor));

    pMethod->ppParams = \
        new ParamDescriptor *[c_nMaxParams];
    if (!pMethod->ppParams) goto ErrorExit;

    pMethod->mName = new char[strlen(pszName) + 1];
    if (!pMethod->mName) goto ErrorExit;
    strcpy(pMethod->mName, pszName);

    return pMethod;

ErrorExit:
    if (pMethod->ppParams)
        delete [] pMethod->ppParams;
    delete pMethod;
    return NULL;
}

void DeleteMethods(MethodDescriptor *pMethod)
{
    assert(pMethod != NULL);
    assert(pMethod->mName != NULL);
    assert(pMethod->ppParams != NULL);

    for (int n = 0; n < pMethod->cParams; n++) {
        DeleteParam(pMethod->ppParams[n]);
    }
    delete [] pMethod->ppParams;
    delete pMethod->pszSignature;
    delete pMethod->mName;
    delete pMethod;
}

InterfaceDirEntry *NewInterfaceDirEntry(const char *pszName, const char *pszNamespace)
{
    assert(pszName != NULL);

    InterfaceDirEntry *pInterface;

    pInterface = new InterfaceDirEntry;
    if (!pInterface) return NULL;
    memset(pInterface, 0, sizeof(InterfaceDirEntry));

    pInterface->mDesc = new InterfaceDescriptor;
    if (!pInterface->mDesc) goto ErrorExit;
    memset(pInterface->mDesc, 0, sizeof(InterfaceDescriptor));

    pInterface->mDesc->ppConsts = \
        new InterfaceConstDescriptor *[c_nMaxInterfaceConsts];
    if (!pInterface->mDesc->ppConsts) goto ErrorExit;

    pInterface->mDesc->ppMethods = \
        new MethodDescriptor *[c_nMaxMethods];
    if (!pInterface->mDesc->ppMethods) goto ErrorExit;

    pInterface->mName = new char[strlen(pszName) + 1];
    if (!pInterface->mName) goto ErrorExit;
    strcpy(pInterface->mName, pszName);
    if (pszNamespace != NULL) {
        pInterface->mNameSpace = new char[strlen(pszNamespace) + 1];
        if (!pInterface->mNameSpace) goto ErrorExit;
        strcpy(pInterface->mNameSpace, pszNamespace);
    }

    return pInterface;

ErrorExit:
    if (pInterface->mDesc) {
        if (pInterface->mDesc->ppMethods)
            delete [] pInterface->mDesc->ppMethods;
        delete pInterface->mDesc;
    }
    delete pInterface;
    return NULL;
}

void DeleteInterfaceDirEntry(InterfaceDirEntry *pInterface)
{
    assert(pInterface != NULL);
    assert(pInterface->mName != NULL);
    assert(pInterface->mDesc != NULL);
    assert(pInterface->mDesc->ppMethods != NULL);

    for (int n = 0; n < pInterface->mDesc->cMethods; n++) {
        DeleteMethods(pInterface->mDesc->ppMethods[n]);
    }
    delete [] pInterface->mDesc->ppMethods;
    delete pInterface->mDesc;
    delete pInterface->mName;
    if (pInterface->mNameSpace) delete pInterface->mNameSpace;
    delete pInterface;
}

StructElement *NewStructElement(const char *pszName)
{
    StructElement *pElement;

    assert(pszName != NULL);

    pElement = new StructElement;
    if (!pElement) return NULL;
    memset(pElement, 0, sizeof(StructElement));

    pElement->mName = new char[strlen(pszName) + 1];
    if (!pElement->mName) {
        delete pElement;
        return NULL;
    }
    strcpy(pElement->mName, pszName);
    return pElement;
}

void DeleteStructElement(StructElement *pElement)
{
    assert(pElement != NULL);
    assert(pElement->mName != NULL);

    if (pElement->type.mNestedType) delete pElement->type.mNestedType;
    delete pElement->mName;
    delete pElement;
}

StructDirEntry *NewStructDirEntry(const char *pszName)
{
    StructDirEntry *pStruct;

    assert(pszName != NULL);

    pStruct = new StructDirEntry;
    if (!pStruct) return NULL;
    memset(pStruct, 0, sizeof(StructDirEntry));

    pStruct->mDesc = new StructDescriptor;
    if (!pStruct->mDesc) goto ErrorExit;
    memset(pStruct->mDesc, 0, sizeof(StructDescriptor));

    pStruct->mDesc->ppElems = new StructElement *[c_nMaxStructElements];
    if (!pStruct->mDesc->ppElems) goto ErrorExit;

    pStruct->mName = new char[strlen(pszName) + 1];
    if (!pStruct->mName) goto ErrorExit;
    strcpy(pStruct->mName, pszName);

    return pStruct;

ErrorExit:
    if (pStruct->mDesc) {
        if (pStruct->mDesc->ppElems)
            delete [] pStruct->mDesc->ppElems;
        delete pStruct->mDesc;
    }
    delete pStruct;
    return NULL;
}

void DeleteStructDirEntry(StructDirEntry *pStruct)
{
    assert(pStruct != NULL);
    assert(pStruct->mName != NULL);
    assert(pStruct->mDesc != NULL);
    assert(pStruct->mDesc->ppElems != NULL);

    for (int n = 0; n < pStruct->mDesc->cElems; n++) {
        DeleteStructElement(pStruct->mDesc->ppElems[n]);
    }
    delete [] pStruct->mDesc->ppElems;
    delete pStruct->mDesc;
    delete pStruct->mName;
    if (pStruct->mNameSpace) delete pStruct->mNameSpace;
    delete pStruct;
}

ArrayDirEntry *NewArrayDirEntry()
{
    ArrayDirEntry *pArray = new ArrayDirEntry;
    if (NULL == pArray)
        return NULL;

    memset(pArray, 0, sizeof(ArrayDirEntry));

    return pArray;
}

void DeleteArrayDirEntry(ArrayDirEntry *pArray)
{
    assert(pArray);

    if (pArray->mNameSpace) {
        delete pArray->mNameSpace;
        pArray->mNameSpace = NULL;
    }

    if (pArray->type.mNestedType) {
        delete pArray->type.mNestedType;
        pArray->type.mNestedType = NULL;
    }

    delete pArray;
}

ConstDirEntry *NewConstDirEntry(const char *pszName)
{
    assert(pszName != NULL);

    ConstDirEntry *pConst = new ConstDirEntry;
    if (NULL == pConst)
        return NULL;

    memset(pConst, 0, sizeof(ConstDirEntry));

    pConst->mName = new char[strlen(pszName) + 1];
    if (!pConst->mName) goto ErrorExit;
    strcpy(pConst->mName, pszName);

    return pConst;

ErrorExit:
    delete pConst;
    return NULL;
}

void DeleteConstDirEntry(ConstDirEntry *pConst)
{
    assert(pConst != NULL);
    assert(pConst->mName != NULL);

    if (pConst->mNameSpace) {
        delete pConst->mNameSpace;
        pConst->mNameSpace = NULL;
    }

    delete pConst->mName;
    if (pConst->type == TYPE_STRING && pConst->v.strValue.pszValue != NULL) {
        free(pConst->v.strValue.pszValue);
    }
    delete pConst;
}

EnumElement *NewEnumElement(const char *pszName)
{
    EnumElement *pElement;

    assert(pszName != NULL);

    pElement = new EnumElement;
    if (!pElement) return NULL;
    memset(pElement, 0, sizeof(EnumElement));

    pElement->mName = new char[strlen(pszName) + 1];
    if (!pElement->mName) {
        delete pElement;
        return NULL;
    }
    strcpy(pElement->mName, pszName);
    return pElement;
}

void DeleteEnumElement(EnumElement *pElement)
{
    assert(pElement != NULL);
    assert(pElement->mName != NULL);

    delete pElement->mName;
    delete pElement;
}

EnumDirEntry *NewEnumDirEntry(const char *pszName, const char *pszNamespace)
{
    EnumDirEntry *pEnum;

    assert(pszName != NULL);

    pEnum = new EnumDirEntry;
    if (!pEnum) return NULL;
    memset(pEnum, 0, sizeof(EnumDirEntry));

    pEnum->mDesc = new EnumDescriptor;
    if (!pEnum->mDesc) goto ErrorExit;
    memset(pEnum->mDesc, 0, sizeof(EnumDescriptor));

    pEnum->mDesc->ppElems = new EnumElement *[c_nMaxEnumElements];
    if (!pEnum->mDesc->ppElems) goto ErrorExit;

    pEnum->mName = new char[strlen(pszName) + 1];
    if (!pEnum->mName) goto ErrorExit;
    strcpy(pEnum->mName, pszName);
    if (pszNamespace != NULL) {
        pEnum->mNameSpace = new char[strlen(pszNamespace) + 1];
        if (!pEnum->mNameSpace) goto ErrorExit;
        strcpy(pEnum->mNameSpace, pszNamespace);
    }

    return pEnum;

ErrorExit:
    if (pEnum->mDesc) {
        if (pEnum->mDesc->ppElems)
            delete [] pEnum->mDesc->ppElems;
        delete pEnum->mDesc;
    }
    delete pEnum;
    return NULL;
}

void DeleteEnumDirEntry(EnumDirEntry *pEnum)
{
    assert(pEnum != NULL);
    assert(pEnum->mName != NULL);
    assert(pEnum->mDesc != NULL);
    assert(pEnum->mDesc->ppElems != NULL);

    for (int n = 0; n < pEnum->mDesc->cElems; n++) {
        DeleteEnumElement(pEnum->mDesc->ppElems[n]);
    }
    delete [] pEnum->mDesc->ppElems;
    delete pEnum->mDesc;
    delete pEnum->mName;
    if (pEnum->mNameSpace) delete pEnum->mNameSpace;
    delete pEnum;
}

AliasDirEntry *NewAliasDirEntry(const char *pszName)
{
    assert(pszName != NULL);

    AliasDirEntry *pAlias;

    pAlias = new AliasDirEntry;
    if (!pAlias) return NULL;
    memset(pAlias, 0, sizeof(AliasDirEntry));

    pAlias->mName = new char[strlen(pszName) + 1];
    if (!pAlias->mName) {
        delete pAlias;
        return NULL;
    }
    strcpy(pAlias->mName, pszName);

    return pAlias;
}

void DeleteAliasDirEntry(AliasDirEntry *pAlias)
{
    assert(pAlias != NULL);
    assert(pAlias->mName != NULL);

    if (pAlias->type.mNestedType) delete pAlias->type.mNestedType;
    delete pAlias->mName;
    if (pAlias->mNameSpace) delete pAlias->mNameSpace;
    delete pAlias;
}

CLSModule *CreateCLS()
{
    CLSModule *pModule;

    pModule = new CLSModule;
    if (!pModule) return NULL;
    memset(pModule, '\0', sizeof(CLSModule));

    memcpy(pModule->mMagic, MAGIC_STRING, MAGIC_STRING_LENGTH);
    pModule->mCLSModuleVersion = CLSMODULE_VERSION;

    pModule->mClassDirs = new ClassDirEntry *[MAX_CLASS_NUMBER];
    pModule->mInterfaceDirs = new InterfaceDirEntry *[MAX_INTERFACE_NUMBER];
    pModule->mDefinedInterfaceIndexes = new int[MAX_DEFINED_INTERFACE_NUMBER];
    pModule->mStructDirs = new StructDirEntry *[MAX_STRUCT_NUMBER];
    pModule->mEnumDirs = new EnumDirEntry *[MAX_ENUM_NUMBER];
    pModule->mAliasDirs = new AliasDirEntry *[MAX_ALIAS_NUMBER];
    pModule->mLibraryNames = new char *[MAX_LIBRARY_NUMBER];
    pModule->mArrayDirs = new ArrayDirEntry *[MAX_ARRAY_NUMBER];
    pModule->mConstDirs = new ConstDirEntry *[MAX_CONST_NUMBER];

    if (!pModule->mClassDirs || !pModule->mInterfaceDirs ||
        !pModule->mStructDirs || !pModule->mEnumDirs ||
        !pModule->mAliasDirs || !pModule->mLibraryNames ||
        !pModule->mArrayDirs || !pModule->mConstDirs ||
        !pModule->mDefinedInterfaceIndexes) {
        DestroyCLS(pModule);
        return NULL;
    }

    return pModule;
}

void DestroyCLS(CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);

    if (pModule->mClassDirs) {
        for (n = 0; n < pModule->mClassCount; n++) {
            DeleteClassDirEntry(pModule->mClassDirs[n]);
        }
        delete [] pModule->mClassDirs;
    }

    if (pModule->mInterfaceDirs) {
        for (n = 0; n < pModule->mInterfaceCount; n++) {
            DeleteInterfaceDirEntry(pModule->mInterfaceDirs[n]);
        }
        delete [] pModule->mInterfaceDirs;
    }

    if (pModule->mDefinedInterfaceIndexes) {
        delete [] pModule->mDefinedInterfaceIndexes;
    }

    if (pModule->mArrayDirs) {
        for (n = 0; n < pModule->mArrayCount; n++) {
            DeleteArrayDirEntry(pModule->mArrayDirs[n]);
        }
        delete [] pModule->mArrayDirs;
    }

    if (pModule->mStructDirs) {
        for (n = 0; n < pModule->mStructCount; n++) {
            DeleteStructDirEntry(pModule->mStructDirs[n]);
        }
        delete [] pModule->mStructDirs;
    }

    if (pModule->mEnumDirs) {
        for (n = 0; n < pModule->mEnumCount; n++) {
            DeleteEnumDirEntry(pModule->mEnumDirs[n]);
        }
        delete [] pModule->mEnumDirs;
    }

    if (pModule->mAliasDirs) {
        for (n = 0; n < pModule->mAliasCount; n++) {
            DeleteAliasDirEntry(pModule->mAliasDirs[n]);
        }
        delete [] pModule->mAliasDirs;
    }

    if (pModule->mConstDirs) {
        for (n = 0; n < pModule->mConstCount; n++) {
            DeleteConstDirEntry(pModule->mConstDirs[n]);
        }
        delete [] pModule->mConstDirs;
    }

    if (pModule->mLibraryNames) {
        for (n = 0; n < pModule->mLibraryCount; n++) {
            delete pModule->mLibraryNames[n];
        }
        delete [] pModule->mLibraryNames;
    }

    if (pModule->mUunm) delete pModule->mUunm;
    if (pModule->mName) delete pModule->mName;

    delete pModule;
}

int CreateClassDirEntry(
    const char *pszName, CLSModule *pModule, unsigned long dwAttribs)
{
    int n;
    ClassDirEntry *pClass;
    ClassDescriptor *pDesc;

    char *pszNamespace = NULL;
    const char *dot = strrchr(pszName, '.');
    if (dot != NULL) {
        pszNamespace = (char*)malloc(dot - pszName + 1);
        memset(pszNamespace, 0, dot - pszName + 1);
        memcpy(pszNamespace, pszName, dot - pszName);
        pszName = dot + 1;
    }
    n = SelectClassDirEntry(pszName, pszNamespace, pModule);
    if (n >= 0) {
        pDesc = pModule->mClassDirs[n]->mDesc;
        if (pszNamespace != NULL) free(pszNamespace);

        if (CLASS_TYPE(dwAttribs) != CLASS_TYPE(pDesc->dwAttribs)) {
            ExtraMessage(pModule->mClassDirs[n]->mNameSpace,
                        "class", pszName);
            _ReturnError (CLSError_NameConflict);
        }
        if (pDesc->mInterfaceCount > 0
            || (pDesc->dwAttribs & ClassAttrib_hasparent) > 0
            || IsValidUUID(&pDesc->clsid)) {
            ExtraMessage(pModule->mClassDirs[n]->mNameSpace,
                        "class", pszName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, pszNamespace, pModule, GType_Class, NULL);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_NameConflict);
    }
    if (pModule->mClassCount >= MAX_CLASS_NUMBER) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_FullEntry);
    }

    pClass = NewClassDirEntry(pszName, pszNamespace);
    if (pszNamespace != NULL) free(pszNamespace);
    if (!pClass) _ReturnError (CLSError_OutOfMemory);
    pModule->mClassDirs[pModule->mClassCount] = pClass;

    _ReturnOK (pModule->mClassCount++);
}

int CreateInterfaceDirEntry(
    const char *pszName, CLSModule *pModule, unsigned long dwAttribs)
{
    int n;
    InterfaceDirEntry *pInterface;
    InterfaceDescriptor *pDesc;

    char *pszNamespace = NULL;
    const char *dot = strrchr(pszName, '.');
    if (dot != NULL) {
        pszNamespace = (char*)malloc(dot - pszName + 1);
        memset(pszNamespace, 0, dot - pszName + 1);
        memcpy(pszNamespace, pszName, dot - pszName);
        pszName = dot + 1;
    }
    n = SelectInterfaceDirEntry(pszName, pszNamespace, pModule);
    if (n >= 0) {
        pDesc = pModule->mInterfaceDirs[n]->mDesc;
        if (pszNamespace != NULL) free(pszNamespace);

        if (INTERFACE_TYPE(dwAttribs) != \
            INTERFACE_TYPE(pDesc->dwAttribs)) {
            ExtraMessage(pModule->mInterfaceDirs[n]->mNameSpace,
                        "interface", pModule->mInterfaceDirs[n]->mName);
            _ReturnError (CLSError_NameConflict);
        }
        if (pDesc->cMethods > 0 || pDesc->sParentIndex != 0
            || IsValidUUID(&pDesc->iid)) {
            ExtraMessage(pModule->mInterfaceDirs[n]->mNameSpace,
                        "interface", pModule->mInterfaceDirs[n]->mName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, pszNamespace, pModule, GType_Interface, NULL);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_NameConflict);
    }
    if (pModule->mInterfaceCount >= MAX_INTERFACE_NUMBER) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_FullEntry);
    }

    pInterface = NewInterfaceDirEntry(pszName, pszNamespace);
    if (pszNamespace != NULL) free(pszNamespace);
    if (!pInterface) _ReturnError (CLSError_OutOfMemory);
    pModule->mInterfaceDirs[pModule->mInterfaceCount] = pInterface;

    _ReturnOK (pModule->mInterfaceCount++);
}

int CreateArrayDirEntry(CLSModule *pModule)
{
    ArrayDirEntry *pArray;

    pArray = NewArrayDirEntry();
    if (!pArray) _ReturnError (CLSError_OutOfMemory);

    pModule->mArrayDirs[pModule->mArrayCount] = pArray;

    _ReturnOK (pModule->mArrayCount++);
}

int CreateStructDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    StructDirEntry *pStruct;

    n = SelectStructDirEntry(pszName, pModule);
    if (n >= 0) {
        if (pModule->mStructDirs[n]->mDesc->cElems > 0) {
            ExtraMessage(pModule->mStructDirs[n]->mNameSpace,
                        "struct", pModule->mStructDirs[n]->mName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_Struct, NULL);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pModule->mStructCount >= MAX_STRUCT_NUMBER)
        _ReturnError (CLSError_FullEntry);

    pStruct = NewStructDirEntry(pszName);
    if (!pStruct) _ReturnError (CLSError_OutOfMemory);
    pModule->mStructDirs[pModule->mStructCount] = pStruct;

    _ReturnOK (pModule->mStructCount++);
}

int CreateEnumDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    EnumDirEntry *pEnum;

    char *pszNamespace = NULL;
    const char *dot = strrchr(pszName, '.');
    if (dot != NULL) {
        pszNamespace = (char*)malloc(dot - pszName + 1);
        memset(pszNamespace, 0, dot - pszName + 1);
        memcpy(pszNamespace, pszName, dot - pszName);
        pszName = dot + 1;
    }
    n = SelectEnumDirEntry(pszName, pszNamespace, pModule);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        if (pModule->mEnumDirs[n]->mDesc->cElems > 0) {
            ExtraMessage(pModule->mEnumDirs[n]->mNameSpace,
                        "enum", pModule->mEnumDirs[n]->mName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, pszNamespace, pModule, GType_Enum, NULL);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_NameConflict);
    }
    if (pModule->mEnumCount >= MAX_ENUM_NUMBER) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_FullEntry);
    }

    pEnum = NewEnumDirEntry(pszName, pszNamespace);
    if (pszNamespace != NULL) free(pszNamespace);
    if (!pEnum) _ReturnError (CLSError_OutOfMemory);
    pModule->mEnumDirs[pModule->mEnumCount] = pEnum;

    _ReturnOK (pModule->mEnumCount++);
}

int CreateConstDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    ConstDirEntry *pConst;

    n = SelectConstDirEntry(pszName, pModule);
    if (n >= 0) {
        _ReturnError (CLSError_DupEntry);
    }

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_Const, NULL);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pModule->mConstCount >= MAX_CONST_NUMBER) _ReturnError (CLSError_FullEntry);

    pConst = NewConstDirEntry(pszName);
    if (!pConst) _ReturnError (CLSError_OutOfMemory);
    pModule->mConstDirs[pModule->mConstCount] = pConst;

    _ReturnOK (pModule->mConstCount++);
}

int CreateAliasDirEntry(
    const char *pszName, CLSModule *pModule, TypeDescriptor *pType)
{
    int n;
    AliasDirEntry *pAlias;
    GlobalSymbolType gType;
    TypeDescriptor type;

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_None, &gType);
    if (n >= 0) {
        if (gType == GType_Class) {
            _ReturnError (CLSError_NameConflict);
        }

        memset(&type, 0, sizeof(type));
        type.mType = (CARDataType)gType;
        type.mIndex = n;
        if (!IsEqualType(pModule, &type, pType)) {
            _ReturnError (CLSError_NameConflict);
        }
    }
    if (pModule->mAliasCount >= MAX_ALIAS_NUMBER)
        _ReturnError (CLSError_FullEntry);

    pAlias = NewAliasDirEntry(pszName);
    if (!pAlias) _ReturnError (CLSError_OutOfMemory);
    memcpy(&pAlias->type, pType, sizeof(TypeDescriptor));
    pModule->mAliasDirs[pModule->mAliasCount] = pAlias;

    _ReturnOK (pModule->mAliasCount++);
}

int CreateClassInterface(USHORT index, ClassDescriptor *pDesc)
{
    int n;
    ClassInterface *pClassInterface;

    assert(pDesc != NULL);

    n = SelectClassInterface(index, pDesc);
    if (n >= 0) _ReturnError (CLSError_DupEntry);

    if (pDesc->mInterfaceCount >= c_nMaxClassInterfaces)
        _ReturnError (CLSError_FullEntry);

    pClassInterface = NewClassInterface(index);
    if (!pClassInterface) _ReturnError (CLSError_OutOfMemory);
    pDesc->ppInterfaces[pDesc->mInterfaceCount] = pClassInterface;

    _ReturnOK (pDesc->mInterfaceCount++);
}

int CreateInterfaceConstDirEntry(
    const char *pszName, InterfaceDescriptor *pInterface)
{
    int n;
    InterfaceConstDescriptor *pConst;

    n = SelectInterfaceConstDirEntry(pszName, pInterface);
    if (n >= 0) {
        _ReturnError (CLSError_DupEntry);
    }

    n = SelectInterfaceMemberSymbol(pszName, pInterface);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pInterface->mConstCount >= c_nMaxInterfaceConsts) _ReturnError (CLSError_FullEntry);

    pConst = NewInterfaceConstDirEntry(pszName);
    if (!pConst) _ReturnError (CLSError_OutOfMemory);
    pInterface->ppConsts[pInterface->mConstCount] = pConst;

    _ReturnOK (pInterface->mConstCount++);
}

int CreateInterfaceMethod(
    const char *pszName, InterfaceDescriptor *pInterface)
{
    MethodDescriptor *pMethod;

    assert(pInterface != NULL);
    assert(pszName != NULL);

    if (pInterface->cMethods >= c_nMaxMethods)
        _ReturnError (CLSError_FullEntry);

    pMethod = NewMethod(pszName);
    if (!pMethod) _ReturnError (CLSError_OutOfMemory);
    pInterface->ppMethods[pInterface->cMethods] = pMethod;

    _ReturnOK (pInterface->cMethods++);
}

int CreateMethodParam(
    const char *pszName, MethodDescriptor *pMethod)
{
    int n;
    ParamDescriptor *pParam;

    assert(pMethod != NULL);
    assert(pszName != NULL);

    n = SelectMethodParam(pszName, pMethod);
    if (n >= 0) {
        ExtraMessage("method parameter", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    if (pMethod->cParams >= c_nMaxParams) _ReturnError (CLSError_FullEntry);
    pParam = NewParam(pszName);
    if (!pParam) _ReturnError (CLSError_OutOfMemory);
    pMethod->ppParams[pMethod->cParams] = pParam;

    _ReturnOK (pMethod->cParams++);
}

int CreateStructElement(
    const char *pszName, StructDescriptor *pStruct)
{
    int n;
    StructElement *pElement;

    assert(pStruct != NULL);
    assert(pszName != NULL);

    n = SelectStructElement(pszName, pStruct);
    if (n >= 0) {
        ExtraMessage("struct member", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    if (pStruct->cElems >= c_nMaxStructElements)
        _ReturnError (CLSError_FullEntry);

    pElement = NewStructElement(pszName);
    if (!pElement) _ReturnError (CLSError_OutOfMemory);
    pStruct->ppElems[pStruct->cElems] = pElement;

    _ReturnOK (pStruct->cElems++);
}

int CreateEnumElement(
    const char *pszName, CLSModule *pModule, EnumDescriptor *pEnum)
{
    int n;
    EnumElement *pElement;

    assert(pModule != NULL);
    assert(pEnum != NULL);
    assert(pszName != NULL);

    n = SelectEnumElement(pszName, pEnum);
    if (n >= 0) {
        ExtraMessage("enum member", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    pElement = GlobalSelectEnumElement(pszName, pModule);
    if (pElement) _ReturnError (CLSError_NameConflict);

    if (pEnum->cElems >= c_nMaxEnumElements)
        _ReturnError (CLSError_FullEntry);

    pElement = NewEnumElement(pszName);
    if (!pElement) _ReturnError (CLSError_OutOfMemory);
    pEnum->ppElems[pEnum->cElems] = pElement;

    _ReturnOK (pEnum->cElems++);
}

int MethodAppend(
    const MethodDescriptor *pSrc, InterfaceDescriptor *pDesc)
{
    int n, m;
    MethodDescriptor *pDest;
    ParamDescriptor *pParam;

    n = CreateInterfaceMethod(pSrc->mName, pDesc);
    if (n < 0) _Return (n);

    pDest = pDesc->ppMethods[n];
    pDest->pszSignature = new char[strlen(pSrc->pszSignature) + 1];
    strcpy(pDest->pszSignature, pSrc->pszSignature);

    TypeCopy(&pSrc->type, &pDest->type);

    for (n = 0; n < pSrc->cParams; n++) {
        m = CreateMethodParam(pSrc->ppParams[n]->mName, pDest);
        if (m < 0) _Return (m);
        pParam = pDest->ppParams[m];
        pParam->dwAttribs = pSrc->ppParams[n]->dwAttribs;
        TypeCopy(&pSrc->ppParams[n]->type, &pParam->type);
    }

    _ReturnOK (CLS_NoError);
}

int InterfaceMethodsAppend(const CLSModule *pModule,
    const InterfaceDescriptor *pSrc, InterfaceDescriptor *pDest)
{
    int n, m;

    if (0 != pSrc->sParentIndex) {
        n = InterfaceMethodsAppend(pModule,
                pModule->mInterfaceDirs[pSrc->sParentIndex]->mDesc,
                pDest);
        if (n < 0) _Return (n);
    }

    for (n = 0; n < pSrc->cMethods; n++) {
        m = MethodAppend(pSrc->ppMethods[n], pDest);
        if (m < 0) _Return (m);
    }

    _ReturnOK (CLS_NoError);
}
