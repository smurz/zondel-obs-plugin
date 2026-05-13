# Changelog

All notable changes to this project will be documented here.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

### Documentation
- Added end-user `README.md` (install, use, troubleshooting, build-from-source).
- Added `docs/PROTOCOL.md` documenting the wire protocol.

### Internal
- Branch protection on `main` will be enabled before v0.1.0 (Task 20): require PRs, `check-format` + `build-project` checks, linear history, 1 approval on PRs touching `src/`.

## [0.1.0] - TBD-on-first-tag

### Added
- Initial beta release.
- OBS audio filter `zondel_audio_filter` (display: "Zondel Audio Processor").
- Win32 named-pipe client targeting `\\.\pipe\Zondel`.
- Properties UI: Bypass toggle, Status indicator, Open Zondel button, advanced (pipe endpoint, pipe timeout 1-20 ms default 5 ms).
- Format adaptation in the plugin: handles mono + stereo at 8/16/24/32/44.1/48/96 kHz via downmix and 4-point Catmull-Rom cubic resampler with 480-sample chunking ring buffer.
- Back-off circuit breaker on repeated failures.

### Known limitations
- Windows-only. Linux/macOS scaffolding exists as stubs but is not built.
- Sources with more than 2 channels (5.1/7.1) are passed through unprocessed (v2 will add surround downmix).
- Resampler is voice-grade cubic; if quality complaints surface in beta we may swap in Speex for v1.1.
- Installer is unsigned; SmartScreen will warn users.
