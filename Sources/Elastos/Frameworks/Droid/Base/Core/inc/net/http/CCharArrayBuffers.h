
#ifndef __CCHARARRAYBUFFERS_H__
#define __CCHARARRAYBUFFERS_H__

#include "_CCharArrayBuffers.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Utility methods for working on CharArrayBuffers.
 * 
 * {@hide}
 */
CarClass(CCharArrayBuffers)
{
public:
    CARAPI constructor();

private:
    static const Char8 UPPERCASEADDON;
};

}
}
}
}

#endif // __CCHARARRAYBUFFERS_H__
