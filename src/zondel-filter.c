/* License: GPL-2.0-or-later. Copyright (c) 2026 Zondel.
 *
 * Skeleton: registers the filter, accepts/returns audio unchanged. Task 14
 * fills in the adaptation pipeline; Task 15 fills in the properties UI.
 */
#include <obs-module.h>
#include <plugin-support.h>
#include "zondel-filter.h"
#include "zondel-state.h"
#include "pipe-client.h"

typedef struct zondel_filter {
    obs_source_t   *source;
    pipe_client_t  *pipe;
    zondel_state_t  state;
    char            pipe_name[256];
    uint32_t        timeout_us;
    float          *scratch_send;
    float          *scratch_recv;
    size_t          scratch_capacity;
} zondel_filter_t;

static const char *zondel_get_name(void *type_data) {
    (void)type_data;
    return obs_module_text("Zondel.Filter.Name");
}

static void *zondel_create(obs_data_t *settings, obs_source_t *source) {
    zondel_filter_t *f = bzalloc(sizeof(*f));
    f->source = source;
    zondel_state_init(&f->state);

    const char *ep = obs_data_get_string(settings, "pipe_endpoint");
    if (!ep || !*ep) ep = "\\\\.\\pipe\\Zondel";
    strncpy(f->pipe_name, ep, sizeof(f->pipe_name) - 1);

    int timeout_ms = (int)obs_data_get_int(settings, "pipe_timeout_ms");
    if (timeout_ms < 1) timeout_ms = 5;
    if (timeout_ms > 20) timeout_ms = 20;
    f->timeout_us = (uint32_t)timeout_ms * 1000;

    f->pipe = pipe_client_create(f->pipe_name);
    f->scratch_capacity = 480;
    f->scratch_send = bzalloc(f->scratch_capacity * sizeof(float));
    f->scratch_recv = bzalloc(f->scratch_capacity * sizeof(float));

    zondel_state_set_bypass(&f->state, obs_data_get_bool(settings, "bypass") ? 1 : 0);
    return f;
}

static void zondel_destroy(void *data) {
    zondel_filter_t *f = data;
    if (!f) return;
    if (f->pipe) pipe_client_destroy(f->pipe);
    bfree(f->scratch_send);
    bfree(f->scratch_recv);
    bfree(f);
}

static void zondel_get_defaults(obs_data_t *s) {
    obs_data_set_default_string(s, "pipe_endpoint", "\\\\.\\pipe\\Zondel");
    obs_data_set_default_int(s, "pipe_timeout_ms", 5);
    obs_data_set_default_bool(s, "bypass", false);
}

static struct obs_audio_data *zondel_filter_audio(void *data, struct obs_audio_data *audio) {
    zondel_filter_t *f = data;
    if (!f) return audio;
    (void)audio; /* hot path implemented in task 14 */
    return audio;
}

static obs_properties_t *zondel_get_properties(void *data) {
    (void)data;
    obs_properties_t *p = obs_properties_create();
    obs_properties_add_bool(p, "bypass", obs_module_text("Zondel.Property.Bypass"));
    return p;
}

static void zondel_update(void *data, obs_data_t *settings) {
    zondel_filter_t *f = data;
    if (!f) return;
    zondel_state_set_bypass(&f->state, obs_data_get_bool(settings, "bypass") ? 1 : 0);
}

struct obs_source_info zondel_filter_info = {
    .id            = "zondel_audio_filter",
    .type          = OBS_SOURCE_TYPE_FILTER,
    .output_flags  = OBS_SOURCE_AUDIO,
    .get_name      = zondel_get_name,
    .create        = zondel_create,
    .destroy       = zondel_destroy,
    .update        = zondel_update,
    .get_defaults  = zondel_get_defaults,
    .get_properties = zondel_get_properties,
    .filter_audio  = zondel_filter_audio,
};
