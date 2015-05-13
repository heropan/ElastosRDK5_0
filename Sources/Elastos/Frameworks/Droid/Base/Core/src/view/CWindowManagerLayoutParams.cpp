
#include "ext/frameworkext.h"
#include "view/CWindowManagerLayoutParams.h"
#include "text/TextUtils.h"
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace View {

IVIEWGROUPLP_METHODS_IMPL(CWindowManagerLayoutParams, ViewGroupLayoutParams);

CWindowManagerLayoutParams::CWindowManagerLayoutParams()
    : mX(0)
    , mY(0)
    , mHorizontalWeight(0.0f)
    , mVerticalWeight(0.0f)
    , mType(0)
    , mMemoryType(0)
    , mFlags(0)
    , mPrivateFlags(0)
    , mSoftInputMode(0)
    , mGravity(0)
    , mHorizontalMargin(0.0f)
    , mVerticalMargin(0.0f)
    , mFormat(0)
    , mWindowAnimations(0)
    , mAlpha(1.0f)
    , mDimAmount(1.0f)
    , mScreenBrightness(BRIGHTNESS_OVERRIDE_NONE)
    , mButtonBrightness(BRIGHTNESS_OVERRIDE_NONE)
    , mScreenOrientation(IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED)
    , mSystemUiVisibility(0)
    , mSubtreeSystemUiVisibility(0)
    , mHasSystemUiListeners(FALSE)
    , mInputFeatures(0)
    , mUserActivityTimeout(-1)
{
    ASSERT_SUCCEEDED(CStringWrapper::New(String(""), (ICharSequence**)&mTitle));
}

Boolean CWindowManagerLayoutParams::MayUseInputMethod(
    /* [in] */ Int32 flags)
{
    switch (flags & (FLAG_NOT_FOCUSABLE | FLAG_ALT_FOCUSABLE_IM)) {
    case 0:
    case FLAG_NOT_FOCUSABLE |
        FLAG_ALT_FOCUSABLE_IM:
        return TRUE;
    }

    return FALSE;
}

ECode CWindowManagerLayoutParams::SetTitle(
    /* [in] */ ICharSequence* _title)
{
    AutoPtr<ICharSequence> title = _title;
    if (NULL == title)
       FAIL_RETURN(CStringWrapper::New(String(""), (ICharSequence**)&title));

    mTitle = TextUtils::StringOrSpannedString(title);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);

    *title = mTitle;
    INTERFACE_ADDREF(*title);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::CopyFrom(
    /* [in] */ IWindowManagerLayoutParams* source,
    /* [out] */ Int32* changes)
{
    VALIDATE_NOT_NULL(changes);
    if (!source) {
        return E_INVALID_ARGUMENT;
    }

    CWindowManagerLayoutParams* src = (CWindowManagerLayoutParams*)source;
    *changes = 0;

    if (mWidth != src->mWidth) {
        mWidth = src->mWidth;
        *changes |= LAYOUT_CHANGED;
    }

    if (mHeight != src->mHeight) {
        mHeight = src->mHeight;
        *changes |= LAYOUT_CHANGED;
    }

    if (mX != src->mX) {
        mX = src->mX;
        *changes |= LAYOUT_CHANGED;
    }

    if (mY != src->mY) {
        mY = src->mY;
        *changes |= LAYOUT_CHANGED;
    }

    if (mHorizontalWeight != src->mHorizontalWeight) {
        mHorizontalWeight = src->mHorizontalWeight;
        *changes |= LAYOUT_CHANGED;
    }

    if (mVerticalWeight != src->mVerticalWeight) {
        mVerticalWeight = src->mVerticalWeight;
        *changes |= LAYOUT_CHANGED;
    }

    if (mHorizontalMargin != src->mHorizontalMargin) {
        mHorizontalMargin = src->mHorizontalMargin;
        *changes |= LAYOUT_CHANGED;
    }

    if (mVerticalMargin != src->mVerticalMargin) {
        mVerticalMargin = src->mVerticalMargin;
        *changes |= LAYOUT_CHANGED;
    }

    if (mType != src->mType) {
        mType = src->mType;
        *changes |= TYPE_CHANGED;
    }

    if (mFlags != src->mFlags) {
        mFlags = src->mFlags;
        *changes |= FLAGS_CHANGED;
    }

    if (mPrivateFlags != src->mPrivateFlags) {
        mPrivateFlags = src->mPrivateFlags;
        *changes |= MEMORY_TYPE_CHANGED;
    }

    if (mSoftInputMode != src->mSoftInputMode) {
        mSoftInputMode = src->mSoftInputMode;
        *changes |= SOFT_INPUT_MODE_CHANGED;
    }

    if (mGravity != src->mGravity) {
        mGravity = src->mGravity;
        *changes |= LAYOUT_CHANGED;
    }

    if (mFormat != src->mFormat) {
        mFormat = src->mFormat;
        *changes |= FORMAT_CHANGED;
    }

    if (mWindowAnimations != src->mWindowAnimations) {
        mWindowAnimations = src->mWindowAnimations;
        *changes |= ANIMATION_CHANGED;
    }

    if (mToken == NULL) {
        // NOTE: mToken only copied if the recipient doesn't
        // already have one.
        mToken = src->mToken;
    }

    if (mPackageName.IsNull()) {
        // NOTE: packageName only copied if the recipient doesn't
        // already have one.
        mPackageName = src->mPackageName;
    }

    String title1, title2;
    mTitle->ToString(&title1);
    src->mTitle->ToString(&title2);
    if (!title1.Equals(title2)) {
        mTitle = src->mTitle;
        *changes |= TITLE_CHANGED;
    }

    if (mAlpha != src->mAlpha) {
        mAlpha = src->mAlpha;
        *changes |= ALPHA_CHANGED;
    }

    if (mDimAmount != src->mDimAmount) {
        mDimAmount = src->mDimAmount;
        *changes |= DIM_AMOUNT_CHANGED;
    }

    if (mScreenBrightness != src->mScreenBrightness) {
        mScreenBrightness = src->mScreenBrightness;
        *changes |= SCREEN_BRIGHTNESS_CHANGED;
    }

    if (mButtonBrightness != src->mButtonBrightness) {
        mButtonBrightness = src->mButtonBrightness;
        *changes |= BUTTON_BRIGHTNESS_CHANGED;
    }

    if (mScreenOrientation != src->mScreenOrientation) {
        mScreenOrientation = src->mScreenOrientation;
        *changes |= SCREEN_ORIENTATION_CHANGED;
    }

    if (mSystemUiVisibility != src->mSystemUiVisibility
        || mSubtreeSystemUiVisibility != src->mSubtreeSystemUiVisibility) {
        mSystemUiVisibility = src->mSystemUiVisibility;
        mSubtreeSystemUiVisibility = src->mSubtreeSystemUiVisibility;
        *changes |= SYSTEM_UI_VISIBILITY_CHANGED;
    }

    if (mHasSystemUiListeners != src->mHasSystemUiListeners) {
        mHasSystemUiListeners = src->mHasSystemUiListeners;
        *changes |= SYSTEM_UI_LISTENER_CHANGED;
    }

    if (mInputFeatures != src->mInputFeatures) {
        mInputFeatures = src->mInputFeatures;
        *changes |= INPUT_FEATURES_CHANGED;
    }

    if (mUserActivityTimeout != src->mUserActivityTimeout) {
        mUserActivityTimeout = src->mUserActivityTimeout;
        *changes |= USER_ACTIVITY_TIMEOUT_CHANGED;
    }

    return NOERROR;
}

/**
 * Scale the layout params' coordinates and size.
 * @hide
 */
ECode CWindowManagerLayoutParams::Scale(
    /* [in] */ Float scale)
{
    mX = (Int32)(mX * scale + 0.5f);
    mY = (Int32)(mY * scale + 0.5f);

    if (mWidth > 0) {
        mWidth = (Int32)(mWidth * scale + 0.5f);
    }

    if (mHeight > 0) {
        mHeight = (Int32)(mHeight * scale + 0.5f);
    }

    return NOERROR;
}

/**
 * Backup the layout parameters used in compatibility mode.
 * @see LayoutParams#restore()
 */
ECode CWindowManagerLayoutParams::Backup()
{
    if (mCompatibilityParamsBackup == NULL) {
        // we backup 4 elements, x, y, mWidth, mHeight
        mCompatibilityParamsBackup = ArrayOf<Int32>::Alloc(4);
    }
    (*mCompatibilityParamsBackup)[0] = mX;
    (*mCompatibilityParamsBackup)[1] = mY;
    (*mCompatibilityParamsBackup)[2] = mWidth;
    (*mCompatibilityParamsBackup)[3] = mHeight;

    return NOERROR;
}

/**
 * Restore the layout params' coordinates, size and gravity
 * @see LayoutParams#backup()
 */
ECode CWindowManagerLayoutParams::Restore()
{
    if (mCompatibilityParamsBackup != NULL) {
        mX = (*mCompatibilityParamsBackup)[0];
        mY = (*mCompatibilityParamsBackup)[1];
        mWidth = (*mCompatibilityParamsBackup)[2];
        mHeight = (*mCompatibilityParamsBackup)[3];
    }

    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mX;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetX(
    /* [in] */ Int32 x)
{
    mX = x;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mY;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetY(
    /* [in] */ Int32 y)
{
    mY = y;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetMemoryType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mMemoryType;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetMemoryType(
    /* [in] */ Int32 type)
{
    mMemoryType = type;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetPrivateFlags(
    /* [out] */ Int32* privateFlags)
{
    VALIDATE_NOT_NULL(privateFlags);
    *privateFlags = mPrivateFlags;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetPrivateFlags(
    /* [in] */ Int32 privateFlags)
{
    mPrivateFlags = privateFlags;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    mSoftInputMode = mode;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetSoftInputMode(
    /* [out] */ Int32* mode)
{
    *mode = mSoftInputMode;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = mGravity;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    mGravity = gravity;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    VALIDATE_NOT_NULL(horizontalMargin);
    *horizontalMargin = mHorizontalMargin;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetHorizontalMargin(
    /* [in] */ Float horizontalMargin)
{
    mHorizontalMargin = horizontalMargin;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    VALIDATE_NOT_NULL(verticalMargin);
    *verticalMargin = mVerticalMargin;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetVerticalMargin(
    /* [in] */ Float verticalMargin)
{
    mVerticalMargin = verticalMargin;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetFormat(
    /* [out] */ Int32* format)
{
    VALIDATE_NOT_NULL(format);
    *format = mFormat;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetFormat(
    /* [in] */ Int32 format)
{
    mFormat = format;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetWindowAnimations(
     /* [out] */ Int32* animations)
{
    VALIDATE_NOT_NULL(animations);

    *animations = mWindowAnimations;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetWindowAnimations(
    /* [in] */ Int32 animations)
{
    mWindowAnimations = animations;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetAlpha(
    /* [out] */ Float* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mAlpha;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetAlpha(
    /* [in] */ Float alpha)
{
    mAlpha = alpha;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetDimAmount(
    /* [out] */ Float* dimAmount)
{
    VALIDATE_NOT_NULL(dimAmount);
    *dimAmount = mDimAmount;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetDimAmount(
    /* [in] */ Float dimAmount)
{
    mDimAmount = dimAmount;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetScreenBrightness(
    /* [out] */ Float* brightness)
{
    VALIDATE_NOT_NULL(brightness);
    *brightness = mScreenBrightness;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetScreenBrightness(
    /* [in] */ Float brightness)
{
    mScreenBrightness = brightness;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetButtonBrightness(
    /* [out] */ Float* brightness)
{
    VALIDATE_NOT_NULL(brightness);
    *brightness = mButtonBrightness;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetButtonBrightness(
    /* [in] */ Float brightness)
{
    mButtonBrightness = brightness;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetToken(
    /* [out] */ IBinder** token)
{
    VALIDATE_NOT_NULL(token);
    *token = mToken;
    INTERFACE_ADDREF(*token);
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetToken(
    /* [in] */ IBinder* token)
{
    mToken = token;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetPackageName(
    /* [in] */ const String& packageName)
{
    mPackageName = packageName;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetScreenOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mScreenOrientation;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetScreenOrientation(
    /* [in] */ Int32 orientation)
{
    mScreenOrientation = orientation;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetSystemUiVisibility(
    /* [out] */ Int32* systemUiVisibility)
{
    VALIDATE_NOT_NULL(systemUiVisibility);
    *systemUiVisibility = mSystemUiVisibility;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetSystemUiVisibility(
    /* [in] */ Int32 systemUiVisibility)
{
    mSystemUiVisibility = systemUiVisibility;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetSubtreeSystemUiVisibility(
    /* [out] */ Int32* subtreeSystemUiVisibility)
{
    VALIDATE_NOT_NULL(subtreeSystemUiVisibility);
    *subtreeSystemUiVisibility = mSubtreeSystemUiVisibility;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetSubtreeSystemUiVisibility(
    /* [in] */ Int32 subtreeSystemUiVisibility)
{
    mSubtreeSystemUiVisibility = subtreeSystemUiVisibility;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetHasSystemUiListeners(
    /* [out] */ Boolean* hasSystemUiListeners)
{
    VALIDATE_NOT_NULL(hasSystemUiListeners);
    *hasSystemUiListeners = mHasSystemUiListeners;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetHasSystemUiListeners(
    /* [in] */ Boolean hasSystemUiListeners)
{
    mHasSystemUiListeners = hasSystemUiListeners;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetInputFeatures(
    /* [out] */ Int32* inputFeatures)
{
    VALIDATE_NOT_NULL(inputFeatures);
    *inputFeatures = mInputFeatures;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetInputFeatures(
    /* [in] */ Int32 inputFeatures)
{
    mInputFeatures = inputFeatures;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::GetUserActivityTimeout(
    /* [out] */ Int32* userActivityTimeout)
{
    VALIDATE_NOT_NULL(userActivityTimeout);
    *userActivityTimeout = mUserActivityTimeout;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::SetUserActivityTimeout(
    /* [in] */ Int32 userActivityTimeout)
{
    mUserActivityTimeout = userActivityTimeout;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    if (!source) {
        return E_INVALID_ARGUMENT;
    }

    source->ReadInt32(&mWidth);
    source->ReadInt32(&mHeight);
    source->ReadInt32(&mX);
    source->ReadInt32(&mY);
    source->ReadInt32(&mType);
    source->ReadInt32(&mFlags);
    source->ReadInt32(&mPrivateFlags);
    source->ReadInt32(&mSoftInputMode);
    source->ReadInt32(&mGravity);
    source->ReadFloat(&mHorizontalMargin);
    source->ReadFloat(&mVerticalMargin);
    source->ReadInt32(&mFormat);
    source->ReadInt32(&mWindowAnimations);
    source->ReadFloat(&mAlpha);
    source->ReadFloat(&mDimAmount);
    source->ReadFloat(&mScreenBrightness);
    source->ReadFloat(&mButtonBrightness);
    source->ReadInterfacePtr((Handle32*)&mToken);
    source->ReadString(&mPackageName);
    mTitle = NULL;
    source->ReadInterfacePtr((Handle32*)&mTitle);
    source->ReadInt32(&mScreenOrientation);
    source->ReadInt32(&mSystemUiVisibility);
    source->ReadInt32(&mSubtreeSystemUiVisibility);
    source->ReadBoolean(&mHasSystemUiListeners);
    source->ReadInt32(&mInputFeatures);
    source->ReadInt64(&mUserActivityTimeout);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    if (!dest) {
        return E_INVALID_ARGUMENT;
    }

    dest->WriteInt32(mWidth);
    dest->WriteInt32(mHeight);
    dest->WriteInt32(mX);
    dest->WriteInt32(mY);
    dest->WriteInt32(mType);
    dest->WriteInt32(mFlags);
    dest->WriteInt32(mPrivateFlags);
    dest->WriteInt32(mSoftInputMode);
    dest->WriteInt32(mGravity);
    dest->WriteFloat(mHorizontalMargin);
    dest->WriteFloat(mVerticalMargin);
    dest->WriteInt32(mFormat);
    dest->WriteInt32(mWindowAnimations);
    dest->WriteFloat(mAlpha);
    dest->WriteFloat(mDimAmount);
    dest->WriteFloat(mScreenBrightness);
    dest->WriteFloat(mButtonBrightness);
    dest->WriteInterfacePtr((IInterface*)mToken.Get());
    dest->WriteString(mPackageName);
    dest->WriteInterfacePtr((IInterface*)mTitle.Get());
    dest->WriteInt32(mScreenOrientation);
    dest->WriteInt32(mSystemUiVisibility);
    dest->WriteInt32(mSubtreeSystemUiVisibility);
    dest->WriteBoolean(mHasSystemUiListeners);
    dest->WriteInt32(mInputFeatures);
    dest->WriteInt64(mUserActivityTimeout);

    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor()
{
    ViewGroupLayoutParams::Init(MATCH_PARENT, MATCH_PARENT);

    mType = TYPE_APPLICATION;
    mFormat = IPixelFormat::OPAQUE;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor(
    /* [in] */ Int32 type)
{
    ViewGroupLayoutParams::Init(MATCH_PARENT, MATCH_PARENT);

    mType = type;
    mFormat = IPixelFormat::OPAQUE;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 flags)
{
    ViewGroupLayoutParams::Init(MATCH_PARENT, MATCH_PARENT);

    mType = type;
    mFlags = flags;
    mFormat = IPixelFormat::OPAQUE;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 format)
{
    ViewGroupLayoutParams::Init(MATCH_PARENT, MATCH_PARENT);

    mType = type;
    mFlags = flags;
    mFormat = format;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 type,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 format)
{
    ViewGroupLayoutParams::Init(w, h);

    mType = type;
    mFlags = flags;
    mFormat = format;
    return NOERROR;
}

ECode CWindowManagerLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 xpos,
    /* [in] */ Int32 ypos,
    /* [in] */ Int32 type,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 format)
{
    ViewGroupLayoutParams::Init(w, h);
    mX = xpos;
    mY = ypos;
    mType = type;
    mFlags = flags;
    mFormat = format;
    return NOERROR;
}

// ECode CWindowManagerLayoutParams::constructor(
//    /* [in] */ IParcel *in)
// {
//     return ReadFromParcel(in);
// }

ECode CWindowManagerLayoutParams::ToString(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    AutoPtr<StringBuilder> sb = new StringBuilder(256);
    sb->AppendCStr("WM.LayoutParams{");
    sb->AppendCStr("(");
    sb->AppendInt32(mX);
    sb->AppendCStr(",");
    sb->AppendInt32(mY);
    sb->AppendCStr(")(");
    if (mWidth == MATCH_PARENT)
        sb->AppendCStr("fill");
    else if (mWidth == WRAP_CONTENT)
        sb->AppendCStr("wrap");
    else
        sb->AppendInt32(mWidth);
    sb->AppendCStr("x");
    if (mHeight == MATCH_PARENT)
        sb->AppendCStr("fill");
    else if (mHeight == WRAP_CONTENT)
        sb->AppendCStr("wrap");
    else
        sb->AppendInt32(mHeight);
    sb->AppendCStr(")");
    if (mHorizontalMargin != 0) {
        sb->AppendCStr(" hm=");
        sb->AppendFloat(mHorizontalMargin);
    }
    if (mVerticalMargin != 0) {
        sb->AppendCStr(" vm=");
        sb->AppendFloat(mVerticalMargin);
    }
    if (mGravity != 0) {
        sb->AppendCStr(" gr=#");
        sb->AppendString(StringUtils::Int32ToString(mGravity, 16));
    }
    if (mSoftInputMode != 0) {
        sb->AppendCStr(" sim=#");
        sb->AppendString(StringUtils::Int32ToString(mSoftInputMode, 16));
    }
    sb->AppendCStr(" ty=");
    sb->AppendInt32(mType);
    sb->AppendCStr(" fl=#");
    sb->AppendString(StringUtils::Int32ToString(mFlags, 16));
    if (mPrivateFlags != 0) {
        sb->AppendCStr(" pfl=0x");
        sb->AppendString(StringUtils::Int32ToString(mPrivateFlags, 16));
    }
    if (mFormat != IPixelFormat::OPAQUE) {
        sb->AppendCStr(" fmt=");
        sb->AppendInt32(mFormat);
    }
    if (mWindowAnimations != 0) {
        sb->AppendCStr(" wanim=0x");
        sb->AppendString(StringUtils::Int32ToString(mWindowAnimations, 16));
    }
    if (mScreenOrientation != IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED) {
        sb->AppendCStr(" or=");
        sb->AppendInt32(mScreenOrientation);
    }
    if (mAlpha != 1.0f) {
        sb->AppendCStr(" alpha=");
        sb->AppendFloat(mAlpha);
    }
    if (mScreenBrightness != BRIGHTNESS_OVERRIDE_NONE) {
        sb->AppendCStr(" sbrt=");
        sb->AppendFloat(mScreenBrightness);
    }
    if (mButtonBrightness != BRIGHTNESS_OVERRIDE_NONE) {
        sb->AppendCStr(" bbrt=");
        sb->AppendFloat(mButtonBrightness);
    }
    if ((mFlags & FLAG_COMPATIBLE_WINDOW) != 0) {
        sb->AppendCStr(" compatible=true");
    }
    if (mSystemUiVisibility != 0) {
        sb->AppendCStr(" sysui=0x");
        sb->AppendString(StringUtils::Int32ToString(mSystemUiVisibility, 16));
    }
    if (mSubtreeSystemUiVisibility != 0) {
        sb->AppendCStr(" vsysui=0x");
        sb->AppendString(StringUtils::Int32ToString(mSubtreeSystemUiVisibility, 16));
    }
    if (mHasSystemUiListeners) {
        sb->AppendCStr(" sysuil=");
        sb->AppendBoolean(mHasSystemUiListeners);
    }
    if (mInputFeatures != 0) {
        sb->AppendCStr(" if=0x");
        sb->AppendString(StringUtils::Int32ToString(mInputFeatures, 16));
    }
    if (mUserActivityTimeout >= 0) {
        sb->AppendCStr(" userActivityTimeout=");
        sb->AppendInt64(mUserActivityTimeout);
    }
    sb->AppendCStr("}");
    *description = sb->ToString();

    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
