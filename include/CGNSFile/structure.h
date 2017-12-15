#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cgnslib.h>

#define CHKERRQ(err) if((err)) cg_error_exit()

#define PHYSICAL_DIMENSION 3
#define CELL_DIMENSION 3

#define MAX_STRING_LEN 200

typedef struct cgns_unstructured_file
{
	char fileName[MAX_STRING_LEN];
	char baseName[MAX_STRING_LEN];
	char zoneName[MAX_STRING_LEN];
	char sectionName[MAX_STRING_LEN];

	int nx, ny, nz;

	int file, base, zone, section;
	int cellDimension, physicalDimension;

	int coorX, coorY, coorZ;
} cgns_unstructured_file;

#endif
