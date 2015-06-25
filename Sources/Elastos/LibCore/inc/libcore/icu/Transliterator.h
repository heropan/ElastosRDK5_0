
#ifndef __LIBCORE_ICU_TRANSLITERATOR__
#define __LIBCORE_ICU_TRANSLITERATOR__

#include "Object.h"

using Elastos::Core::Object;

namespace Libcore {
namespace ICU {

class Transliterator
    : public Object
    , public ITransliterator
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ const String& id);

protected:
    virtual ~Transliterator();

public:
    CARAPI GetAvailableIDs(
        /* [out, callee] */ ArrayOf<String>** ids);

    CARAPI Transliterate(
        /* [in] */ const String& s,
        /* [out] */ String* rev);

private:
    static CARAPI_(Int64) Create(
        const String& id);

    static CARAPI_(void) Destroy(
        Int64 peer);

    static CARAPI_(String) Transliterate(
        Int64 peer,
        const String& s);

private:
    Int64 mPeer;

};

} // ICU
} // Libcore

#endif // __LIBCORE_ICU_TRANSLITERATOR__