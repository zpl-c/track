#include "track.h"

#define ZPL_IMPL
#define ZPL_NANO
#define ZPL_STATIC_LIB
#define ZPL_ENABLE_PARSER
#include "zpl.h"

typedef struct {
    track_event_proc *event_proc;
    track_module_unregister_proc *unregister_proc;
    zpl_array(zpl_string) allowlist;
    zpl_array(zpl_string) denylist;
    void *user_data;
} track_module;

typedef struct {
    zpl_array(track_module) modules;
} track_ctx;

static track_ctx ctx;

//~ Lifecycle methods
int track_init(void) {
    zpl_zero_item(&ctx);
    
    zpl_array_init(ctx.modules, zpl_heap_allocator());
    return 0;
}
int track_destroy(void) {
    if (!ctx.modules) return -1;
    
    for (int i = 0; i < zpl_array_count(ctx.modules); i++) {
        track_module *module = &ctx.modules[i];
        
        if (module->unregister_proc)
            module->unregister_proc(module->user_data);
        
        // TODO(zaklaus): clean up filters
    } 
    
    zpl_array_free(ctx.modules);
    ctx.modules = NULL;
    
    return 0;
}

//~ Event tracking
int track_event(char const *event_id, char const *user_id, char const *data) {
    if (!ctx.modules) return -1;
    
#ifndef TRACK_DISABLE_VALIDATION
    zpl_string json_data = zpl_string_make(zpl_heap_allocator(), data);
    zpl_json_object validated_data;
    zpl_json_error err = zpl_json_parse(&validated_data, json_data, zpl_heap_allocator());
    zpl_json_free(&validated_data);
    zpl_string_free(json_data);
    
    if (err != ZPL_JSON_ERROR_NONE)
        return -2;
#endif
    
    for (int i = 0; i < zpl_array_count(ctx.modules); i++) {
        track_module *module = &ctx.modules[i];
        
        // TODO(zaklaus): check if event can pass our filters
        
        module->event_proc(event_id, user_id, data, module->user_data);
    }
    
    return 0;
}

//~ Module registration methods
int track_module_register(track_event_proc *event_proc, track_module_unregister_proc *unregister_proc, void *user_data) {
    ZPL_ASSERT_NOT_NULL(ctx.modules);
    ZPL_ASSERT_NOT_NULL(event_proc);
    
    track_module module;
    module.event_proc = event_proc;
    module.unregister_proc = unregister_proc;
    module.allowlist = module.denylist = 0;
    module.user_data = user_data;
    zpl_array_append(ctx.modules, module);
    
    return (zpl_array_count(ctx.modules) - 1);
}
int track_module_filter(int module_id, char const **allowlist, int allowlist_len, char const **denylist, int denylist_len) {
    // TODO(zaklaus): implement this feature
    (void)module_id;
    (void)allowlist;
    (void)denylist;
    (void)allowlist_len;
    (void)denylist_len;
    return 0;
}
