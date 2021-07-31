/* FILE NAME   : render.h
 * PROGRAMMER  : Andrei Vasilev.
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Render class declaration.
 */

#ifndef __render_h_
#define __render_h_

#include "res/resource.h"
#include "res/shader.h"
#include "res/primitives.h"
#include "res/prim.h"
#include "res/materials.h"
#include "res/textures.h"
#include "../timer.h"

namespace vagl
{
  // Render system type
  class render : public shader_manager, public material_manager, public texture_manager, public primitives_manager
  {
  private:
    const HWND &hWnd;           // Window to dender descriptor
    const HDC hDC;              // Window to dender device context handle
    const INT &FrameW, &FrameH; // Window to render frame size

    HGLRC hGLRC;                // OpenGL rendering context

    vagl::timer Timer;

  public:
    // User Camera
    Camera Camera;
    /* Render system type constructor.
     * ARGUMENTS:
     *   - window handle ref:
     *       HWND &hWnd;
     *   - window size ref:
     *       INT &W, &H;
     */
    render( HWND &hWnd, INT &W, INT &H );

    /* Render system type destructor.
     * ARGUMENTS: None.
     */
    ~render( VOID );

    /* Rendring resizing function.
     * ARGUMENTS: None
     * RETURNS: None.
     */
    VOID Resize( VOID );

    /* Rendring copy frame function.
     * ARGUMENTS:
         - WINDOW HANDLE:
             HWND hWnd;
     * RETURNS: None.
     */
    VOID CopyFrame( VOID );

    /* Rendring start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Start( VOID );

    /* Rendring end function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */ 
    VOID End( VOID );

    /* Debug output function.
     * ARGUMENTS:
     *   - source APi or device:
     *      UINT Source;
     *   - error type:
     *      UINT Type;
     *   - error message id:
     *      UINT Id, 
     *   - message severity:
     *      UINT severity, 
     *   - message text length:
     *      INT Length, 
     *   - message text:
     *      CHAR *Message, 
     *   - user addon parameters pointer:
     *      VOID *UserParam;
     * RETURNS: None.
     */
    static VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                        INT Length, const CHAR *Message, const VOID *UserParam );

    /***
     * Primitive methods
     ***/

    /* Load primitive from '*.OBJ' file function.
     * ARGUMENTS:
     *   - pointer to primitive to create:
     *       prim *Pr;
     *   - '*.OBJ' file name:
     *       CHAR *FileName;
     * RETURNS:
     *   (BOOL) TRUE if success, FALSE otherwise.
     */
    BOOL LoadPrim( prim *Pr, const std::string FileName );

    /* Draw render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      prim *Pr;
     *   - matrix:
     *      const matr &World;
     * RETURNS: None.
     */
    VOID DrawPrim( prim *Pr, const matr &World, INT NumOfPrims );

    /* Bound box calculate function.
     * ARGUMENTS:
     *   - vertex array:
     *      vertex *V;
     *   - index:
     *      INT *I;
     *   - num of index:
     *      INT NoofI;
     *   - bound box pointers:
     *      vec3 *MinBB, *MaxBB;
     * RETURNS: None.
     */
    static VOID EvalBoundBox( vertex::std *V, INT *I, INT NoofI, vec3 *MinBB, vec3 *MaxBB )
    {
      INT i;

      for (i = 3; i < NoofI; i += 3)
      {
        *MinBB = ::mth::VecMinBB(*MinBB, V[I[i]].P);
        *MaxBB = ::mth::VecMaxBB(*MaxBB, V[I[i]].P);
      }
    } /* End of 'vagl::render::EvalBoundBox' function */

    /* Material storage initializate function.
     * ARGUMENTS:
     *   - Material number:
     *       INT MtlNo;
     * RETURNS:
     *   (INT) shader program id.
     */
    INT Apply( material *Mtl );

    VOID MtlUpdate( material *Mtl )
    {
      INT prg = shader_manager::Find(Mtl->ShdName)->Prg;

      Mtl->KaLoc = glGetUniformLocation(prg, "Ka");
      Mtl->KdLoc = glGetUniformLocation(prg, "Kd");
      Mtl->KsLoc = glGetUniformLocation(prg, "Ks");
      Mtl->PhLoc = glGetUniformLocation(prg, "Ph");
      Mtl->TransLoc = glGetUniformLocation(prg, "Trans");

      for (INT i = 0; i < 8; i++)
      {
        CHAR tname[] = "IsTexture0";

        tname[9] = '0' + i;
        Mtl->TexLoc[i] = glGetUniformLocation(prg, tname);
        if (Mtl->Tex[i] != -1)
        {
          /* Activate sampler */
          glActiveTexture(GL_TEXTURE0 + i);
          /* Bind texture to sampler */
          INT Id = render::texture_manager::Find(Mtl->Tex[i])->TexId;
          glBindTexture(GL_TEXTURE_2D, Id);
        }

      }
    }

    /* Free render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      prim*Pr;
     * RETURNS: None.
     */
    static VOID PrimFree( prim *Pr )
    {
      /* making an array of vertices active */
      glBindVertexArray(Pr->VA);
      /* unhooking the buffer */
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &Pr->VBuf);
      /* making an array of vertices inactive */
      glBindVertexArray(0);
      glDeleteVertexArrays(1, &Pr->VA);

      /* making an array of indexes inactive */
      glDeleteBuffers(1, &Pr->IBuf);

      delete Pr->Mtl;
      memset(Pr, 0, sizeof(prim));
    } /* End of 'DG5_RndPrimFree' function */

    static BOOL PrimCreate( prim *Pr, prim_type Type, vertex::std *V, INT NoofV, INT *Ind, INT NoofI )
    {
      memset(Pr, 0, sizeof(prim));

      Pr->Mtl = new material;
      ///Pr->NumOfEl = NoofI;
      //PrimAutoNorm(V, Ind, 24);
      if (V != NULL)
      {
        glGenBuffers(1, &Pr->VBuf);
        glGenVertexArrays(1, &Pr->VA);
        /* making an array of vertices active */
        glBindVertexArray(Pr->VA);
        /* making a buffer active */
        glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
        /* merging data */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex::std) * NoofV, V, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vertex::std), (VOID *)0);                                 /* position */
        glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vertex::std), (VOID *)sizeof(vec3));                      /* texture coordinates */
        glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vertex::std), (VOID *)(sizeof(vec3) + sizeof(vec2)));     /* normal */
        glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vertex::std), (VOID *)(sizeof(vec3) * 2 + sizeof(vec2))); /* color */

        /* turning on attributes */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        /* turn off vertex array */
        glBindVertexArray(0);
      }
      if (Ind != NULL && NoofI != 0)
      {
        glGenBuffers(1, &Pr->IBuf);
        Pr->NumOfElements = NoofI;
        /* making a buffer active */
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
        /* merging data */
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * Pr->NumOfElements, Ind, GL_STATIC_DRAW);
      }
      else
        Pr->NumOfElements = NoofV;

      Pr->Transform = matr::Identity();
      Pr->Type = Type;
      return TRUE;
    } /* End of 'PrimCreate' function */

    /* Auto normal function.
     * ARGUMENTS:
     *   - vertex array:
     *      dg5VERTEX *V;
     *   - index:
     *      INT *I;
     *   - num of index:
     *      INT NoofI;
     * RETURNS: None.
     */
    static VOID PrimAutoNorm( vertex::std *V, INT *I, INT NoofI )
    {
      INT i;

      for (i = 0; i < NoofI; i += 3)
      {
        vec3 N = ((V[I[i + 1]].P - V[I[i]].P) % (V[I[i + 2]].P - V[I[i]].P));
        if (N.Length2() != 0)
          N.Normalize();

        V[I[i + 0]].N = ~(V[I[i + 0]].N + N);
        V[I[i + 1]].N = ~(V[I[i + 1]].N + N);
        V[I[i + 2]].N = ~(V[I[i + 2]].N + N);
      }
    } /* End of 'DG5_RndAutoNorm' function */

  };
}

#endif /* __render_h_ */


/* END OF 'render.h' FILE */