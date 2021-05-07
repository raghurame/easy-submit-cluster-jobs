#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/sysinfo.h>

char *getParentPath(const char *filePath)
{
	char *parentPath;
	parentPath = (char *) malloc (3000 * sizeof (char));
	int i, slash_index = 0;

	// Check the index of final '/'
	for (i = 0; filePath[i] != '\0'; i++)
	{
		if (filePath[i] == '/')
		{
			slash_index = i;
		}
	}

	// Copying the filePath till slash_index; return parentPath
	for (i = 0; i < slash_index; i++)
	{
		parentPath[i] = filePath[i];
	}
	parentPath[slash_index] = '\0';
	return parentPath;
}

char *getFullPath(const char *cwd, const char *subpath)
{
	char *fullPath;
	fullPath = (char *) malloc (3000 * sizeof(char));
	int fullPath_array = 0, i;

	for (i = 0; cwd[i] != '\0'; i++)
	{
		fullPath[fullPath_array] = cwd[i]; fullPath_array++;
	}
	for (i = 1; subpath[i] != '\0'; i++)
	{
		fullPath[fullPath_array] = subpath[i]; fullPath_array++;
	}
	fullPath[fullPath_array] = '\0';
	return fullPath;
}

int main (int argc, const char *argv[])
{
	if (argc == 2)
	{
		//printf ("arg passed: %s\n", argv[1]);
		char *findText, *fullDirectory, *parentPath;
		findText = (char *) malloc (200 * sizeof (char));

		// Edit the following char variables
		// char *exe_path = "/home/external/iitb/raghuram/lammps-11Aug17/src/lmp_mpi", *console_output = "thermo";
		// char *number_of_nodes = "1", *processors_per_node = "16";
		char *exe_path, *console_output, *machinefile_path, lineString[5000], lineString2[5000], *error_output, *exe_args, *partitionName, *jobName;
		exe_path = (char *) malloc (500 * sizeof (char));
		console_output = (char *) malloc (50 * sizeof (char));
		machinefile_path = (char *) malloc (500 * sizeof (char));
		error_output = (char *) malloc (50 * sizeof (char));
		exe_args = (char *) malloc (50 * sizeof (char));
		partitionName = (char *) malloc (50 * sizeof (char));
		jobName = (char *) malloc (100 * sizeof (char));

		int nNodes, nProcessors_pernode;

		fprintf(stdout, "%s: ", "Enter path of exe... (path should not exceed 500 characters)");
		fgets (lineString, 500, stdin);
		sscanf (lineString, "%s\n", &exe_path);
		fprintf(stdout, "%s\n", &exe_path);
		fprintf(stdout, "\n");
		// strcpy (exe_path, lineString);

		fprintf(stdout, "%s: ", "Enter the name of file to save terminal output (&1)... (file name should not exceed 50 characters)");
		fgets (lineString, 50, stdin);
		sscanf (lineString, "%s\n", &console_output);
		fprintf(stdout, "%s\n", &console_output);
		fprintf(stdout, "\n");
		// strcpy (console_output, lineString);

		fprintf(stdout, "%s: ", "Enter the name of file to save error output (&2)... (file name should not exceed 50 characters)");
		fgets (lineString, 50, stdin);
		sscanf (lineString, "%s\n", &error_output);
		fprintf(stdout, "%s\n", &error_output);
		fprintf(stdout, "\n");
		// strcpy (error_output, lineString);

		fprintf(stdout, "%s: ", "Enter the path for machine file... (path should not exceed 500 characters)");
		fgets (lineString, 50, stdin);
		sscanf (lineString, "%s\n", &machinefile_path);
		fprintf(stdout, "%s\n", &machinefile_path);
		fprintf(stdout, "\n");
		// strcpy (machinefile_path, lineString);

		fprintf(stdout, "%s: ", "Enter the args to be passed to executable... (args should not exceed 50 characters)");
		fgets (lineString, 50, stdin);
		// sscanf (lineString, "%s\n", &exe_args);
		// fprintf(stdout, "%s\n", &exe_args);
		// strcpy (exe_args, lineString);
		fprintf(stdout, "\n");

		fprintf(stdout, "%s: ", "Enter the partition name to submit the job... (partition name should not exceed 50 characters)");
		fgets (lineString, 50, stdin);
		sscanf (lineString, "%s\n", &partitionName);
		fprintf(stdout, "%s\n", &partitionName);
		fprintf(stdout, "\n");
		// strcpy (exe_args, lineString);

		strcpy(findText, "find . -print | grep -i ");
		strcat(findText, argv[1]);
		FILE *pFind;
		pFind = popen (findText, "r");

		fprintf(stdout, "Found %d / %d processors configured on the current node\n", get_nprocs_conf (), get_nprocs ());
		fprintf(stdout, "How many nodes are required to run the job? "); fgets (lineString, 100, stdin); sscanf (lineString, "%d", &nNodes); fprintf(stdout, "\n");
		fprintf(stdout, "How many processors are required to run the job? "); fgets (lineString, 100, stdin); sscanf (lineString, "%d", &nProcessors_pernode); fprintf(stdout, "\n");

		char *current_working_directory;
		current_working_directory = (char *) malloc (2000 * sizeof (char));

		if (getcwd(current_working_directory, sizeof(current_working_directory)) != NULL)
		{
			//printf ("current working directory: %s\n", current_working_directory);
		}

		/*
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		MODEL SHELL SCRIPT: SLURM IN DENDRITE
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		#!/bin/sh

		#SBATCH --job-name=alphaSubstrate_equilibration_equilibration.in
		#SBATCH --mail-type=ALL
		#SBATCH --mail-user=raghuramelancheran@gmail.com
		#SBATCH --ntasks-per-node=1
		#SBATCH --nodes=1
		#SBATCH --partition=ASP

		export PATH=/opt/intel-trial/impi/5.0.1.035/intel64/bin/:/opt/intel-trial/bin/:$PATH
		export LD_LIBRARY_PATH=/opt/intel-trial/composer_xe_2015.0.090/mkl/lib/intel64:$LD_LIBRARY_PATH
		cd /home1/ASP/raghuram/melt/Seagate2TB/alpha_substrate/compressed/T350K/dihedral/50ns;
		MACHINE_FILE=/home1/ASP/raghuram/machinefile
		/opt/slurm/bin/scontrol show hostname $SLURM_JOB_NODELIST &> $MACHINE_FILE
		./localorderparameter_func --maxtf 3000 --input dump_equilibrate600.melt 1>/dev/null 2>orderParameter.err

		*/

		strcpy (jobName, argv[1]);

		while (fgets(lineString, 2000, pFind) != NULL)
		{
			fullDirectory = getFullPath (current_working_directory, lineString);
			//printf ("Full directory: %s\n", fullDirectory);
			parentPath = getParentPath (fullDirectory);
			//printf ("Parent Path: %s\n", parentPath);
			strcat (jobName, parentPath);

			// Creating shell scripts
			char *shellScriptPath;
			shellScriptPath = (char *) malloc (2000 * sizeof (char));
			strcpy(shellScriptPath, parentPath);
			strcat(shellScriptPath, "/submit_jobs.sh");
			//printf ("shell script path: %s\n", shellScriptPath);

			FILE *createShell;
			createShell = fopen (shellScriptPath, "w");
			fprintf(stdout, "#!/bin/sh\n\n#SBATCH --job-name=%s\n#SBATCH --ntasks-per-node=%d\n#SBATCH --nodes=%d\n#SBATCH --partition=%s\n\n", argv[1], nProcessors_pernode, nNodes, &partitionName);

			FILE *submitConfig = fopen ("submit.config", "r");

			while ((fgets (lineString2, 5000, submitConfig) != NULL))
			{
				fprintf(stdout, "%s", lineString2);
			}
			fclose (submitConfig);

			fprintf(stdout, "%s\ncd %s\nMACHINE_FILE=%s\n/opt/slurm/bin/scontrol show hostname $SLURM_JOB_NODELIST &> $MACHINE_FILE\n%s %s 1>%s 2>%s\n", &machinefile_path, &fullDirectory, &exe_path, &exe_args, &console_output, &error_output);
			fclose (createShell);

			// Passing commands to submit the jobs after creating the shell scripts
			// char submit_shell_command[2000];
			// strcpy (submit_shell_command, "qsub ");
			// strcat (submit_shell_command, parentPath);
			// strcat (submit_shell_command, "/submit_jobs.sh");
			sleep (10);
		}

	}
	else if (argc == 1)
	{
	printf ("Help text:\n~~~~~~~~~~\n\nUse submit command to submit all the jobs in the current working directory as well as in all the sub-directories.\n\n");
	}
	else if (argc > 2)
	{
	printf ("Too many arguments passed. Make sure you don't add unnecessary empty spaces. Type the command without args for help text.\n\n");
	}

	return 0;
}
