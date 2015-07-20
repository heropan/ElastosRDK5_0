
#include "content/res/CTypedArray.h"
#include "content/res/CAssetManager.h"
#include "util/XmlUtils.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include <stdlib.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Utility::XmlUtils;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CTypedArray::CTypedArray()
    : mLength(0)
    , mResources(NULL)
{
    ASSERT_SUCCEEDED(CTypedValue::NewByFriend((CTypedValue**)&mValue));
}

ECode CTypedArray::Length(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);
    *len = mLength;
    return NOERROR;
}

ECode CTypedArray::GetIndexCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = (*mIndices)[0];
    return NOERROR;
}

ECode CTypedArray::GetIndex(
    /* [in] */ Int32 at,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = (*mIndices)[1 + at];
    return NOERROR;
}

ECode CTypedArray::GetResources(
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);

    *res = (IResources*)mResources;
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CTypedArray::GetText(
    /* [in] */ Int32 index,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *csq = NULL;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_STRING) {
        AutoPtr<ICharSequence> temp = LoadStringValueAt(index);
        *csq = temp;
        REFCOUNT_ADD(*csq);
        return NOERROR;
    }

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index, v)) {
        Slogger::W(CResources::TAG, "Converting to string: %p", v.Get());
        return v->CoerceToString(csq);
    }
    Slogger::W(CResources::TAG, "getString of bad type: %d", type);
    *csq = NULL;
    return NOERROR;
}

ECode CTypedArray::GetString(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *str = String(NULL);
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_STRING) {
        AutoPtr<ICharSequence> csq = LoadStringValueAt(index);
        assert(csq);
        return csq->ToString(str);
    }

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index, v)) {
        Slogger::W(CResources::TAG, "Converting to string: %p", v.Get());
        AutoPtr<ICharSequence> csq;
        if (FAILED(v->CoerceToString((ICharSequence**)&csq))) {
            *str = String(NULL);
            return NOERROR;
        }
        return csq->ToString(str);
    }
    Slogger::W(CResources::TAG, "getString of bad type: %d", type);
    *str = String(NULL);
    return NOERROR;
}

ECode CTypedArray::GetNonResourceString(
    /* [in] */ Int32 index,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_STRING) {
        Int32 cookie = (*data)[index + CAssetManager::STYLE_ASSET_COOKIE];
        if (cookie < 0) {
            AutoPtr<ICharSequence> csq;
            if (FAILED(mXml->GetPooledString((*data)[index + CAssetManager::STYLE_DATA],
                    (ICharSequence**)&csq))) {
                *str = String(NULL);
                return NOERROR;
            }
            return csq->ToString(str);
        }
    }
    *str = String(NULL);
    return NOERROR;
}

ECode CTypedArray::GetNonConfigurationString(
    /* [in] */ Int32 index,
    /* [in] */ Int32 allowedChangingConfigs,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (((*data)[index + CAssetManager::STYLE_CHANGING_CONFIGURATIONS]
            & ~allowedChangingConfigs) != 0) {
        *str = String(NULL);
        return NOERROR;
    }
    if (type == ITypedValue::TYPE_NULL) {
        *str = String(NULL);
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_STRING) {
        AutoPtr<ICharSequence> csq = LoadStringValueAt(index);
        if (csq == NULL) {
            *str = String(NULL);
            return NOERROR;
        }
        return csq->ToString(str);
    }
    Slogger::W(CResources::TAG, "getString of bad type: 0x%08x", type);
    *str = NULL;
    return NOERROR;
}

ECode CTypedArray::GetBoolean(
    /* [in] */ Int32 index,
    /* [in] */ Boolean defValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *value = (*data)[index + CAssetManager::STYLE_DATA] != 0;
        return NOERROR;
    }

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index, v)) {
        Slogger::W(CResources::TAG, "Converting to boolean: %p", v.Get());
        AutoPtr<ICharSequence> csq;
        v->CoerceToString((ICharSequence**)&csq);
        *value = XmlUtils::ConvertValueToBoolean(csq, defValue);
        return NOERROR;
    }
    Slogger::W(CResources::TAG, "getBoolean of bad type: 0x%08x", type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *value = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index, v)) {
        Slogger::W(CResources::TAG, "Converting to int: %p", v.Get());
        AutoPtr<ICharSequence> csq;
        v->CoerceToString((ICharSequence**)&csq);
        *value = XmlUtils::ConvertValueToInt32(csq, defValue);
        return NOERROR;
    }
    Slogger::W(CResources::TAG, "getInt of bad type: 0x%08x", type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_FLOAT) {
        *value = Elastos::Core::Math::Int32BitsToFloat((*data)[index + CAssetManager::STYLE_DATA]);
        return NOERROR;
    }
    else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *value = (Float)(*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index, v)) {
        Slogger::W(CResources::TAG, "Converting to float: %p", v.Get());
        AutoPtr<ICharSequence> csq;
        if (SUCCEEDED(v->CoerceToString((ICharSequence**)&csq))) {
            String str;
            csq->ToString(&str);
            *value = atof((const char*)str);
            return NOERROR;
        }
    }
    Slogger::W(CResources::TAG, "getFloat of bad type: 0x%08x", type);
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetColor(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *value = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_STRING) {
        AutoPtr<CTypedValue> v = mValue;
        if (GetValueAt(index, v)) {
            Int32 resourceId;
            v->GetResourceId(&resourceId);
            AutoPtr<IColorStateList> csl;
            mResources->LoadColorStateList(v, resourceId, (IColorStateList**)&csl);
            return csl->GetDefaultColor(value);
        }
        *value = defValue;
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to color: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetColorStateList(
    /* [in] */ Int32 index,
    /* [out] */ IColorStateList** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<CTypedValue> value = mValue;
    if (GetValueAt(index * CAssetManager::STYLE_NUM_ENTRIES, value)) {
        Int32 resourceId;
        value->GetResourceId(&resourceId);
        return mResources->LoadColorStateList(value, resourceId, list);
    }
    *list = NULL;
    return NOERROR;
}

ECode CTypedArray::GetInteger(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *value = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to integer: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimension(
    /* [in] */ Int32 index,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimension(
                (*data)[index + CAssetManager::STYLE_DATA], mResources->mMetrics);
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to dimension: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimensionPixelOffset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimensionPixelOffset(
                (*data)[index + CAssetManager::STYLE_DATA], mResources->mMetrics);
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to dimension: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetDimensionPixelSize(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_DIMENSION) {
        *value = CTypedValue::ComplexToDimensionPixelSize(
                (*data)[index + CAssetManager::STYLE_DATA], mResources->mMetrics);
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to dimension: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetLayoutDimension(
    /* [in] */ Int32 index,
    /* [in] */ const String& name,
    /* [out] */ Int32* dimension)
{
    VALIDATE_NOT_NULL(dimension);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type >= ITypedValue::TYPE_FIRST_INT && type <= ITypedValue::TYPE_LAST_INT) {
        *dimension = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_DIMENSION) {
        *dimension = CTypedValue::ComplexToDimensionPixelSize(
                (*data)[index + CAssetManager::STYLE_DATA], mResources->mMetrics);
        return NOERROR;
    }

    String des;
    GetPositionDescription(&des);
    Slogger::E(CResources::TAG, ")%s: You must supply a %s attribute.", (const char*)des, (const char*)name);
    return E_RUNTIME_EXCEPTION;
}

ECode CTypedArray::GetLayoutDimension(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* dimension)
{
    VALIDATE_NOT_NULL(dimension);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type >= ITypedValue::TYPE_FIRST_INT
            && type <= ITypedValue::TYPE_LAST_INT) {
        *dimension = (*data)[index + CAssetManager::STYLE_DATA];
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_DIMENSION) {
        *dimension = CTypedValue::ComplexToDimensionPixelSize(
                (*data)[index + CAssetManager::STYLE_DATA], mResources->mMetrics);
        return NOERROR;
    }

    *dimension = defValue;
    return NOERROR;
}

ECode CTypedArray::GetFraction(
    /* [in] */ Int32 index,
    /* [in] */ Int32 base,
    /* [in] */ Int32 pbase,
    /* [in] */ Float defValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        *value = defValue;
        return NOERROR;
    }
    else if (type == ITypedValue::TYPE_FRACTION) {
        *value = CTypedValue::ComplexToFraction(
                (*data)[index + CAssetManager::STYLE_DATA], base, pbase);
        return NOERROR;
    }

    Slogger::E(CResources::TAG, "Can't convert to fraction: type=0x%08x", type);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CTypedArray::GetResourceId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 defValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    if ((*data)[index + CAssetManager::STYLE_TYPE] != ITypedValue::TYPE_NULL) {
        Int32 resid = (*data)[index + CAssetManager::STYLE_RESOURCE_ID];
        if (resid != 0) {
            *value = resid;
            return NOERROR;
        }
    }
    *value = defValue;
    return NOERROR;
}

ECode CTypedArray::GetDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    AutoPtr<CTypedValue> value = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, value)) {
        // if (FALSE) {
        //     System.out.println("******************************************************************");
        //     System.out.println("Got drawable resource: type="
        //                        + value.type
        //                        + " str=" + value.string
        //                        + " int=0x" + Integer.toHexString(value.data)
        //                        + " cookie=" + value.assetCookie);
        //     System.out.println("******************************************************************");
        // }
        Int32 resourceId;
        value->GetResourceId(&resourceId);
        return mResources->LoadDrawable(value, resourceId, drawable);
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CTypedArray::GetTextArray(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<ICharSequence*>** array)
{
    AutoPtr<CTypedValue> value = mValue;
    if (GetValueAt(index*CAssetManager::STYLE_NUM_ENTRIES, value)) {
        // if (false) {
        //     System.out.println("******************************************************************");
        //     System.out.println("Got drawable resource: type="
        //                        + value.type
        //                        + " str=" + value.string
        //                        + " int=0x" + Integer.toHexString(value.data)
        //                        + " cookie=" + value.assetCookie);
        //     System.out.println("******************************************************************");
        // }
        Int32 resourceId;
        value->GetResourceId(&resourceId);
        return mResources->GetTextArray(resourceId, array);
    }
    *array = NULL;
    return NOERROR;
}

ECode CTypedArray::GetValue(
    /* [in] */ Int32 index,
    /* [in, out] */ ITypedValue* outValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(outValue);
    VALIDATE_NOT_NULL(value);

    *value = GetValueAt(index * CAssetManager::STYLE_NUM_ENTRIES, (CTypedValue*)outValue);
    return NOERROR;
}

ECode CTypedArray::HasValue(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* hasValue)
{
    VALIDATE_NOT_NULL(hasValue);

    index *= CAssetManager::STYLE_NUM_ENTRIES;
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    *hasValue = type != ITypedValue::TYPE_NULL;
    return NOERROR;
}

ECode CTypedArray::PeekValue(
    /* [in] */ Int32 index,
    /* [out] */ ITypedValue** value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<CTypedValue> v = mValue;
    if (GetValueAt(index * CAssetManager::STYLE_NUM_ENTRIES, v)) {
        *value = (ITypedValue*)v;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CTypedArray::GetPositionDescription(
    /* [out] */ String* des)
{
    VALIDATE_NOT_NULL(des);

    if (mXml != NULL) {
        return mXml->GetPositionDescription(des);
    }
    *des = String("<internal>");
    return NOERROR;
}

ECode CTypedArray::Recycle()
{
    AutoLock lock(mResources->mTmpValueLock);
    AutoPtr<ITypedArray> cached = mResources->mCachedStyledAttributes;
    if (cached == NULL || ((CTypedArray*)cached.Get())->mData->GetLength() < mData->GetLength()) {
        mXml = NULL;
        mResources->mCachedStyledAttributes = this;
    }
    return NOERROR;
}

Boolean CTypedArray::GetValueAt(
    /* [in] */ Int32 index,
    /* [in, out] */ CTypedValue* outValue)
{
    assert(outValue);

    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 type = (*data)[index + CAssetManager::STYLE_TYPE];
    if (type == ITypedValue::TYPE_NULL) {
        return FALSE;
    }
    outValue->mType = type;
    outValue->mData = (*data)[index + CAssetManager::STYLE_DATA];
    outValue->mAssetCookie = (*data)[index + CAssetManager::STYLE_ASSET_COOKIE];
    outValue->mResourceId = (*data)[index + CAssetManager::STYLE_RESOURCE_ID];
    outValue->mChangingConfigurations = (*data)[index + CAssetManager::STYLE_CHANGING_CONFIGURATIONS];
    outValue->mDensity = (*data)[index + CAssetManager::STYLE_DENSITY];
    outValue->mString = (type == ITypedValue::TYPE_STRING) ?
            LoadStringValueAt(index) : NULL;
    return TRUE;
}

AutoPtr<ICharSequence> CTypedArray::LoadStringValueAt(
    /* [in] */ Int32 index)
{
    AutoPtr< ArrayOf<Int32> > data = mData;
    Int32 cookie = (*data)[index + CAssetManager::STYLE_ASSET_COOKIE];
    if (cookie < 0) {
        if (mXml != NULL) {
            AutoPtr<ICharSequence> csq;
            mXml->GetPooledString((*data)[index + CAssetManager::STYLE_DATA], (ICharSequence**)&csq);
            return csq;
        }
        return NULL;
    }
    // System.out.println("Getting pooled from: " + v);
    return mResources->mAssets->GetPooledString(
            cookie, (*data)[index + CAssetManager::STYLE_DATA]);
}

ECode CTypedArray::constructor(
    /* [in] */ IResources* resources,
    /* [in] */ ArrayOf<Int32>* data,
    /* [in] */ ArrayOf<Int32>* indices,
    /* [in] */ Int32 len)
{
    mResources = (CResources*)resources;
    mData = data;
    mIndices = indices;
    mLength = len;
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos
