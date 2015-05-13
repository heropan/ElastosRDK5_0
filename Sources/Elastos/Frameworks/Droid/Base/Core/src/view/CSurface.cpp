
#include "ext/frameworkdef.h"
#include "view/CSurface.h"
#include "view/CSurfaceSession.h"
#include "graphics/CMatrix.h"
#include "graphics/CSurfaceTexture.h"
#include "graphics/CRegion.h"
#include "os/CSystemProperties.h"
#include "os/NativeBinder.h"
#include <elastos/Logger.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/PixelFormat.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkRegion.h>
#include <utils/String8.h>
#include <skia/core/SkPixelRef.h>
//#include <ScopedUtfChars.h>
#include <ui/DisplayInfo.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <binder/IBinder.h>
#include <binder/IMemory.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::DroidObjectForIBinder;

namespace Elastos {
namespace Droid {
namespace View {

const char* CSurface::TAG = "CSurface";

Boolean CSurface::HEADLESS = GetResult();

static SkBitmap::Config convertPixelFormat(android::PixelFormat format)
{
    /* note: if PIXEL_FORMAT_RGBX_8888 means that all alpha bytes are 0xFF, then
        we can map to SkBitmap::kARGB_8888_Config, and optionally call
        bitmap.setIsOpaque(true) on the resulting SkBitmap (as an accelerator)
    */
    switch (format) {
    case android::PIXEL_FORMAT_RGBX_8888:    return SkBitmap::kARGB_8888_Config;
    case android::PIXEL_FORMAT_RGBA_8888:    return SkBitmap::kARGB_8888_Config;
    case android::PIXEL_FORMAT_RGBA_4444:    return SkBitmap::kARGB_4444_Config;
    case android::PIXEL_FORMAT_RGB_565:      return SkBitmap::kRGB_565_Config;
    case android::PIXEL_FORMAT_A_8:          return SkBitmap::kA8_Config;
    default:                        return SkBitmap::kNo_Config;
    }
}

////////////////////////////////////////////////////////////
// CSurface::CompatibleCanvas

CSurface::_CompatibleCanvas::_CompatibleCanvas(
    /* [in] */ CSurface* owner)
    : mOwner(owner)
{
    ASSERT_SUCCEEDED(Canvas::Init());
}

ECode CSurface::_CompatibleCanvas::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    Canvas::GetWidth(width);
    if (mOwner->mCompatibilityTranslator != NULL) {
        Float appInvertedScale;
        mOwner->mCompatibilityTranslator->GetApplicationInvertedScale(&appInvertedScale);
        *width = (Int32)(*width * appInvertedScale + 0.5f);
    }

    return NOERROR;
}

ECode CSurface::_CompatibleCanvas::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    Canvas::GetHeight(height);
    if (mOwner->mCompatibilityTranslator != NULL) {
        Float appInvertedScale;
        mOwner->mCompatibilityTranslator->GetApplicationInvertedScale(&appInvertedScale);
        *height = (Int32)(*height * appInvertedScale + 0.5f);
    }

    return NOERROR;
}

ECode CSurface::_CompatibleCanvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    Boolean isEquals;
    if (mOwner->mCompatibleMatrix == NULL || mOrigMatrix == NULL
        || (mOrigMatrix->Equals(matrix, &isEquals), isEquals)) {
       // don't scale the matrix if it's not compatibility mode, or
       // the matrix was obtained from getMatrix.
       return Canvas::SetMatrix(matrix);
    }
    else {
       AutoPtr<IMatrix> m;
       CMatrix::New(mOwner->mCompatibleMatrix, (IMatrix**)&m);
       Boolean res;
       m->PreConcat(matrix, &res);
       return Canvas::SetMatrix(m);
    }
}

ECode CSurface::_CompatibleCanvas::GetMatrixEx(
    /* [out] */ IMatrix** m)
{
    VALIDATE_NOT_NULL(m);

    Canvas::GetMatrixEx(m);
    if (mOrigMatrix == NULL) {
        CMatrix::New((IMatrix**)&mOrigMatrix);
    }
    return  mOrigMatrix->Set(*m);
}

CSurface::CompatibleCanvas::CompatibleCanvas(
    /* [in] */ CSurface* owner)
    : _CompatibleCanvas(owner)
{}

ICANVAS_METHODS_IMPL(CSurface::CompatibleCanvas, CSurface::_CompatibleCanvas);

PInterface CSurface::CompatibleCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICanvas*)this;
    }
    else if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    else if (riid == EIID_ICanvas) {
        return (ICanvas*)this;
    }

    return NULL;
}

UInt32 CSurface::CompatibleCanvas::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CSurface::CompatibleCanvas::Release()
{
    return ElRefBase::Release();
}

ECode CSurface::CompatibleCanvas::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICanvas*)this) {
        *pIID = EIID_ICanvas;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

class ScreenshotPixelRef : public SkPixelRef {
public:
    ScreenshotPixelRef(SkColorTable* ctable) {
        mCTable = ctable;
        SkSafeRef(ctable);
        setImmutable();
    }

    virtual ~ScreenshotPixelRef() {
        SkSafeUnref(mCTable);
    }

    android::status_t update(
        const android::sp<android::IBinder>& display, int width, int height,
            int minLayer, int maxLayer, bool allLayers) {
        android::status_t res = (width > 0 && height > 0)
                ? (allLayers
                        ? mScreenshot.update(display, width, height)
                        : mScreenshot.update(display, width, height, minLayer, maxLayer))
                : mScreenshot.update(display);
        if (res != android::NO_ERROR) {
            return res;
        }

        return android::NO_ERROR;
    }

    uint32_t getWidth() const {
        return mScreenshot.getWidth();
    }

    uint32_t getHeight() const {
        return mScreenshot.getHeight();
    }

    uint32_t getStride() const {
        return mScreenshot.getStride();
    }

    uint32_t getFormat() const {
        return mScreenshot.getFormat();
    }

protected:
    // overrides from SkPixelRef
    virtual void* onLockPixels(SkColorTable** ct) {
        *ct = mCTable;
        return (void*)mScreenshot.getPixels();
    }

    virtual void onUnlockPixels() {
    }

private:
    android::ScreenshotClient mScreenshot;
    SkColorTable* mCTable;

    typedef SkPixelRef INHERITED;
};

ECode CSurface::Screenshot(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bitmap)
{
    // TODO: should take the display as a parameter
    AutoPtr<IBinder> displayToken;
    GetBuiltInDisplay(BUILT_IN_DISPLAY_ID_MAIN, (IBinder**)&displayToken);
    assert(bitmap != NULL);
    AutoPtr<IBitmap> temp = NativeScreenshot(displayToken, width, height, 0, 0, TRUE);
    *bitmap = temp;
    INTERFACE_ADDREF(*bitmap);

    return NOERROR;
}

ECode CSurface::ScreenshotEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [out] */ IBitmap** bitmap)
{
    // TODO: should take the display as a parameter
    AutoPtr<IBinder> displayToken;
    GetBuiltInDisplay(BUILT_IN_DISPLAY_ID_MAIN, (IBinder**)&displayToken);

    assert(bitmap != NULL);
    AutoPtr<IBitmap> temp = NativeScreenshot(displayToken, width, height, minLayer, maxLayer, FALSE);
    *bitmap = temp;
    INTERFACE_ADDREF(*bitmap);

    return NOERROR;
}

ECode CSurface::OpenTransaction()
{
    android::SurfaceComposerClient::openGlobalTransaction();
    return NOERROR;
}

ECode CSurface::CloseTransaction()
{
    android::SurfaceComposerClient::closeGlobalTransaction();
    return NOERROR;
}

ECode CSurface::SetAnimationTransaction()
{
    NativeSetAnimationTransaction();
    return NOERROR;
}

ECode CSurface::GetBuiltInDisplay(
    /* [in] */ Int32 builtInDisplayId,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    AutoPtr<IBinder> temp = NativeGetBuiltInDisplay(builtInDisplayId);
    *binder = temp;
    INTERFACE_ADDREF(*binder);
    return NOERROR;
}

ECode CSurface::CreateDisplay(
    /* [in] */ const String& name,
    /* [in] */ Boolean secure,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    if (name.IsNull()) {
        assert(0);
        //throw new IllegalArgumentException("name must not be null");
    }

    AutoPtr<IBinder> temp = NativeCreateDisplay(name, secure);
    *binder = temp;
    INTERFACE_ADDREF(*binder);
    return NOERROR;
}

ECode CSurface::SetDisplaySurface(
    /* [in] */ IBinder* displayToken,
    /* [in] */ ISurface* surface)
{
    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }

    NativeSetDisplaySurface(displayToken, surface);
    return NOERROR;
}

ECode CSurface::SetDisplayLayerStack(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 layerStack)
{
    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }

    NativeSetDisplayLayerStack(displayToken, layerStack);
    return NOERROR;
}

ECode CSurface::SetDisplayProjection(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* layerStackRect,
    /* [in] */ IRect* displayRect)
{
    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }

    if (layerStackRect == NULL) {
        assert(0);
        //throw new IllegalArgumentException("layerStackRect must not be null");
    }

    if (displayRect == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayRect must not be null");
    }

    NativeSetDisplayProjection(displayToken, orientation, layerStackRect, displayRect);
    return NOERROR;
}

ECode CSurface::GetDisplayInfo(
    /* [in] */ IBinder* displayToken,
    /* [in] */ IPhysicalDisplayInfo* outInfo,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }
    if (outInfo == NULL) {
        assert(0);
        //throw new IllegalArgumentException("outInfo must not be null");
    }

    *result = NativeGetDisplayInfo(displayToken, outInfo);
    return NOERROR;
}

ECode CSurface::BlankDisplay(
    /* [in] */ IBinder* displayToken)
{
    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }

    NativeBlankDisplay(displayToken);
    return NOERROR;
}

ECode CSurface::UnblankDisplay(
    /* [in] */ IBinder* displayToken)
{
    if (displayToken == NULL) {
        assert(0);
        //throw new IllegalArgumentException("displayToken must not be null");
    }

    NativeUnblankDisplay(displayToken);
    return NOERROR;
}

ECode CSurface::NativeSetAnimationTransaction()
{
    android::SurfaceComposerClient::setAnimationTransaction();
    return NOERROR;
}

AutoPtr<IBitmap> CSurface::NativeScreenshot(
    /* [in] */ IBinder* displayTokenObj,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [in] */ Boolean allLayers)
{
    android::sp<android::IBinder> displayToken = IBinderForDroidObject(displayTokenObj);
    if (displayToken == NULL) {
        return NULL;
    }

    ScreenshotPixelRef* pixels = new ScreenshotPixelRef(NULL);
    if (pixels->update(displayToken, width, height,
            minLayer, maxLayer, allLayers) != android::NO_ERROR) {
        delete pixels;
        return NULL;
    }

    uint32_t w = pixels->getWidth();
    uint32_t h = pixels->getHeight();
    uint32_t s = pixels->getStride();
    uint32_t f = pixels->getFormat();
    ssize_t bpr = s * android::bytesPerPixel(f);

    SkBitmap* bitmap = new SkBitmap();
    bitmap->setConfig(convertPixelFormat(f), w, h, bpr);
    if (f == android::PIXEL_FORMAT_RGBX_8888) {
        bitmap->setIsOpaque(true);
    }

    if (w > 0 && h > 0) {
        bitmap->setPixelRef(pixels)->unref();
        bitmap->lockPixels();
    } else {
        // be safe with an empty bitmap.
        delete pixels;
        bitmap->setPixels(NULL);
    }

    AutoPtr<IBitmap> bmap;
    CBitmap::New(
        (Int32)bitmap, NULL, FALSE, NULL, -1, (IBitmap**)&bmap);
    return bmap;
}

AutoPtr<IBinder> CSurface::NativeGetBuiltInDisplay(
    /* [in] */ Int32 id)
{
    android::sp<android::IBinder> token(android::SurfaceComposerClient::getBuiltInDisplay(id));
    return DroidObjectForIBinder(token);
}

AutoPtr<IBinder> CSurface::NativeCreateDisplay(
    /* [in] */ const String& nameObj,
    /* [in] */ Boolean secure)
{
    //TODO
    //ScopedUtfChars name(env, nameObj);
    // android::sp<IBinder> token(SurfaceComposerClient::createDisplay(
    //         String8(name.c_str()), bool(secure)));
    android::sp<android::IBinder> token(android::SurfaceComposerClient::createDisplay(
            android::String8((const char*)nameObj), bool(secure)));

    assert(0);
    //TODO
    //return javaObjectForIBinder(env, token);
    return NULL;
}

ECode CSurface::NativeSetDisplaySurface(
    /* [in] */ IBinder* displayToken,
    /* [in] */ ISurface* surfaceObj)
{
    assert(0);
    //TODO
    // android::sp<IBinder> token(ibinderForJavaObject(env, tokenObj));
    android::sp<android::IBinder> token = NULL;
    if (token == NULL) return NOERROR;

    android::sp<android::ISurfaceTexture> surfaceTexture = NULL;
    if (surfaceObj != NULL) {
        android::sp<android::Surface> surface = ((CSurface*)surfaceObj)->mNativeSurface;
        if (surface != NULL) {
            surfaceTexture = surface->getSurfaceTexture();
        }
    }

    android::SurfaceComposerClient::setDisplaySurface(token, surfaceTexture);

    return NOERROR;
}

ECode CSurface::NativeSetDisplayLayerStack(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 layerStack)
{
    android::sp<android::IBinder> token(IBinderForDroidObject(displayToken));
    if (token == NULL) return NOERROR;

    android::SurfaceComposerClient::setDisplayLayerStack(token, layerStack);

    return NOERROR;
}

ECode CSurface::NativeSetDisplayProjection(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* layerStackRectObj,
    /* [in] */ IRect* displayRectObj)
{
    android::sp<android::IBinder> token(IBinderForDroidObject(displayToken));
    if (token == NULL) return NOERROR;

    android::Rect layerStackRect;
    layerStackRectObj->GetLeft(&layerStackRect.left);
    layerStackRectObj->GetTop(&layerStackRect.top);
    layerStackRectObj->GetRight(&layerStackRect.right);
    layerStackRectObj->GetBottom(&layerStackRect.bottom);

    android::Rect displayRect;
    displayRectObj->GetLeft(&displayRect.left);
    displayRectObj->GetTop(&displayRect.top);
    displayRectObj->GetRight(&displayRect.right);
    displayRectObj->GetBottom(&displayRect.bottom);

    android::SurfaceComposerClient::setDisplayProjection(token, orientation, layerStackRect, displayRect);

    return NOERROR;
}

Boolean CSurface::NativeGetDisplayInfo(
    /* [in] */ IBinder* displayToken,
    /* [in] */ IPhysicalDisplayInfo* infoObj)
{
    android::sp<android::IBinder> token(IBinderForDroidObject(displayToken));
    if (token == NULL) return FALSE;

    android::DisplayInfo info;
    if (android::SurfaceComposerClient::getDisplayInfo(token, &info)) {
        return FALSE;
    }

    infoObj->SetWidth(info.w);
    infoObj->SetHeight(info.h);
    infoObj->SetRefreshRate(info.fps);
    infoObj->SetDensity(info.density);
    infoObj->SetXDpi(info.xdpi);
    infoObj->SetYDpi(info.ydpi);
    infoObj->SetSecure(info.secure);

    return TRUE;
}

ECode CSurface::NativeBlankDisplay(
    /* [in] */ IBinder* displayToken)
{
    android::sp<android::IBinder> token(IBinderForDroidObject(displayToken));
    if (token == NULL) return NOERROR;

    // ALOGD_IF_SLOW(100, "Excessive delay in blankDisplay() while turning screen off");
    android::SurfaceComposerClient::blankDisplay(token);

    return NOERROR;
}

ECode CSurface::NativeUnblankDisplay(
    /* [in] */ IBinder* displayToken)
{
    android::sp<android::IBinder> token(IBinderForDroidObject(displayToken));
    if (token == NULL) return NOERROR;

    // ALOGD_IF_SLOW(100, "Excessive delay in unblankDisplay() while turning screen on");
    android::SurfaceComposerClient::unblankDisplay(token);

    return NOERROR;
}

CSurface::CSurface()
    : mGenerationId(0)
    , mCanvasSaveCount(0)
    , mWidth(0)
    , mHeight(0)
{
    mCanvas = new CompatibleCanvas(this);
}

ECode CSurface::constructor()
{
    FAIL_RETURN(CheckHeadless());
    //mCloseGuard->Open("release");
    return NOERROR;
}

ECode CSurface::constructor(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ const String& name,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
    if (session == NULL) {
        Logger::E(TAG, "session must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (name.IsNull()) {
        Logger::E(TAG, "name must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((flags & HIDDEN) == 0) {
        // Log.w(TAG, "Surfaces should always be created with the HIDDEN flag set "
        //         + "to ensure that they are not made visible prematurely before "
        //         + "all of the surface's properties have been configured.  "
        //         + "Set the other properties and make the surface visible within "
        //         + "a transaction.  New surface name: " + name,
        //         new Throwable());
    }

    FAIL_RETURN(CheckHeadless());

    mName = name;
    mWidth = w;
    mHeight = h;
    NativeCreate(session, name, w, h, format, flags);
    //mCloseGuard->Open("release");
    return NOERROR;
}

ECode CSurface::constructor(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    if (surfaceTexture == NULL) {
        Logger::E(TAG, "surfaceTexture must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(CheckHeadless());
    //surfaceTexture->ToString(&mName);
    // try {
    ECode ec = NativeCreateFromSurfaceTexture(surfaceTexture);
    if (ec == E_OUT_OF_RESOURCES_EXCEPTION) {
        ec = E_RUNTIME_EXCEPTION;
    }
    // } catch (OutOfResourcesException ex) {
    //     // We can't throw OutOfResourcesException because it would be an API change.
    //     throw new RuntimeException(ex);
    // }

    //mCloseGuard->Open("release");
    return ec;
}

CSurface::~CSurface()
{
    // if (mCloseGuard != NULL) {
    //     mCloseGuard->WarnIfOpen();
    // }

    NativeRelease();
}

ECode CSurface::ReleaseSurface()
{
    NativeRelease();
    //mCloseGuard->Close();
    return NOERROR;
}

ECode CSurface::Destroy()
{
    NativeDestroy();
    //mCloseGuard->Close();
    return NOERROR;
}

ECode CSurface::IsValid(
    /* [out] */ Boolean* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = NativeIsValid();
    return NOERROR;
}

ECode CSurface::GetGenerationId(
    /* [out] */ Int32* generationId)
{
    VALIDATE_NOT_NULL(generationId);
    *generationId = mGenerationId;
    return NOERROR;
}

ECode CSurface::IsConsumerRunningBehind(
    /* [out] */ Boolean* consumer)
{
    VALIDATE_NOT_NULL(consumer);
    *consumer = NativeIsConsumerRunningBehind();
    return NOERROR;
}

ECode CSurface::LockCanvas(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    return NativeLockCanvas(dirty, canvas);
}

ECode CSurface::UnlockCanvasAndPost(
    /* [in] */ ICanvas* canvas)
{
    return NativeUnlockCanvasAndPost(canvas);
}

ECode CSurface::UnlockCanvas(
    /* [in] */ ICanvas* canvas)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CSurface::SetCompatibilityTranslator(
    /* [in] */ ICompatibilityInfoTranslator* translator)
{
    if (translator != NULL) {
        Float appScale;
        translator->GetApplicationScale(&appScale);
        mCompatibleMatrix = NULL;
        CMatrix::New((IMatrix**)&mCompatibleMatrix);
        assert(mCompatibleMatrix != NULL);

        mCompatibleMatrix->SetScaleEx(appScale, appScale);
    }
    return NOERROR;
}

ECode CSurface::SetLayer(
    /* [in] */ Int32 zorder)
{
    NativeSetLayer(zorder);
    return NOERROR;
}

ECode CSurface::SetPosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    NativeSetPosition(x, y);
    return NOERROR;
}

ECode CSurface::SetSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    mWidth = w;
    mHeight = h;
    NativeSetSize(w, h);
    return NOERROR;
}

ECode CSurface::GetWidth(
    /* [out] */ Int32* w)
{
    VALIDATE_NOT_NULL(w);
    *w = mWidth;
    return NOERROR;
}

ECode CSurface::GetHeight(
    /* [out] */ Int32* h)
{
    VALIDATE_NOT_NULL(h);
    *h = mHeight;
    return NOERROR;
}

ECode CSurface::Hide()
{
    return NativeSetFlags(SURFACE_HIDDEN, SURFACE_HIDDEN);
}

ECode CSurface::Show()
{
    return NativeSetFlags(0, SURFACE_HIDDEN);
}

ECode CSurface::SetTransparentRegionHint(
    /* [in] */ IRegion* region)
{
    return NativeSetTransparentRegionHint(region);
}

ECode CSurface::SetAlpha(
    /* [in] */ Float alpha)
{
    return NativeSetAlpha(alpha);
}

ECode CSurface::SetMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    return NativeSetMatrix(dsdx, dtdx, dsdy, dtdy);
}

ECode CSurface::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    return NativeSetFlags(flags, mask);
}

ECode CSurface::SetWindowCrop(
    /* [in] */ IRect* crop)
{
    return NativeSetWindowCrop(crop);
}

ECode CSurface::SetLayerStack(
    /* [in] */ Int32 layerStack)
{
    return NativeSetLayerStack(layerStack);
}

ECode CSurface::CopyFrom(
    /* [in] */ ISurface* other)
{
    if (other == NULL) {
        assert(0);
        //throw new IllegalArgumentException("other must not be null");
    }
    if (other != this) {
        NativeCopyFrom(other);
    }

    return NOERROR;
}

ECode CSurface::TransferFrom(
    /* [in] */ ISurface* other)
{
    if (other == NULL) {
        assert(0);
        //throw new IllegalArgumentException("other must not be null");
    }
    if (other != this) {
        NativeTransferFrom(other);
    }

    return NOERROR;
}

ECode CSurface::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    if (source == NULL) {
        Logger::E(TAG, "source must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    source->ReadString(&mName);
    NativeReadFromParcel(source);
    return NOERROR;
}

ECode CSurface::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (dest == NULL) {
        Logger::E(TAG, "dest must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    dest->WriteString(mName);
    NativeWriteToParcel(dest);
    return NOERROR;
}

ECode CSurface::GetSurface(
    /* [out] */ Handle32* surface)
{
    VALIDATE_NOT_NULL(surface);
    GetSurface();
    *surface = reinterpret_cast<Handle32>(mNativeSurface.get());
    return NOERROR;
}

void CSurface::GetSurface()
{
    if (mNativeSurface == NULL && mNativeSurfaceControl != NULL) {
        /*
         * if this method is called from the WindowManager's process, it means
         * the client is is not remote, and therefore is allowed to have
         * a Surface (data), so we create it here.
         * If we don't have a SurfaceControl, it means we're in a different
         * process.
         */
        mNativeSurface = mNativeSurfaceControl->getSurface();
    }
}

void CSurface::SetSurface(
        /* [in] */ const android::sp<android::Surface>& surface)
{
    if (mNativeSurface != NULL  && mNativeSurface != surface) {
        mGenerationId++;
    }
    mNativeSurface = surface;
}

ECode CSurface::GetCanvas(
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    *canvas = mCanvas;
    INTERFACE_ADDREF(*canvas);
    return NOERROR;
}

ECode CSurface::GetCanvasSaveCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mCanvasSaveCount;
    return NOERROR;
}

ECode CSurface::GetNativeSurfaceControl(
    /* [out] */ Handle32* surfaceControl)
{
    VALIDATE_NOT_NULL(surfaceControl);
    *surfaceControl = reinterpret_cast<Handle32>(mNativeSurfaceControl.get());
    return NOERROR;
}

ECode CSurface::SetSurface(
    /* [in] */ Handle32 surface)
{
    SetSurface((android::Surface*)surface);

    return NOERROR;
}


// @Override
// public String toString() {
//     return "Surface(name=" + mName + ", identity=" + nativeGetIdentity() + ")";
// }

ECode CSurface::CheckHeadless()
{
    if (HEADLESS) {
        Logger::E(TAG, "Device is headless");
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeCreate(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ const String& name,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 format,
    /* [in] */ Int32 flags)
{
    assert(session != NULL);
    android::sp<android::SurfaceComposerClient> client =
        ((CSurfaceSession*)session)->mNativeClient;

    assert(client != NULL);


    android::sp<android::SurfaceControl> surface = client->createSurface(
            android::String8((const char*) name), w, h, format, flags);

    if (surface == NULL) {;
        return E_OUT_OF_RESOURCES_EXCEPTION;
    }

    SetSurfaceControl(surface);

    return NOERROR;
}

ECode CSurface::NativeCreateFromSurfaceTexture(
    /* [in] */ ISurfaceTexture* surfaceTexture)
{
    android::sp<android::SurfaceTexture> st =
        (android::SurfaceTexture*)(((CSurfaceTexture*)surfaceTexture)->mSurfaceTexture);
    if (st == NULL) {
        Logger::E(TAG, "SurfaceTexture has already been released");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    android::sp<android::ISurfaceTexture> bq = st->getBufferQueue();

    android::sp<android::Surface> surface(new android::Surface(bq));
    if (surface == NULL) {
        assert(0);
        //jniThrowException(env, OutOfResourcesException, NULL);
        return NOERROR;
    }

    SetSurface(surface);

    return NOERROR;
}

ECode CSurface::NativeRelease()
{
    SetSurfaceControl(NULL);
    SetSurface(NULL);
    return NOERROR;
}

ECode CSurface::NativeDestroy()
{
    if (android::SurfaceControl::isValid(mNativeSurfaceControl)) {
        mNativeSurfaceControl->clear();
    }

    SetSurfaceControl(NULL);
    SetSurface(NULL);

    return NOERROR;
}

Boolean CSurface::NativeIsValid()
{
    if (mNativeSurfaceControl != NULL) {
        return android::SurfaceControl::isValid(mNativeSurfaceControl) ? TRUE : FALSE;
    }

    GetSurface();

    return android::Surface::isValid(mNativeSurface) ? TRUE : FALSE;
}

Int32 CSurface::NativeGetIdentity()
{
    if (mNativeSurfaceControl != NULL) {
        return mNativeSurfaceControl->getIdentity();
    }

    GetSurface();

    if (mNativeSurface != NULL) {
        return mNativeSurface->getIdentity();
    }

    return -1;
}

Boolean CSurface::NativeIsConsumerRunningBehind()
{
    GetSurface();

    if (!android::Surface::isValid(mNativeSurface)) {
        //doThrowIAE(env);
        assert(0);
        return FALSE;
    }

    int value = 0;

    ANativeWindow* anw = static_cast<ANativeWindow*>(mNativeSurface.get());
    anw->query(anw, NATIVE_WINDOW_CONSUMER_RUNNING_BEHIND, &value);
    return value;
}

ECode CSurface::NativeLockCanvas(
    /* [in] */ IRect* dirtyRectObj,
    /* [out] */ ICanvas** canvas)
{
    GetSurface();

    if (!android::Surface::isValid(mNativeSurface)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // get dirty region
    android::Region dirtyRegion;
    if (dirtyRectObj) {
        android::Rect dirty;
        dirtyRectObj->GetLeft(&dirty.left);
        dirtyRectObj->GetTop(&dirty.top);
        dirtyRectObj->GetRight(&dirty.right);
        dirtyRectObj->GetBottom(&dirty.bottom);
        if (!dirty.isEmpty()) {
            dirtyRegion.set(dirty);
        }
    } else {
        dirtyRegion.set(android::Rect(0x3FFF, 0x3FFF));
    }

    android::Surface::SurfaceInfo info;
    android::status_t err = mNativeSurface->lock(&info, &dirtyRegion);
    if (err < 0) {
        if (err == android::NO_MEMORY) {
            return E_OUT_OF_RESOURCES_EXCEPTION;
        }
        else {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    // Associate a SkCanvas object to this surface
    mCanvas->SetSurfaceFormat(info.format);

    Handle32 native;
    mCanvas->GetNativeCanvas(&native);
    SkCanvas* nativeCanvas = (SkCanvas*)native;
    SkBitmap bitmap;
    ssize_t bpr = info.s * android::bytesPerPixel(info.format);
    bitmap.setConfig(convertPixelFormat(info.format), info.w, info.h, bpr);
    if (info.format == android::PIXEL_FORMAT_RGBX_8888) {
        bitmap.setIsOpaque(true);
    }

    if (info.w > 0 && info.h > 0) {
        bitmap.setPixels(info.bits);
    } else {
        // be safe with an empty bitmap.
        bitmap.setPixels(NULL);
    }

    nativeCanvas->setBitmapDevice(bitmap);

    SkRegion clipReg;
    if (dirtyRegion.isRect()) { // very common case
        const android::Rect b(dirtyRegion.getBounds());
        clipReg.setRect(b.left, b.top, b.right, b.bottom);
    } else {
        size_t count;
        android::Rect const* r = dirtyRegion.getArray(&count);
        while (count) {
            clipReg.op(r->left, r->top, r->right, r->bottom, SkRegion::kUnion_Op);
            r++, count--;
        }
    }

    nativeCanvas->clipRegion(clipReg);

    mCanvasSaveCount = nativeCanvas->save();

    if (dirtyRectObj) {
        const android::Rect& bounds(dirtyRegion.getBounds());
        dirtyRectObj->Set(bounds.left, bounds.top, bounds.right, bounds.bottom);
    }

    *canvas = mCanvas;
    INTERFACE_ADDREF(*canvas);

    return NOERROR;
}

ECode CSurface::NativeUnlockCanvasAndPost(
    /* [in] */ ICanvas* canvasObj)
{
    if (mCanvas.Get() != canvasObj) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    GetSurface();

    if (!android::Surface::isValid(mNativeSurface)) {
        return NOERROR;
    }

    // detach the canvas from the surface
    Handle32 native;
    canvasObj->GetNativeCanvas(&native);
    SkCanvas* nativeCanvas = (SkCanvas*)native;
    int saveCount = mCanvasSaveCount;
    nativeCanvas->restoreToCount(saveCount);
    nativeCanvas->setBitmapDevice(SkBitmap());
    mCanvasSaveCount = 0;

    // unlock surface
    android::status_t err = mNativeSurface->unlockAndPost();
    if (err < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeOpenTransaction()
{
    android::SurfaceComposerClient::openGlobalTransaction();
    return NOERROR;
}

ECode CSurface::NativeCloseTransaction()
{
    android::SurfaceComposerClient::closeGlobalTransaction();
    return NOERROR;
}

ECode CSurface::NativeSetLayer(
    /* [in] */ Int32 zorder)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setLayer(zorder);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CSurface::NativeSetPosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setPosition(x, y);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setSize(w, h);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetTransparentRegionHint(
    /* [in] */ IRegion* regionObj)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    SkRegion* region = (SkRegion*)((CRegion*)regionObj)->mNativeRegion;
    assert(region != NULL);

    const SkIRect& b(region->getBounds());
    android::Region reg(android::Rect(b.fLeft, b.fTop, b.fRight, b.fBottom));
    if (region->isComplex()) {
        SkRegion::Iterator it(*region);
        while (!it.done()) {
            const SkIRect& r(it.rect());
            reg.addRectUnchecked(r.fLeft, r.fTop, r.fRight, r.fBottom);
            it.next();
        }
    }

    android::status_t err = mNativeSurfaceControl->setTransparentRegionHint(reg);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetAlpha(
    /* [in] */ Float alpha)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setAlpha(alpha);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetMatrix(
    /* [in] */ Float dsdx,
    /* [in] */ Float dtdx,
    /* [in] */ Float dsdy,
    /* [in] */ Float dtdy)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setMatrix(dsdx, dtdx, dsdy, dtdy);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setFlags(flags, mask);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetWindowCrop(
    /* [in] */ IRect* cropObj)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::Rect crop;
    if (cropObj) {
        cropObj->GetLeft(&crop.left);
        cropObj->GetTop(&crop.top);
        cropObj->GetRight(&crop.right);
        cropObj->GetBottom(&crop.bottom);
    } else {
        crop.left = crop.top = crop.right = crop.bottom = 0;
    }

    android::status_t err = mNativeSurfaceControl->setCrop(crop);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeSetLayerStack(
    /* [in] */ Int32 layerStack)
{
    if (mNativeSurfaceControl == NULL) return NOERROR;

    android::status_t err = mNativeSurfaceControl->setLayerStack(layerStack);
    if (err < 0 && err != android::NO_INIT) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CSurface::NativeCopyFrom(
    /* [in] */ ISurface* otherObj)
{
    /*
     * This is used by the WindowManagerService just after constructing
     * a Surface and is necessary for returning the Surface reference to
     * the caller. At this point, we should only have a SurfaceControl.
     */
    android::sp<android::SurfaceControl> other = ((CSurface*)otherObj)->mNativeSurfaceControl;

    if (!android::SurfaceControl::isSameSurface(mNativeSurfaceControl, other)) {
        // we reassign the surface only if it's a different one
        // otherwise we would loose our client-side state.
        SetSurfaceControl(other);
    }

    return NOERROR;
}

ECode CSurface::NativeTransferFrom(
    /* [in] */ ISurface* otherObj)
{
    SetSurfaceControl(((CSurface*)otherObj)->mNativeSurfaceControl);
    SetSurface(((CSurface*)otherObj)->mNativeSurface);
    ((CSurface*)otherObj)->mNativeSurfaceControl = NULL;
    ((CSurface*)otherObj)->mNativeSurface = NULL;

    return NOERROR;
}

ECode CSurface::NativeReadFromParcel(
    /* [in] */ IParcel* source)
{
    Handle32 data;
    source->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;

    if (parcel == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    android::sp<android::Surface> surface(android::Surface::readFromParcel(*parcel));
    SetSurfaceControl(NULL);
    SetSurface(surface);
    return NOERROR;
}

ECode CSurface::NativeWriteToParcel(
    /* [in] */ IParcel* dest)
{
    Handle32 data;
    dest->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;

    if (parcel == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    // The Java instance may have a SurfaceControl (in the case of the
    // WindowManager or a system app). In that case, we defer to the
    // SurfaceControl to send its ISurface. Otherwise, if the Surface is
    // available we let it parcel itself. Finally, if the Surface is also
    // NULL we fall back to using the SurfaceControl path which sends an
    // empty surface; this matches legacy behavior.
    if (mNativeSurfaceControl != NULL) {
        android::SurfaceControl::writeSurfaceToParcel(mNativeSurfaceControl, parcel);
    }
    else {
        if (mNativeSurface != NULL) {
            android::Surface::writeToParcel(mNativeSurface, parcel);
        }
        else {
            android::SurfaceControl::writeSurfaceToParcel(NULL, parcel);
        }
    }
//    if ((flags & Parcelable.PARCELABLE_WRITE_RETURN_VALUE) != 0) {
        NativeRelease();
//    }
    return NOERROR;
}

void CSurface::SetSurfaceControl(
        /* [in] */ const android::sp<android::SurfaceControl>& surface)
{
    mNativeSurfaceControl = surface;
}

Boolean CSurface::GetResult()
{
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    String result;
    sp->GetEx(String("ro.config.headless"), String("0"), &result);
    return CString("1").Equals(result);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
