#==========================================================================
# Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
#==========================================================================

lube.lbo :  $(MAKEDIR)/background/backcpp.lub       \
            $(MAKEDIR)/background/background.lub    \
            $(MAKEDIR)/background/backh.lub         \
            $(MAKEDIR)/background/nakedbackh.lub \
            $(MAKEDIR)/background/nakedbackcpp.lub \
            $(MAKEDIR)/background/clsobjh.lub       \
            $(MAKEDIR)/background/clsobjcpp.lub     \
            $(MAKEDIR)/background/declaspect.lub    \
            $(MAKEDIR)/background/declbase.lub      \
            $(MAKEDIR)/background/declcallback.lub  \
            $(MAKEDIR)/background/declfilter.lub    \
            $(MAKEDIR)/background/declregime.lub   \
            $(MAKEDIR)/background/declsync.lub      \
            $(MAKEDIR)/background/implaspect.lub    \
            $(MAKEDIR)/background/implbase.lub      \
            $(MAKEDIR)/background/implcallback.lub  \
            $(MAKEDIR)/background/implfilter.lub    \
            $(MAKEDIR)/background/implregime.lub   \
            $(MAKEDIR)/background/implsync.lub      \
            $(MAKEDIR)/background/modpub.lub        \
            $(MAKEDIR)/background/callbacksinkcpp.lub       \
            $(MAKEDIR)/background/callbacksinkh.lub         \
            $(MAKEDIR)/background/sinkcpp.lub       \
            $(MAKEDIR)/background/sinkh.lub         \
            $(MAKEDIR)/background/aspectintf.lub    \
            $(MAKEDIR)/background/superh.lub        \
            $(MAKEDIR)/background/genericnew.lub  \
            $(MAKEDIR)/background/makefile.lub    \
	        $(MAKEDIR)/carusage/carusage.lub        \
            $(MAKEDIR)/carusage/methodusage.lub     \
            $(MAKEDIR)/carusage/interfaceusage.lub  \
            $(MAKEDIR)/carusage/classusage.lub      \
            $(MAKEDIR)/carusage/objectusage.lub     \
            $(MAKEDIR)/carusage/callbackusage.lub   \
            $(MAKEDIR)/carusage/genericusage.lub    \
            $(MAKEDIR)/carusage/aspectusage.lub     \
            $(MAKEDIR)/carusage/regimeusage.lub    \
            $(MAKEDIR)/carusage/enumusage.lub       \
            $(MAKEDIR)/carusage/constusage.lub      \
            $(MAKEDIR)/carusage/structusage.lub     \
            $(MAKEDIR)/foreground/forecpp.lub       \
            $(MAKEDIR)/foreground/forecpph.lub      \
            $(MAKEDIR)/foreground/foregeneric.lub   \
            $(MAKEDIR)/foreground/foreground.lub    \
            $(MAKEDIR)/foreground/foreh.lub         \
            $(MAKEDIR)/foreground/nakedforeh.lub     \
            $(MAKEDIR)/foreground/nakedforecpp.lub   \
            $(MAKEDIR)/foreground/sources.lub       \
            $(MAKEDIR)/header/baseh.lub             \
            $(MAKEDIR)/header/trigger.lub           \
            $(MAKEDIR)/header/classobj.lub          \
            $(MAKEDIR)/header/basenewdecl.lub       \
            $(MAKEDIR)/header/supernew.lub          \
            $(MAKEDIR)/header/classnewdecl.lub      \
            $(MAKEDIR)/header/supercallback.lub     \
            $(MAKEDIR)/header/clientcallback.lub    \
            $(MAKEDIR)/header/cstyleh.lub           \
            $(MAKEDIR)/header/decltype.lub          \
            $(MAKEDIR)/header/decluuid.lub          \
            $(MAKEDIR)/header/header.lub            \
            $(MAKEDIR)/header/clienth.lub           \
            $(MAKEDIR)/header/serverh.lub           \
            $(MAKEDIR)/header/genericcallback.lub \
            $(MAKEDIR)/headercpp/headercpp.lub     \
            $(MAKEDIR)/headercpp/basecpp.lub       \
            $(MAKEDIR)/headercpp/basenewimpl.lub    \
            $(MAKEDIR)/headercpp/classnewimpl.lub   \
            $(MAKEDIR)/public/cls2abrg.lub          \
            $(MAKEDIR)/public/cls2car.lub           \
            $(MAKEDIR)/public/public.lub            \
            $(MAKEDIR)/public/uuidcpp.lub           \
            $(MAKEDIR)/public/uuidh.lub

a2clube.lbo :   $(MAKEDIR)/android/android.lub          \
                $(MAKEDIR)/android/socpp.lub            \
                $(MAKEDIR)/android/soh.lub              \
                $(MAKEDIR)/android/sosources.lub        \
                $(MAKEDIR)/android/so2libdef.lub        \
                $(MAKEDIR)/android/dirs.lub             \
                $(MAKEDIR)/android/a2cforeground.lub    \
                $(MAKEDIR)/android/a2cforecpph.lub      \
                $(MAKEDIR)/android/a2cforecpp.lub       \
                $(MAKEDIR)/android/a2cforeh.lub         \
                $(MAKEDIR)/android/a2csources.lub

CAR2Javalube.lbo :    $(MAKEDIR)/car2java/CAR2Java.lub             \
                      $(MAKEDIR)/car2java/CoNativeClass.lub        \
                      $(MAKEDIR)/car2java/CoNativeInterface.lub