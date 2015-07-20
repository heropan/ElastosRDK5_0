#ifndef __CWINDOWINFO_H__
#define __CWINDOWINFO_H__

#include "_CWindowInfo.h"
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowInfo)
{
public:
    CWindowInfo();

    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    static CARAPI Obtain(
        /* [out] */ IWindowInfo** info);

    static CARAPI Obtain(
        /* [in] */ IWindowInfo *other,
        /* [out] */ IWindowInfo** info);

    CARAPI Recycle();

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetDisplayId(
        /* [out] */ Int32* id);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI GetFrame(
        /* [out] */ IRect** frame);

    CARAPI GetTouchableRegion(
        /* [out] */ IRect** region);

    CARAPI GetToken(
        /* [out] */ IInterface** token);

    CARAPI GetCompatibilityScale(
        /* [out] */ Float* compatibilityScale);

    CARAPI GetVisible(
        /* [out] */ Boolean* visible);

    CARAPI GetLayer(
        /* [out] */ Int32* layer);

    CARAPI SetToken(
        /* [in] */ IInterface* token);

    CARAPI SetFrame(
        /* [in] */ IRect* frame);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI SetDisplayId(
        /* [in] */ Int32 displayId);

    CARAPI SetCompatibilityScale(
        /* [in] */ Float compatibilityScale);

    CARAPI SetVisible(
        /* [in] */ Boolean visible);

    CARAPI SetLayer(
        /* [in] */ Int32 layer);

    CARAPI SetTouchableRegion(
        /* [in] */ IRect* touchableRegion);

private:
    CARAPI Clear();

    static const Int32 MAX_POOL_SIZE;
    static Int32 UNDEFINED;
    static Object sPoolLock;
    static AutoPtr<IWindowInfo> sPool;
    static Int32 sPoolSize;

    AutoPtr<IWindowInfo> mNext;
    Boolean mInPool;

    AutoPtr<IInterface> mToken;

    AutoPtr<IRect> mFrame;

    AutoPtr<IRect> mTouchableRegion;

    Int32 mType;

    Float mCompatibilityScale;

    Boolean mVisible;

    Int32 mDisplayId;

    Int32 mLayer;
};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif