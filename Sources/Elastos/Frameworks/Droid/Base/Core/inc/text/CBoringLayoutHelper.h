
#ifndef __CBORINGLAYOUTHELPER_H__
#define __CBORINGLAYOUTHELPER_H__

#include "_CBoringLayoutHelper.h"
#include "text/LayoutMacro.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CBoringLayoutHelper)
{
    ILAYOUTHELPER_METHODS_DECL()

    CARAPI Make(
        /*[in]*/ ICharSequence* source,
        /*[in]*/ ITextPaint* paint,
        /*[in]*/ Int32 outerwidth,
        /*[in]*/ LayoutAlignment align,
        /*[in]*/ Float spacingmult,
        /*[in]*/ Float spacingadd,
        /*[in]*/ IBoringLayoutMetrics* metrics,
        /*[in]*/ Boolean includepad,
        /*[out]*/ IBoringLayout** ret);

    CARAPI MakeEx(
        /*[in]*/ ICharSequence* source,
        /*[in]*/ ITextPaint* paint,
        /*[in]*/ Int32 outerwidth,
        /*[in]*/ LayoutAlignment align,
        /*[in]*/ Float spacingmult,
        /*[in]*/ Float spacingadd,
        /*[in]*/ IBoringLayoutMetrics* metrics,
        /*[in]*/ Boolean includepad,
        /*[in]*/ TextUtilsTruncateAt ellipsize,
        /*[in]*/ Int32 ellipsizedWidth,
        /*[out]*/ IBoringLayout** ret);

    CARAPI IsBoring(
        /*[in]*/ ICharSequence* text,
        /*[in]*/ ITextPaint* paint,
        /*[out]*/ IBoringLayoutMetrics** ret);

    CARAPI IsBoringEx(
        /*[in]*/ ICharSequence* text,
        /*[in]*/ ITextPaint* paint,
        /*[in]*/ ITextDirectionHeuristic* textDir,
        /*[out]*/ IBoringLayoutMetrics** ret);

    CARAPI IsBoringEx2(
        /*[in]*/ ICharSequence* text,
        /*[in]*/ ITextPaint* paint,
        /*[in]*/ IBoringLayoutMetrics* metrics,
        /*[out]*/ IBoringLayoutMetrics** ret);

    CARAPI IsBoringEx3(
        /*[in]*/ ICharSequence* text,
        /*[in]*/ ITextPaint* paint,
        /*[in]*/ ITextDirectionHeuristic* textDir,
        /*[in]*/ IBoringLayoutMetrics* metrics,
        /*[out]*/ IBoringLayoutMetrics** ret);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif//__CBORINGLAYOUTHELPER_H__