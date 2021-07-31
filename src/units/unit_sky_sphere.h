    /* FILE NAME  : unit_sky_sphere.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit shooter targets.
 */

#include <map>

#include "../anim/anim.h"

namespace vagl
{

  /* My unit class */
  class unit_sky_sphere : public unit
  {
  public:
    prim *Pr;

    /* Shoter target constructor */
    unit_sky_sphere( anim *Ani )
    {
      Pr = new prim;
      Ani->PrimCreate(Pr, prim_type::POINTS, NULL, 1, NULL, 0);
      Pr->Mtl->Tex[0] = Ani->LoadG32("BIN/TEXTURES/clouds.g32", 4);
      Ani->shader_manager::Add("SKY");
      Pr->Mtl->ShdName = "SKY";
      Pr->Mtl->Ph = 1;
    } /* End of 'unit_sky_sphere' function */

    /* Response function.
     * ARGUMENTS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Response( anim *Ani ) override
    {
    } /* End of 'Response' function */

    /* Render function.
     * ARGUMENTS:
     *   - animation instance:
     *       anim *Ani;
     * RETURNS: None.
     */
    VOID Render( anim *Ani ) override
    {
      glDisable(GL_DEPTH_TEST);
      glDepthMask(FALSE);
      Ani->DrawPrim(Pr, matr::Identity(), 0);
      glDepthMask(TRUE);
      glEnable(GL_DEPTH_TEST);
    } /* End of 'Render' function */

  }; /* End of 'unit_sky_sphere' class */
}
/* END OF 'unit_sky_sphere.h' FILE */
