# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.1](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/compare/v0.3.0...v0.3.1) - 2026-10-15

### Documentation

- document the c api ([#8](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/pull/8))

### Miscellaneous

- pass arduino-lint for the library manager

## [0.3.0](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/compare/v0.2.0...v0.3.0) - 2026-10-15

### Breaking

- `lv_theme_shadcn_fonts_t` gains `h1`, `h2` and `h3` heading fonts at the end of the struct. Positional initialisers with four fields still compile but warn under `-Wmissing-field-initializers`; use designated initialisers (`.small`, `.normal`, `.large`, `.mono`, `.h1`, ...). In Rust, `Fonts` has the new `h1`, `h2` and `h3` fields.
- Plain containers (cards) now use shadcn's larger card radius.

### Added

- add shadcn radius setting
- add button sizes
- add hover feedback for pointer devices
- add shadcn text roles
- add badge, alert, separator, skeleton and card roles
- expose radius and sizes in the rust crate
- expose text and component roles in the rust crate
- import any shadcn theme css into a palette
- reduced motion flag to disable the skeleton pulse
- pick radius in the gallery
- use text and component roles in the demo scenes, with a new display scene

### Documentation

- document radius, sizes, roles, typography and theme import

## [0.2.0](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/compare/v0.1.1...v0.2.0) - 2026-10-15

### Added

- add every shadcn base and accent palette
- expose shadcn palettes in the rust crate
- switch base and accent live in the gallery

### Fixed

- style lines so scale needles are visible

### Documentation

- show every palette in the readme
- list lines and unstyled media widgets

## [0.1.1](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/compare/v0.1.0...v0.1.1) - 2026-10-14

### Fixed

- publish to the alix-anneraud platformio account
- keep list rows inside rounded corners
- pad tab bar evenly around the active tab
- keep the focus ring visible in the buttons demo

### Miscellaneous

- make the repository root the sys crate package

## [0.1.0](https://github.com/AlixANNERAUD/lvgl_theme_shadcn/releases/tag/v0.1.0) - 2026-10-14

### Added

- add lvgl_theme_shadcn_sys crate
- add safe lvgl_theme_shadcn crate

### Changed

- return option from theme palette and rename with_primary

### Miscellaneous

- release crates with release-plz
