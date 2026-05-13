/* License: GPL-2.0-or-later. Copyright (c) 2026 Zondel.
 * Win32 named-pipe client. Filled in across tasks 6-9.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>

struct pipe_client {
    char  endpoint[256];
    HANDLE handle;
    int    state;
};

pipe_client_t *pipe_client_create(const char *endpoint) {
    pipe_client_t *c = (pipe_client_t *)calloc(1, sizeof(*c));
    if (!c) return NULL;
    strncpy(c->endpoint, endpoint ? endpoint : "\\\\.\\pipe\\Zondel", sizeof(c->endpoint) - 1);
    c->handle = INVALID_HANDLE_VALUE;
    c->state  = PIPE_DISCONNECTED;
    return c;
}

void pipe_client_destroy(pipe_client_t *c) {
    if (!c) return;
    if (c->handle != INVALID_HANDLE_VALUE) CloseHandle(c->handle);
    free(c);
}

int pipe_client_send_recv(pipe_client_t *c,
                          const float *send_interleaved, float *recv_interleaved,
                          uint32_t frames, uint32_t sample_rate, uint16_t channels,
                          uint32_t timeout_us) {
    (void)c; (void)send_interleaved; (void)recv_interleaved;
    (void)frames; (void)sample_rate; (void)channels; (void)timeout_us;
    return PIPE_ERR_NOT_CONNECTED;  /* filled in tasks 6-9 */
}

int pipe_client_state(const pipe_client_t *c) {
    return c ? c->state : PIPE_DISCONNECTED;
}
