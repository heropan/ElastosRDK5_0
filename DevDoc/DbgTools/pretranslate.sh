#!/bin/bash
# pretranslate.sh
# pre-translate android java file to Elastos C++ file.
# usages:
# chmod u+x pretranslate.sh
# ./pretranslate.sh srcFilename targetFilename
# or ./pretranslate.sh targetFilename

count=$#
src_file=$1
target_file=$2

if [ $count -eq 1 ]; then
    target_file=$1
    echo "target: $target_file"
elif [ $count -eq 2 ]; then
    src_file=$1
    target_file=$2
    cp $src_file $target_file
    echo "src: $src_file"
    echo "target: $target_file"
fi

# process import
#
sed -i "s@import java.@using Elastos::@" $target_file
sed -i "s@import android.@using Elastos::Droid::@" $target_file
sed -i "s@import com.android.@using Elastos::Droid::@" $target_file
sed -i "s@import com.android.internal.@using Elastos::Droid::Internal::@" $target_file
sed -i "s@import org.xmlpull.v1.@using Org::Xmlpull::V1::I@" $target_file
sed -i "s@import libcore.io.@using Libcore::IO::I@" $target_file

sed -i "s@::io.channels.@::IO::Channels::I@" $target_file
sed -i "s@::io.@::IO::I@" $target_file
sed -i "s@::nio.@::IO::I@" $target_file
sed -i "s@::math.@::Math::I@" $target_file
sed -i "s@::security.@::Security::I@" $target_file
sed -i "s@::sql.sqlite.jdbc.@::Sql::Sqlite::Jdbc::I@" $target_file
sed -i "s@::sql.sqlite.@::Sql::Sqlite::I@" $target_file
sed -i "s@::sql.@::Sql::I@" $target_file
sed -i "s@::util.concurrent.atomic.@::Utility::Concurrent::Atomic::I@" $target_file
sed -i "s@::util.concurrent.locks.@::Utility::Concurrent::Locks::I@" $target_file
sed -i "s@::util.concurrent.@::Utility::Concurrent::I@" $target_file
sed -i "s@::util.jar.@::Utility::Jar::I@" $target_file
sed -i "s@::util.prefs.@::Utility::Prefs::I@" $target_file
sed -i "s@::util.regex.@::Utility::Regex::I@" $target_file
sed -i "s@::util.zip.@::Utility::Zip::I@" $target_file

sed -i "s@::accessibilityservice.@::Accessibilityservice::I@" $target_file
sed -i "s@::accounts.@::Accounts::I@" $target_file
sed -i "s@::android.@::Android::I@" $target_file
sed -i "s@::animation.@::Animation::I@" $target_file
sed -i "s@::app.admin.@::App::Admin::I@" $target_file
sed -i "s@::app.backup.@::App::Backup::I@" $target_file
sed -i "s@::app.job.@::App::Job::I@" $target_file
sed -i "s@::app.trust.@::App::Trust::I@" $target_file
sed -i "s@::app.usage.@::App::Usage::I@" $target_file
sed -i "s@::app.@::App::I@" $target_file
sed -i "s@::appwidget.@::Appwidget::I@" $target_file
sed -i "s@::bluetooth.@::Bluetooth::I@" $target_file
sed -i "s@::content.pm.@::Content::Pm::I@" $target_file
sed -i "s@::content.res.@::Content::Res::I@" $target_file
sed -i "s@::content.@::Content::I@" $target_file
sed -i "s@::database.sqlite.@::Database::Sqlite::I@" $target_file
sed -i "s@::database.@::Database::I@" $target_file
sed -i "s@::emoji.@::Emoji::I@" $target_file
sed -i "s@::gesture.@::Gesture::I@" $target_file
sed -i "s@::googleremote.@::Googleremote::I@" $target_file
sed -i "s@::graphics.drawable.@::Graphics::Drawable::I@" $target_file
sed -i "s@::graphics.pdf.@::Graphics::Pdf::I@" $target_file
sed -i "s@::graphics.@::Graphics::I@" $target_file
sed -i "s@::hardware.camera2.dispatch.@::Hardware::Camera2::Dispatch::I@" $target_file
sed -i "s@::hardware.camera2.impl.@::Hardware::Camera2::Impl::I@" $target_file
sed -i "s@::hardware.camera2.legacy.@::Hardware::Camera2::Legacy::I@" $target_file
sed -i "s@::hardware.camera2.marshal.@::Hardware::Camera2::Marshal::I@" $target_file
sed -i "s@::hardware.camera2.marshal.impl.@::Hardware::Camera2::Marshal::Impl::I@" $target_file
sed -i "s@::hardware.camera2.params.@::Hardware::Camera2::Params::I@" $target_file
sed -i "s@::hardware.camera2.utils.@::Hardware::Camera2::Utils::I@" $target_file
sed -i "s@::hardware.camera2.@::Hardware::Camera2::I@" $target_file
sed -i "s@::hardware.display.@::Hardware::Display::I@" $target_file
sed -i "s@::hardware.hdmi.@::Hardware::Hdmi::I@" $target_file
sed -i "s@::hardware.input.@::Hardware::Input::I@" $target_file
sed -i "s@::hardware.location.@::Hardware::Location::I@" $target_file
sed -i "s@::hardware.soundtrigger.@::Hardware::soundtrigger::I@" $target_file
sed -i "s@::hardware.usb.@::Hardware::Usb::I@" $target_file
sed -i "s@::hardware.@::Hardware::I@" $target_file
sed -i "s@::impl.@::Impl::I@" $target_file
sed -i "s@::inputmethods.@::Inputmethods::I@" $target_file
sed -i "s@::inputmethods.pinyinIME@::Inputmethods::PinyinIME::I@" $target_file
sed -i "s@::inputmethodservice.@::Inputmethodservice::I@" $target_file
sed -i "s@::location.@::Location::I@" $target_file
sed -i "s@::media.@::Media::I@" $target_file
sed -i "s@::mtp.@::Mtp::I@" $target_file
sed -i "s@::net.@::Net::I@" $target_file
sed -i "s@::opengl.@::Opengl::I@" $target_file
sed -i "s@::os.@::Os::I@" $target_file
sed -i "s@::preference.@::Preference::I@" $target_file
sed -i "s@::privacy.@::Privacy::I@" $target_file
sed -i "s@::provider.@::Provider::I@" $target_file
sed -i "s@::sax.@::Sax::I@" $target_file
sed -i "s@::server.@::Server::I@" $target_file
sed -i "s@::service.@::Service::I@" $target_file
sed -i "s@::speech.@::Speech::I@" $target_file
sed -i "s@::statusbar.@::Statusbar::I@" $target_file
sed -i "s@::telephony.@::Telephony::I@" $target_file
sed -i "s@::text.@::Text::I@" $target_file
sed -i "s@::transition.@::Transition::I@" $target_file
sed -i "s@::util.@::Utility::I@" $target_file
sed -i "s@::view.@::View::I@" $target_file
sed -i "s@::webkit.@::Webkit::I@" $target_file
sed -i "s@::widget.@::Widget::I@" $target_file
sed -i "s@::wifi.@::Wifi::I@" $target_file
sed -i "s@::app.@::App::I@" $target_file


# process types
#
sed -i 's/\<byte\>/Byte/g' $target_file
sed -i 's/\<boolean\>/Boolean/g' $target_file
sed -i 's/\<char\>/Char32/g' $target_file
sed -i 's/\<int\>/Int32/g' $target_file
sed -i 's/\<long\>/Int64/g' $target_file
sed -i 's/\<float\>/Float/g' $target_file
sed -i 's/\<double\>/Double/g' $target_file
sed -i 's/\<true\>/TRUE/g' $target_file
sed -i 's/\<false\>/FALSE/g' $target_file
sed -i 's/\<null\>/NULL/g' $target_file

# process static final
#
sed -i "s@static final@static const@" $target_file
sed -i "s@final static@static const@" $target_file

# process comments
#
sed -i "s:    @Override:    //@Override:g" $target_file

# Log.d or Slog.d => Logger::D( or Slogger::D(
#
sed -i "s@Log.d(@Logger::D(@g" $target_file
sed -i "s@Log.i(@Logger::I(@g" $target_file
sed -i "s@Log.e(@Logger::E(@g" $target_file
sed -i "s@Log.w(@Logger::W(@g" $target_file
sed -i "s@Log.v(@Logger::V(@g" $target_file
sed -i "s@Slog.d(@Slogger::D(@g" $target_file
sed -i "s@Slog.i(@Slogger::I(@g" $target_file
sed -i "s@Slog.e(@Slogger::E(@g" $target_file
sed -i "s@Slog.w(@Slogger::W(@g" $target_file
sed -i "s@Slog.v(@Slogger::V(@g" $target_file

# public void => CARAPI
#
sed -i "s:public void:CARAPI:g" $target_file

# public static const class => public class
#
sed -i "s:public static const class:public class:g" $target_file
sed -i "s:public final class:public class:g" $target_file

# String toString() { => CARAPI ToString(/* [out] */ String* str) {
#
sed -i "s:String toString() {:CARAPI ToString(\n        /* [out] */ String* str)\n    {:g" $target_file
sed -i "s:public CARAPI:CARAPI:g" $target_file

# a.fun(  => a->Fun(
#
sed -i 's:\([a-zA-Z]\w*\)\(\.\)\([a-zA-Z]\)\(\w*\)\((\):\1->\u\3\4\5:g' $target_file

# a().fun(  => a()->Fun(
#
sed -i 's:\([a-zA-Z]\w*()\)\(\.\)\([a-zA-Z]\)\(\w*\)\((\):\1->\u\3\4\5:g' $target_file

# fun(  => Fun(
#
sed -i 's:\([a-zA-Z]\w*\)\((\):\u\1\2:g' $target_file

# synchronized ( => synchronized(
#
sed -i "s:synchronized (:synchronized(:g" $target_file

# android.Manifest.permission. => Manifest::permission::
#
sed -i "s@android.Manifest.permission.@Manifest::permission::@g" $target_file
sed -i "s@Manifest.permission.@Manifest::permission::@g" $target_file

sed -i "s@R.string.@R::string::@g" $target_file
sed -i "s@R.color.@R::Color::@g" $target_file
sed -i "s@com.android.R@R@g" $target_file
sed -i "s@com.android.internal.R@R@g" $target_file

sed -i "s@Intent.ACTION_@IIntent::ACTION_@g" $target_file
sed -i "s@Intent.FLAG_@IIntent::FLAG_@g" $target_file

sed -i "s@Settings.Global.@Settings::Global::@g" $target_file


