HELP NEEDED

Here is a simple C++ program using libTIFF to copy 2 times horizontally a Deflate compressed TIFF,
with 1 row per strip.

I understand that there should be a way to avoid decompression (2 times) and recompressions since we
know by advance the uncompressed size (width of the image).
This would allow to compute a rolling adler32 checksum and concatenate compressed zip streams for a given strip, thus preventing decompressing / recompressing.
I tried many things, with input from Mark Adler himself (https://stackoverflow.com/questions/14744692/concatenate-multiple-zlib-compressed-data-streams-into-a-single-stream-efficient)

My latest attempt was copying the first image uncompressed stream verbatim exact for last 4 bytes (checksum)
copy the second from offset 2 (no header) to size - 6 (no checksum)
combine the two checksum with adler32_combine(adler1, adler2, imagewidth * sampleperpixel);  (the number of decoded bytes), an emopty block, etc, as is described in Stack Overflow. 
write the rawstrip

it does not not work, the two input streams (that are identical but could not be if input images were different) are correct, 
infgen gives a result, decode to normal output, but I CANNOT COMBINE THEM, either the adler32 values are bad because of indianness,
or I am dumb or whatever...
The combined compressed output has always infgen give an error and  output TIFF is unreadable

The code compiles on a Mac M1, with g++, needs libtiff zlib lib(turbo)jpeg and liblzma, all installed using brew

My use case is building a Mars image from 90x44 images of 47420x47420  pixels each !

https://murray-lab.caltech.edu/CTX/V01/tiles/

So for each of the 2086480 strips of the final image I decode 90 times all 47420 pixels of each input image strip of the inputs images 
and compress 90 x 47420 = 4267800 pixels (also bytes because the input images are grey)

While I have working code to do this, it takes about 6 days for the whole image and any possible speed 
improvement is very valuable.

The image after this step is about 5.5 terabytes with default deflate compression

Thanks.

