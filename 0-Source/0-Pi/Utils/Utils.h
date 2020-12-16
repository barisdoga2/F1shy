#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

long long getCurrentMillis();
void mssleep(long long ms);

#endif