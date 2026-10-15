import colorsys
import math
import re

OKLCH = re.compile(r"oklch\(\s*([0-9.]+)\s+([0-9.]+)\s+([0-9.]+)\s*(?:/\s*([0-9.]+)(%?))?\s*\)")

NUMBER = r"[-+]?(?:\d+\.?\d*|\.\d+)(?:e[-+]?\d+)?"
FUNCTION = re.compile(r"^(oklch|hsla?|rgba?)\(\s*(.*?)\s*\)$", re.IGNORECASE | re.DOTALL)
BARE_HSL = re.compile(r"^(%s)(?:deg)?[\s,]+(%s)%%[\s,]+(%s)%%(?:\s*/\s*(%s%%?))?$" % ((NUMBER,) * 4), re.IGNORECASE)
HEX = re.compile(r"^#([0-9a-f]{3,4}|[0-9a-f]{6}|[0-9a-f]{8})$", re.IGNORECASE)


def parse(value):
    match = OKLCH.fullmatch(value.strip())
    if match is None:
        raise ValueError("unsupported colour %r" % value)
    alpha = 1.0
    if match.group(4) is not None:
        alpha = float(match.group(4)) / (100.0 if match.group(5) else 1.0)
    return float(match.group(1)), float(match.group(2)), float(match.group(3)), alpha


def to_linear(lightness, chroma, hue):
    a = chroma * math.cos(math.radians(hue))
    b = chroma * math.sin(math.radians(hue))
    l = (lightness + 0.3963377774 * a + 0.2158037573 * b) ** 3
    m = (lightness - 0.1055613458 * a - 0.0638541728 * b) ** 3
    s = (lightness - 0.0894841775 * a - 1.2914855480 * b) ** 3
    return (
        4.0767416621 * l - 3.3077115913 * m + 0.2309699292 * s,
        -1.2684380046 * l + 2.6097574011 * m - 0.3413193965 * s,
        -0.0041960863 * l - 0.7034186147 * m + 1.7076147010 * s,
    )


def encode(channel):
    channel = min(max(channel, 0.0), 1.0)
    if channel <= 0.0031308:
        return 12.92 * channel
    return 1.055 * channel ** (1.0 / 2.4) - 0.055


def oklch_bytes(lightness, chroma, hue):
    return tuple(int(round(encode(c) * 255.0)) for c in to_linear(lightness, chroma, hue))


def to_hex(value):
    lightness, chroma, hue, alpha = parse(value)
    if alpha != 1.0:
        raise ValueError("translucent colour %r needs an opaque substitute" % value)
    return "".join("%02x" % channel for channel in oklch_bytes(lightness, chroma, hue))


def clamp_byte(value):
    return int(round(min(max(value, 0.0), 255.0)))


def split_arguments(body):
    body = body.replace(",", " ")
    alpha = None
    if "/" in body:
        body, alpha = body.split("/", 1)
    parts = body.split()
    if alpha is None and len(parts) == 4:
        alpha = parts.pop()
    return parts, alpha


def alpha_value(token):
    if token is None:
        return 1.0
    token = token.strip()
    if token.endswith("%"):
        value = float(token[:-1]) / 100.0
    else:
        value = float(token)
    return min(max(value, 0.0), 1.0)


def number(token, percent_scale=None):
    token = token.strip()
    if token.lower() == "none":
        return 0.0
    if token.endswith("%"):
        if percent_scale is None:
            return float(token[:-1])
        return float(token[:-1]) / 100.0 * percent_scale
    return float(re.sub(r"(deg|turn|rad|grad)$", "", token, flags=re.IGNORECASE))


def hue_degrees(token):
    token = token.strip().lower()
    value = float(re.sub(r"(deg|turn|rad|grad)$", "", token))
    if token.endswith("turn"):
        value *= 360.0
    elif token.endswith("rad"):
        value = math.degrees(value)
    elif token.endswith("grad"):
        value *= 0.9
    return value % 360.0


def parse_oklch(body):
    parts, alpha = split_arguments(body)
    if len(parts) != 3:
        raise ValueError("bad oklch")
    lightness = number(parts[0], 1.0)
    chroma = number(parts[1], 0.4)
    hue = 0.0 if parts[2].lower() == "none" else hue_degrees(parts[2])
    return oklch_bytes(lightness, chroma, hue), alpha_value(alpha)


def parse_hsl(body):
    parts, alpha = split_arguments(body)
    if len(parts) != 3:
        raise ValueError("bad hsl")
    hue = 0.0 if parts[0].lower() == "none" else hue_degrees(parts[0])
    saturation = min(max(number(parts[1], 1.0) if parts[1].endswith("%") else number(parts[1]) / 100.0, 0.0), 1.0)
    lightness = min(max(number(parts[2], 1.0) if parts[2].endswith("%") else number(parts[2]) / 100.0, 0.0), 1.0)
    red, green, blue = colorsys.hls_to_rgb(hue / 360.0, lightness, saturation)
    return tuple(clamp_byte(c * 255.0) for c in (red, green, blue)), alpha_value(alpha)


def parse_rgb(body):
    parts, alpha = split_arguments(body)
    if len(parts) != 3:
        raise ValueError("bad rgb")
    channels = tuple(clamp_byte(number(p, 255.0)) for p in parts)
    return channels, alpha_value(alpha)


def parse_hex(digits):
    if len(digits) in (3, 4):
        digits = "".join(c * 2 for c in digits)
    channels = tuple(int(digits[i:i + 2], 16) for i in (0, 2, 4))
    alpha = int(digits[6:8], 16) / 255.0 if len(digits) == 8 else 1.0
    return channels, alpha


def parse_css_color(value):
    text = value.strip().rstrip(";").strip()
    text = re.sub(r"\s*!important$", "", text)
    if text.lower() == "transparent":
        return (0, 0, 0), 0.0
    match = HEX.match(text)
    if match:
        return parse_hex(match.group(1))
    match = FUNCTION.match(text)
    if match:
        kind = match.group(1).lower()
        try:
            if kind == "oklch":
                return parse_oklch(match.group(2))
            if kind.startswith("hsl"):
                return parse_hsl(match.group(2))
            return parse_rgb(match.group(2))
        except ValueError:
            raise ValueError("unsupported colour %r" % value)
    match = BARE_HSL.match(text)
    if match:
        body = "%s %s%% %s%%" % match.group(1, 2, 3)
        if match.group(4) is not None:
            body += " / " + match.group(4)
        return parse_hsl(body)
    raise ValueError("unsupported colour %r" % value)


def composite(foreground, alpha, background):
    return tuple(clamp_byte(f * alpha + b * (1.0 - alpha)) for f, b in zip(foreground, background))


def hex_string(channels):
    return "".join("%02x" % channel for channel in channels)
