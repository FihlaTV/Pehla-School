## Pehla School

Pehla School consists of 2 apps as following:

1. Pehla School
* Main learning app with tailored curriculum.

2. Pehla School Library
* A collection of videos and books for children to enjoy.
* Video will play in place, while a separate book viewer will open to read a book.


The apps above share code from kitkitschoollogger

Both the apps are developed using cocos2d-x engine


## Build Environment

Android studio and Android NDK is necessary to build the APKs.
Be sure to use the specified versions mentioned below, to avoid errors. 
Android studio will also ask to install the relavent SDKs and build tools that are missing - please install them along the way. 


1. Download and install [Android Studio](https://developer.android.com/studio/).

2. Download Android [NDK r14b](https://dl.google.com/android/repository/android-ndk-r14b-windows-x86_64.zip).
3. Unzip the NDK file and set its path in the pehlaschool and pehlaschoollibrary code.

## Copy Resource files

Download the relevant resources/3rd party files from [release assets](https://github.com/XPRIZE/GLEXP-Team-KitkitSchool/releases/download/v1.0/cocos2d.zip) 

* to build pehlaschool, place the 3rd party files in the following folders
  * ROOT/pehlaschool/cocos2d
  * ROOT/pehlaschoollibrary/cocos2d

## Build the APKs


Build with Android Studio

- set the appropriate 'Build Variant' 
   - pehlaschool : hindiDebug/hindiRelease
   - pehlaschoollibrary : englishDebug
- build variant of pehlaschoollogger will be set automatically to release if not then set it manually
- in the Build menu click on Build APK option

The resulting debug and release APKs will be generated in _app/build/outputs/apk/_ and _app/_ folders respectively.

**Note: Please make sure there is enough storage available on the device**