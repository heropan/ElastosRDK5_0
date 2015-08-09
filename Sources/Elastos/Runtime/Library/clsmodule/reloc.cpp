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

    if (0 != p->mInterfaceCount) {
        p->ppInterfaces = \
            (ClassInterface **)((int)p->ppInterfaces + sBase);

        for (int n = 0; n < p->mInterfaceCount; n++) {
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
    if (0 != p->mConstCount) {
        p->ppConsts = (InterfaceConstDescriptor **)((int)p->ppConsts + sBase);

        for (int n = 0; n < p->mConstCount; n++) {
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

    p->mName += sBase;
    if (p->mUunm) p->mUunm += sBase;
    if (p->mServiceName) p->mServiceName += sBase;
    if (p->mDefinedInterfaceCount > 0) {
        p->mDefinedInterfaceIndexes =
            (int *)((int)(p->mDefinedInterfaceIndexes) + sBase);
    }

    if (0 != p->mClassCount) {
        p->mClassDirs = (ClassDirEntry **)((int)p->mClassDirs + sBase);

        for (int n = 0; n < p->mClassCount; n++) {
            p->mClassDirs[n] = \
                (ClassDirEntry *)((int)p->mClassDirs[n] + sBase);
            MapClassDirEntry(p->mClassDirs[n]);
        }
    }
    else {
        p->mClassDirs = NULL;
    }

    if (0 != p->mInterfaceCount) {
        p->mInterfaceDirs = \
            (InterfaceDirEntry **)((int)p->mInterfaceDirs + sBase);

        for (int n = 0; n < p->mInterfaceCount; n++) {
            p->mInterfaceDirs[n] = \
                (InterfaceDirEntry *)((int)p->mInterfaceDirs[n] + sBase);
            MapInterfaceDirEntry(p->mInterfaceDirs[n]);
        }
    }
    else {
        p->mInterfaceDirs = NULL;
    }

    if (0 != p->mArrayCount) {
        p->mArrayDirs = (ArrayDirEntry **)((int)p->mArrayDirs + sBase);

        for (int n = 0; n < p->mArrayCount; n++) {
            p->mArrayDirs[n] = \
                (ArrayDirEntry *)((int)p->mArrayDirs[n] + sBase);
            MapArrayDirEntry(p->mArrayDirs[n]);
        }
    }
    else {
        p->mArrayDirs = NULL;
    }

    if (0 != p->mStructCount) {
        p->mStructDirs = (StructDirEntry **)((int)p->mStructDirs + sBase);

        for (int n = 0; n < p->mStructCount; n++) {
            p->mStructDirs[n] = \
                (StructDirEntry *)((int)p->mStructDirs[n] + sBase);
            MapStructDirEntry(p->mStructDirs[n]);
        }
    }
    else {
        p->mStructDirs = NULL;
    }

    if (0 != p->mEnumCount) {
        p->mEnumDirs = (EnumDirEntry **)((int)p->mEnumDirs + sBase);

        for (int n = 0; n < p->mEnumCount; n++) {
            p->mEnumDirs[n] = \
                (EnumDirEntry *)((int)p->mEnumDirs[n] + sBase);
            MapEnumDirEntry(p->mEnumDirs[n]);
        }
    }
    else {
        p->mEnumDirs = NULL;
    }

    if (0 != p->mConstCount) {
        p->mConstDirs = (ConstDirEntry **)((int)p->mConstDirs + sBase);

        for (int n = 0; n < p->mConstCount; n++) {
            p->mConstDirs[n] = \
                (ConstDirEntry *)((int)p->mConstDirs[n] + sBase);
            MapConstDirEntry(p->mConstDirs[n]);
        }
    }
    else {
        p->mConstDirs = NULL;
    }

    if (0 != p->mAliasCount) {
        p->mAliasDirs = (AliasDirEntry **)((int)p->mAliasDirs + sBase);

        for (int n = 0; n < p->mAliasCount; n++) {
            p->mAliasDirs[n] = \
                (AliasDirEntry *)((int)p->mAliasDirs[n] + sBase);
            MapAliasDirEntry(p->mAliasDirs[n]);
        }
    }
    else {
        p->mAliasDirs = NULL;
    }

    if (0 != p->mLibraryCount) {
        p->mLibraryNames = (char **)((int)p->mLibraryNames + sBase);
        for (int n = 0; n < p->mLibraryCount; n++) {
            p->mLibraryNames[n] += sBase;
        }
    }
    else {
        p->mLibraryNames = NULL;
    }
}

int RelocFlattedCLS(
    /* [in] */ const void* src,
    /* [in] */ int size,
    /* [out] */ CLSModule** outDest)
{
    CLSModule* srcModule = (CLSModule *)src;

    CLSModule* destModule = (CLSModule *)new char[srcModule->mSize];
    if (!destModule) _ReturnError(CLSError_OutOfMemory);

    if (srcModule->mAttribs & CARAttrib_compress) {
        int n = UncompressCLS(src, size, destModule);
        if (n != srcModule->mSize) {
            delete [] (char *)destModule;
            _ReturnError(CLSError_FormatSize);
        }
    }
    else {
        memcpy(destModule, srcModule, size);
    }

    DoRelocCLS(destModule);
    destModule->mAttribs |= CARAttrib_inheap;

    *outDest = destModule;
    _ReturnOK(CLS_NoError);
}

int DisposeFlattedCLS(
    /* [in] */ void* dest)
{
    if (((CLSModule *)dest)->mAttribs & CARAttrib_inheap) {
        delete [] (char *)dest;
    }
    _ReturnOK(CLS_NoError);
}
