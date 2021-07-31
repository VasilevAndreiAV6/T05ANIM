/* FILE NAME  : unit_btr.cpp
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit BTR.
 */

#include "../anim/anim.h"

namespace vagl
{
  /* BTR unit class */
  class unit_mebius : public unit
  {
  public:
    primitives *Prs;

    VOID CreateCube( anim *Ani, prim *Pr, FLT W, FLT H, FLT D, const matr &Trans )
    {
      vertex::std V[24];
      INT I[24];

      for (INT i = 0; i < 24; i++)
        I[i] = i;

      for (INT i = 0; i < 24; i++)
        V[i].C = vec4(1);

      /*
      V[0] = vec3(0, 0, 0);
      V[1] = vec3(W, 0, 0);
      V[2] = vec3(W, H, 0);
      V[3] = vec3(0, H, 0);

      V[4] = vec3(0, H, D);
      V[5] = vec3(W, H, D);
      V[6] = vec3(W, 0, D);
      V[7] = vec3(0, 0, D);
      */
      V[0].P =  vec3(0, 0, 0);
      V[1].P =  vec3(0, 0, D);
      V[2].P =  vec3(0, H, 0);
      V[3].P =  vec3(0, H, D);
      
      V[5].P =  vec3(0, H, 0);
      V[4].P =  vec3(0, H, D);
      V[7].P =  vec3(W, H, 0);
      V[6].P =  vec3(W, H, D);

      V[8].P =  vec3(W, H, 0);
      V[9].P =  vec3(W, H, D);
      V[10].P = vec3(W, 0, 0);
      V[11].P = vec3(W, 0, D);

      V[13].P = vec3(W, H, D);
      V[12].P = vec3(W, 0, D);
      V[14].P = vec3(0, H, D);
      V[15].P = vec3(0, 0, D);

      V[17].P = vec3(W, 0, D);
      V[16].P = vec3(0, 0, D);
      V[19].P = vec3(W, 0, 0);
      V[18].P = vec3(0, 0, 0);
      

      V[20].P = vec3(W, 0, 0);
      V[21].P = vec3(0, 0, 0);
      V[22].P = vec3(W, H, 0);
      V[23].P = vec3(0, H, 0);

      Ani->PrimCreate(Pr, prim_type::TRISTRIP, V, 24, I, 24);
      Pr->Transform = Trans;
    }

    unit_mebius( anim *Ani )
    {
      //INT mode[2];

      //glGetIntegerv(GL_POLYGON_MODE, mode);
      //if (mode[0] == GL_FILL)
      //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      //else
      //  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      INT NumOfPrs = 60;
      Prs = new primitives;
      Prs->Prims = new prim[NumOfPrs];
      Prs->Name = "Mebius";
      Prs->NumOfPrims = NumOfPrs;
      Prs->Trans = matr::Identity();

      /*for (INT i = 0; i < NumOfV; i += 2)
      {
        V[i].P.X = cos(2 * PI * i / NumOfV);
        V[i].P.Y = 0;
        V[i].P.Z = sin(2 * PI * i / NumOfV);

        V[i + 1].P.X = cos(2 * PI * i / NumOfV);
        V[i + 1].P.Y = 0.1;
        V[i + 1].P.Z = sin(2 * PI * i / NumOfV);

        V[i].C = vec4(1);
        V[i + 1].C = vec4(1);
      }*/
      
      for (INT i = 0; i < NumOfPrs; i++)
      {
        matr m = matr::Identity();

        m *= matr::RotateZ(-360.0 * i / NumOfPrs);
        m *= matr::RotateY(-180.0 * i / NumOfPrs);
        
        m *= m * matr::Translate(vec3(cos(2 * PI * i / NumOfPrs), 0, sin(2 * PI * i / NumOfPrs)));

        CreateCube(Ani, &Prs->Prims[i], 0.05, 0.2, 0.1, m);
      }
    }

    VOID Render( anim *Ani ) override
    {
      Prs->PrimsDraw(matr::Scale(vec3(50)), 0);
    }

  }; /* End of 'unit_mebius' class */
}
/* END OF 'unit_mebius.h' FILE */
