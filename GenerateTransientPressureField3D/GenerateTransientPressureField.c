#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"
#include "CGNSFile/read.h"

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	int entry, count;
	double *pressure;

	int field;

	int numberOfTimeSteps, timeStep;
	double timeInstant, deltaT;
	char **pressureSolutionName;
	int solutionIndex;
	char iterativeBaseName[MAX_STRING_LEN], iterativeZoneName[MAX_STRING_LEN];
	double *timeArray;
	cgsize_t timeArrayDimension[1];
	cgsize_t solutionNameDimension[2];
	char * solutionNames;

	simpleReadFile(&data, three_dimensional);

	/* Transient solution */
	numberOfTimeSteps = 20;
	deltaT = 1;
	pressure = (double *) malloc(data.numberOfVertices*sizeof(double));
	pressureSolutionName = (char **) malloc(numberOfTimeSteps*sizeof(char*));
	for(timeStep=0 ; timeStep<numberOfTimeSteps ; ++timeStep)
	{
		timeInstant = timeStep * deltaT;
		pressureSolutionName[timeStep] = (char *) malloc(12 * sizeof(char));/* 12 because 'pressure_01\0' is a 12 character string */
		sprintf(pressureSolutionName[timeStep], "pressure_%02d", timeStep+1); /* Paraview start at one */
		/*printf("\npressure solution name, time step %d: %s\n\n", timeStep, pressureSolutionName[timeStep]);*/
		for(count=0 ; count<data.numberOfVertices ; ++count)
			pressure[count] = count * timeInstant;

		/* Write field */
		err = cg_sol_write(data.file, data.base, data.zone, pressureSolutionName[timeStep], CGNS_ENUMV(Vertex), &solutionIndex); CHKERRQ(err);
		err = cg_field_write(data.file, data.base, data.zone, solutionIndex, CGNS_ENUMV(RealDouble), "Pressure", pressure, &field); CHKERRQ(err);
	}

	/* Create a iterative base */
	strcpy(iterativeBaseName, "TimeIterativeValues");
	err = cg_biter_write(data.file, data.base, iterativeBaseName, numberOfTimeSteps); CHKERRQ(err);

	/* Create array of time steps */
	timeArray = (double*) malloc(numberOfTimeSteps * sizeof(double));
	timeArrayDimension[0] = numberOfTimeSteps;
	for(timeStep=0 ; timeStep<numberOfTimeSteps ; ++timeStep)
		timeArray[timeStep] = deltaT * timeStep;
	err = cg_goto(data.file, data.base, iterativeBaseName, 0, "end"); CHKERRQ(err);
	err = cg_array_write("TimeValues", CGNS_ENUMV(RealDouble), 1, timeArrayDimension, timeArray); CHKERRQ(err);

	/* Create iterative zone */
	strcpy(iterativeZoneName, "ZoneIterativeData");
    cg_ziter_write(data.file, data.base, data.zone, iterativeZoneName);
	err = cg_goto(data.file, data.base, "Zone_t", data.zone, iterativeZoneName, 0, "end"); CHKERRQ(err);

	solutionNameDimension[0] = 11; /* 11 because 'pressure_01' is a 11 character string */
	solutionNameDimension[1] = numberOfTimeSteps;
	solutionNames = (char*) malloc(12*numberOfTimeSteps*sizeof(char));
	strcpy(solutionNames, "\0");
	for(timeStep=0 ; timeStep<numberOfTimeSteps ; ++timeStep)
		strcat(solutionNames, pressureSolutionName[timeStep]);
	err = cg_array_write("PressureSolutionPointers", CGNS_ENUMV(Character), 2, solutionNameDimension, solutionNames); CHKERRQ(err);

    err = cg_simulation_type_write(data.file, data.base, CGNS_ENUMV(TimeAccurate)); CHKERRQ(err);

	/* Close file and program */
	cg_close(data.file);
	free(pressure);
	exit(EXIT_SUCCESS);
}
