
#include "ext/frameworkext.h"
#include "text/CSimpleStringSplitter.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::EIID_IIterator;

namespace Elastos {
namespace Droid {
namespace Text {

ECode CSimpleStringSplitter::constructor(
    /*  [in] */ Char32 delimiter)
{
    mDelimiter = delimiter;
    return NOERROR;
}

ECode CSimpleStringSplitter::SetString(
    /*  [in] */ const String& string)
{
    mString = string;
    mPosition = 0;
    mLength = mString.GetLength();
    return NOERROR;
}

ECode CSimpleStringSplitter::GetIterator(
   /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    *it = THIS_PROBE(IIterator);
    INTERFACE_ADDREF(*it);
    return NOERROR;
}

ECode CSimpleStringSplitter::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPosition < mLength;
    return NOERROR;
}

ECode CSimpleStringSplitter::NextString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    Int32 end = mString.IndexOf(mDelimiter, mPosition);
    if (end == -1) {
        end = mLength;
    }

    if (mPosition > end) {
        *str = String(NULL);
        return E_STRING_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    String nextString = mString.Substring(mPosition, end);
    mPosition = end + 1; // Skip the delimiter.

    *str = nextString;
    return NOERROR;
}

ECode CSimpleStringSplitter::Next(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    String str;
    NextString(&str);

    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(str, (ICharSequence**)&seq);
    *obj = seq;
    INTERFACE_ADDREF(*obj);
    return NOERROR;
}

ECode CSimpleStringSplitter::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
