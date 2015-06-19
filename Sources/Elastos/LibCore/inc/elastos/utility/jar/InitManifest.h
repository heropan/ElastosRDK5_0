
#include <elautoptr.h>
#include "CManifest.h"
#include <elastos/utility/etl/HashMap.h>
#include "Object.h"

using Elastos::IO::IUnsafeByteSequence;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Utility {
namespace Jar {

class InitManifest
    : public Object
{
public:
    InitManifest(
        /* in */ ArrayOf<Byte>* buf,
        /* in */ IAttributes* main,
        /* in */ IName* ver);

    CARAPI InitEntries(
        /* [in, out] */ IMap* entries,
        /* [in, out] */ IMap* chunks);

    CARAPI_(Int32) GetPos();

private:
    /**
    * Read a single line from the manifest buffer.
    */
    CARAPI ReadHeader(
        /* [out] */ Boolean* result);

    CARAPI ReadName();

    CARAPI ReadValue();

private:
    AutoPtr<ArrayOf<Byte> > mBuf;
    Int32 mPos;
    AutoPtr<IName> mName;
    String mNameStr;
    AutoPtr<ICharSequence> mValue;
    AutoPtr<IUnsafeByteSequence> mValueBuffer;
    Int32 mConsecutiveLineBreaks;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos
