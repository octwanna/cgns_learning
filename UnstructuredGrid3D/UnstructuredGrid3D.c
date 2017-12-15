#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	strcpy(data.fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns");
	strcpy(data.baseName, "Simple structured grid");
	strcpy(data.zoneName, "Grid");
	strcpy(data.sectionName, "Hexaedron conectivity");

	data.nx = 21;
	data.ny = 17;
	data.nz = 9;

	data.cellDimension = CELL_DIMENSION;
	data.physicalDimension = PHYSICAL_DIMENSION;

	err = cg_open(data.fileName, CG_MODE_WRITE, &(data.file)); CHKERRQ(err);
	err = cg_base_write(data.file, data.baseName, data.cellDimension, data.physicalDimension, &(data.base)); CHKERRQ(err);
	generate_zone(&data);
	generateCoordinates(&data);
	generateElementsConnectivity(&data);

	cg_close(data.file);

	printf("\nSuccessfully wrote grid to file '%s'\n\n", data.fileName);

	exit(EXIT_SUCCESS);
}
