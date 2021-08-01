/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : render.cpp
 * PROGRAMMER  : Andrei Vasilev.
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Resources.
 *               Render class.
 */

#include <cstdio> 
#include "render.h"
#include "../anim.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

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
VOID APIENTRY vagl::render::glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                           INT Length, const CHAR *Message, const VOID *UserParam )
{
  INT len = 0;
  static CHAR Buf[10000];
 
  /* Ignore non-significant error/warning codes */
  if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
    return;

  len += sprintf_s(Buf + len, 10000, "Debug message (%i) %s\n", Id, Message);
  switch (Source)
  {
  case GL_DEBUG_SOURCE_API:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: API%s");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: Application");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    len += sprintf_s(Buf + len, 10000, "%s", "Source: Other");
    break;
  }
  len += sprintf_s(Buf + len, 10000, "%s", "\n");
 
  switch (Type)
  {
  case GL_DEBUG_TYPE_ERROR:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Error");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Deprecated Behaviour");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Undefined Behaviour");
    break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Portability");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Performance");
    break;
  case GL_DEBUG_TYPE_MARKER:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Marker");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Push Group");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Pop Group");
    break;
  case GL_DEBUG_TYPE_OTHER:
    len += sprintf_s(Buf + len, 10000, "%s", "Type: Other");
    break;
  }
  len += sprintf_s(Buf + len, 10000, "%s", "\n");
 
  switch (Severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    len += sprintf_s(Buf + len, 10000, "%s", "Severity: high");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    len += sprintf_s(Buf + len, 10000, "%s", "Severity: medium");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    len += sprintf_s(Buf + len, 10000, "%s", "Severity: low");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    len += sprintf_s(Buf + len, 10000, "%s", "Severity: notification");
    break;
  }
  len += sprintf_s(Buf + len, 10000, "%s", "\n\n");
 
  OutputDebugString(Buf);
} /* End of 'glDebugOutput' function */

/* Render system type constructor.
 * ARGUMENTS:
 *   - window handle ref:
 *       HWND &hWnd;
 *   - window size ref:
 *       INT &W, &H;
 */
vagl::render::render( HWND &hWnd, INT &W, INT &H ) :
  hWnd(hWnd), hDC(GetDC(hWnd)), FrameW(W), FrameH(H)
{
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    0
  };
 
  // OpenGL init: pixel format setup
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(hDC, i, &pfd);
 
  // OpenGL init: setup rendering context
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);
 
  // Initialize GLEW library
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(hWnd, "Error OpenGL initialization", "ERROR", MB_ICONERROR | MB_OK);
    exit(0);
  } 

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(hWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
  glDebugMessageCallback(glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
 
  /* Enable a new OpenGL profile support */
  UINT nums;
  wglChoosePixelFormatARB(hDC, PixelAttribs, nullptr, 1, &i, &nums);
  HGLRC hRC = wglCreateContextAttribsARB(hDC, nullptr, ContextAttribs);
 
  wglMakeCurrent(nullptr, nullptr);
  wglDeleteContext(hGLRC);
 
  hGLRC = hRC;
  wglMakeCurrent(hDC, hGLRC);

  // Set default OpenGL parameters
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);
 
  // Set default render parameters
  glClearColor(0.6, 0.6, 0.6, 1);
  shader_manager::Add("DEFAULT");

  alutInit(NULL, 0);
  alGetError();
} // End of 'render' constructor

/* Render system type destructor.
 * ARGUMENTS: None.
 */
vagl::render::~render( VOID )
{
  wglMakeCurrent(nullptr, nullptr);
  wglDeleteContext(hGLRC);
  ReleaseDC(hWnd, hDC);
} // End of 'render' destructor

/* Rendring resizing function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID vagl::render::Resize( VOID )
{
  glViewport(0, 0, FrameW, FrameH);

  Camera.Resize(FrameW, FrameH);
} // End of 'vagl::render::Resize' function

/* Rendring copy frame function.
  * ARGUMENTS:
      - WINDOW HANDLE:
          HWND hWnd;
  * RETURNS: None.
  */
VOID vagl::render::CopyFrame( VOID )
{
  wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
} // End of 'vagl::render::CopyFrame' function

/* Rendring start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID vagl::render::Start( VOID )
{
  anim *Ani = anim::GetPtr();
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Camera.Frame2Ray(Ani->W / 2, Ani->H / 2);

  static shader *shd = shader_manager::Find("DEFAULT");
  //static shader *shd1 = shader_manager::Find("SKY");
#if _DEBUG
  static FLT ReloadTime = 30.47;

  ReloadTime += Ani->GlobalDeltaTime;
  if (ReloadTime > 1)
  {
    shd = shader_manager::Find("DEFAULT");
    //shd1 = shader_manager::Find("SKY");
    //ReloadTime = 0;
  }
#endif /* _DEBUG */

  INT prg = shd->Prg;
  //INT prg1 = shd1->Prg;
  INT CamLocation = shd->CamLocation;
  //INT CamDirLoc = shd1->CamDirLoc;
  //INT CamRightLoc = shd1->CamRightLoc;
  //INT CamUpLoc = shd1->CamUpLoc;
  glUseProgram(prg);
  if (CamLocation != -1)
    glUniform3fv(CamLocation, 1, Ani->Camera.Loc);
  //glUseProgram(prg1);
  //if (CamDirLoc != -1)
  //  glUniform3fv(CamDirLoc, 1, Ani->Camera.Dir);
  //if (CamRightLoc != -1)
  //  glUniform3fv(CamRightLoc, 1, Ani->Camera.Right);
  //if (CamUpLoc != -1)
  //  glUniform3fv(CamUpLoc, 1, Ani->Camera.Up);
} /* End of 'vagl::render::Start' function */

/* Rendring end function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */ 
VOID vagl::render::End( VOID )
{
  glFinish();
} // End of 'vagl::render::End' function

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       prim *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL vagl::render::LoadPrim( prim *Pr, const std::string FileName )
{
  INT
    noofv = 0,
    noofi = 0;
  FILE *F;
  CHAR Buf[1000];

  /* Open file */
  fopen_s(&F, FileName.c_str(), "r");
  if (F == NULL)
    return FALSE;

  /* Count vertex and index quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      noofv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      noofi++;
  }

  std::vector<INT> I(3 * noofi);
  std::vector<vertex::std> V(noofv);

  /* Read vertices and facets data */
  rewind(F);
  noofv = noofi = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf_s(Buf + 2, "%f%f%f", &x, &y, &z);
      V[noofv].P = vec3(x, y, z);
      V[noofv++].C = vec4(x, y, z, 1);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      /* Read one of possible facet references */
      sscanf_s(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf_s(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf_s(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf_s(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      n1--;
      n2--;
      n3--;
      I[noofi++] = n1;
      I[noofi++] = n2;
      I[noofi++] = n3;
    }
  }

  vagl::topology::trimesh<vertex::std> T(V, I);
  /* making an auto normalize */
  //T.EvalNormals();
  T.EvalNormals();

  vagl::topology::base<vertex::std> *B = dynamic_cast<vagl::topology::base<vertex::std> *>(&T);
  /* create primitive */
  Pr->Create(*B);

  fclose(F);
  return TRUE;
} /* End of 'vagl::render::LoadPrim' function */

/* Draw render primitive function.
 * ARGUMENTS:
 *   - primitive:
 *      prim *Pr;
 *   - matrix:
 *      const matr &World;
 * RETURNS: None.
 */
VOID vagl::render::DrawPrim( prim *Pr, const matr &World, INT NumOfPrims )
{
  matr
    w = Pr->Transform * World,
    wvp = w * Camera.VP;
  INT gl_prim_type = Pr->Type == prim_type::LINES ? GL_LINES :
                     Pr->Type == prim_type::TRIMESH ? GL_TRIANGLES :
                     Pr->Type == prim_type::TRISTRIP ? GL_TRIANGLE_STRIP :
                     Pr->Type == prim_type::PATH ? GL_PATCHES :
                     GL_POINTS;

  Apply(Pr->Mtl);
  static shader *shd = shader_manager::Find(Pr->Mtl->ShdName);
  vagl::anim &AnimInstance = vagl::anim::Get();
#if _DEBUG
  static FLT ReloadTime = 30.47;

  ReloadTime += AnimInstance.GlobalDeltaTime;
  if (ReloadTime > 1)
  {
    shd = shader_manager::Find(Pr->Mtl->ShdName);
    ReloadTime = 0;
  }
#endif /* _DEBUG */

  INT prg = shd->Prg;
  INT MatrLoc = shd->MatrWVPLoc;
  if (MatrLoc != -1)
    glUniformMatrix4fv(MatrLoc, 1, FALSE, wvp);


  /* making an array of vertices active */
  glBindVertexArray(Pr->VA);
  if (gl_prim_type == GL_PATCHES)
  {
    glPatchParameteri(GL_PATCH_VERTICES, 16);
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  }
  else if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  else
  {
    /* making an array of indexes active */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    /* drawing */
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    /* disable index array */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  /* disable vertex array */
  glBindVertexArray(0);
} /* End of 'PrimDraw' function */

INT vagl::render::Apply( material *Mtl )
{
  /* Set shader program Id */
  INT prg = shader_manager::Find(Mtl->ShdName)->Prg;

  vagl::anim &AnimInstance = vagl::anim::Get();
#if _DEBUG
  static FLT ReloadTime = 30.47;

  ReloadTime += AnimInstance.GlobalDeltaTime;
  if (ReloadTime > 1)
  {
    MtlUpdate(Mtl);
    ReloadTime = 0;
  }
#endif /* _DEBUG */

  /* Set shading parameters */
  glUseProgram(prg);
  if (Mtl->KaLoc != -1)
    glUniform3fv(Mtl->KaLoc, 1, Mtl->Ka);
  if (Mtl->KdLoc != -1)
    glUniform3fv(Mtl->KdLoc, 1, Mtl->Kd);
  if (Mtl->KsLoc != -1)
    glUniform3fv(Mtl->KsLoc, 1, Mtl->Ks);
  if (Mtl->PhLoc != -1)
    glUniform1f(Mtl->PhLoc, Mtl->Ph);
  if (Mtl->TransLoc != -1)
    glUniform1f(Mtl->TransLoc, Mtl->Trans);
  /* Set textures */

  for (INT i = 0; i < 8; i++)
  {
    if (Mtl->Tex[i] != -1)
    {
      /* Activate sampler */
      glActiveTexture(GL_TEXTURE0 + i);
      /* Bind texture to sampler */
      INT Id = render::texture_manager::Find(Mtl->Tex[i])->TexId;
      glBindTexture(GL_TEXTURE_2D, Id);
    }


    if (Mtl->TexLoc[i] != -1)
      glUniform1i(Mtl->TexLoc[i], Mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'Apply' function */

/* END OF 'render.cpp' FILE */