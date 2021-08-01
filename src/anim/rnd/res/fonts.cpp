///* FILE NAME  : fonts.h
// * PROGRAMMER : AV6
// * LAST UPDATE: 28.07.2021
// * PURPOSE    : Font system.
// */
//
//#include "fonts.h"
//#include "../../anim.h"
//
//namespace vagl
//{
//  struct current
//  {
//    prim FntPrims[256];
//    material *Mtl;
//  } cur;
//  font_manager::font_manager( font *Fnt, std::string FontName )
//  {
//    Fnt = new font;
//    anim Ani = anim::Get();
//    FILE *F;
//    VOID *txt;
//    static CHAR Buf[1000];
//
//    sprintf_s(Buf, "BIN/FONT/%s.g3df", FontName.c_str());
//    Ani.render::shader_manager::Add("FONT");
//    fopen_s(&F, Buf, "rb");
//    if (F != NULL)
//    {
//      if (fread(Fnt, sizeof(font), 1, F) == 1 && Fnt->Sign == *(DWORD *)"G3DF")
//        if ((txt = malloc(4 * Fnt->W * Fnt->H)) != NULL)
//        { 
//          cur.Mtl = new material;
//
//          cur.Mtl->Name += "Font: ";
//          cur.Mtl->Name += FontName;
//          fread(txt, 4, Fnt->W * Fnt->H, F);
//          cur.Mtl->Tex[0] = Ani.render::AddImage(FontName, Fnt->W, Fnt->H, 4, txt);
//          cur.Mtl->ShdName = "FONT";
//          free(txt);
//
//          for (INT i = 0; i < 256; i++)
//          {
//            Ani.PrimCreate(&cur.FntPrims[i], prim_type::TRISTRIP, Fnt->Chars[i], 4, NULL, 0);
//            cur.FntPrims[i].Mtl = cur.Mtl;
//          }
//        }
//      fclose(F);
//    }
//  }
//
//  /* Draw text function.
//    * ARGUMENTS:
//    *   - string to draw:
//    *       CHAR *Str;
//    *   - font size scale factor:
//    *       FLT Size;
//    *   - start draw position:
//    *       VEC Pos;
//    *   - draw text color:
//    *       VEC4 Color;
//    * RETURNS: None.
//    */
//  VOID font_manager::Draw( font *Fnt, CHAR *Str, FLT Size, vec3 Pos, vec4 Color )
//  {
//    anim Ani = anim::Get();
//    FLT scale = Size * Fnt->LineH;
//    matr m = matr::Scale(vec3(scale)) * matr::Translate(Pos);
//    vec3 StartPos = Pos;
// 
//    while (*Str != 0)
//    {
//      if (*Str == '\n')
//        Pos.Y -= scale, Pos.X = StartPos.X;
//      else if (Fnt->AdvancedX[(BYTE)*Str] != 0)
//      {
//        m[3 * 4 + 0] = Pos.X;
//        m[3 * 4 + 1] = Pos.Y;
//        Ani.DrawPrim(&cur.FntPrims[(BYTE)*Str], m, 0);
//        Pos.X += Fnt->AdvancedX[(BYTE)*Str] * scale;
//      }
//      Str++;
//    }
//  } /* End of 'FntDraw' function */
//}
///* END OF 'fonts.cpp' FILE */
