#ifndef __AndroidBidi_H__
#define __AndroidBidi_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * Access the ICU bidi implementation.
 * @hide
 */
/* package */
class AndroidBidi {
public:
    static CARAPI_(Int32) Bidi(
        /* [in] */ Int32 dir,
        /* [in] */ ArrayOf<Char32>* chs,
        /* [in] */ ArrayOf<Byte>* chInfo,
        /* [in] */ Int32 n,
        /* [in] */ Boolean haveInfo);

    /**
     * Returns run direction information for a line within a paragraph.
     *
     * @param dir base line direction, either Layout.DIR_LEFT_TO_RIGHT or
     *     Layout.DIR_RIGHT_TO_LEFT
     * @param levels levels as returned from {@link #bidi}
     * @param lstart start of the line in the levels array
     * @param chars the character array (used to determine whitespace)
     * @param cstart the start of the line in the chars array
     * @param len the length of the line
     * @return the directions
     */
    static CARAPI_(AutoPtr<ILayoutDirections>) Directions(
        /* [in] */ Int32 dir,
        /* [in] */ ArrayOf<Byte>* levels,
        /* [in] */ Int32 lstart,
        /* [in] */ ArrayOf<Char32>* chars,
        /* [in] */ Int32 cstart,
        /* [in] */ Int32 len);
private:
    //native
    static CARAPI_(Int32) RunBidi(
        /* [in] */ Int32 dir,
        /* [in] */ ArrayOf<Char32>* chs,
        /* [in] */ ArrayOf<Byte>* chInfo,
        /* [in] */ Int32 n,
        /* [in] */ Boolean haveInfo);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__AndroidBidi_H__
