
#include "CFilePartHelper.h"
#include "CFilePart.h"
#include "elautoptr.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

ECode CFilePartHelper::GetBoundary(
    /* [out] */ String* boundary)
{
    return CFilePart::GetBoundary(boundary);
}

ECode CFilePartHelper::SendParts(
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
            Part* part = (Part*)(PartBase*)(CFilePart*)(IFilePart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CFilePart::SendParts(outStream, parts, partBoundary);
}

ECode CFilePartHelper::GetLengthOfParts(
    /* [in] */ ArrayOf<IPart *>* iparts,
    /* [out] */ Int64* length)
{
    AutoPtr<ArrayOf<Part*> > parts;
    if (iparts != NULL) {
        Int32 count = iparts->GetLength();
        parts = ArrayOf<Part*>::Alloc(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IPart> ipart = (*iparts)[i];
            Part* part = (Part*)(PartBase*)(CFilePart*)(IFilePart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CFilePart::GetLengthOfParts(parts, length);
}

ECode CFilePartHelper::GetLengthOfParts(
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
            Part* part = (Part*)(PartBase*)(CFilePart*)(IFilePart*)(IPart*)ipart;
            (*parts)[i] = part;
        }
    }
    return CFilePart::GetLengthOfParts(parts, partBoundary, length);
}

}
}
}
}
}
}

