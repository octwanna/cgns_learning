#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

void getUserInput(int argc, char *argv[], cgns_unstructured_file *data)
{
	verifyUserInput(argc);
	getNumberOfVerticesFromUserInput(argc, argv, data);
	getDistancesFromUserInput(argc, argv, data);
}

void verifyUserInput(int argc)
{
	if(argc!=4 && argc!=7)
	{
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "\tprogram NX NY NZ\n");
		fprintf(stdout, "or\n");
		fprintf(stdout, "\tprogram NX NY NZ LX LY LZ\n");
		exit(EXIT_FAILURE);
	}
	return ;
}

void getNumberOfVerticesFromUserInput(int argc, char *argv[], cgns_unstructured_file *data)
{
	data->nx = atoi(argv[1]);
	data->ny = atoi(argv[2]);
	data->nz = atoi(argv[3]);
	return ;
}
void getDistancesFromUserInput(int argc, char *argv[], cgns_unstructured_file *data)
{
	if(argc==4)
	{
		data-lengthX = data->nx;
		data-lengthY = data->ny;
		data-lengthZ = data->nz;
	}
	if(argc==7){
		data-lengthX = atof(argv[4]);
		data-lengthY = atof(argv[5]);
		data-lengthZ = atof(argv[6]);
	}
	data->dx = data->lengthX/(data->nx-1);
	data->dy = data->lengthY/(data->ny-1);
	data->dz = data->lengthZ/(data->nz-1);
	return ;
}

void generateCGNSFile(cgns_unstructured_file *data)
{
	int err;
	err = cg_open(data.fileName, CG_MODE_WRITE, &(data.file)); CHKERRQ(err);
	return ;
}

void generateBase
{
	int err;
	err = cg_base_write(data.file, data.baseName, data.cellDimension, data.physicalDimension, &(data.base)); CHKERRQ(err);
	return ;
}

void generateZone(cgns_unstructured_file *data)
{
	int err;
	cgsize_t size[3];
	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;

	/* Define zone size */
	size[0] = NX*NY*NZ;
	size[1] = (NX-1)*(NY-1)*(NZ-1);
	size[2] = 0;

	/* Open zone */
	err = cg_zone_write(data->file, data->base, data->zoneName, size, Unstructured, &(data->zone)); CHKERRQ(err);

	return;
}

void generateCoordinates(cgns_unstructured_file *data)
{
	int err;
	int i, j, k, entry;
	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;

	double * x;
	double * y;
	double * z;

	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*NZ*sizeof(double**));
	y = (double *) malloc(NX*NY*NZ*sizeof(double**));
	z = (double *) malloc(NX*NY*NZ*sizeof(double**));

	/* Generate coordinates values */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			for(k=0 ; k<NZ ; ++k)
			{
				entry = i + j*NX + k*NX*NY;
				x[entry] = i;
				y[entry] = j;
				z[entry] = k;
			}
		}
	}
	/* Write coordinates */
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorXName, x, &(data->coorX)); CHKERRQ(err);
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorYName, y, &(data->coorY)); CHKERRQ(err);
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorZName, z, &(data->coorZ)); CHKERRQ(err);

	free(x);
	free(y);
	free(z);
}

void generateElementsConnectivity(cgns_unstructured_file *data)
{
	int i, j, k;
	int numberOfElements, elementNumber, firstVerticeIndex;
	cgsize_t *hexaedronConnectivity;
	cgsize_t firstElementNumber = 1;
	cgsize_t lastElementNumber;

	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;


	/* Elements conectivity */
		/* 0 <= elementNumber <= (NX-1)*(NY-1)*(NZ-1) */
	numberOfElements = (NX-1)*(NY-1)*(NZ-1);
	hexaedronConnectivity = (cgsize_t *) malloc(8*numberOfElements*sizeof(cgsize_t));
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			for(k=0 ; k<(NZ-1) ; ++k)
			{
				elementNumber = i + (NX-1)*j + (NX-1)*(NY-1)*k;
				firstVerticeIndex = i + j*NX + k*NX*NY;
				hexaedronConnectivity[8*elementNumber+0] = 1 + firstVerticeIndex;
				hexaedronConnectivity[8*elementNumber+1] = 1 + firstVerticeIndex + 1;
				hexaedronConnectivity[8*elementNumber+2] = 1 + firstVerticeIndex + NX + 1;
				hexaedronConnectivity[8*elementNumber+3] = 1 + firstVerticeIndex + NX;
				hexaedronConnectivity[8*elementNumber+4] = 1 + firstVerticeIndex + NX*NY;
				hexaedronConnectivity[8*elementNumber+5] = 1 + firstVerticeIndex + NX*NY + 1;
				hexaedronConnectivity[8*elementNumber+6] = 1 + firstVerticeIndex + NX*(NY+1) + 1;
				hexaedronConnectivity[8*elementNumber+7] = 1 + firstVerticeIndex + NX*(NY+1);
			}
		}
	}
	lastElementNumber = numberOfElements;
	cg_section_write(data->file, data->base, data->zone, data->sectionName, CGNS_ENUMV(HEXA_8), firstElementNumber, lastElementNumber, 0, hexaedronConnectivity, &(data->section));

	free(hexaedronConnectivity);
}
