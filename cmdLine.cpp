/*
 * cmdLine.cpp
 *
 *  Created on: 20.01.2011
 *      Author: mars
 */

#include "cmdLine.h"

#include <string.h>

struct cmd
{
   const char* _cmdName;
   ECmdId _cmdId;
};

static const cmd g_cmd_line[] =
{
      { "root", ciRoot },
      { "unroot", ciUnroot }
};

ECmdId get_cmd( int anArgc, char* anArgv[] )
{
   ECmdId ret( ciUnknown );

   if( anArgc == 2 )
   {
      for( int i( 0 ); i < sizeof( g_cmd_line ) / sizeof( *g_cmd_line ); ++i )
      {
         if( !strcmp( anArgv[ 1 ], g_cmd_line[ i ]._cmdName ) )
         {
            ret = g_cmd_line[ i ]._cmdId;

            break;
         }
      }
   }

   return ret;
}
