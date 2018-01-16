#include <stdlib.h>
#include <stdio.h>
#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

#define PHYSICAL_DIMENSION 2
#define CELL_DIMENSION 2

int main(int argc, char *argv[])
{
	int err;
	char path[900], fileName[900], nx_str[5], ny_str[5];
	char * workspace = getenv("SINMEC_WORKSPACE");
	cgns_unstructured_file data;

	getUserInput2D(argc, argv, &data);

	strcpy(path, workspace);
	strcat(path, "/CGNS_unstructured_2D/");
	strcat(fileName, path);
	strcat(fileName, "unstructured_2D_");
	sprintf(nx_str, "%d", data.nx);
	sprintf(ny_str, "%d", data.ny);
	strcat(fileName, nx_str);
	strcat(fileName, "_");
	strcat(fileName, ny_str);
	strcat(fileName, ".cgns");

	strcpy(data.fileName, fileName);
	strcpy(data.baseName, "Simple structured grid");
	strcpy(data.zoneName, "Grid");
	strcpy(data.sectionName, "quadrangle conectivity");

	data.cellDimension = CELL_DIMENSION;
	data.physicalDimension = PHYSICAL_DIMENSION;

	generateCGNSFile(&data);
	generateBase(&data);
	generateZone2D(&data);
	generateCoordinates2D(&data);
	generateElementsConnectivity2D(&data);

	cg_close(data.file);

	printf("\nSuccessfully wrote grid to file '%s'\n\n", data.fileName);

	exit(EXIT_SUCCESS);
}
