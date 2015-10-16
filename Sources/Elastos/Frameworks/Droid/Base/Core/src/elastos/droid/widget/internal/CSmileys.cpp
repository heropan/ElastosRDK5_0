
#include "elastos/droid/internal/CSmileys.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const Int32 CSmileys::sIconIds[17] = {
    R::drawable::emo_im_happy,
    R::drawable::emo_im_sad,
    R::drawable::emo_im_winking,
    R::drawable::emo_im_tongue_sticking_out,
    R::drawable::emo_im_surprised,
    R::drawable::emo_im_kissing,
    R::drawable::emo_im_yelling,
    R::drawable::emo_im_cool,
    R::drawable::emo_im_money_mouth,
    R::drawable::emo_im_foot_in_mouth,
    R::drawable::emo_im_embarrassed,
    R::drawable::emo_im_angel,
    R::drawable::emo_im_undecided,
    R::drawable::emo_im_crying,
    R::drawable::emo_im_lips_are_sealed,
    R::drawable::emo_im_laughing,
    R::drawable::emo_im_wtf
};

ECode CSmileys::GetSmileyResource(
    /* [in] */ Int32 which,
    /* [out] */ Int32* rs)
{
    *rs = sIconIds[which];
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos