/* FILE NAME  : mth_Camera.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Space Camera handle module.
 */

#ifndef __mth_camera_h_
#define __mth_camera_h_

#include "mth_vec3.h"
#include "mth_matr.h"
#include "mth_ray.h"

/* Math library namespace */
namespace mth
{
  /* Space Camera handle class */
  template<class type>
    class Camera
    {
    private:
      /* Update Camera project parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateProj( VOID )
      {
        type ratio_x = Size / 2, ratio_y = Size / 2;

        if (FrameW >= FrameH)
          ratio_x *= (type)FrameW / FrameH;
        else
          ratio_y *= (type)FrameH / FrameW;
        Proj = matr<type>::Frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjDist, FarClip);
      } /* End of 'UpdateProj' function */

      /* Update Camera view parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateView( VOID )
      {
        View = matr<type>::View(Loc, At, Up);
        Dir[0] = -View[2];
        Dir[1] = -View[6];
        Dir[2] = -View[10];
        Right[0] = View[0];
        Right[1] = View[4];
        Right[2] = View[8];
      } /* End of 'UpdateView' function */

    public:
      vec3<type>
        Loc,            // Camera location
        Dir, Up, Right, // basis Camera directions
        At;             // Camera pivot point
      type
        ProjDist,       // near project plane distance
        FarClip,        // far project plane distance
        Size;           // inner project plane rectangle size
      INT
        FrameW, FrameH; // Camera frame size
      matr<type>
        View,           // view matrix
        Proj,           // projection matrix
        VP;             // View and Proj madtrix production
      ray<type>
        CamRay;         // Camera ray

      /* Default constructor */
      Camera( VOID ) :
        Loc(0, 0, 5), Dir(0, 0, -1), Up(0, 1, 0), Right(0, 0, -1), At(0, 0, 0),
        ProjDist(0.1), FarClip(1000), Size(0.1),
        FrameW(30), FrameH(30), CamRay(ray<type>())
      {
        UpdateProj();
        UpdateView();
        VP = View * Proj;
      } /* End of 'matr' function */

      /* Set project Camera parameters function.
       * ARGUMENTS:
       *   - new near project plane size:
       *       type NewSize;
       *   - new project distance:
       *       type NewProjDist;
       *   - new far clip plane distance:
       *       type FarC;
       * RETURNS:
       *   (Camera &) self reference.
       */
      Camera & SetProj( type NewSize, type NewProjDist, type NewFarClip )
      {
        // Store new projection parameters
        ProjDist = NewProjDist;
        FarClip = NewFarClip;
        Size = NewSize;

        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'SetProj' function */

      /* Set Camera project pixel size function.
       * ARGUMENTS:
       *   - new frame size in pixels:
       *       type NewFrameW, NewFrameH;
       * RETURNS:
       *   (Camera &) self reference.
       */
      Camera & Resize( type NewFrameW, type NewFrameH )
      {
        FrameW = NewFrameW;
        FrameH = NewFrameH;

        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'Resize' function */

      /* Set Camera location and orientation function.
       * ARGUMENTS:
       *   - new Camera location:
       *       const vec3<type> &L;
       *   - new Camera pivot point:
       *       const vec3<type> &A;
       *   - new Camera approx up direction:
       *       const vec3<type> &U;
       * RETURNS:
       *   (Camera &) self reference.
       */
      Camera & SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U = vec3<type>(0, 1, 0) )
      {
        Loc = L;
        At = A;
        Up = U;

        UpdateView();
        VP = View * Proj;
        return *this;
      } /* End of 'SetLocAtUp' function */
 
      /* Camera rotate function.
       * ARGUMENTS:
       *   - rotate axis:
       *       const vec3<type> &Axis;
       *   - rotation angle (in degree):
       *       type AngleInDegree;
       * RETURNS:
       *   (Camera &) self reference.
       */
      Camera & Rotate( const vec3<type> &Axis, type AngleInDegree )
      {
        matr<type> m = matr<type>::Translate(-At) * matr<type>::Rotate(Axis, AngleInDegree) * matr<type>::Translate(At);
        Loc = m.TransformPoint(Loc);
        Up = m.TransformVector(Up);
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Rotate' function */

      /* Camera movement function.
       * ARGUMENTS:
       *   - movement directions:
       *       const vec3<type> &Direction;
       * RETURNS:
       *   (Camera &) self reference.
       */
      Camera & Move( const vec3<type> &Direction )
      {
        Loc += Direction;
        At += Direction;
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Move' function */

      /* Build ray from screen to world function.
       * ARGUMENTS:
       *   - frame pixel coordinates:
       *       FLT Sx, Sy;
       * RETURNS:
       *   (ray) result ray.
       */
      ray<type> Frame2Ray( type Sx, type Sy )
      {
        type ratio_x = Size, ratio_y = Size;

        if (FrameW >= FrameH)
          ratio_x *= (type)FrameW / FrameH;
        else
          ratio_y *= (type)FrameH / FrameW;
        
        vec3<type> A = Dir * ProjDist;
        vec3<type> B = Right * (Sx - FrameW / 2.0) * ratio_x / FrameW;
        vec3<type> C = Up * (-Sy + FrameH / 2.0) * ratio_y / FrameH;
        vec3<type> X = A + B + C;
        CamRay.Org = Loc + X;
        CamRay.Dir = ~X;
        return CamRay;
      } /* End of 'Frame2Ray' function */

    }; /* End of 'Camera' class */
} /* end of 'mth' namespace */

#endif /* __mth_camera_h_ */

/* END OF 'mth_camera.h' FILE */
