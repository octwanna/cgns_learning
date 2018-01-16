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

	simpleReadFile(&data);
	/* Read the grid */
	err = cg_ngrids(data.file, data.base, data.zone, &numberOfGrids); CHKERRQ(err);
	if(numberOfGrids!=1)
	{
		fprintf(stderr, "Wrong number of grids. It should be 1!\n\n");
		cg_error_exit();
	}
	gridNumber = 1;
	err = cg_grid_read(data.file, data.base, data.zone, gridNumber, gridName); CHKERRQ(err);

	/* Read coordinates */
	err = cg_ncoords(data.file, data.base, data.zone, &numberOfCoordinates); CHKERRQ(err);
	x = (double *) malloc(data.size[0]*sizeof(double));
	y = (double *) malloc(data.size[0]*sizeof(double));
	z = (double *) malloc(data.size[0]*sizeof(double));
	range_min = 1;
	range_max = data.size[0];
	err = cg_coord_read(data.file, data.base, data.zone, "CoordinateX", CGNS_ENUMV(RealDouble), &range_min, &range_max, x); CHKERRQ(err);
	err = cg_coord_read(data.file, data.base, data.zone, "CoordinateY", CGNS_ENUMV(RealDouble), &range_min, &range_max, y); CHKERRQ(err);
	err = cg_coord_read(data.file, data.base, data.zone, "CoordinateZ", CGNS_ENUMV(RealDouble), &range_min, &range_max, z); CHKERRQ(err);

	/* Create pressure field */
	pressure = (double *) malloc(data.size[0]*sizeof(double));
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
