
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

static const String AwPdfExporter::TAG("AwPdfExporter");

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

void AwPdfExporter::ExportToPdf(
    /* [in] */ const IParcelFileDescriptor* fd,
    /* [in] */ IPrintAttributes* attributes,
    /* [in] */ IValueCallback* resultCallback,
    /* [in] */ ICancellationSignal* cancellationSignal)
{
    if (fd == NULL) {
//        throw new IllegalArgumentException("fd cannot be null");
        assert(0);
    }

    if (resultCallback == NULL) {
//        throw new IllegalArgumentException("resultCallback cannot be null");
        assert(0);
    }

    if (mResultCallback != NULL) {
//        throw new IllegalStateException("printing is already pending");
        assert(0);
    }

    AutoPtr<IMediaSize> mediaSize;
    if (attributes->GetMediaSize((IMediaSize**)&mediaSize), mediaSize == NULL) {
//        throw new  IllegalArgumentException("attributes must specify a media size");
        assert(0);
    }

    AutoPtr<IResolution> resolution;
    if (attributes->GetResolution((IResolution**)&resolution), resolution == NULL) {
//        throw new IllegalArgumentException("attributes must specify print resolution");
        assert(0);
    }

    AutoPtr<IMargins> margins;
    if (attributes->GetMinMargins((IMargins**)&margins), margins == NULL) {
//        throw new IllegalArgumentException("attributes must specify margins");
        assert(0);
    }

    if (mNativeAwPdfExporter == 0) {
        resultCallback->OnReceiveValue(FALSE);
        return;
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
        mResultCallback->OnReceiveValue(FALSE);
        mResultCallback = NULL;
    }
}

Int32 AwPdfExporter::GetPrintDpi(
    /* [in] */ IPrintAttributes* attributes)
{
    // TODO(sgurun) android print attributes support horizontal and
    // vertical DPI. Chrome has only one DPI. Revisit this.
    AutoPtr<IResolution> resolution;
    attributes->GetResolution((IResolution**)&resolution);
    Int32 horizontalDpi;
    resolution->GetHorizontalDpi(&horizontalDpi);
    Int32 verticalDpi;
    resolution->GetVerticalDpi(&verticalDpi);
    if (horizontalDpi != verticalDpi) {
//        Log.w(TAG, "Horizontal and vertical DPIs differ. Using horizontal DPI " +
//                " hDpi=" + horizontalDpi + " vDPI=" + verticalDpi);
    }
    return horizontalDpi;
}

//@CalledByNative
void AwPdfExporter::didExportPdf(
    /* [in] */ Boolean success)
{
    mResultCallback->OnReceiveValue(success);
    mResultCallback = NULL;
    mAttributes = NULL;
    // The caller should close the file.
    mFd = NULL;
}

//@CalledByNative
Int32 AwPdfExporter::GetPageWidth()
{
    AutoPtr<IMediaSize> mediaSize;
    mAttributes->GetMediaSize((IMediaSize**)&mediaSize);
    Int32 mils;
    mediaSize->GetWidthMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetPageHeight()
{
    AutoPtr<IMediaSize> mediaSize;
    mAttributes->GetMediaSize((IMediaSize**)&mediaSize);
    Int32 mils;
    mediaSize->GetHeightMils(&mils);
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
    AutoPtr<IMargins> margins;
    mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils;
    margins->GetLeftMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetRightMargin()
{
    AutoPtr<IMargins> margins;
    mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils;
    margins->GetRightMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetTopMargin()
{
    AutoPtr<IMargins> margins;
    mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils;
    margins->GetTopMils(&mils);
    return mils;
}

//@CalledByNative
Int32 AwPdfExporter::GetBottomMargin()
{
    AutoPtr<IMargins> margins;
    mAttributes->GetMinMargins((IMargins**)&margins);
    Int32 mils;
    margins->GetBottomMils(&mils);
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
