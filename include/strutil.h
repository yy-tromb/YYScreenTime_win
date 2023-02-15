#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool wcsStartsWith(wchar_t *target,size_t targetSize,wchar_t *query,size_t querySize);
bool wcsEndsWith(wchar_t *tatget,size_t targetSize,wchar_t *query,size_t querySize);