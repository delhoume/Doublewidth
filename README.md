HELP NEEDED

Here is a simple C++ program using libTIFF to copy 2 times horizontally a Deflate compressed TIFF,
with 1 row per strip.

I understand that there should be a way to avoid decompression (2 times) and recompressions since we
know by advance the uncompressed size (width of the image).
This would allow to compute a rolling adler32 checksum and concatenate compressed zip streams for a given strip, thus preventing decompressing / recompressing.
I tried many things, with input from Mark Adler himself (https://stackoverflow.com/questions/14744692/concatenate-multiple-zlib-compressed-data-streams-into-a-single-stream-efficient)

My latest attempt was copying the first image uncompressed stream verbatim except for last 4 bytes (checksum)
copy the second stream from offset 2 (no header) to size - 6 (no checksum)
combine the two checksum with adler32_combine(adler1, adler2, imagewidth * sampleperpixel);  (the number of decoded bytes), 
add an empty block, the the rollinig checksum, as it is described in Stack Overflow. 
write the rawstrip

it does not not work, the two input streams (that are identical but can be different if input images were different - I simplified the code) are correct, 
infgen gives a result, decode to normal output, but I CANNOT COMBINE THEM, either the adler32 values are bad because of indianness, or I messed up with pointer arithmetic, 
or I am dumb most likely) or whatever...
The combined compressed output has always infgen give an error and  output TIFF is unreadable

The code compiles on a Mac M1, with g++13, needs libtiff zlib lib(turbo)jpeg and liblzma, all installed using brew

My use case is building a Mars image from  a mosaic of 90x44 images (47420x47420 1 byte per pixel each) !

https://murray-lab.caltech.edu/CTX/V01/tiles/

So for each of the 2086480 strips of the final image I decode 90 times all 47420 pixels of each input image strip and compress 90 x 47420 = 4267800 pixels (also bytes because the input images are grey).

While I have working code to do this, it takes about 6 days for the whole image and any possible speed improvement is very valuable.

The image after this step is about 5.5 terabytes with default deflate compression.

Then I process it again to create a pyramidal TIFF than can be interactively displayed with my TIFF specialized Windows viewer https://github.com/delhoume/vliv 
Final image is JPEG compressed to save disk space.

cassini_paris.tif is an 512x326 3 bytes/pixel Deflate compressed 1 row per strip image
raw_0.zst is output from first strip compressed contents, decompressed length is 512 * 3 = 1536 bytes
double_0.zts is output from first strip of doubled paris_cassini.tif, decompressed length is 1024 * 3 = 3072 (obtained by doubling previous result of doubling)

Thanks

