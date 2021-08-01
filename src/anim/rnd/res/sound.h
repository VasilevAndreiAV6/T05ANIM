/* FILE NAME  : sound.h
 * PROGRAMMER : AV6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Sounds handle module.
 */

#ifndef __sound_h_
#define __sound_h_

#include "../openAL/al.h"
#include "../openAL/alc.h"
#include "../openAL/alut.h"

#pragma comment(lib, "src/anim/rnd/openAL/openal32")
#pragma comment(lib, "src/anim/rnd/openAL/alut")

/* Project namespace */
namespace vagl
{
  class sound
  {
  private:
    UINT 
      Buf,
      src,
      size,
      freq;
    INT
      format;
    CHAR loop;
    VOID *mem;
  public:
    BOOL IsPlaying;

    /* Sound class constructor */
    sound( VOID )
    {
    } /* End of 'sound' function*/

    /* Sound class constructor */
    sound( const std::string FileName )
    {
      alGenBuffers(1, (&Buf));
      alutLoadWAVFile(const_cast<CHAR *>(FileName.c_str()), &format, &mem, &size, &freq, &loop);
      alBufferData(Buf, format, mem, size, freq);
      alutUnloadWAV(format, mem, size, freq);

      alGenSources(1, &src);

      alSourcei(src, AL_BUFFER, Buf);
      alSourcei(src, AL_PITCH, 1.0);
      alSourcei(src, AL_GAIN, 1.0);
      alSourcei(src, AL_LOOPING, 0);
    } /* End of 'sound' function */

    /* Start playing function.
     * ARGUEMNTS: None.
     * RETURNS: None.
     */
    VOID StartPlaying( VOID )
    {
      alSourcePlay(src);
    }

  }; /* End of 'sound' class */
} /* end of 'vagl' namespace */

#endif /* __sound_h_ */

/* END OF 'sound.h' FILE */
