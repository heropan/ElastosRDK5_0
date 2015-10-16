
#include "elastos/droid/widget/SimpleCursorAdapter.h"
#include <elastos/StringToIntegral.h>
#include "elastos/droid/net/CUriHelper.h"

using Elastos::Core::StringToIntegral;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Widget {

SimpleCursorAdapter::SimpleCursorAdapter()
    : mStringConversionColumn(-1)
{
}

SimpleCursorAdapter::SimpleCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
    : ResourceCursorAdapter(context, layout, c)
    , mStringConversionColumn(-1)
 {
    InitImpl(c, from, to);
}

SimpleCursorAdapter::SimpleCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to,
    /* [in] */ Int32 flags)
    : ResourceCursorAdapter(context, layout, c, flags)
    , mStringConversionColumn(-1)
{
    InitImpl(c, from, to);
}

ECode SimpleCursorAdapter::InitImpl(
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    VALIDATE_NOT_NULL(from);
    VALIDATE_NOT_NULL(to);

    mTo = to->Clone();
    mOriginalFrom = from->Clone();
    return FindColumns(c, from);
}

ECode SimpleCursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    ResourceCursorAdapter::Init(context, layout, c);
    return InitImpl(c, from, to);
}

ECode SimpleCursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to,
    /* [in] */ Int32 flags)
{
    ResourceCursorAdapter::Init(context, layout, c, flags);
    return InitImpl(c, from, to);
}

ECode SimpleCursorAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor)
{
    AutoPtr<ISimpleCursorAdapterViewBinder> binder = mViewBinder;
    Int32 count = mTo->GetLength();
    AutoPtr<ArrayOf<Int32> > from = mFrom;
    AutoPtr<ArrayOf<Int32> > to = mTo;

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> v;
        view->FindViewById((*to)[i], (IView**)&v);
        if (v != NULL) {
            Boolean bound = FALSE;
            if (binder != NULL) {
                binder->SetViewValue(v, cursor, (*from)[i], &bound);
            }

            if (!bound) {
                String text;
                cursor->GetString((*from)[i], &text);
                if (text.IsNull()) {
                    text = String("");
                }

                if (ITextView::Probe(v.Get())) {
                    SetViewText(ITextView::Probe(v.Get()), text);
                }
                else if (IImageView::Probe(v.Get())) {
                    SetViewImage(IImageView::Probe(v.Get()), text);
                }
                else {
                    // throw new IllegalStateException(v.getClass().getName() + " is not a " +
                    //         " view that can be bounds by this SimpleCursorAdapter");
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
            }
        }
    }
    return NOERROR;
}

AutoPtr<ISimpleCursorAdapterViewBinder> SimpleCursorAdapter::GetViewBinder()
{
    return mViewBinder;
}

ECode SimpleCursorAdapter::SetViewBinder(
    /* [in] */ ISimpleCursorAdapterViewBinder* viewBinder)
{
    mViewBinder = viewBinder;
    return NOERROR;
}

ECode SimpleCursorAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ const String& value)
{
    VALIDATE_NOT_NULL(v);

    Int32 intVal;
    ECode ec = StringToIntegral::Parse(value, &intVal);
    if (SUCCEEDED(ec)) {
        return v->SetImageResource(intVal);
    }
    else {
        // NumberFormatException
        AutoPtr<IUriHelper> helper;
        CUriHelper::AcquireSingleton((IUriHelper**)&helper);
        AutoPtr<IUri> uri;
        helper->Parse(value, (IUri**)&uri);
        return v->SetImageURI(uri);
    }
    return NOERROR;
}

ECode SimpleCursorAdapter::SetViewText(
    /* [in] */ ITextView* v,
    /* [in] */ const String& text)
{
    VALIDATE_NOT_NULL(v);

    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(text, (ICharSequence**)&seq);
    return v->SetText(seq);
}

Int32 SimpleCursorAdapter::GetStringConversionColumn()
{
    return mStringConversionColumn;
}

ECode SimpleCursorAdapter::SetStringConversionColumn(
    /* [in] */ Int32 stringConversionColumn)
{
    mStringConversionColumn = stringConversionColumn;
    return NOERROR;
}

AutoPtr<ICursorToStringConverter> SimpleCursorAdapter::GetCursorToStringConverter()
{
    return mCursorToStringConverter;
}

ECode SimpleCursorAdapter::SetCursorToStringConverter(
    /* [in] */ ICursorToStringConverter* cursorToStringConverter)
{
    mCursorToStringConverter = cursorToStringConverter;
    return NOERROR;
}

AutoPtr<ICharSequence> SimpleCursorAdapter::ConvertToString(
    /* [in] */ ICursor* cursor)
{
    AutoPtr<ICharSequence> seq;
    if (mCursorToStringConverter != NULL) {
        mCursorToStringConverter->ConvertToString(cursor, (ICharSequence**)&seq);
        return seq;
    }
    else if (mStringConversionColumn > -1) {
        String str;
        cursor->GetString(mStringConversionColumn, &str);
        CStringWrapper::New(str, (ICharSequence**)&seq);
        return seq;
    }

    return ResourceCursorAdapter::ConvertToString(cursor);
}

ECode SimpleCursorAdapter::FindColumns(
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from)
{
    if (c != NULL && from != NULL) {
        Int32 count = from->GetLength();
        if (mFrom == NULL || mFrom->GetLength() != count) {
            mFrom = NULL;
            mFrom = ArrayOf<Int32>::Alloc(count);
        }

        for (Int32 i = 0; i < count; i++) {
            c->GetColumnIndexOrThrow((*from)[i], &(*mFrom)[i]);
        }
    }
    else {
        mFrom = NULL;
    }
    return NOERROR;
}

AutoPtr<ICursor> SimpleCursorAdapter::SwapCursor(
    /* [in] */ ICursor* c)
{
    // super.swapCursor() will notify observers before we have
    // a valid mapping, make sure we have a mapping before this
    // happens
    FindColumns(c, mOriginalFrom);
    return ResourceCursorAdapter::SwapCursor(c);
}

ECode SimpleCursorAdapter::ChangeCursorAndColumns(
    /* [in] */ ICursor* c,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    mOriginalFrom = NULL;
    mTo = NULL;
    if (from)
        mOriginalFrom = from->Clone();
    if (to)
        mTo = to->Clone();

    // super.changeCursor() will notify observers before we have
    // a valid mapping, make sure we have a mapping before this
    // happens
    FindColumns(c, mOriginalFrom);
    return ResourceCursorAdapter::ChangeCursor(c);
}


}// namespace Elastos
}// namespace Droid
}// namespace Widget

