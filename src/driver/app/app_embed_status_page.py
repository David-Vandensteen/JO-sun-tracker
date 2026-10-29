#!/usr/bin/env python3
"""Embed app/app_status_page.html into driver/app_status_page.h as a PROGMEM C++ raw string.

Run after editing app_status_page.html:  python3 src/driver/app/app_embed_status_page.py
"""
from pathlib import Path

WEB_DIR = Path(__file__).resolve().parent
SOURCE = WEB_DIR / "app_status_page.html"
TARGET = WEB_DIR.parent / "app_status_page.h"
DELIMITER = "HTML"
END_MARKER = f"){DELIMITER}\""


def main() -> None:
    html = SOURCE.read_text(encoding="utf-8")
    if END_MARKER in html:
        raise SystemExit(f"{SOURCE.name} must not contain '{END_MARKER}'")

    header = (
        "// Generated from app/app_status_page.html by app/app_embed_status_page.py. Do not edit.\n"
        "#ifndef STATUS_PAGE_H\n"
        "#define STATUS_PAGE_H\n"
        "#include <Arduino.h>\n\n"
        f'static const char STATUS_PAGE[] PROGMEM = R"{DELIMITER}({html}){DELIMITER}";\n\n'
        "#endif\n"
    )
    TARGET.write_text(header, encoding="utf-8")
    print(f"Wrote {TARGET}")


if __name__ == "__main__":
    main()
