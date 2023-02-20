#include "YuvLibFile.h"
#include "YuvLibJpeg.h"

#define FRAME_WIDTH     1280
#define FRAME_HEIGHT     720

#define JPEG_QUALITY      75

int Nv12ToJpgFile(const char *pJpgFile, const char* pYuvFrame,
    const int nWidth, const int nHeight)
{
    YuvJpegInfo oJpegInfo = {0};
    char* pYBase  = NULL;
    char* pUvBase   = NULL;
    char* pJpegLine = NULL;
    int i   = 0;
    int j   = 0;
    int idx = 0;

    if (YUV_Jpeg_InitInfo(&oJpegInfo, JPEG_QUALITY, nWidth, nHeight, pJpgFile))
    {
        return -1;
    }
    
    pYBase  = (char*)pYuvFrame;
    pUvBase = (char*)pYuvFrame+nWidth*nHeight;

    pJpegLine = oJpegInfo.pYuvLine;
    while (YUV_Jpeg_NeedMoreData(&oJpegInfo))
    {
        idx=0;
        for (i=0; i<nWidth; i++)
        {   
            pJpegLine[idx++] = pYBase[i];
            pJpegLine[idx++] = pUvBase[(i/2)*2];
            pJpegLine[idx++] = pUvBase[(i/2)*2+1];
        }  
        YUV_Jpeg_SaveLine(&(oJpegInfo));
        
        pYBase += nWidth;
        if (j % 2 == 0)
        {
            pUvBase += nWidth;
        }
        j++;
    }
    
    YUV_Jpeg_ReleaseInfo(&(oJpegInfo));

    return 0;
}

int main(const int argc, const char** argv)
{
    char nv12File[FILE_NAME_SIZE] = {0};
    char jpegFile[FILE_NAME_SIZE] = {0};
    
    int dataCount = (int)(FRAME_WIDTH*FRAME_HEIGHT*1.5);
    char* pNv12 = (char*)malloc(dataCount);

    YUV_CreateFileNameFrame(nv12File, "../RgbToNv12/Nv12", FRAME_WIDTH, FRAME_HEIGHT);
    YUV_ReadFromFile(nv12File, pNv12, dataCount);

    YUV_CreateFileName(jpegFile, "Nv12", "jpg", FRAME_WIDTH, FRAME_HEIGHT);
    Nv12ToJpgFile(jpegFile, pNv12, FRAME_WIDTH, FRAME_HEIGHT);

    free(pNv12);
}

