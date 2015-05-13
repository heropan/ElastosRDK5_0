
#include <elastos.h>
#include "ElRefWrapper.h"
#include <Elastos.Core.h>
#include <elautoptr.h>
#include <stdio.h>

using namespace Elastos;


void testRefBase()
{
    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);

    RefWrapper* refObj = new RefWrapper(out.Get());

    IByteArrayOutputStream::Probe(refObj->Get())->Reset();

    StrongPtr<RefWrapper> spObj = refObj;
    WeakPtr<RefWrapper> wpObj(spObj);

    StrongPtr<RefWrapper> spObj2 = wpObj.Promote();

    IByteArrayOutputStream::Probe(spObj2->Get())->Reset();


    AutoPtr<ICharArrayWriter> writer;
    CCharArrayWriter::New((ICharArrayWriter**)&writer);

    RefWrapper* refObj2 = new RefWrapper(writer.Get());

    ICharArrayWriter::Probe(refObj->Get())->Reset();

    StrongPtr<RefWrapper> spObj3 = refObj2;
    WeakPtr<RefWrapper> wpObj2(spObj3);

    StrongPtr<RefWrapper> spObj4 = wpObj2.Promote();

    ICharArrayWriter::Probe(spObj2->Get())->Reset();
}

int main(int argc, char *argv[])
{
    printf("==== call testElStrongRefBase ====\n");
    testRefBase();
    printf("==== call testElStrongRefBase end ====\n");

    return 0;
}
