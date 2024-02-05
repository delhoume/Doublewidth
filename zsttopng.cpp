#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>


using namespace std;

extern "C" {
#include <zlib.h>
}

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define MAGIC false

// fdelhoume 2024
// take a zlib stream extracted from a strip in a Deflate TIFF and creates a PNG of <width x 1> 

int main(int argc, char *argv[]) {
  if (argc != 5)  {
    std::cout << "Usage: zsttopng <zst in> <width> <bytesperpixel> <png out>" << std::endl;
    std::cout << "       fdelhoume 2024" << std::endl;
    return 0;
  }
  
  unsigned int width = atoi(argv[2]);
  unsigned int bpp = atoi(argv[3]);

  unsigned char* bufferin = new unsigned char[width * bpp]; // more than enough, unless compressed size is larger than uncompressed....

  ifstream input(argv[1], ios::in|ios::binary|ios::ate);
  unsigned int size = input.tellg();
  input.seekg(0, ios::beg);
  input.read((char*)bufferin, size);
  input.close();

  unsigned char*  bufferout = new unsigned char[width * bpp];

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = size;
    infstream.next_in = bufferin; // input char array
    infstream.avail_out = width * bpp; // size of output
    infstream.next_out = bufferout; // output char array
     
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    // now save as PNG
    stbi_write_png(argv[4], width, 1, bpp, bufferout, 0);

delete [] bufferin;
delete [] bufferout;
    return 0; // success
}
