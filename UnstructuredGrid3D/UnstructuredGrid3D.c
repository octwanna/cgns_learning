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

void generate_zone(cgns_unstructured_file *data);
void generateCoordinates(cgns_unstructured_file *data);
void generateElementsConnectivity(cgns_unstructured_file *data);

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

void generate_zone(cgns_unstructured_file *data)
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
