#include "view/CWindowInfo.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 CWindowInfo::MAX_POOL_SIZE = 20;
Int32 CWindowInfo::UNDEFINED = -1;
Mutex CWindowInfo::sPoolLock;
AutoPtr<IWindowInfo> CWindowInfo::sPool;
Int32 CWindowInfo::sPoolSize;

CWindowInfo::CWindowInfo()
    : mInPool(FALSE)
    , mType(UNDEFINED)
    , mCompatibilityScale(UNDEFINED)
    , mVisible(FALSE)
    , mDisplayId(UNDEFINED)
    , mLayer(UNDEFINED)
{
    CRect::New((IRect**)&mFrame);
    CRect::New((IRect**)&mTouchableRegion);
}

ECode CWindowInfo::constructor()
{
    return NOERROR;
}

ECode CWindowInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Handle32 tokenTmp;
    source->ReadInterfacePtr(&tokenTmp);
    mToken = reinterpret_cast<IInterface*>(tokenTmp);
    AutoPtr<IInterface> interfaceTmp;
    Handle32 hldTmp;
    source->ReadInterfacePtr(&hldTmp);
    interfaceTmp = reinterpret_cast<IInterface*>(hldTmp);
    mFrame->Set((IRect*)(IRect::Probe(interfaceTmp)));

    source->ReadInterfacePtr(&hldTmp);
    interfaceTmp = reinterpret_cast<IInterface*>(hldTmp);
    mTouchableRegion->Set((IRect*)(IRect::Probe(interfaceTmp)));
    source->ReadInt32(&mType);
    source->ReadFloat(&mCompatibilityScale);
    Int32 intTmp;
    source->ReadInt32(&intTmp);
    mVisible = (intTmp == 1);
    source->ReadInt32(&mDisplayId);
    source->ReadInt32(&mLayer);
    return NOERROR;
}

ECode CWindowInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInterfacePtr(reinterpret_cast<IInterface*>(mToken.Get()));
    dest->WriteInterfacePtr(mFrame);
    dest->WriteInterfacePtr(mTouchableRegion);
    dest->WriteInt32(mType);
    dest->WriteFloat(mCompatibilityScale);
    dest->WriteInt32(mVisible ? 1 : 0);
    dest->WriteInt32(mDisplayId);
    dest->WriteInt32(mLayer);
    return NOERROR;
}

ECode CWindowInfo::Obtain(
    /* [out] */ IWindowInfo** info)
{
    AutoLock lock(sPoolLock);
    if (sPoolSize > 0) {
        *info = sPool;
        CWindowInfo* tmp = (CWindowInfo*)(*info);
        sPool = tmp->mNext;
        tmp->mNext = NULL;
        tmp->mInPool = FALSE;
        sPoolSize--;
        REFCOUNT_ADD(*info)
        return NOERROR;
    } else {
        return CWindowInfo::New(info);
    }
}

ECode CWindowInfo::Obtain(
    /* [in] */ IWindowInfo *other,
    /* [out] */ IWindowInfo** info)
{
    Obtain(info);
    CWindowInfo* tmp = (CWindowInfo*)(*info);
    CWindowInfo* otherTmp = (CWindowInfo*)(other);
    tmp->mToken = otherTmp->mToken;
    tmp->mFrame->Set(otherTmp->mFrame);
    tmp->mTouchableRegion->Set(otherTmp->mTouchableRegion);
    tmp->mType = otherTmp->mType;
    tmp->mCompatibilityScale = otherTmp->mCompatibilityScale;
    tmp->mVisible = otherTmp->mVisible;
    tmp->mDisplayId = otherTmp->mDisplayId;
    tmp->mLayer = otherTmp->mLayer;
    return NOERROR;
}

ECode CWindowInfo::Recycle()
{
    if (mInPool) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Clear();
    AutoLock lock(sPoolLock);
    if (sPoolSize < MAX_POOL_SIZE) {
        mNext = sPool;
        sPool = this;
        mInPool = TRUE;
        sPoolSize++;
    }
    return NOERROR;
}

ECode CWindowInfo::ToString(
    /* [out] */ String* str)
{
    StringBuilder builder;
    builder += "Window [token:";
    if (mToken != NULL) {
        builder += mToken;
    } else {
        builder += "NULL";
    }
    builder += ", displayId:";
    builder += mDisplayId;
    builder += ", type:";
    builder += mType;
    builder += ", visible:";
    builder += mVisible;
    builder += ", layer:";
    builder += mLayer;
    builder += ", compatibilityScale:";
    builder += mCompatibilityScale;
    builder += ", frame:";
    String strTmp;
    mFrame->ToString(&strTmp);
    builder += strTmp;
    mTouchableRegion->ToString(&strTmp);
    builder += strTmp;
    builder += "]";
    builder.ToString(str);
    return NOERROR;
}


ECode CWindowInfo::GetDisplayId(
    /* [out] */ Int32* id)
{
    *id = mDisplayId;
    return NOERROR;
}

ECode CWindowInfo::GetType(
    /* [out] */  Int32* type)
{
    *type = mType;
    return NOERROR;
}

ECode CWindowInfo::GetFrame(
    /* [out] */  IRect** frame)
{
    *frame = mFrame;
    REFCOUNT_ADD(mFrame)
    return NOERROR;
}

ECode CWindowInfo::GetTouchableRegion(
    /* [out] */  IRect** region)
{
    *region = mTouchableRegion;
    REFCOUNT_ADD(mTouchableRegion)
    return NOERROR;
}

ECode CWindowInfo::GetToken(
    /* [out] */ IInterface** token)
{
    *token = mToken;
    REFCOUNT_ADD(mToken);
    return NOERROR;
}

ECode CWindowInfo::GetCompatibilityScale(
    /* [out] */ Float* compatibilityScale)
{
    *compatibilityScale = mCompatibilityScale;
    return NOERROR;
}

ECode CWindowInfo::GetVisible(
    /* [out] */ Boolean* visible)
{
    *visible = mVisible;
    return NOERROR;
}

ECode CWindowInfo::GetLayer(
    /* [out] */ Int32* layer)
{
    *layer = mLayer;
    return NOERROR;
}

ECode CWindowInfo::SetToken(
    /* [in] */ IInterface* token)
{
    mToken = token;
    return NOERROR;
}

ECode CWindowInfo::SetFrame(
    /* [in] */ IRect* frame)
{
    mFrame = frame;
    return NOERROR;
}

ECode CWindowInfo::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CWindowInfo::SetDisplayId(
    /* [in] */ Int32 displayId)
{
    mDisplayId = displayId;
    return NOERROR;
}

ECode CWindowInfo::SetCompatibilityScale(
    /* [in] */ Float compatibilityScale)
{
    mCompatibilityScale = compatibilityScale;
    return NOERROR;
}

ECode CWindowInfo::SetVisible(
    /* [in] */ Boolean visible)
{
    mVisible = visible;
    return NOERROR;
}

ECode CWindowInfo::SetLayer(
    /* [in] */ Int32 layer)
{
    mLayer = layer;
    return NOERROR;
}

ECode CWindowInfo::SetTouchableRegion(
    /* [in] */ IRect* touchableRegion)
{
    mTouchableRegion = touchableRegion;
    return NOERROR;
}

ECode CWindowInfo::Clear()
{
    mToken = NULL;
    mFrame->SetEmpty();
    mTouchableRegion->SetEmpty();
    mType = UNDEFINED;
    mCompatibilityScale = UNDEFINED;
    mVisible = FALSE;
    mDisplayId = UNDEFINED;
    mLayer = UNDEFINED;
    return NOERROR;
}

}// namespace View
}// namespace Droid
}// namespace Elastos