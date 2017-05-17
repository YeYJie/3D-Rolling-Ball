#ifndef _TGALOADER_H_
#define _TGALOADER_H_

#include <cstdlib> // for NULL

class TgaFile
{
public:
    // type code : 2 for uncompressed RGB image
    //             3 for uncompressed black-and-white image
    unsigned char imageTypeCode;
    short int imageWidth;
    short int imageHeight;
    // bitCount / 8 = colorMode
    unsigned char bitCount;
    // color mode : 3 for RGB  4 for RGBA
    int colorMode;
    unsigned char *imageData;

// constructor and deconstructor
    TgaFile() : imageTypeCode(0), imageWidth(0), imageHeight(0),
                bitCount(0), colorMode(0), imageData(NULL) {}
    ~TgaFile() {
        imageTypeCode = imageWidth = imageHeight = bitCount = colorMode = 0;
        if(imageData)
            free(imageData);
        imageData = NULL;
    }
};

bool loadTgaFile(const char *filename, TgaFile *tgaFile);

#endif
