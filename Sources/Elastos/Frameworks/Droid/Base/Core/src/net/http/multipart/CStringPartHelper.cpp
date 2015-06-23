
#include "CStringPartHelper.h"
#include "CStringPart.h"
#include "elautoptr.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

ECode CStringPartHelper::GetBoundary(
    /* [out] */ String* boundary)
{
    return CStringPart::GetBoundary(boundary);
}

ECode CStringPartHelper::SendParts(
    /* [in] */ Elastos::IO::IOutputStream* outStream,
    /* [in] */ ArrayOf<IPart *>* iparts,
    /* [in] */ ArrayOf<Byte>* partBoundary)
{
    AutoPtr<ArrayOf<Part*> > parts;
    if (iparts != NULL) {
        Int32 count = iparts->GetLength();
        parts = ArrayOf<Part*>::Alloc(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IPart> ipart = (*iparts)[i];
            Part* part = (Part*)(PartBase*)(CStringPart*)(IStringPart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CStringPart::SendParts(outStream, parts, partBoundary);
}

ECode CStringPartHelper::GetLengthOfParts(
    /* [in] */ ArrayOf<IPart *>* iparts,
    /* [out] */ Int64* length)
{
    AutoPtr<ArrayOf<Part*> > parts;
    if (iparts != NULL) {
        Int32 count = iparts->GetLength();
        parts = ArrayOf<Part*>::Alloc(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IPart> ipart = (*iparts)[i];
            Part* part = (Part*)(PartBase*)(CStringPart*)(IStringPart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CStringPart::GetLengthOfParts(parts, length);
}

ECode CStringPartHelper::GetLengthOfParts(
    /* [in] */ ArrayOf<IPart *>* iparts,
    /* [in] */ ArrayOf<Byte>* partBoundary,
    /* [out] */ Int64* length)
{
    AutoPtr<ArrayOf<Part*> > parts;
    if (iparts != NULL) {
        Int32 count = iparts->GetLength();
        parts = ArrayOf<Part*>::Alloc(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IPart> ipart = (*iparts)[i];
            Part* part = (Part*)(PartBase*)(CStringPart*)(IStringPart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CStringPart::GetLengthOfParts(parts, partBoundary, length);
}

}
}
}
}
}
}

