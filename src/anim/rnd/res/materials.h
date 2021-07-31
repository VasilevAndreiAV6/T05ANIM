/* FILE NAME   : materials.h
 * PROGRAMMER  : AV6
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Materials class declaration.
 */

#ifndef __materials_h_
#define __materials_h_

#include "../../../vagl.h"

/* NSF name space */
namespace vagl
{
  class shader_manager;

  /* Material class */
  class material
  {
  public:
    /* Illumination coefficients */
    vec3
      Ka,                   /* Ambient coefficient */
      Kd,                   /* Diffuse coefficient */
      Ks;                   /* Specular coefficient */
    FLT Ph;                 /* Phong power coefficient */
    FLT Trans;              /* Transparency factor */
    INT Tex[8];     /* Texture references from texture table (or -1) */
    INT 
      KaLoc, /* Uniform buffer locations */
      KdLoc,
      KsLoc,
      PhLoc,
      TransLoc,
      TexLoc[8];

    std::string ShdName;
    std::string Name;

    /* Material constructor */
    material( VOID ) : Ka(vec3(0.1)), Kd(vec3(0.8)), Ks(vec3(0.5)), Ph(30.47), Trans(1)
    {
      for (INT i = 0; i < 8; i++)
        Tex[i] = -1;
      ShdName = "DEFAULT";
      Name = "DefaultMtl";
    } /* End of 'material' function */

    VOID Free( VOID )
    {
    }
  }; /* End of 'material' class */

  /* Material manager class */
  class material_manager : public resource_manager<material, std::string>
  {
  public:
    /* Shader manager constructor */
    material_manager( VOID ) : resource_manager<material, std::string>()
    {
    } /* End of 'shader_manager' function */

    /* Add shader function.
     * ARGUMENTS:
     *   - shader name:
     *       CHAR *ShdFileNamePrefix;
     * RETURNS: None.
     */
    material * Add( material &Mtl )
    {
      return resource_manager::add(Mtl);
    } /* End of 'Add' function */

    VOID MtlApply( INT MtlNo )
    {
      //material::Apply();
    }
  }; /* End of 'material_manager' class */
} /* end of 'vagl' namespace */

#endif /* __materials_h_ */

/* END OF 'materials.h' FILE */
