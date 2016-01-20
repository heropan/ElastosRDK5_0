#ifndef __ELASTOS_DROID_SERVER_RANDOM_BLOCK_H__
#define __ELASTOS_DROID_SERVER_RANDOM_BLOCK_H__

#include "_Elastos.Droid.Server.h"
#include <elastos/core/Object.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IDataOutput;
using Elastos::IO::IRandomAccessFile;
using Elastos::IO::ICloseable;

namespace Elastos {
namespace Droid {
namespace Server {

/**
 * A block of 512 random {@code byte}s.
 */
class RandomBlock
    : public Object
{
public:
    static CARAPI FromFile(
        /* [in] */ const String& filename,
        /* [out] */ RandomBlock** block);

    CARAPI ToFile(
        /* [in] */ const String& filename,
        /* [in] */ Boolean sync);

private:
    static CARAPI FromStream(
        /* [in] */ IInputStream* in,
        /* [out] */ RandomBlock** block);

    static void TruncateIfPossible(
        /* [in] */ IRandomAccessFile* f);

    CARAPI ToDataOut(
        /* [in] */ IDataOutput* out);

    static void Close(
        /* [in] */ ICloseable* c);

private:
    RandomBlock();

private:
    static const String TAG;// = "RandomBlock";
    static const Boolean DEBUG;// = false;
    static const Int32 BLOCK_SIZE;// = 512;
    AutoPtr<ArrayOf<Byte> > mBlock;// = new byte[BLOCK_SIZE];
};


} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_RANDOM_BLOCK_H__
