/* FILE NAME  : shooter_control.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit shooter control.
 */

#include "../../anim/anim.h"

namespace vagl
{
  /* My unit class */
  class shooter_control : public unit
  {
  public:
    primitives *Prs;                                                          // Hands primitive
    matr HandsRotate = matr::Translate(vec3(-1, -3, -1)) * matr::RotateY(180); // Rotate hands matrix
    matr HandsPos = matr::Identity();                                         // Position hands matrix
    BOOL IsAim = FALSE;
    BOOL IsShoot = FALSE;
    BOOL IsJump = FALSE;
    
    /* Shooter control constructor */
    shooter_control( anim *Ani )
    {
      //ShowCursor(FALSE);
      Ani->Camera.Resize(Ani->W, Ani->H);
      Prs = new vagl::primitives;
      Ani->render::primitives_manager::PrimsLoad(Prs, const_cast<CHAR *>("BIN/MODELS/gun.g3dm"));
      for (INT i = 0; i < Prs->NumOfPrims; i++)
      {
        Prs->Prims[i].Mtl->Ka = vec3(0.1);
        Prs->Prims[i].Mtl->Kd = vec3(0.8);
        Prs->Prims[i].Mtl->Ks = vec3(0.4);
        Prs->Prims[i].Mtl->Ph = 30.47;
      }
      Prs->Prims[1].Mtl->Ph = 100000;
      Ani->Camera.Move(vec3(0, Ani->Camera.Up.Y * 5, 0));
    } /* End of 'shooter_control' function */

    ~shooter_control( VOID )
    {
      delete Prs;
    }

    /* Response function.
     * ARGUMENTS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Response( anim *Ani ) override
    {
      POINT pt;

      GetCursorPos(&pt);
      POINT pt1 = pt;
      ScreenToClient(Ani->win::hWnd, &pt);
      pt.x = pt1.x - pt.x;
      pt.y = pt1.y - pt.y;
      SetCursorPos(Ani->W / 2 + pt.x, Ani->H / 2 + pt.y);
      Ani->input::mouse::Response();
      if (Ani->KeysClick['F'])
        Ani->FlipFullScreen();
      if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
            Ani->Camera.Move(((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) +
                           Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime).GetVecInPlaneXZ());
      //if (Ani->Keys[VK_LBUTTON])
      {
        Ani->Camera.Rotate(mth::vec3<FLT>(0, 1, 0), (10 * 1 * Ani->MouseDX +
                                              3 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        HandsRotate *= matr::Rotate(-mth::vec3<FLT>(0, 1, 0), (10 * -1 * Ani->MouseDX +
                                              -3 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
      }

      //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir.Y < 0.95 && Ani->MouseDY <= 0)
      if (Ani->Camera.Dir.Y < 0.95)
      {
        Ani->Camera.Rotate(Ani->Camera.Right, (10 * 1 * Ani->MouseDY +
                                                3 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        HandsRotate *= matr::Rotate(-Ani->Camera.Right, (10 * -1 * Ani->MouseDY +
                                                -3 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
      }

      //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir.Y > -0.95 && Ani->MouseDY >= 0)
      if (Ani->Camera.Dir.Y > -0.95)
      {
        Ani->Camera.Rotate(Ani->Camera.Right, (10 * 1 * Ani->MouseDY +
                                                3 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        HandsRotate *= matr::Rotate(-Ani->Camera.Right, (10 * -1 * Ani->MouseDY +
                                                -3 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
      }

      if (Ani->KeysClick[VK_SPACE])
        IsJump = TRUE;

      if (IsJump)
      {
        static DBL JumpTime;
        JumpTime += Ani->GlobalDeltaTime;
        Ani->Camera.Move(vec3(0, cos(PI * JumpTime) / 20, 0));
        if (Ani->Camera.Loc.Y < 5)
        {
          IsJump = 0;
          JumpTime = 0;
        }
      }

      if (Ani->KeysClick[VK_LBUTTON])
        IsShoot = TRUE;

      if (IsShoot)
      {
        static DBL ShotTime;
        ShotTime += Ani->GlobalDeltaTime;
        if (ShotTime < 0.05)
          HandsRotate = matr::RotateX(-2 * ShotTime) * HandsRotate;
        if (ShotTime > 0.05)
        {
          HandsRotate = matr::RotateX(2 * (ShotTime - 0.0518)) * HandsRotate;
        }
        if (ShotTime > 0.10)
        {
          IsShoot = 0;
          ShotTime = 0;
        }
      }

      vec3 Pos = vec3(Ani->Camera.Loc.X, Ani->Camera.Loc.Y, Ani->Camera.Loc.Z);
      HandsPos = matr::Translate(Pos);
      if (Ani->Keys[VK_RBUTTON] || Ani->Keys[VK_RCONTROL])
      {
        if (!IsAim)
        {
          static FLT AimTime;
          AimTime += Ani->GlobalDeltaTime;
          Ani->Camera.SetProj(0.1, 0.1 + 0.5 * AimTime, 10000);
          HandsRotate = matr::Translate(vec3(18 * Ani->GlobalDeltaTime, 1 * Ani->GlobalDeltaTime, -5 * Ani->GlobalDeltaTime)) * HandsRotate;
          if (AimTime > 0.1)
          {
            IsAim = 1;
            AimTime = 0;
          }
        }
      }
      else if ((!Ani->Keys[VK_RBUTTON]) || (!Ani->Keys[VK_RCONTROL]))
      {
        if (IsAim)
        {
          static FLT AimTime;
          AimTime += Ani->GlobalDeltaTime;
          Ani->Camera.SetProj(0.1, 0.15 - 0.5 * AimTime, 10000);
          HandsRotate = matr::Translate(vec3(-18 * Ani->GlobalDeltaTime, -1 * Ani->GlobalDeltaTime, 5 * Ani->GlobalDeltaTime)) * HandsRotate;
          if (AimTime > 0.1)
          {
            IsAim = 0;
            AimTime = 0;
          }
        }
      }
    } /* End of 'Response' function */

    /* Render function.
     * ARGUMETNS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Render( anim *Ani ) override
    {
      Prs->PrimsDraw(matr::Scale(vec3(0.1)) * HandsRotate * HandsPos, 1 * Prs->NumOfPrims);
    } /* End of 'Render' function */

  }; /* End of 'unit_control' class */
}
/* END OF 'shooter_control.h' FILE */
