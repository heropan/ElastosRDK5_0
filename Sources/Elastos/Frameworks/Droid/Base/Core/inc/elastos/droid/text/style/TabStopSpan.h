#ifndef __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__

#include "Elastos/droid/text/style/ParagraphStyle.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Represents a single tab stop on a line.
 */
//public interface
class TabStopSpan
    : public ParagraphStyle
    , public ITabStopSpan
{
public:
};

/**
 * The default implementation of TabStopSpan.
 */
//static
class Standard
    : public TabStopSpan
    , public ITabStopSpanStandard
{
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

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_TabStopSpan_H__
