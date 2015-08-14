#ifndef __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Represents a single tab stop on a line.
 */
//public interface
class TabStopSpan
{
public:
    /**
     * The default implementation of TabStopSpan.
     */
    //static
    class Standard
        : public ElRefBase
        , public ITabStopSpan
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);
    public:
        /**
         * Constructor.
         *
         * @param where the offset of the tab stop from the leading margin of
         *        the line
         */
        Standard(
            /* [in] */ Int32 where);

        CARAPI GetTabStop(
            /* [out] */ Int32* ret);

    private:
        Int32 mTab;
    };
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__
