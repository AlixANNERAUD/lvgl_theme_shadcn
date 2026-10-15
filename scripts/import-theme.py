import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from shadcn_colors import composite, hex_string, parse_css_color

ROLES = [
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
CHARTS = ["chart-1", "chart-2", "chart-3", "chart-4", "chart-5"]
FALLBACK_SOURCE = {
    "card": "background",
    "card_fg": "foreground",
    "popover": "card",
    "popover_fg": "card_fg",
    "primary": "foreground",
    "primary_fg": "background",
    "secondary": "card",
    "secondary_fg": "foreground",
    "muted": "secondary",
    "muted_fg": "foreground",
    "accent": "secondary",
    "accent_fg": "foreground",
    "border": "muted",
    "input": "border",
    "ring": "muted_fg",
}
ORDER = [
    "background", "foreground", "card", "card_fg", "popover", "popover_fg", "primary", "primary_fg",
    "secondary", "secondary_fg", "muted", "muted_fg", "accent", "accent_fg", "destructive",
    "destructive_fg", "border", "input", "ring",
]
DEFAULTS = {
    "light": {"background": (255, 255, 255), "foreground": (10, 10, 10), "destructive": (231, 0, 11)},
    "dark": {"background": (10, 10, 10), "foreground": (250, 250, 250), "destructive": (255, 100, 103)},
}
DECLARATION = re.compile(r"(--[A-Za-z0-9_-]+)\s*:\s*([^;{}]*?)\s*(?:;|$)")
VAR_REFERENCE = re.compile(r"^var\(\s*(--[A-Za-z0-9_-]+)\s*(?:,[^)]*)?\)$")
LIGHT_SELECTORS = {":root", "html", ":host", "*", "body"}


def warn(message):
    sys.stderr.write("warning: %s\n" % message)


def strip_comments(text):
    return re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)


def matching_brace(text, open_index):
    depth = 0
    for index in range(open_index, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return index
    raise ValueError("unbalanced braces in css")


def selector_mode(prelude):
    modes = set()
    for selector in prelude.split(","):
        selector = selector.strip()
        if selector in LIGHT_SELECTORS:
            modes.add("light")
        elif re.search(r"\.dark\b|\[data-theme=[\"']?dark[\"']?\]|\[data-mode=[\"']?dark[\"']?\]", selector):
            modes.add("dark")
    return modes


def collect(text, result):
    position = 0
    while True:
        open_index = text.find("{", position)
        if open_index < 0:
            return
        prelude = text[position:open_index].split(";")[-1].strip()
        close_index = matching_brace(text, open_index)
        body = text[open_index + 1:close_index]
        position = close_index + 1
        if prelude.startswith("@"):
            if re.match(r"@(layer|media|supports)\b", prelude):
                collect(body, result)
            continue
        for mode in selector_mode(prelude):
            for name, value in DECLARATION.findall(body):
                result[mode][name] = value


def parse_css(text):
    result = {"light": {}, "dark": {}}
    collect(strip_comments(text), result)
    return result


def resolve(variables, name, depth=0):
    value = variables.get(name)
    if value is None or depth > 8:
        return value
    match = VAR_REFERENCE.match(value.strip())
    if match:
        return resolve(variables, match.group(1), depth + 1)
    return value


def read_colour(variables, key, label):
    raw = resolve(variables, "--" + key)
    if raw is None:
        return None
    try:
        return parse_css_color(raw)
    except ValueError:
        warn("%s: cannot parse --%s: %s" % (label, key, raw))
        return None


def luminance(channels):
    return 0.2126 * channels[0] + 0.7152 * channels[1] + 0.0722 * channels[2]


def mix(first, second, amount):
    return tuple(int(round(a * (1.0 - amount) + b * amount)) for a, b in zip(first, second))


def build_palette(mode, variables, other_foreground):
    label = mode
    colours = {}
    for field, key in ROLES:
        colour = read_colour(variables, key, label)
        if colour is not None:
            colours[field] = colour
    defaults = DEFAULTS[mode]
    for field in ("background", "foreground"):
        if field not in colours:
            warn("%s: missing --%s, using default" % (label, dict(ROLES)[field]))
            colours[field] = (defaults[field], 1.0)
    background = colours["background"][0]
    channels = {}
    for field in ORDER:
        if field in colours:
            colour, alpha = colours[field]
            if alpha < 1.0:
                base = background if field != "background" else defaults["background"]
                colour = composite(colour, alpha, base)
            channels[field] = colour
            continue
        if field == "destructive":
            warn("%s: missing --destructive, using default red" % label)
            channels[field] = defaults["destructive"]
        elif field == "destructive_fg":
            if other_foreground is not None:
                channels[field] = other_foreground
            else:
                channels[field] = max([channels["foreground"], background], key=luminance)
        else:
            source = FALLBACK_SOURCE[field]
            warn("%s: missing --%s, using %s" % (label, dict(ROLES)[field], source))
            channels[field] = channels[source]
    primary = channels["primary"]
    for index, key in enumerate(CHARTS):
        colour = read_colour(variables, key, label)
        if colour is None:
            warn("%s: missing --%s, derived from primary" % (label, key))
            channels["chart%d" % index] = mix(primary, background, 0.18 * index)
        else:
            value, alpha = colour
            channels["chart%d" % index] = composite(value, alpha, background) if alpha < 1.0 else value
    return channels


def build(css):
    parsed = parse_css(css)
    light, dark = parsed["light"], parsed["dark"]
    if not light and not dark:
        raise ValueError("no :root or .dark variables found")
    palettes = {}
    foregrounds = {}
    if light:
        foregrounds["light"] = read_colour(light, "foreground", "light")
    if dark:
        merged = dict(light)
        merged.update(dark)
        foregrounds["dark"] = read_colour(merged, "foreground", "dark")
    if light:
        other = foregrounds["dark"][0] if "dark" in foregrounds and foregrounds["dark"] else None
        palettes["light"] = build_palette("light", light, other)
    if dark:
        merged = dict(light)
        merged.update(dark)
        palettes["dark"] = build_palette("dark", merged, None)
    return palettes


def identifier(name):
    name = re.sub(r"[^A-Za-z0-9_]", "_", name.strip()).strip("_").lower()
    if not name or name[0].isdigit():
        name = "theme_" + name
    return name


def role_value(channels, field):
    return "0x" + hex_string(channels[field])


def render_c(name, palettes):
    macro = name.upper() + "_HEX"
    out = [
        "#include \"lv_theme_shadcn.h\"",
        "",
        "#define %s(c) LV_COLOR_MAKE(((c) >> 16) & 0xff, ((c) >> 8) & 0xff, (c) & 0xff)" % macro,
    ]
    for mode in ("light", "dark"):
        if mode not in palettes:
            continue
        channels = palettes[mode]
        out.append("")
        out.append("static const lv_theme_shadcn_palette_t %s_%s = {" % (name, mode))
        for field, _ in ROLES:
            out.append("    .%s = %s(%s)," % (field, macro, role_value(channels, field)))
        charts = ", ".join("%s(%s)" % (macro, role_value(channels, "chart%d" % i)) for i in range(5))
        out.append("    .chart = { %s }," % charts)
        out.append("    .dark = %s," % ("true" if mode == "dark" else "false"))
        out.append("};")
    out.append("")
    out.append("#undef %s" % macro)
    return "\n".join(out) + "\n"


def render_rust(name, palettes):
    out = ["use lvgl_theme_shadcn::{Color, Palette};"]
    for mode in ("light", "dark"):
        if mode not in palettes:
            continue
        channels = palettes[mode]
        base = "zinc_dark" if mode == "dark" else "zinc_light"
        out.append("")
        out.append("pub fn %s_%s() -> Palette {" % (name, mode))
        out.append("    let mut palette = Palette::%s();" % base)
        for field, _ in ROLES:
            out.append("    palette.set_%s(Color::hex(%s));" % (field, role_value(channels, field)))
        for index in range(5):
            out.append("    palette.set_chart(%d, Color::hex(%s));" % (index, role_value(channels, "chart%d" % index)))
        out.append("    palette.set_dark(%s);" % ("true" if mode == "dark" else "false"))
        out.append("    palette")
        out.append("}")
    return "\n".join(out) + "\n"


def main(argv):
    parser = argparse.ArgumentParser(description="Convert a shadcn theme css into an LVGL shadcn palette.")
    parser.add_argument("input", nargs="?", default="-", help="css file, or - for stdin")
    parser.add_argument("--lang", choices=("c", "rust"), default="c")
    parser.add_argument("--name", default="custom")
    parser.add_argument("-o", "--output")
    args = parser.parse_args(argv[1:])
    if args.input == "-":
        css = sys.stdin.read()
    else:
        with open(args.input, encoding="utf-8") as handle:
            css = handle.read()
    try:
        palettes = build(css)
    except ValueError as error:
        sys.stderr.write("error: %s\n" % error)
        return 1
    name = identifier(args.name)
    text = render_c(name, palettes) if args.lang == "c" else render_rust(name, palettes)
    if args.output:
        with open(args.output, "w", encoding="utf-8", newline="\n") as handle:
            handle.write(text)
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
