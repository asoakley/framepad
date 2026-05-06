#include "StreamDeck.h"
#include "LCD_Driver.h"
#include "LCD_GUI.h"
#include "LCD_Bmp.h"
#include "ff.h"
#include "fatfs_storage.h"
#include "HIDTask.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern LCD_DIS sLCD_DIS;

StreamDeckConfig stream_deck_config = {0};

#define ICON_WIDTH (sLCD_DIS.LCD_Dis_Column / GRID_COLS)
#define ICON_HEIGHT (sLCD_DIS.LCD_Dis_Page / GRID_ROWS)
#define ICON_MARGIN 16                                                                        // Margin around icons in pixels
#define ICON_SIZE (((ICON_WIDTH < ICON_HEIGHT) ? ICON_WIDTH : ICON_HEIGHT) - 2 * ICON_MARGIN) // Square icons with margin

// Simple config file format: each line "image.bin command"
// For now, assume fixed positions in order
void stream_deck_load_config(void)
{
    FIL config_file;
    FRESULT res;
    char line[256];
    uint8_t icon_index = 0;

    res = f_open(&config_file, "config.txt", FA_READ);
    if (res != FR_OK)
    {
        printf("Failed to open config.txt\n");
        return;
    }

    while (f_gets(line, sizeof(line), &config_file) && icon_index < MAX_ICONS)
    {
        char *image_part = strtok(line, " ");
        char *command_part = strtok(NULL, "\n");

        if (image_part && command_part)
        {
            strncpy(stream_deck_config.icons[icon_index].image_filename, image_part, MAX_IMAGE_FILENAME_LENGTH - 1);
            strncpy(stream_deck_config.icons[icon_index].command, command_part, MAX_COMMAND_LENGTH - 1);

            // Calculate centered position for square icon
            uint8_t row = icon_index / GRID_COLS;
            uint8_t col = icon_index % GRID_COLS;
            stream_deck_config.icons[icon_index].position.x = col * ICON_WIDTH + (ICON_WIDTH - ICON_SIZE) / 2;
            stream_deck_config.icons[icon_index].position.y = row * ICON_HEIGHT + (ICON_HEIGHT - ICON_SIZE) / 2;
            stream_deck_config.icons[icon_index].position.width = ICON_SIZE;
            stream_deck_config.icons[icon_index].position.height = ICON_SIZE;
            stream_deck_config.icons[icon_index].active = true;

            icon_index++;
        }
    }

    stream_deck_config.num_icons = icon_index;
    f_close(&config_file);
}

// Function to load and display raw binary image (RGB565)
void display_raw_image(const char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{

    // see https://longfangsong.github.io/en/image-to-rgb565/

    FIL image_file;
    FRESULT res;
    UINT bytes_read;
    uint16_t *buffer = (uint16_t *)malloc(width * height * 2); // RGB565 is 2 bytes per pixel

    if (!buffer)
    {
        printf("Failed to allocate buffer for image\n");
        return;
    }

    res = f_open(&image_file, filename, FA_READ);
    if (res != FR_OK)
    {
        printf("Failed to open image file %s\n", filename);
        free(buffer);
        return;
    }

    res = f_read(&image_file, buffer, width * height * 2, &bytes_read);
    if (res != FR_OK || bytes_read != width * height * 2)
    {
        printf("Failed to read image data\n");
        f_close(&image_file);
        free(buffer);
        return;
    }

    f_close(&image_file);

    // Display the image
    LCD_SetWindow(x, y, x + width - 1, y + height - 1);
    for (uint32_t i = 0; i < width * height; i++)
    {
        // Little-endian RGB565 in file
        LCD_WriteData(buffer[i]);
    }

    free(buffer);
}

void stream_deck_draw_grid(void)
{
    for (uint8_t i = 0; i < stream_deck_config.num_icons; i++)
    {
        if (stream_deck_config.icons[i].active)
        {
            IconConfig *icon = &stream_deck_config.icons[i];
            display_raw_image(icon->image_filename,
                              icon->position.x,
                              icon->position.y,
                              icon->position.width,
                              icon->position.height);
        }
    }
}

void stream_deck_handle_touch(uint16_t x, uint16_t y)
{
    for (uint8_t i = 0; i < stream_deck_config.num_icons; i++)
    {
        if (stream_deck_config.icons[i].active)
        {
            IconConfig *icon = &stream_deck_config.icons[i];
            if (x >= icon->position.x && x < icon->position.x + icon->position.width &&
                y >= icon->position.y && y < icon->position.y + icon->position.height)
            {
                stream_deck_send_command(icon->command);
                break;
            }
        }
    }
}

void stream_deck_send_command(const char *command)
{
    hid_send_command(command);
}

void stream_deck_init(void)
{
    stream_deck_load_config();
    stream_deck_draw_grid();
}