#pragma once
/*
decrypte.h
----------------------------------------------------------------------------
Decryption or encryption of txt by key.
Header file for decrypte.c
*/

#include "HardCodedData.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

char* decrypte_line(char* line, long key);
