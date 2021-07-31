/* FILE NAME  : primitives.cpp
 * PROGRAMMER : AV6
 * LAST UPDATE: 26.07.2021
 * PURPOSE    : Primitives definitions.
 */

#include <fstream>

#include "primitives.h"
#include "../../anim.h"

/* Draw array of primitives function.
  * ARGUMENTS:
  *   - pointer to primitives structure:
  *       dg5PRIMS *Prs;
  *   - global transformation matrix:
  *       MATR World;
  * RETURNS: None.
  */
VOID vagl::primitives::PrimsDraw( mth::matr<FLT> World, INT NumOfPrims )
{
  vagl::anim &AnimInstance = vagl::anim::Get();

  World = Trans * World;

  /* Draw all nontransparent primitives */
  for (INT i = 0; i < NumOfPrims; i++)
    if (Prims[i].Mtl->Trans == 1)
      AnimInstance.render::DrawPrim(&Prims[i], World, NumOfPrims);

  /* Draw all transparent primitives * /
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (INT i = 0; i < NumOfPrims; i++)
    if (Prims[i].Mtl->Trans != 1)
      AnimInstance.render::DrawPrim(&Prims[i], World);
  glCullFace(GL_BACK);
  for (INT i = 0; i < NumOfPrims; i++)
    if (Prims[i].Mtl->Trans != 1)
      AnimInstance.render::DrawPrim(&Prims[i], World);
  glDisable(GL_CULL_FACE);
  } /* End of 'PrimsDraw' function */}

/* Load array of primitives from .G3DM file function.
  * ARGUMENTS:
  *   - pointer to primitives structure:
  *       dg5PRIMS *Prs;
  *   - file name:
  *       CHAR *FileName;
  * RETURNS:
  *   (BOOL) TRUE if successful, FALSE otherwise.
  */
BOOL vagl::primitives_manager::PrimsLoad( primitives *Prs, const std::string FileName )
{
  vagl::anim *Ani = vagl::anim::GetPtr();
  INT p, t, m;
  DWORD Sign;         /* == "G3DM" */
  INT NumOfPrims;
  INT NumOfMaterials;
  INT NumOfTextures;
  struct g3dmmtl
  {
    CHAR Name[300];     /* Material name */

    /* Illumination coefficients */
    vec3 Ka = vec3(0), Kd = vec3(0), Ks = vec3(0);    /* Ambient, diffuse, specular coefficients */
    FLT Ph = 0;             /* Phong power coefficient – shininess */
    FLT Trans = 0;          /* Transparency factor */
    DWORD Tex[8] = {0};       /* Texture references */
                        /* (8 time: texture number in G3DM file, -1 if no texture) */

    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;           /* Shader number (uses after load into memory) */
  };

  memset(Prs, 0, sizeof(primitives));

  /* Open file */
  std::fstream f(FileName, std::fstream::in | std::fstream::binary);

  f.seekg(0, std::fstream::end);
  INT flen = (INT)f.tellg();
  f.seekg(0, std::fstream::beg);
 
  BYTE *mem = new BYTE[flen], *ptr = mem;
  f.read(reinterpret_cast<CHAR *>(mem), flen);

  rdr rd(ptr);
 
  rd(&Sign);
  rd(&NumOfPrims);
  rd(&NumOfMaterials);
  rd(&NumOfTextures);

  material *materials = new material[NumOfMaterials];
  g3dmmtl *mtls = new g3dmmtl[NumOfMaterials];
  if (!PrimsAdd(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }
  /* Primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    DWORD MtlNo;              /* Material number in table below (in material section) */

    rd(&NumOfVertexes);
    rd(&NumOfFacetIndexes);
    rd(&MtlNo);

    vertex::std *Vert = new vertex::std[NumOfVertexes];
    INT *Ind = new INT[NumOfFacetIndexes];
    rd(Vert, NumOfVertexes);
    rd(Ind, NumOfFacetIndexes);

    render::EvalBoundBox(Vert, Ind, NumOfFacetIndexes, &Prs->Prims[p].MinBB, &Prs->Prims[p].MaxBB);
    if (Prs->MaxBB < Prs->Prims[p].MaxBB)
      Prs->MaxBB = Prs->Prims[p].MaxBB;
    if (Prs->MinBB > Prs->Prims[p].MinBB)
      Prs->MinBB = Prs->Prims[p].MinBB;
    vagl::render::PrimCreate(&Prs->Prims[p], prim_type::TRIMESH, Vert, NumOfVertexes, Ind, NumOfFacetIndexes);

    delete[] Ind;
    delete[] Vert;
    Prs->Prims[p].MtlNo = MtlNo;
  }
  /* Materials */
  rd(mtls, NumOfMaterials);
  for (m = 0; m < NumOfMaterials; m++)
  {
    materials[m].Name = mtls[m].Name;
    materials[m].Ka = mtls[m].Ka;
    materials[m].Kd = mtls[m].Kd;
    materials[m].Ks = mtls[m].Ks;
    materials[m].Ph = mtls[m].Ph;
    materials[m].Trans = mtls[m].Trans;
    for (INT i = 0; i < 8; i++)
      materials[m].Tex[i] = (mtls[m].Tex[i] == -1 ? -1 : mtls[m].Tex[i] + Ani->render::texture_manager::TextureSize);
  }

  for (INT i = 0; i < NumOfPrims; i++)
    if (Prs->Prims[i].MtlNo != -1)
      Prs->Prims[i].Mtl = Ani->render::material_manager::Add(materials[Prs->Prims[i].MtlNo]);
    else
      Prs->Prims[i].Mtl = new material;
  /* Textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    struct textur
    {
      CHAR Name[300]; /* Texture name */
      DWORD W, H;     /* Texture image size in pixels */
      DWORD C;        /* Texture image components (1-mono, 3-bgr or 4-bgra) */
    } *tex = reinterpret_cast<textur *>(ptr);



    ptr += sizeof(*tex);
    Ani->render::texture_manager::AddImage(tex->Name, tex->W, tex->H, tex->C, ptr);
    ptr += tex->W * tex->H * tex->C;
  }

  delete[] mem;
  delete[] materials;
  delete[] mtls;
  return TRUE;
} /* End of 'DG5_RndPrimsLoad' function */

/* END OF 'primitives.cpp' FILE */
