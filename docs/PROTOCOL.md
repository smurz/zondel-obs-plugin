# Zondel Bridge Pipe Protocol

> Version: 1 (frozen at Zondel build 80; bumped by Zondel side, mirrored here)
> Transport: Windows named pipe, `\\.\pipe\Zondel`
> Encoding: little-endian, byte-aligned, no framing layer

## Connection

Open the pipe with `CreateFileA("\\\\.\\pipe\\Zondel", GENERIC_READ | GENERIC_WRITE,
0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL)`. The server allows up to 8
concurrent clients.

`ERROR_FILE_NOT_FOUND` means Zondel is not running.
`ERROR_PIPE_BUSY` means all 8 client slots are in use.

## Request frame

Each round-trip sends a request frame, then reads a response frame.

```
+------+------+------+------+----------------+----------+
|  4   |  4   |  2   |  N   | (rest is data) |          |
+------+------+------+------+----------------+----------+
| frames  | sr  | ch  | float32 PCM payload            |
+------+------+------+------+----------------+----------+
```

| Field    | Bytes | Type    | Notes                                          |
|----------|-------|---------|------------------------------------------------|
| frames   | 4     | uint32  | samples per channel; 1 .. 48000                |
| sr       | 4     | uint32  | sample rate Hz; 8000 .. 48000                  |
| ch       | 2     | uint16  | channel count; 1 or 2                          |
| payload  | F*C*4 | float32 | interleaved if ch=2, otherwise straight        |

Total request size = 10 + F*C*4 bytes.

## Response frame

```
+------+----+----------------+
|  4   | 1  |     payload    |
+------+----+----------------+
| frames | st | float32 PCM  |
+------+----+----------------+
```

| Field    | Bytes | Type    | Notes                                          |
|----------|-------|---------|------------------------------------------------|
| frames   | 4     | uint32  | echoed frame count                             |
| st       | 1     | uint8   | status; 0 = ok                                 |
| payload  | F*C*4 | float32 | processed audio, same shape as request         |

If the response's echoed `frames` doesn't match the request, the protocol has
desynchronised; the client must close and reopen the pipe.

## Server behaviour notes (v1)

- The server's DSP pipeline processes **48 kHz / 1 channel / 480 sample** frames.
  Requests in any other shape are echoed unchanged (effective pass-through).
- The server is single-threaded per client but multi-client.
- Read timeouts are caller-side. The server has no idle timeout.

## Versioning

This document is frozen at Zondel build 80. If Zondel introduces a non-backward
compatible change (different header layout, new status codes, etc.), the new
version of this file will document it and the plugin's `buildspec.json` will
bump the major version accordingly.
