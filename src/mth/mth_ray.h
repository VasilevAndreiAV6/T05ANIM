/* FILE NAME  : mth_matr.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math ray module.
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mth_vec3.h"

/* Space math namespace */
namespace mth
{
  template<typename type>
    class ray
    {
    public:
      vec3<type> Org, Dir; /* Ray orogin and direction */

      /* Ray constructor */
      ray( VOID ) : Org(vec3<type>()), Dir(vec3<type>())
      {
      } /* End of 'ray' function */

      /* Ray constructor */
      ray( vec3<type> Org, vec3<type> Dir ) : Org(Org), Dir(Dir)
      {
      } /* End of 'ray' function */

    }; /* End of 'ray' class*/
} /* end of 'mth' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */
