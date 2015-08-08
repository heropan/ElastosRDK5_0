//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <clsbase.h>

static int sBase;

void MapClassDescriptor(
    /* [in] */ ClassDescriptor* p)
{
    if (0 != p->cAggregates)
        p->pAggrIndexs = (USHORT *)((int)p->pAggrIndexs + sBase);
    if (0 != p->cAspects)
        p->pAspectIndexs = (USHORT *)((int)p->pAspectIndexs + sBase);
    if (0 != p->cClasses)
        p->pClassIndexs = (USHORT *)((int)p->pClassIndexs + sBase);

    if (0 != p->cInterfaces) {
        p->ppInterfaces = \
            (ClassInterface **)((int)p->ppInterfaces + sBase);

        for (int n = 0; n < p->cInterfaces; n++) {
            p->ppInterfaces[n] = \
                (ClassInterface *)((int)p->ppInterfaces[n] + sBase);
        }
    }
}

void MapClassDirEntry(
    /* [in] */ ClassDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    p->pDesc = (ClassDescriptor *)((int)p->pDesc + sBase);

    MapClassDescriptor(p->pDesc);
}

void MapInterfaceConstDescriptor(
    /* [in] */ InterfaceConstDescriptor* p)
{
    p->pszName += sBase;
    if (p->type == TYPE_STRING && p->v.pStrValue != NULL) p->v.pStrValue += sBase;
}

void MapMethodDescriptor(
    /* [in] */ MethodDescriptor* p)
{
    p->pszName += sBase;
    p->pszSignature += sBase;

    if (0 != p->cParams) {
        p->ppParams = (ParamDescriptor **)((int)p->ppParams + sBase);

        for (int n = 0; n < p->cParams; n++) {
            p->ppParams[n] = \
                (ParamDescriptor *)((int)p->ppParams[n] + sBase);
            p->ppParams[n]->pszName += sBase;

            if (p->ppParams[n]->type.pNestedType) {
                p->ppParams[n]->type.pNestedType = (TypeDescriptor *) \
                    ((int)p->ppParams[n]->type.pNestedType + sBase);
            }
        }
    }
}

void MapInterfaceDescriptor(
    /* [in] */ InterfaceDescriptor* p)
{
    if (0 != p->cConsts) {
        p->ppConsts = (InterfaceConstDescriptor **)((int)p->ppConsts + sBase);

        for (int n = 0; n < p->cConsts; n++) {
            p->ppConsts[n] = \
                (InterfaceConstDescriptor *)((int)p->ppConsts[n] + sBase);
            MapInterfaceConstDescriptor(p->ppConsts[n]);
        }
    }

    if (0 != p->cMethods) {
        p->ppMethods = (MethodDescriptor **)((int)p->ppMethods + sBase);

        for (int n = 0; n < p->cMethods; n++) {
            p->ppMethods[n] = \
                (MethodDescriptor *)((int)p->ppMethods[n] + sBase);
            MapMethodDescriptor(p->ppMethods[n]);
        }
    }
}

void MapInterfaceDirEntry(
    /* [in] */ InterfaceDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    p->pDesc = (InterfaceDescriptor *)((int)p->pDesc + sBase);

    MapInterfaceDescriptor(p->pDesc);
}

void MapArrayDirEntry(
    /* [in] */ ArrayDirEntry* p)
{
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    if (p->type.pNestedType) {
        p->type.pNestedType = (TypeDescriptor *) \
            ((int)p->type.pNestedType + sBase);
    }
}

void MapStructDescriptor(
    /* [in] */ StructDescriptor* p)
{
    if (0 != p->cElems) {
        p->ppElems = (StructElement **)((int)p->ppElems + sBase);

        for (int n = 0; n < p->cElems; n++) {
            p->ppElems[n] = (StructElement *)((int)p->ppElems[n] + sBase);
            p->ppElems[n]->pszName += sBase;

            if (p->ppElems[n]->type.pNestedType) {
                p->ppElems[n]->type.pNestedType = (TypeDescriptor *) \
                    ((int)p->ppElems[n]->type.pNestedType + sBase);
            }
        }
    }
}

void MapStructDirEntry(
    /* [in] */ StructDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    p->pDesc = (StructDescriptor *)((int)p->pDesc + sBase);

    MapStructDescriptor(p->pDesc);
}

void MapEnumDescriptor(
    /* [in] */ EnumDescriptor* p)
{
    if (0 != p->cElems) {
        p->ppElems = (EnumElement **)((int)p->ppElems + sBase);

        for (int n = 0; n < p->cElems; n++) {
            p->ppElems[n] = (EnumElement *)((int)p->ppElems[n] + sBase);
            p->ppElems[n]->pszName += sBase;
        }
    }
}

void MapEnumDirEntry(
    /* [in] */ EnumDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    p->pDesc = (EnumDescriptor *)((int)p->pDesc + sBase);

    MapEnumDescriptor(p->pDesc);
}

void MapConstDirEntry(
    /* [in] */ ConstDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;
    if (p->type == TYPE_STRING && p->v.strValue.pszValue != NULL) p->v.strValue.pszValue += sBase;
}

void MapAliasDirEntry(
    /* [in] */ AliasDirEntry* p)
{
    p->pszName += sBase;
    if (p->pszNameSpace) p->pszNameSpace += sBase;

    if (p->type.pNestedType) {
        p->type.pNestedType = \
            (TypeDescriptor *)((int)p->type.pNestedType + sBase);
    }
}

void DoRelocCLS(
    /* [in] */ CLSModule* p)
{
    sBase = (int)p;

    p->pszName += sBase;
    if (p->pszUunm) p->pszUunm += sBase;
    if (p->pszServiceName) p->pszServiceName += sBase;
    if (p->cDefinedInterfaces) {
        p->pDefinedInterfaceIndex =
            (int *)((int)(p->pDefinedInterfaceIndex) + sBase);
    }

    if (0 != p->cClasses) {
        p->ppClassDir = (ClassDirEntry **)((int)p->ppClassDir + sBase);

        for (int n = 0; n < p->cClasses; n++) {
            p->ppClassDir[n] = \
                (ClassDirEntry *)((int)p->ppClassDir[n] + sBase);
            MapClassDirEntry(p->ppClassDir[n]);
        }
    }
    else {
        p->ppClassDir = NULL;
    }

    if (0 != p->cInterfaces) {
        p->ppInterfaceDir = \
            (InterfaceDirEntry **)((int)p->ppInterfaceDir + sBase);

        for (int n = 0; n < p->cInterfaces; n++) {
            p->ppInterfaceDir[n] = \
                (InterfaceDirEntry *)((int)p->ppInterfaceDir[n] + sBase);
            MapInterfaceDirEntry(p->ppInterfaceDir[n]);
        }
    }
    else {
        p->ppInterfaceDir = NULL;
    }

    if (0 != p->cArrays) {
        p->ppArrayDir = (ArrayDirEntry **)((int)p->ppArrayDir + sBase);

        for (int n = 0; n < p->cArrays; n++) {
            p->ppArrayDir[n] = \
                (ArrayDirEntry *)((int)p->ppArrayDir[n] + sBase);
            MapArrayDirEntry(p->ppArrayDir[n]);
        }
    }
    else {
        p->ppArrayDir = NULL;
    }

    if (0 != p->cStructs) {
        p->ppStructDir = (StructDirEntry **)((int)p->ppStructDir + sBase);

        for (int n = 0; n < p->cStructs; n++) {
            p->ppStructDir[n] = \
                (StructDirEntry *)((int)p->ppStructDir[n] + sBase);
            MapStructDirEntry(p->ppStructDir[n]);
        }
    }
    else {
        p->ppStructDir = NULL;
    }

    if (0 != p->cEnums) {
        p->ppEnumDir = (EnumDirEntry **)((int)p->ppEnumDir + sBase);

        for (int n = 0; n < p->cEnums; n++) {
            p->ppEnumDir[n] = \
                (EnumDirEntry *)((int)p->ppEnumDir[n] + sBase);
            MapEnumDirEntry(p->ppEnumDir[n]);
        }
    }
    else {
        p->ppEnumDir = NULL;
    }

    if (0 != p->cConsts) {
        p->ppConstDir = (ConstDirEntry **)((int)p->ppConstDir + sBase);

        for (int n = 0; n < p->cConsts; n++) {
            p->ppConstDir[n] = \
                (ConstDirEntry *)((int)p->ppConstDir[n] + sBase);
            MapConstDirEntry(p->ppConstDir[n]);
        }
    }
    else {
        p->ppConstDir = NULL;
    }

    if (0 != p->cAliases) {
        p->ppAliasDir = (AliasDirEntry **)((int)p->ppAliasDir + sBase);

        for (int n = 0; n < p->cAliases; n++) {
            p->ppAliasDir[n] = \
                (AliasDirEntry *)((int)p->ppAliasDir[n] + sBase);
            MapAliasDirEntry(p->ppAliasDir[n]);
        }
    }
    else {
        p->ppAliasDir = NULL;
    }

    if (0 != p->cLibraries) {
        p->ppLibNames = (char **)((int)p->ppLibNames + sBase);
        for (int n = 0; n < p->cLibraries; n++) {
            p->ppLibNames[n] += sBase;
        }
    }
    else {
        p->ppLibNames = NULL;
    }
}

int RelocFlattedCLS(
    /* [in] */ const void* src,
    /* [in] */ int size,
    /* [out] */ CLSModule** outDest)
{
    CLSModule* srcModule = (CLSModule *)src;

    CLSModule* destModule = (CLSModule *)new char[srcModule->nSize];
    if (!destModule) _ReturnError(CLSError_OutOfMemory);

    if (srcModule->dwAttribs & CARAttrib_compress) {
        int n = UncompressCLS(src, size, destModule);
        if (n != srcModule->nSize) {
            delete [] (char *)destModule;
            _ReturnError(CLSError_FormatSize);
        }
    }
    else {
        memcpy(destModule, srcModule, size);
    }

    DoRelocCLS(destModule);
    destModule->dwAttribs |= CARAttrib_inheap;

    *outDest = destModule;
    _ReturnOK(CLS_NoError);
}

int DisposeFlattedCLS(
    /* [in] */ void* dest)
{
    if (((CLSModule *)dest)->dwAttribs & CARAttrib_inheap) {
        delete [] (char *)dest;
    }
    _ReturnOK(CLS_NoError);
}
