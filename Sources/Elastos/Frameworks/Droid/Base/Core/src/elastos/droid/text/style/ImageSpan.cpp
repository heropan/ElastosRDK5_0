#include "elastos/droid/text/style/ImageSpan.h"
#include "elastos/droid/graphics/drawable/CBitmapDrawable.h"
#include "elastos/droid/graphics/CBitmapFactory.h"

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::Drawable::IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::CBitmapDrawable;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ImageSpan::ImageSpan()
{}

ImageSpan::ImageSpan(
    /* [in] */ IBitmap* b)
{
    Init(NULL, b, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    Init(NULL, b, verticalAlignment);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b)
{
    Init(context, b, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, b, verticalAlignment);
}

ImageSpan::ImageSpan(
    /* [in] */ IDrawable* d)
{
    Init(d, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IDrawable* d,
    /* [in] */ Int32 verticalAlignment)
{
    Init(d, verticalAlignment);
}

ImageSpan::ImageSpan(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source)
{
    Init(d, source, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source,
    /* [in] */ Int32 verticalAlignment)
{
    Init(d, source, verticalAlignment);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    Init(context, uri, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, uri, verticalAlignment);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId)
{
    Init(context, resourceId, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

ImageSpan::ImageSpan(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, resourceId, verticalAlignment);
}


void ImageSpan::Init(
    /* [in] */ IBitmap* b)
{
    Init(NULL, b, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    Init(NULL, b, verticalAlignment);
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b)
{
    Init(context, b, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    DynamicDrawableSpan::Init(verticalAlignment);
    mContext = context;
    if(context != NULL) {
        AutoPtr<IResources> rs;
        context->GetResources( (IResources**)&rs );
        CBitmapDrawable::New(rs, b, (IBitmapDrawable**)&mDrawable);
    }
    else {
        CBitmapDrawable::New((IBitmapDrawable**)&mDrawable);
    }
    Int32 width;
    mDrawable->GetIntrinsicWidth(&width);
    Int32 height;
    mDrawable->GetIntrinsicHeight(&height);
    mDrawable->SetBounds(0, 0, width > 0 ? width : 0, height > 0 ? height : 0);
}

void ImageSpan::Init(
    /* [in] */ IDrawable* d)
{
    Init(d, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IDrawable* d,
    /* [in] */ Int32 verticalAlignment)
{

    DynamicDrawableSpan::Init(verticalAlignment);
    mDrawable = d;
}

void ImageSpan::Init(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source)
{
    Init(d, source, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source,
    /* [in] */ Int32 verticalAlignment)
{
    DynamicDrawableSpan::Init(verticalAlignment);
    mDrawable = d;
    mSource = source;
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    Init(context, uri, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 verticalAlignment)
{

    DynamicDrawableSpan::Init(verticalAlignment);
    mContext = context;
    mContentUri = uri;
    uri->ToString(&mSource);
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId)
{
    Init(context, resourceId, IDynamicDrawableSpan::ALIGN_BOTTOM);
}

void ImageSpan::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId,
    /* [in] */ Int32 verticalAlignment)
{

    DynamicDrawableSpan::Init(verticalAlignment);
    mContext = context;
    mResourceId = resourceId;
}


AutoPtr<IDrawable> ImageSpan::GetDrawable()
{
    AutoPtr<IDrawable> drawable;

    if (mDrawable != NULL) {
        drawable = mDrawable;
    }
    else  if (mContentUri != NULL) {
        AutoPtr<IBitmap> bitmap;
        //try {
            AutoPtr<IContentResolver> cr;
            mContext->GetContentResolver((IContentResolver**)&cr);
            AutoPtr<IInputStream> is;
            cr->OpenInputStream(mContentUri, (IInputStream**)&is);
            AutoPtr<IBitmapFactory> bf;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bf);
            bf->DecodeStream(is.Get(), (IBitmap**)&bitmap);
            AutoPtr<IResources> rs;
            mContext->GetResources((IResources**)&rs);
            CBitmapDrawable::New(rs, bitmap, (IBitmapDrawable**)&drawable);
            Int32 dIntrinsicWidth, dIntrinsicHeight;
            drawable->GetIntrinsicWidth(&dIntrinsicWidth);
            drawable->GetIntrinsicHeight(&dIntrinsicHeight);
            drawable->SetBounds(0, 0, dIntrinsicWidth, dIntrinsicHeight);
            is->Close();
        //} catch (Exception e) {
        //    Log.e("sms", "Failed to loaded content " + mContentUri, e);
        //}
    }
    else {
        //try {
            mContext->GetDrawable(mResourceId, (IDrawable**)&drawable);
            Int32 dIntrinsicWidth, dIntrinsicHeight;
            drawable->GetIntrinsicWidth(&dIntrinsicWidth);
            drawable->GetIntrinsicHeight(&dIntrinsicHeight);
            drawable->SetBounds(0, 0, dIntrinsicWidth, dIntrinsicHeight);
        //} catch (Exception e) {
        //    Log.e("sms", "Unable to find resource: " + mResourceId);
        //}
    }

    return drawable;
}

String ImageSpan::GetSource()
{
    return mSource;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos
