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

    for (n = 0; n < pModule->cClasses; n++) {
        if (!strcmp(pszName, pModule->ppClassDir[n]->pszName)) {
            if (pModule->ppClassDir[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->ppClassDir[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->ppClassDir[n]->pszNameSpace)) _ReturnOK (n);
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

    for (n = 0; n < pModule->cInterfaces; n++) {
        if (!strcmp(pszName, pModule->ppInterfaceDir[n]->pszName)) {
            if (pModule->ppInterfaceDir[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->ppInterfaceDir[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->ppInterfaceDir[n]->pszNameSpace)) _ReturnOK (n);
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

    for (n = 0; n < pModule->cArrays; n++) {
        if (nElems == pModule->ppArrayDir[n]->nElements
            && 0 == memcmp(&desp, &(pModule->ppArrayDir[n]->type), sizeof(TypeDescriptor)))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectStructDirEntry(const char *pszName, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->cStructs; n++) {
        if (!strcmp(pszName, pModule->ppStructDir[n]->pszName))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectEnumDirEntry(const char *pszName, const char *pszNamespaces, const CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);
    assert(pszName != NULL);

    for (n = 0; n < pModule->cEnums; n++) {
        if (!strcmp(pszName, pModule->ppEnumDir[n]->pszName)) {
            if (pModule->ppEnumDir[n]->pszNameSpace == NULL) _ReturnOK (n);
            //temp
            if (!strcmp("systypes", pModule->ppEnumDir[n]->pszNameSpace)) _ReturnOK (n);

            const char *begin, *semicolon;
            begin = pszNamespaces;
            while (begin != NULL) {
                semicolon = strchr(begin, ';');
                if (semicolon != NULL) { *const_cast<char*>(semicolon) = '\0'; }
                if (!strcmp(begin, pModule->ppEnumDir[n]->pszNameSpace)) _ReturnOK (n);
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

    for (n = 0; n < pModule->cConsts; n++) {
        if (!strcmp(pszName, pModule->ppConstDir[n]->pszName)) {
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

    for (n = 0; n < pModule->cAliases; n++) {
        if (!strcmp(pszName, pModule->ppAliasDir[n]->pszName))
            _ReturnOK (n);
    }

    _ReturnError (CLSError_NotFound);
}

int SelectClassInterface(USHORT sIndex, const ClassDescriptor *pDesc)
{
    int n;

    assert(pDesc != NULL);

    for (n = 0; n < pDesc->cInterfaces; n++) {
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

    for (n = 0; n < pDesc->cConsts; n++) {
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
            ExtraMessage(pModule->ppClassDir[n]->pszNameSpace,
                        "class", pszName);
            if (pType) *pType = GType_Class;
            _Return (n);
        }
    }

    if (except != GType_Interface) {
        n = SelectInterfaceDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->ppInterfaceDir[n]->pszNameSpace,
                        "interface", pszName);
            if (pType) *pType = GType_Interface;
            _Return (n);
        }
    }

    if (except != GType_Struct) {
        n = SelectStructDirEntry(pszName, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->ppStructDir[n]->pszNameSpace,
                        "struct", pszName);
            if (pType) *pType = GType_Struct;
            _Return (n);
        }
    }

    if (except != GType_Alias) {
        n = SelectAliasDirEntry(pszName, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->ppAliasDir[n]->pszNameSpace,
                        "alias", pszName);
            if (pType) *pType = GType_Alias;
            _Return (n);
        }
    }

    if (except != GType_Enum) {
        n = SelectEnumDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) {
            ExtraMessage(pModule->ppEnumDir[n]->pszNameSpace,
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

    for (n = 0; n < pModule->cEnums; n++) {
        m = SelectEnumElement(pszName, pModule->ppEnumDir[n]->pDesc);
        if (m >= 0) {
            ExtraMessage(pModule->ppEnumDir[n]->pszNameSpace,
                        "enum", pModule->ppEnumDir[n]->pszName);
            return pModule->ppEnumDir[n]->pDesc->ppElems[m];
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

    for (n = 0; n < pDesc->cConsts; ++n) {
        if (!strcmp(pszName, pDesc->ppConsts[n]->pszName)) _Return (n);
    }

    for (n = 0; n < pDesc->cMethods; ++n) {
        if (!strcmp(pszName, pDesc->ppMethods[n]->pszName)) _Return (n);
    }

    _ReturnError (CLSError_NotFound);
}
