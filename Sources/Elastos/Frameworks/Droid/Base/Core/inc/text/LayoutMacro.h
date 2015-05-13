#ifndef __LAYOUTMACRO_H__
#define __LAYOUTMACRO_H__

#define ILAYOUT_METHODS_DECL()                     \
    CARAPI Draw(                                   \
        /* [in] */ ICanvas* c);                    \
                                                   \
    CARAPI DrawEx(                                 \
        /* [in] */ ICanvas* canvas,                \
        /* [in] */ IPath* highlight,               \
        /* [in] */ IPaint* highlightPaint,         \
        /* [in] */ Int32 cursorOffsetVertical);    \
                                                   \
    CARAPI DrawText(                               \
        /* [in] */ ICanvas* canvas,                \
        /* [in] */ Int32 firstLine,                \
        /* [in] */ Int32 lastLine);                \
                                                   \
    CARAPI DrawBackground(                         \
        /* [in] */ ICanvas* canvas,                \
        /* [in] */ IPath* highlight,               \
        /* [in] */ IPaint* highlightPaint,         \
        /* [in] */ Int32 cursorOffsetVertical,     \
        /* [in] */ Int32 firstLine,                \
        /* [in] */ Int32 lastLine);                \
                                                   \
    CARAPI GetLineRangeForDraw(                    \
        /* [in] */ ICanvas* canvas,                \
        /* [out] */ Int64* range);                 \
                                                   \
    CARAPI GetText(                                \
        /* [out] */ ICharSequence** text);         \
                                                   \
    CARAPI GetPaint(                               \
        /* [out] */ ITextPaint** paint);           \
                                                   \
    CARAPI GetWidth(                               \
        /* [out] */ Int32* width);                 \
                                                   \
    CARAPI GetEllipsizedWidth(                     \
        /* [out] */ Int32* width);                 \
                                                   \
    CARAPI IncreaseWidthTo(                        \
        /* [in] */ Int32 wid);                     \
                                                   \
    CARAPI GetHeight(                              \
        /* [out] */ Int32* height);                \
                                                   \
    CARAPI GetAlignment(                           \
        /* [out] */ LayoutAlignment* alignment);   \
                                                   \
    CARAPI GetSpacingMultiplier(                   \
        /* [out] */ Float* spacingMutlt);          \
                                                   \
    CARAPI GetSpacingAdd(                          \
        /* [out] */ Float* spacingAdd);            \
                                                   \
    CARAPI GetTextDirectionHeuristic(              \
        /* [out] */ ITextDirectionHeuristic** textDir);\
                                                   \
    CARAPI GetLineCount(                           \
        /* [out] */ Int32* number);                \
                                                   \
    CARAPI GetLineBounds(                          \
        /* [in] */ Int32 line,                     \
        /* [in] */ IRect* bounds,                  \
        /* [out] */ Int32* baseline);              \
                                                   \
    CARAPI GetLineTop(                             \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* pos);                   \
                                                   \
    CARAPI GetLineDescent(                         \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* descent);               \
                                                   \
    CARAPI GetLineStart(                           \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* offset);                \
                                                   \
    CARAPI GetParagraphDirection(                  \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* direction);             \
                                                   \
    CARAPI GetLineContainsTab(                     \
        /* [in] */ Int32 line,                     \
        /* [out] */ Boolean* result);              \
                                                   \
    CARAPI GetLineDirections(                      \
        /* [in] */ Int32 line,                     \
        /* [out] */ ILayoutDirections** dirctions);      \
                                                   \
    CARAPI GetTopPadding(                          \
        /* [out] */ Int32* number);                \
                                                   \
    CARAPI GetBottomPadding(                       \
        /* [out] */ Int32* number);                \
                                                   \
    CARAPI IsLevelBoundary(                        \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Boolean* flag);                \
                                                   \
    CARAPI IsRtlCharAt(                            \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Boolean* flag);                \
                                                   \
    CARAPI GetPrimaryHorizontal(                   \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Float* pos);                   \
                                                   \
    CARAPI GetSecondaryHorizontal(                 \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Float* pos);                   \
                                                   \
    CARAPI GetLineLeft(                            \
        /* [in] */ Int32 line,                     \
        /* [out] */ Float* pos);                   \
                                                   \
    CARAPI GetLineRight(                           \
        /* [in] */ Int32 line,                     \
        /* [out] */ Float* pos);                   \
                                                   \
    CARAPI GetLineMax(                             \
        /* [in] */ Int32 line,                     \
        /* [out] */ Float* extent);                \
                                                   \
    CARAPI GetLineWidth(                           \
        /* [in] */ Int32 line,                     \
        /* [out] */ Float* extent);                \
                                                   \
    CARAPI GetLineForVertical(                     \
        /* [in] */ Int32 vertical,                 \
        /* [out] */ Int32* number);                \
                                                   \
    CARAPI GetLineForOffset(                       \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Int32* number);                \
                                                   \
    CARAPI GetOffsetForHorizontal(                 \
        /* [in] */ Int32 line,                     \
        /* [in] */ Float horiz,                    \
        /* [out] */ Int32* offset);                \
                                                   \
    CARAPI GetLineEnd(                             \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* end);                   \
                                                   \
    CARAPI GetLineVisibleEnd(                      \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* offset);                \
                                                   \
    CARAPI GetLineBottom(                          \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* pos);                   \
                                                   \
    CARAPI GetLineBaseline(                        \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* pos);                   \
                                                   \
    CARAPI GetLineAscent(                          \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* ascent);                \
                                                   \
    CARAPI GetOffsetToLeftOf(                      \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Int32* retOffset);             \
                                                   \
    CARAPI GetOffsetToRightOf(                     \
        /* [in] */ Int32 offset,                   \
        /* [out] */ Int32* retOffset);             \
                                                   \
    CARAPI GetCursorPath(                          \
        /* [in] */ Int32 point,                    \
        /* [in] */ IPath* dest,                    \
        /* [in] */ ICharSequence* editingBuffer);  \
                                                   \
    CARAPI GetSelectionPath(                       \
        /* [in] */ Int32 start,                    \
        /* ]in] */ Int32 end,                      \
        /* [in] */ IPath* dest);                   \
                                                   \
    CARAPI GetParagraphAlignment(                  \
        /* [in] */ Int32 line,                     \
        /* [out] */ LayoutAlignment* align);       \
                                                   \
    CARAPI GetParagraphLeft(                       \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* leftEdge);              \
                                                   \
    CARAPI GetParagraphRight(                      \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* rigthEdge);             \
                                                   \
    CARAPI GetEllipsisStart(                       \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* ellipsisStart);         \
                                                   \
    CARAPI GetEllipsisCount(                       \
        /* [in] */ Int32 line,                     \
        /* [out] */ Int32* ellipsisiCount)

#define ILAYOUT_METHODS_IMPL(className, superClass, overRideClass)   \
ECode className::Draw(                                               \
    /* [in] */ ICanvas* c)                                           \
{                                                                    \
    return superClass::Draw(c);                                        \
}                                                                    \
                                                                     \
ECode className::DrawEx(                                             \
    /* [in] */ ICanvas* canvas,                                      \
    /* [in] */ IPath* highlight,                                     \
    /* [in] */ IPaint* highlightPaint,                               \
    /* [in] */ Int32 cursorOffsetVertical)                           \
{                                                                    \
    return superClass::DrawEx(canvas, highlight,                       \
        highlightPaint, cursorOffsetVertical);                           \
}                                                                    \
                                                                     \
ECode className::DrawText(                                           \
    /* [in] */ ICanvas* canvas,                                      \
    /* [in] */ Int32 firstLine,                                      \
    /* [in] */ Int32 lastLine)                                       \
{                                                                    \
    return superClass::DrawText(canvas,                                \
        firstLine, lastLine);                                            \
}                                                                    \
                                                                     \
ECode className::DrawBackground(                                     \
    /* [in] */ ICanvas* canvas,                                      \
    /* [in] */ IPath* highlight,                                     \
    /* [in] */ IPaint* highlightPaint,                               \
    /* [in] */ Int32 cursorOffsetVertical,                           \
    /* [in] */ Int32 firstLine,                                      \
    /* [in] */ Int32 lastLine)                                       \
{                                                                    \
    return superClass::DrawBackground(canvas,                          \
        highlight, highlightPaint,                                       \
        cursorOffsetVertical, firstLine, lastLine);                      \
}                                                                    \
                                                                     \
ECode className::GetLineRangeForDraw(                                \
    /* [in] */ ICanvas* canvas,                                      \
    /* [out] */ Int64* range)                                        \
{                                                                    \
    VALIDATE_NOT_NULL(range);                                          \
    *range = superClass::GetLineRangeForDraw(canvas);                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetText(                                            \
    /* [out] */ ICharSequence** text)                                \
{                                                                    \
    VALIDATE_NOT_NULL(text);                                           \
    AutoPtr<ICharSequence> temp = superClass::GetText();               \
    *text = temp;                                                      \
    if (*text) {                                                       \
        (*text)->AddRef();                                               \
    }                                                                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetPaint(                                           \
    /* [out] */ ITextPaint** paint)                                  \
{                                                                    \
    VALIDATE_NOT_NULL(paint);                                          \
    AutoPtr<ITextPaint> temp = superClass::GetPaint();                 \
    *paint = temp;                                                     \
    if (*paint) {                                                      \
        (*paint)->AddRef();                                              \
    }                                                                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetWidth(                                           \
    /* [out] */ Int32* width)                                        \
{                                                                    \
    VALIDATE_NOT_NULL(width);                                          \
    *width = superClass::GetWidth();                                   \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetEllipsizedWidth(                                 \
    /* [out] */ Int32* width)                                        \
{                                                                    \
    VALIDATE_NOT_NULL(width);                                          \
    *width = superClass::GetEllipsizedWidth();                         \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::IncreaseWidthTo(                                    \
    /* [in] */ Int32 wid)                                            \
{                                                                    \
    return superClass::IncreaseWidthTo(wid);                           \
}                                                                    \
                                                                     \
ECode className::GetHeight(                                          \
    /* [out] */ Int32* height)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(height);                                         \
    *height = superClass::GetHeight();                                 \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetAlignment(                                       \
    /* [out] */ LayoutAlignment* alignment)                          \
{                                                                    \
    VALIDATE_NOT_NULL(alignment);                                      \
    *alignment = superClass::GetAlignment();                           \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetSpacingMultiplier(                               \
    /* [out] */ Float* spacingMutlt)                                 \
{                                                                    \
    VALIDATE_NOT_NULL(spacingMutlt);                                   \
    *spacingMutlt = superClass::GetSpacingMultiplier();                \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetSpacingAdd(                                      \
    /* [out] */ Float* spacingAdd)                                   \
{                                                                    \
    VALIDATE_NOT_NULL(spacingAdd);                                     \
    *spacingAdd = superClass::GetSpacingAdd();                         \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetTextDirectionHeuristic(                          \
    /* [out] */ ITextDirectionHeuristic** textDir)                   \
{                                                                    \
    VALIDATE_NOT_NULL(textDir);                                        \
    AutoPtr<ITextDirectionHeuristic> temp = superClass::GetTextDirectionHeuristic(); \
    *textDir = temp;                                                   \
    if (*textDir) {                                                    \
        (*textDir)->AddRef();                                            \
    }                                                                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineCount(                                       \
    /* [out] */ Int32* number)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(number);                                         \
    *number = superClass::GetLineCount();                              \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineBounds(                                      \
    /* [in] */ Int32 line,                                           \
    /* [in] */ IRect* bounds,                                        \
    /* [out] */ Int32* baseline)                                     \
{                                                                    \
    VALIDATE_NOT_NULL(baseline);                                       \
    *baseline = superClass::GetLineBounds(line, bounds);               \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineTop(                                         \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetLineTop(line);                               \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineDescent(                                     \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* descent)                                      \
{                                                                    \
    VALIDATE_NOT_NULL(descent);                                        \
    *descent = superClass::GetLineDescent(line);                       \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineStart(                                       \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* offset)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(offset);                                         \
    *offset = superClass::GetLineStart(line);                          \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetParagraphDirection(                              \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* direction)                                    \
{                                                                    \
    VALIDATE_NOT_NULL(direction);                                      \
    *direction = superClass::GetParagraphDirection(line);              \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineContainsTab(                                 \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Boolean* result)                                     \
{                                                                    \
    VALIDATE_NOT_NULL(result);                                         \
    *result = superClass::GetLineContainsTab(line);                    \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineDirections(                                  \
    /* [in] */ Int32 line,                                           \
    /* [out] */ ILayoutDirections** dirctions)                             \
{                                                                    \
    VALIDATE_NOT_NULL(dirctions);                                      \
    AutoPtr<ILayoutDirections> temp = superClass::GetLineDirections(line);   \
    *dirctions = temp;                                                 \
    if (*dirctions) {                                                  \
        (*dirctions)->AddRef();                                          \
    }                                                                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetTopPadding(                                      \
    /* [out] */ Int32* number)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(number);                                         \
    *number = superClass::GetTopPadding();                             \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetBottomPadding(                                   \
    /* [out] */ Int32* number)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(number);                                         \
    *number = superClass::GetBottomPadding();                          \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::IsLevelBoundary(                                    \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Boolean* flag)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(flag);                                           \
    *flag = superClass::IsLevelBoundary(offset);                       \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::IsRtlCharAt(                                        \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Boolean* flag)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(flag);                                           \
    *flag = superClass::IsRtlCharAt(offset);                           \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetPrimaryHorizontal(                               \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Float* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetPrimaryHorizontal(offset);                   \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetSecondaryHorizontal(                             \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Float* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetSecondaryHorizontal(offset);                 \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineLeft(                                        \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Float* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetLineLeft(line);                              \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineRight(                                       \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Float* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetLineRight(line);                             \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineMax(                                         \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Float* extent)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(extent);                                         \
    *extent = superClass::GetLineMax(line);                            \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineWidth(                                       \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Float* extent)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(extent);                                         \
    *extent = superClass::GetLineWidth(line);                          \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineForVertical(                                 \
    /* [in] */ Int32 vertical,                                       \
    /* [out] */ Int32* number)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(number);                                         \
    *number = superClass::GetLineForVertical(vertical);                \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineForOffset(                                   \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Int32* number)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(number);                                         \
    *number = superClass::GetLineForOffset(offset);                    \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetOffsetForHorizontal(                             \
    /* [in] */ Int32 line,                                           \
    /* [in] */ Float horiz,                                          \
    /* [out] */ Int32* offset)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(offset);                                         \
    *offset = superClass::GetOffsetForHorizontal(line, horiz);         \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineEnd(                                         \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* end)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(end);                                            \
    *end = superClass::GetLineEnd(line);                               \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineVisibleEnd(                                  \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* offset)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(offset);                                         \
    *offset = superClass::GetLineVisibleEnd(line);                     \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineBottom(                                      \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetLineBottom(line);                            \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineBaseline(                                    \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* pos)                                          \
{                                                                    \
    VALIDATE_NOT_NULL(pos);                                            \
    *pos = superClass::GetLineBaseline(line);                          \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetLineAscent(                                      \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* ascent)                                       \
{                                                                    \
    VALIDATE_NOT_NULL(ascent);                                         \
    *ascent = superClass::GetLineAscent(line);                         \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetOffsetToLeftOf(                                  \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Int32* retOffset)                                    \
{                                                                    \
    VALIDATE_NOT_NULL(retOffset);                                      \
    *retOffset = superClass::GetOffsetToLeftOf(offset);                \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetOffsetToRightOf(                                 \
    /* [in] */ Int32 offset,                                         \
    /* [out] */ Int32* retOffset)                                    \
{                                                                    \
    VALIDATE_NOT_NULL(retOffset);                                      \
    *retOffset = superClass::GetOffsetToRightOf(offset);               \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetCursorPath(                                      \
    /* [in] */ Int32 point,                                          \
    /* [in] */ IPath* dest,                                          \
    /* [in] */ ICharSequence* editingBuffer)                         \
{                                                                    \
    return superClass::GetCursorPath(point, dest, editingBuffer);      \
}                                                                    \
                                                                     \
ECode className::GetSelectionPath(                                   \
    /* [in] */ Int32 start,                                          \
    /* [in] */ Int32 end,                                            \
    /* [in] */ IPath* dest)                                          \
{                                                                    \
    return superClass::GetSelectionPath(start, end, dest);             \
}                                                                    \
                                                                     \
ECode className::GetParagraphAlignment(                              \
    /* [in] */ Int32 line,                                           \
    /* [out] */ LayoutAlignment* align)                              \
{                                                                    \
    VALIDATE_NOT_NULL(align);                                          \
    *align = superClass::GetParagraphAlignment(line);                  \
                                                                     \
    return NOERROR;                                                    \
}                                                                    \
                                                                     \
ECode className::GetParagraphLeft(                                   \
    /* [in] */ Int32 line,                                           \
    /* [out] */ Int32* leftEdge)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(leftEdge);                                        \
    *leftEdge = superClass::GetParagraphLeft(line);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParagraphRight(                                     \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* rigthEdge)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(rigthEdge);                                       \
    *rigthEdge = superClass::GetParagraphRight(line);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetEllipsisStart(                                      \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* ellipsisStart)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(ellipsisStart);                                   \
    *ellipsisStart = superClass::GetEllipsisStart(line);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetEllipsisCount(                                      \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* ellipsisiCount)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(ellipsisiCount);                                  \
    *ellipsisiCount = superClass::GetEllipsisCount(line);               \
                                                                        \
    return NOERROR;                                                     \
}

#define ILAYOUTHELPER_METHODS_DECL()                                    \
    CARAPI GetDesiredWidth(                                             \
        /* [in] */ ICharSequence* source,                               \
        /* [in] */ ITextPaint* paint,                                   \
        /* [out] */ Float* ret);                                        \
                                                                        \
    CARAPI GetDesiredWidthEx(                                           \
        /* [in] */ ICharSequence* source,                               \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 end,                                           \
        /* [in] */ ITextPaint* paint,                                   \
        /* [out] */ Float* ret);                                        \
                                                                        \
    CARAPI GetDirsAllLeftToRight(                                       \
        /* [out] */ ILayoutDirections** dir);                           \
                                                                        \
    CARAPI GetDirsAllRightToLeft(                                       \
        /* [out] */ ILayoutDirections** dir);

#define ILAYOUTHELPER_METHODS_IMPL(className, superClass, overRideClass)   \
ECode className::GetDesiredWidth(                                       \
    /* [in] */ ICharSequence* source,                                   \
    /* [in] */ ITextPaint* paint,                                       \
    /* [out] */ Float* ret)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(ret);                                             \
    *ret = overRideClass::GetDesiredWidth(source, paint);               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDesiredWidthEx(                                     \
    /* [in] */ ICharSequence* source,                                   \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 end,                                               \
    /* [in] */ ITextPaint* paint,                                       \
    /* [out] */ Float* ret)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(ret);                                             \
    *ret = overRideClass::GetDesiredWidth(source, start, end, paint);   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDirsAllLeftToRight(                                 \
    /* [out] */ ILayoutDirections** dir)                                \
{                                                                       \
    VALIDATE_NOT_NULL(dir);                                             \
    *dir = Layout::DIRS_ALL_LEFT_TO_RIGHT;                              \
    INTERFACE_ADDREF(*dir);                                             \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDirsAllRightToLeft(                                 \
    /* [out] */ ILayoutDirections** dir)                                \
{                                                                       \
    VALIDATE_NOT_NULL(dir);                                             \
    *dir = Layout::DIRS_ALL_RIGHT_TO_LEFT;                              \
    INTERFACE_ADDREF(*dir);                                             \
    return NOERROR;                                                     \
}

#endif //__LAYOUTMACRO_H__
