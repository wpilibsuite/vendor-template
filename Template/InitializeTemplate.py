import os
import shutil

from os.path import join

project_name = input("Please enter a name for your project: ")
project_name = project_name.replace('-', '')
project_name = project_name.replace(' ', '')
project_name = project_name.replace('.', '')
project_name = project_name.replace('_', '')
package_name = input("Please enter a java package name: ")
use_jni = input("Enter 1 to use JNI and a driver library, otherwise enter 0: ")
use_jni = use_jni == '1'

driver_name = project_name + "Driver"
package_with_underscores = package_name.replace(".", "_")
java_package_dir = join(join("java", 'src'), package_name.replace(".", os.sep))
template_location = 'TemplateFiles'

driver_name_lower = driver_name.lower()
project_name_lower = project_name.lower()

def replaceImplDriver(file, output):
    dir = os.path.dirname(output)
    
    if not os.path.exists(os.path.dirname(output)):
        if dir:
            os.makedirs(os.path.dirname(output))
    shutil.copyfile(file, output)
    with open(output) as f:
        s = f.read()
    s = s.replace("$implreplace$", project_name)
    s = s.replace("$driverreplace$", driver_name)
    s = s.replace("$javapackage$", package_name)
    s = s.replace("$packageunderscores$", package_with_underscores)
    s = s.replace("$implreplacelower$", project_name_lower)
    s = s.replace("$driverreplacelower$", driver_name_lower)
    if use_jni:
        s = s.replace("$usedriverreplace$", "ext.useDriver = true")
        s = s.replace("$includeriverreplace$", "include 'arm:driver'")
    else:
        s = s.replace("$usedriverreplace$", "ext.useDriver = false")
        s = s.replace("$includeriverreplace$", "")
    with open(output, "w") as f:
        f.write(s)

#http://stackoverflow.com/questions/1868714/how-do-i-copy-an-entire-directory-of-files-into-an-existing-directory-using-pyth

def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)        


copytree(join(template_location, "rootfiles"), os.getcwd())

java_template_location = join(template_location, 'java')

shutil.copytree(join(java_template_location, 'arm-linux'), join('java', 'arm-linux'))

java_withJNI_location = join(java_template_location, 'useJNI')

c_source_with_driver  = join(template_location, 'useDriver')

java_withoutJNI_location = join(java_template_location, 'withoutJNI')

c_source_without_driver  = join(template_location, 'withoutDriver')

jni_files_to_copy = {
        join(java_withJNI_location, 'Sample.java') : join(java_package_dir, 'Sample.java'),
        join(template_location, 'driver.gradle') : 'driver.gradle',
        join(java_withJNI_location, 'implJNI.java') : join(java_package_dir, project_name + 'JNI.java'),
        join(java_withJNI_location, 'implJNI.cpp') : join(join('java', 'lib'), project_name + 'JNI.cpp'),
        join(c_source_with_driver, 'SampleDriver.h') : join(join('driver', 'include'), 'SampleDriver.h'),
        join(c_source_with_driver, 'SampleDriver.cpp') : join(join('driver', 'src'), 'SampleDriver.cpp'),
        join(c_source_with_driver, 'Sample.h') : join(join('cpp', 'include'), 'Sample.h'),
        join(c_source_with_driver, 'Sample.cpp') : join(join('cpp', 'src'), 'Sample.cpp'),
    }

non_jni_files_to_copy = {
        join(java_withoutJNI_location, 'Sample.java') : join(java_package_dir, 'Sample.java'),
        join(c_source_without_driver, 'Sample.h') : join(join('cpp', 'include'), 'Sample.h'),
        join(c_source_without_driver, 'Sample.cpp') : join(join('cpp', 'src'), 'Sample.cpp'),
    }

files_to_copy = {
        join(java_template_location, 'java.gradle') : join('java', 'java.gradle'),
        join(template_location, 'cpp.gradle') : 'cpp.gradle',
        join(template_location, 'settings.gradle') : 'settings.gradle',
        join(template_location, 'build.gradle') : 'build.gradle',
        join(template_location, 'dependencies.gradle') : 'dependencies.gradle',
    }

for key,value in files_to_copy.items():
    replaceImplDriver(key, value)

os.makedirs(join(join('libraries', 'cpp'), 'include'))
os.makedirs(join(join('libraries', 'cpp'), 'lib'))
os.makedirs(join(join('libraries', 'driver'), 'include'))
os.makedirs(join(join('libraries', 'driver'), 'lib'))
os.makedirs(join('libraries', 'java'))

if use_jni:
    for key,value in jni_files_to_copy.items():
        replaceImplDriver(key, value)
else:
    for key,value in non_jni_files_to_copy.items():
        replaceImplDriver(key, value)
