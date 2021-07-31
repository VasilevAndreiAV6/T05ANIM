/* FILE NAME  : main.cpp
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Entry point.
 */

#include "units/units.h"

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  vagl::anim *Ani = vagl::anim::GetPtr();

  //vagl::unit *control = new vagl::unit_control(Ani);
  //vagl::unit *btr = new vagl::unit_btr(Ani);
  //vagl::unit *sky = new vagl::unit_sky_sphere(Ani);
  //vagl::unit *mebius = new vagl::unit_mebius(Ani);
  vagl::unit *cntrl = new vagl::shooter_control(Ani);
  vagl::unit *map = new vagl::shooter_map(Ani);
  vagl::unit *target = new vagl::shooter_target(Ani);

  Ani->Scene << /*sky << */cntrl << map << target;

  Ani->Run();
  return 0;
} /* End of 'WinMain' function */

/* END OF 'main.cpp' FILE */
