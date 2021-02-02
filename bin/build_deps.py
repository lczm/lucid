import os
import shutil
import subprocess


def move_to_lib():
    working_dir = os.getcwd()
    working_dir = working_dir.split(os.path.sep)

    # Not being ran from the bin folder
    if (working_dir[-1] == 'bin'):
        os.chdir(os.path.join(os.pardir, "lib"))
    else:
        os.chdir("lib")

    return 0


def build_assimp():
    os.chdir("assimp")

    # Run cmake on lucid/lib/assimp - Generates the build file
    subprocess.run(["cmake", "-GNinja", "CMakeLists.txt"])

    # Build the processes itself, use Ninja
    # On Windows, the developer command prompt comes with ninja by default
    subprocess.run(["ninja"])

    print("Finished building assimp")
    return 0


def move_assimp():
    # Copy the DLL
    files = os.listdir("./bin")
    file = [file for file in files if ".dll" in file][0]

    src = os.path.join("./bin", file)
    dest = os.path.join(os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying assimp.dll")

    # Copy the Lib
    files = os.listdir("./lib")
    file = [file for file in files if ".lib" in file][0]

    src = os.path.join("./lib", file)
    dest = os.path.join(os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying assimp.lib")

    os.chdir(os.pardir)
    return 0


def build_openal():
    os.chdir("openal-soft")

    # Go into openal-soft/build
    os.chdir("build")

    # Run cmake on lucid/lib/openal-soft/build
    subprocess.run(["cmake", "-GNinja", ".."])

    # Run ninja
    subprocess.run(["ninja"])

    print("Finished building openal-soft")
    return 0


def move_openal():
    # Copy the DLL
    files = os.listdir('.')
    file = [file for file in files if "OpenAL32.dll" in file][0]

    # Because openal-soft is in the lib/openal-soft/build folder, needs to traverse up twice
    src = os.path.join(file)
    dest = os.path.join(os.pardir, os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying OpenAL32.dll")

    file = [file for file in files if "OpenAL32.lib" in file][0]

    src = os.path.join(file)
    dest = os.path.join(os.pardir, os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying OpenAL32.lib")

    os.chdir(os.path.join(os.pardir, os.pardir))
    return 0


def build_libsndfile():
    os.chdir("libsndfile")

    if not os.path.isdir("build"):
        os.mkdir("build")

    os.chdir("build")

    # Run cmake on lucid/lib/libsndfile/build
    subprocess.run(["cmake", "-GNinja", ".."])

    # Run ninja
    subprocess.run(["ninja"])

    print("Finished building libsndfile")
    return 0


def move_libsndfile():
    # Copy the DLL
    files = os.listdir('.')
    file = [file for file in files if "sndfile.lib" in file][0]

    # Because openal-soft is in the lib/openal-soft/build folder, needs to traverse up twice
    src = os.path.join(file)
    dest = os.path.join(os.pardir, os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying sndfile.lib")
    return 0


def build_freetype():
    os.chdir("freetype2")

    if not os.path.isdir("build"):
        os.mkdir("build")

    os.chdir("build")

    # Run cmake on lucid/lib/freetype2/build
    subprocess.run(["cmake", "-DBUILD_SHARED_LIBS:BOOL=true", "-GNinja", ".."])

    # Run ninja
    subprocess.run(["ninja"])

    print("Finished building freetype2")
    return 0


def move_freetype():
    # Copy the DLL
    files = os.listdir('.')
    file = [file for file in files if "freetyped.lib" in file][0]

    # Because openal-soft is in the lib/openal-soft/build folder, needs to traverse up twice
    src = os.path.join(file)
    dest = os.path.join(os.pardir, os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying freetyped.lib")

    file = [file for file in files if "freetyped.dll" in file][0]

    # Because openal-soft is in the lib/openal-soft/build folder, needs to traverse up twice
    src = os.path.join(file)
    dest = os.path.join(os.pardir, os.pardir, os.pardir, "build")

    shutil.copy(src, dest)
    print("Finished copying freetyped.dll")
    return 0


if __name__ == "__main__":
    move_to_lib()

    build_assimp()
    move_assimp()

    build_openal()
    move_openal()

    build_libsndfile()
    move_libsndfile()

    build_freetype()
    move_freetype()
