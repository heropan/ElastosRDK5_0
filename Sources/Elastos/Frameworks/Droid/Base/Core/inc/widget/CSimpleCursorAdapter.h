#ifndef __CSIMPLECURSORADAPTER_H__
#define __CSIMPLECURSORADAPTER_H__

#include "_CSimpleCursorAdapter.h"
#include "widget/AdapterMacro.h"
#include "widget/SimpleCursorAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSimpleCursorAdapter), public SimpleCursorAdapter
{
public:
    IADAPTER_METHODS_DECL()
    ILISTADAPTER_METHODS_DECL()
    ISPINNERADAPTER_METHODS_DECL()
    IBASEADAPTER_METHODS_DECL()
    ICURSORADAPTER_METHODS_DECL()
    IRESOURCECURSORADAPTER_METHODS_DECL()
    ISIMPLECURSORADAPTER_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layout,
        /* [in] */ ICursor* c,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layout,
        /* [in] */ ICursor* c,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to,
        /* [in] */ Int32 flags);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);

    CARAPI ConvertToString(
        /* [in] */ ICursor* cursor,
        /* [out] */ ICharSequence** charSequence);

    CARAPI RunQueryOnBackgroundThread(
        /* [in] */ ICharSequence* constraint,
        /* [out] */ ICursor** cursor);

    CARAPI GetCursor(
        /* [out] */ ICursor** cursor);

    CARAPI ChangeCursor(
        /* [in] */ ICursor* cursor);
};


}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif // __CSIMPLECURSORADAPTER_H__
