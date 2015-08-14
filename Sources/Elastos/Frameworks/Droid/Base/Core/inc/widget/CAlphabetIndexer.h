
#ifndef __ELASTOS_DROID_WIDGET_CALPHABETINDEXER_H__
#define __ELASTOS_DROID_WIDGET_CALPHABETINDEXER_H__

#include "_CAlphabetIndexer.h"
#include "widget/AlphabetIndexer.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CAlphabetIndexer), public AlphabetIndexer
{
public:

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnChanged();

    CARAPI OnInvalidated();

    CARAPI SetCursor(
        /* [in] */ ICursor * pCursor);

    CARAPI GetSections(
        /* [out, callee] */ ArrayOf<IInterface*> ** ppSections);

    CARAPI GetPositionForSection(
        /* [in] */ Int32 section,
        /* [out] */ Int32 * pPosition);

    CARAPI GetSectionForPosition(
        /* [in] */ Int32 position,
        /* [out] */ Int32 * pSection);

    CARAPI constructor(
        /* [in] */ ICursor * pCursor,
        /* [in] */ Int32 sortedColumnIndex,
        /* [in] */ ICharSequence * pAlphabet);

private:
    // TODO: Add your private member variables here.
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_CALPHABETINDEXER_H__
