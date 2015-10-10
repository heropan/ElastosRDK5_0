#include "elastos/droid/webkit/native/android_webview/AwPdfExporter.h"

#include "elastos/utility/logging/Logger.h"

using Elastos::Utility::Logging::Logger;
//TODO using Elastos::Droid::Print::IMargins;
//TODO using Elastos::Droid::Print::IMediaSize;
//TODO using Elastos::Droid::Print::IResolution;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String AwPdfExporter::TAG("AwPdfExporter");

AwPdfExporter::AwPdfExporter(
    /* [in] */ IViewGroup* containerView)
    : mNativeAwPdfExporter(0)
{
    SetContainerView(containerView);
}

void AwPdfExporter::SetContainerView(
    /* [in] */ IViewGroup* containerView)
{
    mContainerView = containerView;
}

ECode AwPdfExporter::ExportToPdf(
    /* [in] */ IParcelFileDescriptor* fd,
    /* [in] */ /*TODO IPrintAttributes*/IInterface* attributes,
    /* [in] */ /*TODO IValueCallback*/IInterface* resultCallback,
    /* [in] */ ICancellationSignal* cancellationSignal)
{
    if (fd == NULL) {
        //throw new IllegalArgumentException("fd cannot be null");
        Logger::E(TAG, "fd cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (resultCallback == NULL) {
        //throw new IllegalArgumentException("resultCallback cannot be null");
        Logger::E(TAG, "resultCallback cannot be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mResultCallback != NULL) {
        //throw new IllegalStateException("printing is already pending");
        Logger::E(TAG, "printing is already pending");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //TODO AutoPtr<IMediaSize> mediaSize;
    //TODO if (attributes->GetMediaSize((IMediaSize**)&mediaSize), mediaSize == NULL)
    {
        //throw new  IllegalArgumentException("attributes must specify a media size");
        Logger::E(TAG, "attributes must specify a media size");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //TODO AutoPtr<IResolution> resolution;
    //TODO if (attributes->GetResolution((IResolution**)&resolution), resolution == NULL)
    {
        //throw new IllegalArgumentException("attributes must specify print resolution");
        Logger::E(TAG, "attributes must specify print resolution");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //TODO AutoPtr<IMargins> margins;
    //TODO if (attributes->GetMinMargins((IMargins**)&margins), margins == NULL)
    {
        //throw new IllegalArgumentException("attributes must specify margins");
        Logger::E(TAG, "attributes must specify margins");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mNativeAwPdfExporter == 0) {
        //TODO resultCallback->OnReceiveValue(FALSE);
        return NOERROR;
    }

    mResultCallback = resultCallback;
    mAttributes = attributes;
    mFd = fd;

    Int32 _fd;
    mFd->GetFd(&_fd);
    NativeExportToPdf(mNativeAwPdfExporter, _fd, cancellationSignal);
}

//@CalledByNative
void AwPdfExporter::SetNativeAwPdfExporter(
    /* [in] */ Int64 nativePdfExporter)
{
    mNativeAwPdfExporter = nativePdfExporter;
    // Handle the cornercase that Webview.Destroy is called before the native side
    // has a chance to complete the pdf exporting.
    if (nativePdfExporter == 0 && mResultCallback != NULL) {
        //TODO mResultCallback->OnReceiveValue(FALSE);
        mResultCallback = NULL;
    }
}

Int32 AwPdfExporter::GetPrintDpi(
    /* [in] */ /*TODO IPrintAttributes*/IInterface* attributes)
{
    // TODO(sgurun) android print attributes support horizontal and
    // vertical DPI. Chrome has only one DPI. Revisit this.
    //TODO AutoPtr<IResolution> resolution;
    //TODO attributes->GetResolution((IResolution**)&resolution);
    Int32 horizontalDpi = 0;
    //TODO resolution->GetHorizontalDpi(&horizontalDpi);
    Int32 verticalDpi = 0;
    //TODO resolution->GetVerticalDpi(&verticalDpi);
    if (horizontalDpi != verticalDpi) {
        Logger::W(TAG, "Horizontal and vertical DPIs differ. Using horizontal DPI \
                       hDpi=%d, vDPI=%d.", horizontalDpi, verticalDpi);
    }
    return horizontalDpi;
}

//@CalledByNative
void AwPdfExporter::DidExportPdf(
    /* [in] */ Boolean success)
{
    //TODO mResultCallback->OnReceiveValue(success);
    mResultCallback = NULL;
    mAttributes = NULL;
    // The caller should close the file.
    mFd = NULL;
}

//@CalledByNative
Int32 AwPdfExporter::GetPageWidth()
{
    //TODO AutoPtr<IMediaSize> mediaSize;
    //TODO mAttributes->GetMediaSize((IMediaSize**)&mediaSize);
    Int32 mils = 0;
    //TODO mediaSize->GetWidthMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetPageHeight()
{
    //TODO AutoPtr<IMediaSize> mediaSize;
    //TODO mAttributes->GetMediaSize((IMediaSize**)&mediaSize);
    Int32 mils = 0;
    //TODO mediaSize->GetHeightMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetDpi()
{
    return GetPrintDpi(mAttributes);
}

//@CalledByNative
Int32 AwPdfExporter::GetLeftMargin()
{
    //TODO AutoPtr<IMargins> margins;
    //TODO mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils = 0;
    //TODO margins->GetLeftMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetRightMargin()
{
    //TODO AutoPtr<IMargins> margins;
    //TODO mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils = 0;
    //TODO margins->GetRightMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetTopMargin()
{
    //TODO AutoPtr<IMargins> margins;
    //TODO mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils = 0;
    //TODO margins->GetTopMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetBottomMargin()
{
    //TODO AutoPtr<IMargins> margins;
    //TODO mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils = 0;
    //TODO margins->GetBottomMils(&mils);
    return mils;
}

void AwPdfExporter::NativeExportToPdf(
    /* [in] */ Int64 nativeAwPdfExporter,
    /* [in] */ Int32 fd,
    /* [in] */ ICancellationSignal* cancellationSignal)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
