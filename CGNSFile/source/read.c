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
int simpleReadFile(cgns_unstructured_file *data)
{
	/* Open file */	
	readFile(data);
	readBase(data);
	readZone(data);
}

void readFile(cgns_unstructured_file *data)
{
	strcpy(data->fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns");
	strcpy(data->solutionName, "FlowSolution");
	openCGNSFile(data);
	return ;
}

void readBase(cgns_unstructured_file *data)
{
	int err, numberOfBases;
	err = cg_nbases(data->file, &numberOfBases); CHKERRQ(err);
	verify(numberOfBases, 1, "File with number of bases different of 1!");
	data->base = 1;
	err = cg_base_read(data->file, data->base, data->baseName, &(data->cellDimension), &(data->physicalDimension)); CHKERRQ(err);
	verify(data->cellDimension, 3, "File with cell dimension different of 3!\n\n");
	verify(data->physicalDimension, 3, "File with cell dimension different of 3!\n\n");
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
		fprintf(stderr, "Wrong type of mesh!");
		cg_error_exit();
	}
	err = cg_zone_read(data->file, data->base, data->zone, data->zoneName, &(data->size)); CHKERRQ(err);
	verify(data->cellDimension, 3, "File with cell dimension different of 3!\n\n");
	return;
}
