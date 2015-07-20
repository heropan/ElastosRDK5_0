
#include "content/res/CResources.h"
#include "content/res/CColorStateList.h"
#include "content/res/XmlBlock.h"
#include "graphics/Movie.h"
#include "util/XmlUtils.h"
#include "graphics/drawable/CColorDrawable.h"
#include "graphics/drawable/Drawable.h"
#include "content/pm/CActivityInfoHelper.h"
#include "os/Build.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Libcore::ICU::INativePluralRulesHelper;
using Libcore::ICU::CNativePluralRulesHelper;
using Libcore::ICU::INativePluralRules;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocale;
using Elastos::Core::StringUtils;
using Elastos::Core::IAppendable;
using Elastos::Core::StringBuilder;
using Elastos::Utility::IFormatter;
using Elastos::Utility::CFormatter;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Graphics::Movie;
using Elastos::Droid::Graphics::Drawable::CColorDrawable;
using Elastos::Droid::Graphics::Drawable::IColorDrawable;
using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableConstantState;
using Elastos::Droid::Content::Pm::IActivityInfoHelper;
using Elastos::Droid::Content::Pm::CActivityInfoHelper;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CResources::Theme::Theme(
    /* [in] */ CResources* host)
    : mHost(host)
    , mAssets(host->mAssets)
{
    mAssets->CreateTheme(&mTheme);
}

CResources::Theme::~Theme()
{
    mAssets->ReleaseTheme(mTheme);
}

PInterface CResources::Theme::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IResourcesTheme*)this;
    }
    else if (riid == EIID_IResourcesTheme) {
        return (IResourcesTheme*)this;
    }
    return NULL;
}

UInt32 CResources::Theme::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CResources::Theme::Release()
{
    return ElRefBase::Release();
}

ECode CResources::Theme::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CResources::Theme::ApplyStyle(
    /* [in] */ Int32 resid,
    /* [in] */ Boolean force)
{
    CAssetManager::ApplyThemeStyle(mTheme, resid, force);
    return NOERROR;
}

ECode CResources::Theme::SetTo(
    /* [in] */ IResourcesTheme* other)
{
    CAssetManager::CopyTheme(mTheme, ((Theme*)other)->mTheme);
    return NOERROR;
}

ECode CResources::Theme::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);

    Int32 len = attrs->GetLength();
    AutoPtr<CTypedArray> array = mHost->GetCachedStyledAttributes(len);
    assert(array);
    array->mRsrcs = attrs;
    Boolean result;
    FAIL_RETURN(CAssetManager::ApplyStyle(mTheme, 0, 0, 0, *attrs,
            array->mData, array->mIndices, &result));
    *styles = (ITypedArray*)array;
    REFCOUNT_ADD(*styles);
    return NOERROR;
}

ECode CResources::Theme::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);

    Int32 len = attrs->GetLength();
    AutoPtr<CTypedArray> array = mHost->GetCachedStyledAttributes(len);
    assert(array);
    array->mRsrcs = attrs;

    Boolean result;
    FAIL_RETURN(CAssetManager::ApplyStyle(mTheme, 0, resid, 0, *attrs,
            array->mData, array->mIndices, &result));
    // if (false) {
    //     int[] data = array.mData;

    //     System.out.println("**********************************************************");
    //     System.out.println("**********************************************************");
    //     System.out.println("**********************************************************");
    //     System.out.println("Attributes:");
    //     String s = "  Attrs:";
    //     int i;
    //     for (i=0; i<attrs.length; i++) {
    //         s = s + " 0x" + Integer.toHexString(attrs[i]);
    //     }
    //     System.out.println(s);
    //     s = "  Found:";
    //     TypedValue value = new TypedValue();
    //     for (i=0; i<attrs.length; i++) {
    //         int d = i*AssetManager.STYLE_NUM_ENTRIES;
    //         value.type = data[d+AssetManager.STYLE_TYPE];
    //         value.data = data[d+AssetManager.STYLE_DATA];
    //         value.assetCookie = data[d+AssetManager.STYLE_ASSET_COOKIE];
    //         value.resourceId = data[d+AssetManager.STYLE_RESOURCE_ID];
    //         s = s + " 0x" + Integer.toHexString(attrs[i])
    //             + "=" + value;
    //     }
    //     System.out.println(s);
    // }
    *styles = (ITypedArray*)array;
    REFCOUNT_ADD(*styles);
    return NOERROR;
}

ECode CResources::Theme::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    VALIDATE_NOT_NULL(attrs);

    Int32 len = attrs->GetLength();
    AutoPtr<CTypedArray> array = mHost->GetCachedStyledAttributes(len);
    assert(array);

    // XXX note that for now we only work with compiled XML files.
    // To support generic XML files we will need to manually parse
    // out the attributes from the XML file (applying type information
    // contained in the resources and such).
    //begin from this
    XmlBlock::Parser* parser = (XmlBlock::Parser*)set;
    Boolean result;
    FAIL_RETURN(CAssetManager::ApplyStyle(
        mTheme, defStyleAttr, defStyleRes,
        parser != NULL ? parser->mParseState : 0, *attrs,
                array->mData, array->mIndices, &result));

    array->mRsrcs = attrs;
    array->mXml = parser;

    // if (false) {
    //     int[] data = array.mData;

    //     System.out.println("Attributes:");
    //     String s = "  Attrs:";
    //     int i;
    //     for (i=0; i<set.getAttributeCount(); i++) {
    //         s = s + " " + set.getAttributeName(i);
    //         int id = set.getAttributeNameResource(i);
    //         if (id != 0) {
    //             s = s + "(0x" + Integer.toHexString(id) + ")";
    //         }
    //         s = s + "=" + set.getAttributeValue(i);
    //     }
    //     System.out.println(s);
    //     s = "  Found:";
    //     TypedValue value = new TypedValue();
    //     for (i=0; i<attrs.length; i++) {
    //         int d = i*AssetManager.STYLE_NUM_ENTRIES;
    //         value.type = data[d+AssetManager.STYLE_TYPE];
    //         value.data = data[d+AssetManager.STYLE_DATA];
    //         value.assetCookie = data[d+AssetManager.STYLE_ASSET_COOKIE];
    //         value.resourceId = data[d+AssetManager.STYLE_RESOURCE_ID];
    //         s = s + " 0x" + Integer.toHexString(attrs[i])
    //             + "=" + value;
    //     }
    //     System.out.println(s);
    // }
    *styles = (ITypedArray*)array;
    REFCOUNT_ADD(*styles);
    return NOERROR;
}

ECode CResources::Theme::ResolveAttribute(
    /* [in] */ Int32 resid,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs,
    /* [out] */ Boolean* isFounded)
{
    VALIDATE_NOT_NULL(isFounded);

    *isFounded = mAssets->GetThemeValue(mTheme, resid, outValue, resolveRefs);
    // if (false) {
    //     System.out.println(
    //         "resolveAttribute #" + Integer.toHexString(resid)
    //         + " got=" + got + ", type=0x" + Integer.toHexString(outValue.type)
    //         + ", data=0x" + Integer.toHexString(outValue.data));
    // }
    return NOERROR;
}

ECode CResources::Theme::Dump(
    /* [in] */ Int32 priority,
    /* [in] */ const String& tag,
    /* [in] */ const String& prefix)
{
    CAssetManager::DumpTheme(mTheme, priority, tag, prefix);
    return NOERROR;
}


const String CResources::TAG("CResources");
const String CResources::WIDGET_SUFFIX("widget_preview");
const Boolean CResources::DEBUG_LOAD = FALSE;
const Boolean CResources::DEBUG_CONFIG = FALSE;
const Boolean CResources::DEBUG_ATTRIBUTES_CACHE = FALSE;
const Boolean CResources::TRACE_FOR_PRELOAD = FALSE;
const Boolean CResources::TRACE_FOR_MISS_PRELOAD = FALSE;
const Int32 CResources::ID_OTHER = 0x01000004;

AutoPtr<IResources> CResources::mSystem;
CResources::DrawableMap CResources::sPreloadedDrawables;
CResources::ColorStateMap CResources::sPreloadedColorStateLists;
CResources::DrawableMap CResources::sPreloadedColorDrawables;
Boolean CResources::sPreloaded = FALSE;
Int32 CResources::sPreloadedDensity = 0;

Mutex CResources::mSyncLock;

CResources::CResources()
    : mPreloading(FALSE)
    , mLastCachedXmlBlockIndex(-1)
{
    ASSERT_SUCCEEDED(CTypedValue::NewByFriend((CTypedValue**)&mTmpValue));
    ASSERT_SUCCEEDED(CConfiguration::NewByFriend((CConfiguration**)&mTmpConfig));

    mCachedXmlBlockIds = ArrayOf<Int32>::Alloc(4);
    mCachedXmlBlocks = ArrayOf<XmlBlock*>::Alloc(4);

    ASSERT_SUCCEEDED(CConfiguration::NewByFriend((CConfiguration**)&mConfiguration));
    ASSERT_SUCCEEDED(CDisplayMetrics::NewByFriend((CDisplayMetrics**)&mMetrics));
}

CResources::~CResources()
{
    mAssets = NULL;
    mCachedXmlBlocks = NULL;
}

Int32 CResources::SelectDefaultTheme(
    /* [in] */ Int32 curTheme,
    /* [in] */ Int32 targetSdkVersion)
{
    return SelectSystemTheme(curTheme, targetSdkVersion,
            R::style::Theme, R::style::Theme_Holo, R::style::Theme_DeviceDefault);
}

Int32 CResources::SelectSystemTheme(
    /* [in] */ Int32 curTheme,
    /* [in] */ Int32 targetSdkVersion,
    /* [in] */ Int32 orig,
    /* [in] */ Int32 holo,
    /* [in] */ Int32 deviceDefault)
{
    if (curTheme != 0) {
        return curTheme;
    }
    if (targetSdkVersion < Build::VERSION_CODES::HONEYCOMB) {
        return orig;
    }
    if (targetSdkVersion < Build::VERSION_CODES::ICE_CREAM_SANDWICH) {
        return holo;
    }
    return deviceDefault;
}

AutoPtr<IResources> CResources::GetSystem()
{
    AutoLock lock(mSyncLock);

    AutoPtr<IResources> ret = mSystem;
    if (ret == NULL) {
        AutoPtr<CResources> cres;
        CResources::NewByFriend((CResources**)&cres);
        ret = cres;
        mSystem = ret;
    }

    return ret;
}

ECode CResources::GetText(
    /* [in] */ Int32 id,
    /* [out] */ ICharSequence** seq)
{
    VALIDATE_NOT_NULL(seq);
    *seq = NULL;

    AutoPtr<ICharSequence> temp = mAssets->GetResourceText(id);
    if (temp != NULL) {
        *seq = temp;
        REFCOUNT_ADD(*seq);
        return NOERROR;
    }

    Slogger::E(TAG, "String resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetQuantityText(
    /* [in] */ Int32 id,
    /* [in] */ Int32 quantity,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    *csq = NULL;

    AutoPtr<INativePluralRulesHelper> helper;
    CNativePluralRulesHelper::AcquireSingleton((INativePluralRulesHelper **)&helper);
    AutoPtr<INativePluralRules> rules;
    helper->ForLocale(mConfiguration->mLocale , (INativePluralRules **)&rules);
    Int32 value;
    rules->QuantityForInt(quantity, &value);
    AutoPtr<ICharSequence> temp = mAssets->GetResourceBagText(id, AttrForQuantityCode(value));
    if (temp != NULL) {
        *csq = temp;
        REFCOUNT_ADD(*csq);
        return NOERROR;
    }

    temp = mAssets->GetResourceBagText(id, ID_OTHER);
    if (temp != NULL) {
        *csq = temp;
        REFCOUNT_ADD(*csq);
        return NOERROR;
    }

    Slogger::E(TAG, "Plural resource ID #0x%08x quantity=%d item=%s", id, quantity,
             StringForQuantityCode(value).string());
    return E_NOT_FOUND_EXCEPTION;
}

// AutoPtr<NativePluralRules> CResources::GetPluralRule()
// {
//     AutoLock lock(_m_syncLock);
//     if (mPluralRule == NULL) {
//         mPluralRule = NativePluralRules::ForLocale(mConfiguration->mLocale);
//     }
//     return mPluralRule;
// }

Int32 CResources::AttrForQuantityCode(
    /* [in] */ Int32 quantityCode)
{
    Int32 ret;
    switch (quantityCode) {
        case INativePluralRules::ZERO:
            ret = 0x01000005;
            break;
        case INativePluralRules::ONE:
            ret = 0x01000006;
            break;
        case INativePluralRules::TWO:
            ret = 0x01000007;
            break;
        case INativePluralRules::FEW:
            ret = 0x01000008;
            break;
        case INativePluralRules::MANY:
            ret = 0x01000009;
            break;
        default:
            ret = ID_OTHER;
    }
    return ret;
}

String CResources::StringForQuantityCode(
    /* [in] */ Int32 quantityCode)
{
    String ret;
    switch (quantityCode) {
        case INativePluralRules::ZERO:
            ret = "zero";
            break;
        case INativePluralRules::ONE:
            ret = "one";
            break;
        case INativePluralRules::TWO:
            ret = "two";
            break;
        case INativePluralRules::FEW:
            ret = "few";
            break;
        case INativePluralRules::MANY:
            ret = "many";
            break;
        default:
            ret = "other";
    }
    return ret;
}

ECode CResources::GetString(
    /* [in] */ Int32 id,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ICharSequence> res;
    if (SUCCEEDED(GetText(id, (ICharSequence**)&res)) && res != NULL) {
        res->ToString(str);
        return NOERROR;
    }

    Slogger::E(TAG, "String resource ID #0x%08x", id);
    *str = NULL;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetString(
    /* [in] */ Int32 id,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    String raw;
    FAIL_RETURN(GetString(id, &raw));
    if (raw.IsNull()) {
        // throw new NullPointerException("format == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 bufferSize = raw.GetLength() + (formatArgs == NULL ? 0 : formatArgs->GetLength() * 10);
    AutoPtr<IAppendable> a = new StringBuilder(bufferSize);
    AutoPtr<Elastos::Utility::IFormatter> f;
    Elastos::Utility::CFormatter::New(a, mConfiguration->mLocale, (IFormatter**)&f);
    FAIL_RETURN(f->Format(raw, formatArgs));
    return f->ToString(str);
}

ECode CResources::GetQuantityString(
    /* [in] */ Int32 id,
    /* [in] */ Int32 quantity,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ICharSequence> res;
    ECode ec = GetQuantityText(id, quantity, (ICharSequence**)&res);
    if (SUCCEEDED(ec) && res != NULL) {
        String raw;
        res->ToString(&raw);
        assert(0);
        // return String.format(mConfiguration.locale, raw, formatArgs);
        return NOERROR;
    }

    *str = NULL;
    return ec;
}

ECode CResources::GetQuantityString(
    /* [in] */ Int32 id,
    /* [in] */ Int32 quantity,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    AutoPtr<ICharSequence> res;
    ECode ec = GetQuantityText(id, quantity, (ICharSequence**)&res);
    if (SUCCEEDED(ec) && res != NULL) {
        res->ToString(str);
        return NOERROR;
    }

    *str = NULL;
    return ec;
}

ECode CResources::GetText(
    /* [in] */ Int32 id,
    /* [in] */ ICharSequence* def,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    AutoPtr<ICharSequence> temp;
    if (id != 0) {
        temp = mAssets->GetResourceText(id);
    }
    if (temp != NULL) {
        *csq = temp;
    }
    else *csq = def;

    REFCOUNT_ADD(*csq);
    return NOERROR;
}

ECode CResources::GetTextArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<ICharSequence*>** seqs)
{
    VALIDATE_NOT_NULL(seqs);
    *seqs = NULL;

    AutoPtr< ArrayOf<ICharSequence*> > temp = mAssets->GetResourceTextArray(id);
    if (temp) {
        *seqs = temp;
        REFCOUNT_ADD(*seqs);
        return NOERROR;
    }

    Slogger::E(TAG, "Text array resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetStringArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<String>** strs)
{
    VALIDATE_NOT_NULL(strs);
    *strs = NULL;

    AutoPtr< ArrayOf<String> > temp = mAssets->GetResourceStringArray(id);
    if (temp) {
        *strs = temp;
        REFCOUNT_ADD(*strs);
        return NOERROR;
    }

    Slogger::E(TAG, "String array resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetIntArray(
    /* [in] */ Int32 id,
    /* [out, callee] */ ArrayOf<Int32>** ints)
{
    VALIDATE_NOT_NULL(ints);
    *ints = NULL;

    AutoPtr< ArrayOf<Int32> > temp;
    FAIL_RETURN(mAssets->GetArrayIntResource(id, (ArrayOf<Int32>**)&temp));
    if (temp) {
        *ints = temp;
        ARRAYOF_ADDREF(*ints);
        return NOERROR;
    }

    Slogger::E(TAG, "Int array resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::ObtainTypedArray(
    /* [in] */ Int32 id,
    /* [out] */ ITypedArray** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    Int32 len = mAssets->GetArraySize(id);
    if (len < 0) {
        Slogger::E(TAG, "Array resource ID #0x%08x", id);
        return E_NOT_FOUND_EXCEPTION;
    }

    AutoPtr<CTypedArray> temp = GetCachedStyledAttributes(len);
    FAIL_RETURN(mAssets->RetrieveArray(id, temp->mData, &temp->mLength))
    (*temp->mIndices)[0] = 0;

    *array = (ITypedArray*)temp.Get();
    REFCOUNT_ADD(*array);
    return NOERROR;
}

ECode CResources::GetDimension(
    /* [in] */ Int32 id,
    /* [out] */ Float* dim)
{
    VALIDATE_NOT_NULL(dim);

    AutoLock lock(mTmpValueLock);

    FAIL_RETURN(GetValue(id, (ITypedValue*)mTmpValue.Get(), TRUE))
    if (mTmpValue->mType == ITypedValue::TYPE_DIMENSION) {
        *dim = CTypedValue::ComplexToDimension(mTmpValue->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *dim = 0.0f;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDimensionPixelOffset(
    /* [in] */ Int32 id,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType == ITypedValue::TYPE_DIMENSION) {
        *offset = CTypedValue::ComplexToDimensionPixelOffset(
                mTmpValue->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *offset = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDimensionPixelSize(
    /* [in] */ Int32 id,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType == ITypedValue::TYPE_DIMENSION) {
        *size = CTypedValue::ComplexToDimensionPixelSize(
                mTmpValue->mData,
                (IDisplayMetrics*)mMetrics.Get());
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *size = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetFraction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 base,
    /* [in] */ Int32 pbase,
    /* [out] */ Float* fraction)
{
    VALIDATE_NOT_NULL(fraction);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType == ITypedValue::TYPE_FRACTION) {
        *fraction = CTypedValue::ComplexToFraction(mTmpValue->mData, base, pbase);
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *fraction = 0.0f;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDrawable(
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoLock lock(mTmpValueLock);
    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    return LoadDrawable(value, id, drawable);
}

ECode CResources::GetDrawableForDensity(
    /* [in] */ Int32 id,
    /* [in] */ Int32 density,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoLock lock(mTmpValueLock);
    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValueForDensity(id, density, value, TRUE))

    /*
     * Pretend the requested density is actually the display density. If
     * the drawable returned is not the requested density, then force it
     * to be scaled later by dividing its density by the ratio of
     * requested density to actual device density. Drawables that have
     * undefined density or no density don't need to be handled here.
     */
    if (mTmpValue->mDensity > 0 && mTmpValue->mDensity != ITypedValue::DENSITY_NONE) {
        if (mTmpValue->mDensity == density) {
            mTmpValue->mDensity = mMetrics->mDensityDpi;
        }
        else {
            mTmpValue->mDensity = (mTmpValue->mDensity * mMetrics->mDensityDpi) / density;
        }
    }

    return LoadDrawable(value, id, drawable);
}

ECode CResources::GetMovie(
    /* [in] */ Int32 id,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    AutoPtr<IInputStream> is;
    OpenRawResource(id, (IInputStream**)&is);
    ECode ec = Movie::DecodeStream(is, movie);
    is->Close(); // don't care, since the return value is valid

    return ec;
}

ECode CResources::GetColor(
    /* [in] */ Int32 id,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType >= ITypedValue::TYPE_FIRST_INT
            && mTmpValue->mType <= ITypedValue::TYPE_LAST_INT) {
        *color = mTmpValue->mData;
        return NOERROR;
    }
    else if (mTmpValue->mType == ITypedValue::TYPE_STRING) {
        AutoPtr<IColorStateList> csl;
        ASSERT_SUCCEEDED(LoadColorStateList(value, id, (IColorStateList**)&csl));
        return csl->GetDefaultColor(color);
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *color = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetColorStateList(
    /* [in] */ Int32 id,
    /* [out] */ IColorStateList** list)
{
    VALIDATE_NOT_NULL(list);

    AutoLock lock(mTmpValueLock);
    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    return LoadColorStateList(value, id, list);
}

ECode CResources::GetBoolean(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* b)
{
    VALIDATE_NOT_NULL(b);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType >= ITypedValue::TYPE_FIRST_INT
            && mTmpValue->mType <= ITypedValue::TYPE_LAST_INT) {
        *b = mTmpValue->mData != 0;
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *b = FALSE;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetInteger(
    /* [in] */ Int32 id,
    /* [out] */ Int32* i)
{
    VALIDATE_NOT_NULL(i);

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType >= ITypedValue::TYPE_FIRST_INT
            && mTmpValue->mType <= ITypedValue::TYPE_LAST_INT) {
        *i = mTmpValue->mData;
        return NOERROR;
    }

    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    *i = 0;
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetLayout(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);
    return LoadXmlResourceParser(id, String("layout"), parser);
}

ECode CResources::GetAnimation(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** animation)
{
    VALIDATE_NOT_NULL(animation);
    return LoadXmlResourceParser(id, String("anim"), animation);
}

ECode CResources::GetXml(
    /* [in] */ Int32 id,
    /* [out] */ IXmlResourceParser** xml)
{
    VALIDATE_NOT_NULL(xml);
    return LoadXmlResourceParser(id, String("xml"), xml);
}

ECode CResources::OpenRawResource(
    /* [in] */ Int32 id,
    /* [out] */ IInputStream** res)
{
    VALIDATE_NOT_NULL(res);
    AutoLock lock(mTmpValueLock);
    return OpenRawResource(id, (ITypedValue*)mTmpValue.Get(), res);
}

ECode CResources::OpenRawResource(
    /* [in] */ Int32 id,
    /* [in] */ ITypedValue* value,
    /* [out] */ IInputStream** res)
{
    VALIDATE_NOT_NULL(res);

    FAIL_RETURN(GetValue(id, value, TRUE))

//    try {
    String str;
    ((CTypedValue*)value)->mString->ToString(&str);
    return mAssets->OpenNonAsset(
            ((CTypedValue*)value)->mAssetCookie,
            str, IAssetManager::ACCESS_STREAMING, res);
//    } catch (Exception e) {
//        NotFoundException rnf = new NotFoundException("File " + value.string.toString() +
//                " from drawable resource ID #0x" + Integer.toHexString(id));
//        rnf.initCause(e);
//        throw rnf;
//    }
}

ECode CResources::OpenRawResourceFd(
    /* [in] */ Int32 id,
    /* [out] */ IAssetFileDescriptor** des)
{
    VALIDATE_NOT_NULL(des);

    AutoLock lock(mTmpValueLock);

    FAIL_RETURN(GetValue(id, (ITypedValue*)mTmpValue.Get(), TRUE))

//    try {
    String str;
    mTmpValue->mString->ToString(&str);
    return mAssets->OpenNonAssetFd(
            mTmpValue->mAssetCookie, str, des);
//    } catch (Exception e) {
//        NotFoundException rnf = new NotFoundException(
//            "File " + value.string.toString()
//            + " from drawable resource ID #0x"
//            + Integer.toHexString(id));
//        rnf.initCause(e);
//        throw rnf;
//    }
}

ECode CResources::GetValue(
    /* [in] */ Int32 id,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    VALIDATE_NOT_NULL(outValue);

    if (mAssets->GetResourceValue(id, 0, outValue, resolveRefs)) {
        return NOERROR;
    }
    Slogger::E(TAG, "Resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetValueForDensity(
    /* [in] */ Int32 id,
    /* [in] */ Int32 density,
    /* [in] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    if (mAssets->GetResourceValue(id, density, outValue, resolveRefs)) {
        return NOERROR;
    }
    Slogger::E(TAG, "Resource ID #0x%08x", id);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetValue(
    /* [in] */ const String& name,
    /* [in, out] */ ITypedValue* outValue,
    /* [in] */ Boolean resolveRefs)
{
    VALIDATE_NOT_NULL(outValue);

    Int32 id;
    GetIdentifier(name, String("string"), String(NULL), &id);
    if (id != 0) {
        return GetValue(id, outValue, resolveRefs);
    }
    Slogger::E(TAG, "String resource name %s", (const char*)name);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::NewTheme(
    /* [out] */ IResourcesTheme** theme)
{
    VALIDATE_NOT_NULL(theme);

    *theme = new CResources::Theme(this);
    REFCOUNT_ADD(*theme);
    return NOERROR;
}

ECode CResources::ObtainAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** array)
{
    VALIDATE_NOT_NULL(array);

    Int32 len = attrs->GetLength();
    AutoPtr<CTypedArray> temp = GetCachedStyledAttributes(len);

    // XXX note that for now we only work with compiled XML files.
    // To support generic XML files we will need to manually parse
    // out the attributes from the XML file (applying type information
    // contained in the resources and such).
    AutoPtr<XmlBlock::Parser> parser = (XmlBlock::Parser*)set;
    Boolean result;
    FAIL_RETURN(mAssets->RetrieveAttributes(
            parser->mParseState, *attrs, temp->mData, temp->mIndices, &result));

    temp->mRsrcs = attrs->Clone();
    temp->mXml = parser;

    *array = (ITypedArray*)temp;
    REFCOUNT_ADD(*array);

    return NOERROR;
}

ECode CResources::UpdateConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics)
{
    return UpdateConfiguration(config, metrics, NULL);
}

ECode CResources::UpdateConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ ICompatibilityInfo* compat)
{
    {
        AutoLock lock(mTmpValueLock);

        // if (false) {
        //     Slog.i(TAG, "**** Updating config of " + this + ": old config is "
        //             + mConfiguration + " old compat is " + mCompatibilityInfo);
        //     Slog.i(TAG, "**** Updating config of " + this + ": new config is "
        //             + config + " new compat is " + compat);
        // }

        if (compat != NULL) {
            mCompatibilityInfo = (CCompatibilityInfo*)compat;
        }
        if (metrics != NULL) {
            mMetrics->SetTo(metrics);
        }

        // NOTE: We should re-arrange this code to create a Display
        // with the CompatibilityInfo that is used everywhere we deal
        // with the display in relation to this app, rather than
        // doing the conversion here.  This impl should be okay because
        // we make sure to return a compatible display in the places
        // where there are public APIs to retrieve the display...  but
        // it would be cleaner and more maintainble to just be
        // consistently dealing with a compatible display everywhere in
        // the framework.
        if (mCompatibilityInfo != NULL) {
            mCompatibilityInfo->ApplyToDisplayMetrics(mMetrics);
        }

        Int32 configChanges = 0xfffffff;
        if (config != NULL) {
            mTmpConfig->SetTo(config);
            Int32 density;
            config->GetDensityDpi(&density);
            if (density == IConfiguration::DENSITY_DPI_UNDEFINED) {
                mMetrics->GetNoncompatDensityDpi(&density);
            }
            if (mCompatibilityInfo != NULL) {
                mCompatibilityInfo->ApplyToConfiguration(density, mTmpConfig);
            }
            if (mTmpConfig->mLocale == NULL) {
                AutoPtr<ILocaleHelper> helper;
                CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
                helper->GetDefault((ILocale**)&mTmpConfig->mLocale);
                mTmpConfig->SetLayoutDirection(mTmpConfig->mLocale);
            }
            mConfiguration->UpdateFrom(mTmpConfig, &configChanges);
            AutoPtr<IActivityInfoHelper> activityInfoHelper;
            CActivityInfoHelper::AcquireSingleton((IActivityInfoHelper**)&activityInfoHelper);
            activityInfoHelper->ActivityInfoConfigToNative(configChanges, &configChanges);
        }
        if (mConfiguration->mLocale == NULL) {
            AutoPtr<ILocaleHelper> helper;
            CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
            helper->GetDefault((ILocale**)&mConfiguration->mLocale);
            mConfiguration->SetLayoutDirection(mConfiguration->mLocale);
        }
        if (mConfiguration->mDensityDpi != IConfiguration::DENSITY_DPI_UNDEFINED) {
            mMetrics->mDensityDpi = mConfiguration->mDensityDpi;
            mMetrics->mDensity = mConfiguration->mDensityDpi * IDisplayMetrics::DENSITY_DEFAULT_SCALE;
        }
        mMetrics->mScaledDensity = mMetrics->mDensity * mConfiguration->mFontScale;

        String locale(NULL);
        if (mConfiguration->mLocale != NULL) {
            mConfiguration->mLocale->GetLanguage(&locale);
            String country;
            mConfiguration->mLocale->GetCountry(&country);
            if (!country.IsNullOrEmpty()) {
                locale += String("-") + country;
            }
        }
        Int32 width, height;
        if (mMetrics->mWidthPixels >= mMetrics->mHeightPixels) {
            width = mMetrics->mWidthPixels;
            height = mMetrics->mHeightPixels;
        }
        else {
            //noinspection SuspiciousNameCombination
            width = mMetrics->mHeightPixels;
            //noinspection SuspiciousNameCombination
            height = mMetrics->mWidthPixels;
        }
        Int32 keyboardHidden = mConfiguration->mKeyboardHidden;
        if (keyboardHidden == IConfiguration::KEYBOARDHIDDEN_NO
                && mConfiguration->mHardKeyboardHidden
                        == IConfiguration::HARDKEYBOARDHIDDEN_YES) {
            keyboardHidden = IConfiguration::KEYBOARDHIDDEN_SOFT;
        }
        mAssets->SetConfiguration(mConfiguration->mMcc, mConfiguration->mMnc,
                locale, mConfiguration->mOrientation,
                mConfiguration->mTouchscreen, mConfiguration->mDensityDpi,
                mConfiguration->mKeyboard, keyboardHidden,
                mConfiguration->mNavigation, width, height,
                mConfiguration->mSmallestScreenWidthDp,
                mConfiguration->mScreenWidthDp, mConfiguration->mScreenHeightDp,
                mConfiguration->mScreenLayout, mConfiguration->mUiMode,
                Build::VERSION::RESOURCES_SDK_INT);

        if (DEBUG_CONFIG) {
            Slogger::I(TAG, "**** Updating config of %p: final config is %p final compat is %p",
                    this, mConfiguration.Get(), mCompatibilityInfo.Get());
        }

        ClearDrawableCache(mDrawableCache, configChanges);
        ClearDrawableCache(mColorDrawableCache, configChanges);
        mColorStateListCache.Clear();
        FlushLayoutCache();
    }

    {
        // AutoLock lock(_m_syncLock);
        // if (mPluralRule != NULL) {
        //     assert(config);
        //     mPluralRule = NativePluralRules::ForLocale((CConfiguration*)config->mLocale);
        // }
    }

    return NOERROR;
}

void CResources::ClearDrawableCache(
    /* [in] */ HashMap<Int64, AutoPtr<IWeakReference> >& cache,
    /* [in] */ Int32 configChanges)
{
    if (DEBUG_CONFIG) {
        Slogger::D(TAG, "Cleaning up drawables config changes: 0x%08x", configChanges);
    }

    AutoPtr<IWeakReference> wr;
    DrawableMapIterator it;
    Int32 con;
    for (it = cache.Begin(); it != cache.End(); ++it) {
        wr = it->mSecond;
        if (wr.Get() != NULL) {
            AutoPtr<IDrawableConstantState> cs;
            wr->Resolve(EIID_IDrawableConstantState, (IInterface**)&cs);
            if (cs != NULL) {
                cs->GetChangingConfigurations(&con);
                if (CConfiguration::NeedNewResources(configChanges, con)) {
                    if (DEBUG_CONFIG) {
                        Slogger::D(TAG, "FLUSHING #0x%08x /%p with changes: 0x%08x", it->mFirst, cs.Get(), con);
                    }
                    cache[it->mFirst] = NULL;
                }
                else if (DEBUG_CONFIG) {
                    Slogger::D(TAG, "(Keeping #0x%080x /%p with changes: 0x%08x", it->mFirst, cs.Get(), con);
                }
            }
        }
    }
}

void CResources::UpdateSystemConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ ICompatibilityInfo* compat)
{
    if (mSystem != NULL) {
        mSystem->UpdateConfiguration(config, metrics, compat);
        //Log.i(TAG, "Updated system resources " + mSystem
        //        + ": " + mSystem.getConfiguration());
    }
}

void CResources::UpdateSystemConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics)
{
    UpdateSystemConfiguration(config, metrics, NULL);
}

ECode CResources::GetDisplayMetrics(
    /* [out] */ IDisplayMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics);

    if (DEBUG_CONFIG) Slogger::V(TAG, "Returning DisplayMetrics: %d x %d %f",
            mMetrics->mWidthPixels, mMetrics->mHeightPixels, mMetrics->mDensity);
    *metrics = (IDisplayMetrics*)mMetrics.Get();
    REFCOUNT_ADD(*metrics);
    return NOERROR;
}

ECode CResources::GetConfiguration(
    /* [out] */ IConfiguration** config)
{
    VALIDATE_NOT_NULL(config);

    *config = (IConfiguration*)mConfiguration.Get();
    REFCOUNT_ADD(*config);
    return NOERROR;
}

ECode CResources::GetCompatibilityInfo(
    /* [out] */ ICompatibilityInfo** ci)
{
    VALIDATE_NOT_NULL(ci);

    *ci = mCompatibilityInfo != NULL ? mCompatibilityInfo.Get()
            : CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO.Get();
    REFCOUNT_ADD(*ci);
    return NOERROR;
}

ECode CResources::SetCompatibilityInfo(
    /* [in] */ ICompatibilityInfo* ci)
{
    mCompatibilityInfo = ci;
    return UpdateConfiguration((IConfiguration*)mConfiguration.Get(),
            (IDisplayMetrics*)mMetrics.Get());
}

ECode CResources::GetIdentifier(
    /* [in] */ const String& name,
    /* [in] */ const String& defType,
    /* [in] */ const String& defPackage,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);

    ECode ec = StringUtils::ParseInt32(name, id);
    if (SUCCEEDED(ec)) {
        return NOERROR;
    }

    *id = mAssets->GetResourceIdentifier(name, defType, defPackage);
    return NOERROR;
}

ECode CResources::GetResourceName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mAssets->GetResourceName(resid);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Slogger::E(TAG, "Unable to find resource ID #0x%08x", resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetDrawableResourceName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    {
        AutoLock lock(mTmpValueLock);

        String cookie;
        String path;

        AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
        FAIL_RETURN(GetValue(resid, value, TRUE))

        Int32 ac;
        value->GetAssetCookie(&ac);
        mAssets->GetCookieName(ac, &cookie);
        AutoPtr<ICharSequence> valueStr;
        value->GetString((ICharSequence**)&valueStr);
        valueStr->ToString(&path);
        StringBuilder sb(cookie);
        sb += IResources::path_separate;
        sb += path;
        *name = sb.ToString();
    }

    if (!name->IsNull()) {
        return NOERROR;
    }

    Slogger::E(TAG, "Unable to find resource ID #0x%08x", resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourcePackageName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mAssets->GetResourcePackageName(resid);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Slogger::E(TAG, "Unable to find resource ID #0x%08x", resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourceTypeName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mAssets->GetResourceTypeName(resid);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Slogger::E(TAG, "Unable to find resource ID #0x%08x", resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::GetResourceEntryName(
    /* [in] */ Int32 resid,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mAssets->GetResourceEntryName(resid);
    if (!name->IsNull()) {
        return NOERROR;
    }

    Slogger::E(TAG, "Unable to find resource ID #0x%08x", resid);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::ParseBundleExtras(
    /* [in] */ IXmlResourceParser* parser,
    /* [in, out] */ IBundle* outBundle)
{
    VALIDATE_NOT_NULL(outBundle);

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);
    Int32 type;
    Int32 depth;
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
            && (type != IXmlPullParser::END_TAG || (parser->GetDepth(&depth), depth > outerDepth))) {
        if (type == IXmlPullParser::END_TAG || type == IXmlPullParser::TEXT) {
            continue;
        }

        String nodeName;
        parser->GetName(&nodeName);
        if (nodeName.Equals("extra")) {
            ParseBundleExtra(nodeName, IAttributeSet::Probe(parser), outBundle);
            XmlUtils::SkipCurrentTag(parser);
        }
        else {
            XmlUtils::SkipCurrentTag(parser);
        }
    }

    return NOERROR;
}

ECode CResources::ParseBundleExtra(
    /* [in] */ const String& tagName,
    /* [in] */ IAttributeSet* attrs,
    /* [in, out] */ IBundle* outBundle)
{
    VALIDATE_NOT_NULL(outBundle);

    assert(attrs != NULL);

    Int32 size = ARRAY_SIZE(R::styleable::Extra);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::Extra, size);

    AutoPtr<ITypedArray> sa;
    ObtainAttributes(attrs, layout, (ITypedArray**)&sa);

    String name;
    sa->GetString(R::styleable::Extra_name, &name);
    if (name.IsNull()) {
        sa->Recycle();
        String s;
        attrs->GetPositionDescription(&s);
        Slogger::E(TAG, "<%s> requires an android:name attribute at %s",
                (const char*)tagName, (const char*)s);
        // throw new XmlPullParserException("<" + tagName
        //         + "> requires an android:name attribute at "
        //         + attrs.getPositionDescription());
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    AutoPtr<ITypedValue> v;
    sa->PeekValue(R::styleable::Extra_value, (ITypedValue**)&v);
    if (v != NULL) {
        Int32 type;
        v->GetType(&type);
        if (type == ITypedValue::TYPE_STRING) {
            AutoPtr<ICharSequence> cs;
            v->CoerceToString((ICharSequence**)&cs);
            // TODO: ALEX need PutCharSequence
            outBundle->PutCharSequence(name, cs);
        }
        else if (type == ITypedValue::TYPE_INT_BOOLEAN) {
            Int32 data;
            v->GetData(&data);
            outBundle->PutBoolean(name, (data != 0));
        }
        else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
            Int32 data;
            v->GetData(&data);
            outBundle->PutInt32(name, data);
        }
        else if (type == ITypedValue::TYPE_FLOAT) {
            Float data;
            v->GetFloat(&data);
            outBundle->PutFloat(name, data);
        }
        else {
            sa->Recycle();
//          throw new XmlPullParserException("<" + tagName
//                  + "> only supports string, integer, float, color, and boolean at "
//                  + attrs.getPositionDescription());
            return E_XML_PULL_PARSER_EXCEPTION;
        }
    }
    else {
        sa->Recycle();
//        throw new XmlPullParserException("<" + tagName
//                + "> requires an android:value or android:resource attribute at "
//                + attrs.getPositionDescription());
        String s;
        attrs->GetPositionDescription(&s);
        Slogger::E(TAG, "<%s> requires an android:name attribute at %s",
                (const char*)tagName, (const char*)s);
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    sa->Recycle();

    return NOERROR;
}

ECode CResources::GetAssets(
    /* [out] */ IAssetManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    *manager = (IAssetManager*)mAssets.Get();
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode CResources::FlushLayoutCache()
{
    AutoLock lock(mCachedXmlBlockIdsLock);

    // First see if this block is in our cache.
    for (Int32 i = 0; i < mCachedXmlBlockIds->GetLength(); i++) {
        (*mCachedXmlBlockIds)[i] = -0;
        AutoPtr<XmlBlock> oldBlock = (*mCachedXmlBlocks)[i];
        if (oldBlock != NULL) {
            oldBlock->Close();
            oldBlock = NULL;
        }
        mCachedXmlBlocks->Set(i, NULL);
    }

    return NOERROR;
}

ECode CResources::StartPreloading()
{
    AutoLock lock(_m_syncLock);

    if (sPreloaded) {
        Slogger::E(TAG, "Resources already preloaded");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    sPreloaded = TRUE;
    mPreloading = TRUE;
    // sPreloadedDensity = IDisplayMetrics::DENSITY_DEVICE;
    mConfiguration->mDensityDpi = sPreloadedDensity;
    UpdateConfiguration(NULL, NULL);
    return NOERROR;
}

ECode CResources::FinishPreloading()
{
    if (mPreloading) {
        mPreloading = FALSE;
        FlushLayoutCache();
    }
    return NOERROR;
}

Boolean CResources::VerifyPreloadConfig(
    /* [in] */ ITypedValue* value,
    /* [in] */ const String& name)
{
    if ((((CTypedValue*)value)->mChangingConfigurations & ~(IActivityInfo::CONFIG_FONT_SCALE
            | IActivityInfo::CONFIG_DENSITY)) != 0) {
        Int32 id;
        value->GetResourceId(&id);
        String resName;
        ECode ec = GetResourceName(id, &resName);
        if (ec == E_NOT_FOUND_EXCEPTION) {
            resName = "?";
        }
        Slogger::W(TAG, "Preloaded %s resource #0x%08x (%s) that varies with configuration!!",
            name.string(), id, resName.string());
        return FALSE;
    }
    return TRUE;
}

ECode CResources::LoadDrawable(
    /* [in] */ ITypedValue* value,
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = NULL;

    CTypedValue* typedValue = (CTypedValue*)value;

    if (TRACE_FOR_PRELOAD) {
        // Log only framework resources
        if (((unsigned Int32)id >> 24) == 0x1) {
            String name;
            GetResourceName(id, &name);
            Slogger::D(TAG, "PreloadDrawable %s", name.string());
        }
    }

    Boolean isColorDrawable = FALSE;
    Int32 type;
    value->GetType(&type);
    if (type >= ITypedValue::TYPE_FIRST_COLOR_INT &&
            type <= ITypedValue::TYPE_LAST_COLOR_INT) {
        isColorDrawable = TRUE;
    }
    Int32 data, assetCookie;
    value->GetData(&data);
    value->GetAssetCookie(&assetCookie);
    Int64 key = isColorDrawable ? data : (((Int64)assetCookie) << 32) | data | (((Int64) id ) << 32);

    AutoPtr<IDrawable> dr = GetCachedDrawable(
            isColorDrawable ? mColorDrawableCache : mDrawableCache, key);
    if (dr != NULL) {
        dr->SetResId(id);
        *drawable = dr;
        REFCOUNT_ADD(*drawable);
        return NOERROR;
    }

    AutoPtr<IWeakReference> wr;
    DrawableMapIterator it;
    if (isColorDrawable) {
        it = sPreloadedColorDrawables.Find(key);
        if (it != sPreloadedColorDrawables.End()) {
            wr = it->mSecond;
        }
    }
    else if (sPreloadedDensity == mConfiguration->mDensityDpi) {
        it = sPreloadedDrawables.Find(key);
        if (it != sPreloadedDrawables.End()) {
            wr = it->mSecond;
        }
    }

    AutoPtr<IDrawableConstantState> cs;
    if (wr != NULL) {
        wr->Resolve(EIID_IDrawableConstantState, (IInterface**)&cs);
    }

    if (cs != NULL) {
        cs->NewDrawable((IResources*)this, (IDrawable**)&dr);
    }
    else {
        if (isColorDrawable) {
            CColorDrawable::New(typedValue->mData, (IColorDrawable**)&dr);
        }

        if (dr == NULL) {
            if (typedValue->mString == NULL) {
                Slogger::E(TAG, "Resource is not a Drawable (color or path): %p", value);
                return E_NOT_FOUND_EXCEPTION;
                // throw new NotFoundException(
                //         "Resource is not a Drawable (color or path): " + value);
            }

            String file;
            typedValue->mString->ToString(&file);

            if (TRACE_FOR_MISS_PRELOAD) {
                // Log only framework resources
                if (((unsigned Int32)id >> 24) == 0x1) {
                    String name;
                    GetResourceName(id, &name);
                    if (!name.IsNull()) {
                        Slogger::D(TAG, "Loading framework drawable #%d: %s at %s",
                            id, name.string(), file.string());
                    }
                }
            }

            if (DEBUG_LOAD) {
                Slogger::V(TAG, "Loading drawable for cookie %d: %s",
                        typedValue->mAssetCookie, file.string());
            }

            if (file.EndWith(".xml")) {
                // try {
                AutoPtr<IXmlResourceParser> rp;
                ECode ec = LoadXmlResourceParser(file, id, typedValue->mAssetCookie,
                    String("drawable"), (IXmlResourceParser**)&rp);
                if (FAILED(ec)) {
                    Slogger::E(TAG, "File %s from drawable resource ID #0x%08x", file.string(), id);
                    return E_NOT_FOUND_EXCEPTION;
                }

                ec = Drawable::CreateFromXml((IResources*)this, rp, (IDrawable**)&dr);
                rp->Close();
                if (FAILED(ec)) {
                    Slogger::E(TAG, "File %s from drawable resource ID #0x%08x", file.string(), id);
                    return E_NOT_FOUND_EXCEPTION;
                }
                // } catch (Exception e) {
                //     NotFoundException rnf = new NotFoundException(
                //         "File " + file + " from drawable resource ID #0x"
                //         + Integer.toHexString(id));
                //     rnf.initCause(e);
                //     throw rnf;
                // }
            }
            else {
                // try {
                AutoPtr<IInputStream> is;
                ECode ec = mAssets->OpenNonAsset(typedValue->mAssetCookie, file,
                    IAssetManager::ACCESS_STREAMING, (IInputStream**)&is);
                if (FAILED(ec)) {
                    Slogger::E(TAG, "File %s from drawable resource ID #0x%08x", file.string(), id);
                    return E_NOT_FOUND_EXCEPTION;
                }
                // System.out.println("Opened file " + file + ": " + is);
                ec = Drawable::CreateFromResourceStream((IResources*)this, value, is,
                    file, NULL, (IDrawable**)&dr);
                is->Close();
                if (FAILED(ec)) {
                    Slogger::E(TAG, "File %s from drawable resource ID #0x%08x", file.string(), id);
                    return E_NOT_FOUND_EXCEPTION;
                }
                // System.out.println("Created stream: " + dr);
                // } catch (Exception e) {
                //     NotFoundException rnf = new NotFoundException(
                //         "File " + file + " from drawable resource ID #0x"
                //         + Integer.toHexString(id));
                //     rnf.initCause(e);
                //     throw rnf;
                // }
            }
        }
    }

    if (dr != NULL) {
        cs = NULL;
        dr->SetChangingConfigurations(typedValue->mChangingConfigurations);
        dr->GetConstantState((IDrawableConstantState**)&cs);
        if (cs != NULL) {
            IWeakReferenceSource* wrs = (IWeakReferenceSource*)cs->Probe(EIID_IWeakReferenceSource);
            assert(wrs != NULL);
            AutoPtr<IWeakReference> wr;
            wrs->GetWeakReference((IWeakReference**)&wr);
            if (id >= 0x7f020000 && id <= 0x7f02ffff) {
                String name;
                GetResourceName(id, &name);
                if (name.EndWith(WIDGET_SUFFIX)) {
                    sPreloadedDrawables[key] = wr;
                }
            }

            if (mPreloading) {
                if (VerifyPreloadConfig(value, String("drawable"))) {
                    if (isColorDrawable) {
                        sPreloadedColorDrawables[key] = wr;
                    }
                    else {
                        sPreloadedDrawables[key] = wr;
                    }
                }
            }
            else {
                AutoLock lock(mTmpValueLock);
                //Log.i(TAG, "Saving cached drawable @ #" +
                //        Integer.toHexString(key.intValue())
                //        + " in " + this + ": " + cs);
                if (isColorDrawable) {
                    mColorDrawableCache[key] = wr;
                }
                else {
                    mDrawableCache[key] = wr;
                }
            }
        }
    }

    if (dr != NULL) dr->SetResId(id);
    *drawable = dr;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

AutoPtr<IDrawable> CResources::GetCachedDrawable(
    /* [in] */ HashMap<Int64, AutoPtr<IWeakReference> >& drawableCache,
    /* [in] */ Int64 key)
{
    AutoLock lock(mTmpValueLock);
    AutoPtr<IDrawableConstantState> drawable;
    DrawableMapIterator it = drawableCache.Find(key);
    if (it != drawableCache.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        if (wr != NULL) {
            wr->Resolve(EIID_IDrawableConstantState, (IInterface**)&drawable);
        }
    }

    AutoPtr<IDrawable> dr;
    if (drawable != NULL) {   // we have the key
        //Log.i(TAG, "Returning cached drawable @ #" +
        //        Integer.toHexString(((Integer)key).intValue())
        //        + " in " + this + ": " + entry);

        drawable->NewDrawable((IResources*)this, (IDrawable**)&dr);
    }
    else {  // our entry has been purged
        drawableCache.Erase(it);
    }
    return dr;
}

ECode CResources::LoadColorStateList(
    /* [in] */ ITypedValue* value,
    /* [in] */ Int32 id,
    /* [out] */ IColorStateList** stateList)
{
    VALIDATE_NOT_NULL(stateList);

    CTypedValue* typedValue = (CTypedValue*)value;

    if (TRACE_FOR_PRELOAD) {
        // Log only framework resources
       if (((unsigned Int32)id >> 24) == 0x1) {
            String name;
            GetResourceName(id, &name);
            if (!name.IsNull())
                Slogger::D(NULL, "PreloadColorStateList %s", (const char*)name);
        }
    }

    Int64 key = (((Int64)typedValue->mAssetCookie) << 32) | typedValue->mData;

    AutoPtr<IColorStateList> csl;
    Int32 type;
    value->GetType(&type);
    if (type >= ITypedValue::TYPE_FIRST_COLOR_INT && type <= ITypedValue::TYPE_LAST_COLOR_INT) {
        ColorStateIterator it = sPreloadedColorStateLists.Find(key);
        if (it != sPreloadedColorStateLists.End()) {
            AutoPtr<IWeakReference> wr = it->mSecond;
            if (wr) {
                wr->Resolve(EIID_IColorStateList, (IInterface**)&csl);
            }
        }

        if (csl != NULL) {
            *stateList = csl;
            REFCOUNT_ADD(*stateList)
            return NOERROR;
        }

        ASSERT_SUCCEEDED(CColorStateList::ValueOf(typedValue->mData,
                (IColorStateList**)&csl));
        if (mPreloading) {
            if (VerifyPreloadConfig(value, String("color"))) {
                IWeakReferenceSource* wrs = (IWeakReferenceSource*)csl->Probe(EIID_IWeakReferenceSource);
                assert(wrs != NULL);
                AutoPtr<IWeakReference> wr;
                wrs->GetWeakReference((IWeakReference**)&wr);
                sPreloadedColorStateLists[key] = wr;
            }
        }

        *stateList = csl;
        REFCOUNT_ADD(*stateList)
        return NOERROR;
    }

    csl = GetCachedColorStateList(key);
    if (csl != NULL) {
        *stateList = csl;
        REFCOUNT_ADD(*stateList)
        return NOERROR;
    }

    ColorStateIterator it = sPreloadedColorStateLists.Find(key);
    if (it != sPreloadedColorStateLists.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        if (wr) {
            wr->Resolve(EIID_IColorStateList, (IInterface**)&csl);
        }
    }

    if (csl != NULL) {
        *stateList = csl;
        REFCOUNT_ADD(*stateList)
        return NOERROR;
    }

    if (typedValue->mString == NULL) {
        Slogger::E(TAG, "Resource is not a ColorStateList (color or path): %p", value);
        return E_NOT_FOUND_EXCEPTION;
        // throw new NotFoundException(
        //         "Resource is not a ColorStateList (color or path): " + value);
    }

    String file;
    typedValue->mString->ToString(&file);

    if (file.EndWith(".xml")) {
        // try {
        AutoPtr<IXmlResourceParser> rp;
        ECode ec = LoadXmlResourceParser(file, id, typedValue->mAssetCookie,
                String("colorstatelist"), (IXmlResourceParser**)&rp);
        if (FAILED(ec)) {
            Slogger::E(TAG, "File %s from color state list resource ID #0x%08x",
                file.string(), id);
            return E_NOT_FOUND_EXCEPTION;
        }

        ec = CColorStateList::CreateFromXml(this, rp, (IColorStateList**)&csl);
        rp->Close();

        if (FAILED(ec)) {
            Slogger::E(TAG, "File %s from color state list resource ID #0x%08x",
                file.string(), id);
            return E_NOT_FOUND_EXCEPTION;
        }
        // } catch (Exception e) {
        //     NotFoundException rnf = new NotFoundException(
        //         "File " + file + " from color state list resource ID #0x"
        //         + Integer.toHexString(id));
        //     rnf.initCause(e);
        //     throw rnf;
        // }
    }
    else {
        Slogger::E(TAG, "File %s from color state list resource ID #0x%08x: .xml extension required",
            file.string(), id);
        return E_NOT_FOUND_EXCEPTION;
    }

    if (csl != NULL) {
        IWeakReferenceSource* wrs = (IWeakReferenceSource*)csl->Probe(EIID_IWeakReferenceSource);
        assert(wrs != NULL);
        AutoPtr<IWeakReference> wr;
        wrs->GetWeakReference((IWeakReference**)&wr);
        if (mPreloading) {
            if (VerifyPreloadConfig(value, String("color"))) {
                sPreloadedColorStateLists[key] = wr;
            }
        }
        else {
            AutoLock lock(mTmpValueLock);

            // Log.i(TAG, "Saving cached color state list @ #" +
            //         Integer.toHexString(key.intValue())
            //         + " in " + this + ": " + csl);
            mColorStateListCache[key] = wr;
        }
    }

    *stateList = csl;
    REFCOUNT_ADD(*stateList);
    return NOERROR;
}

AutoPtr<IColorStateList> CResources::GetCachedColorStateList(
    /* [in] */ Int64 key)
{
    AutoLock lock(mTmpValueLock);

    AutoPtr<IColorStateList> entry;
    ColorStateIterator it = mColorStateListCache.Find(key);
    if (it != mColorStateListCache.End()) {
        AutoPtr<IWeakReference> wr = it->mSecond;
        if (wr != NULL) {
            wr->Resolve(EIID_IColorStateList, (IInterface**)&entry);
        }
    }

    if (entry != NULL) {   // we have the key
        //Log.i(TAG, "Returning cached color state list @ #" +
        //        Integer.toHexString(((Integer)key).intValue())
        //        + " in " + this + ": " + entry);
    }
    else { // our entry has been purged
        mColorStateListCache.Erase(it);
    }

    return entry;
}

ECode CResources::LoadXmlResourceParser(
    /* [in] */ Int32 id,
    /* [in] */ const String& type,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);
    *parser = NULL;

    AutoLock lock(mTmpValueLock);

    AutoPtr<ITypedValue> value = (ITypedValue*)mTmpValue.Get();
    FAIL_RETURN(GetValue(id, value, TRUE))
    if (mTmpValue->mType == ITypedValue::TYPE_STRING) {
        String str;
        mTmpValue->mString->ToString(&str);
        return LoadXmlResourceParser(str, id, mTmpValue->mAssetCookie, type, parser);
    }
//    throw new NotFoundException(
//            "Resource ID #0x" + Integer.toHexString(id) + " type #0x"
//            + Integer.toHexString(value.type) + " is not valid");
    Slogger::E(TAG, "Resource ID #0x%08x type #0x%08x is not valid", id, mTmpValue->mType);
    return E_NOT_FOUND_EXCEPTION;
}

ECode CResources::LoadXmlResourceParser(
    /* [in] */ const String& file,
    /* [in] */ Int32 id,
    /* [in] */ Int32 assetCookie,
    /* [in] */ const String& type,
    /* [out] */ IXmlResourceParser** parser)
{
    VALIDATE_NOT_NULL(parser);
    *parser = NULL;

    if (id != 0) {
        // try {
        // These may be compiled...
        AutoLock lock(mCachedXmlBlockIdsLock);
        String name;
        // First see if this block is in our cache.
        Int32 num = mCachedXmlBlockIds->GetLength();
        for (Int32 i = 0; i < num; i++) {
            if ((*mCachedXmlBlockIds)[i] == id) {
                mAssets->GetCookieName(assetCookie, &name);
                String filename = file;
                if (!name.IsNull()) {
                    StringBuilder sb(name);
                    sb += IResources::path_separate;
                    sb += file;
                    filename = sb.ToString();
                }

                AutoPtr<IXmlResourceParser> p = (*mCachedXmlBlocks)[i]->NewParser(filename);
                *parser = p;
                REFCOUNT_ADD(*parser);
                return NOERROR;
            }
        }

        // Not in the cache, create a new block and put it at
        // the next slot in the cache.
        AutoPtr<XmlBlock> block;
        if (FAILED(mAssets->OpenXmlBlockAsset(assetCookie, file, (XmlBlock**)&block))) {
            Slogger::E(TAG, "File %s from xml type %s resource ID #0x%08x",
                file.string(), type.string(), id);
            return E_NOT_FOUND_EXCEPTION;
        }
        if (block != NULL) {
            Int32 pos = (mLastCachedXmlBlockIndex + 1) % num;
            mLastCachedXmlBlockIndex = pos;
            AutoPtr<XmlBlock> oldBlock = (*mCachedXmlBlocks)[pos];

            mCachedXmlBlocks->Set(pos, NULL);
            if (oldBlock != NULL) {
                oldBlock->Close();
                oldBlock = NULL;
            }

            (*mCachedXmlBlockIds)[pos] = id;
            mCachedXmlBlocks->Set(pos, block);

            mAssets->GetCookieName(assetCookie, &name);
            String filename = file;
            if (!name.IsNull()) {
                StringBuilder sb(name);
                sb += IResources::path_separate;
                sb += file;
                filename = sb.ToString();
            }
            AutoPtr<IXmlResourceParser> p = block->NewParser(filename);
            *parser = p;
            REFCOUNT_ADD(*parser);
            return NOERROR;
        }
        // } catch (Exception e) {
        //     NotFoundException rnf = new NotFoundException(
        //             "File " + file + " from xml type " + type + " resource ID #0x"
        //             + Integer.toHexString(id));
        //     rnf.initCause(e);
        //     throw rnf;
        // }
    }

    // throw new NotFoundException(
    //     "File " + file + " from xml type " + type + " resource ID #0x"
    //     + Integer.toHexString(id));
    Slogger::E(TAG, "File %s from xml type %s resource ID #0x%08x",
        file.string(), type.string(), id);
    return E_NOT_FOUND_EXCEPTION;
}

AutoPtr<CTypedArray> CResources::GetCachedStyledAttributes(
    /* [in] */ Int32 len)
{
    AutoLock lock(mTmpValueLock);

    AutoPtr<CTypedArray> attrs = mCachedStyledAttributes;
    if (attrs != NULL) {
        mCachedStyledAttributes = NULL;

        // if (DEBUG_ATTRIBUTES_CACHE) {
        //     mLastRetrievedAttrs = new RuntimeException("here");
        //     mLastRetrievedAttrs.fillInStackTrace();
        // }

        attrs->mLength = len;
        Int32 fullLen = len * CAssetManager::STYLE_NUM_ENTRIES;
        if (attrs->mData->GetLength() >= fullLen) {
            return attrs;
        }
        attrs->mData = ArrayOf<Int32>::Alloc(fullLen);
        attrs->mIndices = ArrayOf<Int32>::Alloc(1 + len);
        return attrs;
    }

    // if (DEBUG_ATTRIBUTES_CACHE) {
    //     RuntimeException here = new RuntimeException("here");
    //     here.fillInStackTrace();
    //     if (mLastRetrievedAttrs != 0) {
    //         Logger::I(TAG, String("Allocated new TypedArray of ") + (const char*)len + String(" in ")
    //                 + (const char*)this + String("RuntimeException"));
    //         Logger::I(TAG, String("Last retrieved attributes here RuntimeException"));
    //     }
    //     mLastRetrievedAttrs = here;
    // }

    AutoPtr< ArrayOf<Int32> > data = ArrayOf<Int32>::Alloc(len * CAssetManager::STYLE_NUM_ENTRIES);
    AutoPtr< ArrayOf<Int32> > indices = ArrayOf<Int32>::Alloc(1 + len);
    CTypedArray::NewByFriend(THIS_PROBE(IResources), data, indices, len, (CTypedArray**)&attrs);
    return attrs;
}

ECode CResources::constructor(
    /* [in] */ IAssetManager* assets,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ IConfiguration* config)
{
    return constructor(assets, metrics, config, NULL);
}

ECode CResources::constructor(
    /* [in] */ IAssetManager* assets,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ IConfiguration* config,
    /* [in] */ ICompatibilityInfo* compInfo)
{
    if (!assets) {
        return E_INVALID_ARGUMENT;
    }
    mAssets = (CAssetManager*)assets;
    mMetrics->SetToDefaults();
    mCompatibilityInfo = compInfo;
    UpdateConfiguration(config, metrics);
    mAssets->EnsureStringBlocks();
    return NOERROR;
}

ECode CResources::constructor()
{
    mAssets = CAssetManager::GetSystem();
    // NOTE: Intentionally leaving this uninitialized (all values set
    // to zero), so that anyone who tries to do something that requires
    // metrics will get a very wrong value.
    mConfiguration->SetToDefaults();
    mMetrics->SetToDefaults();
    UpdateConfiguration(NULL, NULL);
    mAssets->EnsureStringBlocks();
    mCompatibilityInfo = CCompatibilityInfo::DEFAULT_COMPATIBILITY_INFO;
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos
