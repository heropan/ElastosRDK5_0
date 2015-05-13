//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

//#ifdef _win32
#include <elatypes.h>
//#else
//#include <elastos.h>
//#endif

#include "clstype.h"
#include "abrgcls.h"

#define RELOC(p, t, b)    p = (t)((int)(p) + b)

void RelocAbridgedCLS(void *pvAbrgCLS)
{
    size_t n, m, nBase;
    AbridgedModuleInfo *pInfo;
    AbridgedClass *pClass;
    AbridgedInterface *pIntf;

    pInfo = (AbridgedModuleInfo *)pvAbrgCLS;
    nBase = (int)pvAbrgCLS;

    if ((int)(pInfo->pClasses) > nBase) return; // has relocated

    RELOC(pInfo->pClasses, AbridgedClass *, nBase);
    RELOC(pInfo->pInterfaces, AbridgedInterface *, nBase);

    for (n = 0; n < pInfo->cClasses; n++) {
        pClass = &pInfo->pClasses[n];
        RELOC(pClass->ppInterfaces, AbridgedInterface **, nBase);

        for (m = 0; m < pClass->cInterfaces; m++) {
            RELOC(pClass->ppInterfaces[m], AbridgedInterface *, nBase);
        }
        RELOC(pClass->pszUunm, char *, nBase);
    }

    for (n = 0; n < pInfo->cInterfaces; n++) {
        pIntf = &pInfo->pInterfaces[n];
        RELOC(pIntf->pMethods, AbridgedMethod *, nBase);
        for (m = 0; m < pIntf->cMethods; m++) {
            RELOC(pIntf->pMethods[m].pParams, AbridgedParamsInfo *, nBase);
        }
    }
}
