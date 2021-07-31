/* FILE NAME  : unit_btr.cpp
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit BTR.
 */

#include "../anim/anim.h"

namespace vagl
{
  /* BTR unit class */
  class unit_btr : public unit
  {
  public:
    primitives *Prs;

    unit_btr( anim *Ani )
    {
      Prs = new primitives;
      Ani->render::primitives_manager::PrimsLoad(Prs, "BIN/MODELS/btr.g3dm");
    }

    unit_btr( VOID )
    {
      delete Prs;
    }

    VOID Render( anim *Ani ) override
    {
      Prs->PrimsDraw(matr::Scale(vec3(.01)), 0);
    }

  }; /* End of 'unit_btr' class */
}
/* END OF 'unit_btr.h' FILE */
