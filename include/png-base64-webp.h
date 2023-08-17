#ifndef IMAGE_ENCODER_H
#define IMAGE_ENCODER_H

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>
#include <webp/encode.h>

// Function to encode a PNG image to base64
// The caller is responsible for freeing the returned string using free().
char* encodePNGToBase64(const char* imagePath);

#endif // IMAGE_ENCODER_H

typedef struct {
    png_byte* data;
    int width;
    int height;
    int rowbytes;
    png_byte color_type;
} PNGMetaData;

// Helper function to read PNG data
PNGMetaData readPNGData(const char* imagePath) {
    PNGMetaData metaData = {NULL, 0, 0, 0, 0};
    FILE* fp = fopen(imagePath, "rb");
    if (!fp) {
        return metaData;
    }

    png_byte header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return metaData;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return metaData;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return metaData;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return metaData;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    int dataSize = height * rowbytes;
    png_byte* imageData = (png_byte*)malloc(dataSize);
    if (!imageData) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return metaData;
    }

    for (int y = 0; y < height; y++) {
        memcpy(imageData + y * rowbytes, row_pointers[y], rowbytes);
    }

    metaData.data = imageData;
    metaData.width = width;
    metaData.height = height;
    metaData.rowbytes = rowbytes;
    metaData.color_type = color_type;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return metaData;
}

// Helper function to convert PNG data to WebP format
static png_byte* convertPNGToWebP(const png_byte* pngData, int width, int height, int rowbytes, png_byte color_type, int* outWebPSize) {
    png_byte* webPData = NULL;
    if (color_type == PNG_COLOR_TYPE_RGB) {
        *outWebPSize = WebPEncodeLosslessRGB(pngData, width, height, rowbytes, &webPData);
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        *outWebPSize = WebPEncodeLosslessRGBA(pngData, width, height, rowbytes, &webPData);
    }
    return webPData;
}

char* encodePNGToBase64(const char* imagePath) {
    PNGMetaData metaData = readPNGData(imagePath);
    if (!metaData.data) {
        return NULL;
    }

    int webPSize;
    png_byte* webPData = convertPNGToWebP(metaData.data, metaData.width, metaData.height, metaData.rowbytes, metaData.color_type, &webPSize);
    free(metaData.data);  // Free the original PNG data after conversion

    if (!webPData) {
        return NULL;
    }

    const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64Size = 4 * ((webPSize + 2) / 3);
    char* base64Output = (char*)malloc(base64Size + 1);
    if (!base64Output) {
        free(webPData);
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < webPSize; i += 3) {
        int value = (webPData[i] << 16) + (webPData[i + 1] << 8) + webPData[i + 2];
        base64Output[j++] = base64_chars[(value >> 18) & 0x3F];
        base64Output[j++] = base64_chars[(value >> 12) & 0x3F];
        base64Output[j++] = base64_chars[(value >> 6) & 0x3F];
        base64Output[j++] = base64_chars[value & 0x3F];
    }

    base64Output[j] = '\0';

    free(webPData);
    return base64Output;
}
