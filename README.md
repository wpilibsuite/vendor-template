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

The template will automatically grab the newest version of WPILib and all of it's dependencies to ensure the buiild is compatable with the lastest images and plugins

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

## Generating JNI headers

In order to write proper JNI cpp files, you will probably need the JNI headers generated in order to know the function declarations. In order to do this, run the following command.
```bash
./gradlew jniHeaders
```

This will generate the JNI headers, and place them into `/arm/driver/build/include`. You can then use those headers in order to create your JNI cpp files.

## User Selectable Options
In order to facilitate more custom uses, a large amount of user selectable options have been provided. These options can be found either in the `locations.gradle` or `properties.gradle` in the root of the directory. Below is a list of options in each file, and what they do.

`locations.gradle` includes options for selecting locations for source code and release directories. By default, these will point to directories inside of the project, however they can be changed to point anywhere on disk, for instance if you already have your source code in another directory.

`properties.gradle` includes other user configurable options by the library. By default, the properties that select if libraries are merged together are set to true, however properties that would include sources are set to false. This allows you to decide whether to include sources or not in the release files. 

## Linking to other libraries
During creation, a folder called libraries is created in the root of the repo. Inside this folder are folders for the cpp library and the driver library. Headers and C libraries can be placed in these folders, and they will be automatically added to the projects. In addition, raw source code can be placed in the `src` folder in order to be build with the library. The driver library will only be able to access the driver libraries, and the implementation will be able to access both sets of native libraries. These libraries are placed in the release zips in order to be usable. In addition, a Java folder will be created as well. There is a `lib` folder and a `src` folder in here as well. The lib folder will be used in the classpath, and will be embedded in the output jar if the `embedJavaLibraries` option is set to true in `properties.gradle`. The `src` folder will always be built and included in the output jar. 

Note that all of these paths can be individually changed in the `locations.gradle` file in order to work with your build system.

## Packing for distribution
During the build process, a release folder will be created in the root of the repository. Inside of this folder will be multiple files, but there are only two that are really important. `ProjectName-usershared.zip` and `ProjectName-userstatic.zip`. These are almost identical in all respects except the static version will include static libraries for linking with wpilibc, and the shared version will include shared libraries for linking with wpilibc. 

The zips will include a folder structure already set up for integrating directly into eclipse. By default, the `/java/lib` folder will include the library jar. If the option for `embedJavaLibraries` was not selected, any external jars that were linked to will also be placed in this folder. Otherwise, the contents of those libraries will be included in the jar. In addition, if the JNI option was selected, or the library link folder included any shared libraries, those will be copied into the lib folder as well. If the `includeJavaSources` or `includeJavaJavadoc` properties were enabled, the jars for those will also be included in the lib directory.

For C++, the `/cpp/include` folder will include all headers included in both the driver project and the cpp project, along with any headers from other libraries that were included. The `/cpp/lib` folder will include either the shared or static libraries built by this project, in addition to any libraries included externally. And finally, if you enabled either `includeCppSources` or `includeDriverSources`, the source files will be placed in `/cpp/src/ProjectName`. Note that any source files included from external libraries will never be included, so if you want proprietary C++ source files, the external library directories are our recommended location to place those source files.

For choosing between static and shared, that option is entirely up to you. The eclipse plugins will gracefully handle both methods of distribution, so that is entirely up to your user preference.

## Linking to release WPILib

By default, the library will use the latest development branch of WPILib. This branch is updated every time a new commit is pushed to master. For most development purposes, this is the recommended branch to be using. However, if you want to build a release that links to the latest wpilib release build, you can do so with the following commands.

```bash
./gradlew clean
./gradlew build -PreleaseType=OFFICIAL
```

The clean is required in order to remove all development dependencies, and then the build with the releaseType property selects the the release repository. To switch back to the development repository, run a clean, and then run build by itself like before.

## Editing the build settings

By default, you should not need to modify any settings in any of the gradle files other than `locations.gradle` and `properties.gradle`. However, if you do realize you need any addition changes, most of the common ones are listed below.

- Include more then one source or header directory to a cpp project.
  - Inside either `cpp.gradle` or `driver.gradle`, depending on which project you want to modify, you will see a property called `srcDirs` inside of the `source` property. Into that array is where you would add any addtional cpp source locations. For headers, modify the `srcDirs` property inside of the `exportedHeaders` property to include the new location you want to use. 
  - Note if you want to add a seperate location for prebuilt libraries, this is more difficult, so we would recommend contacting WPILib for help with this.
- Include more then one library or source directory for Java
  - Inside of `java/java.gradle`, inside of the sourceSets properties there is a `srcDirs` property. Add any addition source locations to this array. For libraries, in the dependencies property you will see something like these 2 lines below. Copy both of these lines to have a second copy right below the existing one, and change `javaLibraryLoc` to be a string to your desired location.
  ```
  compile fileTree(dir: javaLibraryLoc, include: ['*.jar'])
  runtime fileTree(dir: javaLibraryLoc, include: ['*.jar'])
  ```
- Adding new JNI libraries.
  - When JNI headers are generated, only specific classes specified have headers generated for them. If you would like to add additional Java classes to create headers from, inside of `locations.gradle` there is a property called `jniDefinitions`. Add any additional classes you would like to generate headers for to this array.

If you run into something different you would like to change, contact us and we will attempt to help figure out how to properly perform that change.
