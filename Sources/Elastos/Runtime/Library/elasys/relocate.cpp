//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elatypes.h>
#include <string.h>
#include <clsinfo.h>

_ELASTOS_NAMESPACE_USING

CAR_INLINE void RelocateUnalignedPtr(
    /* [in] */ void *ptr,
    /* [in] */ size_t offset)
{
#ifdef _x86
    *(size_t *)ptr += offset;
#else
    union
    {
        size_t  s;
        byte_t  bytes[4];
    } u;
    u.bytes[0] = ((byte_t *)ptr)[0];
    u.bytes[1] = ((byte_t *)ptr)[1];
    u.bytes[2] = ((byte_t *)ptr)[2];
    u.bytes[3] = ((byte_t *)ptr)[3];

    u.s += offset;

    ((byte_t *)ptr)[0] = u.bytes[0];
    ((byte_t *)ptr)[1] = u.bytes[1];
    ((byte_t *)ptr)[2] = u.bytes[2];
    ((byte_t *)ptr)[3] = u.bytes[3];
#endif
}

void RelocateModuleInfo(
    /* [in] */ CIModuleInfo *srcModInfo,
    /* [out] */ CIModuleInfo *destModInfo)
{
    CIClassInfo * classInfo;
    CIInterfaceInfo ** interfaceInfos;
    CIInterfaceInfo * interfaceInfo;
    CIMethodInfo * methodInfo;

    memcpy(destModInfo, srcModInfo, srcModInfo->totalSize);

    destModInfo->interfaces = (CIInterfaceInfo*)((Byte *)destModInfo +
        (UInt32)destModInfo->interfaces);
    interfaceInfo = destModInfo->interfaces;
    for (Int32 i = 0; i < destModInfo->interfaceNum; i++) {
        interfaceInfo[i].methods = (CIMethodInfo *) \
              ((Byte *)destModInfo + (UInt32)interfaceInfo[i].methods);
        methodInfo = interfaceInfo[i].methods;
        for (Int32 j = 0; j < interfaceInfo[i].methodNumMinus4; j++) {
            methodInfo[j].params = (CIBaseType*) \
                 ((Byte *)destModInfo + (UInt32)methodInfo[j].params);
        }
    }

    destModInfo->classes = (CIClassInfo *)((Byte *)destModInfo +
       (UInt32)destModInfo->classes);
    classInfo = destModInfo->classes;
    for (Int32 i = 0; i < destModInfo->classNum; i++) {
        classInfo[i].interfaces = (CIInterfaceInfo **) \
                ((Byte *)destModInfo + (Int32)(classInfo[i].interfaces));
        interfaceInfos = classInfo[i].interfaces;
        for (Int32 j = 0; j < classInfo[i].interfaceNum; j++) {
            RelocateUnalignedPtr(interfaceInfos + j, (size_t)destModInfo);
        }
        classInfo[i].pszUunm =
                (char*)((Byte *)destModInfo + (UInt32)classInfo[i].pszUunm);
    }
}

CAR_INLINE void  FlatUnalignedPtr(
    /* [in] */ void *ptr,
    /* [in] */ size_t offset)
{
#ifdef _x86
    *(size_t *)ptr -= offset;
#elif defined(_arm)
    union
    {
        size_t  s;
        byte_t  bytes[4];
    } u;
    u.bytes[0] = ((byte_t *)ptr)[0];
    u.bytes[1] = ((byte_t *)ptr)[1];
    u.bytes[2] = ((byte_t *)ptr)[2];
    u.bytes[3] = ((byte_t *)ptr)[3];

    u.s -= offset;

    ((byte_t *)ptr)[0] = u.bytes[0];
    ((byte_t *)ptr)[1] = u.bytes[1];
    ((byte_t *)ptr)[2] = u.bytes[2];
    ((byte_t *)ptr)[3] = u.bytes[3];
#else
#error unsupported architecure.
#endif
}

void FlatModuleInfo(
    /* [in] */ CIModuleInfo *srcModInfo,
    /* [out] */ CIModuleInfo *destModInfo)
{
    Int32 i, j;
    CIClassInfo * classInfo;
    CIInterfaceInfo ** interfaceInfos;
    CIInterfaceInfo * interfaceInfo;
    CIMethodInfo * methodInfo;

    memcpy(destModInfo, srcModInfo, srcModInfo->totalSize);
    destModInfo->interfaces = (CIInterfaceInfo*) \
            ((Byte*)destModInfo->interfaces - (UInt32)srcModInfo);
    interfaceInfo = (CIInterfaceInfo*)((Byte*) destModInfo->interfaces + \
                      (UInt32)destModInfo);

    for (i = 0; i < destModInfo->interfaceNum; i++) {
        interfaceInfo[i].methods = (CIMethodInfo *) \
              ((Byte*)interfaceInfo[i].methods - (UInt32)srcModInfo);

        methodInfo = (CIMethodInfo*)((Byte*)interfaceInfo[i].methods + \
                            (UInt32)destModInfo);
        for (j = 0; j < interfaceInfo[i].methodNumMinus4; j++) {
            methodInfo[j].params = (CIBaseType*) \
                 ((Byte*)methodInfo[j].params - (UInt32)srcModInfo);
        }
    }

    destModInfo->classes = (CIClassInfo*) \
            ((Byte*)destModInfo->classes - (UInt32)srcModInfo);

    classInfo = (CIClassInfo*)((Byte*)destModInfo->classes + (UInt32)destModInfo);
    for (i = 0; i < destModInfo->classNum; i++) {
        classInfo[i].interfaces = (CIInterfaceInfo**) \
                ((Byte*)classInfo[i].interfaces - (UInt32)srcModInfo);

        interfaceInfos = (CIInterfaceInfo**)((Byte*)classInfo[i].interfaces + \
                (UInt32)destModInfo);
        for (j = 0; j < classInfo[i].interfaceNum; j++) {
            FlatUnalignedPtr(interfaceInfos + j, (size_t)srcModInfo);
        }
        classInfo[i].pszUunm =  \
            (char*)((Byte*)classInfo[i].pszUunm - (UInt32)srcModInfo);
    }
}
