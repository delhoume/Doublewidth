#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

extern "C" {
#include <tiffio.h>
#include <zlib.h>
}

#define MAGIC false

// fdelhoume 2024
// take a 1 height strip based Deflate TIFF and doubles it horizontally

int main(int argc, char *argv[]) {
  if (argc != 3)  {
    std::cout << "Usage: doublewidth <tiff in> <tiff out>" << std::endl;
    std::cout << "       fdelhoume 2024" << std::endl;
    return 0;
  }
  
  TIFF *tifin = TIFFOpen(argv[1], "r");
  
  unsigned int imagewidth;
  unsigned int imageheight;
  unsigned int rows_per_strip;   
 uint16_t bitspersample, samplesperpixel, compression;

TIFFGetField(tifin, TIFFTAG_IMAGEWIDTH, &imagewidth);
TIFFGetField(tifin, TIFFTAG_IMAGELENGTH, &imageheight);
TIFFGetField(tifin, TIFFTAG_BITSPERSAMPLE, &bitspersample);
TIFFGetField(tifin, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
TIFFGetField(tifin, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
TIFFGetFieldDefaulted(tifin, TIFFTAG_COMPRESSION, &compression);

// strip of size 1 are easier for this example
if (TIFFIsTiled(tifin) || (rows_per_strip != 1)) {
  std::cout << "Input image must have strips (of size 1)" << std::endl;
  return 1;
}

unsigned int dstwidth = imagewidth * 2;
unsigned int dstheight = imageheight;

TIFF *tifout = TIFFOpen(argv[2], "w");

tsize_t insize = imagewidth * samplesperpixel;
tsize_t outsize = dstwidth * samplesperpixel;
unsigned char *rawdata1 = new unsigned char[insize];
unsigned char *rawdata2 = new unsigned char[insize];
unsigned char *outdata = new unsigned char[outsize];

TIFFSetField(tifout, TIFFTAG_IMAGEWIDTH, dstwidth);
TIFFSetField(tifout, TIFFTAG_IMAGELENGTH, dstheight);
TIFFSetField(tifout, TIFFTAG_BITSPERSAMPLE,bitspersample);
TIFFSetField(tifout, TIFFTAG_SAMPLESPERPIXEL,samplesperpixel);
TIFFSetField(tifout, TIFFTAG_PHOTOMETRIC,PHOTOMETRIC_RGB);
TIFFSetField(tifout, TIFFTAG_ROWSPERSTRIP, 1);
TIFFSetField(tifout, TIFFTAG_COMPRESSION, COMPRESSION_ADOBE_DEFLATE);
TIFFSetField(tifout, TIFFTAG_ZIPQUALITY, 6);


  for (unsigned int y = 0; y < imageheight; ++y) {
   // size_t readBytes1 = TIFFReadEncodedStrip(tifin, y, outdata, insize);
   //size_t readBytes2 = TIFFReadEncodedStrip(tifin, y, outdata + insize, insize);

      size_t readBytes1 = TIFFReadRawStrip(tifin, y, rawdata1, insize);
      size_t readBytes2 = TIFFReadRawStrip(tifin, y, rawdata2, insize);

      if (MAGIC) {
          // HERE SOME MAGIC WITH ZLIB PLEASE TO FILL THE RAW OUTPUT BUFFER FROM RAW INPUTS
          size_t outBytes = 0;
          TIFFWriteRawStrip(tifout, y, outdata, outBytes);
        } else {
          // decodes 2 times, encode once
          TIFFReadFromUserBuffer(tifin, 0, rawdata1, readBytes1, outdata, insize);
          TIFFReadFromUserBuffer(tifin, 0, rawdata2, readBytes2, outdata + insize, insize);
           TIFFWriteEncodedStrip(tifout, y, outdata, outsize);
      }
  }
  TIFFClose(tifin);
  TIFFClose(tifout);
  delete [] rawdata1; 
  delete [] rawdata2;
  delete [] outdata;
  return 0; // success
}
