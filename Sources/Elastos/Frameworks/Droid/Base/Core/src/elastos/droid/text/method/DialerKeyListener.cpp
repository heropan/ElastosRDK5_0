#include "elastos/droid/text/method/DialerKeyListener.h"
#include "elastos/droid/text/method/MetaKeyKeyListener.h"
#include "elastos/droid/view/CKeyCharacterMap.h"

using Elastos::Droid::View::CKeyCharacterMap;
using Elastos::Droid::View::IKeyData;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Char32 DialerKeyListener::CHARACTERS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '*', '+', '-', '(', ')', ',', '/', 'N', '.', ' ', ';' };

AutoPtr< ArrayOf<Char32> > DialerKeyListener::GetAcceptedChars()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(22);
    for(Int32 i=0; i<22; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

AutoPtr< ArrayOf<Char32> > DialerKeyListener::GetCHARACTERS()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(22);
    for(Int32 i=0; i<22; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

Int32 DialerKeyListener::GetInputType()
{
    return IInputType::TYPE_CLASS_PHONE;
}

Int32 DialerKeyListener::Lookup(
    /* [in] */ IKeyEvent* event,
    /* [in] */ ISpannable* content)
{
    Int32 metaState;
    event->GetMetaState(&metaState);
    Int32 meta = metaState | GetMetaState(content);
    Char32 num;
    event->GetNumber(&num);
    Int32 number = (Int32)num;

    /*
     * Prefer number if no meta key is active, or if it produces something
     * valid and the meta lookup does not.
     */
    if ((meta & (IMetaKeyKeyListener::META_ALT_ON | IMetaKeyKeyListener::META_SHIFT_ON)) == 0) {
        if (number != 0) {
            return number;
        }
    }

    Int32 match = NumberKeyListener::Lookup(event, content);

    if (match != 0) {
        return match;
    } else {
        /*
         * If a meta key is active but the lookup with the meta key
         * did not produce anything, try some other meta keys, because
         * the user might have pressed SHIFT when they meant ALT,
         * or vice versa.
         */

        if (meta != 0) {
            //Java:    KeyData kd = new KeyData();
            AutoPtr<CKeyCharacterMap::KeyData> kd;
            kd = new CKeyCharacterMap::KeyData();
            AutoPtr< ArrayOf<Char32> > accepted = GetAcceptedChars();
            Boolean bGetKeyData;

            if ((event->GetKeyData(((IKeyData*)kd), &bGetKeyData), bGetKeyData)) {
                for (Int32 i = 1; i < (kd->mMeta)->GetLength(); i++) {
                    if (NumberKeyListener::Ok(accepted.Get(), (*(kd->mMeta))[i])) {
                        return (*(kd->mMeta))[i];
                    }
                }
            }
        }

        /*
         * Otherwise, use the number associated with the key, since
         * whatever they wanted to do with the meta key does not
         * seem to be valid here.
         */

        return number;
    }
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos