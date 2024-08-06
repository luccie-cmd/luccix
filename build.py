#!/bin/python3
import os, sys, glob, subprocess
basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
out_dir = ""
# ANSI Color Codes
ANSI_COLOR_RESET = "\x1b[0m"
ANSI_COLOR_BLACK = "\x1b[30m"
ANSI_COLOR_RED = "\x1b[31m"
ANSI_COLOR_GREEN = "\x1b[32m"
ANSI_COLOR_YELLOW = "\x1b[33m"
ANSI_COLOR_BLUE = "\x1b[34m"
ANSI_COLOR_MAGENTA = "\x1b[35m"
ANSI_COLOR_CYAN = "\x1b[36m"
ANSI_COLOR_WHITE = "\x1b[37m"

ANSI_COLOR_BRIGHT_BLACK = "\x1b[30;1m"
ANSI_COLOR_BRIGHT_RED = "\x1b[31;1m"
ANSI_COLOR_BRIGHT_GREEN = "\x1b[32;1m"
ANSI_COLOR_BRIGHT_YELLOW = "\x1b[33;1m"
ANSI_COLOR_BRIGHT_BLUE = "\x1b[34;1m"
ANSI_COLOR_BRIGHT_MAGENTA = "\x1b[35;1m"
ANSI_COLOR_BRIGHT_CYAN = "\x1b[36;1m"
ANSI_COLOR_BRIGHT_WHITE = "\x1b[37;1m"

# ANSI Background Color Codes
ANSI_BG_COLOR_BLACK = "\x1b[40m"
ANSI_BG_COLOR_RED = "\x1b[41m"
ANSI_BG_COLOR_GREEN = "\x1b[42m"
ANSI_BG_COLOR_YELLOW = "\x1b[43m"
ANSI_BG_COLOR_BLUE = "\x1b[44m"
ANSI_BG_COLOR_MAGENTA = "\x1b[45m"
ANSI_BG_COLOR_CYAN = "\x1b[46m"
ANSI_BG_COLOR_WHITE = "\x1b[47m"

ANSI_BG_COLOR_BRIGHT_BLACK = "\x1b[40;1m"
ANSI_BG_COLOR_BRIGHT_RED = "\x1b[41;1m"
ANSI_BG_COLOR_BRIGHT_GREEN = "\x1b[42;1m"
ANSI_BG_COLOR_BRIGHT_YELLOW = "\x1b[43;1m"
ANSI_BG_COLOR_BRIGHT_BLUE = "\x1b[44;1m"
ANSI_BG_COLOR_BRIGHT_MAGENTA = "\x1b[45;1m"
ANSI_BG_COLOR_BRIGHT_CYAN = "\x1b[46;1m"
ANSI_BG_COLOR_BRIGHT_WHITE = "\x1b[47;1m"

# ANSI Text Style Codes
ANSI_STYLE_BOLD = "\x1b[1m"
ANSI_STYLE_UNDERLINE = "\x1b[4m"

def callCmd(command):
    with open("commands.txt", "a+") as f:
        f.write(command+'\n')
    # Run the command and capture the output
    result = subprocess.run(command, capture_output=True, text=True, shell=True)
    if result.returncode != 0:
        print(result.stderr)
    return [result.returncode, result.stdout]

def compareFiles(file1, file2):
    try:
        with open(file1) as f1:
            with open(file2) as f2:
                return f1.read() == f2.read()
    except FileNotFoundError:
        return False

def buildAsm(compiler: str, file: str, out_file: str, out_format: str, extra_args: list[str]):
    print(f"ASM   {file}")
    options = extra_args.copy()
    if compiler == "nasm":
        options.append('-f')
        if out_format == "elf":
            options.append('elf64')
        else:
            print("Invalid ASM output format")
            exit(1)
    else:
        print("Other compilers are not yet implemented")
        print("Attempting to implement own assembler soon")
        exit(1)
    options.append("-o")
    options.append(out_file+'.o')
    options.append(file)
    command = compiler
    for arg in options:
        command += f' {arg}'
    code, _ = callCmd(command)
    if code != 0:
        print(f"ASM   {file} Failed")
    return code

def buildC(compiler, file, out_file, extra_args: list[str]):
    print(f"C     {file}")
    options = extra_args.copy()
    options.append("-std=c11")
    options.append('-Wall')
    options.append('-Wextra')
    options.append('-Werror')
    options.append("-c")
    options.append("-o")
    options.append(out_file+'.o')
    options.append(file)
    command = compiler
    for arg in options:
        command += f' {arg}'
    code, _ = callCmd(command)
    if code != 0:
        print(f"C     {file} Failed")
    return code

def checkExtension(file: str, valid_extensions: list[str]):
    for ext in valid_extensions:
        if file.endswith(ext):
            return True
    return False

def getExtension(file):
    return file.split(".")[-1]

def toCommand(command, options):
    for opt in options:
        command += f" {opt}"
    return command

def buildDir(directory_path: str, out_path: str, extra_args: list[str]=[]) -> int:
    files = glob.glob(directory_path+'/**', recursive=True)
    extra_args.append(f"-I{directory_path}")
    for file in files:
        if not os.path.isfile(file) or file[-1] == '/':
            continue
        if not checkExtension(file, ["asm", "c", "lx", "cc", "cpp"]):
            continue
        code, _ = callCmd(toCommand("cpp", extra_args+['-o', './tmp.txt', file]))
        if code != 0:
            print(f"Failed to pre process {file}")
            exit(1)
        if compareFiles(f"./tmp.txt", os.path.abspath(f"/tmp/{basename}/cache/{file}")):
            continue
        callCmd(toCommand("mkdir", ['-p', f'{out_path}/{os.path.dirname(file)}']))
        callCmd(toCommand("mkdir", ['-p', f'/tmp/{basename}/cache/{os.path.dirname(file)}']))
        callCmd(f"cp ./tmp.txt /tmp/{basename}/cache/{file}")
        code = 0
        if getExtension(file) == "asm":
            code = buildAsm("nasm", file, out_path+'/'+file, "elf", extra_args)
        elif getExtension(file) == "c":
            code = buildC("gcc", file, out_path+'/'+file, extra_args)
        else:
            print(f"Invalid or unhandled extension for {file} {getExtension(file)}")
            callCmd(f"rm -f /tmp/{basename}/cache/{file}")
        if code != 0:
            callCmd(f"rm -f /tmp/{basename}/cache/{file}")
            exit(code)

def linkDir(directory_path: str, out_file: str, extra_args: list[str]=[]):
    obj_files_with_dir = glob.glob(directory_path+'/**', recursive=True)
    options = extra_args.copy()
    for file in obj_files_with_dir:
        if not os.path.isfile(file) or file[-1] == '/':
            continue
        if not checkExtension(file, ["o"]):
            continue
        options.append(file)
    options.append("-o")
    options.append(out_file)
    callCmd(toCommand("gcc", options))

def buildAndLinkDir(path: str, out_path: str, out_file: str, extra_comp_args=[], extra_link_args=[]):
    buildDir(path, out_path, extra_comp_args)
    linkDir(out_path+'/'+path, out_file, extra_link_args)
    return True

def removeUnecessary(object_dir, src_dir):
    obj_files_with_dir = glob.glob(object_dir+'/**', recursive=True)
    obj_files: list[str] = []
    for file in obj_files_with_dir:
        if not os.path.isfile(file) or file[-1] == '/':
            continue
        if not checkExtension(file, ["o"]):
            continue
        obj_files.append(file)
    src_files_with_dir = glob.glob(src_dir+'/**', recursive=True)
    src_files = []
    for file in src_files_with_dir:
        if not os.path.isfile(file) or file[-1] == '/':
            continue
        if not checkExtension(file, ["asm", "c", "lx", "cc", "cpp"]):
            continue
        file = file.removeprefix("./")
        src_files.append(file)
    for obj_file in obj_files:
        # new_obj_file = obj_file.removeprefix('./')
        new_obj_file = obj_file.removeprefix(out_dir+'/')
        new_obj_file = new_obj_file.removesuffix(".o")
        if new_obj_file not in src_files:
            print(f"Removing unused file {obj_file}")
            callCmd(f"rm -rf {obj_file}")
            callCmd(f"rm -rf /tmp/{basename}/cache/{new_obj_file}")

def main():
    global out_dir
    out_dir = sys.argv[sys.argv.index("-o")+1]
    if "clean" in sys.argv:
        callCmd(f"rm -rf {out_dir}")
        callCmd(f"rm -rf /tmp/{basename}")
    if "exit" in sys.argv:
        exit(0)
    callCmd(f"mkdir -p {out_dir}")
    removeUnecessary(out_dir, "./assembler/src")
    print("[ ] Building assembler...")
    if buildAndLinkDir("./assembler/src", out_dir, out_dir+'/assembler.exe', ['-I./assembler/include']):
        print(f"[{ANSI_COLOR_BRIGHT_GREEN}X{ANSI_COLOR_RESET}] Building assembler")
    else:
        print(f"[{ANSI_COLOR_RED}X{ANSI_COLOR_RESET}] Building assembler")

if __name__ == '__main__':
    main()