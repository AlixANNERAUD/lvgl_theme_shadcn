import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
IMPORTER = os.path.join(ROOT, "scripts", "import-theme.py")
HERE = os.path.dirname(os.path.abspath(__file__))
FIXTURES = {
    "neutral-v4.css": ("neutral", True),
    "shadcn-v3-hsl.css": ("slate_v3", True),
    "tweakcn-export.css": ("tweak", False),
}
EXPECTED_HEX = {
    "shadcn-v3-hsl.css": {
        "light": {"background": 0xffffff, "foreground": 0x020817, "primary": 0x0f172a, "primary_fg": 0xf8fafc,
                  "muted_fg": 0x64748b, "destructive": 0xef4444, "border": 0xe2e8f0, "ring": 0x020817},
        "dark": {"background": 0x020817, "foreground": 0xf8fafc, "primary": 0xf8fafc, "secondary": 0x1e293b,
                 "muted_fg": 0x94a3b8, "destructive": 0x7f1d1d, "ring": 0xcbd5e1},
    },
    "tweakcn-export.css": {
        "light": {"background": 0xfafafa, "foreground": 0x171717, "primary": 0x7c3bed, "primary_fg": 0xfafafa,
                  "destructive": 0xe7000b, "border": 0xe1e1e1, "input": 0xe4e4e7, "chart0": 0x8b5cf6, "chart4": 0xef4444},
        "dark": {"background": 0x0c0a14, "primary": 0x9767e4, "destructive": 0xff6467, "ring": 0x8b5cf6,
                 "chart0": 0xa78bfa},
    },
}
failures = []


def check(condition, message):
    if not condition:
        failures.append(message)


def run_importer(*args):
    return subprocess.run([sys.executable, IMPORTER] + list(args), stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE, stdin=subprocess.DEVNULL, universal_newlines=True)


def parse_values(text, name, mode):
    block = re.search(r"static const lv_theme_shadcn_palette_t %s_%s = \{(.*?)\n\};" % (name, mode), text, re.DOTALL)
    if block is None:
        return None
    body = block.group(1)
    values = {}
    for field, value in re.findall(r"\.(\w+) = \w+\(0x([0-9a-f]{6})\)", body):
        values[field] = int(value, 16)
    chart = re.search(r"\.chart = \{(.*?)\}", body)
    for index, value in enumerate(re.findall(r"0x([0-9a-f]{6})", chart.group(1))):
        values["chart%d" % index] = int(value, 16)
    return values


def main():
    with tempfile.TemporaryDirectory() as workdir:
        for fixture, (name, both) in sorted(FIXTURES.items()):
            result = run_importer(os.path.join(HERE, fixture), "--name", name)
            check(result.returncode == 0, "%s: importer failed: %s" % (fixture, result.stderr))
            output = os.path.join(workdir, name + ".c")
            with open(output, "w", encoding="utf-8") as handle:
                handle.write(result.stdout)
            light = parse_values(result.stdout, name, "light")
            dark = parse_values(result.stdout, name, "dark")
            check(light is not None and dark is not None, "%s: missing mode in output" % fixture)
            if light is None or dark is None:
                continue
            check(len(light) == 24 and len(dark) == 24, "%s: incomplete palette" % fixture)
            if fixture == "neutral-v4.css":
                check(result.stderr == "", "neutral: unexpected warnings %r" % result.stderr)
                check(light["primary"] == 0x171717 and dark["border"] == 0x222222, "neutral values")
            if fixture == "shadcn-v3-hsl.css":
                check("chart-1" in result.stderr, "v3: expected chart warning")
            for mode, values in (("light", light), ("dark", dark)):
                for field, value in EXPECTED_HEX.get(fixture, {}).get(mode, {}).items():
                    check(values[field] == value, "%s %s.%s: %06x != %06x" % (fixture, mode, field, values[field], value))
        empty = subprocess.run([sys.executable, IMPORTER], input="", stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                               universal_newlines=True)
        check(empty.returncode == 1, "empty input should fail")
        only = subprocess.run([sys.executable, IMPORTER, "--name", "solo"], input=":root { --background: #fff; --foreground: #000; }",
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        check(only.returncode == 0 and "solo_light" in only.stdout and "solo_dark" not in only.stdout, "light-only output")
        rust = run_importer(os.path.join(HERE, "tweakcn-export.css"), "--lang", "rust", "--name", "tweak")
        check(rust.returncode == 0 and "pub fn tweak_light() -> Palette" in rust.stdout and "pub fn tweak_dark() -> Palette" in rust.stdout,
              "rust output")
    for failure in failures:
        sys.stderr.write("FAIL: %s\n" % failure)
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
