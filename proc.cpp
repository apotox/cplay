/*
 * proc.cpp
 *
 *  Created on: 17.01.2011
 *      Author: mars
 */

#include "proc.h"

#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/stat.h>

///////////////////////////////////////////////////////////////////////////////////
static const char* g_proc_name      ( "/proc/" );
static const char* g_cmdline_name   ( "/cmdline" );

///////////////////////////////////////////////////////////////////////////////////
pid_t get_pid( const char* aName )
{
   pid_t ret( -1 );

   DIR* dp( opendir( g_proc_name ) );

   if( dp )
   {
      dirent* ep( 0 );

      while( ( ep = readdir( dp ) ) && ret == -1 )
      {
         char path[ PATH_MAX ] = { 0 };

         strcat( path, g_proc_name );
         strcat( path, ep->d_name );
         strcat( path, g_cmdline_name );

         int file( open( path, O_RDONLY ) );

         if( file )
         {
            char procName[ PATH_MAX ] = { 0 };

            if( read( file, procName, sizeof( procName ) ) > 0 )
            {
               if( strstr( procName, aName ) )
               {
                  sscanf( ep->d_name, "%d", &ret );
               }
            }

            close( file );
         }
      }

      closedir( dp );
   }

   return ret;
}

///////////////////////////////////////////////////////////////////////////////////
uid_t get_pid_user( pid_t aPid )
{
   uid_t ret( -1 );

   char buf[ PATH_MAX ] = { 0 };
   sprintf( buf, "/proc/%d", aPid );

   struct stat theStat;
   if( !stat( buf, &theStat ) )
   {
      ret = theStat.st_uid;
   }

   return ret;
}
