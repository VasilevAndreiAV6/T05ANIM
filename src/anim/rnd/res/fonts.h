///* FILE NAME  : fonts.h
// * PROGRAMMER : AV6
// * LAST UPDATE: 28.07.2021
// * PURPOSE    : Font system.
// */
//
//#ifndef __fonts_h_
//#define __fonts_h_
//
//#include "resource.h"
//#include "prim.h"
//
///* Project namespace */
//namespace vagl
//{
//  class font
//  {
//  public:
//    DWORD Sign;                /* Signature ("G3DF") */
//    DWORD LineH;               /* Texture font height in pixels */
//    DWORD BaseH;               /* Base line distance in pixels */
//    FLT AdvancedX[256];        /* Character offsets */
//    vertex::std Chars[256][4]; /* Character glyphs conrner points */
//    DWORD W, H;                /* Texture size */
//  };
//
//  class font_manager
//  {
//  public:
//    /* Font class constructor */
//    font_manager( VOID )
//    {
//    } /* End of 'font' function */
//
//    font_manager( font *Fnt, std::string FontName );
//
//    /* Draw text function.
//     * ARGUMENTS:
//     *   - string to draw:
//     *       CHAR *Str;
//     *   - font size scale factor:
//     *       FLT Size;
//     *   - start draw position:
//     *       VEC Pos;
//     *   - draw text color:
//     *       VEC4 Color;
//     * RETURNS: None.
//     */
//    VOID Draw( font *Fnt, CHAR *Str, FLT Size, vec3 Pos, vec4 Color );
//
//  }; /* End of 'font' class */
//} /* end of 'vagl' namespace */
//
//#endif /* __fonts_h_ */
//
///* END OF 'fonts.h' FILE */
