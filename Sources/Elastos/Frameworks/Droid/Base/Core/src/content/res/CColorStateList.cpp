
#include "content/res/CColorStateList.h"
#include "util/StateSet.h"
#include "util/ArrayUtils.h"
#include "util/Xml.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

static AutoPtr< ArrayOf<Int32Array > > InitEMPTY()
{
    AutoPtr< ArrayOf<Int32> > state = ArrayOf<Int32>::Alloc(1);
    (*state)[0] = 0;
    AutoPtr< ArrayOf<Int32Array > > states = ArrayOf<Int32Array >::Alloc(1);
    states->Set(0, state);
    return states;
}

const String CColorStateList::TAG("CColorStateList");
AutoPtr< ArrayOf<Int32Array > > CColorStateList::EMPTY = InitEMPTY();
HashMap<Int32, AutoPtr<IWeakReference> > CColorStateList::sCache;
Object CColorStateList::sCacheLock;

CAR_INTERFACE_IMPL(CColorStateList, Object, IColorStateList)

CAR_OBJECT_IMPL(CColorStateList)

CColorStateList::CColorStateList()
    : mDefaultColor(0xffff0000)
{}

CColorStateList::~CColorStateList()
{
}

ECode CColorStateList::ValueOf(
    /* [in] */ Int32 color,
    /* [out] */ IColorStateList** _csl)
{
    VALIDATE_NOT_NULL(_csl);
    *_csl = NULL;

    //TODO: should we collect these eventually?
    AutoLock lock(sCacheLock);

    AutoPtr<IColorStateList> csl;
    HashMap<Int32, AutoPtr<IWeakReference> >::Iterator it = sCache.Find(color);
    if (it != sCache.End()) {
        IWeakReference* wr = it->mSecond;
        wr->Resolve(EIID_IColorStateList, (IInterface**)&csl);
    }

    if (csl == NULL) {
        AutoPtr< ArrayOf<Int32> > colors = ArrayOf<Int32>::Alloc(1);
        (*colors)[0] = color;
        AutoPtr< ArrayOf<Handle32> > emptyArray = ArrayOf<Handle32>::Alloc(1);
        emptyArray->Set(0, (Handle32)(*EMPTY)[0].Get());
        AutoPtr<CColorStateList> ccsl;
        FAIL_RETURN(CColorStateList::NewByFriend(emptyArray, colors, (CColorStateList**)&ccsl));
        csl = ccsl;
        AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(csl);
        assert(wrs != NULL);
        AutoPtr<IWeakReference> wr;
        wrs->GetWeakReference((IWeakReference**)&wr);
        sCache[color] = wr;
    }

    *_csl = csl.Get();
    REFCOUNT_ADD(*_csl);
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
        REFCOUNT_ADD(*csl);
    }
    else {
        String pos;
        parser->GetPositionDescription(&pos);
        Slogger::E(TAG, "CColorStateList%s : invalid drawable tag %s",
            pos.string(), name.string());
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

    AutoPtr< ArrayOf<Handle32> > array = ArrayOf<Handle32>::Alloc(mStateSpecs->GetLength());
    for (Int32 i = 0; i < mStateSpecs->GetLength(); ++i) {
        array->Set(i, (Handle32)(*mStateSpecs)[i].Get());
    }
    return CColorStateList::New(array, colors, colorState);
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
    AutoPtr< ArrayOf<Int32Array > > stateSpecList = ArrayOf<Int32Array >::Alloc(listAllocated);

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
                attrs->GetAttributeResourceValue(i, 0, &colorRes);

                if (colorRes == 0) {
                    attrs->GetAttributeIntValue(i, color, &color);
                    haveColor = TRUE;
                }
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValue(i, FALSE, &value);
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

            AutoPtr< ArrayOf<Int32Array > > nstate = ArrayOf<Int32Array >::Alloc(listAllocated);
            nstate->Copy(stateSpecList, 0, listSize);

            colorList = ncolor;
            stateSpecList = nstate;
        }

        colorList->Set(listSize, color);
        stateSpecList->Set(listSize, stateSpec.Get());
        listSize++;
    }

    mColors = ArrayOf<Int32>::Alloc(listSize);
    mColors->Copy(colorList, 0, listSize);

    mStateSpecs = ArrayOf<Int32Array >::Alloc(listSize);
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
        ArrayOf<Int32>* stateSpec = (*mStateSpecs)[i];
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
        dest->WriteArrayOf((Handle32)(*mStateSpecs)[i].Get());
    }
    dest->WriteArrayOf((Handle32)mColors.Get());
    return NOERROR;
}

ECode CColorStateList::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 N;
    source->ReadInt32(&N);
    AutoPtr< ArrayOf<Int32Array > > stateSpec = ArrayOf<Int32Array >::Alloc(N);
    for (Int32 i = 0; i < N; i++) {
        source->ReadArrayOf((Handle32*)&(*stateSpec)[i]);
    }
    AutoPtr< ArrayOf<Int32> > colors;
    source->ReadArrayOf((Handle32*)&colors);

    assert(stateSpec->GetLength() == colors->GetLength());

    mStateSpecs = stateSpec;
    mColors = colors;

    if (mStateSpecs && mStateSpecs->GetLength() > 0) {
        mDefaultColor = (*colors)[0];

        for (Int32 i = 0; i < mStateSpecs->GetLength(); ++i) {
            if ((*mStateSpecs)[i]->GetLength() == 0) {
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
    mColors = colors;

    if (states) {
        mStateSpecs = ArrayOf<Int32Array >::Alloc(states->GetLength());
        Int32Array array;
        for (Int32 i = 0; i < states->GetLength(); ++i) {
            array = (ArrayOf<Int32>*)(*states)[i];
            mStateSpecs->Set(i, array);
        }
    }
    else {
        mStateSpecs = NULL;
    }

    if (mStateSpecs && mStateSpecs->GetLength() > 0) {
        mDefaultColor = (*colors)[0];

        for (Int32 i = 0; i < mStateSpecs->GetLength(); ++i) {
            if ((*mStateSpecs)[i]->GetLength() == 0) {
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

