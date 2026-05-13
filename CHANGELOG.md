# Changelog

All notable changes to this project will be documented here.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.1.0-beta.2] - 2026-05-13

Patch beta. The `v0.1.0-beta.1` tag was published but its release-packaging
workflow failed (artifact-download pattern referenced `displayName` instead
of `name`, so no artifacts were downloaded). No GitHub Release was created
for beta.1; this beta supersedes it.

### Fixed
- `release.yaml`: artifact-pattern uses literal repo name `zondel-obs-plugin`
  not the `displayName` output, and added a defensive guard with a clearer
  error if the pattern matches nothing.

## [0.1.0-beta.1] - 2026-05-13

First public beta. The plugin's source ID (`zondel_audio_filter`) is **not yet
frozen** during the v0.x series — scene-collection compatibility guarantees
begin at v1.0.0.

### Added
- OBS audio filter `zondel_audio_filter` (display: "Zondel Audio Processor").
- Win32 named-pipe client targeting `\\.\pipe\Zondel` with overlapped I/O,
  5 ms hard timeout, close-and-reconnect on timeout.
- Properties UI: Bypass toggle, Status indicator, Open Zondel button, Advanced
  group (pipe endpoint, pipe timeout 1-20 ms, default 5 ms).
- Format adaptation in the plugin: handles mono + stereo at 8/16/24/32/44.1/48/96 kHz
  via downmix and 4-point Catmull-Rom cubic resampler with 480-sample chunking
  ring buffer.
- Back-off circuit breaker: 3 consecutive pipe failures → 2 s pass-through window.
- End-user `README.md` (install, use, troubleshooting, build-from-source).
- Public `docs/PROTOCOL.md` documenting the wire protocol.
- Inno Setup installer (`zondel-obs-plugin-vX.Y.Z-setup.exe`).
- GitHub Actions release workflow producing `.zip`, `.exe`, and `SHA256SUMS.txt`
  on every `v*.*.*` tag.
- Branch protection on `main`: PR-required, `check-format` + `build-project`
  must pass, linear history, 1 approval on PRs touching `src/`.

### Known limitations
- Windows-only. Linux/macOS scaffolding exists as stubs but is not built.
- Sources with more than 2 channels (5.1/7.1) are passed through unprocessed
  (v2 will add surround downmix).
- Resampler is voice-grade cubic; if quality complaints surface in beta we may
  swap in Speex for v1.1.
- Installer is **unsigned**; SmartScreen will warn users.
- ARM64 Windows builds are best-effort (matrix `continue-on-error`); hardened
  in v1.1.
