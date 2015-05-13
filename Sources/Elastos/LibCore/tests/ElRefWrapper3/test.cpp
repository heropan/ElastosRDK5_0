
#include <elastos.h>
#include "ElRefWrapper.h"
#include <Elastos.Core.h>
#include <elautoptr.h>
#include <stdio.h>

using namespace Elastos;


void f1(StrongPtr<IByteArrayOutputStream>& a1)
{
    a1->Reset();
}

void testRefBase()
{
    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);

    StrongPtr<IByteArrayOutputStream> spObj = out.Get();
    f1(spObj);

    WeakPtr<IByteArrayOutputStream> wpObj(spObj);

    StrongPtr<IByteArrayOutputStream> spObj2 = wpObj.Promote();
    spObj2->Reset();
}

int main(int argc, char *argv[])
{
    printf("==== call testElStrongRefBase ====\n");
    testRefBase();
    printf("==== call testElStrongRefBase end ====\n");

    return 0;
}
