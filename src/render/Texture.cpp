/* File:        Texture.cpp
 * Purpose:     Handles reading of image files for the client renderer.
 *
 * Description: - Takes the name of an image resource file and
 *				- attempts to load it as an image. File formats
 *				- are prioritized as DDS then PNG.
 *
 * Issues:      - No DDS support yet
 *				- No threading for resource manager
 *				- Doesn't attempt to load other format if load failed while parsing
 */

#include <iostream>

#include "Texture.h"
#include <png.h>

void PNGReadDataFromStream(png_structp pngPtr, png_bytep data, png_size_t length);

Texture::Texture(std::string const &filename)
{
	name = filename;
}

Texture::~Texture()
{

}

bool Texture::LoadPNG(std::istream &stream)
{
	/* Read PNG signature to ensure file is valid */
	png_byte pngsig[8];
	stream.read((char*)pngsig, 8);

	if (!stream.good())
	{
		std::cout << "Error: Image::LoadPNG(): std::ifstream::read failed" << std::endl;
		return false;
	}

	int valid_png = png_sig_cmp(pngsig, 0, 8);
	if (valid_png != 0)
	{
		std::cout << "Error: Invalid PNG" << std::endl;
		return false;
	}

	/* Create the PNG read data struct */
	png_structp png_rstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_rstruct)
	{
		std::cout << "Error: Failed to initialize png read struct for: " << name << std::endl;
		return false;
	}

	/* Create the PNG info data struct */
	png_infop png_istruct = png_create_info_struct(png_rstruct);
	if (!png_istruct)
	{
		std::cout << "Error: Failed to initialize png info struct for: " << name << std::endl;
		png_destroy_read_struct(&png_rstruct, (png_infopp)0, (png_infopp)0);
		return false;
	}

	/* declare PNG data containers */
	png_bytep *row_pointers = 0;
	char *data = 0;

	/* Set error handling jumppoint */
	if (setjmp(png_jmpbuf(png_rstruct)))
	{
		png_destroy_read_struct(&png_rstruct, &png_istruct, (png_infopp)0);
		if (row_pointers != 0)
			delete[] row_pointers;
		if (data != 0)
			delete[] data;

		std::cout << "Error: Failed to read PNG file " << name << std::endl;
		return false;
	}

	png_set_read_fn(png_rstruct, (png_voidp)&stream, PNGReadDataFromStream); // Set custom read function
	
	/* Read PNG info/header and get image properties */
	png_set_sig_bytes(png_rstruct, 8); // Tell libpng we've already read signature (offset istream)
	png_read_info(png_rstruct, png_istruct); // Read info/header information

	png_uint_32 png_width = png_get_image_width(png_rstruct, png_istruct);
	png_uint_32 png_height = png_get_image_height(png_rstruct, png_istruct);
	png_uint_32 png_bpc = png_get_bit_depth(png_rstruct, png_istruct);	 // Bits per channel
	png_uint_32 png_channels = png_get_channels(png_rstruct, png_istruct);
	png_uint_32 png_color_type = png_get_color_type(png_rstruct, png_istruct); // (RGB, RGBA, lum, lum alpha, palette...)

	/* Convert PNG to required color type if necessary */
	if (png_color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_rstruct);
		png_channels = 3;
	}
	else if (png_color_type == PNG_COLOR_TYPE_GRAY)
	{
		if (png_bpc < 8)
			png_set_expand_gray_1_2_4_to_8(png_rstruct);

		png_bpc = 8;
	}
	
	if (png_get_valid(png_rstruct, png_istruct, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_rstruct);
		png_channels += 1;
	}

	if (png_bpc == 16) // Downsample 16BPC images to standard 8BPC
		png_set_strip_16(png_rstruct);

	/* Read image data from file */
	row_pointers = new png_bytep[png_height];
	data = new char[png_width * png_height * png_bpc * png_channels / 8];

	const unsigned int stride = png_width * png_bpc * png_channels / 8; // Bytes in one row

	/* Reverse row order */
	for (size_t i = 0; i < png_height; i++)
	{
		png_uint_32 q = (png_height - i - 1) * stride;
		row_pointers[i] = (png_bytep)data + q;
	}

	/* The actual read() call */
	png_read_image(png_rstruct, row_pointers);

	/* Store image properties */
	width = png_width;
	height = png_height;
	channels = png_channels;
	bpp = png_bpc * channels;

	/* Create GL texture */
	glGenTextures(1, &gl_texture); // Create OpenGL texture
	glBindTexture(GL_TEXTURE_2D, gl_texture); // Bind for use
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Load texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* Cleanup */
	delete[] (png_bytep)row_pointers;
	png_destroy_read_struct(&png_rstruct, &png_istruct, (png_infopp)0);

	std::cout << "Loaded: " << name << ".png " << width << "x" << height << "x" << bpp << std::endl;

	return true;
}

void PNGReadDataFromStream(png_structp pngPtr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(pngPtr);
	((std::istream*)a)->read((char*)data, length);
}