
#include "elastos/droid/hardware/display/CDisplayManager.h"
#include "elastos/droid/hardware/display/DisplayManagerGlobal.h"
#include <elastos/core/AutoLock.h>

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

const char* CDisplayManager::TAG = "DisplayManager";
const Boolean CDisplayManager::DEBUG;

CAR_INTERFACE_IMPL(CDisplayManager, Object, IDisplayManager)

CAR_OBJECT_IMPL(CDisplayManager)

CDisplayManager::CDisplayManager()
{
}

CDisplayManager::~CDisplayManager()
{
}

ECode CDisplayManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    mGlobal = DisplayManagerGlobal::GetInstance();

    return NOERROR;
}

ECode CDisplayManager::GetDisplay(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplay** display)
{
    VALIDATE_NOT_NULL(display);

    synchronized(mLock) {
        AutoPtr<IDisplay> temp = GetOrCreateDisplayLocked(displayId, FALSE /*assumeValid*/);
        *display = temp;
        REFCOUNT_ADD(*display);
    }

    return NOERROR;
}

ECode CDisplayManager::GetDisplays(
    /* [out] */ ArrayOf<IDisplay*>** displays)
{
    return GetDisplays(String(NULL), displays);
}

ECode CDisplayManager::GetDisplays(
    /* [in] */ const String& category,
    /* [out] */ ArrayOf<IDisplay*>** displays)
{
    VALIDATE_NOT_NULL(displays);

    AutoPtr<ArrayOf<Int32> > displayIds;
    mGlobal->GetDisplayIds((ArrayOf<Int32>**)&displayIds);

    synchronized(mLock) {
        //try {
            if (category.IsNull()) {
                AddAllDisplaysLocked(mTempDisplays, displayIds);
            }
            else if (category.Equals(DISPLAY_CATEGORY_PRESENTATION)) {
                AddPresentationDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_WIFI);
                AddPresentationDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_HDMI);
                AddPresentationDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_OVERLAY);
                AddPresentationDisplaysLocked(mTempDisplays, displayIds, IDisplay::TYPE_VIRTUAL);
            }
            *displays = ArrayOf<IDisplay*>::Alloc(mTempDisplays.GetSize());
            if (*displays == NULL) {
                mTempDisplays.Clear();
                return E_OUT_OF_MEMORY_ERROR;
            }
            REFCOUNT_ADD(*displays);
            List<AutoPtr<IDisplay> >::Iterator iter = mTempDisplays.Begin();
            for (Int32 i = 0; iter != mTempDisplays.End(); ++iter, i++) {
                (*displays)->Set(i, *iter);
            }

        //} finally {
            mTempDisplays.Clear();
        //}
    }

    return NOERROR;
}

void CDisplayManager::AddAllDisplaysLocked(
    /* [in] */ List<AutoPtr<IDisplay> >& displays,
    /* [in] */ ArrayOf<Int32>* displayIds)
{
    for (Int32 i = 0; i < displayIds->GetLength(); i++) {
        AutoPtr<IDisplay> display = GetOrCreateDisplayLocked((*displayIds)[i], TRUE /*assumeValid*/);
        if (display != NULL) {
            displays.PushBack(display);
        }
    }
}

void CDisplayManager::AddPresentationDisplaysLocked(
    /* [in] */ List<AutoPtr<IDisplay> >& displays,
    /* [in] */ ArrayOf<Int32>* displayIds,
    /* [in] */ Int32 matchType)
{
    for (Int32 i = 0; i < displayIds->GetLength(); i++) {
        AutoPtr<IDisplay> display = GetOrCreateDisplayLocked((*displayIds)[i], TRUE /*assumeValid*/);
        if (display != NULL) {
            Int32 flag;
            display->GetFlags(&flag);
            if ((flag & IDisplay::FLAG_PRESENTATION) != 0) {
                Int32 type;
                display->GetType(&type);
                if (type == matchType) {
                    displays.PushBack(display);
                }
            }
        }
    }
}

AutoPtr<IDisplay> CDisplayManager::GetOrCreateDisplayLocked(
    /* [in] */ Int32 displayId,
    /* [in] */ Boolean assumeValid)
{
    AutoPtr<IDisplay> display;
    HashMap<Int32, AutoPtr<IDisplay> >::Iterator iter = mDisplays.Find(displayId);
    if(iter != mDisplays.End())
        display = iter->mSecond;
    Boolean isValid;
    if (display == NULL) {
        AutoPtr<IDisplayAdjustments> infoHolder;
        assert(0 && "TODO GetDisplayAdjustments");
        // mContext->GetDisplayAdjustments(
        //     displayId, (IDisplayAdjustments**)&infoHolder);
        mGlobal->GetCompatibleDisplay(displayId, infoHolder, (IDisplay**)&display);
        if (display != NULL) {
            mDisplays[displayId] = display;
        }
    }
    else if (!assumeValid && !(display->IsValid(&isValid), isValid)) {
        display = NULL;
    }

    return display;
}

ECode CDisplayManager::RegisterDisplayListener(
    /* [in] */ IDisplayListener* listener,
    /* [in] */ IHandler* handler)
{
    return mGlobal->RegisterDisplayListener(listener, handler);
}

ECode CDisplayManager::UnregisterDisplayListener(
    /* [in] */ IDisplayListener* listener)
{
    return mGlobal->UnregisterDisplayListener(listener);
}

ECode CDisplayManager::StartWifiDisplayScan()
{
    return mGlobal->StartWifiDisplayScan();
}

ECode CDisplayManager::StopWifiDisplayScan()
{
    return mGlobal->StopWifiDisplayScan();
}

ECode CDisplayManager::ConnectWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    return mGlobal->ConnectWifiDisplay(deviceAddress);
}

ECode CDisplayManager::PauseWifiDisplay()
{
    return mGlobal->PauseWifiDisplay();
}

ECode CDisplayManager::ResumeWifiDisplay()
{
    return mGlobal->ResumeWifiDisplay();
}

ECode CDisplayManager::DisconnectWifiDisplay()
{
    return mGlobal->DisconnectWifiDisplay();
}

ECode CDisplayManager::RenameWifiDisplay(
    /* [in] */ const String& deviceAddress,
    /* [in] */ const String& alias)
{
    return mGlobal->RenameWifiDisplay(deviceAddress, alias);
}

ECode CDisplayManager::ForgetWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    return mGlobal->ForgetWifiDisplay(deviceAddress);
}

ECode CDisplayManager::GetWifiDisplayStatus(
    /* [out] */ IWifiDisplayStatus** status)
{
    VALIDATE_NOT_NULL(status);

    return mGlobal->GetWifiDisplayStatus(status);
}

ECode CDisplayManager::CreateVirtualDisplay(
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags,
    /* [out] */ IVirtualDisplay** result)
{
    VALIDATE_NOT_NULL(result);

    return CreateVirtualDisplay(name, width, height, densityDpi, surface, flags, NULL, NULL, result);
}

ECode CDisplayManager::CreateVirtualDisplay(
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags,
    /* [in] */ IVirtualDisplayCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IVirtualDisplay** result)
{
    VALIDATE_NOT_NULL(result);

    return CreateVirtualDisplay(NULL, name, width, height, densityDpi, surface, flags, cb, handler, result);
}

ECode CDisplayManager::CreateVirtualDisplay(
    /* [in] */ IMediaProjection* projection,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurface* surface,
    /* [in] */ Int32 flags,
    /* [in] */ IVirtualDisplayCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IVirtualDisplay** result)
{
    VALIDATE_NOT_NULL(result);

    return mGlobal->CreateVirtualDisplay(mContext, projection, name, width, height, densityDpi, surface, flags, cb, handler, result);
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
