#ifndef __FACTORYFINDER_H__
#define __FACTORYFINDER_H__

#include "Object.h"

using Elastos::Core::Object;

namespace Elastosx {
namespace Xml {
namespace Datatype {

class FactoryFinder : public Object
{
public:

private:
    FactoryFinder();

    static void DebugPrint();

    static AutoPtr<IClassLoader> FindClassLoader();

    static AutoPtr<IInterface> NewInstance();

    static AutoPtr<IInterface> Find(
        /* [in] */ String factoryId,
        /* [in] */ String fallbackClassName);

private:
    /** <p>Name of class to display in output messages.</p> */
    static const String sCLASS_NAME;

    /** <p>Debug flag to trace loading process.</p> */
    static Boolean sDebug;

    /** <p>Cache properties for performance.</p> */
    static AutoPtr<IProperties> sCacheProps;

    /** <p>First time requires initialization overhead.</p> */
    static Boolean firstTime;

    /** Default columns per line. */
    static const Int32 DEFAULT_LINE_LENGTH;
};

} // namespace Datatype
} // namespace Xml
} // namespace Elastosx

#endif