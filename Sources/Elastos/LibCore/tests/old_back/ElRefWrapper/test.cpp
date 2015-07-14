
#include <elastos.h>
#include "ElRefWrapper.h"
#include <Elastos.CoreLibrary.h>
#include <elautoptr.h>
#include <stdio.h>

using namespace Elastos;


void testRefBase()
{
    AutoPtr<IByteArrayOutputStream> out;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&out);

    RefWrapper<IByteArrayOutputStream>* refObj = new RefWrapper<IByteArrayOutputStream>(out.Get());

    refObj->Get()->Reset();

    StrongPtr< RefWrapper<IByteArrayOutputStream> > spObj = refObj;
    WeakPtr< RefWrapper<IByteArrayOutputStream> > wpObj(spObj);

    StrongPtr< RefWrapper<IByteArrayOutputStream> > spObj2 = wpObj.Promote();

    spObj2->Get()->Reset();


    AutoPtr<ICharArrayWriter> writer;
    CCharArrayWriter::New((ICharArrayWriter**)&writer);

    RefWrapper<ICharArrayWriter>* refObj2 = new RefWrapper<ICharArrayWriter>(writer.Get());

    refObj2->Get()->Reset();

    StrongPtr< RefWrapper<ICharArrayWriter> > spObj3 = refObj2;
    WeakPtr< RefWrapper<ICharArrayWriter> > wpObj2(spObj3);

    StrongPtr< RefWrapper<ICharArrayWriter> > spObj4 = wpObj2.Promote();

    spObj4->Get()->Reset();


    RefWrapper<ICharArrayWriter>* refObj3 = new RefWrapper<ICharArrayWriter>();

    StrongPtr< RefWrapper<ICharArrayWriter> > spObj5 = refObj3;
    WeakPtr< RefWrapper<ICharArrayWriter> > wpObj3(spObj5);

    StrongPtr< RefWrapper<ICharArrayWriter> > spObj6 = wpObj3.Promote();
}

int main(int argc, char *argv[])
{
    printf("==== call testElStrongRefBase ====\n");
    testRefBase();
    printf("==== call testElStrongRefBase end ====\n");

    return 0;
}
