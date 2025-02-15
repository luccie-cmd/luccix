#!/bin/python3
import os
import glob
import sys
import subprocess
import threading
from script.util import parseSize, compareFiles

def readConfig(path: str) -> dict[str, list[str]]:
    """Reads a configuration file and returns a dictionary of lists of strings."""
    config = {}
    try:
        with open(path, "r") as f:
            lines = f.readlines()
            for line in lines:
                line = line.strip()
                if ':' in line:
                    key, value = line.split(":", 1)
                    key = key.strip()
                    value = value.strip().strip("'")
                    config[key] = [v.strip().strip("'") for v in value.split(",")]
    except FileNotFoundError:
        print(f"Error: Configuration file '{path}' not found.")
        exit(1)
    return config


def writeConfig(config: dict[str, list[str]], path: str) -> None:
    """Writes a configuration dictionary to a file."""
    with open(path, "w") as f:
        for key, values in config.items():
            f.write(f"{key}:")
            for val in enumerate(values):
                f.write(f"'{val[1]}'")
                if val[0]+1 < len(values):
                    f.write(", ")
            f.write('\n')

def checkConfig(config: dict[str, list[str]], allowed_config: list[list[str, list[str], bool]]) -> bool:
    for (key, valid_values, is_mandatory) in allowed_config:
        if key not in config:
            if is_mandatory:
                print(f"Mandatory config {key} is missing")
                return False
            continue
        if valid_values:
            for value in config[key]:
                if value not in valid_values:
                    print(f"Invalid value '{value}' for key '{key}'")
                    return False
    return True

CONFIG = readConfig("./script/config.py")
OLD_CONFIG = readConfig("./script/config.py.old")
ALLOWED_CONFIG = [
    ["config", ["release", "debug"], True],
    ["compiler", ["gcc", "clang"], True],
    ["asan", ["yes", "no"], True],
    ["outDir", [], True],
]
if not checkConfig(CONFIG, ALLOWED_CONFIG):
    print("Invalid config file.")
    print("Allowed config items")
    for option in ALLOWED_CONFIG:
        name = option[0]
        values = option[1]
        required = option[2]
        print(f"{name} (required = {required})")
        if len(values) == 0:
            print("    This can be anything as long as it's provided")
        else:
            for val in values:
                print(f"  - {val}")
    exit(1)
writeConfig(CONFIG, "./script/config.py.old")
force_rebuild = False
if OLD_CONFIG != CONFIG:
    force_rebuild = True
    print("Configuration changed, rebuilding...")
CONFIG["CFLAGS"] = ['-c', '-DCOMPILE', '-DGL_GLEXT_PROTOTYPES', '-DGLM_ENABLE_EXPERIMENTAL']
CONFIG["CFLAGS"] += ['-Wall', '-Wextra', '-Werror']
CONFIG["CFLAGS"] += ['-finline-functions', '-ffast-math', '-funroll-loops', '-fomit-frame-pointer', '-fstrict-aliasing', '-fmax-errors=1']
CONFIG["CFLAGS"] += ['-march=native', '-mtune=native', '-mavx', '-mavx2', '-mfma', '-nostdlib']
CONFIG["CXXFLAGS"] = ['-frtti']
CONFIG["ASFLAGS"] = ['-felf64']
CONFIG["LDFLAGS"] = ['']
CONFIG["INCPATHS"] = ['-Iinclude']
if "debug" in CONFIG.get("config"):
    CONFIG["ASFLAGS"] += ["-O0"]
    CONFIG["ASFLAGS"] += ["-DDEBUG"]
    CONFIG["CFLAGS"] += ["-O0"]
    CONFIG["CFLAGS"] += ["-DDEBUG"]
    CONFIG["CFLAGS"] += ["-ggdb"]
else:
    CONFIG["ASFLAGS"] += ["-O2"]
    CONFIG["ASFLAGS"] += ["-DNDEBUG"]
    CONFIG["CFLAGS"] += ["-O2"]
    CONFIG["CFLAGS"] += ["-DNDEBUG"]

if "clang" in CONFIG.get("compiler"):
    CONFIG["CFLAGS"] += ['-Wno-nan-infinity-disabled']
elif "gcc" in CONFIG.get("compiler"):
    CONFIG["CFLAGS"] += ['-fprefetch-loop-arrays', '-fsingle-precision-constant']

if "debug" in CONFIG.get("config"):
    CONFIG["LDFLAGS"] += ["-O0"]
    CONFIG["CFLAGS"] += ["-g"]
else:
    CONFIG["LDFLAGS"] += ["-O3"]

if "yes" in CONFIG.get("asan"):
    CONFIG["LDFLAGS"] += ["-fsanitize=address"]
    CONFIG["CFLAGS"] += ["-fsanitize=address"]


def callCmd(command, print_out=False):
    with open("commands.txt", "a") as f:
        f.write(command+'\n')
    result = subprocess.run(command, capture_output=not print_out, text=True, shell=True, stderr=None)
    return [result.returncode, result.stdout]

callCmd("rm -rf commands.txt")

def checkExtension(file: str, valid_extensions: list[str]):
    for ext in valid_extensions:
        if file.endswith(ext):
            return True
    return False

def getExtension(file):
    return file.split(".")[-1]

def buildASM(file):
    compiler = "nasm"
    options = CONFIG["ASFLAGS"].copy()
    command = compiler + " " + file
    for option in options:
        command += " " + option
    print(f"AS    {file}")
    command += f" -o {CONFIG['outDir'][0]}/{file}.o"
    return callCmd(command, True)[0]

def buildC(file):
    compiler = CONFIG["compiler"][0]
    options = CONFIG["CFLAGS"].copy()
    options.append("-std=c11")
    command = compiler + " " + file
    for option in options:
        command += " " + option
    print(f"C     {file}")
    command += f" -o {CONFIG['outDir'][0]}/{file}.o"
    return callCmd(command, True)[0]

def buildCXX(file):
    compiler = CONFIG["compiler"][0]
    if compiler == "gcc":
        compiler = "g"
    compiler += "++"
    options = CONFIG["CFLAGS"].copy()
    options += CONFIG["CXXFLAGS"].copy()
    options.append("-std=c++23")
    command = compiler + " " + file
    for option in options:
        command += " " + option
    print(f"CXX   {file}")
    command += f" -o {CONFIG['outDir'][0]}/{file}.o"
    return callCmd(command, True)[0]

def linkDir(kernel_dir, use_gcc: bool=False, use_libc:bool=False, static_lib_files=[], outname="a"):
    files = glob.glob(kernel_dir+'/**', recursive=True)
    command = "ld"
    if use_gcc:
        compiler = CONFIG["compiler"][0]
        if compiler == "gcc":
            compiler = "g"
        compiler += "++"
        command = compiler
    options = CONFIG["LDFLAGS"]
    for option in options:
        command += " " + option
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["o"]):
            continue
        command += " " + file
    if not use_libc:
        command += " -nostdlib"
    if use_gcc:
        if not use_libc:
            command += " -Wl,--no-whole-archive"
            command += " -Wl,--whole-archive"
    else:
        if not use_libc:
            command += " --no-whole-archive"
            command += " --whole-archive"
    for static_lib in static_lib_files:
        command += f" {static_lib}"
    if "debug" in CONFIG.get("config"):
        if use_gcc:
            command += f" -Wl,-Map={CONFIG['outDir'][0]}/{outname}.map"
        else:
            command += f" -Map={CONFIG['outDir'][0]}/{outname}.map"
    command += f" -o {CONFIG['outDir'][0]}/{outname}.elf"
    file = f"{CONFIG['outDir'][0]}/{outname}.elf"
    print(f"LD   {file}")
    if callCmd(command, True)[0] != 0:
        print(f"LD   {file} Failed")
        exit(1)
    callCmd(f"objdump -C -d -Mintel64 -g -l -R -r -T -t -L {CONFIG['outDir'][0]}/{outname}.elf > {CONFIG['outDir'][0]}/{outname}.asm")

def buildDir(directory, out_file="", static_lib: bool=False):
    os.makedirs(CONFIG["outDir"][0]+'/'+directory, exist_ok=True)
    CONFIG["INCPATHS"] += [f'-I{directory}']
    CONFIG["INCPATHS"] += [f'-I{directory}/include']
    files = glob.glob(directory+'/**', recursive=True)
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["c", "cc", "asm"]):
            continue
        basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
        str_paths = ""
        for incPath in CONFIG["INCPATHS"]:
            str_paths += f" {incPath}"
        for flags in CONFIG["CFLAGS"]:
            flag: str = flags
            if flag.startswith("-D"):
                str_paths += f" {flag}"
        callCmd(f"cpp {str_paths} {file} -o ./tmp.txt")
        if not force_rebuild and compareFiles("./tmp.txt", os.path.abspath(f"/tmp/{basename}/cache/{file}")):
            continue
        callCmd(f"mkdir -p {CONFIG['outDir'][0]}/{os.path.dirname(file)}")
        callCmd(f"mkdir -p /tmp/{basename}/cache/{os.path.dirname(file)}")
        callCmd(f"cp ./tmp.txt /tmp/{basename}/cache/{file}")
        code = 0
        CONFIG["ASFLAGS"] += CONFIG["INCPATHS"]
        CONFIG["CFLAGS"] += CONFIG["INCPATHS"]
        if getExtension(file) == "asm":
            code = buildASM(file)
        elif getExtension(file) == "cc":
            code = buildCXX(file)
        elif getExtension(file) == "c":
            code = buildC(file)
        else:
            print(f"Invalid or unhandled extension {getExtension(file)}")
            exit(1)

        for incPath in CONFIG["INCPATHS"]:
            CONFIG["ASFLAGS"].remove(incPath)
            CONFIG["CFLAGS"].remove(incPath)

        if code != 0:
            callCmd(f"rm -f /tmp/{basename}/cache/{file}")
            exit(code)

    if static_lib:
        buildAR(directory, out_file)

def buildAR(directory: str, out_file: str):
    files = glob.glob(f"{CONFIG['outDir'][0]}/{directory}/**", recursive=True)
    obj_files = []
    a_files = []
    for file in files:
        if not os.path.isfile(file):
            continue
        if not checkExtension(file, ["a", 'o']):
            continue
        if getExtension(file) == "o":
            obj_files.append(file)
        elif getExtension(file) == "a":
            a_files.append(file)
    while len(a_files) >= 1:
        for a_file in a_files:
            callCmd(f"mkdir -p tmp/{os.path.basename(a_file)[:-2]}")
            cmd = f"ar x {a_file} --output=tmp/{os.path.basename(a_file)[:-2]}"
            callCmd(cmd)
        files = glob.glob(f"tmp/**", recursive=True)
        a_files.clear()
        for file in files:
            if not os.path.isfile(file):
                continue
            if not checkExtension(file, ['a', 'o']):
                continue
            if getExtension(file) == 'o':
                obj_files.append(file)
            elif getExtension(file) == 'a':
                a_files.append(file)
    obj_files_str = " ".join(obj_files)
    cmd = f"ar rcs {out_file} {obj_files_str}"
    print(f"AR    {out_file}")
    callCmd(cmd)
    if os.path.exists("tmp"):
        callCmd("rm -rf tmp")

def getInfo():
    callCmd("rm -f info.txt")
    callCmd("touch info.txt")
    callCmd(f"cloc . --exclude-dir=limine,bin,script --exclude-ext=txt >> info.txt")
    callCmd(f"tree -I 'bin' -I 'script' -I '.vscode' -I '*.txt' >> info.txt")

def main():
    basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
    if "clean" in sys.argv:
        callCmd(f"rm -rf /tmp/{basename}")
        callCmd(f"rm -rf {CONFIG['outDir'][0]}")
        global force_rebuild
        force_rebuild = True
    if force_rebuild:
        print("Rebuilding...")
    print("> Creating necesarry dirs")
    callCmd(f"mkdir -p {CONFIG['outDir'][0]}")
    print("> Building stdlib")
    buildDir("stdlib", f"{CONFIG['outDir'][0]}/stdlib.a", True)
    print("> Building assembler")
    buildDir("assembler")
    print("> Linking assembler")
    linkDir(f"{CONFIG['outDir'][0]}/assembler", use_gcc=True, use_libc=True, outname="assembler", static_lib_files=[f"{CONFIG['outDir'][0]}/stdlib.a"])
    print("> Getting info")
    getInfo()

if __name__ == '__main__':
    main()