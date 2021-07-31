/* FILE NAME  : anim.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Animation class declaration.
 */

#ifndef __anim_h_
#define __anim_h_

#include "../win/win.h"
#include "../utils/stock.h"
#include "input.h"
#include "rnd/render.h"

namespace vagl
{
  class anim;
  class scene;

  /* Unit class */
  class unit
  {
  public:
    virtual ~unit( VOID )
    {
    }

    virtual VOID Response( anim *Ani )
    {
    }

    virtual VOID Render( anim *Ani )
    {
    }
  }; /* End of 'unit' class */

   /* Scene class */
  class scene : public unit
  {
  public:
    stock<unit *> Units;
    /* Constructor */
    scene( VOID ) : Units()
    {
    } /* End of constructor */

    /* Destructor */
    ~scene( VOID ) override
    {
      Units.Walk([]( unit *Uni ){ delete Uni; });
    } /* End of destructor */

    /* Virtual unit render function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Render( anim *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Render(Ani);  });
    } /* End of 'Render' function */

    /* Virtual unit response function.
     * ARGUMENTS:
     *   - pointer to animation context:
     *       animation *Ani;
     * RETURNS: None.
     */
    VOID Response( anim *Ani ) override
    {
      Units.Walk([Ani]( unit *Uni ){  Uni->Response(Ani); });
    } /* End of 'Response' function */

    /* Add unit to scene operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (scene &) Self-reference.
     */
    scene & operator<<( unit *Uni )
    {
      Units.Add(Uni);
      return *this;
    }

    /* Add unit to scene operator.
     * ARGUMENTS:
     *   - name of the unit:
     *       const std::string &Name;
     * RETURNS:
     *   (scene &) Self-reference.
     */
    scene & operator<<( const std::string &Name );
  }; /* End of 'scene' class */


  class anim : public win, public input, public timer, public scene, public render
  {
  private:

    anim( VOID ) : win(), input(win::hWnd, win::MouseWheel), 
                   timer(), render(win::hWnd, win::W, win::H)
    {
    }

    VOID Init( VOID ) override;
    VOID Close( VOID ) override;
    VOID Resize( VOID ) override;
    VOID Activate( VOID ) override;
    VOID Timer( VOID ) override;
    VOID Idle( VOID ) override;
    VOID Erase( HDC hDC ) override;
    VOID Paint( HDC hDC ) override;

    static anim Instance;
  public:
    std::map<std::string, unit * (*)( VOID )> UnitNames;

    template<class UnitType>
    class unit_register
    {
    public:
      static unit * Create( VOID )
      {
        return new UnitType(&Instance);
      }
      unit_register( const std::string &Name )
      {
        Instance.UnitNames[Name] = Create;
      }
    };
    scene Scene; // scene stock

    /* Get animation instance function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim &) animation instance.
     */
    static anim & Get( VOID )
    {
      return Instance;
    } /* End of 'Get' function */

    /* Get animation instance pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim *) animation instance.
     */
    static anim * GetPtr( VOID )
    {
      return &Instance;
    } /* End of 'GetPtr' function */

    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID )
    {
      timer::Response();
      input::Response();

#if _DEBUG
      static FLT ReloadTime = 30.47;
      ReloadTime += timer::GlobalDeltaTime;
      if (ReloadTime > 1)
      {
        shader_manager::Update();
        ReloadTime = 0;
      }
#endif /* _DEBUG */

      render::Start();

      Scene.Response(&Instance);
      Scene.Render(&Instance);

      render::End();
    } /* End of 'Render' function */
  }; /* End of 'anim' class */
} /* end of 'vagl' namespace */

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
