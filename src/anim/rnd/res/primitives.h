/* FILE NAME   : prim.h
 * PROGRAMMER  : Andrei Vasilev.
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Primitives class.
 */

#ifndef __primitives_h_
#define __primitives_h_


#include "resource.h"
#include "prim.h"

/* NSF name space */
namespace vagl
{
  /* Unstructured data file reader class */
  class rdr
  {
  private:
    // Memory pointer reference
    BYTE *&Ptr;
 
  public:
    /* Class constructor.
      * ARGUMENTS:
      *   - reference to memory pointer:
      *       BYTE *&PtrRef;
      */
    rdr( BYTE *&PtrRef ) : Ptr(PtrRef)
    {
    } /* End of 'rdr' function */
 
    /* Read data function.
      * ARGUMENT:
      *   - read data pointer:
      *       type *Data;
      *   - read data count:
      *       INT Count;
      * RETURNS: None.
      */
    template<typename type>
      VOID operator()( type *Data, const INT Count = 1 )
      {
        if (Count == 1)
          *Data = *(type *)Ptr, Ptr += sizeof(type);
        else
          memcpy(Data, Ptr, sizeof(type) * Count), Ptr += sizeof(type) * Count;
      } /* End of 'operator()' function */
  }; /* End of 'rdr' class */

  /* Primitives handle class */
  class primitives
  {
  private:
 
  public:
    INT NumOfPrims; /* Number of primitives in array */  
    prim *Prims;    /* Array of primitives */
    matr Trans;     /* Common transformation matrix */
    std::string Name;
    vec3 MinBB, MaxBB;

    /* Class constructor */
    primitives( VOID ) : NumOfPrims(0), Prims(nullptr), Trans(matr::Identity()), Name {}
    {
    } /* End of 'prim' function */
 
    /* Class destructor */
    ~primitives( VOID )
    {
    } /* End of '~prim' function */

    VOID Free( VOID )
    {
    }

    /* Draw array of primitives function.
     * ARGUMENTS:
     *   - pointer to primitives structure:
     *       dg5PRIMS *Prs;
     *   - global transformation matrix:
     *       MATR World;
     * RETURNS: None.
     */
    VOID PrimsDraw( matr World, INT NumOfPrims );

    /* Bound box calculate function.
     * ARGUMENTS:
     *   - vertex array:
     *      dg5VERTEX *V;
     *   - index:
     *      INT *I;
     *   - num of index:
     *      INT NoofI;
     *   - bound box pointers:
     *      VEC *MinBB, *MaxBB;
     * RETURNS: None.
     */
    VOID EvalBoundBox( vertex::std *V, INT *I, INT NoofI, vec3 *MinBB, vec3 *MaxBB )
    {
      INT i;

      for (i = 3; i < NoofI; i += 3)
      {
        *MinBB = mth::VecMinBB(*MinBB, V[I[i]].P);
        *MaxBB = mth::VecMaxBB(*MaxBB, V[I[i]].P);
      }
    } /* End of 'DG5_RndAutoNorm' function */

  }; /* End of 'primitives' class */

 
  /* Primitives manager class */
  class primitives_manager : public resource_manager<primitives, std::string>
  {
  public:
    /* Primitives manager constructor */
    primitives_manager( VOID ) : resource_manager<primitives, std::string>()
    {
    } /* End of 'primitives_manager' function */

    /* Add primitives function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    BOOL PrimsAdd( primitives *Prs, INT NumOfPrims )
    {
      memset(Prs, 0, sizeof(primitives));

      if ((Prs->Prims = reinterpret_cast<prim *>(malloc(sizeof(prim) * NumOfPrims))) == NULL)
        return FALSE;

      memset(Prs->Prims, 0, sizeof(prim) * NumOfPrims);
      Prs->NumOfPrims = NumOfPrims;
      Prs->Trans = matr::Identity();

      return TRUE;
    } /* End oif 'PrimsAdd' function */

    static VOID PrimsFree( primitives *Prs )
    {
      for (INT i = 0; i < Prs->NumOfPrims; i++)
        ;//render::PrimFree(&Prs->Prims[i]);
      delete[] Prs->Prims;
    }

    /* Load array of primitives from .G3DM file function.
     * ARGUMENTS:
     *   - pointer to primitives structure:
     *       dg5PRIMS *Prs;
     *   - file name:
     *       CHAR *FileName;
     * RETURNS:
     *   (BOOL) TRUE if successful, FALSE otherwise.
     */
    BOOL PrimsLoad( primitives *Prs, const std::string FileName );
  };
} /* end of 'vagl' namespace */

#endif /* __prim_h_ */

/* END OF 'primitives.h' FILE */
