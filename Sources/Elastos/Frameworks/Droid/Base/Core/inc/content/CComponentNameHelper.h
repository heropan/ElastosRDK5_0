
#ifndef __CCOMPONENTNAMEHELPER_H__
#define __CCOMPONENTNAMEHELPER_H__

#include "_CComponentNameHelper.h"
#include <ext/frameworkext.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CComponentNameHelper)
{
public:
    /**
     * Recover a ComponentName from a String that was previously created with
     * {@link #flattenToString()}.  It splits the string at the first '/',
     * taking the part before as the package name and the part after as the
     * class name.  As a special convenience (to use, for example, when
     * parsing component names on the command line), if the '/' is immediately
     * followed by a '.' then the final class name will be the concatenation
     * of the package name with the string following the '/'.  Thus
     * "com.foo/.Blah" becomes package="com.foo" class="com.foo.Blah".
     *
     * @param str The String that was returned by flattenToString().
     * @return Returns a new ComponentName containing the package and class
     * names that were encoded in <var>str</var>
     *
     * @see #flattenToString()
     */
    CARAPI UnflattenFromString(
        /* [in] */ const String& str,
        /* [out] */ IComponentName** componentName);

    /**
     * Write a ComponentName to a Parcel, handling null pointers.  Must be
     * read with {@link #readFromParcel(Parcel)}.
     *
     * @param c The ComponentName to be written.
     * @param out The Parcel in which the ComponentName will be placed.
     *
     * @see #readFromParcel(Parcel)
     */
    CARAPI WriteToParcel(
        /* [in] */ IComponentName* cn,
        /* [in] */ IParcel* dest);

    /**
     * Read a ComponentName from a Parcel that was previously written
     * with {@link #writeToParcel(ComponentName, Parcel)}, returning either
     * a null or new object as appropriate.
     *
     * @param in The Parcel from which to read the ComponentName
     * @return Returns a new ComponentName matching the previously written
     * object, or null if a null had been written.
     *
     * @see #writeToParcel(ComponentName, Parcel)
     */
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source,
        /* [out] */ IComponentName** cn);
};

} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CCOMPONENTNAMEHELPER_H__
