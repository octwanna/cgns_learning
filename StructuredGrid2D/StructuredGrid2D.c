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
	double LX, LY;
	double dx, dy;
	double xPosition, yPosition;
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
	/* get user input */
	if(argc!=3 && argc!=5)
	{
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "\tprogram NX NY\n");
		fprintf(stdout, "or\n");
		fprintf(stdout, "\tprogram NX NY LX LY\n");
		exit(EXIT_FAILURE);
	}
	NX = atoi(argv[1]);
	NY = atoi(argv[2]);
	if(argc==3)
	{
		LX = NX;
		LY = NY;
	}
	if(argc==5){
		LX = atof(argv[3]);
		LY = atof(argv[4]);
	}
	dx = LX/(NX-1);
	dy = LY/(NY-1);

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
	for(i=0, xPosition=0 ; i<NX ; ++i, xPosition+=dx)
	{
		for(j=0, yPosition=0 ; j<NY ; ++j, yPosition+=dy)
		{
			entry = j + i*NY ;
			x[entry] = xPosition;
			y[entry] = yPosition;
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
