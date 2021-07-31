/* FILE NAME  : shooter_map.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit shooter map.
 */

#include "../../anim/anim.h"

namespace vagl
{
  /* My unit class */
  class shooter_map : public unit
  {
  public:
    primitives *Plane;
    primitives *Grass;
    primitives *Prs;
    vec3 RandomPositions[6];


    shooter_map( anim *Ani )
    {
      for (INT i = 0; i < 6; i++)
        RandomPositions[i] = vec3::Rnd1() * 100;
      Plane = new primitives;
      Plane->NumOfPrims = 1;
      Plane->Name = "shooter_map";
      Plane->Trans = matr::Identity();
      Plane->Prims = new vagl::prim[Plane->NumOfPrims];

      vertex::std V[4];
      V[0].P = vec3(-100, 0, -100);
      V[1].P = vec3(100, 0, -100);
      V[2].P = vec3(-100, 0, 100);
      V[3].P = vec3(100, 0, 100);
      V[0].N = vec3(0, 1, 0);
      V[1].N = vec3(0, 1, 0);
      V[2].N = vec3(0, 1, 0);
      V[3].N = vec3(0, 1, 0);
      V[0].T = vec2(-100, -100);
      V[1].T = vec2(100, -100);
      V[2].T = vec2(-100, 100);
      V[3].T = vec2(100, 100);
      Ani->render::PrimCreate(Plane->Prims, prim_type::TRISTRIP, V, 4, NULL, 0);
      Plane->Prims->Mtl->Tex[0] = Ani->LoadG32("BIN/TEXTURES/grass.g24", 3);
      Plane->Prims->Mtl->Ph = 100000;

      Grass = new primitives[6];
      Prs = new primitives;
      for (INT i = 0; i < Prs->NumOfPrims; i++)
      {
        Prs->Prims[i].Mtl->Ka = vec3(0.1);
        Prs->Prims[i].Mtl->Kd = vec3(0.8);
        Prs->Prims[i].Mtl->Ks = vec3(0.4);
        Prs->Prims[i].Mtl->Ph = 30.47;
      }
      Ani->PrimsLoad(&Grass[0], "BIN/MODELS/tree1.g3dm");
      Ani->PrimsLoad(&Grass[1], "BIN/MODELS/tree2.g3dm");
      Ani->PrimsLoad(&Grass[2], "BIN/MODELS/tree3.g3dm");
      Ani->PrimsLoad(&Grass[3], "BIN/MODELS/tree4.g3dm");
      Ani->PrimsLoad(&Grass[4], "BIN/MODELS/tree5.g3dm");
      Ani->PrimsLoad(&Grass[5], "BIN/MODELS/tree6.g3dm");
      for (INT j = 0; j < 6; j++)
        for (INT i = 0; i < Grass[j].NumOfPrims; i++)
        {
          Grass[j].Prims[i].Mtl->Ka = vec3(0.1);
          Grass[j].Prims[i].Mtl->Kd = vec3(0.8);
          Grass[j].Prims[i].Mtl->Ks = vec3(0.4);
          Grass[j].Prims[i].Mtl->Ph = 30.47;
        }
      //Ani->PrimsLoad(Prs, "BIN/MODELS/castle.g3dm");
    }

    ~shooter_map( VOID )
    {
      delete[] Plane->Prims;
      delete Plane;
      delete Prs;
      delete[] Grass;
    }

    VOID Response( anim *Ani ) override
    {
    }

    VOID Render( anim *Ani ) override
    {
      Plane->PrimsDraw(matr::Scale(vec3(8)), 1 * Plane->NumOfPrims);
      //Prs->PrimsDraw(matr::Scale(vec3(8)), 1 * Plane->NumOfPrims);

      for (INT i = 0; i < 6; i++)
        Grass[i].PrimsDraw(matr::Scale(vec3(.5)) * matr::Translate(vec3(RandomPositions[i][0], 0, RandomPositions[i][2])), 1 * Grass[i].NumOfPrims);
      for (INT i = 0; i < 6; i++)
        Grass[i].PrimsDraw(matr::Scale(vec3(.5)) * matr::Translate(vec3(RandomPositions[i][1], 0, RandomPositions[i][2]) / 2), 1 * Grass[i].NumOfPrims);
      for (INT i = 0; i < 6; i++)
        Grass[i].PrimsDraw(matr::Scale(vec3(.5)) * matr::Translate(vec3(RandomPositions[i][0] * 2, 0, RandomPositions[i][1])), 1 * Grass[i].NumOfPrims);
    }

  }; /* End of 'shooter_map' class */
}
/* END OF 'shooter_map.h' FILE */
