/*
 * cmdLine.h
 *
 *  Created on: 20.01.2011
 *      Author: mars
 */

#ifndef CMDLINE_H_
#define CMDLINE_H_

enum ECmdId
{
   ciRoot,
   ciUnroot,
   ciUnknown = -1
};

extern ECmdId get_cmd( int anArgc, char* anArgv[] );

#endif /* CMDLINE_H_ */
