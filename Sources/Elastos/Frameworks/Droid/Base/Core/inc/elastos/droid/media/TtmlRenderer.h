#ifndef _ELASTOS_DROID_MEDIA_TTMLRENDERER_H__
#define _ELASTOS_DROID_MEDIA_TTMLRENDERER_H__

#include "elastos/droid/media/SubtitleController.h"
#include <elastos/core/Object.h>

namespace Elastos{
namespace Droid {
namespace Media {

class TtmlRenderer
    : public ISubtitleControllerRenderer
    , public ITtmlRenderer
    , public Object
{
public:
    TtmlRenderer();

    virtual ~TtmlRenderer();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI Supports(
        /* [in] */ IMediaFormat* format,
        /* [out] */ Boolean* result);

    CARAPI CreateTrack(
        /* [in] */ IMediaFormat* format,
        /* [out] */ ISubtitleTrack** result);

private:
    AutoPtr<IContext> mContext;
    static const String MEDIA_MIMETYPE_TEXT_TTML;
    AutoPtr<TtmlRenderingWidget> mRenderingWidget;
};

class TtmlUtils
{
public:
    static CARAPI ParseTimeExpression(
        /* [in] */ const String& time,
        /* [in] */ Int32 frameRate,
        /* [in] */ Int32 subframeRate,
        /* [in] */ Int32 tickRate,
        /* [out] */ Int64** result);

    static CARAPI ApplyDefaultSpacePolicy(
        /* [in] */ const String& in,
        /* [out] */ String** result);

    static CARAPI ApplySpacePolicy(
        /* [in] */ const String& in,
        /* [in] */ Boolean treatLfAsSpace,
        /* [out] */ String* result);

    static CARAPI ExtractText(
        /* [in] */ TtmlNode* root,
        /* [in] */ Int64 startUs,
        /* [in] */ Int64 endUs,
        /* [out] */ String* result);

    static CARAPI ExtractTtmlFragment(
        /* [in] */ TtmlNode* root,
        /* [in] */ Int64 startUs,
        /* [in] */ Int64 endUs,
        /* [out] */ String* result);

private:
    static CARAPPI_(void) ExtractText(
        /* [in] */ TtmlNode* node,
        /* [in] */ Int64 startUs,
        /* [in] */ Int64 endUs,
        /* [in] */ StringBuilder& out,
        /* [in] */ Boolean inPTag);

private:
    static const AutoPtr<IPattern> CLOCK_TIME;
    static const AutoPtr<IPattern> OFFSET_TIME;
};

class TtmlCue
    : public SubtitleTrack::Cue
{
public:
    TtmlCue();

    virtual ~TtmlCue();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 endTimeMs,
        /* [in] */ const String& text,
        /* [in] */ const String& ttmlFragment);

public:
    String mText;
    String mTtmlFragment;
};

class TtmlNode
    : public Object
    , public ITtmlNode
{
public:
    TtmlNode();

    virtual ~TtmlNode();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& attributes,
        /* [in] */ const String& text,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 endTimeMs,
        /* [in] */ ITtmlNode* parent,
        /* [in] */ Int64 runId);

    CARAPI IsActive(
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 endTimeMs,
        /* [out] */ Boolean* result);

public:
    String mName;
    String mAttributes;
    ITtmlNode* mParent;
    String mText;
    List<AutoPtr<ITtmlNode> > mChildren;
    Int64 mRunId;
    Int64 mStartTimeMs;
    Int64 mEndTimeMs;
};

class TtmlParser
    : public Object
    , public ITtmlParser
{
public:
    TtmlParser();

    virtual ~TtmlParser();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ ITtmlNodeListener* listener);

    CARAPI Parse(
        /* [in] */ const String& ttmlText,
        /* [in] */ Int64 runId);
private:
    CARAPI_(void) LoadParser(
        /* [in] */ const String& ttmlFragment);

    CARAPI_(void) ExtractAttribute(
        /* [in] */ XmlPullParser* parser,
        /* [in] */ Int32 i,
        /* [in] */ StringBuilder& out);

    CARAPI ParseNode(
        /* [in] */ TtmlNode* parent,
        /* [out] */ TtmlNode** result);

    CARAPI IsEndOfDoc(
        /* [out] */ Boolean* result);

    static CARAPI IsSupportedTag(
        /* [in] */ const String& tag,
        /* [out] */ Boolean* result);
public:
    static const String TAG;

private:
    // TODO: read and apply the following attributes if specified.
    static const Int32 DEFAULT_FRAMERATE = 30;
    static const Int32 DEFAULT_SUBFRAMERATE = 1;
    static const Int32 DEFAULT_TICKRATE = 1;

    AutoPtr<IXmlPullParser> mParser;
    AutoPtr<ITtmlNodeListener> mListener;
    Int64 mCurrentRunId;
};

class TtmlTrack
    : public SubtitleTrack
    , public ITtmlNodeListener
{
public:
    TtmlTrack();

    virtual ~TtmlTrack();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ ITtmlRenderingWidget* renderingWidget,
        /* [in] */ IMediaFormat* format);

    CARAPI GetRenderingWidget(
        /* [out] */ ITtmlRenderingWidget** result);

    CARAPI OnData(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Boolean eos,
        /* [in] */ Int64 runID);

    CARAPI OnTtmlNodeParsed(
        /* [in] */ ITtmlNode* node);

    CARAPI OnRootNodeParsed(
        /* [in] */ ITtmlNode* node);

    CARAPI UpdateView(
        /* [in] */ IVector* activeCues);

    CARAPI GetNextResult(
        /* [out] */ ITtmlCue** result);

private:
    CARAPI_(void) AddTimeEvents(TtmlNode* node);

    AutoPtr<List<AutoPtr<TtmlNode> > > GetActiveNodes(
        /* [in] */ Int64 startTimeUs,
        /* [in] */ Int64 endTimeUs,
        /* [out] */ IList** result);
private:
    static const String TAG = "TtmlTrack";

    AutoPtr<TtmlParser> mParser = new TtmlParser(this);
    AutoPtr<TtmlRenderingWidget> mRenderingWidget;
    String mParsingData;
    Int64 mCurrentRunID;

    List<AutoPtr<TtmlNode> >  mTtmlNodes;
    Set<Int64> mTimeEvents;
    AutoPtr<TtmlNode> mRootNode;
};

class TtmlRenderingWidget
    : public LinearLayout
    , public ISubtitleTrackRenderingWidget
    , public ITtmlRenderingWidget
{
public:
    TtmlRenderingWidget();

    virtual ~TtmlRenderingWidget();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    CARAPI SetOnChangedListener(
        /* [in] */ IOnChangedListener* listener);

    CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI SetActiveCues(
        /* [in] */ IVector* activeCues);

private:
    /** Callback for rendering changes. */
    AutoPtr<IOnChangedListener> mListener;
    AutoPtr<TextView> mTextView;
};

} // namespace Elastos
} // namespace Droid
} // namespace Media

#endif // _ELASTOS_DROID_MEDIA_TTMLRENDERER_H__