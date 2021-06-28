#pragma once
// requires zpl to be included first

typedef struct {
    zpl_file f;
    int len;
    char *text;
} track_escaped_string;

static inline track_escaped_string track_escape_string(char const *text, char const *escape_symbols, char escape_char) {
    zpl_adt_node node;
    zpl_adt_set_str(&node, 0, text);
    zpl_file tmp;
    zpl_file_stream_new(&tmp, zpl_heap_allocator());
    zpl_adt_print_string(&tmp, &node, escape_symbols, escape_char);
    
    zpl_isize data_len = 0;
    zpl_u8 *escaped_data = zpl_file_stream_buf(&tmp, &data_len);
    escaped_data[data_len] = 0;
    
    return (track_escaped_string){
        .f = tmp,
        .len = (int)data_len,
        .text = (char*)escaped_data
    };
}