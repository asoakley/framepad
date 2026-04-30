#ifndef STREAMDECK_H
#define STREAMDECK_H

#include <stdint.h>
#include <stdbool.h>

#define GRID_ROWS 2
#define GRID_COLS 3
#define MAX_ICONS (GRID_ROWS * GRID_COLS)
#define MAX_COMMAND_LENGTH 64
#define MAX_IMAGE_FILENAME_LENGTH 32

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} IconPosition;

typedef struct
{
    char image_filename[MAX_IMAGE_FILENAME_LENGTH];
    char command[MAX_COMMAND_LENGTH];
    IconPosition position;
    bool active;
} IconConfig;

typedef struct
{
    IconConfig icons[MAX_ICONS];
    uint8_t num_icons;
} StreamDeckConfig;

extern StreamDeckConfig stream_deck_config;

// Function declarations
void stream_deck_init(void);
void stream_deck_load_config(void);
void stream_deck_draw_grid(void);
void stream_deck_handle_touch(uint16_t x, uint16_t y);
void stream_deck_send_command(const char *command);

#endif // STREAMDECK_H