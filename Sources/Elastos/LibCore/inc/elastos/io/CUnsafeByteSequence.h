#ifndef __CUNSAFEBYTESEQUENCE_H__
#define __CUNSAFEBYTESEQUENCE_H__

#include <_CUnsafeByteSequence.h>

namespace Elastos {
namespace IO{

CarClass(CUnsafeByteSequence)
{
public:
    CARAPI constructor(Int32 capacity);

    CARAPI Size(
            /* [out]*/ Int32* size);

    CARAPI Rewind();

    CARAPI Write(
            /* [in] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 length);

    CARAPI WriteEx(
            /* [in] */ Int32 b);

    CARAPI ToByteArray(
            /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI ToString(
            /* [out] */ String* result);
private:
    AutoPtr<ArrayOf<Byte> > mBytes;
    Int32 mCount;
};

} // namespace IO
} // namespace Elastos

#endif
