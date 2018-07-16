// BioHarness_SychTimestamp.cpp : Defines the entry point for the console application.
//

// SJ Note: The project has kept generating errors related to functions and variables. So, In the property - C++/Preprocess, I added _CRT_SECURE_NO_WARNINGS as guided in http://www.youtube.com/watch?v=qWxGZLjwKL0
// So, now the errors disappeared.


// CHECK! - if 'session_num' is not bounded as comments, the code is for sb11 and sb 05 in the SEPARATE group. 
// In this case, you will need to deactivate handling for sb11 and sb05, and activate original parts (if necessary)
// Even you are working with sb11 and sb05, you have to switch the valued in 'separate_driving_date' appropriately.


#include "stdafx.h"

#include "windows.h"
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <string.h>   
#include <stdlib.h>
#include "math.h"

#define FNAME_LENGTH 50
#define DATA_SIZE 50

#define N_SENSOR_FILES 3

#define BB_RR 0
#define ECG 1
#define GENERAL 2

//#define N_SUBJECTS 6	// SHIFT GROUP
#define N_SUBJECTS 2	// SEPARATE GROUP

using namespace std; // must exist for file open

ifstream input_files[N_SENSOR_FILES][N_SUBJECTS];		
ofstream output_files[N_SENSOR_FILES][N_SUBJECTS];	

/* Sync group

1) "EXACT" Group (no need to sync) - sb13, sb14, sb15, sb19, sb21, sb23

2) "SHFIT' Group - sb24 (-2.5s), sb25 (-4.0s), sb26 (-6.5s)

3) "SEPARATE" Group (need to sync part 1 and part 2 separately) - sb5, sb11

*/

// Note: EXACT GROUP (7 people - no need to process) = 9, 13, 14, 15, 19, 21, 23
// Note: SHIFT GROIP (6 people - need to process both part 1 and 2 together) = 8, 10, 17, 24, 25, 26
// Note: SEPARATE GROUP (2 people - need to process separately) = 5 (+6000ms & 0ms), 11 (+19000ms, -7000ms)

/* For SHIFT GROUP
int i_subject_id[N_SUBJECTS]={8, 10, 17, 24, 25, 26};	// SHIFT GROUP
int i_lagged_ms[N_SUBJECTS]={-3500, -1000, 21500, -2500, -4000, -6500}; // mili_seconds (SHIFT GROUP)*/

// FOR SEPARATE GROUP
int i_subject_id[N_SUBJECTS]={5, 11};	
int i_lagged_ms_part1[N_SUBJECTS]={6000, 19000};
int i_lagged_ms_part2[N_SUBJECTS]={0, -7000};
char part1_date[N_SUBJECTS][50]={"01/11/2013","18/11/2013"};
char part2_date[N_SUBJECTS][50]={"23/12/2013","08/01/2014"}; ///// SEPARATE GROUP - up to here
int session_num[N_SUBJECTS]={0,0};

int _tmain(int argc, _TCHAR* argv[])
{	
	char fname_input_files[N_SENSOR_FILES][N_SUBJECTS][FNAME_LENGTH];
	char fname_output_files[N_SENSOR_FILES][N_SUBJECTS][FNAME_LENGTH];

	// File Open
	for(int i=0; i<N_SUBJECTS; i++)
	{
		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			if(j==BB_RR)
			{				
				sprintf(fname_input_files[j][i],"sb%02d_BH_BB_RR.csv",i_subject_id[i]);
				sprintf(fname_output_files[j][i],"sb%02d_BH_BB_RR_synced.csv",i_subject_id[i]);
			}else if(j==ECG)
			{
				sprintf(fname_input_files[j][i],"sb%02d_BH_ECG.csv",i_subject_id[i]);
				sprintf(fname_output_files[j][i],"sb%02d_BH_ECG_synced.csv",i_subject_id[i]);
			}else if(j==GENERAL)
			{
				sprintf(fname_input_files[j][i],"sb%02d_BH_General.csv",i_subject_id[i]);
				sprintf(fname_output_files[j][i],"sb%02d_BH_General_synced.csv",i_subject_id[i]);
			}

			input_files[j][i].open(fname_input_files[j][i]);
			if(!input_files[j][i]) {cerr<< fname_input_files[j][i] << "<-- Unable to open the input file. \n";}
			else{printf("%s is opened\n", fname_input_files[j][i]);}

			output_files[j][i].open(fname_output_files[j][i]);
			if(!output_files[j][i]) {cerr<< fname_output_files[j][i] << "<-- Unable to create the output file. \n";}
			else{printf("%s is opened\n", fname_output_files[j][i]);}
		}
	}

	char data_BB_RR[3][DATA_SIZE];			//datestamp timestamp, and then other data
	char data_ECG[3][DATA_SIZE];			//datestamp timestamp, and then other data
	char data_General[3][DATA_SIZE];		//datestamp timestamp, and then other data

	char scanned_string[500];
	char * pch;	
	int hh, mm, ss, ms;

	for(int i=0; i<N_SUBJECTS; i++)
	{	
		//printf("New subject\n\n");
		//cin.get();
		//printf("Thanks!\n");

		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			hh=0; mm=0; ss=0; ms=0;

			input_files[j][i] >> scanned_string;		// Skip the 1st 'title' row

			output_files[j][i] << scanned_string << endl;

			//printf("scanned_string: %s\n\n", scanned_string);
			
			while(!input_files[j][i].eof()) 
			{	
				input_files[j][i] >> scanned_string;	// front datestamp - "01/11/2013" 11:25:48.444

				char datestamp[50]="Null";
				strcpy(datestamp,scanned_string);		

				if(strcmp(datestamp,part1_date[i])==0)
				{
					session_num[i]=1;		// Part 1 driving of subject i
				}else if(strcmp(datestamp,part2_date[i])==0)
				{
					session_num[i]=2;		// Part 2 driving of subject i
				}else
				{
					session_num[i]=0;
				}
				
				input_files[j][i] >> scanned_string;

				pch = strtok (scanned_string, ",");		// proceed until the delimter ','
				if(pch == NULL) break;					// in order to take care of the last 'NULL' row - if there is no ',' to search, 'break' - Note that this checking must come before 'strcpy'

				if(j==BB_RR)
				{
					strcpy(data_BB_RR[0], pch);									// back timstamp - 01/11/2013 "11:25:48.444"
					pch = strtok (NULL, "\n"); strcpy(data_BB_RR[1], pch);		// the rest BB_RR data

					//printf("BB_RR: %s, %s, and then data_BB_RR[1] \n", datestamp, data_BB_RR[0]);
					sscanf(data_BB_RR[0],"%d:%d:%d.%d",&hh,&mm,&ss,&ms);
					
				}else if(j==ECG)
				{
					strcpy(data_ECG[0], pch);									// back timstamp - 01/11/2013 "11:25:48.444"
					pch = strtok (NULL, "\n"); strcpy(data_ECG[1], pch);		// the rest ECG	data	

					//printf("ECG: %s, %s, and then data_ECG[1] \n", datestamp, data_ECG[0]);
					sscanf(data_ECG[0],"%d:%d:%d.%d",&hh,&mm,&ss,&ms);
				}else if(j==GENERAL)
				{
					strcpy(data_General[0], pch);								// back timstamp - 01/11/2013 "11:25:48.444"
					pch = strtok (NULL, "\n"); strcpy(data_General[1], pch);	//the rest General	data

					printf("General: %s, %s, and then data_General[1] \n", datestamp, data_General[0]);
					sscanf(data_General[0],"%d:%d:%d.%d",&hh,&mm,&ss,&ms);
				}

				
				int synced_time_in_ms=0;

				/* FOR SHIFT GROUP
				synced_time_in_ms=((hh*60+mm)*60+ss)*1000+ms+i_lagged_ms[i];

				ms=div(synced_time_in_ms,1000).rem;
				ss=div(div(synced_time_in_ms,1000).quot,60).rem;
				mm=div(div(div(synced_time_in_ms,1000).quot,60).quot,60).rem;
				hh=div(div(div(synced_time_in_ms,1000).quot,60).quot,60).quot;*/

				if(session_num[i]==1)
				{
					synced_time_in_ms=((hh*60+mm)*60+ss)*1000+ms+i_lagged_ms_part1[i]; // part 1 driving of subject i
				}else if(session_num[i]==2)
				{
					synced_time_in_ms=((hh*60+mm)*60+ss)*1000+ms+i_lagged_ms_part2[i]; // part 2 driving of subject i
				}

				ms=div(synced_time_in_ms,1000).rem;
				ss=div(div(synced_time_in_ms,1000).quot,60).rem;
				mm=div(div(div(synced_time_in_ms,1000).quot,60).quot,60).rem;
				hh=div(div(div(synced_time_in_ms,1000).quot,60).quot,60).quot;
			
				char new_timestamp[50];
				sprintf(new_timestamp,"%02d:%02d:%02d.%03d",hh,mm,ss,ms);

				if(j==BB_RR)
				{
					output_files[j][i] << datestamp << " " << new_timestamp << "," << data_BB_RR[1] << endl;
				}else if(j==ECG)
				{
					output_files[j][i] << datestamp << " " << new_timestamp << "," << data_ECG[1] << endl;
				}else if(j==GENERAL)
				{
					output_files[j][i] << datestamp << " " << new_timestamp << "," << data_General[1] << endl;
				}
			}
		}
	}

	printf("All done. Enter any key");
	cin.get();

	//File Close
	for(int i=0; i<N_SUBJECTS; i++)
	{			
		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			input_files[j][i].close(); input_files[j][i].clear(); printf("%s is closed \n", fname_input_files[j][i]);	
			output_files[j][i].close(); output_files[j][i].clear(); printf("%s is closed \n", fname_output_files[j][i]); 
		}
	}

	return 0;
}

