
#include "text/DynamicLayout.h"
#include "text/CDynamicLayout.h"
#include "text/TextDirectionHeuristics.h"
#include "text/TextUtils.h"
#include "text/CStaticLayout.h"
#include "util/ArrayUtils.h"

using Elastos::Droid::Graphics::IPaintFontMetricsInt;
using Elastos::Droid::Text::Style::IUpdateLayout;
using Elastos::Droid::Text::Style::EIID_IWrapTogetherSpan;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Text {

// {6A565FF4-9481-4C51-B29D-53E8E20C8D3A}
static const InterfaceID EIID_ChangeWatcher =
    { 0x6a565ff4, 0x9481, 0x4c51, { 0xb2, 0x9d, 0x53, 0xe8, 0xe2, 0xc, 0x8d, 0x3a } };

DynamicLayout::ChangeWatcher::ChangeWatcher(
    /* [in] */ IWeakReference* layout)
    : mLayout(layout)
{
}

PInterface DynamicLayout::ChangeWatcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ISpanWatcher*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }
    else if (riid == EIID_ISpanWatcher) {
        return (ISpanWatcher*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)(ISpanWatcher*)this;
    }
    else if (riid == EIID_ChangeWatcher) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 DynamicLayout::ChangeWatcher::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DynamicLayout::ChangeWatcher::Release()
{
    return ElRefBase::Release();
}

ECode DynamicLayout::ChangeWatcher::GetInterfaceID(
    /* [in] */ IInterface *objects,
    /* [out] */ InterfaceID *IID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void DynamicLayout::ChangeWatcher::Reflow(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    AutoPtr<IDynamicLayout> dl;
    mLayout->Resolve(EIID_IDynamicLayout, (IInterface**)&dl);

    if (dl != NULL) {
        DynamicLayout* layout = (DynamicLayout*)(CDynamicLayout*)dl.Get();
        layout->Reflow(s, where, before, after);
    }
    else if (s != NULL && ISpannable::Probe(s) != NULL) {
        ISpannable::Probe(s)->RemoveSpan(THIS_PROBE(IInterface));
    }
}

ECode DynamicLayout::ChangeWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    // Intentionally empty
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    Reflow(s, where, before, after);
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    // Intentionally empty
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnSpanAdded(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (o != NULL && IUpdateLayout::Probe(o) != NULL) {
        Reflow(s, start, end - start, end - start);
    }
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnSpanRemoved(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (o != NULL && IUpdateLayout::Probe(o) != NULL) {
        Reflow(s, start, end - start, end - start);
    }
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnSpanChanged(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    if (o != NULL && IUpdateLayout::Probe(o) != NULL) {
        Reflow(s, start, end - start, end - start);
        Reflow(s, nstart, nend - nstart, nend - nstart);
    }
    return NOERROR;
}


const Int32 DynamicLayout::PRIORITY;
const Int32 DynamicLayout::BLOCK_MINIMUM_CHARACTER_LENGTH;
const Int32 DynamicLayout::INVALID_BLOCK_INDEX;

Boolean DynamicLayout::sIsStaticLayoutInited = FALSE;
AutoPtr<CStaticLayout> DynamicLayout::sStaticLayout;
Mutex DynamicLayout::sLock;

const Int32 DynamicLayout::START;
const Int32 DynamicLayout::DIR;
const Int32 DynamicLayout::TAB;
const Int32 DynamicLayout::TOP;
const Int32 DynamicLayout::DESCENT;
const Int32 DynamicLayout::COLUMNS_NORMAL;
const Int32 DynamicLayout::ELLIPSIS_START;
const Int32 DynamicLayout::ELLIPSIS_COUNT;
const Int32 DynamicLayout::COLUMNS_ELLIPSIZE;
const Int32 DynamicLayout::START_MASK;
const Int32 DynamicLayout::DIR_SHIFT;
const Int32 DynamicLayout::TAB_MASK;
const Int32 DynamicLayout::ELLIPSIS_UNDEFINED;

AutoPtr<CStaticLayout> DynamicLayout::GetStaticLayout()
{
    if (!sIsStaticLayoutInited) {
        assert(sStaticLayout == NULL);
        sIsStaticLayoutInited = TRUE;
        CStaticLayout::NewByFriend(NULL, (CStaticLayout**)&sStaticLayout);
    }
    return sStaticLayout;
}

void DynamicLayout::SetStaticLayout(
    /* [in] */ CStaticLayout* layout)
{
    sStaticLayout = layout;
}

DynamicLayout::DynamicLayout()
    : mIncludePad(FALSE)
    , mEllipsize(FALSE)
    , mEllipsizedWidth(0)
    , mNumberOfBlocks(0)
    , mTopPadding(0)
    , mBottomPadding(0)
{}

/**
 * Make a layout for the specified text that will be updated as
 * the text is changed.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mIncludePad(FALSE)
    , mEllipsize(FALSE)
    , mEllipsizedWidth(0)
    , mNumberOfBlocks(0)
    , mTopPadding(0)
    , mBottomPadding(0)
{
    Init(base, base, paint, width, align, spacingmult, spacingadd,
             includepad);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : mIncludePad(FALSE)
    , mEllipsize(FALSE)
    , mEllipsizedWidth(0)
    , mNumberOfBlocks(0)
    , mTopPadding(0)
    , mBottomPadding(0)
{
    Init(base, display, paint, width, align, spacingmult, spacingadd,
             includepad, -1, 0);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    : mIncludePad(FALSE)
    , mEllipsize(FALSE)
    , mEllipsizedWidth(0)
    , mNumberOfBlocks(0)
    , mTopPadding(0)
    , mBottomPadding(0)
{
    Init(base, display, paint, width, align, TextDirectionHeuristics::FIRSTSTRONG_LTR,
                spacingmult, spacingadd, includepad, ellipsize, ellipsizedWidth);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 * *
 * *@hide
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    : mIncludePad(FALSE)
    , mEllipsize(FALSE)
    , mEllipsizedWidth(0)
    , mNumberOfBlocks(0)
    , mTopPadding(0)
    , mBottomPadding(0)
{
    Init(base, display, paint, width, align,
         textDir, spacingmult, spacingadd,
         includepad, ellipsize, ellipsizedWidth);
}

/**
 * Make a layout for the specified text that will be updated as
 * the text is changed.
 */
ECode DynamicLayout::Init(
    /* [in] */ ICharSequence* base,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return Init(base, base, paint, width, align, spacingmult, spacingadd,
             includepad);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 */
ECode DynamicLayout::Init(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
{
    return Init(base, display, paint, width, align, spacingmult, spacingadd,
             includepad, -1, 0);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 */
ECode DynamicLayout::Init(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    return Init(base, display, paint, width, align, TextDirectionHeuristics::FIRSTSTRONG_LTR,
                spacingmult, spacingadd, includepad, ellipsize, ellipsizedWidth);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-null, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 * *
 * *@hide
 */
ECode DynamicLayout::Init(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ ITextDirectionHeuristic* textDir,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    AutoPtr<ICharSequence> paramCS;
    if (ellipsize == TextUtilsTruncateAt_NONE) {
        paramCS = display;
    }
    else {
        if (display != NULL && ISpanned::Probe(display)) {
            paramCS = (Ellipsizer*)new SpannedEllipsizer(display);
        }
        else {
            paramCS = new Ellipsizer(display);
        }
    }
    Layout::Init(paramCS,
          paint, width, align, textDir, spacingmult, spacingadd);

    mBase = base;
    mDisplay = display;

    if (ellipsize != TextUtilsTruncateAt_NONE) {
        mInts = new PackedIntVector(COLUMNS_ELLIPSIZE);
        mEllipsizedWidth = ellipsizedWidth;
        mEllipsizeAt = ellipsize;
    }
    else {
        mInts = new PackedIntVector(COLUMNS_NORMAL);
        mEllipsizedWidth = width;
        mEllipsizeAt = TextUtilsTruncateAt_NONE;
    }

    mObjects = new PackedObjectVector<ILayoutDirections>(1);

    mIncludePad = includepad;

    /*
     * This is annoying, but we can't refer to the layout until
     * superclass construction is finished, and the superclass
     * constructor wants the reference to the display text.
     *
     * This will break if the superclass constructor ever actually
     * cares about the content instead of just holding the reference.
     */
    if (ellipsize != TextUtilsTruncateAt_NONE) {
        AutoPtr<Ellipsizer> e = (Ellipsizer*)GetText().Get();
        e->mLayout = this;
        e->mWidth = ellipsizedWidth;
        e->mMethod = ellipsize;
        mEllipsize = TRUE;
    }

    // Initial state is a single line with 0 characters (0 to 0),
    // with top at 0 and bottom at whatever is natural, and
    // undefined ellipsis.

    AutoPtr< ArrayOf<Int32> > start;

    if (ellipsize != TextUtilsTruncateAt_NONE) {
        start = ArrayOf<Int32>::Alloc(COLUMNS_ELLIPSIZE);
        (*start)[ELLIPSIS_START] = ELLIPSIS_UNDEFINED;
    }
    else {
        start = ArrayOf<Int32>::Alloc(COLUMNS_NORMAL);
    }

    //Directions[] dirs = new Directions[] { DIRS_ALL_LEFT_TO_RIGHT };
    AutoPtr< ArrayOf<ILayoutDirections*> > dirs = ArrayOf<ILayoutDirections*>::Alloc(1);
    dirs->Set(0, DIRS_ALL_LEFT_TO_RIGHT);

    AutoPtr<IPaintFontMetricsInt> fm;
    paint->GetFontMetricsIntEx((IPaintFontMetricsInt**)&fm);
    Int32 asc, desc;
    fm->GetAscent(&asc);
    fm->GetDescent(&desc);

    (*start)[DIR] = ILayout::DIR_LEFT_TO_RIGHT << DIR_SHIFT;
    (*start)[TOP] = 0;
    (*start)[DESCENT] = desc;
    mInts->InsertAt(0, start);

    (*start)[TOP] = desc - asc;
    mInts->InsertAt(1, start);

    mObjects->InsertAt(0, dirs);
    // Update from 0 characters to whatever the real text is
    Int32 len;
    base->GetLength(&len);
    Reflow(base, 0, 0, len);

    if (ISpannable::Probe(base) != NULL) {
        if (mWatcher == NULL) {
            IWeakReferenceSource* wrs = THIS_PROBE(IWeakReferenceSource);
            AutoPtr<IWeakReference> wr;
            wrs->GetWeakReference((IWeakReference**)&wr);
            mWatcher = new ChangeWatcher(wr);
        }

        // Strip out any watchers for other DynamicLayouts.
        AutoPtr<ISpannable> sp = ISpannable::Probe(base);
        sp->GetLength(&len);
        AutoPtr< ArrayOf<IInterface*> > spans;
        sp->GetSpans(0, len, EIID_ChangeWatcher, (ArrayOf<IInterface*>**)&spans);
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            sp->RemoveSpan((*spans)[i]);
        }

        base->GetLength(&len);
        sp->SetSpan(mWatcher->Probe(EIID_IInterface), 0, len,
                   ISpannable::SPAN_INCLUSIVE_INCLUSIVE |
                   (PRIORITY << ISpannable::SPAN_PRIORITY_SHIFT));
    }

    return NOERROR;
}

void DynamicLayout::Reflow(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    if (s != mBase) {
        return;
    }

    AutoPtr<ICharSequence> text = mDisplay;
    Int32 len;
    text->GetLength(&len);

    // seek back to the start of the paragraph
    Int32 find = TextUtils::LastIndexOf(text, '\n', where - 1);
    if (find < 0) {
        find = 0;
    }
    else {
        find = find + 1;
    }

    {
        Int32 diff = where - find;
        before += diff;
        after += diff;
        where -= diff;
    }

    // seek forward to the end of the paragraph

    Int32 look = TextUtils::IndexOf(text, '\n', where + after);
    if (look < 0) {
        look = len;
    }
    else {
        look++; // we want the index after the \n
    }

    Int32 change = look - (where + after);
    before += change;
    after += change;

    // seek further out to cover anything that is forced to wrap together

    if (ISpanned::Probe(text) != NULL) {
        AutoPtr<ISpanned> sp = ISpanned::Probe(text);
        Boolean again;

        do {
            again = FALSE;

            AutoPtr< ArrayOf<IInterface*> > force;
            sp->GetSpans(where, where + after,
                    EIID_IWrapTogetherSpan, (ArrayOf<IInterface*>**)&force);

            for (Int32 i = 0; i < force->GetLength(); i++) {
                Int32 st, en;
                sp->GetSpanStart((*force)[i], &st);
                sp->GetSpanEnd((*force)[i], &en);

                if (st < where) {
                    again = TRUE;

                    Int32 diff = where - st;
                    before += diff;
                    after += diff;
                    where -= diff;
                }

                if (en > where + after) {
                    again = TRUE;

                    Int32 diff = en - (where + after);
                    before += diff;
                    after += diff;
                }
            }
        } while(again);
    }

    // find affected region of old layout
    Int32 startline = GetLineForOffset(where);
    Int32 startv = GetLineTop(startline);

    Int32 endline = GetLineForOffset(where + before);
    if (where + after == len) {
        endline = GetLineCount();
    }
    Int32 endv = GetLineTop(endline);
    Boolean islast = (endline == GetLineCount());

    // generate new layout for affected text
    AutoPtr<CStaticLayout> reflowed;

    {
        Mutex::Autolock lock(&sLock);

        reflowed = GetStaticLayout();
        SetStaticLayout(NULL);
    }

    if (reflowed == NULL) {
        CStaticLayout::NewByFriend(NULL, (CStaticLayout**)&reflowed);
    }
    else {
        reflowed->Prepare();
    }

    reflowed->Generate(text, where, where + after,
                GetPaint(), GetWidth(), GetTextDirectionHeuristic(), GetSpacingMultiplier(),
                GetSpacingAdd(), FALSE,
                TRUE, mEllipsizedWidth, mEllipsizeAt);
    Int32 n = reflowed->GetLineCount();

    // If the new layout has a blank line at the end, but it is not
    // the very end of the buffer, then we already have a line that
    // starts there, so disregard the blank line.
    Int32 offset;
    if (where + after != len && (reflowed->GetLineStart(n - 1, &offset), offset) == where + after) {
        n--;
    }

    // remove affected lines from old layout
    mInts->DeleteAt(startline, endline - startline);
    mObjects->DeleteAt(startline, endline - startline);

    // adjust offsets in layout for new height and offsets

    Int32 ht = reflowed->GetLineTop(n);
    Int32 toppad = 0, botpad = 0;

    if (mIncludePad && startline == 0) {
        toppad = reflowed->GetTopPadding();
        mTopPadding = toppad;
        ht -= toppad;
    }
    if (mIncludePad && islast) {
        botpad = reflowed->GetBottomPadding();
        mBottomPadding = botpad;
        ht += botpad;
    }

    mInts->AdjustValuesBelow(startline, START, after - before);
    mInts->AdjustValuesBelow(startline, TOP, startv - endv + ht);

    // insert new layout

    AutoPtr< ArrayOf<Int32> > ints;

    if (mEllipsize) {
        ints = ArrayOf<Int32>::Alloc(COLUMNS_ELLIPSIZE);
        (*ints)[ELLIPSIS_START] = ELLIPSIS_UNDEFINED;
    }
    else {
        ints = ArrayOf<Int32>::Alloc(COLUMNS_NORMAL);
    }

    AutoPtr< ArrayOf<ILayoutDirections*> > objects = ArrayOf<ILayoutDirections*>::Alloc(1);

    for (Int32 i = 0; i < n; i++) {
        (*ints)[START] = reflowed->GetLineStart(i) |
                (reflowed->GetParagraphDirection(i) << DIR_SHIFT) |
                (reflowed->GetLineContainsTab(i) ? TAB_MASK : 0);

        Int32 top = reflowed->GetLineTop(i) + startv;
        if (i > 0) {
            top -= toppad;
        }
        (*ints)[TOP] = top;

        Int32 desc = reflowed->GetLineDescent(i);
        if (i == n - 1) {
            desc += botpad;
        }

        (*ints)[DESCENT] = desc;

        AutoPtr<ILayoutDirections> directions = reflowed->GetLineDirections(i);
        objects->Set(0, directions);

        if (mEllipsize) {
            (*ints)[ELLIPSIS_START] = reflowed->GetEllipsisStart(i);
            (*ints)[ELLIPSIS_COUNT] = reflowed->GetEllipsisCount(i);
        }

        mInts->InsertAt(startline + i, ints);
        mObjects->InsertAt(startline + i, objects);
    }

    UpdateBlocks(startline, endline - 1, n);

    {
        Mutex::Autolock lock(&sLock);

        SetStaticLayout(reflowed);
        reflowed->Finish();
    }
}

/**
 * Create the initial block structure, cutting the text into blocks of at least
 * BLOCK_MINIMUM_CHARACTER_SIZE characters, aligned on the ends of paragraphs.
 */
void DynamicLayout::CreateBlocks()
{
    Int32 offset = BLOCK_MINIMUM_CHARACTER_LENGTH;
    mNumberOfBlocks = 0;
    AutoPtr<ICharSequence> text = mDisplay;

    while (TRUE) {
        offset = TextUtils::IndexOf(text, '\n', offset);
        if (offset < 0) {
            Int32 len = 0;
            text->GetLength(&len);
            AddBlockAtOffset(len);
            break;
        }
        else {
            AddBlockAtOffset(offset);
            offset += BLOCK_MINIMUM_CHARACTER_LENGTH;
        }
    }

    // mBlockIndices and mBlockEndLines should have the same length
    mBlockIndices = ArrayOf<Int32>::Alloc(mBlockEndLines->GetLength());
    for (Int32 i = 0; i < mBlockEndLines->GetLength(); i++) {
        (*mBlockIndices)[i] = INVALID_BLOCK_INDEX;
    }
}

/**
 * Create a new block, ending at the specified character offset.
 * A block will actually be created only if has at least one line, i.e. this offset is
 * not on the end line of the previous block.
 */
void DynamicLayout::AddBlockAtOffset(
    /* [in] */ Int32 offset)
{
    Int32 line = GetLineForOffset(offset);

    if (mBlockEndLines == NULL) {
        // Initial creation of the array, no test on previous block ending line
        mBlockEndLines = ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(1));
        (*mBlockEndLines)[mNumberOfBlocks] = line;
        mNumberOfBlocks++;
        return;
    }

    Int32 previousBlockEndLine = (*mBlockEndLines)[mNumberOfBlocks - 1];
    if (line > previousBlockEndLine) {
        if (mNumberOfBlocks == mBlockEndLines->GetLength()) {
            // Grow the array if needed
            AutoPtr< ArrayOf<Int32> > blockEndLines =
                ArrayOf<Int32>::Alloc(ArrayUtils::IdealInt32ArraySize(mNumberOfBlocks + 1));
            blockEndLines->Copy(mBlockEndLines);
            mBlockEndLines = blockEndLines;
        }
        (*mBlockEndLines)[mNumberOfBlocks] = line;
        mNumberOfBlocks++;
    }
}

void DynamicLayout::UpdateBlocks(
    /* [in] */ Int32 startLine,
    /* [in] */ Int32 endLine,
    /* [in] */ Int32 newLineCount)
{
    if (mBlockEndLines == NULL) {
        CreateBlocks();
        return;
    }

    Int32 firstBlock = -1;
    Int32 lastBlock = -1;
    for (Int32 i = 0; i < mNumberOfBlocks; i++) {
        if ((*mBlockEndLines)[i] >= startLine) {
            firstBlock = i;
            break;
        }
    }
    for (Int32 i = firstBlock; i < mNumberOfBlocks; i++) {
        if ((*mBlockEndLines)[i] >= endLine) {
            lastBlock = i;
            break;
        }
    }
    const Int32 lastBlockEndLine = (*mBlockEndLines)[lastBlock];

    Boolean createBlockBefore = startLine > (firstBlock == 0 ? 0 :
            (*mBlockEndLines)[firstBlock - 1] + 1);
    Boolean createBlock = newLineCount > 0;
    Boolean createBlockAfter = endLine < (*mBlockEndLines)[lastBlock];

    Int32 numAddedBlocks = 0;
    if (createBlockBefore) numAddedBlocks++;
    if (createBlock) numAddedBlocks++;
    if (createBlockAfter) numAddedBlocks++;

    const Int32 numRemovedBlocks = lastBlock - firstBlock + 1;
    const Int32 newNumberOfBlocks = mNumberOfBlocks + numAddedBlocks - numRemovedBlocks;

    if (newNumberOfBlocks == 0) {
        // Even when text is empty, there is actually one line and hence one block
        (*(mBlockEndLines.Get()))[0] = 0;
        (*(mBlockIndices.Get()))[0] = INVALID_BLOCK_INDEX;
        mNumberOfBlocks = 1;
        return;
    }

    if (newNumberOfBlocks > mBlockEndLines->GetLength()) {
        const Int32 newSize = ArrayUtils::IdealInt32ArraySize(newNumberOfBlocks);
        AutoPtr< ArrayOf<Int32> > blockEndLines = ArrayOf<Int32>::Alloc(newSize);
        AutoPtr< ArrayOf<Int32> > blockIndices = ArrayOf<Int32>::Alloc(newSize);
        blockEndLines->Copy(mBlockEndLines, firstBlock);
        blockIndices->Copy(mBlockIndices, firstBlock);
        blockEndLines->Copy(firstBlock + numAddedBlocks, mBlockEndLines, lastBlock + 1, mNumberOfBlocks - lastBlock - 1);
        blockIndices->Copy(firstBlock + numAddedBlocks, mBlockIndices, lastBlock + 1, mNumberOfBlocks - lastBlock - 1);
        mBlockEndLines = blockEndLines;
        mBlockIndices = blockIndices;
    }
    else {
        mBlockEndLines->Copy(firstBlock + numAddedBlocks, mBlockEndLines, lastBlock + 1, mNumberOfBlocks - lastBlock - 1);
        mBlockIndices->Copy(firstBlock + numAddedBlocks, mBlockIndices, lastBlock + 1, mNumberOfBlocks - lastBlock - 1);
    }

    mNumberOfBlocks = newNumberOfBlocks;
    const Int32 deltaLines = newLineCount - (endLine - startLine + 1);
    for (Int32 i = firstBlock + numAddedBlocks; i < mNumberOfBlocks; i++) {
        (*mBlockEndLines)[i] += deltaLines;
    }

    Int32 blockIndex = firstBlock;
    if (createBlockBefore) {
        (*mBlockEndLines)[blockIndex] = startLine - 1;
        (*mBlockIndices)[blockIndex] = INVALID_BLOCK_INDEX;
        blockIndex++;
    }

    if (createBlock) {
        (*mBlockEndLines)[blockIndex] = startLine + newLineCount - 1;
        (*mBlockIndices)[blockIndex] = INVALID_BLOCK_INDEX;
        blockIndex++;
    }

    if (createBlockAfter) {
        (*mBlockEndLines)[blockIndex] = lastBlockEndLine + deltaLines;
        (*mBlockIndices)[blockIndex] = INVALID_BLOCK_INDEX;
    }
}

/**
 * This package private method is used for test purposes only
 * @hide
 */
void DynamicLayout::SetBlocksDataForTest(
    /* [in] */ ArrayOf<Int32>* blockEndLines,
    /* [in] */ ArrayOf<Int32>* blockIndices,
    /* [in] */ Int32 numberOfBlocks)
{
    mBlockEndLines = ArrayOf<Int32>::Alloc(blockEndLines->GetLength());
    mBlockIndices = ArrayOf<Int32>::Alloc(blockIndices->GetLength());
    mBlockEndLines->Copy(blockEndLines);
    mBlockIndices->Copy(blockIndices);
    mNumberOfBlocks = numberOfBlocks;
}

/**
 * @hide
 */
AutoPtr< ArrayOf<Int32> > DynamicLayout::GetBlockEndLines()
{
    return mBlockEndLines;
}

/**
 * @hide
 */
AutoPtr< ArrayOf<Int32> > DynamicLayout::GetBlockIndices()
{
    return mBlockIndices;
}

/**
 * @hide
 */
Int32 DynamicLayout::GetNumberOfBlocks()
{
    return mNumberOfBlocks;
}

//@Override
Int32 DynamicLayout::GetLineCount()
{
    return mInts->Size() - 1;
}

//@Override
Int32 DynamicLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, TOP);
}

//@Override
Int32 DynamicLayout::GetLineDescent(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, DESCENT);
}

//@Override
Int32 DynamicLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, START) & START_MASK;
}

//@Override
Boolean DynamicLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return (mInts->GetValue(line, TAB) & TAB_MASK) != 0;
}

//@Override
Int32 DynamicLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, DIR) >> DIR_SHIFT;
}

//@Override
AutoPtr<ILayoutDirections> DynamicLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    assert(line >= 0);
    AutoPtr<ILayoutDirections> ld;
    if (mObjects != NULL) {
        ld = mObjects->GetValue(line, 0);
    }
    return ld;
}

//@Override
Int32 DynamicLayout::GetTopPadding()
{
    return mTopPadding;
}

//@Override
Int32 DynamicLayout::GetBottomPadding()
{
    return mBottomPadding;
}

//@Override
Int32 DynamicLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

//@Override
Int32 DynamicLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    if (mEllipsizeAt == -1) {
        return 0;
    }

    return mInts->GetValue(line, ELLIPSIS_START);
}

//@Override
Int32 DynamicLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    if (mEllipsizeAt == -1) {
        return 0;
    }

    return mInts->GetValue(line, ELLIPSIS_COUNT);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
