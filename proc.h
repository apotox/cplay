/*
 * proc.h
 *
 *  Created on: 17.01.2011
 *      Author: mars
 */

#ifndef PROC_H_
#define PROC_H_

#include <unistd.h>

pid_t get_pid( const char* aName );
uid_t get_pid_user( pid_t aPid );

#endif /* PROC_H_ */
