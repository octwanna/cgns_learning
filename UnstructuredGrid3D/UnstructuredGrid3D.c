#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	getUserInput(argc, argv, &data);

	strcpy(data.fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns");
	strcpy(data.baseName, "Simple structured grid");
	strcpy(data.zoneName, "Grid");
	strcpy(data.sectionName, "Hexaedron conectivity");

	/*data.nx = 21;*/
	/*data.ny = 17;*/
	/*data.nz = 9;*/

	data.cellDimension = CELL_DIMENSION;
	data.physicalDimension = PHYSICAL_DIMENSION;

	generateCGNSFile(&data);
	generateBase(&data);
	generateZone(&data);
	generateCoordinates(&data);
	generateElementsConnectivity(&data);

	cg_close(data.file);

	printf("\nSuccessfully wrote grid to file '%s'\n\n", data.fileName);

	exit(EXIT_SUCCESS);
}
