
#ifndef __NEWINSTANCE_H__
#define __NEWINSTANCE_H__

#include <elastos.h>
#include "cmdef.h"
#include "_CAttributesImpl.h"

using Elastos::Core::IClassLoader;

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

class NewInstanceHelper {
public:
    /**
     * Creates a new instance of the specified class name
     *
     * Package private so this code is not exposed at the API level.
     */
    static AutoPtr<IInterface> NewInstance (
        IClassLoader* classLoader,
        const String& className);

    /**
     * Figure out which ClassLoader to use.  For JDK 1.2 and later use
     * the context ClassLoader.
     */
    static AutoPtr<IClassLoader> GetClassLoader ();

};

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __NEWINSTANCE_H__
