#include "Font.h"
#include "../Logger.h"

#ifdef _WIN32
namespace star
{

	bool Font::Init(const tstring& path, int32 size, FT_Library& library )
	{
		mSize = size;
		mTextures = new GLuint[FONT_TEXTURES];

		//Convert from wstring to const char* trough std::string
		std::string font_path = string_cast<std::string>(path);
		auto error = FT_New_Face(library,font_path.c_str(),0,&mFace);
		if(error == FT_Err_Unknown_File_Format)
		{
			star::Logger::GetInstance()->Log(star::LogLevel::Error,_T("Font Manager : Font : ") + path + _T(" ,could be opened but its in unsuported format"));
			return (false);
		}
		else if(error)
		{
			star::Logger::GetInstance()->Log(star::LogLevel::Error,_T("Font Manager : Font : ") + path + _T(" ,is invalid and cant be opened or read or its broken"));
			return (false);
		}
		star::Logger::GetInstance()->Log(star::LogLevel::Info,_T("Font Manager : Font : ") + path + _T(" ,loaded and ready for use"));

		FT_Set_Char_Size(mFace,size<<6, size<<6, FONT_DPI, FONT_DPI);

		mList_base=glGenLists(FONT_TEXTURES);
		glGenTextures(FONT_TEXTURES,mTextures);
		for(unsigned char i=0; i < FONT_TEXTURES; ++i)
		{
			Make_D_List(mFace, i, mList_base, mTextures);
		}

		FT_Done_Face(mFace);

	}

	void Font::DeleteFont()
	{
		glDeleteLists(mList_base,FONT_TEXTURES);
		glDeleteTextures(FONT_TEXTURES,mTextures);
		delete[] mTextures;
	}

	void Font::Make_D_List( FT_Face face, char ch, GLuint list_base, GLuint * tex_base )
	{

		FT_UInt glyph_index;
		glyph_index = FT_Get_Char_Index(face,ch );

		auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if(error)
		{
			star::Logger::GetInstance()->Log(star::LogLevel::Error,_T("Font : could not load Glyph"));
			return;
		}

		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if(error)
		{
			star::Logger::GetInstance()->Log(star::LogLevel::Error,_T("Font : could not load Glyph"));
			return;
		}

		FT_Bitmap& bitmap = face->glyph->bitmap;

		int32 width = NextPowerOfTwo(bitmap.width);
		int32 height = NextPowerOfTwo(bitmap.rows);

		GLubyte* expanded_data = new GLubyte[2 * width * height];

		for(int j = 0; j <height ; j++) {
			for(int i = 0; i < width; i++) {
				expanded_data[2 * (i + j * width)] = 255;
				expanded_data[2 * (i + j * width) + 1] = 
					(i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
			}
		}

		glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
		delete[] expanded_data;

		glBindTexture(GL_TEXTURE_2D,tex_base[ch]);
		float x=static_cast<float>(bitmap.width) / static_cast<float>(width);
		float y=static_cast<float>(bitmap.rows) / static_cast<float>(height);

		fontVertices tempVertices;
		tempVertices.ver[0] = (GLfloat)bitmap.width;
		tempVertices.ver[1] = (GLfloat)bitmap.rows;
		tempVertices.ver[2] = 0;
		tempVertices.ver[3] = (GLfloat)bitmap.width;
		tempVertices.ver[4] = 0;
		tempVertices.ver[5] = 0;
		tempVertices.ver[6] = 0;
		tempVertices.ver[7] = (GLfloat)bitmap.rows;
		tempVertices.ver[8] = 0;
		tempVertices.ver[9] = 0;
		tempVertices.ver[10] = 0;
		tempVertices.ver[11] = 0;

		mVecticesList.push_back(tempVertices);

		fontUvCoords tempCoords;
		tempCoords.uv[0] = x;
		tempCoords.uv[1] = 0;
		tempCoords.uv[2] = x;
		tempCoords.uv[3] = y;
		tempCoords.uv[4] = 0;
		tempCoords.uv[5] = 0;
		tempCoords.uv[6] = 0;
		tempCoords.uv[7] = y;

		mUVcoordsList.push_back(tempCoords);
	}

	int32 Font::NextPowerOfTwo( const int32& a )
	{
		int32 rval=1;
		while(rval<a)rval<<=1;
		return rval;
	}

}
#endif