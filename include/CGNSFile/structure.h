#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cgnslib.h>

#define CHKERRQ(err) if((err)) cg_error_exit()

#define MAX_STRING_LEN 900

typedef struct cgns_unstructured_file
{
	char fileName[MAX_STRING_LEN];
	char baseName[MAX_STRING_LEN];
	char zoneName[MAX_STRING_LEN];
	char sectionName[MAX_STRING_LEN];
	char solutionName[MAX_STRING_LEN];

	int nx, ny, nz;
	double lengthX, lengthY, lengthZ;
	double dx, dy, dz;

	int file, base, zone, section, solution;
	int cellDimension, physicalDimension;
	ZoneType_t zonetype;
	cgsize_t size[9];

	int coorX, coorY, coorZ;
} cgns_unstructured_file;

#endif
