# vendor-template

vendor-template is a build system for vendors to use in order to link their libraries properly into WPILib.

## Build Requirements
To run a build, a few requirements must be met:

- ARM Toolchain - You must have the FRC ARM toolchain installed, which can be found [here](http://first.wpi.edu/FRC/roborio/toolchains/).

In addition, python is required to generate the repository correctly. However after initial creation python is no longer necessary

## Building
Gradle is the main build system used by this template. All tasks are run with the `gradlew` wrapper, which is included in the root of the repository. All targets that can be accomplished by Gradle are referred to as tasks. The main task available is `build`. To run Gradle, cd into the build directory and run:

```bash
./gradlew build
```

The template will automatically grab the newest version of WPILib and all of it's dependancies to ensure the buiild is compatable with the lastest images and plugins

## Setting up the repository
In order to properly set up the repository, a file called InitializeTemplate.py is provided. Running this will ask for 3 questions. The first is a project name (ex. CANJaguar, ADXRS535). The second is a java package to place the code into. The 3rd is asking if you require JNI. If there is functionality you need that is not provided by WPILib but can be accessed using either the HAL or native code, that is when to use JNI.

Once you enter that info, all files will be copied into their correct places, and the builds will be ready to go.

## Using the repository (JNI)
If you selected to use JNI, the build system will build 3 libraries. The first will be a driver/JNI C++ library. This library will link to the HAL and WPIUtil, however it has no access to either NetworkTables or WPILibC directly. The reason for this is JNI will potentially run into issues if either of those libraries are linked to. The goal of this library is to provide a low level C++ class that can be used from any language. This library should also expose a C API to access that class in order to allow interop for other FRC languages. In addition, this will be the library the JNI code is placed into, and is the native library Java will use. For this library, if you can avoid any HAL level calls, and instead use the NI provided FPGA libraries directly, this driver library should also be usable from LabVIEW. However this has not been tested, and if you do use anything from the HAL, the library will most likely not be usable from LabVIEW.

The second library is a Java library. This links to WPILibJ, and can access any functionality provided by WPILibJ. This can also use the JNI provided by the driver library

The 3rd library is the C++ Implementation library. This library has full access to WPILibC and NetworkTables, and should be used to create a public interface for WPILibC teams to use. It can use the driver to create an interface.

## Using the repository (No JNI)
If you did not select to use the JNI, the build system will provide 2 libraries. The first is a C++ library that links to WPILibC with full access to all functionality provided there. The second is a Java library with full access to WPILibJ. This Java library will not have any JNI code in it, so if you need native code, please use the JNI option instead.

## Our recommendations
If your library can be used completely using WPILib level classes and functionality, the no JNI build option should be the right one. However, if you do need native level code, in order for everything to link properly and be functional, you will most likely have to use the driver/JNI library with a thin wrapper on top written in either WPILib language.

## Packing for distribution
During the build process, a release folder will be created in the root of the repository. Inside of this folder will be 6 zips. 

- ProjectName-driver.zip will contain the driver library and all libraries needed to run it. Note if you selected no JNI/Driver library, this zip will be emply except for 2 prebuilt libraries. Will also contain all necessary dependancies and headers
- ProjectName-java.zip will contain the java jar, along with the javadocs for java. In addition, if JNI was selected, this zip will contain the native JNI/driver library as well.
- ProjectName-cpp.zip will contain the C++ library for use with wpilibc. Will also contain all necessary dependancies and headers
- ProjectName-driversources.zip will contain the sources and headers for the driver/jni library. This will not show up if you did not use the JNI/Driver option.
- ProjectName-javasources.zip will contain the Java sources
- ProjectName-cppsources.zip will contain the sources and headers for the wpilibc library. This will not contain the sources for the driver library.

## Linking to other libraries
During creation, a folder called libraries is created in the root of the repo. Inside this folder are folders for the implementation library and the driver library. Headers and C libraries can be placed in these folders, and they will be automatically added to the projects. The driver library will only be able to access the driver libraries, and the implementation will be able to access both sets of native libraries. These libraries are placed in the release zips in order to be usable.
