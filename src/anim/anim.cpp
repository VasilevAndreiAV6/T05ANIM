/* FILE NAME  : anim.cpp
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Animation class functoions definitions.
 */

#include <cstdio>

#include "anim.h"

vagl::anim vagl::anim::Instance;

/* Init callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Init( VOID )
{
  SetCursorPos(W / 2, H / 2);
} /* End of 'vagl::anim::Init' function */

/* Close callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Close( VOID )
{
} /* End of 'vagl::anim::Close' function */

/* Resize callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Resize( VOID )
{
  render::Resize();
} /* End of 'vagl::anim::Resize' function */

/* Timer callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Timer( VOID )
{
  CHAR Buf[100];

  sprintf_s(Buf, "%.0f", FPS);
  //SetWindowTextA(win::hWnd, Buf);
  Render();
  CopyFrame();
} /* End of 'vagl::anim::Timer' function */

/* Activate callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Activate( VOID )
{
} /* End of 'vagl::anim::Activate' function */

/* Idle callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Idle( VOID )
{
  Render();
  CopyFrame();
} /* End of 'vagl::anim::Idle' function */

/* Erase callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Erase( HDC hDC )
{
} /* End of 'vagl::anim::Erase' function */

/* Paint callback function.
 * ARGMUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::anim::Paint( HDC hDC )
{
} /* End of 'vagl::anim::Paint' function */

/* Add unit to scene operator.
 * ARGUMENTS:
 *   - name of the unit:
 *       const std::string &Name;
 * RETURNS:
 *   (scene &) Self-reference.
 */
vagl::scene & vagl::scene::operator<<( const std::string &Name )
{
  anim &Ani = anim::Get();
  if (Ani.UnitNames.find(Name) != Ani.UnitNames.end())
  {
    Units << Ani.UnitNames[Name]();

    return *this;
  }
  return *this;
} /* End of 'bogl::scene::operator<<' function */

/* END OF 'anim.cpp' FILE */
