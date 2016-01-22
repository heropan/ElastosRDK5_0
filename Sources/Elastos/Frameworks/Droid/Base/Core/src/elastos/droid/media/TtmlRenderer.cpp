#include "elastos/droid/media/TtmlRenderer.h"
#include <elastos/utility/regex/Pattern.h>
#include <elastos/core/StringUtils.h>

using Elastos::Utility::Regex::IMatcher;
using Elastos::Utility::Regex::Pattern;
using Org::Xmlpull::V1::CXmlPullParserFactory;
using Org::Xmlpull::V1::IXmlPullParserFactory;

namespace Elastos{
namespace Droid {
namespace Media {
//================================================================================
//                      TtmlRenderer
//================================================================================
const String TtmlRenderer::MEDIA_MIMETYPE_TEXT_TTML(String("application/ttml+xml"));

TtmlRenderer::TtmlRenderer()
{}

TtmlRenderer::~TtmlRenderer()
{}

CAR_INTERFACE_IMPL_2(TtmlRenderer, Object, ITtmlRenderer, ISubtitleControllerRenderer)

ECode TtmlRenderer::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}


ECode TtmlRenderer::Supports(
    /* [in] */ IMediaFormat* format,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean flag = FALSE;
    format->ContainsKey(IMediaFormat::KEY_MIME, &flag);
    if (flag) {
        *result = format->GetString(IMediaFormat::KEY_MIME).Equals(MEDIA_MIMETYPE_TEXT_TTML);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode TtmlRenderer::CreateTrack(
    /* [in] */ IMediaFormat* format,
    /* [out] */ ISubtitleTrack** result)
{
    VALIDATE_NOT_NULL(result);

    if (mRenderingWidget == NULL) {
        mRenderingWidget = new TtmlRenderingWidget();
        mRenderingWidget->constructor(mContext);
    }
    AutoPtr<TtmlTrack> tt = new TtmlTrack();
    tt->constructor(mRenderingWidget, format);
    *result = ISubtitleTrack::Probe(tt);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//================================================================================
//                      TtmlUtils
//================================================================================
static AutoPtr<IPattern> initCLOCK_TIME()
{
    AutoPtr<IPattern> p;
    Pattern::Compile(
            String("^([0-9][0-9]+):([0-9][0-9]):([0-9][0-9])")
            + String("(?:(\\.[0-9]+)|:([0-9][0-9])(?:\\.([0-9]+))?)?$"), (IPattern**)&p);
    return p;
}

const AutoPtr<IPattern> TtmlUtils::CLOCK_TIME = initOFFSET_TIME();

static AutoPtr<IPattern> initOFFSET_TIME()
{
    AutoPtr<IPattern> p;
    Pattern::Compile(String("^([0-9]+(?:\\.[0-9]+)?)(h|m|s|ms|f|t)$"), (IPattern**)&p);
    return p;
}

const AutoPtr<IPattern> TtmlUtils::OFFSET_TIME = initOFFSET_TIME();

ECode TtmlUtils::ParseTimeExpression(
    /* [in] */ const String& time,
    /* [in] */ Int32 frameRate,
    /* [in] */ Int32 subframeRate,
    /* [in] */ Int32 tickRate,
    /* [out] */ Int64** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IMatcher> matcher;
    CLOCK_TIME->Matcher(time, (IMatcher**)&matcher);
    Boolean flag = FALSE;
    matcher->Matches(&flag);
    if (flag) {
        String hours;
        matcher->Group(1, &hours);
        Double durationSeconds = StringUtils::ParseInt64(hours) * 3600;
        String minutes;
        matcher->Group(2, &minutes);
        durationSeconds += StringUtils::ParseInt64(minutes) * 60;
        String seconds;
        matcher->Group(3, &seconds);
        durationSeconds += StringUtils::ParseInt64(seconds);
        String fraction;
        matcher->Group(4, &fraction);
        durationSeconds += (fraction != NULL) ? StringUtils::ParseDouble(fraction) : 0;
        String frames;
        matcher->Group(5, &frames);
        durationSeconds += (frames != NULL) ? ((Double)StringUtils::ParseInt64(frames)) / frameRate : 0;
        String subframes;
        matcher->Group(6, &subframes);
        durationSeconds += (subframes != NULL) ? ((Double)StringUtils::ParseInt64(subframes))
                / subframeRate / frameRate
                    : 0;
        return (Int64)(durationSeconds * 1000);
    }
    matcher = NULL;
    OFFSET_TIME->Matcher(time, (IMatcher**)&matcher);
    matcher->Matches(&flag);
    if (flag) {
        String timeValue;
        matcher->Group(1, &timeValue);
        Double value = StringUtils::ParseDouble(timeValue);
        String unit;
        matcher->Group(2, &unit);
        if (unit.Equals(String("h"))) {
            value *= 3600L * 1000000L;
        } else if (unit.Equals(String("m"))) {
            value *= 60 * 1000000;
        } else if (unit.Equals(String("s"))) {
            value *= 1000000;
        } else if (unit.Equals(String("ms"))) {
            value *= 1000;
        } else if (unit.Equals(String("f"))) {
            value = value / frameRate * 1000000;
        } else if (unit.Equals(String("t"))) {
            value = value / tickRate * 1000000;
        }
            return (Int64)value;
        }

    Slogger::E("TtmlUtils", "Malformed time expression : %s" + time.string());
    return E_NUMBER_FORMAT_EXCEPTION;
}

ECode TtmlUtils::ApplyDefaultSpacePolicy(
    /* [in] */ const String& in,
    /* [out] */ String** result)
{
    VALIDATE_NOT_NULL(result);
    return ApplySpacePolicy(in, TRUE, result);
}

ECode TtmlUtils::ApplySpacePolicy(
    /* [in] */ const String& in,
    /* [in] */ Boolean treatLfAsSpace,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    // Removes CR followed by LF. ref:
    // http://www.w3.org/TR/xml/#sec-line-ends
    String crRemoved; = in.replaceAll("\r\n", "\n");
    StringUtils::ReplaceAll(in, String("\r\n"), String("\n"), &crRemoved);

    // Apply suppress-at-line-break="auto" and
    // white-space-treatment="ignore-if-surrounding-linefeed"
    String spacesNeighboringLfRemoved;
    StringUtils::ReplaceAll(crRemoved, String(" *\n *"), String("\n"), &spacesNeighboringLfRemoved);
    // Apply linefeed-treatment="treat-as-space"
    String lfToSpace;
    if (treatLfAsSpace) {
        StringUtils::ReplaceAll(spacesNeighboringLfRemoved, String("\n"), String(" "), &lfToSpace);
    } else {
        lfToSpace = spacesNeighboringLfRemoved;
    }
    // Apply white-space-collapse="true"
    String spacesCollapsed;
    StringUtils::ReplaceAll(lfToSpace, String("[ \t\\x0B\f\r]+"), String(" "), &spacesCollapsed);
    *result = spacesCollapsed;
    return NOERROR;
}

ECode TtmlUtils::ExtractText(
    /* [in] */ ITtmlNode* root,
    /* [in] */ Int64 startUs,
    /* [in] */ Int64 endUs,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    StringBuilder text ;
    ExtractText(root, startUs, endUs, text, FALSE);
    return StringUtils::ReplaceAll(text.ToString(), String("\n$"), String(""), result);
}

ECode TtmlUtils::ExtractTtmlFragment(
    /* [in] */ ITtmlNode* root,
    /* [in] */ Int64 startUs,
    /* [in] */ Int64 endUs,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    StringBuilder fragment;
    ExtractTtmlFragment(root, startUs, endUs, fragment);
    *result = fragment->ToString();
    return NOERROR;
}

void TtmlUtils::ExtractText(
    /* [in] */ ITtmlNode* node_,
    /* [in] */ Int64 startUs,
    /* [in] */ Int64 endUs,
    /* [in] */ StringBuilder& out,
    /* [in] */ Boolean inPTag)
{
    AutoPtr<TtmlNode> node = (TtmlNode*)node_;
    if (node->mName.Equals(ITtmlUtils::PCDATA) && inPTag) {
        out->Append(node->mText);
    } else if (node->mName.Equals(ITtmlUtils::TAG_BR) && inPTag) {
        out->Append(String("\n"));
    } else if (node->mName.Equals(ITtmlUtils::TAG_METADATA)) {
        // do nothing.
    } else if (node->isActive(startUs, endUs)) {
        Boolean pTag = node->mName.Equals(ITtmlUtils::TAG_P);
        Int32 length;
        out->GetLength(&length);
        // for (Int32 i = 0; i < node->mChildren.size(); ++i) {
        //     ExtractText(node->mChildren.get(i), startUs, endUs, out-> pTag || inPTag);
        // }
        // if (pTag && length != out->length()) {
        //     out->Append("\n");
        // }
    }

}

//================================================================================
//                      TtmlCue
//================================================================================
TtmlCue::TtmlCue()
{}

TtmlCue::~TtmlCue()
{}

CAR_INTERFACE_IMPL(TtmlCue, Object, ISubtitleTrackCue)

ECode TtmlCue::constructor(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 endTimeMs,
    /* [in] */ const String& text,
    /* [in] */ const String& ttmlFragment)
{
    mStartTimeMs = startTimeMs;
    mEndTimeMs = endTimeMs;
    mText = text;
    mTtmlFragment = ttmlFragment;
    return NOERROR;
}

//================================================================================
//                      TtmlNode
//================================================================================
TtmlNode::TtmlNode()
{}

TtmlNode::~TtmlNode()
{}

CAR_INTERFACE_IMPL(TtmlNode, Object, ITtmlNode)

ECode TtmlNode::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& attributes,
    /* [in] */ const String& text,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 endTimeMs,
    /* [in] */ ITtmlNode* parent,
    /* [in] */ Int64 runId)
{
    mName = name;
    mAttributes = attributes;
    mText = text;
    mStartTimeMs = startTimeMs;
    mEndTimeMs = endTimeMs;
    mParent = parent;
    mRunId = runId;
    return NOERROR;
}

ECode TtmlNode::IsActive(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 endTimeMs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mEndTimeMs > startTimeMs && mStartTimeMs < endTimeMs;
    return NOERROR;
}

//================================================================================
//                      TtmlParser
//================================================================================
TtmlParser::TtmlParser();

TtmlParser::~TtmlParser();

CAR_INTERFACE_IMPL(TtmlParser, Object, ITtmlParser)

ECode TtmlParser::constructor(
    /* [in] */ ITtmlNodeListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode TtmlParser::Parse(
    /* [in] */ const String& ttmlText,
    /* [in] */ Int64 runId)
{
    mParser = NULL;
    mCurrentRunId = runId;
    LoadParser(ttmlText);
    return ParseTtml();
}

void TtmlParser::LoadParser(
    /* [in] */ const String& ttmlFragment)
{
    AutoPtr<IXmlPullParserFactory> factory;

    CXmlPullParserFactory::NewInstance((IXmlPullParserFactory**)&factory);
    factory->SetNamespaceAware(FALSE);
    factory->NewPullParser((IXmlPullParser**)&mParser);

    AutoPtr<IStringReader> in;
    CStringReader::New(ttmlFragment, (IStringReader**)&in);
    mParser->SetInput(IReader::Probe(in));
}

void TtmlParser::ExtractAttribute(
    /* [in] */ XmlPullParser* parser,
    /* [in] */ Int32 i,
    /* [in] */ StringBuilder& out)
{
    out.Append(String(" "));
    out.Append(parser.getAttributeName(i));
    out.Append(String("=\""));
    out.Append(parser.getAttributeValue(i));
    out.Append(String("\""));
}

ECode TtmlParser::ParseNode(
    /* [in] */ TtmlNode* parent,
    /* [out] */ TtmlNode** result)
{}

ECode TtmlParser::IsEndOfDoc(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 eventType;
    mParser->GetEventType(&eventType);
    *result = (eventType == IXmlPullParser.END_DOCUMENT);
    return NOERROR;
}


ECode TtmlParser::IsSupportedTag(
    /* [in] */ const String& tag,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (tag.Equals(ITtmlUtils::TAG_TT) || tag.Equals(ITtmlUtils::TAG_HEAD) ||
            tag.Equals(ITtmlUtils::TAG_BODY) || tag.Equals(ITtmlUtils::TAG_DIV) ||
            tag.Equals(ITtmlUtils::TAG_P) || tag.Equals(ITtmlUtils::TAG_SPAN) ||
            tag.Equals(ITtmlUtils::TAG_BR) || tag.Equals(ITtmlUtils::TAG_STYLE) ||
            tag.Equals(ITtmlUtils::TAG_STYLING) || tag.Equals(ITtmlUtils::TAG_LAYOUT) ||
            tag.Equals(ITtmlUtils::TAG_REGION) || tag.Equals(ITtmlUtils::TAG_METADATA) ||
            tag.Equals(ITtmlUtils::TAG_SMPTE_IMAGE) || tag.Equals(ITtmlUtils::TAG_SMPTE_DATA) ||
            tag.Equals(ITtmlUtils::TAG_SMPTE_INFORMATION)) {
        *result = TRUE;
    }
    *result = FALSE;
    return NOERROR;
}

//================================================================================
//                      TtmlTrack
//================================================================================
TtmlTrack::TtmlTrack()
{}

TtmlTrack::~TtmlTrack()
{}

CAR_INTERFACE_IMPL_3(TtmlTrack, Object, ITtmlNodeListener, ISubtitleTrack, IMediaTimeProviderOnMediaTimeListener)

ECode TtmlTrack::constructor(
    /* [in] */ ITtmlRenderingWidget* renderingWidget,
    /* [in] */ IMediaFormat* format)
{
    SubtitleTrack::constructor(format);

    mRenderingWidget = renderingWidget;
    mParsingData = String("");
    return NOERROR;
}

ECode TtmlTrack::GetRenderingWidget(
    /* [out] */ ITtmlRenderingWidget** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ITtmlRenderingWidget::Probe(mRenderingWidget);
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode TtmlTrack::OnData(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Boolean eos,
    /* [in] */ Int64 runID)
{
    // try {
    // TODO: handle UTF-8 conversion properly
    String str(String(*data));

    // implement intermixing restriction for TTML.
    synchronized(mParser) {
        if (mCurrentRunID != NULL && runID != mCurrentRunID) {
            Slogger::E("TtmlTrack", "Run #%ld in progress.  Cannot process run #%d", mCurrentRunID, runID);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        mCurrentRunID = runID;
        mParsingData += str;
        if (eos) {
            // try {
            mParser->Parse(mParsingData, mCurrentRunID);
            // } catch (XmlPullParserException e) {
                // e.printStackTrace();
            // } catch (IOException e) {
                // e.printStackTrace();
            // }
            FinishedRun(runID);
            mParsingData = String("");
            mCurrentRunID = NULL;
        }
    }
    // } catch (java.io.UnsupportedEncodingException e) {
        // Log.w(TAG, "subtitle data is not UTF-8 encoded: " + e);
    // }
    return NOERROR;
}

ECode TtmlTrack::OnTtmlNodeParsed(
    /* [in] */ ITtmlNode* node)
{
    mTtmlNodes.PushBack((TtmlNode*)node);
    AddTimeEvents(node);
    return NOERROR;
}

ECode TtmlTrack::OnRootNodeParsed(
    /* [in] */ ITtmlNode* node)
{
    mRootNode = node;
    AutoPtr<TtmlCue> cue;
    GetNextResult(&cue);
    while (cue != NULL) {
        AddCue(cue);
    }
    mRootNode = NULL;
    mTtmlNodes.Clear();
    mTimeEvents.Clear();
    return NOERROR;
}

ECode TtmlTrack::UpdateView(
    /* [in] */ IVector* activeCues)
{
    if (!mVisible) {
        // don't keep the state if we are not visible
        return NOERROR;
    }

    if (DEBUG && mTimeProvider != NULL) {
        Int64 vol;
        ECode ec = mTimeProvider->GetCurrentTimeUs(FALSE, TRUE, &vol);
        if (SUCEEDED(ec)) {
            Slogger::D(TAG, "at %ld ms the active cues are:", vol/1000);
        }

        if (ec == (ECode)E_ILLEGAL_STATE_EXCEPTION) {
            Slogger::D(TAG, "at (illegal state) the active cues are:");
        }
        // try {
        // } catch (IllegalStateException e) {
            // Slogger::D(TAG, "at (illegal state) the active cues are:");
        // }
    }

    return mRenderingWidget->SetActiveCues(activeCues);
}

ECode TtmlTrack::GetNextResult(
    /* [out] */ ITtmlCue** result)
{
    VALIDATE_NOT_NULL(result);

    while (mTimeEvents.size() >= 2) {
        Int64 start = mTimeEvents.PopFront();
        Int64 end = mTimeEvents.First();
        AutoPtr<IList> activeCues;
        GetActiveNodes(start, end, (IList**)&activeCues);
        Boolean flag = FALSE;
        activeCues->IsEmpty(&flag);
        if (!flag) {
            AutoPtr<TtmlCue> cue = new TtmlCue();
            String ret;
            TtmlUtils::ExtractText(mRootNode, start, end, &ret);
            String str;
            TtmlUtils::ApplySpacePolicy(ret, FALSE, &str);
            String str_;
            TtmlUtils::ExtractTtmlFragment(mRootNode, start, end, &str_);
            cue->constructor(start, end, str, str_);
            *result = cue;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

void TtmlTrack::AddTimeEvents(ITtmlNode* node)
{
    mTimeEvents.Insert((TtmlNode*)node->mStartTimeMs);
    mTimeEvents.Insert((TtmlNode*)node->mEndTimeMs);
    List<AutoPtr<ITtmlNode> >::Iterator it = ((TtmlNode*)node->mChildren).Begin();
    for (; it != ((TtmlNode*)node->mChildren).End(); ++it) {
        AutoPtr<ITtmlNode> n = *it;
        AddTimeEvents(n);
    }
}

AutoPtr<List<AutoPtr<TtmlNode> > > TtmlTrack::GetActiveNodes(
    /* [in] */ Int64 startTimeUs,
    /* [in] */ Int64 endTimeUs)
{
    VALIDATE_NOT_NULL(result);
    Int32 size = mTtmlNodes.GetSize();
    AutoPtr<List<AutoPtr<TtmlNode> > > activeNodes = new List<AutoPtr<TtmlNode> >(size);
    List<AutoPtr<TtmlNode> >::Iterator it = mTtmlNodes.Begin();
    Boolean flag = FALSE;
    for (; it != mTtmlNodes.End(); ++it){
        AutoPtr<TtmlNode> node = *it;
        node->IsActive(startTimeUs, endTimeUs, &flag);
        if (flag) {
            activeNodes->PushBack(node);
        }
    }
    return activeNodes;

}

//================================================================================
//                      TtmlRenderingWidget
//================================================================================

TtmlRenderingWidget::TtmlRenderingWidget()
{}

TtmlRenderingWidget::~TtmlRenderingWidget()
{}

CAR_INTERFACE_IMPL_10(TtmlRenderingWidget, Object, ISubtitleTrackRenderingWidget, ITtmlRenderingWidget,
    ILinearLayout, IViewGroup, IViewParent, IViewManager, IView,
    IDrawableCallback, IKeyEventCallback, IAccessibilityEventSource)

ECode TtmlRenderingWidget::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode TtmlRenderingWidget::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode TtmlRenderingWidget::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode TtmlRenderingWidget::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    LinearLayout::constructor(context, attrs, defStyleAttr, defStyleRes);
    // Cannot render text over video when layer type is hardware.
    SetLayerType(View.LAYER_TYPE_SOFTWARE, null);

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::CAPTIONING_SERVICE, (IInterface**)&obj);
    AutoPtr<CaptioningManager> captionManager = (CaptioningManager*)ICaptioningManager::Probe(obj);
    AutoPtr<TextView> tv = new TextView();
    tv->constructor(context);
    mTextView = tv;
    AutoPtr<ICaptioningManagerCaptionStyle> style;
    captionManager->GetUserStyle((ICaptioningManagerCaptionStyle**)&style);
    Int32 color = (CaptioningManager::CaptionStyle*)style->mForegroundColor;
    mTextView->SetTextColor(color);
    AddView(mTextView, LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
    return mTextView->SetGravity(IGravity::BOTTOM | IGravity::CENTER_HORIZONTAL);
}

ECode TtmlRenderingWidget::SetOnChangedListener(
    /* [in] */ IOnChangedListener* listener)
{
    mListener = listener;
    return NOERROR;
}

ECode TtmlRenderingWidget::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Int32 widthSpec = MeasureSpec::MakeMeasureSpec(width, IMeasureSpec::EXACTLY);
    Int32 heightSpec = MeasureSpec::MakeMeasureSpec(height, IMeasureSpec::EXACTLY);

    Measure(widthSpec, heightSpec);
    return Layout(0, 0, width, height);
}

ECode TtmlRenderingWidget::SetVisible(
    /* [in] */ Boolean visible)
{
    if (visible) {
        SetVisibility(IView::VISIBLE);
    } else {
        SetVisibility(IView::GONE);
    }
    return NOERROR;
}

ECode TtmlRenderingWidget::OnAttachedToWindow()
{
    return LinearLayout::onAttachedToWindow();
}

ECode TtmlRenderingWidget::OnDetachedFromWindow()
{
    return LinearLayout::OnDetachedFromWindow();
}

ECode TtmlRenderingWidget::SetActiveCues(
    /* [in] */ IVector* activeCues)
{
    Int32 count;
    activeCues->GetSize(&count);
    String subtitleText(String(""));
    AutoPtr<IInterface> obj;
    for (Int32 i = 0; i < count; i++) {
        activeCues->Get(i, (IInterface**)&obj);
        AutoPtr<TtmlCue> cue =  (TtmlCue*)ISubtitleTrackCue::Probe(obj);
        subtitleText += cue->mText + String("\n");
    }
    mTextView->SetText(StringUtils::ParseCharSequence(subtitleText).Get());

    if (mListener != NULL) {
        mListener->OnChanged(this);
    }
    return NOERROR;
}

} // namespace Elastos
} // namespace Droid
} // namespace Media