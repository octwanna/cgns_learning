#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"
#include "CGNSFile/read.h"

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	int numberOfGrids, gridNumber;
	char gridName[MAX_STRING_LEN];
	int numberOfCoordinates;
	double *x, *y, *z;

	cgsize_t range_min, range_max;

	int entry, count;
	double *pressure;

	int field;

	simpleReadFile(&data, two_dimensional);

	/* Create pressure field */
	pressure = (double *) malloc(data.numberOfVertices*sizeof(double));
	for(count=0 ; count<data.size[0] ; ++count)
	{
		pressure[count] = (double) count;
	}

	/* Write field */
	err = cg_sol_write(data.file, data.base, data.zone, data.solutionName, CGNS_ENUMV(Vertex), &(data.solution)); CHKERRQ(err);
	err = cg_field_write(data.file, data.base, data.zone, data.solution, CGNS_ENUMV(RealDouble), "Pressure", pressure, &field); CHKERRQ(err);

	/* Close file and program */
	cg_close(data.file);
	free(pressure);
	exit(EXIT_SUCCESS);
}
