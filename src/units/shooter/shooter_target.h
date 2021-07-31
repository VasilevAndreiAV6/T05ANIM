/* FILE NAME  : shooter_target.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit shooter targets.
 */

#include "../../anim/anim.h"

namespace vagl
{

  /* My unit class */
  class shooter_target : public unit
  {
  public:
    /* Target coordinates class */
    class target
    {
    public:
      FLT RandomCoef;
      vec3 Point;
      vec3 Norm;
      matr Trans;
      BOOL IsCross = 0;
      BOOL IsShoot = 0;
      BOOL IsFallen = 0;
      FLT FallTime = 0;

       target( VOID ) : Point(vec3()), Norm(vec3()), Trans(matr()), RandomCoef(rand() / (FLT)RAND_MAX)
       {
       }

       /* Evaluate normals function.
        * ARGUEMNTS:
        *   - target position:
        *       vec3 Pos;
        *   - rotate angle:
        *       FLT AngleInDegree;
        * RETURNS: None.
        */
       VOID EvalNormal( vec3 Pos, FLT AngleInDegree )
       {
         Trans = matr::RotateY(AngleInDegree);
         Trans *= matr::Translate(Pos);

         Norm = vec3(sin(mth::D2R(AngleInDegree)), 0, cos(mth::D2R(AngleInDegree)));
         Point = Pos;
       } /* End of 'EvalNormal' function */

    }; /* End of 'target_coords' class */

    target *Targets;
    INT NumOfTargets = 15;
    primitives *Prs;
    INT NumOfShooted = 0;
    prim Pr;
    vertex::std V[4];

    /* Shoter target constructor */
    shooter_target( anim *Ani )
    {
      Targets = new target[NumOfTargets];
      Prs = new primitives;

      V[0].P = vec3(-20, -20, 1);
      V[1].P = vec3(20, -20, 1);
      V[2].P = vec3(-20, 20, 1);
      V[3].P = vec3(20, 20, 1);
      V[0].N = vec3(0, 1, 0);
      V[1].N = vec3(0, 1, 0);
      V[2].N = vec3(0, 1, 0);
      V[3].N = vec3(0, 1, 0);
      V[0].T = vec2(30, 30);
      V[1].T = vec2(-30, 30);
      V[2].T = vec2(30, -30);
      V[3].T = vec2(-30, -30);
      Ani->PrimCreate(&Pr, prim_type::TRISTRIP, V, 4, NULL, 0);
      Pr.Mtl = new material;
      Pr.Mtl->Name = "WIN";
      Pr.Mtl->Tex[0] = Ani->LoadG32("BIN/TEXTURES/WIN.G32", 4);

      Ani->PrimsLoad(Prs, "BIN/MODELS/target.g3dm");
      for (INT i = 0; i < Prs->NumOfPrims; i++)
      {
        Prs->Prims[i].Mtl->Ka = vec3(0.1);
        Prs->Prims[i].Mtl->Kd = vec3(0.8);
        Prs->Prims[i].Mtl->Ks = vec3(0.4);
        Prs->Prims[i].Mtl->Ph = 30.47;
      }
      for (INT i = 0; i < NumOfTargets; i++)
      {
        vec3 v(mth::vec3<FLT>::Rnd1());

        v *= 200;
        Targets[i].EvalNormal(vec3(v.X, 5, v.Z), 0);//360 - mth::R2D(atan(v.X / v.Z)));
      }
    } /* End of 'shooter_target' function */

    ~shooter_target( VOID )
    {
      delete Pr.Mtl;
      delete Prs;
      delete[] Targets;
    }

    /* Response function.
     * ARGUMENTS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Response( anim *Ani ) override
    {
      std::string Buf;

      if (Ani->KeysClick['R'])
      {
        for (INT i = 0; i < NumOfTargets; i++)
          Targets[i].IsShoot = 0;
        NumOfShooted = 0;
      }
      #pragma region Intersection
      for (INT i = 0; i < NumOfTargets; i++)
        if (!Targets[i].IsShoot && (Ani->KeysClick[VK_LBUTTON] || Ani->KeysClick[VK_RETURN]))
        {
          vec3 P = Targets[i].Point;
          vec3 N = Targets[i].Norm;
          FLT D = -(P & N);
          ray Ray = Ani->Camera.CamRay;
          FLT t = - (N[0] * Ray.Org.X + N[1] * Ray.Org.Y + N[2] * Ray.Org.Z + D) / 
                    (N[0] * Ray.Dir.X + N[1] * Ray.Dir.Y + N[2] * Ray.Dir.Z);

          vec3 V = Ray.Org + Ray.Dir * t;

          Targets[i].IsCross = FALSE;
          Targets[i].Point.Y += 3;
          if (V < (Targets[i].Point + 2.0) && V > (Targets[i].Point - 2.0))
            Targets[i].IsCross = TRUE;
          Targets[i].Point.Y -= 3;
          if (Targets[i].IsCross)
            Targets[i].IsShoot = TRUE, NumOfShooted++;
        }
      #pragma endregion
      Buf += std::to_string(Ani->FPS);
      static FLT ReloadTime = 30.47;
      ReloadTime += Ani->GlobalDeltaTime;
      if (ReloadTime > 1)
      {
        SetWindowText(Ani->win::hWnd, Buf.c_str());
        ReloadTime = 0;
      }
    } /* End of 'Response' function */

    /* Render function.
     * ARGUMENTS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Render( anim *Ani ) override
    {
      if (NumOfShooted == NumOfTargets)
      {
        Ani->Camera.SetLocAtUp(vec3(0, -10, 0), vec3(0, -10, 1), vec3(0, 1, 0));
        //Ani->Camera.VP = matr::Ortho(-1, 1, -1, 1, -1, 100);
        Ani->DrawPrim(&Pr, matr::Identity(), 0);
      }
      for (INT i = 0; i < NumOfTargets; i++)
        if (!Targets[i].IsShoot)
        {
          FLT angle;
          if (Ani->Camera.Dir.Z < 0)
            angle = mth::R2D(atan(Ani->Camera.Dir.X / Ani->Camera.Dir.Z));
          else
            angle = 180 + mth::R2D(atan(Ani->Camera.Dir.X / Ani->Camera.Dir.Z));
          Targets[i].EvalNormal(Targets[i].Point + vec3(sin(Ani->Time) * Targets[i].RandomCoef / .03, 0, cos(Ani->Time) * Targets[i].RandomCoef / 3) / 100 * Targets[i].RandomCoef, angle);
          Prs->PrimsDraw(matr::Scale(vec3(1, 1, 0.1)) * Targets[i].Trans, 1 * Prs->NumOfPrims);
        }
        else if (Targets[i].IsFallen)
        {
          Prs->PrimsDraw(matr::Scale(vec3(1, 1, 0.1)) * matr::RotateX(-90) * matr::Translate(vec3(0, -5, -5))* Targets[i].Trans, 1 * Prs->NumOfPrims);
        }
        else
        {
          Targets[i].FallTime += Ani->GlobalDeltaTime;
          Prs->PrimsDraw(matr::Scale(vec3(1, 1, 0.1)) * matr::RotateX(-90 * Targets[i].FallTime / 0.3) * matr::Translate(vec3(0, -5, -5))* Targets[i].Trans, 1 * Prs->NumOfPrims);
          if (Targets[i].FallTime > 0.30)
            Targets[i].IsFallen = TRUE;
        }
    } /* End of 'Render' function */

  }; /* End of 'shooter_target' class */
}
/* END OF 'shooter_target.h' FILE */
