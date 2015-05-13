
#ifndef __CCLIPDESCRIPTIONHELPER_H__
#define __CCLIPDESCRIPTIONHELPER_H__

#include "_CClipDescriptionHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CClipDescriptionHelper)
{
public:
    /**
     * Helper to compare two MIME types, where one may be a pattern.
     * @param concreteType A fully-specified MIME type.
     * @param desiredType A desired MIME type that may be a pattern such as * \/ *.
     * @return Returns true if the two MIME types match.
     */
    CARAPI CompareMimeTypes(
        /* [in] */ const String& concreteType,
        /* [in] */ const String& desiredType,
        /* [out] */ Boolean* isMatch);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CCLIPDESCRIPTIONHELPER_H__
