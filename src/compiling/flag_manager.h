#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct FlagInfo
{
	const char* input_file  = nullptr;
	const char* output_file = nullptr;

	bool parser_dump_required    = false;
	bool dot_dump_required       = false;
	bool nametable_dump_required = false;
};

void GetFlagInfo(const int argc, const char** argv, FlagInfo* info);
