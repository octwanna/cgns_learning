#include <stdlib.h>
#include <stdio.h>
#include <cgnslib.h>

#define CHKERRQ(ret) if((ret)) cg_error_exit()

#define PHYSICAL_DIMENSION 2
#define CELL_DIMENSION 2

int main(int argc, char *argv[])
{
	int err;
	int i, j, k, entry;

	int cellDimension = CELL_DIMENSION;
	int physicalDimension = PHYSICAL_DIMENSION;

	int NX, NY;
	double *x=NULL, *y=NULL;

	int file;
	char fileName[] = "/home/guesser/cgns_examples/output/StructuredGrid2D.cgns";

	int base;
	char baseName[] = "Simple structured grid";

	int zone;
	char zoneName[] = "Grid";
	ZoneType_t structured = CGNS_ENUMV(Structured);
	cgsize_t size[3*PHYSICAL_DIMENSION];

	int coorX, coorY;
	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";

	/* get user input */
	if(argc!=3)
	{
		fprintf(stderr, "Usage:\n program NX NY\n\n");
		cg_error_exit();
	}
	NX = atoi(argv[1]);
	NY = atoi(argv[2]);

	/* Open file */
	err = cg_open(fileName, CG_MODE_WRITE, &file); CHKERRQ(err);

	/* Open base */
	err = cg_base_write(file, baseName, cellDimension, physicalDimension, &base); CHKERRQ(err);

	/* Open zone */
	/* Define zone size */
		/* vertices */
		size[0] = NY;
		size[1] = NX;
		/* Cells */
		size[2] = size[0] - 1;
		size[3] = size[1] - 1;
		/* Boundary vertex */
		size[4] = 0;
		size[5] = 0;
	err = cg_zone_write(file, base, zoneName, size, structured, &zone); CHKERRQ(err);

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*sizeof(double));
	y = (double *) malloc(NX*NY*sizeof(double));
	if( x==NULL || y==NULL)
	{
		fprintf(stderr, "Error allocation 'x' and 'y'\n\n");
		cg_error_exit();
	}
	/* Generate coordinates values */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			entry = j + i*NY ;
			x[entry] = i;
			y[entry] = j;
		}
	}
	/* Write coordinates */
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorXName, x, &coorX); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorYName, y, &coorY); CHKERRQ(err);

	/* Close file */
	cg_close(file);

	free(x);
	free(y);
	printf("\nSuccessfully wrote grid to file '%s'\n\n", fileName);
	exit(EXIT_SUCCESS);
}
