#!/bin/python3
import os, sys, glob, subprocess
basename = os.path.basename(os.path.dirname(os.path.realpath(__file__)))
out_dir = ""
changed = False
changedFiles = 0
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
    with open(os.path.dirname(os.path.realpath(__file__))+"/commands.txt", "a+") as f:
        f.write(command+'\n')
    # Run the command and capture the output
    result = subprocess.run(command, capture_output=False, text=True, shell=True)
    return [result.returncode]

callCmd("rm -f commands.txt")
def main():
    global out_dir
    out_dir = sys.argv[sys.argv.index("-o")+1]
    if "clean" in sys.argv:
        callCmd(f"rm -rf {out_dir}")
        callCmd(f"rm -rf /tmp/{basename}")
    if "exit" in sys.argv:
        exit(0)
    callCmd(f"mkdir -p {out_dir}")
    os.chdir("./assembler")
    callCmd(f"python3 build.py -o ../{out_dir}")
    os.chdir("../")

if __name__ == '__main__':
    main()