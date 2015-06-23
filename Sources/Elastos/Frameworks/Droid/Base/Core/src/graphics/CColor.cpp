
#include "graphics/CColor.h"
#include <elastos/core/Math.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkColor.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Graphics {

static AutoPtr<HashMap<String, Int32> > InitColorNameMap()
{
    AutoPtr<HashMap<String, Int32> > map = new HashMap<String, Int32>(11);
    assert(map != NULL);
    (*map)[String("black")] = IColor::BLACK;
    (*map)[String("darkgray")] = IColor::DKGRAY;
    (*map)[String("gray")] = IColor::GRAY;
    (*map)[String("lightgray")] = IColor::LTGRAY;
    (*map)[String("white")] = IColor::WHITE;
    (*map)[String("red")] = IColor::RED;
    (*map)[String("green")] = IColor::GREEN;
    (*map)[String("blue")] = IColor::BLUE;
    (*map)[String("yellow")] = IColor::YELLOW;
    (*map)[String("cyan")] = IColor::CYAN;
    (*map)[String("magenta")] = IColor::MAGENTA;
    return map;
}

AutoPtr<HashMap<String, Int32> > CColor::sColorNameMap = InitColorNameMap();

ECode CColor::Alpha(
    /* [in] */ Int32 color,
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);

    *alpha = (unsigned Int32)color >> 24;
    return NOERROR;
}

ECode CColor::Red(
    /* [in] */ Int32 color,
    /* [out] */ Int32* red)
{
    VALIDATE_NOT_NULL(red);

    *red = (color >> 16) & 0xFF;
    return NOERROR;
}

ECode CColor::Green(
    /* [in] */ Int32 color,
    /* [out] */ Int32* green)
{
    VALIDATE_NOT_NULL(green);

    *green = (color >> 8) & 0xFF;
    return NOERROR;
}

ECode CColor::Blue(
    /* [in] */ Int32 color,
    /* [out] */ Int32* blue)
{
    VALIDATE_NOT_NULL(blue);

    *blue = color & 0xFF;
    return NOERROR;
}

ECode CColor::Rgb(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ Int32* rgb)
{
    VALIDATE_NOT_NULL(rgb);

    *rgb = (0xFF << 24) | (red << 16) | (green << 8) | blue;
    return NOERROR;
}

ECode CColor::Argb(
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ Int32* argb)
{
    VALIDATE_NOT_NULL(argb);

    *argb = (alpha << 24) | (red << 16) | (green << 8) | blue;
    return NOERROR;
}

ECode CColor::Hue(
    /* [in] */ Int32 color,
    /* [out] */ Float* hue)
{
    VALIDATE_NOT_NULL(hue);

    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;

    Int32 V = Elastos::Core::Math::Max(b, Elastos::Core::Math::Max(r, g));
    Int32 temp = Elastos::Core::Math::Min(b, Elastos::Core::Math::Min(r, g));

    Float H;

    if (V == temp) {
        H = 0;
    }
    else {
        const Float vtemp = (Float) (V - temp);
        const Float cr = (V - r) / vtemp;
        const Float cg = (V - g) / vtemp;
        const Float cb = (V - b) / vtemp;

        if (r == V) {
            H = cb - cg;
        }
        else if (g == V) {
            H = 2 + cr - cb;
        }
        else {
            H = 4 + cg - cr;
        }

        H /= 6.f;
        if (H < 0) {
            H++;
        }
    }

    *hue = H;
    return NOERROR;
}

ECode CColor::Saturation(
    /* [in] */ Int32 color,
    /* [out] */ Float* saturation)
{
    VALIDATE_NOT_NULL(saturation);

    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;


    Int32 V = Elastos::Core::Math::Max(b, Elastos::Core::Math::Max(r, g));
    Int32 temp = Elastos::Core::Math::Min(b, Elastos::Core::Math::Min(r, g));

    Float S;

    if (V == temp) {
        S = 0;
    }
    else {
        S = (V - temp) / (Float) V;
    }

    *saturation = S;
    return NOERROR;
}

ECode CColor::Brightness(
    /* [in] */ Int32 color,
    /* [out] */ Float* brightness)
{
    VALIDATE_NOT_NULL(brightness);

    Int32 r = (color >> 16) & 0xFF;
    Int32 g = (color >> 8) & 0xFF;
    Int32 b = color & 0xFF;

    Int32 V = Elastos::Core::Math::Max(b, Elastos::Core::Math::Max(r, g));

    *brightness = (V / 255.f);
    return NOERROR;
}

ECode CColor::ParseColor(
    /* [in] */ const String& colorString,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    if (colorString.GetChar(0) == '#') {
       // Use a long to avoid rollovers on #ffXXXXXX
        Int64 c = StringUtils::ParseInt64(colorString.Substring(1), 16);
        if (colorString.GetLength() == 7) {
            // Set the alpha value
            c |= 0x00000000ff000000;
        }
        else if (colorString.GetLength() != 9) {
            //throw new IllegalArgumentException("Unknown color");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        *color = (Int32)c;
        return NOERROR;
    }
    else {
        String cstr = colorString.ToLowerCase(/*Locale.US*/);
        HashMap<String, Int32>::Iterator it = sColorNameMap->Find(cstr);
        if (it != sColorNameMap->End()) {
            *color = it->mSecond;
            return NOERROR;
        }
    }
    // throw new IllegalArgumentException("Unknown color");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CColor::HSBtoColor(
    /* [in] */ const ArrayOf<Float>& hsb,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    return HSBtoColor(hsb[0], hsb[1], hsb[2], color);
}

ECode CColor::HSBtoColor(
    /* [in] */ Float h,
    /* [in] */ Float s,
    /* [in] */ Float b,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    h = Constrain(h, 0.0f, 1.0f);
    s = Constrain(s, 0.0f, 1.0f);
    b = Constrain(b, 0.0f, 1.0f);

    Float red = 0.0f;
    Float green = 0.0f;
    Float blue = 0.0f;

    Float hf = (h - (Int32) h) * 6.0f;
    Int32 ihf = (Int32) hf;
    Float f = hf - ihf;
    Float pv = b * (1.0f - s);
    Float qv = b * (1.0f - s * f);
    Float tv = b * (1.0f - s * (1.0f - f));

    switch (ihf) {
        case 0:         // Red is the dominant color
            red = b;
            green = tv;
            blue = pv;
            break;
        case 1:         // Green is the dominant color
            red = qv;
            green = b;
            blue = pv;
            break;
        case 2:
            red = pv;
            green = b;
            blue = tv;
            break;
        case 3:         // Blue is the dominant color
            red = pv;
            green = qv;
            blue = b;
            break;
        case 4:
            red = tv;
            green = pv;
            blue = b;
            break;
        case 5:         // Red is the dominant color
            red = b;
            green = pv;
            blue = qv;
            break;
    }

    *color = 0xFF000000 | (((Int32) (red * 255.0f)) << 16) |
            (((Int32) (green * 255.0f)) << 8) | ((Int32) (blue * 255.0f));
    return NOERROR;
}

ECode CColor::RGBToHSV(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ ArrayOf<Float>* hsv)
{
    VALIDATE_NOT_NULL(hsv);

    if (hsv->GetLength() < 3) {
        // throw new RuntimeException("3 components required for hsv");
        return E_RUNTIME_EXCEPTION;
    }

    NativeRGBToHSV(red, green, blue, hsv);
    return NOERROR;
}

ECode CColor::ColorToHSV(
    /* [in] */ Int32 color,
    /* [out] */ ArrayOf<Float>* hsv)
{
    VALIDATE_NOT_NULL(hsv);

    return RGBToHSV((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, hsv);
}

ECode CColor::HSVToColor(
    /* [in] */ const ArrayOf<Float>& hsv,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    return HSVToColor(0xFF, hsv, color);
}

ECode CColor::HSVToColor(
    /* [in] */ Int32 alpha,
    /* [in] */ const ArrayOf<Float>& hsv,
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);

    if (hsv.GetLength() < 3) {
        // throw new RuntimeException("3 components required for hsv");
        return E_RUNTIME_EXCEPTION;
    }
    *color = NativeHSVToColor(alpha, hsv);
    return NOERROR;
}

void CColor::NativeRGBToHSV(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [out] */ ArrayOf<Float>* hsvArray)
{
    SkScalar hsv[3];
    SkRGBToHSV(red, green, blue, hsv);

    SkASSERT(hsvArray);
    if (hsvArray->GetLength() < 3) {
        sk_throw();
    }

    Float* values = hsvArray->GetPayload();
    for (Int32 i = 0; i < 3; i++) {
        values[i] = SkScalarToFloat(hsv[i]);
    }
}

Int32 CColor::NativeHSVToColor(
    /* [in] */ Int32 alpha,
    /* [in] */ const ArrayOf<Float>& hsvArray)
{
    if (hsvArray.GetLength() < 3) {
        sk_throw();
        return 0;
    }

    Float* values = hsvArray.GetPayload();
    SkScalar hsv[3];

    for (Int32 i = 0; i < 3; i++) {
        hsv[i] = SkFloatToScalar(values[i]);
    }

    return SkHSVToColor(alpha, hsv);
}

Float CColor::Constrain(
    /* [in] */ Float amount,
    /* [in] */ Float low,
    /* [in] */ Float high)
{
    return amount < low ? low : (amount > high ? high : amount);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
