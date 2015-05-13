
#include "content/res/CColorStateList.h"
#include "util/StateSet.h"
#include "util/ArrayUtils.h"
#include "util/Xml.h"
#include "R.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

static AutoPtr< ArrayOf<Handle32> > InitEMPTY()
{
    AutoPtr< ArrayOf<Int32> > state = ArrayOf<Int32>::Alloc(1);
    (*state)[0] = 0;
    AutoPtr< ArrayOf<Handle32> > states = ArrayOf<Handle32>::Alloc(1);
    (*states)[0] = (Handle32)state.Get();
    state->AddRef();
    return states;
}

const String CColorStateList::TAG = String("CColorStateList");
AutoPtr< ArrayOf<Handle32> > CColorStateList::EMPTY = InitEMPTY();
HashMap<Int32, AutoPtr<IColorStateList> > CColorStateList::sCache;
Mutex CColorStateList::sCacheLock;

CColorStateList::CColorStateList()
    : mDefaultColor(0xffff0000)
{}

CColorStateList::~CColorStateList()
{
    if (mStateSpecs != NULL) {
        for (Int32 i = 0; i < mStateSpecs->GetLength(); i++) {
            ArrayOf<Int32>* state = (ArrayOf<Int32>*)(*mStateSpecs)[i];
            state->Release();
        }
    }
}

ECode CColorStateList::ValueOf(
    /* [in] */ Int32 color,
    /* [out] */ IColorStateList** _csl)
{
    VALIDATE_NOT_NULL(_csl);
    *_csl = NULL;

    //TODO: should we collect these eventually?
    Mutex::Autolock lock(sCacheLock);

    AutoPtr<IColorStateList> csl;
    HashMap<Int32, AutoPtr<IColorStateList> >::Iterator it = sCache.Find(color);
    if (it != sCache.End()) csl = it->mSecond;

    if (csl == NULL) {
        AutoPtr< ArrayOf<Int32> > colors = ArrayOf<Int32>::Alloc(1);
        (*colors)[0] = color;
        AutoPtr<CColorStateList> ccsl;
        FAIL_RETURN(CColorStateList::NewByFriend(EMPTY, colors, (CColorStateList**)&ccsl));
        csl = ccsl;
        sCache[color] = csl;
    }

    *_csl = csl.Get();
    INTERFACE_ADDREF(*_csl);
    return NOERROR;
}

ECode CColorStateList::CreateFromXml(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [out] */ IColorStateList** csl)
{
    VALIDATE_NOT_NULL(csl);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    Int32 type;
    FAIL_RETURN(parser->Next(&type));
    while (type != IXmlPullParser::START_TAG
            && type != IXmlPullParser::END_DOCUMENT) {
        FAIL_RETURN(parser->Next(&type));
    }

    if (type != IXmlPullParser::START_TAG) {
        // throw new XmlPullParserException("No start tag found");
        Slogger::E(TAG, "CColorStateList No start tag found");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    return CreateFromXmlInner(r, parser, attrs, csl);
}

ECode CColorStateList::CreateFromXmlInner(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IColorStateList** csl)
{
    String name;
    FAIL_RETURN(parser->GetName((String*)&name));

    if (name.Equals("selector")) {
        AutoPtr<CColorStateList> ccsl;
        FAIL_RETURN(CColorStateList::NewByFriend((CColorStateList**)&ccsl));
        *csl = ccsl;
        INTERFACE_ADDREF(*csl);
    }
    else {
        String pos;
        parser->GetPositionDescription(&pos);
        Slogger::E(TAG, "CColorStateList%s : invalid drawable tag %s",
                (const char*)pos, (const char*)name);
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    return ((CColorStateList*)(*csl))->Inflate(r, parser, attrs);
}

ECode CColorStateList::WithAlpha(
    /* [in] */ Int32 alpha,
    /* [out] */ IColorStateList** colorState)
{
    VALIDATE_NOT_NULL(colorState);

    Int32 length = mColors->GetLength();
    AutoPtr< ArrayOf<Int32> > colors = ArrayOf<Int32>::Alloc(length);

    for (Int32 i = 0; i < length; i++) {
        (*colors)[i] = ((*mColors)[i] & 0xFFFFFF) | (alpha << 24);
    }

    return CColorStateList::New(mStateSpecs, colors, colorState);
}

ECode CColorStateList::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    Int32 innerDepth;
    parser->GetDepth(&innerDepth);
    innerDepth++;

    Int32 depth;

    Int32 listAllocated = 20;
    Int32 listSize = 0;
    AutoPtr< ArrayOf<Int32> > colorList = ArrayOf<Int32>::Alloc(listAllocated);
    AutoPtr< ArrayOf<Handle32> > stateSpecList = ArrayOf<Handle32>::Alloc(listAllocated);

    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && ((parser->GetDepth(&depth), depth) >= innerDepth
                    || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (depth > innerDepth || !name.Equals("item")) {
            continue;
        }

        Int32 colorRes = 0;
        Int32 color = 0xffff0000;
        Boolean haveColor = FALSE;

        Int32 i;
        Int32 j = 0;
        Int32 numAttrs;
        attrs->GetAttributeCount(&numAttrs);
        AutoPtr< ArrayOf<Int32> > stateSpec = ArrayOf<Int32>::Alloc(numAttrs);
        for (i = 0; i < numAttrs; i++) {
            Int32 stateResId;
            attrs->GetAttributeNameResource(i, &stateResId);

            if (stateResId == 0)
                break;

            if (stateResId == R::attr::color) {
                attrs->GetAttributeResourceValueEx(i, 0, &colorRes);

                if (colorRes == 0) {
                    attrs->GetAttributeIntValueEx(i, color, &color);
                    haveColor = TRUE;
                }
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValueEx(i, FALSE, &value);
                (*stateSpec)[j++] = value ? stateResId : -stateResId;
            }
        }
        AutoPtr< ArrayOf<Int32> > temp = StateSet::TrimStateSet(stateSpec, j);
        stateSpec = temp;

        if (colorRes != 0) {
            r->GetColor(colorRes, &color);
        }
        else if (!haveColor) {
            String pos;
            parser->GetPositionDescription(&pos);
            Slogger::E(TAG, "CColorStateList %s: <item> tag requires a 'android:color' attribute.",
                    (const char*)pos);
            return E_XML_PULL_PARSER_EXCEPTION;
        }

        if (listSize == 0 || stateSpec->GetLength() == 0) {
            mDefaultColor = color;
        }

        if (listSize + 1 >= listAllocated) {
            listAllocated = ArrayUtils::IdealInt32ArraySize(listSize + 1);

            AutoPtr< ArrayOf<Int32> > ncolor = ArrayOf<Int32>::Alloc(listAllocated);
            ncolor->Copy(colorList, 0, listSize);

            AutoPtr< ArrayOf<Handle32> > nstate = ArrayOf<Handle32>::Alloc(listAllocated);
            nstate->Copy(stateSpecList, 0, listSize);

            colorList = ncolor;
            stateSpecList = nstate;
        }

        (*colorList)[listSize] = color;
        (*stateSpecList)[listSize] = (Handle32)stateSpec.Get();
        stateSpec->AddRef();
        listSize++;
    }

    mColors = ArrayOf<Int32>::Alloc(listSize);
    mColors->Copy(colorList, 0, listSize);

    mStateSpecs = ArrayOf<Handle32>::Alloc(listSize);
    mStateSpecs->Copy(stateSpecList, 0, listSize);

    return NOERROR;
}

ECode CColorStateList::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = mStateSpecs->GetLength() > 1;

    return NOERROR;
}

ECode CColorStateList::GetColorForState(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [in] */ Int32 defaultColor,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    Int32 setLength = mStateSpecs->GetLength();
    for (Int32 i = 0; i < setLength; i++) {
        ArrayOf<Int32>* stateSpec = (ArrayOf<Int32>*)(*mStateSpecs)[i];
        if (StateSet::StateSetMatches(stateSpec, stateSet)) {
            *color = (*mColors)[i];
            return NOERROR;
        }
    }
    *color = defaultColor;

    return NOERROR;
}

ECode CColorStateList::GetDefaultColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    *color = mDefaultColor;

    return NOERROR;
}

ECode CColorStateList::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Int32 N = mStateSpecs->GetLength();
    dest->WriteInt32(N);
    for (int i = 0; i < N; i++) {
        dest->WriteArrayOf((*mStateSpecs)[i]);
    }
    dest->WriteArrayOf((Handle32)mColors.Get());
    return NOERROR;
}

ECode CColorStateList::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 N;
    source->ReadInt32(&N);
    AutoPtr< ArrayOf<Handle32> > stateSpec = ArrayOf<Handle32>::Alloc(N);
    for (Int32 i = 0; i < N; i++) {
        source->ReadArrayOf((Handle32*)&(*stateSpec)[i]);
    }
    AutoPtr< ArrayOf<Int32> > colors;
    source->ReadArrayOf((Handle32*)&colors);

    assert(stateSpec->GetLength() == colors->GetLength());
    mStateSpecs = stateSpec->Clone();
    for (Int32 i = 0; i < colors->GetLength(); i++) {
        (*mStateSpecs)[i] = (Handle32)((ArrayOf<Int32>*)(*colors)[i])->Clone();
    }
    mColors = colors->Clone();

    if (colors->GetLength() > 0) {
        mDefaultColor = (*colors)[0];

        for (Int32 i = 0; i < colors->GetLength(); i++) {
            if (((ArrayOf<Int32>*)(*colors)[i])->GetLength() == 0) {
                mDefaultColor = (*colors)[i];
            }
        }
    }
    return NOERROR;
}

ECode CColorStateList::constructor()
{
    return NOERROR;
}

ECode CColorStateList::constructor(
    /* [in] */ ArrayOf<Handle32>* states,
    /* [in] */ ArrayOf<Int32>* colors)
{
    mStateSpecs = states;
    mColors = colors;

    if (states->GetLength() > 0) {
        mDefaultColor = (*colors)[0];

        for (Int32 i = 0; i < states->GetLength(); ++i) {
            if (((ArrayOf<Int32>*)(*states)[i])->GetLength() == 0) {
                mDefaultColor = (*colors)[i];
            }
        }
    }
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

