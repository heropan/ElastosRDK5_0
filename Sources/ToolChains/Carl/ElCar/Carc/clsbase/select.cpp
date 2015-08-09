//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <clsbase.h>

int SelectClassDirEntry(const char *pszName, const char *pszNamespaces, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mClassCount; n++) {
        if (!strcmp(pszName, pModule->mClassDirs[n]->pszName)) {
            if (pModule->mClassDirs[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->mClassDirs[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->mClassDirs[n]->pszNameSpace)) _ReturnOK (n);
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = ';'; begin = semicolon + 1; }
                else begin = NULL;
            }
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectInterfaceDirEntry(const char *pszName, const char *pszNamespaces, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mInterfaceCount; n++) {
        if (!strcmp(pszName, pModule->mInterfaceDirs[n]->pszName)) {
            if (pModule->mInterfaceDirs[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->mInterfaceDirs[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->mInterfaceDirs[n]->pszNameSpace)) _ReturnOK (n);
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = ';'; begin = semicolon + 1; }
                else begin = NULL;
            }
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectArrayDirEntry(unsigned short nElems, const TypeDescriptor &desp,
    const CLSModule *pModule)
{
    int n;
    assert(nElems > 0);
    assert(pModule != NULL);

    for (n = 0; n < pModule->mArrayCount; n++) {
        if (nElems == pModule->mArrayDirs[n]->nElements
            && 0 == memcmp(&desp, &(pModule->mArrayDirs[n]->type), sizeof(TypeDescriptor)))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectStructDirEntry(const char *pszName, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mStructCount; n++) {
        if (!strcmp(pszName, pModule->mStructDirs[n]->pszName))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectEnumDirEntry(const char *pszName, const char *pszNamespaces, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mEnumCount; n++) {
        if (!strcmp(pszName, pModule->mEnumDirs[n]->pszName)) {
            if (pModule->mEnumDirs[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->mEnumDirs[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->mEnumDirs[n]->pszNameSpace)) _ReturnOK (n);
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = ';'; begin = semicolon + 1; }
                else begin = NULL;
            }
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectConstDirEntry(const char *pszName, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mConstCount; n++) {
        if (!strcmp(pszName, pModule->mConstDirs[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectAliasDirEntry(const char *pszName, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->mAliasCount; n++) {
        if (!strcmp(pszName, pModule->mAliasDirs[n]->pszName))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectClassInterface(USHORT sIndex, const ClassDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);

    for (n = 0; n < pDesc->mInterfaceCount; n++) {
        if (pDesc->ppInterfaces[n]->sIndex == sIndex) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectInterfaceConstDirEntry(
    const char *pszName, const InterfaceDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pDesc->mConstCount; n++) {
        if (!strcmp(pszName, pDesc->ppConsts[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectInterfaceMethod(
    const char *pszName, const InterfaceDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pDesc->cMethods; n++) {
        if (!strcmp(pszName, pDesc->ppMethods[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectMethodParam(const char *pszName, const MethodDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pDesc->cParams; n++) {
        if (!strcmp(pszName, pDesc->ppParams[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectStructElement(
    const char *pszName, const StructDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pDesc->cElems; n++) {
        if (!strcmp(pszName, pDesc->ppElems[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int SelectEnumElement(const char *pszName, const EnumDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pDesc->cElems; n++) {
        if (!strcmp(pszName, pDesc->ppElems[n]->pszName)) {
            _ReturnOK (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

int GlobalSelectSymbol(
    const char *pszName, const char *pszNamespace, const CLSModule *pModule,
    GlobalSymbolType except, GlobalSymbolType *pType)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    if (except != GType_Class) {
        n = SelectClassDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->mClassDirs[n]->pszNameSpace,
                        "class", pszName);
            if (pType) *pType = GType_Class;
            _Return (n);
        }
    }

    if (except != GType_Interface) {
        n = SelectInterfaceDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->mInterfaceDirs[n]->pszNameSpace,
                        "interface", pszName);
            if (pType) *pType = GType_Interface;
            _Return (n);
        }
    }

    if (except != GType_Struct) {
        n = SelectStructDirEntry(pszName, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->mStructDirs[n]->pszNameSpace,
                        "struct", pszName);
            if (pType) *pType = GType_Struct;
            _Return (n);
        }
    }

    if (except != GType_Alias) {
        n = SelectAliasDirEntry(pszName, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->mAliasDirs[n]->pszNameSpace,
                        "alias", pszName);
            if (pType) *pType = GType_Alias;
            _Return (n);
        }
    }

    if (except != GType_Enum) {
        n = SelectEnumDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->mEnumDirs[n]->pszNameSpace,
                        "enum", pszName);
            if (pType) *pType = GType_Enum;
            _Return (n);
        }
    }

    _ReturnError (CLSError_NotFound);
}

EnumElement *GlobalSelectEnumElement(
    const char *pszName, const CLSModule *pModule)
{
    int n, m;

    for (n = 0; n < pModule->mEnumCount; n++) {
        m = SelectEnumElement(pszName, pModule->mEnumDirs[n]->pDesc);
        if (m >= 0) {
            ExtraMessage(pModule->mEnumDirs[n]->pszNameSpace,
                        "enum", pModule->mEnumDirs[n]->pszName);
            return pModule->mEnumDirs[n]->pDesc->ppElems[m];
        }
    }
    return NULL;
}

int SelectInterfaceMemberSymbol(
    const char *pszName, InterfaceDescriptor *pDesc)
{
    int n;

    assert(pszName != NULL);
    assert(pDesc != NULL);

    for (n = 0; n < pDesc->mConstCount; ++n) {
        if (!strcmp(pszName, pDesc->ppConsts[n]->pszName)) _Return (n);
    }

    for (n = 0; n < pDesc->cMethods; ++n) {
        if (!strcmp(pszName, pDesc->ppMethods[n]->pszName)) _Return (n);
    }

    _ReturnError (CLSError_NotFound);
}
