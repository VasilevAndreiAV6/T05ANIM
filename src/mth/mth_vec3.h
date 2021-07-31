/* FILE NAME  : mth_vec3.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 20.07.2021
 * PURPOSE    : Math vector module.
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include <cstdlib>
#include <cmath>

#include "mthdef.h"

/* Space math namespace */
namespace mth
{

  template<class Type>
    Type D2R( Type A )
    {
      return A * PI / 180.0;
    }

  template<class Type>
    Type R2D( Type A )
    {
      return A / PI * 180.0;
    }

  /* 3D vector representation type */
  template<class Type>
    class vec3
    {
    private:
      

    template<class Type1>
      friend class matr;
    public:
      Type X, Y, Z; /* Vector components */

      /* Vector constructor. 
       * ARGUMENTS: None.
       */
      vec3( VOID ) : X(0), Y(0), Z(0)
      {
      } /* End of 'vec3' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector value:
       *       Type A;
       */
      explicit vec3( Type A ): X(A), Y(A), Z(A)
      {
      } /* End of 'vec' function */

      /* Vector constructor.
       * ARGUMENTS:
       *   - vector values:
       *       Type A, B, C;
       */
      vec3( Type A, Type B, Type C) : X(A), Y(B), Z(C)
      {
      } /* End of 'vec3' function */

      /* Vector destructor. 
       * ARGUMENTS: None.
       */
      ~vec3( VOID )
      {
      } /* End of '~vec' function */

      /* Return random vector in range 0 to 1.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result vector.
       */
      static vec3<Type> Rnd0( VOID )
      {
        return vec3<Type>(static_cast<Type>(rand()) / RAND_MAX, static_cast<Type>(rand()) / RAND_MAX, static_cast<Type>(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Return random vector in range -1 to 1. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result vector.
       */
      static vec3<Type> Rnd1( VOID )
      {
        return vec3<Type>(static_cast<Type>(rand()) / RAND_MAX * 2 - 1, static_cast<Type>(rand()) / RAND_MAX * 2 - 1, static_cast<Type>(rand()) / RAND_MAX * 2 - 1);
      } /* End of 'Rnd1' function */

      /* Distance between two points.
       * ARGUMENTS:
       *   - vector:
       *       const vec3 &V;
       * RETURNS:
       *   (Type) result distance.
       */
      Type Distance( const vec3 &V ) const
      {
        return !(*this - V);
      } /* End of 'Distance' function */

      /* Return zero vector. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) zero vector.
       */
      static vec3<Type> Zero( VOID )
      {
        return vec3<Type>(0);
      } /* End of 'Zero' function */

      /* Vector normalizing. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) normalized vector.
       */
      vec3<Type> Normalizing( VOID ) const
      {
        return *this / !(*this);
      } /* End of 'Normalizing' function */

      /* Normalize vector and change value.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) Normalized vector.
       */
      vec3<Type> Normalize( VOID )
      {
        return *this = *this / !(*this);
      } /* End of 'Normalize' function */

      /* Get vector component. 
       * ARGUMENTS:
       *   - index of component:
       *       INT I;
       * RETURNS:
       *   (Type) component of vector.
       */
      Type operator[]( INT I ) const
      {
        switch (I)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          return vec3(0)[0];
        }
      } /* End of 'operator[]' function */

      /* Get vector component reference. 
       * ARGUMENTS:
       *   - index of component:
       *       INT I;
       * RETURNS:
       *  (Type &) component reference.
       */
      Type & operator[]( INT Index )
      {
        switch (Index)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          return vec3(0)[0];
        }
      } /* End of 'operator[]' function */

      /* Get vector length. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) vector length.
       */
      Type Length( VOID ) const
      {
         return sqrt(*this & *this);
      } /* End of 'Length' function */

      /* Return square length.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) vector length in square.
       */
      Type Length2( VOID ) const
      {
         return *this & *this;
      } /* End of 'Length2' function */

      /* Adding vector and change value.
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator+=( const vec3<Type> &V )
      {
        return *this = vec3<Type>(V.X + X, V.Y + Y, V.Z + Z);
      } /* End of 'operator+=' function */

      /* Sub vector and change value. 
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator-=( const vec3<Type> &V )
      {
        return *this = vec3<Type>(V.X - X, V.Y - Y, V.Z - Z);
      } /* End of 'operator-=' function */

      /* Multiply vector with number and change value. 
       * ARGUMENTS:
       *   - number to be multiplyed:
       *       Type N;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator*=( Type N )
      {
        return *this = vec3<Type>(X * N, Y * N, Z * N);
      } /* End of 'operator*=' function */

      /* Divide vector with number and change value.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator/=( Type N )
      {
        return *this = vec3<Type>(X / N, Y / N, Z / N);
      } /* End of 'operator/=' function */

      /* Add two vectors. 
       * ARGUMENTS:
       *   - vector to be added:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator+( const vec3<Type> &V ) const
      {
        return vec3<Type>(V.X + X, V.Y + Y, V.Z + Z);
      } /* End of 'operator+' function */

      /* Sub two vectors.
       * ARGUMENTS:
       *   - vector to be substructed:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator-( const vec3<Type> &V ) const
      {
        return vec3<Type>(X - V.X, Y - V.Y, Z - V.Z);
      } /* End of 'operator-' function */

      /* Multiply vector with number. 
       * ARGUMENTS:
       *   - number to be multiplied:
       *       Type N;
       * RETURNS: None.
       *   (vec3) result vector.
       */
      vec3<Type> operator*( Type N ) const
      {
        return vec3<Type>(X * N, Y * N, Z * N);
      } /* End of 'operator*' function */

      /* Divide vector with number.
       * ARGUMENTS:
       *   - number to be divided:
       *       Type N;
       * RESULT:
       *   (vec3) result vector.
       */
      vec3<Type> operator/( Type N ) const
      {
        return vec3<Type>(X / N, Y / N, Z / N);
      } /* End of 'operator/' function */

      /* Unar minus operation. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) result vector.
       */
      vec3<Type> operator-( VOID ) const
      {
        return vec3<Type>(-X, -Y, -Z);
      } /* End of 'operator~' function */

      /* Vector(cross) multiplying.
       * ARGUMENTS:
       *   - vector to be multiplyied:
       *       const vec3 &V;
       * RETURNS:
       *   (Type) result value.
       */
      vec3<Type> operator%( const vec3<Type> &V )
      {
        return vec3<Type>(Y * V.Z - V.Y * Z,
                    Z * V.X - V.Z * X,
                    X * V.Y - V.X * Y);
      } /* End of 'operator%' function */

      /* Scalar(dot) multiplying. 
       * ARGUMENTS:
       *   - vector to be multyplied:
       *       const vec3 &V;
       * RETURNS: 
       *   (Type) result value.
       */
      Type operator&( const vec3<Type> &V ) const
      {
        return X * V.X + Y * V.Y + Z * V.Z;
      } /* End of 'operator&' function */

      /* Vector length. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) vector length.
       */
      Type operator!( VOID ) const
      {
        return sqrt(*this & *this);
      } /* End of 'operator!' function */

      /* Vector normalizing function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) normalized vector.
       */
      vec3<Type> operator~( VOID ) const
      {
        return *this / !(*this);
      } /* End of 'operator~' function */

      /* Return vector pointer.
       * ARGUMENTS: None.
       * RETURNS:
       *  (Type *) vector pointer.
       */
      operator Type *( VOID )
      {
        return &X;
      } /* End of 'operator' function */

      /* Return vector pointer. 
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type *) vector pointer.
       */
      operator const Type *( VOID ) const
      {
        return &X;
      } /* End of 'operator' function */

      vec3<Type> GetVecInPlaneXZ( VOID ) const
      {
        return vec3<Type>(X, 0, Z);
      }

      BOOL operator<( const vec3<Type> &V )
      {
        if (X < V.X && Y < V.Y && Z < V.Z)
          return TRUE;
        return FALSE;
      }

      BOOL operator>( const vec3<Type> &V ) const
      {
        if (X > V.X && Y > V.Y && Z > V.Z)
          return TRUE;
        return FALSE;
      }

      vec3<Type> operator+( FLT A ) const
      {
        return vec3(X + A, Y + A, Z + A);
      }

      vec3<Type> operator-( FLT A ) const
      {
        return vec3(X - A, Y - A, Z - A);
      }

    }; /* End of 'vec3' class */

} /* end of 'mth' namespace */

#endif /* __mth_vec3_h_ */

/* END OF 'mth_vec3.h' FILE */
