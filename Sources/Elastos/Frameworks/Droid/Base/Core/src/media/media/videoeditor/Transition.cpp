#include "Transition.h"
#include "MediaItem.h"
#include <elastos/core/Math.h>

using Elastos::IO::IFile;
using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

const Int32 Transition::BEHAVIOR_MIN_VALUE = 0;
const Int32 Transition::BEHAVIOR_MAX_VALUE = 4;

//8abcb25e-fd2e-4a26-b510-665b5fa8078c
extern "C" const InterfaceID EIID_Transition =
        { 0x8abcb25e, 0xfd2e, 0x4a26, { 0xb5, 0x10, 0x66, 0x5b, 0x5f, 0xa8, 0x07, 0x8c } };

/**
 * An object of this type cannot be instantiated by using the default
 * constructor
 */
// @SuppressWarnings("unused")
 Transition::Transition()
    : mUniqueId(NULL)
    , mBehavior(0)
    , mDurationMs(0)
    , mFilename(NULL)
{
    Init(String(NULL), NULL, NULL, 0, 0);
}

/**
 * Constructor
 *
 * @param transitionId The transition id
 * @param afterMediaItem The transition is applied to the end of this
 *      media item
 * @param beforeMediaItem The transition is applied to the beginning of
 *      this media item
 * @param durationMs The duration of the transition in milliseconds
 * @param behavior The transition behavior
 */
 Transition::Transition(
    /* [in] */ const String& transitionId,
    /* [in] */ AutoPtr<IMediaItem> afterMediaItem,
    /* [in] */ AutoPtr<IMediaItem> beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior)
    : mUniqueId(NULL)
    , mBehavior(0)
    , mDurationMs(0)
    , mFilename(NULL)
{
    Init(transitionId, afterMediaItem, beforeMediaItem, durationMs, behavior);
}

ECode Transition::Init(
    /* [in] */ const String& transitionId,
    /* [in] */ AutoPtr<IMediaItem> afterMediaItem,
    /* [in] */ AutoPtr<IMediaItem> beforeMediaItem,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 behavior)
{
    if (behavior < BEHAVIOR_MIN_VALUE || behavior > BEHAVIOR_MAX_VALUE) {
        //throw new IllegalArgumentException("Invalid behavior: " + behavior);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if ((afterMediaItem == NULL) && (beforeMediaItem == NULL)) {
        //throw new IllegalArgumentException("NULL media items");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mUniqueId = transitionId;
    mAfterMediaItem = afterMediaItem;
    mBeforeMediaItem = beforeMediaItem;
    mDurationMs = durationMs;
    mBehavior = behavior;
    mNativeHelper = NULL;
    Int64 val;
    GetMaximumDuration(&val);
    if (durationMs > val) {
        //throw new IllegalArgumentException("The duration is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (afterMediaItem != NULL) {
        ((MediaItem*)afterMediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&mNativeHelper);
    }
    else {
        ((MediaItem*)beforeMediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&mNativeHelper);
    }
    return NOERROR;
}

ECode Transition::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUniqueId;
    return NOERROR;
}

ECode Transition::GetAfterMediaItem(
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAfterMediaItem;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode Transition::GetBeforeMediaItem(
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBeforeMediaItem;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode Transition::SetDuration(
    /* [in] */ Int64 durationMs)
{
    Int64 duration;
    GetMaximumDuration(&duration);
    if (durationMs > duration) {
        // throw new IllegalArgumentException("The duration is too large");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDurationMs = durationMs;
    Invalidate();
    mNativeHelper->SetGeneratePreview(TRUE);
    return NOERROR;
}

ECode Transition::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode Transition::GetMaximumDuration(
    /* [out] */ Int64* result)
{
    Int64 bDuration, aDuration;
    mBeforeMediaItem->GetTimelineDuration(&bDuration);
    mAfterMediaItem->GetTimelineDuration(&aDuration);
    if (mAfterMediaItem == NULL) {
        *result = bDuration / 2;
        return NOERROR;
    }
    else if (mBeforeMediaItem == NULL) {
        *result = aDuration / 2;
        return NOERROR;
    }
    else {
        *result = (Elastos::Core::Math::Min(aDuration,
                bDuration) / 2);
        return NOERROR;
    }
}

ECode Transition::GetBehavior(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBehavior;
    return NOERROR;
}

ECode Transition::GetTransitionSettings(
    /* [out] */ MediaArtistNativeHelper::TransitionSettings** settings)
{
    AutoPtr<ITransitionAlpha> transitionAlpha;
    AutoPtr<ITransitionSliding> transitionSliding;
    AutoPtr<ITransitionCrossfade> transitionCrossfade;
    AutoPtr<ITransitionFadeBlack> transitionFadeBlack;
    AutoPtr<MediaArtistNativeHelper::TransitionSettings> transitionSetting;
    transitionSetting = new MediaArtistNativeHelper::TransitionSettings();
    Int64 duration;
    GetDuration(&duration);
    transitionSetting->duration = (Int32)duration;
    transitionAlpha = THIS_PROBE(ITransitionAlpha);
    transitionSliding = THIS_PROBE(ITransitionSliding);
    transitionCrossfade = THIS_PROBE(ITransitionCrossfade);
    transitionFadeBlack = THIS_PROBE(ITransitionFadeBlack);
    if (transitionAlpha != NULL) {
        transitionSetting->videoTransitionType = IMediaArtistNativeHelperVideoTransition::ALPHA_MAGIC;
        transitionSetting->audioTransitionType = IMediaArtistNativeHelperAudioTransition::CROSS_FADE;
        Int32 behavior;
        transitionAlpha->GetBehavior(&behavior);
        mNativeHelper->GetVideoTransitionBehaviour(behavior, &transitionSetting->transitionBehaviour);
        transitionSetting->alphaSettings = new MediaArtistNativeHelper::AlphaMagicSettings();
        transitionSetting->slideSettings = NULL;
        transitionAlpha->GetPNGMaskFilename(&transitionSetting->alphaSettings->file);
        transitionAlpha->GetBlendingPercent(&transitionSetting->alphaSettings->blendingPercent);
        transitionAlpha->IsInvert(&transitionSetting->alphaSettings->invertRotation);
        transitionAlpha->GetRGBFileWidth(&transitionSetting->alphaSettings->rgbWidth);
        transitionAlpha->GetRGBFileHeight(&transitionSetting->alphaSettings->rgbHeight);

    }
    else if (transitionSliding != NULL) {
        transitionSetting->videoTransitionType = IMediaArtistNativeHelperVideoTransition::SLIDE_TRANSITION;
        transitionSetting->audioTransitionType = IMediaArtistNativeHelperAudioTransition::CROSS_FADE;
        Int32 behavior;
        transitionSliding->GetBehavior(&behavior);
        mNativeHelper->GetVideoTransitionBehaviour(behavior, &transitionSetting->transitionBehaviour);
        transitionSetting->alphaSettings = NULL;
        transitionSetting->slideSettings = new MediaArtistNativeHelper::SlideTransitionSettings();
        Int32 direction;
        transitionSliding->GetDirection(&direction);
        mNativeHelper->GetSlideSettingsDirection(direction, &transitionSetting->slideSettings->direction);
    }
    else if (transitionCrossfade != NULL) {
        transitionSetting->videoTransitionType = IMediaArtistNativeHelperVideoTransition::CROSS_FADE;
        transitionSetting->audioTransitionType = IMediaArtistNativeHelperAudioTransition::CROSS_FADE;
        Int32 behavior;
        transitionCrossfade->GetBehavior(&behavior);
        mNativeHelper->GetVideoTransitionBehaviour(behavior, &transitionSetting->transitionBehaviour);
        transitionSetting->alphaSettings = NULL;
        transitionSetting->slideSettings = NULL;
    }
    else if (transitionFadeBlack != NULL) {
        transitionSetting->videoTransitionType = IMediaArtistNativeHelperVideoTransition::FADE_BLACK;
        transitionSetting->audioTransitionType = IMediaArtistNativeHelperAudioTransition::CROSS_FADE;
        Int32 behavior;
        transitionFadeBlack->GetBehavior(&behavior);
        mNativeHelper->GetVideoTransitionBehaviour(behavior, &transitionSetting->transitionBehaviour);
        transitionSetting->alphaSettings = NULL;
        transitionSetting->slideSettings = NULL;
    }

    *settings = transitionSetting;
    REFCOUNT_ADD(*settings);
    return NOERROR;
}

AutoPtr<List<AutoPtr<MediaArtistNativeHelper::EffectSettings> > > Transition::IsEffectandOverlayOverlapping(
    /* [in] */ IMediaItem* m,
    /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings,
    /* [in] */ Int32 clipNo)
{
    AutoPtr<ArrayOf<IEffect*> > effects;
    AutoPtr<ArrayOf<IOverlay*> > overlays;
    AutoPtr<List<AutoPtr<MediaArtistNativeHelper::EffectSettings> > > effectSettings = new List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >();
    AutoPtr<MediaArtistNativeHelper::EffectSettings> tmpEffectSettings;

    m->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
    for(Int32 i = 0; i < overlays->GetLength(); i++) {
        AutoPtr<IOverlayFrame> frame = IOverlayFrame::Probe((*overlays)[i]);
        mNativeHelper->GetOverlaySettings(frame, (MediaArtistNativeHelper::EffectSettings**)&tmpEffectSettings);
        mNativeHelper->AdjustEffectsStartTimeAndDuration(tmpEffectSettings,
                clipSettings->beginCutTime, clipSettings->endCutTime);
        if (tmpEffectSettings->duration != 0) {
            effectSettings->PushBack(tmpEffectSettings);
        }
    }

    m->GetAllEffects((ArrayOf<IEffect*>**)&effects);
    for(Int32 i = 0; i < overlays->GetLength(); i++) {
        AutoPtr<IEffectColor> color = IEffectColor::Probe((*effects)[i]);
        if (color != NULL) {
            mNativeHelper->GetEffectSettings(color, (MediaArtistNativeHelper::EffectSettings**)&tmpEffectSettings);
            mNativeHelper->AdjustEffectsStartTimeAndDuration(tmpEffectSettings,
                    clipSettings->beginCutTime, clipSettings->endCutTime);
            if (tmpEffectSettings->duration != 0) {
                AutoPtr<IMediaVideoItem> item = IMediaVideoItem::Probe(m);
                if (item != NULL) {
                    Int32 fps;
                    item->GetFps(&fps);
                    mNativeHelper->GetClosestVideoFrameRate(fps, &tmpEffectSettings->fiftiesFrameRate);
                }
                effectSettings->PushBack(tmpEffectSettings);
            }
        }
    }

    return effectSettings;
}

ECode Transition::Generate()
{
    AutoPtr<IMediaItem> m1;
    GetAfterMediaItem((IMediaItem**)&m1);
    AutoPtr<IMediaItem> m2;
    GetBeforeMediaItem((IMediaItem**)&m2);
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings1 = new MediaArtistNativeHelper::ClipSettings();
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings2 = new MediaArtistNativeHelper::ClipSettings();
    AutoPtr<MediaArtistNativeHelper::TransitionSettings> transitionSetting;
    AutoPtr<MediaArtistNativeHelper::EditSettings> editSettings = new MediaArtistNativeHelper::EditSettings();
    AutoPtr<List<AutoPtr<MediaArtistNativeHelper::EffectSettings> > > effectSettings_clip1;
    AutoPtr<List<AutoPtr<MediaArtistNativeHelper::EffectSettings> > > effectSettings_clip2;

    String output;

    if (mNativeHelper == NULL) {
        if (m1 != NULL)
            ((MediaItem*)m1.Get())->GetNativeContext((MediaArtistNativeHelper**)&mNativeHelper);
        else if (m2 != NULL)
            ((MediaItem*)m2.Get())->GetNativeContext((MediaArtistNativeHelper**)&mNativeHelper);
    }
    GetTransitionSettings((MediaArtistNativeHelper::TransitionSettings**)&transitionSetting);
    if (m1 != NULL && m2 != NULL) {
        /* transition between media items */
        ((MediaItem*)m1.Get())->GetClipSettings((MediaArtistNativeHelper::ClipSettings**)&clipSettings1);
        ((MediaItem*)m2.Get())->GetClipSettings((MediaArtistNativeHelper::ClipSettings**)&clipSettings2);
        clipSettings1->beginCutTime = (Int32)(clipSettings1->endCutTime -
                                                          mDurationMs);
        clipSettings2->endCutTime = (Int32)(clipSettings2->beginCutTime +
                                                          mDurationMs);
        /*
         * Check how many effects and overlays overlap with transition and
         * generate effect clip first if there is any overlap
         */
        effectSettings_clip1 = IsEffectandOverlayOverlapping(m1, clipSettings1, 1);
        effectSettings_clip2 = IsEffectandOverlayOverlapping(m2, clipSettings2, 2);

        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it = effectSettings_clip2->Begin();
        for(; it != effectSettings_clip2->End(); it++) {
            (*it)->startTime += mDurationMs;
        }
        editSettings->effectSettingsArray = ArrayOf<MediaArtistNativeHelper::EffectSettings *>::Alloc(effectSettings_clip1->GetSize()
                                             + effectSettings_clip2->GetSize());

        Int32 i = 0;
        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it1 = effectSettings_clip1->Begin();
        for(; it1 != effectSettings_clip1->End(); it1++, i++) {
            editSettings->effectSettingsArray->Set(i, (*it1));
        }

        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it2 = effectSettings_clip2->Begin();
        for(; it2 != effectSettings_clip2->End(); it2++, i++) {
            editSettings->effectSettingsArray->Set(i, (*it2));
        }
    }
    else if (m1 == NULL && m2 != NULL) {
        /* begin transition at first media item */
        ((MediaItem*)m2.Get())->GenerateBlankFrame(clipSettings1);
        ((MediaItem*)m2.Get())->GetClipSettings((MediaArtistNativeHelper::ClipSettings**)&clipSettings2);
        clipSettings1->endCutTime = (Int32)(mDurationMs + 50);
        clipSettings2->endCutTime = (Int32)(clipSettings2->beginCutTime +
                                                          mDurationMs);
        /*
         * Check how many effects and overlays overlap with transition and
         * generate effect clip first if there is any overlap
         */
        effectSettings_clip2 = IsEffectandOverlayOverlapping(m2, clipSettings2, 2);

        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it = effectSettings_clip2->Begin();
        for(; it != effectSettings_clip2->End(); it++) {
            (*it)->startTime += mDurationMs;
        }

        editSettings->effectSettingsArray = ArrayOf<MediaArtistNativeHelper::EffectSettings *>::Alloc(effectSettings_clip2->GetSize());


        Int32 i = 0;
        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it2 = effectSettings_clip2->Begin();
        for(; it2 != effectSettings_clip2->End(); it2++, i++) {
            editSettings->effectSettingsArray->Set(i, (*it2));
        }
    }
    else if (m1 != NULL && m2 == NULL) {
        /* end transition at last media item */
        ((MediaItem*)m1.Get())->GetClipSettings((MediaArtistNativeHelper::ClipSettings**)&clipSettings1);
        ((MediaItem*)m1.Get())->GenerateBlankFrame(clipSettings2);
        clipSettings1->beginCutTime = (Int32)(clipSettings1->endCutTime -
                                                          mDurationMs);
        clipSettings2->endCutTime = (Int32)(mDurationMs + 50);
        /*
         * Check how many effects and overlays overlap with transition and
         * generate effect clip first if there is any overlap
         */
        effectSettings_clip1 = IsEffectandOverlayOverlapping(m1, clipSettings1, 1);
        editSettings->effectSettingsArray = ArrayOf<MediaArtistNativeHelper::EffectSettings *>::Alloc(effectSettings_clip1->GetSize());

        Int32 i = 0;
        List<AutoPtr<MediaArtistNativeHelper::EffectSettings> >::Iterator it1 = effectSettings_clip1->Begin();
        for(; it1 != effectSettings_clip1->End(); it1++, i++) {
            editSettings->effectSettingsArray->Set(i, (*it1));
        }
    }

    editSettings->clipSettingsArray = ArrayOf<MediaArtistNativeHelper::ClipSettings*>::Alloc(2);
    editSettings->clipSettingsArray->Set(0, clipSettings1);
    editSettings->clipSettingsArray->Set(1, clipSettings2);
    editSettings->backgroundMusicSettings = NULL;
    editSettings->transitionSettingsArray = ArrayOf<MediaArtistNativeHelper::TransitionSettings*>::Alloc(1);
    editSettings->transitionSettingsArray->Set(0, transitionSetting);
    mNativeHelper->GenerateTransitionClip(editSettings, mUniqueId,
                                                  m1, m2, THIS_PROBE(ITransition), &output);
    SetFilename(output);

    return NOERROR;
}

ECode Transition::SetFilename(
    /* [in] */ const String& filename)
{
    mFilename = filename;
    return NOERROR;
}

/**
 * Get the transition filename.
 */
ECode Transition::GetFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFilename;
    return NOERROR;
}

/**
 * Remove any resources associated with this transition
 */
ECode Transition::Invalidate()
{
    if (mFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mFilename = NULL;
    }
    return NOERROR;
}

/**
 * Check if the transition is generated.
 *
 * @return true if the transition is generated
 */
ECode Transition::IsGenerated(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mFilename != NULL);
    return NOERROR;
}

/*
 * {@inheritDoc}
 */
//@Override
ECode Transition::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!ITransition::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    *result = mUniqueId.Equals(((Transition*)ITransition::Probe(object))->mUniqueId);
    return NOERROR;
}

/*
 * {@inheritDoc}
 */
//@Override
ECode Transition::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mUniqueId.GetHashCode();
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
