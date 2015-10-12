#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/text/style/BulletSpan.h"
#include "elastos/droid/graphics/CPath.h"

using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::PaintStyle;
using Elastos::Droid::Graphics::PaintStyle_FILL;
using Elastos::Droid::Graphics::PathDirection_CW;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

const Int32 BulletSpan::BULLET_RADIUS = 3;
AutoPtr<IPath> BulletSpan::sBulletPath = NULL;

BulletSpan::BulletSpan()
{
    Init();
}

BulletSpan::BulletSpan(
    /* [in] */ Int32 gapWidth)
{
    Init(gapWidth);
}

BulletSpan::BulletSpan(
    /* [in] */ Int32 gapWidth,
    /* [in] */ Int32 color)
{
    Init(gapWidth, color);
}

BulletSpan::BulletSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void BulletSpan::Init()
{
    mGapWidth = IBulletSpan::STANDARD_GAP_WIDTH;
    mWantColor = FALSE;
    mColor = 0;
}

void BulletSpan::Init(
        /* [in] */ Int32 gapWidth)
{
    mGapWidth = gapWidth;
    mWantColor = FALSE;
    mColor = 0;
}

void BulletSpan::Init(
        /* [in] */ Int32 gapWidth,
        /* [in] */ Int32 color)
{
    mGapWidth = gapWidth;
    mWantColor = TRUE;
    mColor = color;
}

void BulletSpan::Init(
        /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 BulletSpan::GetSpanTypeId()
{
    return ITextUtils::BULLET_SPAN;
}

Int32 BulletSpan::DescribeContents()
{
    return 0;
}

ECode BulletSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mGapWidth));
    FAIL_RETURN(source->ReadBoolean(&mWantColor));
    FAIL_RETURN(source->ReadInt32(&mColor));
    return NOERROR;
}

ECode BulletSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(mGapWidth));
    FAIL_RETURN(dest->WriteBoolean(mWantColor));
    FAIL_RETURN(dest->WriteInt32(mColor));
    return NOERROR;
}

Int32 BulletSpan::GetLeadingMargin(
    /* [in] */ Boolean first)
{
    return 2 * BULLET_RADIUS + mGapWidth;
}

void BulletSpan::DrawLeadingMargin(
    /* [in] */ ICanvas* c,
    /* [in] */ IPaint* p,
    /* [in] */ Int32 x,
    /* [in] */ Int32 dir,
    /* [in] */ Int32 top,
    /* [in] */ Int32 baseline,
    /* [in] */ Int32 bottom,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean first,
    /* [in] */ ILayout* l)
{
    Int32 spanStart;
    if ((((ISpanned*) text)->GetSpanStart((IInterface*)this, &spanStart), spanStart) == start) {
        PaintStyle style;
        p->GetStyle(&style);
        Int32 oldcolor = 0;

        if (mWantColor) {
            p->GetColor(&oldcolor);
            p->SetColor(mColor);
        }

        p->SetStyle(PaintStyle_FILL);

        Boolean bIsHardwareAccelerated;
        if ((c->IsHardwareAccelerated(&bIsHardwareAccelerated), bIsHardwareAccelerated))
        {
            if (sBulletPath == NULL) {
                CPath::New( (IPath**)&sBulletPath );    //Java:    sBulletPath = new Path();

                // Bullet is slightly better to avoid aliasing artifacts on mdpi devices.
                sBulletPath->AddCircle(0.0f, 0.0f, 1.2f * BULLET_RADIUS, PathDirection_CW);
            }

            Int32 saveRet;
            c->Save(&saveRet);
            c->Translate(x + dir * BULLET_RADIUS, (top + bottom) / 2.0f);
            c->DrawPath(sBulletPath, p);
            c->Restore();
        } else {
            c->DrawCircle(x + dir * BULLET_RADIUS, (top + bottom) / 2.0f, BULLET_RADIUS, p);
        }

        if (mWantColor) {
            p->SetColor(oldcolor);
        }

        p->SetStyle(style);
    }
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos