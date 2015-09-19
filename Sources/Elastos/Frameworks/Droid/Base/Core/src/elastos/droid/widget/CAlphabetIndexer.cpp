
#include "widget/CAlphabetIndexer.h"

namespace Elastos {
namespace Droid {
namespace Widget {

PInterface CAlphabetIndexer::Probe(
        /* [in] */ REIID riid)
{
    return _CAlphabetIndexer::Probe(riid);
}

ECode CAlphabetIndexer::OnChanged()
{
    return AlphabetIndexer::OnChanged();
}

ECode CAlphabetIndexer::OnInvalidated()
{
    return AlphabetIndexer::OnInvalidated();
}

ECode CAlphabetIndexer::SetCursor(
    /* [in] */ ICursor * pCursor)
{
    return AlphabetIndexer::SetCursor(pCursor);
}

ECode CAlphabetIndexer::GetSections(
    /* [out, callee] */ ArrayOf<IInterface*> ** ppSections)
{
    VALIDATE_NOT_NULL(ppSections);
    AutoPtr<ArrayOf<IInterface*> > temp = AlphabetIndexer::GetSections();
    *ppSections = temp;
    REFCOUNT_ADD(*ppSections);
    return NOERROR;
}

ECode CAlphabetIndexer::GetPositionForSection(
    /* [in] */ Int32 section,
    /* [out] */ Int32 * pPosition)
{
    VALIDATE_NOT_NULL(pPosition);
    *pPosition = AlphabetIndexer::GetPositionForSection(section);
    return NOERROR;
}

ECode CAlphabetIndexer::GetSectionForPosition(
    /* [in] */ Int32 position,
    /* [out] */ Int32 * pSection)
{
    VALIDATE_NOT_NULL(pSection);
    *pSection = AlphabetIndexer::GetSectionForPosition(position);
    return NOERROR;
}

ECode CAlphabetIndexer::constructor(
    /* [in] */ ICursor * pCursor,
    /* [in] */ Int32 sortedColumnIndex,
    /* [in] */ ICharSequence * pAlphabet)
{
    return AlphabetIndexer::Init(pCursor, sortedColumnIndex, pAlphabet);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
