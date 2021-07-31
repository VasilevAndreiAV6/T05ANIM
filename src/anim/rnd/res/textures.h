/* FILE NAME   : textures.h
 * PROGRAMMER  : AV6
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Materials class declaration.
 */

#ifndef __textures_h_
#define __textures_h_

#include <fstream>

#include "../../../vagl.h"
#include "resource.h"

/* NSF name space */
namespace vagl
{
  class texture_manager;

  /* Material class */
  class texture
  {
  public:
    std::string Name;
    UINT TexId;
    INT W, H;

    /* Material constructor */
    texture( VOID ) : Name {}, TexId(0), W(0), H(0)
    {
    } /* End of 'texture' function */

    VOID Free( VOID )
    {
    }
  }; /* End of 'texture' class */

  /* Material manager class */
  class texture_manager : public resource_manager<texture, INT>
  {
  public:
    INT TextureSize = 0;

    /* Shader manager constructor */
    texture_manager( VOID ) : resource_manager<texture, INT>()
    {
    } /* End of 'texture_manager' function */

    /* Add shader function.
     * ARGUMENTS:
     *   - texture:
     *       texture &Tex;
     * RETURNS: None.
     */
    INT AddImage( const std::string Name, INT w, INT h, INT C, VOID *Bits )
    {
      texture Tex;
      FLOAT mips;

      mips = w > h ? h : w;
      mips = log(mips) / log(2);
      if (mips < 1)
        mips = 1;

      /* Allocate texture space */
      glGenTextures(1, &Tex.TexId);
      glBindTexture(GL_TEXTURE_2D, Tex.TexId);

      /* Upload texture */
      ///gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
      glTexStorage2D(GL_TEXTURE_2D, mips, C == 3 ? GL_RGB8 : C == 4 ? GL_RGBA8 : GL_R8, w, h);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, Bits);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      Tex.Name[0] = '0' + TextureSize;
      Tex.W = w;
      Tex.H = h;

      glBindTexture(GL_TEXTURE_2D, 0);
      resource_manager::add(Tex);
      return TextureSize++;
    } /* End of 'Add' function */

    /* Texture load from *.G32 or *.G24 file function.
     * ARGUMENTS:
     *   - Texture file name:
     *       CHAR *FileName;
     *   - G32 or G24 tag:
     *       INT tag;
     * RETURNS:
     *   (INT) new img stock number.
     */
    INT LoadG32( const std::string FileName, INT C )
    {
      DWORD d = 0;
      WORD w = 0, h = 0;
      INT x, y, texnum;
      std::fstream f(FileName, std::fstream::in | std::fstream::binary);

      f.read(reinterpret_cast<CHAR *>(&w), 2);
      f.read(reinterpret_cast<CHAR *>(&h), 2);

      DWORD *img = new DWORD[4 * w * h];

      for (x = 0; x < w; x++)
        for (y = 0; y < h; y++)
        {
          f.read(reinterpret_cast<CHAR *>(&d), C);
          d |= 0xFF000000;
          img[(h - y - 1) * w + x] = d;
        }
      texnum = AddImage(FileName, w, h, C, (img));

      delete[] img;
      return texnum;
    } /* End of 'LoadG32' function */

  }; /* End of 'texture_manager' class */
} /* end of 'vagl' namespace */

#endif /* __textures_h_ */

/* END OF 'textures.h' FILE */
