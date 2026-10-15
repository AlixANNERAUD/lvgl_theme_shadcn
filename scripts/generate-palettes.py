import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from shadcn_colors import parse, to_hex

BASES = ["neutral", "stone", "zinc", "gray", "slate", "mauve", "olive", "mist", "taupe"]
LEGACY_BASES = {"gray", "slate"}
ACCENTS = [
    "amber", "blue", "cyan", "emerald", "fuchsia", "green", "indigo", "lime", "orange",
    "pink", "purple", "red", "rose", "sky", "teal", "violet", "yellow",
]
LEGACY_INPUT_DARK = {
    "gray": "oklch(0.373 0.034 259.733)",
    "slate": "oklch(0.372 0.044 257.287)",
}
PALETTE_FIELDS = [
    ("background", "background"),
    ("foreground", "foreground"),
    ("card", "card"),
    ("card_fg", "card-foreground"),
    ("popover", "popover"),
    ("popover_fg", "popover-foreground"),
    ("primary", "primary"),
    ("primary_fg", "primary-foreground"),
    ("secondary", "secondary"),
    ("secondary_fg", "secondary-foreground"),
    ("muted", "muted"),
    ("muted_fg", "muted-foreground"),
    ("accent", "accent"),
    ("accent_fg", "accent-foreground"),
    ("destructive", "destructive"),
    ("destructive_fg", "destructive-foreground"),
    ("border", "border"),
    ("input", "input"),
    ("ring", "ring"),
]
ACCENT_FIELDS = [
    ("primary", "primary"),
    ("primary_fg", "primary-foreground"),
    ("secondary", "secondary"),
    ("secondary_fg", "secondary-foreground"),
]
CHARTS = ["chart-1", "chart-2", "chart-3", "chart-4", "chart-5"]
ENTRY = re.compile(r'^\s*"?([A-Za-z0-9-]+)"?\s*:\s*"([^"]*)"', re.MULTILINE)


def block_after(text, start):
    open_index = text.index("{", start)
    depth = 0
    for index in range(open_index, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[open_index + 1:index]
    raise ValueError("unbalanced braces")


def entries(body):
    return {key: value for key, value in ENTRY.findall(body)}


def modes(body):
    return {
        "light": entries(block_after(body, re.search(r"\blight\s*:\s*\{", body).start())),
        "dark": entries(block_after(body, re.search(r"\bdark\s*:\s*\{", body).start())),
    }


def theme_vars(text, name):
    match = re.search(r'name:\s*"%s"' % re.escape(name), text)
    if match is None:
        raise ValueError("theme %s not found" % name)
    css = re.compile(r"cssVars\s*:\s*\{").search(text, match.end())
    return modes(block_after(text, css.start()))


def legacy_vars(text, name):
    root = re.search(r"baseColorsV4\s*=\s*\{", text)
    body = block_after(text, root.start())
    match = re.search(r"^\s*%s\s*:\s*\{" % re.escape(name), body, re.MULTILINE)
    if match is None:
        raise ValueError("legacy base %s not found" % name)
    return modes(block_after(body, match.start()))


def opaque(name, base_dark, mode_vars, key):
    value = mode_vars[key]
    if parse(value)[3] == 1.0:
        return value
    if key == "border":
        return mode_vars["muted"]
    if key == "input":
        return LEGACY_INPUT_DARK.get(name, base_dark["chart-4"])
    raise ValueError("translucent %s in %s" % (key, name))


def base_palette(name, all_vars, mode):
    mode_vars = dict(all_vars[mode])
    mode_vars["destructive-foreground"] = all_vars["dark"]["foreground"]
    for key in ("border", "input"):
        mode_vars[key] = opaque(name, all_vars["dark"], mode_vars, key)
    fields = [(field, to_hex(mode_vars[key])) for field, key in PALETTE_FIELDS]
    charts = [to_hex(mode_vars[key]) for key in CHARTS]
    return fields, charts


def accent_set(all_vars, mode):
    mode_vars = all_vars[mode]
    fields = [(field, to_hex(mode_vars[key])) for field, key in ACCENT_FIELDS]
    charts = [to_hex(mode_vars[key]) for key in CHARTS]
    return fields, charts


def hexes(values):
    return ", ".join("HEX(0x%s)" % value for value in values)


def render(registry):
    with open(os.path.join(registry, "themes.ts"), encoding="utf-8") as handle:
        themes = handle.read()
    with open(os.path.join(registry, "_legacy-base-colors.ts"), encoding="utf-8") as handle:
        legacy = handle.read()
    out = [
        "#include \"lv_theme_shadcn_private.h\"",
        "",
        "#define HEX(c) LV_COLOR_MAKE(((c) >> 16) & 0xff, ((c) >> 8) & 0xff, (c) & 0xff)",
        "",
        "const lv_theme_shadcn_palette_t lv_theme_shadcn_base_palettes[LV_THEME_SHADCN_BASE_COUNT][2] = {",
    ]
    for name in BASES:
        all_vars = legacy_vars(legacy, name) if name in LEGACY_BASES else theme_vars(themes, name)
        out.append("    {")
        for mode in ("light", "dark"):
            fields, charts = base_palette(name, all_vars, mode)
            out.append("        {")
            for field, value in fields:
                out.append("            .%s = HEX(0x%s)," % (field, value))
            out.append("            .chart = { %s }," % hexes(charts))
            out.append("            .dark = %s," % ("true" if mode == "dark" else "false"))
            out.append("        },")
        out.append("    },")
    out.append("};")
    out.append("")
    out.append("const lv_theme_shadcn_accent_colors_t lv_theme_shadcn_accent_palettes[LV_THEME_SHADCN_ACCENT_COUNT - 1][2] = {")
    for name in ACCENTS:
        all_vars = theme_vars(themes, name)
        out.append("    {")
        for mode in ("light", "dark"):
            fields, charts = accent_set(all_vars, mode)
            values = ", ".join(".%s = HEX(0x%s)" % pair for pair in fields)
            out.append("        { %s," % values)
            out.append("          .chart = { %s } }," % hexes(charts))
        out.append("    },")
    out.append("};")
    out.append("")
    out.append("const char * const lv_theme_shadcn_base_names[LV_THEME_SHADCN_BASE_COUNT] = {")
    out.append("    %s," % ", ".join('"%s"' % name for name in BASES))
    out.append("};")
    out.append("")
    out.append("const char * const lv_theme_shadcn_accent_names[LV_THEME_SHADCN_ACCENT_COUNT] = {")
    names = ["none"] + ACCENTS
    for start in range(0, len(names), 9):
        out.append("    %s," % ", ".join('"%s"' % name for name in names[start:start + 9]))
    out.append("};")
    return "\n".join(out) + "\n"


def main(argv):
    if len(argv) not in (2, 3):
        sys.stderr.write("usage: %s <shadcn apps/v4/registry dir> [output]\n" % argv[0])
        return 2
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    output = argv[2] if len(argv) == 3 else os.path.join(root, "src", "lv_theme_shadcn_palettes.c")
    text = render(argv[1])
    with open(output, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
