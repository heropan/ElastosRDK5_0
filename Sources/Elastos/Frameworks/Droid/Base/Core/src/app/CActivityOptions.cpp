
#include "app/CActivityOptions.h"
#include "app/CActivityOptionsAnimationStartedListener.h"
#include "os/CBundle.h"
#include "os/Handler.h"

using Elastos::Droid::Os::EIID_IRemoteCallback;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::App::CActivityOptionsAnimationStartedListener;

namespace Elastos {
namespace Droid {
namespace App {

//=============================================================================
//              AnimationStartedListenerRunnable
//=============================================================================

AnimationStartedListenerRunnable::AnimationStartedListenerRunnable(
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener)
    : mListener(listener)
{
}

ECode AnimationStartedListenerRunnable::Run()
{
    if (mListener != NULL) {
        mListener->OnAnimationStarted();
    }
    return NOERROR;
}

//=============================================================================
//              CActivityOptions
//=============================================================================
ECode CActivityOptions::MakeCustomAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 enterResId,
    /* [in] */ Int32 exitResId,
    /* [out] */ IActivityOptions** options)
{
    return MakeCustomAnimation(context, enterResId, exitResId, NULL, NULL, options);
}

ECode CActivityOptions::MakeCustomAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 enterResId,
    /* [in] */ Int32 exitResId,
    /* [in] */ IHandler* handler,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** options)
{
    VALIDATE_NOT_NULL(options);
    assert(context != NULL);

    String pkgName;
    context->GetPackageName(&pkgName);

    AutoPtr<IActivityOptions> opts;
    CActivityOptions::New((IActivityOptions**)&opts);

    opts->SetPackageName(pkgName);
    opts->SetAnimationType(IActivityOptions::ANIM_CUSTOM);
    opts->SetCustomEnterResId(enterResId);
    opts->SetCustomExitResId(exitResId);
    ((CActivityOptions*)opts.Get())->SetListener(handler, listener);
    *options = opts;
    INTERFACE_ADDREF(*options);
    return NOERROR;
}

ECode CActivityOptions::SetListener(
    /* [in] */ IHandler* handler,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener)
{
    if (listener != NULL) {
        mAnimationStartedListener = NULL;
        CActivityOptionsAnimationStartedListener::New(handler, listener, (IRemoteCallback**)&mAnimationStartedListener);
    }
    return NOERROR;
}

ECode CActivityOptions::MakeScaleUpAnimation(
    /* [in] */ IView* source,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 startWidth,
    /* [in] */ Int32 startHeight,
    /* [out] */ IActivityOptions** options)
{
    VALIDATE_NOT_NULL(options);
    assert(source != NULL);

    AutoPtr<IContext> context;
    source->GetContext((IContext**)&context);
    String pkgName;
    context->GetPackageName(&pkgName);
    Int32 x, y;
    source->GetLocationOnScreen(&x, &y);

    AutoPtr<IActivityOptions> opts;
    CActivityOptions::New((IActivityOptions**)&opts);
    opts->SetPackageName(pkgName);
    opts->SetAnimationType(IActivityOptions::ANIM_SCALE_UP);
    opts->SetStartX(x + startX);
    opts->SetStartY(y + startY);
    opts->SetStartWidth(startWidth);
    opts->SetStartHeight(startHeight);
    *options = opts;
    INTERFACE_ADDREF(*options);
    return NOERROR;
}

ECode CActivityOptions::MakeThumbnailScaleUpAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [out] */ IActivityOptions** options)
{
    return MakeThumbnailScaleUpAnimation(source, thumbnail, startX, startY, NULL, options);
}

ECode CActivityOptions::MakeThumbnailScaleUpAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** options)
{
    return MakeThumbnailAnimation(source, thumbnail, startX, startY, listener, TRUE, options);
}

ECode CActivityOptions::MakeThumbnailScaleDownAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [out] */ IActivityOptions** options)
{
    return MakeThumbnailAnimation(source, thumbnail, startX, startY, listener, FALSE, options);
}

ECode CActivityOptions::MakeThumbnailAnimation(
    /* [in] */ IView* source,
    /* [in] */ IBitmap* thumbnail,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ IActivityOptionsOnAnimationStartedListener* listener,
    /* [in] */ Boolean scaleUp,
    /* [out] */ IActivityOptions** options)
{
    VALIDATE_NOT_NULL(options);
    assert(source != NULL);

    AutoPtr<IContext> context;
    source->GetContext((IContext**)&context);
    String pkgName;
    context->GetPackageName(&pkgName);
    Int32 x, y;
    source->GetLocationOnScreen(&x, &y);
    AutoPtr<IHandler> handler;
    source->GetHandler((IHandler**)&handler);
    AutoPtr<IActivityOptions> opts;
    CActivityOptions::New((IActivityOptions**)&opts);
    opts->SetPackageName(pkgName);
    opts->SetAnimationType(scaleUp ? IActivityOptions::ANIM_SCALE_UP :IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN);
    opts->SetThumbnail(thumbnail);
    opts->SetStartX(x + startX);
    opts->SetStartY(y + startY);
    ((CActivityOptions*)opts.Get())->SetListener(handler, listener);
    *options = opts;
    INTERFACE_ADDREF(*options);
    return NOERROR;
}

CActivityOptions::CActivityOptions()
    : mAnimationType(0)
    , mCustomEnterResId(0)
    , mCustomExitResId(0)
    , mStartX(0)
    , mStartY(0)
    , mStartWidth(0)
    , mStartHeight(0)
{}

ECode CActivityOptions::constructor()
{
    return NOERROR;
}

ECode CActivityOptions::constructor(
    /* [in] */ IBundle* opts)
{
    assert(opts != NULL);

    opts->GetString(IActivityOptions::KEY_PACKAGE_NAME, &mPackageName);
    opts->GetInt32(IActivityOptions::KEY_ANIM_TYPE, &mAnimationType);

    if (mAnimationType == IActivityOptions::ANIM_CUSTOM) {
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_ENTER_RES_ID, 0, &mCustomEnterResId);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_EXIT_RES_ID, 0, &mCustomExitResId);

        AutoPtr<IBinder> binder;
        opts->GetIBinder(IActivityOptions::KEY_ANIM_START_LISTENER, (IBinder**)&binder);
        mAnimationStartedListener = IRemoteCallback::Probe(binder);
    }
    else if (mAnimationType == IActivityOptions::ANIM_SCALE_UP) {
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_X, 0, &mStartX);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_Y, 0, &mStartY);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_WIDTH, 0, &mStartWidth);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_HEIGHT, 0, &mStartHeight);
    }
    else if (mAnimationType == IActivityOptions::ANIM_THUMBNAIL_SCALE_UP ||
            mAnimationType == IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN) {
        AutoPtr<IParcelable> parcelable;
        opts->GetParcelable(IActivityOptions::KEY_ANIM_THUMBNAIL, (IParcelable**)&parcelable);
        mThumbnail = IBitmap::Probe(parcelable);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_X, 0, &mStartX);
        opts->GetInt32Ex(IActivityOptions::KEY_ANIM_START_Y, 0, &mStartY);

        AutoPtr<IBinder> binder;
        opts->GetIBinder(IActivityOptions::KEY_ANIM_START_LISTENER, (IBinder**)&binder);
        mAnimationStartedListener = IRemoteCallback::Probe(binder);
    }
    return NOERROR;
}

ECode CActivityOptions::GetPackageName(
    /* [out] */ String* pkgName)
{
    VALIDATE_NOT_NULL(pkgName);
    *pkgName = mPackageName;
    return NOERROR;
}

ECode CActivityOptions::SetPackageName(
    /* [in] */ const String& pkgName)
{
    mPackageName = pkgName;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetAnimationType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mAnimationType;
    return NOERROR;
}

ECode CActivityOptions::SetAnimationType(
    /* [in] */ Int32 type)
{
    mAnimationType = type;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetCustomEnterResId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mCustomEnterResId;
    return NOERROR;
}

ECode CActivityOptions::SetCustomEnterResId(
    /* [in] */ Int32 id)
{
    mCustomEnterResId = id;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetCustomExitResId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mCustomExitResId;
    return NOERROR;
}

ECode CActivityOptions::SetCustomExitResId(
    /* [in] */ Int32 id)
{
    mCustomExitResId = id;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetThumbnail(
    /* [out] */ IBitmap** thumbnail)
{
    VALIDATE_NOT_NULL(thumbnail);
    *thumbnail = mThumbnail;
    INTERFACE_ADDREF(*thumbnail);
    return NOERROR;
}

ECode CActivityOptions::SetThumbnail(
    /* [in] */ IBitmap* thumbnail)
{
    mThumbnail = thumbnail;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetStartX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);
    *x = mStartX;
    return NOERROR;
}

ECode CActivityOptions::SetStartX(
    /* [in] */ Int32 x)
{
    mStartX = x;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetStartY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);
    *y = mStartY;
    return NOERROR;
}

ECode CActivityOptions::SetStartY(
    /* [in] */ Int32 y)
{
    mStartY = y;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetStartWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mStartWidth;
    return NOERROR;
}

ECode CActivityOptions::SetStartWidth(
    /* [in] */ Int32 width)
{
    mStartWidth = width;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetStartHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mStartHeight;
    return NOERROR;
}

ECode CActivityOptions::SetStartHeight(
    /* [in] */ Int32 height)
{
    mStartHeight = height;
    return NOERROR;
}

/** @hide */
ECode CActivityOptions::GetOnAnimationStartListener(
    /* [out] */ IRemoteCallback** cb)
{
    VALIDATE_NOT_NULL(cb);
    *cb = mAnimationStartedListener;
    INTERFACE_ADDREF(*cb);
    return NOERROR;
}

ECode CActivityOptions::SetOnAnimationStartListener(
    /* [in] */ IRemoteCallback* cb)
{
    mAnimationStartedListener = cb;
    return NOERROR;
}

ECode CActivityOptions::Abort()
{
   if (mAnimationStartedListener != NULL) {
        // try {
            mAnimationStartedListener->SendResult(NULL);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}

ECode CActivityOptions::Abort(
    /* [in] */ IBundle* options)
{
    if (options != NULL) {
        AutoPtr<IActivityOptions> opts;
        CActivityOptions::New(options, (IActivityOptions**)&opts);
        opts->Abort();
    }
    return NOERROR;
}

ECode CActivityOptions::Update(
    /* [in] */ IActivityOptions* otherOptions)
{
    assert(otherOptions != NULL);
    String pkgName;
    otherOptions->GetPackageName(&pkgName);
    if (!pkgName.IsNull()) {
        mPackageName = pkgName;
    }

    Int32 animationType;
    otherOptions->GetAnimationType(&animationType);
    AutoPtr<IRemoteCallback> callback;
    otherOptions->GetOnAnimationStartListener((IRemoteCallback**)&callback);
    switch (animationType) {
        case IActivityOptions::ANIM_CUSTOM:
            mAnimationType = animationType;
            otherOptions->GetCustomEnterResId(&mCustomEnterResId);
            otherOptions->GetCustomExitResId(&mCustomExitResId);
            mThumbnail = NULL;
            if (callback != NULL) {
                // try {
                    callback->SendResult(NULL);
                // } catch (RemoteException e) {
                // }
            }
            mAnimationStartedListener = callback;
            break;
        case IActivityOptions::ANIM_SCALE_UP:
            mAnimationType = animationType;
            otherOptions->GetStartX(&mStartX);
            otherOptions->GetStartX(&mStartY);
            otherOptions->GetStartWidth(&mStartWidth);
            otherOptions->GetStartHeight(&mStartHeight);
            if (callback != NULL) {
                // try {
                    callback->SendResult(NULL);
                // } catch (RemoteException e) {
                // }
            }
            mAnimationStartedListener = NULL;
            break;
        case IActivityOptions::ANIM_THUMBNAIL_SCALE_UP:
        case IActivityOptions::ANIM_THUMBNAIL_SCALE_DOWN:
            mAnimationType = animationType;
            mThumbnail = NULL;
            otherOptions->GetThumbnail((IBitmap**)&mThumbnail);
            otherOptions->GetStartX(&mStartX);
            otherOptions->GetStartX(&mStartY);
            if (callback != NULL) {
                // try {
                    callback->SendResult(NULL);
                // } catch (RemoteException e) {
                // }
            }
            mAnimationStartedListener = callback;
            break;
    }
    return NOERROR;
}

ECode CActivityOptions::ToBundle(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);

    AutoPtr<IBundle> b;
    CBundle::New((IBundle**)&b);

    if (!mPackageName.IsNull()) {
        b->PutString(IActivityOptions::KEY_PACKAGE_NAME, mPackageName);
    }
    switch (mAnimationType) {
        case ANIM_CUSTOM:
            b->PutInt32(IActivityOptions::KEY_ANIM_TYPE, mAnimationType);
            b->PutInt32(IActivityOptions::KEY_ANIM_ENTER_RES_ID, mCustomEnterResId);
            b->PutInt32(IActivityOptions::KEY_ANIM_EXIT_RES_ID, mCustomExitResId);
            b->PutIBinder(IActivityOptions::KEY_ANIM_START_LISTENER,
                mAnimationStartedListener ? (IBinder*)mAnimationStartedListener->Probe(EIID_IBinder) : NULL);
            break;
        case ANIM_SCALE_UP:
            b->PutInt32(IActivityOptions::KEY_ANIM_TYPE, mAnimationType);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_X, mStartX);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_Y, mStartY);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_WIDTH, mStartWidth);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_HEIGHT, mStartHeight);
            break;
        case ANIM_THUMBNAIL_SCALE_UP:
        case ANIM_THUMBNAIL_SCALE_DOWN:
            b->PutInt32(IActivityOptions::KEY_ANIM_TYPE, mAnimationType);
            b->PutParcelable(IActivityOptions::KEY_ANIM_THUMBNAIL, mThumbnail ? (IParcelable*)mThumbnail->Probe(EIID_IParcelable) : NULL);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_X, mStartX);
            b->PutInt32(IActivityOptions::KEY_ANIM_START_Y, mStartY);
            b->PutIBinder(IActivityOptions::KEY_ANIM_START_LISTENER,
                mAnimationStartedListener ? (IBinder*)mAnimationStartedListener->Probe(EIID_IBinder) : NULL);
            break;
    }

    *bundle = b;
    INTERFACE_ADDREF(*bundle);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
