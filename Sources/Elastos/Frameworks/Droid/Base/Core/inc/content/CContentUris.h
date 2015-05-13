
#ifndef __CCONTENTURIS_H__
#define __CCONTENTURIS_H__

#include "_CContentUris.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::IUriBuilder;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentUris)
{
public:
    /**
     * Converts the last path segment to a long.
     *
     * <p>This supports a common convention for content URIs where an ID is
     * stored in the last segment.
     *
     * @throws UnsupportedOperationException if this isn't a hierarchical URI
     * @throws NumberFormatException if the last segment isn't a number
     *
     * @return the long conversion of the last segment or -1 if the path is
     *  empty
     */
    CARAPI ParseId(
        /* [in] */ IUri* contentUri,
        /* [out] */ Int64* id);

    /**
     * Appends the given ID to the end of the path.
     *
     * @param builder to append the ID to
     * @param id to append
     *
     * @return the given builder
     */
    CARAPI AppendId(
        /* [in] */ IUriBuilder* builder,
        /* [in] */ Int64 id);

    /**
     * Appends the given ID to the end of the path.
     *
     * @param contentUri to start with
     * @param id to append
     *
     * @return a new URI with the given ID appended to the end of the path
     */
    CARAPI WithAppendedId(
        /* [in] */ IUri* contentUri,
        /* [in] */ Int64 id,
        /* [out] */ IUri** newUri);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CCONTENTURIS_H__
