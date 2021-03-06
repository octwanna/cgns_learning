#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"
#include "CGNSFile/read.h"

void verify(int toCheck, int correct, char *errorMessage)
{
	if(toCheck != correct)
	{
		fprintf(stderr, errorMessage);
		cg_error_exit();
	}
	return;
}

void simpleReadFile(cgns_unstructured_file *data, Dimension dimension)
{
	/* Open file */	
	readFile(data, dimension);
	readBase(data, dimension);
	readZone(data);
	readGrid(data);
	readGridCoordinates(data, dimension);
}

void readFile(cgns_unstructured_file *data, Dimension dimension)
{
	if(dimension==two_dimensional) strcpy(data->fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid2D.cgns");
	if(dimension==three_dimensional) strcpy(data->fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns");
	strcpy(data->solutionName, "FlowSolution");
	openCGNSFile(data);
	return ;
}

void readBase(cgns_unstructured_file *data, Dimension dimension)
{
	int err, numberOfBases;
	err = cg_nbases(data->file, &numberOfBases); CHKERRQ(err);
	verify(numberOfBases, 1, "File with number of bases different of 1!");
	data->base = 1;
	err = cg_base_read(data->file, data->base, data->baseName, &(data->cellDimension), &(data->physicalDimension)); CHKERRQ(err);
	if(dimension==two_dimensional)
	{
		verify(data->cellDimension, 2, "File with cell dimension different of 2!\n\n");
		verify(data->physicalDimension, 2, "File with cell dimension different of 2!\n\n");
	}
	if(dimension==three_dimensional)
	{
		verify(data->cellDimension, 3, "File with cell dimension different of 3!\n\n");
		verify(data->physicalDimension, 3, "File with cell dimension different of 3!\n\n");
	}
	return ;
}

void readZone(cgns_unstructured_file *data)
{
	int err, numberOfZones;
	err = cg_nzones(data->file, data->base, &numberOfZones); CHKERRQ(err);
	verify(numberOfZones, 1, "File with number of zones different of 1!\n\n");
	data->zone = 1;
	err = cg_zone_type(data->file, data->base, data->zone, &(data->zonetype));
	if (data->zonetype != Unstructured)
	{
		fprintf(stderr, "Wrong type of grid! It must be 'Unstructured'!");
		cg_error_exit();
	}
	err = cg_zone_read(data->file, data->base, data->zone, data->zoneName, &(data->size)); CHKERRQ(err);
	data->numberOfVertices = data->size[0];
	data->numberOfElements = data->size[1];
	return;
}

void readGrid(cgns_unstructured_file *data)
{
	int err, numberOfGrids;
	err = cg_ngrids(data->file, data->base, data->zone, &numberOfGrids); CHKERRQ(err);
	if(numberOfGrids!=1)
	{
		fprintf(stderr, "Wrong number of grids. It should be 1!\n\n");
		cg_error_exit();
	}
	data->grid = 1;
	err = cg_grid_read(data->file, data->base, data->zone, data->grid, data->gridName); CHKERRQ(err);
}

void readGridCoordinates(cgns_unstructured_file *data, Dimension dimension)
{
	int err, numberOfCoordinates;
	cgsize_t range_min, range_max;
	err = cg_ncoords(data->file, data->base, data->zone, &numberOfCoordinates); CHKERRQ(err);
	if(dimension==two_dimensional) verify(numberOfCoordinates, 2, "Wrong number of coordinates. It should be 2!\n\n");
	if(dimension==three_dimensional) verify(numberOfCoordinates, 3, "Wrong number of coordinates. It should be 3!\n\n");
	data->x = (double *) malloc(data->size[0]*sizeof(double));
	data->y = (double *) malloc(data->size[0]*sizeof(double));
	if(dimension==three_dimensional)
		data->z = (double *) malloc(data->size[0]*sizeof(double));
	range_min = 1;
	range_max = data->size[0];
	err = cg_coord_read(data->file, data->base, data->zone, "CoordinateX", CGNS_ENUMV(RealDouble), &range_min, &range_max, data->x); CHKERRQ(err);
	err = cg_coord_read(data->file, data->base, data->zone, "CoordinateY", CGNS_ENUMV(RealDouble), &range_min, &range_max, data->y); CHKERRQ(err);
	if(dimension==three_dimensional)
		err = cg_coord_read(data->file, data->base, data->zone, "CoordinateZ", CGNS_ENUMV(RealDouble), &range_min, &range_max, data->z); CHKERRQ(err);
}
