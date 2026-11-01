#include "crystal.h"


const c8* find_next(const c8 *str, const c8 *pattern) {
    return strstr(str, pattern);
}


const c8* extract_until_quote(const c8 *start, c8 *dest, i32 max_len) {
    if (*start != '"') return start;
    start++;  // Skip opening quote
    
    i32 len = 0;
    while (*start && *start != '"' && len < max_len - 1) {
        dest[len++] = *start++;
    }
    dest[len] = '\0';
    
    if (*start == '"') start++;  // Skip closing quote
    return start;
}


u32 parse_color_hex(const c8 *str) {
    u32 val = 0;
    
    // Skip opening quote
    if (*str == '"') str++;
    
    // Skip 0x or #
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) str += 2;
    else if (str[0] == '#') str++;
    
    // Parse hex digits until we hit closing quote
    while (*str && *str != '"') {
        c8 c = *str++;
        val = (val << 4);
        if (c >= '0' && c <= '9') val |= (c - '0');
        else if (c >= 'a' && c <= 'f') val |= (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') val |= (c - 'A' + 10);
        else break;
    }
    return val;
}


u8 find_color_idx(const c8 *name) {
    for (u8 i = 0; i < color_count; i++) {
        if (strcmp(colors[i].name, name) == 0) {
            return i;
        }
    }
    return 0;
}


void parse_colors(const c8 *buf) {
    printf("Scanning for colors section...\n");
    
    const c8 *colors_pos = find_next(buf, "\"colors\"");
    if (!colors_pos) {
        printf("No colors section found\n");
        return;
    }
    
    // Find the opening brace
    colors_pos = find_next(colors_pos, "{");
    if (!colors_pos) return;
    colors_pos++;
    
    // Find closing brace
    const c8 *colors_end = find_next(colors_pos, "}");
    if (!colors_end) return;
    
    color_count = 0;
    const c8 *p = colors_pos;
    
    while (p < colors_end && color_count < MAX_COLORS) {
        // Look for opening quote (start of color name)
        p = find_next(p, "\"");
        if (!p || p >= colors_end) break;
        p++;
        
        // Extract color name until closing quote
        c8 color_name[16];
        u8 i = 0;
        while (*p && *p != '"' && i < 15) {
            color_name[i++] = *p++;
        }
        color_name[i] = '\0';
        if (*p != '"') break;
        p++;  // Skip closing quote
        
        // Find the colon
        p = find_next(p, ":");
        if (!p || p >= colors_end) break;
        p++;
        
        // Find the opening quote of the hex value
        p = find_next(p, "\"");
        if (!p || p >= colors_end) break;
        
        // Parse hex value
        u32 hex_val = parse_color_hex(p);
        
        // Store color
        strcpy(colors[color_count].name, color_name);
        colors[color_count].rgb = hex_val;
        printf("Color: %s = 0x%06x\n", color_name, hex_val); // %06lx for u64
        color_count++;
        
        // Move to next value (skip past this hex value)
        p = find_next(p + 1, "\"");
        if (!p) break;
        p++;
        
        // Skip comma if present
        const c8 *comma = find_next(p, ",");
        if (comma && comma < colors_end) p = comma + 1;
    }
}


void parse_groups(const c8 *buf) {
    printf("Scanning for groups section...\n");
    
    const c8 *groups_pos = find_next(buf, "\"groups\"");
    if (!groups_pos) {
        printf("No groups section found\n");
        return;
    }
    
    // Find opening bracket
    groups_pos = find_next(groups_pos, "[");
    if (!groups_pos) return;
    groups_pos++;
    
    // Find closing bracket
    const c8 *groups_end = find_next(groups_pos, "]");
    if (!groups_end) return;
    
    group_count = 0;
    const c8 *p = groups_pos;
    
    while (p < groups_end && group_count < MAX_GROUPS) {
        // Look for opening quote
        p = find_next(p, "\"");
        if (!p || p >= groups_end) break;
        p++;
        
        // Extract group name until closing quote
        c8 group_name[16];
        u8 i = 0;
        while (*p && *p != '"' && i < 15) {
            group_name[i++] = *p++;
        }
        group_name[i] = '\0';
        if (*p != '"') break;
        p++;  // Skip closing quote
        
        // Store group
        strcpy(groups[group_count], group_name);
        printf("Group: %s\n", group_name);
        group_count++;
        
        // Skip comma if present
        const c8 *comma = find_next(p, ",");
        if (comma && comma < groups_end) p = comma + 1;
    }
}


void parse_apps(const c8 *buf) {
    printf("Scanning for apps section...\n");
    
    const c8 *apps_pos = find_next(buf, "\"apps\"");
    if (!apps_pos) {
        printf("No apps section found\n");
        return;
    }
    
    // Find opening bracket
    apps_pos = find_next(apps_pos, "[");
    if (!apps_pos) return;
    apps_pos++;
    
    // Find closing bracket (last ] in file)
    const c8 *buf_end = buf + strlen(buf);
    const c8 *apps_end = buf_end;
    for (const c8 *p = buf_end - 1; p >= apps_pos; p--) {
        if (*p == ']') {
            apps_end = p;
            break;
        }
    }
    
    app_count = 0;
    const c8 *p = apps_pos;
    
    while (p < apps_end && app_count < MAX_APPS) {
        // Look for opening brace (start of app object)
        p = find_next(p, "{");
        if (!p || p >= apps_end) break;
        p++;
        
        // Find closing brace for this app
        const c8 *app_end = find_next(p, "}");
        if (!app_end || app_end > apps_end) break;
        
        App *app = &apps[app_count];
        memset(app, 0, sizeof(App));
        
        // Parse all fields within this app object
        const c8 *ap = p;
        while (ap < app_end) {
            // Look for field name
            ap = find_next(ap, "\"");
            if (!ap || ap >= app_end) break;
            ap++;
            
            // Extract field name
            c8 field_name[32];
            u8 i = 0;
            while (*ap && *ap != '"' && i < 31) {
                field_name[i++] = *ap++;
            }
            field_name[i] = '\0';
            if (*ap != '"') break;
            ap++;
            
            // Find colon
            ap = find_next(ap, ":");
            if (!ap || ap >= app_end) break;
            ap++;
            
            // Find opening quote of value
            ap = find_next(ap, "\"");
            if (!ap || ap >= app_end) break;
            ap++;
            
            // Extract value
            c8 value[256];
            i = 0;
            while (*ap && *ap != '"' && i < 255) {
                value[i++] = *ap++;
            }
            value[i] = '\0';
            
            // Store in appropriate field
            if (strcmp(field_name, "group") == 0) {
                snprintf(app->group, sizeof app->group, "%s", value);
            } else if (strcmp(field_name, "text") == 0) {
                snprintf(app->text, sizeof app->text, "%s", value);
            } else if (strcmp(field_name, "cmd") == 0) {
                snprintf(app->cmd, sizeof app->cmd, "%s", value);
            } else if (strcmp(field_name, "rect_color") == 0) {
                app->rect_color_idx = find_color_idx(value);
            } else if (strcmp(field_name, "text_color") == 0) {
                app->text_color_idx = find_color_idx(value);
            }
            
            if (*ap == '"') ap++;  // Skip closing quote
        }
        
        // Validate app has required fields
        if (app->group[0] && app->text[0] && app->cmd[0]) {
            printf("App: %s (%s)\n", app->text, app->group);
            app_count++;
        }
        
        p = app_end + 1;
    }
}


static c8 config_buffer[32768];
void load_config(const c8 *config) {
    FILE *file = fopen(config, "r");
    if (!file) {
        fprintf(stderr, "Cannot open: %s\n", config);
        exit(1);
    }

    usize sz = fread(config_buffer, 1, sizeof(config_buffer) - 1, file);
    fclose(file);
    config_buffer[sz] = '\0';
    
    printf("\nLoaded config: %zu bytes\n", sz);

    printf("Parsing colors...\n");
    parse_colors(config_buffer);
    
    printf("Parsing groups...\n");
    parse_groups(config_buffer);
    
    printf("Parsing apps...\n");
    parse_apps(config_buffer);
    
    printf("\nLoaded: %d colors, %d groups, %d apps\n\n", 
           color_count, group_count, app_count);
}