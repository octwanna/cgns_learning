#include <stdlib.h>
#include <stdio.h>
#include <cgnslib.h>

#define CHKERRQ(ret) if((ret)) cg_error_exit()

#define PHYSICAL_DIMENSION 3
#define CELL_DIMENSION 3

int main(int argc, char *argv[])
{
	int err;
	int i, j, k, entry;

	int cellDimension = CELL_DIMENSION;
	int physicalDimension = PHYSICAL_DIMENSION;

	int NX, NY, NZ;
	double *x, *y, *z;

	int file;
	char fileName[] = "/home/guesser/cgns_examples/output/StructuredGrid3D.cgns";

	int base;
	char baseName[] = "Simple structured grid";

	int zone;
	char zoneName[] = "Grid";
	ZoneType_t structured = CGNS_ENUMV(Structured);
	cgsize_t size[3*PHYSICAL_DIMENSION];

	int coorX, coorY, coorZ;
	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";
	long double c;

	/* get user input */
	if(argc!=4)
	{
		printf("Usage:\n program NX NY NZ\n\n");
		exit(EXIT_FAILURE);
	}
	NX = atoi(argv[1]);
	NY = atoi(argv[2]);
	NZ = atoi(argv[3]);

	/* Open file */
	err = cg_open(fileName, CG_MODE_WRITE, &file); CHKERRQ(err);

	/* Open base */
	err = cg_base_write(file, baseName, cellDimension, physicalDimension, &base); CHKERRQ(err);

	/* Open zone */
	/* Define zone size */
		/* vertices */
		size[0] = NZ;
		size[1] = NY;
		size[2] = NX;
		/* Cells */
		size[3] = size[0] - 1;
		size[4] = size[1] - 1;
		size[5] = size[2] - 1;
		/* Boundary vertex */
		size[6] = 0;
		size[7] = 0;
		size[8] = 0;
	err = cg_zone_write(file, base, zoneName, size, structured, &zone); CHKERRQ(err);

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
				entry = k+ j*NZ + i*NZ*NY ;
				x[entry] = i;
				y[entry] = j;
				z[entry] = k;
			}
		}
	}
	/* Write coordinates */
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorXName, x, &coorX); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorYName, y, &coorY); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorZName, z, &coorZ); CHKERRQ(err);

	/* Close file */
	cg_close(file);

	free(x);
	free(y);
	free(z);
	printf("\nSuccessfully wrote grid to file '%s'\n\n", fileName);
	exit(EXIT_SUCCESS);
}
