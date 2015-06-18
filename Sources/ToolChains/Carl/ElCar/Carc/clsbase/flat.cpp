//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "clsbase.h"

#ifdef _linux
#define _alloca alloca
#endif

class CFlatBuffer
{
public:
    static int CalcBufferSize(const CLSModule *pModule);

public:
    int Flat(CLSModule *pModule, void *pBuffer);

private:

    int WriteString(const char *s);
    int WriteData(void *pData, int nSize);

    int WriteClassDirEntry(ClassDirEntry *pEntry);
    int WriteClassDescriptor(ClassDescriptor *pDesc);

    int WriteInterfaceDirEntry(InterfaceDirEntry *pEntry);
    int WriteInterfaceDescriptor(InterfaceDescriptor *pDesc);
    int WriteInterfaceConstDescriptor(InterfaceConstDescriptor *pDesc);
    int WriteMethodDescriptor(MethodDescriptor *pDesc);

    int WriteArrayDirEntry(ArrayDirEntry *pEntry);

    int WriteStructDirEntry(StructDirEntry *pEntry);
    int WriteStructDescriptor(StructDescriptor *pDesc);

    int WriteEnumDirEntry(EnumDirEntry *pEntry);
    int WriteEnumDescriptor(EnumDescriptor *pDesc);

    int WriteAliasDirEntry(AliasDirEntry *pEntry);

    int WriteConstDirEntry(ConstDirEntry *pEntry);

private:
    const CLSModule *m_pModule;

    char            *m_pBuffer;
    int             m_nOffset;

    int             m_cStrings;

    static const int MAXNUM = 4096 * 10;
    char            *m_ppStrings[MAXNUM];
};

int CFlatBuffer::WriteData(void *p, int size)
{
    int nBeginAddress = m_nOffset;

    memcpy(m_pBuffer + m_nOffset, p, size);
    m_nOffset += RoundUp4(size);

    return nBeginAddress;
}

int CFlatBuffer::WriteString(const char *s)
{
    int n, nSize, nBeginAddress;

    for (n = 0; n < m_cStrings; n++) {
        if (!strcmp(s, m_ppStrings[n]))
            return (int)(m_ppStrings[n]) - (int)(m_pBuffer);
    }

    nBeginAddress = m_nOffset;
    nSize = strlen(s) + 1;
    memcpy(m_pBuffer + m_nOffset, s, nSize);

    assert(m_cStrings < MAXNUM);
    m_ppStrings[m_cStrings++] = m_pBuffer + m_nOffset;

    m_nOffset += RoundUp4(nSize);
    return nBeginAddress;
}

int CFlatBuffer::WriteClassDescriptor(ClassDescriptor *pDesc)
{
    int *p, n;
    ClassDescriptor d;

    memcpy(&d, pDesc, sizeof(ClassDescriptor));

    if (d.cInterfaces > 0) {
        p = (int *)_alloca(d.cInterfaces * sizeof(int));

        for (n = 0; n < d.cInterfaces; n++)
            p[n] = WriteData(d.ppInterfaces[n], sizeof(ClassInterface));

        d.ppInterfaces = (ClassInterface **) \
            WriteData(p, d.cInterfaces * sizeof(int));
    }

    if (d.cAggregates > 0) {
        d.pAggrIndexs = (USHORT *)WriteData( \
            d.pAggrIndexs, d.cAggregates * sizeof(USHORT));
    }
    if (d.cAspects > 0) {
        d.pAspectIndexs = (USHORT *)WriteData( \
            d.pAspectIndexs, d.cAspects * sizeof(USHORT));
    }
    if (d.cClasses > 0) {
        d.pClassIndexs = (USHORT *)WriteData( \
            d.pClassIndexs, d.cClasses * sizeof(USHORT));
    }

    return WriteData(&d, sizeof(ClassDescriptor));
}

int CFlatBuffer::WriteClassDirEntry(ClassDirEntry *pClassDirEntry)
{
    ClassDirEntry entry;

    memcpy(&entry, pClassDirEntry, sizeof(ClassDirEntry));

    entry.pDesc = (ClassDescriptor *) \
            WriteClassDescriptor(entry.pDesc);

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    return WriteData(&entry, sizeof(ClassDirEntry));
}

int CFlatBuffer::WriteInterfaceConstDescriptor(InterfaceConstDescriptor *pDesc)
{
    InterfaceConstDescriptor d;

    memcpy(&d, pDesc, sizeof(InterfaceConstDescriptor));

    d.pszName = (char *)WriteString(d.pszName);
    if (d.type == TYPE_STRING && d.v.pStrValue != NULL) {
        d.v.pStrValue = (char *)WriteString(d.v.pStrValue);
    }

    return WriteData(&d, sizeof(InterfaceConstDescriptor));
}

int CFlatBuffer::WriteMethodDescriptor(MethodDescriptor *pDesc)
{
    int *p, n;
    MethodDescriptor d;
    ParamDescriptor param;

    memcpy(&d, pDesc, sizeof(MethodDescriptor));

    if (d.cParams > 0) {
        p = (int *)_alloca(d.cParams * sizeof(int));

        for (n = 0; n < d.cParams; n++) {
            memcpy(&param, d.ppParams[n], sizeof(ParamDescriptor));
            param.pszName = (char *)WriteString(param.pszName);
            if (param.type.pNestedType) {
                param.type.pNestedType = (TypeDescriptor *)WriteData( \
                    param.type.pNestedType, sizeof(TypeDescriptor));
            }
            p[n] = WriteData(&param, sizeof(ParamDescriptor));
        }

        d.ppParams = (ParamDescriptor **) \
            WriteData(p, d.cParams * sizeof(int));
    }
    d.pszName = (char *)WriteString(d.pszName);
    d.pszSignature = (char *)WriteString(d.pszSignature);

    return WriteData(&d, sizeof(MethodDescriptor));
}

int CFlatBuffer::WriteInterfaceDescriptor(
    InterfaceDescriptor *pDesc)
{
    int *p, n;
    InterfaceDescriptor d;

    memcpy(&d, pDesc, sizeof(InterfaceDescriptor));

    if (d.cConsts > 0) {
        p = (int *)_alloca(d.cConsts * sizeof(int));

        for (n = 0; n < d.cConsts; n++) {
            p[n] = WriteInterfaceConstDescriptor(d.ppConsts[n]);
        }

        d.ppConsts = (InterfaceConstDescriptor **) \
            WriteData(p, d.cConsts * sizeof(int));
    }

    if (d.cMethods > 0) {
        p = (int *)_alloca(d.cMethods * sizeof(int));

        for (n = 0; n < d.cMethods; n++) {
            p[n] = WriteMethodDescriptor(d.ppMethods[n]);
        }

        d.ppMethods = (MethodDescriptor **) \
            WriteData(p, d.cMethods * sizeof(int));
    }

    return WriteData(&d, sizeof(InterfaceDescriptor));
}

int CFlatBuffer::WriteInterfaceDirEntry(
    InterfaceDirEntry *pInterfaceDirEntry)
{
    InterfaceDirEntry entry;

    memcpy(&entry, pInterfaceDirEntry, sizeof(InterfaceDirEntry));

    entry.pDesc = (InterfaceDescriptor *) \
            WriteInterfaceDescriptor(entry.pDesc);

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    return WriteData(&entry, sizeof(InterfaceDirEntry));
}

int CFlatBuffer::WriteArrayDirEntry(ArrayDirEntry *pArrayDirEntry)
{
    ArrayDirEntry entry;

    memcpy(&entry, pArrayDirEntry, sizeof(ArrayDirEntry));

    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    if (entry.type.pNestedType) {
        entry.type.pNestedType = (TypeDescriptor *)WriteData( \
            entry.type.pNestedType, sizeof(TypeDescriptor));
    }

    return WriteData(&entry, sizeof(ArrayDirEntry));
}

int CFlatBuffer::WriteStructDescriptor(StructDescriptor *pDesc)
{
    int *p, n;
    StructDescriptor d;
    StructElement elem;

    memcpy(&d, pDesc, sizeof(StructDescriptor));

    if (d.cElems > 0) {
        p = (int *)_alloca(d.cElems * sizeof(int));

        for (n = 0; n < d.cElems; n++) {
            memcpy(&elem, d.ppElems[n], sizeof(StructElement));
            elem.pszName = (char *)WriteString(elem.pszName);
            if (elem.type.pNestedType) {
                elem.type.pNestedType = (TypeDescriptor *)WriteData( \
                    elem.type.pNestedType, sizeof(TypeDescriptor));
            }
            p[n] = WriteData(&elem, sizeof(StructElement));
        }

        d.ppElems = (StructElement **) \
            WriteData(p, d.cElems * sizeof(int));
    }

    return WriteData(&d, sizeof(StructDescriptor));
}

int CFlatBuffer::WriteStructDirEntry(StructDirEntry *pStructDirEntry)
{
    StructDirEntry entry;

    memcpy(&entry, pStructDirEntry, sizeof(StructDirEntry));

    entry.pDesc = (StructDescriptor *) \
            WriteStructDescriptor(entry.pDesc);

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    return WriteData(&entry, sizeof(StructDirEntry));
}

int CFlatBuffer::WriteEnumDescriptor(EnumDescriptor *pDesc)
{
    int *p, n;
    EnumDescriptor d;
    EnumElement elem;

    memcpy(&d, pDesc, sizeof(EnumDescriptor));

    if (d.cElems > 0) {
        p = (int *)_alloca(d.cElems * sizeof(int));

        for (n = 0; n < d.cElems; n++) {
            memcpy(&elem, d.ppElems[n], sizeof(EnumElement));
            elem.pszName = (char *)WriteString(elem.pszName);
            p[n] = WriteData(&elem, sizeof(EnumElement));
        }

        d.ppElems = (EnumElement **) \
            WriteData(p, d.cElems * sizeof(int));
    }

    return WriteData(&d, sizeof(EnumDescriptor));
}

int CFlatBuffer::WriteEnumDirEntry(EnumDirEntry *pEnumDirEntry)
{
    EnumDirEntry entry;

    memcpy(&entry, pEnumDirEntry, sizeof(EnumDirEntry));

    entry.pDesc = (EnumDescriptor *) \
            WriteEnumDescriptor(entry.pDesc);

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    return WriteData(&entry, sizeof(EnumDirEntry));
}

int CFlatBuffer::WriteAliasDirEntry(AliasDirEntry *pAliasDirEntry)
{
    AliasDirEntry entry;

    memcpy(&entry, pAliasDirEntry, sizeof(AliasDirEntry));

    if (entry.type.pNestedType) {
        entry.type.pNestedType = (TypeDescriptor *)WriteData( \
            entry.type.pNestedType, sizeof(TypeDescriptor));
    }

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }

    return WriteData(&entry, sizeof(AliasDirEntry));
}

int CFlatBuffer::WriteConstDirEntry(ConstDirEntry *pConstDirEntry)
{
    ConstDirEntry entry;

    memcpy(&entry, pConstDirEntry, sizeof(ConstDirEntry));

    entry.pszName = (char *)WriteString(entry.pszName);
    if (entry.pszNameSpace) {
        entry.pszNameSpace = (char *)WriteString(entry.pszNameSpace);
    }
    if (entry.type == TYPE_STRING && entry.v.strValue.pszValue != NULL) {
        entry.v.strValue.pszValue = (char*)WriteString(entry.v.strValue.pszValue);
    }

    return WriteData(&entry, sizeof(ConstDirEntry));
}

int CFlatBuffer::Flat(CLSModule *pModule, void *pvDest)
{
    int n, *p;

    m_pModule = pModule;
    m_pBuffer = (char *)pvDest;
    m_nOffset = sizeof(CLSModule);
    m_cStrings = 0;

    pModule = (CLSModule *)pvDest;
    memcpy(pModule, m_pModule, sizeof(CLSModule));

    if (pModule->cClasses > 0) {
        p = (int *)_alloca(pModule->cClasses * sizeof(int));

        for (n = 0; n < pModule->cClasses; n++)
            p[n] = WriteClassDirEntry(pModule->ppClassDir[n]);

        pModule->ppClassDir = (ClassDirEntry **) \
            WriteData(p, pModule->cClasses * sizeof(int));
    }
    else {
        pModule->ppClassDir = NULL;
    }

    if (pModule->cInterfaces > 0) {
        p = (int *)_alloca(pModule->cInterfaces * sizeof(int));

        for (n = 0; n < pModule->cInterfaces; n++)
            p[n] = WriteInterfaceDirEntry(pModule->ppInterfaceDir[n]);

        pModule->ppInterfaceDir = (InterfaceDirEntry **) \
            WriteData(p, pModule->cInterfaces * sizeof(int));
    }
    else {
        pModule->ppInterfaceDir = NULL;
    }

    if (pModule->cDefinedInterfaces > 0) {
        p = (int *)_alloca(pModule->cDefinedInterfaces * sizeof(int));

        for (n = 0; n < pModule->cDefinedInterfaces; n++)
            p[n] = pModule->pDefinedInterfaceIndex[n];

        pModule->pDefinedInterfaceIndex = (int *) \
            WriteData(p, pModule->cDefinedInterfaces * sizeof(int));
    }
    else {
        pModule->pDefinedInterfaceIndex = NULL;
    }

    if (pModule->cStructs > 0) {
        p = (int *)_alloca(pModule->cStructs * sizeof(int));

        for (n = 0; n < pModule->cStructs; n++)
            p[n] = WriteStructDirEntry(pModule->ppStructDir[n]);

        pModule->ppStructDir = (StructDirEntry **) \
            WriteData(p, pModule->cStructs * sizeof(int));
    }
    else {
        pModule->ppStructDir = NULL;
    }
    if (pModule->cArrays > 0) {
        p = (int *)_alloca(pModule->cArrays * sizeof(int));

        for (n = 0; n < pModule->cArrays; n++)
            p[n] = WriteArrayDirEntry(pModule->ppArrayDir[n]);

        pModule->ppArrayDir = (ArrayDirEntry **) \
            WriteData(p, pModule->cArrays * sizeof(int));
    }
    else {
        pModule->ppArrayDir = NULL;
    }

    if (pModule->cEnums > 0) {
        p = (int *)_alloca(pModule->cEnums * sizeof(int));

        for (n = 0; n < pModule->cEnums; n++)
            p[n] = WriteEnumDirEntry(pModule->ppEnumDir[n]);

        pModule->ppEnumDir = (EnumDirEntry **) \
            WriteData(p, pModule->cEnums * sizeof(int));
    }
    else {
        pModule->ppEnumDir = NULL;
    }

    if (pModule->cAliases > 0) {
        p = (int *)_alloca(pModule->cAliases * sizeof(int));

        for (n = 0; n < pModule->cAliases; n++)
            p[n] = WriteAliasDirEntry(pModule->ppAliasDir[n]);

        pModule->ppAliasDir = (AliasDirEntry **) \
            WriteData(p, pModule->cAliases * sizeof(int));
    }
    else {
        pModule->ppAliasDir = NULL;
    }

    if (pModule->cConsts > 0) {
        p = (int *)_alloca(pModule->cConsts * sizeof(int));

        for (n = 0; n < pModule->cConsts; n++)
            p[n] = WriteConstDirEntry(pModule->ppConstDir[n]);

        pModule->ppConstDir = (ConstDirEntry **) \
            WriteData(p, pModule->cConsts * sizeof(int));
    }
    else {
        pModule->ppConstDir = NULL;
    }

    if (pModule->cLibraries > 0) {
        p = (int *)_alloca(pModule->cLibraries * sizeof(int));

        for (n = 0; n < pModule->cLibraries; n++)
            p[n] = WriteString(pModule->ppLibNames[n]);

        pModule->ppLibNames = (char **) \
            WriteData(p, pModule->cLibraries * sizeof(int));
    }
    else {
        pModule->ppLibNames = NULL;
    }

    pModule->pszName = (char *)WriteString(pModule->pszName);
    if (pModule->pszUunm) {
        pModule->pszUunm = (char *)WriteString(pModule->pszUunm);
    }

    if (pModule->pszServiceName) {
        pModule->pszServiceName = (char *)WriteString(pModule->pszServiceName);
    }

    pModule->nSize = m_nOffset;

    return m_nOffset;
}

inline int StringAlignSize(const char *s)
{
    return RoundUp4(strlen(s) + 1);
}

int CalcClassSize(ClassDirEntry *p)
{
    int size = sizeof(ClassDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);

    size += sizeof(ClassDescriptor);

    size += p->pDesc->cInterfaces * \
        (sizeof(ClassInterface) + sizeof(ClassInterface *));

    size += RoundUp4(p->pDesc->cAggregates * sizeof(USHORT));
    size += RoundUp4(p->pDesc->cAspects * sizeof(USHORT));
    size += RoundUp4(p->pDesc->cClasses * sizeof(USHORT));

    return size;
}

int CalcInterfaceConstSize(InterfaceConstDescriptor *p)
{
    int size = sizeof(InterfaceConstDescriptor);

    size += StringAlignSize(p->pszName);

    if (p->type == TYPE_STRING && p->v.pStrValue != NULL) {
        size += StringAlignSize(p->v.pStrValue);
    }

    return size;
}

int CalcMethodSize(MethodDescriptor *p)
{
    int n, size = sizeof(MethodDescriptor);

    size += StringAlignSize(p->pszName);
    size += StringAlignSize(p->pszSignature);

    for (n = 0; n < p->cParams; n++) {
        if (p->ppParams[n]->type.pNestedType) {
            size += sizeof(TypeDescriptor);
        }
        size += StringAlignSize(p->ppParams[n]->pszName);
    }
    size += n * (sizeof(ParamDescriptor) + sizeof(ParamDescriptor *));

    return size;
}

int CalcInterfaceSize(InterfaceDirEntry *p)
{
    int n, size = sizeof(InterfaceDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);

    size += sizeof(InterfaceDescriptor);

    for (n = 0; n < p->pDesc->cConsts; n++) {
        size += CalcInterfaceConstSize(p->pDesc->ppConsts[n]);
    }
    size += n * sizeof(InterfaceConstDescriptor *);

    for (n = 0; n < p->pDesc->cMethods; n++) {
        size += CalcMethodSize(p->pDesc->ppMethods[n]);
    }
    size += n * sizeof(MethodDescriptor *);

    return size;
}

int CalcArraySize(ArrayDirEntry *p)
{
    int size = sizeof(ArrayDirEntry);

    if (p->pszNameSpace)
        size += StringAlignSize(p->pszNameSpace);

    if (p->type.pNestedType)
        size += sizeof(TypeDescriptor);

    return size;
}

int CalcStructSize(StructDirEntry *p)
{
    int n, size = sizeof(StructDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);

    size += sizeof(StructDescriptor);

    for (n = 0; n < p->pDesc->cElems; n++) {
        if (p->pDesc->ppElems[n]->type.pNestedType) {
            size += sizeof(TypeDescriptor);
        }
        size += StringAlignSize(p->pDesc->ppElems[n]->pszName);
    }
    size += n * (sizeof(StructElement) + sizeof(StructElement *));

    return size;
}

int CalcEnumSize(EnumDirEntry *p)
{
    int n, size = sizeof(EnumDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);

    size += sizeof(EnumDescriptor);

    for (n = 0; n < p->pDesc->cElems; n++) {
        size += StringAlignSize(p->pDesc->ppElems[n]->pszName);
    }
    size += n * (sizeof(EnumElement) + sizeof(EnumElement *));

    return size;
}

int CalcAliasSize(AliasDirEntry *p)
{
    int size = sizeof(AliasDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);
    if (p->type.pNestedType) size += sizeof(TypeDescriptor);

    return size;
}

int CalcConstSize(ConstDirEntry *p)
{
    int size = sizeof(ConstDirEntry);

    size += StringAlignSize(p->pszName);
    if (p->pszNameSpace) size += StringAlignSize(p->pszNameSpace);

    if (p->type == TYPE_STRING && p->v.strValue.pszValue != NULL) {
        size += StringAlignSize(p->v.strValue.pszValue);
    }

    return size;
}

int CFlatBuffer::CalcBufferSize(const CLSModule *pModule)
{
    int n, size = sizeof(CLSModule);

    for (n = 0; n < pModule->cClasses; n++) {
        size += CalcClassSize(pModule->ppClassDir[n]);
    }
    size += n * sizeof(ClassDirEntry *);

    for (n = 0; n < pModule->cInterfaces; n++) {
        size += CalcInterfaceSize(pModule->ppInterfaceDir[n]);
    }
    size += n * sizeof(InterfaceDirEntry *);

    if (pModule->cDefinedInterfaces) {
        size += pModule->cDefinedInterfaces * sizeof(int);
    }

    for (n = 0; n < pModule->cArrays; n++) {
        size += CalcArraySize(pModule->ppArrayDir[n]);
    }
    size += n * sizeof(ArrayDirEntry *);

    for (n = 0; n < pModule->cStructs; n++) {
        size += CalcStructSize(pModule->ppStructDir[n]);
    }
    size += n * sizeof(StructDirEntry *);

    for (n = 0; n < pModule->cEnums; n++) {
        size += CalcEnumSize(pModule->ppEnumDir[n]);
    }
    size += n * sizeof(EnumDirEntry *);

    for (n = 0; n < pModule->cAliases; n++) {
        size += CalcAliasSize(pModule->ppAliasDir[n]);
    }
    size += n * sizeof(AliasDirEntry *);

    for (n = 0; n < pModule->cConsts; n++) {
        size += CalcConstSize(pModule->ppConstDir[n]);
    }
    size += n * sizeof(ConstDirEntry *);

    for (n = 0; n < pModule->cLibraries; n++) {
        size += StringAlignSize(pModule->ppLibNames[n]);
    }
    size += n * sizeof(char *);

    size += StringAlignSize(pModule->pszName);
    if (pModule->pszUunm) size += StringAlignSize(pModule->pszUunm);
    if (pModule->pszServiceName) {
        size += StringAlignSize(pModule->pszServiceName);
    }

    return size + 4;
}

int FlatCLS(const CLSModule *pSrc, void **ppDest)
{
    int nSize;
    CLSModule *pDest;
    CFlatBuffer buffer;

    nSize = CFlatBuffer::CalcBufferSize(pSrc);
    pDest = (CLSModule *)new char[nSize];
    if (!pDest) _ReturnError (CLSError_OutOfMemory);

    nSize = buffer.Flat((CLSModule *)pSrc, pDest);
    pDest->dwAttribs |= CARAttrib_inheap;

    if (pDest->dwAttribs & CARAttrib_compress) {
        nSize = CompressCLS(pDest);
        if (nSize < 0) {
            delete [] (char *)pDest;
            _Return (nSize);
        }
    }

    *ppDest = (void *)pDest;
    _ReturnOK (nSize);
}
