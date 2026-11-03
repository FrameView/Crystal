#ifndef JSON_H
#define JSON_H


// Find next occurrence of pattern in string
const c8* find_next(const c8 *str, const c8 *pattern);

// Extract quoted string, return pointer to after closing quote
const c8* extract_until_quote(const c8 *start, c8 *dest, i32 max_len);

// Parse hex color from string like "0x87CEEB"
u32 parse_color_hex(const c8 *str);

// Find color index by name
u8 find_color_idx(const c8 *name);

// Parse colors: { "name": "0xHEX", ... }
void parse_colors(const c8 *buf);

// Parse groups: [ "name", "name", ... ]
void parse_groups(const c8 *buf);

// Parse apps: [ { "key": "value", ... }, ... ]
void parse_apps(const c8 *buf);

// Parse musics: { "name": "link", ... }
void parse_musics(const c8 *buf);

// Load the config file
void load_config(const c8 *config);


#endif