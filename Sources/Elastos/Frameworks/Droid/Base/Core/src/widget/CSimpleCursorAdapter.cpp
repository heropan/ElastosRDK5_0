#include "widget/CSimpleCursorAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
ILISTADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
ISPINNERADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
IBASEADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
ICURSORADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
IRESOURCECURSORADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)
ISIMPLECURSORADAPTER_METHODS_IMPL(CSimpleCursorAdapter, SimpleCursorAdapter)

PInterface CSimpleCursorAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IAdapter) {
        return (IAdapter*)(IListAdapter*)this;
    }
    return _CSimpleCursorAdapter::Probe(riid);
}

ECode CSimpleCursorAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    return SimpleCursorAdapter::Init(context, layout, c, from, to);
}

ECode CSimpleCursorAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to,
    /* [in] */ Int32 flags)
{
    return SimpleCursorAdapter::Init(context, layout, c, from, to, flags);
}

ECode CSimpleCursorAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter)
    AutoPtr<IFilter> f = SimpleCursorAdapter::GetFilter();
    *filter = f;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

ECode CSimpleCursorAdapter::ConvertToString(
    /* [in] */ ICursor* cursor,
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ICharSequence> seq = SimpleCursorAdapter::ConvertToString(cursor);
    *result = seq;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CSimpleCursorAdapter::RunQueryOnBackgroundThread(
    /* [in] */ ICharSequence* constraint,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    AutoPtr<ICursor> c = SimpleCursorAdapter::RunQueryOnBackgroundThread(constraint);
    *cursor = c;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode CSimpleCursorAdapter::GetCursor(
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    AutoPtr<ICursor> c = SimpleCursorAdapter::GetCursor();
    *cursor = c;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode CSimpleCursorAdapter::ChangeCursor(
    /* [in] */ ICursor* cursor)
{
    return SimpleCursorAdapter::ChangeCursor(cursor);
}


}// namespace Elastos
}// namespace Droid
}// namespace Widget
