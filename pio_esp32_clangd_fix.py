import json
import subprocess
from pathlib import Path
from typing import List, Tuple

# Constants
PROJECT_ROOT: Path = Path.cwd()
SRC_FILE: Path = PROJECT_ROOT / "src" / "main.cpp"
CCDB_ORIG: Path = PROJECT_ROOT / "compile_commands.json"
CCDB_BAK: Path = PROJECT_ROOT / "compile_commands.json.bak"
CCLS_FILE: Path = PROJECT_ROOT / ".ccls"
CCDB_NEW: Path = PROJECT_ROOT / "compile_commands.json"

KEEP_FLAGS: Tuple[str, ...] = ("-I", "-D", "-std", "-Wall")


def ensure_compile_commands() -> None:
    if not CCDB_ORIG.exists():
        subprocess.run(["pio", "run", "-t", "compiledb"], check=True)
    if not CCDB_BAK.exists():
        CCDB_ORIG.rename(CCDB_BAK)


def extract_compiler() -> str:
    ccdb_data: List[dict[str, str]] = json.loads(CCDB_BAK.read_text())
    if not ccdb_data:
        raise RuntimeError("Empty compile_commands.json.bak")
    cmd: str = ccdb_data[0]["command"]
    compiler: str = cmd.split()[0]
    return compiler


def parse_ccls() -> List[str]:
    """
    Parses .ccls file and extracts relevant compiler flags.

    Returns:
        List[str]: Filtered compiler flags that match KEEP_FLAGS or are "-Wall"
    """
    common_flags: List[str] = []
    cpp_flags: List[str] = []
    with open(CCLS_FILE) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if line.startswith("%cpp"):
                cpp_flags.extend(
                    [
                        flag
                        for flag in line.split()[1:]
                        if flag == "-Wall" or flag.startswith(KEEP_FLAGS)
                    ]
                )
            elif line.startswith("%c"):
                pass  # skip C files for now
            elif not line.startswith("%"):
                common_flags.extend(
                    [
                        flag
                        for flag in line.split()
                        if flag == "-Wall" or flag.startswith(KEEP_FLAGS)
                    ]
                )
    return common_flags + cpp_flags


def generate_single_entry_compile_command(compiler: str, flags: List[str]) -> int:
    entry: dict[str, str] = {
        "directory": str(PROJECT_ROOT),
        "file": str(SRC_FILE),
        "command": f"{compiler} {' '.join(flags)} {SRC_FILE}",
    }
    CCDB_NEW.write_text(json.dumps([entry], indent=2))
    return 1


# Execution
ensure_compile_commands()
compiler_path: str = extract_compiler()
filtered_flags: List[str] = parse_ccls()
entry_count: int = generate_single_entry_compile_command(compiler_path, filtered_flags)

entry_count  # type: ignore
