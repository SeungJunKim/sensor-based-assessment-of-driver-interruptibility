// Integrate_parsed_outputs.cpp : Defines the entry point for the console application.
//
/* Mar 20, 2014 created by SeungJun Kim (HCII, CMU)

[parsed]sb14_OBD.csv
[parsed]sb14_MYEI.csv
[parsed]sb14_BH_General.csv
[parsed]sb14_BH_ECG.csv
[parsed]sb14_BH_BB_RR.csv

*/


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
#include <vector>

#define LOOKUP_TABLE_DATA_SIZE 50
#define MAX_N_DRIVER_PI 500  // sb11's total PIs are over 300

#define N_PARSED_FILES 5
#define OBD 0
#define MYEI 1
#define BH_GENERAL 2
#define BH_BB_RR 3
#define BH_ECG 4

#define FNAME_LENGTH 50

#define PRINT_STANDBY 0
#define PRINT_SDATA 1
#define PRINT_NULL_DATA 2
#define PRINT_SKIP_LOOP 3

#define N_SUBJECTS_TO_PROCESS 1

using namespace std; // must exist for file open

/*
#define N_SUBJECTS_TO_PROCESS 13 
int i_subject_id[N_SUBJECTS_TO_PROCESS]={13, 14, 15, 24, 25, 26, 17, 19, 21, 23, 8, 9, 10};*/


//3 //3 //15
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={13, 14, 15, 24, 25, 26, 17, 19, 21, 23, 8, 9, 10};

//int i_subject_id[N_SUBJECTS_TO_PROCESS];//={13, 14, 15, 24};//, 25, 26, 17, 19, 21, 23, 8, 9, 10};

//int i_subject_id[N_SUBJECTS_TO_PROCESS]={13, 14, 15, 24, 25, 26, 17, 19, 21, 23, 8, 9, 10, 5, 11};
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={13, 14, 15}; // WORKED - a note: after the 'post-shaking2' part, the code freezes when transiting to the next subject, but by not priting 'post-shaking' session data, the issue was avoided.
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={24, 25, 26}; // WORKED
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={17, 19, 21}; // Note: 17 was freezed
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={19, 21, 23};	// WORKED
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={8, 9, 10};	// Note: 9 was freezed
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={8, 10, 13};	// Note: 10 was freezed
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={13}; // WORKED

//int i_subject_id[N_SUBJECTS_TO_PROCESS]={9, 10, 17};
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={9};
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={10};
//int i_subject_id[N_SUBJECTS_TO_PROCESS]={17};


//int conv_UTC_to_sec(char UTC_format_to_convert[]);
int conv_UTC_to_sec(string UTC_format_to_convert);

int _tmain(int argc, _TCHAR* argv[])
{
	char sTemp[1024];

	//int i_subject_id[N_SUBJECTS_TO_PROCESS];
	// Note: N_SUBJECTS_TO_PROCESS should be modified as you selected below
	vector<int> i_subject_id;
	//i_subject_id.push_back(13);
	//i_subject_id.push_back(14);
	//i_subject_id.push_back(15);
	//i_subject_id.push_back(24);
	//i_subject_id.push_back(25);
	//i_subject_id.push_back(26);
	i_subject_id.push_back(17);
	//i_subject_id.push_back(19);
	//i_subject_id.push_back(21);
	//i_subject_id.push_back(23);
	//i_subject_id.push_back(8);
	//i_subject_id.push_back(9);
	//i_subject_id.push_back(10);

	//i_subject_id.push_back(11);
	//i_subject_id.push_back(5);

 	ifstream input_session_timetable_file[N_SUBJECTS_TO_PROCESS];						// session_time_table_01.csv
	ifstream input_driver_activity_files[N_SUBJECTS_TO_PROCESS];						// sb01_activities.csv
	ifstream input_parsed_sensor_data_files[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES];		

	//int N_column_to_print[N_PARSED_FILES]={135, 95, 39, 16, 10}; // OBD, MYEI, BH_GENERAL, BH_BB_RR, BH_ECG  - to fill out 'null (?, ?, ..)' data
	int N_column_to_print[N_PARSED_FILES]={129, 88, 32, 9, 3}; // excluded driver_PI_logs parsed from each executable parsing file.

	ofstream final_output_file[N_SUBJECTS_TO_PROCESS];	//ofstream final_output_file;	

	// file names
	char fname_session_timetable[N_SUBJECTS_TO_PROCESS][FNAME_LENGTH]; //char fname_session_timetable[N_SUBJECTS_TO_PROCESS][FNAME_LENGTH]="Null_Timetable";
	char fname_driver_activity[N_SUBJECTS_TO_PROCESS][FNAME_LENGTH];

	char fname_sensor_data_files[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES][FNAME_LENGTH];

	char fname_SPSS_output_files[N_SUBJECTS_TO_PROCESS][FNAME_LENGTH];//char fname_SPSS_output_files[FNAME_LENGTH]="Null_SPSS_output";

	//Initialize
	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{
		strcpy(fname_driver_activity[i], "Null_Driver_Activity_Fname");
		for(int j=0; j<N_PARSED_FILES; j++)
		{
			strcpy(fname_sensor_data_files[i][j],"Null_Sensor_FName");
		}
	}

	// FILE OPEN
	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{
		// Two look-up tables
		sprintf(fname_session_timetable[i],"session_time_table_%02d.csv",i_subject_id[i]);//subject_id_s);
		sprintf(fname_driver_activity[i],"sb%02d_activities.csv",i_subject_id[i]);//subject_id_s);

		input_driver_activity_files[i].open(fname_driver_activity[i]); // Driver Activities (PIs)
		if(!input_driver_activity_files[i]) {cerr<< fname_driver_activity[i] << "<-- Unable to open driver activity annotation file. \n";}
		else{printf("%s is opened\n", fname_driver_activity[i]);}

		input_session_timetable_file[i].open(fname_session_timetable[i]); // Session Time Table
		if(!input_session_timetable_file[i]) {cerr<< fname_session_timetable[i] << "<-- Unable to open experiment session file. \n";}
		else{printf("%s is opened\n", fname_session_timetable[i]);}

		// Five parsed sensor data files
		sprintf(fname_sensor_data_files[i][OBD],"[parsed]sb%02d_OBD.csv",i_subject_id[i]);//subject_id_s);
		sprintf(fname_sensor_data_files[i][MYEI],"[parsed]sb%02d_MYEI.csv",i_subject_id[i]);//subject_id_s);
		sprintf(fname_sensor_data_files[i][BH_GENERAL],"[parsed]sb%02d_BH_General.csv",i_subject_id[i]);//subject_id_s);
		sprintf(fname_sensor_data_files[i][BH_BB_RR],"[parsed]sb%02d_BH_BB_RR.csv",i_subject_id[i]);//subject_id_s);
		sprintf(fname_sensor_data_files[i][BH_ECG],"[parsed]sb%02d_BH_ECG.csv",i_subject_id[i]);//subject_id_s);

		for(int j=0; j<N_PARSED_FILES; j++)
		{
			input_parsed_sensor_data_files[i][j].open(fname_sensor_data_files[i][j]);		// Five Parsed Files (OBD, MYEI, General, BB_RR, ECG)
			if(!input_parsed_sensor_data_files[i][j]) {cerr<< fname_sensor_data_files[i][j] << "<-- Unable to open this 'Parsed' Sensor data file. \n";}
			else{printf("%s is opened\n", fname_sensor_data_files[i][j]);}
		}

		// One output file
		sprintf(fname_SPSS_output_files[i],"SPSS_output_%02d.csv",i_subject_id[i]);//subject_id_s);

		final_output_file[i].open(fname_SPSS_output_files[i]);
		if(!final_output_file[i]) {cerr<< fname_SPSS_output_files[i] << "<-- Unable to create the SPSS output file. \n";}
		else{printf("%s is opened\n", fname_SPSS_output_files[i]);}
	}
	
	// Load SESSION start/end time info 
	vector<string> STT_subject_id;			//0
	vector<string> STT_exp_type;			//1
	vector<string> p1_date;					//2
	vector<string> p1_preshaking_startT;	//3
	vector<string> p1_preshaking_endT;		//4
	vector<string> p1_baseline_startT;		//5
	vector<string> p1_baseline_endT;		//6
	vector<string> p1_driving_startT;		//7
	vector<string> p1_driving_sigstopT;		//8
	vector<string> p1_driving_restartT;		//9
	vector<string> p1_driving_endT;			//10
	vector<string> p1_postshaking_startT;	//11
	vector<string> p1_postshaking_endT;		//12
	vector<string> p2_date;					//13
	vector<string> p2_preshaking_startT;	//14
	vector<string> p2_preshaking_endT;		//15
	vector<string> p2_baseline_startT;		//16
	vector<string> p2_baseline_endT;		//17
	vector<string> p2_driving_startT;		//18
	vector<string> p2_driving_sigstopT;		//19
	vector<string> p2_driving_restartT;		//20
	vector<string> p2_driving_endT;			//21
	vector<string> p2_postshaking_startT;	//22
	vector<string> p2_postshaking_endT;		//23

	for(int j=0; j<N_SUBJECTS_TO_PROCESS; j++)
	{
		string str;

		STT_subject_id.push_back(str);
		STT_exp_type.push_back(str);

		p1_date.push_back(str);
		p1_preshaking_startT.push_back(str);
		p1_preshaking_endT.push_back(str);
		p1_baseline_startT.push_back(str);
		p1_baseline_endT.push_back(str);
		p1_driving_startT.push_back(str);
		p1_driving_sigstopT.push_back(str);
		p1_driving_restartT.push_back(str);
		p1_driving_endT.push_back(str);
		p1_postshaking_startT.push_back(str);
		p1_postshaking_endT.push_back(str);

		p2_date.push_back(str);
		p2_preshaking_startT.push_back(str);
		p2_preshaking_endT.push_back(str);
		p2_baseline_startT.push_back(str);
		p2_baseline_endT.push_back(str);
		p2_driving_startT.push_back(str);
		p2_driving_sigstopT.push_back(str);
		p2_driving_restartT.push_back(str);
		p2_driving_endT.push_back(str);
		p2_postshaking_startT.push_back(str);
		p2_postshaking_endT.push_back(str);
	}

	char scanned_session_string[1024]="Ready_to_SCAN";
	char *pch_session;	

	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{
		input_session_timetable_file[i] >> scanned_session_string;		// Skipping the 'Title' row

		while(!input_session_timetable_file[i].eof())
		{
			input_session_timetable_file[i] >> scanned_session_string;	// read from the 2nd row
		
			pch_session = strtok(scanned_session_string, ",");	
			if(pch_session == NULL) break;									// in order to take care of the last 'NULL' row - if there is no ';' to search, 'break'
			
			STT_subject_id[i] = pch_session;			// proceed until the 1st delimter

			pch_session = strtok(NULL, ",");	STT_exp_type[i] = pch_session;		
		
			// Part 1 driving
			pch_session = strtok(NULL, ",");	p1_date[i] = pch_session;				//strcpy(p1_date[i], pch_session);
			pch_session = strtok(NULL, ",");	p1_preshaking_startT[i] = pch_session;	//strcpy(p1_preshaking_startT[i], pch_session);
			pch_session = strtok(NULL, ",");	p1_preshaking_endT[i] = pch_session;	//strcpy(p1_preshaking_endT[i], pch_session);
			pch_session = strtok(NULL, ",");	p1_baseline_startT[i] = pch_session;	//strcpy(p1_baseline_startT[i], pch_session);		
			pch_session = strtok(NULL, ",");	p1_baseline_endT[i] = pch_session;		//strcpy(p1_baseline_endT[i], pch_session);

			pch_session = strtok(NULL, ",");	p1_driving_startT[i] = pch_session;		//strcpy(p1_driving_startT[i], pch_session);	
			pch_session = strtok(NULL, ",");	p1_driving_sigstopT[i] = pch_session;	//strcpy(p1_driving_sigstopT[i], pch_session);	
			pch_session = strtok(NULL, ",");	p1_driving_restartT[i] = pch_session;	//strcpy(p1_driving_restartT[i], pch_session);	
			pch_session = strtok(NULL, ",");	p1_driving_endT[i] = pch_session;		//strcpy(p1_driving_endT[i], pch_session);

			pch_session = strtok(NULL, ",");	p1_postshaking_startT[i] = pch_session;	//strcpy(p1_postshaking_startT[i], pch_session);
			pch_session = strtok(NULL, ",");	p1_postshaking_endT[i] = pch_session;	//strcpy(p1_postshaking_endT[i], pch_session);

			// Part 2 driving
			pch_session = strtok(NULL, ",");	p2_date[i] = pch_session;				//strcpy(p2_date[i], pch_session);
			pch_session = strtok(NULL, ",");	p2_preshaking_startT[i] = pch_session;	//strcpy(p2_preshaking_startT[i], pch_session);
			pch_session = strtok(NULL, ",");	p2_preshaking_endT[i] = pch_session;	//strcpy(p2_preshaking_endT[i], pch_session);
			pch_session = strtok(NULL, ",");	p2_baseline_startT[i] = pch_session;	//strcpy(p2_baseline_startT[i], pch_session);		
			pch_session = strtok(NULL, ",");	p2_baseline_endT[i] = pch_session;		//strcpy(p2_baseline_endT[i], pch_session);

			pch_session = strtok(NULL, ",");	p2_driving_startT[i] = pch_session;		//strcpy(p2_driving_startT[i], pch_session);

			pch_session = strtok(NULL, ",");	p2_driving_sigstopT[i] = pch_session;	//strcpy(p2_driving_sigstopT[i], pch_session);	

			pch_session = strtok(NULL, ",");	p2_driving_restartT[i] = pch_session;	//strcpy(p2_driving_restartT[i], pch_session);

			pch_session = strtok(NULL, ",");	p2_driving_endT[i] = pch_session;		//strcpy(p2_driving_endT[i], pch_session);


			pch_session = strtok(NULL, ",");	p2_postshaking_startT[i] = pch_session;	//strcpy(p2_postshaking_startT[i], pch_session);
			pch_session = strtok(NULL, "\n");	p2_postshaking_endT[i] = pch_session;	//strcpy(p2_postshaking_endT[i], pch_session);

		}
	}
	
	// Load Annotated Driver PI Information
	vector<vector<string>> PI_subject_id;
	vector<vector<string>> PI_driving_session;
	vector<vector<string>> PI_timestamp;
	vector<vector<string>> PI_year;
	vector<vector<string>> PI_month;
	vector<vector<string>> PI_date;
	vector<vector<string>> PI_hh;
	vector<vector<string>> PI_mm;
	vector<vector<string>> PI_ss;
	vector<vector<string>> PI_Event_detected;
	vector<vector<string>> PI_Specifics;
	vector<vector<string>> PI_Hand_status;
	vector<vector<string>> PI_Opposite_traffic;
	vector<vector<string>> PI_Left_lane;
	vector<vector<string>> PI_My_lane;
	vector<vector<string>> PI_Right_lane;

	vector<vector<string>> driver_PI_UTC_timestamp;
	vector<vector<string>> driver_PI_Annotation_log;

	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{	
		vector<string> vvc;
		for(int j=0; j<MAX_N_DRIVER_PI; j++)
		{
			string str;
			vvc.push_back(str);
		}
		PI_subject_id.push_back(vvc);
		PI_driving_session.push_back(vvc);
		PI_timestamp.push_back(vvc);
		PI_year.push_back(vvc);
		PI_month.push_back(vvc);
		PI_date.push_back(vvc);
		PI_hh.push_back(vvc);
		PI_mm.push_back(vvc);
		PI_ss.push_back(vvc);
		PI_Event_detected.push_back(vvc);
		PI_Specifics.push_back(vvc);
		PI_Hand_status.push_back(vvc);
		PI_Opposite_traffic.push_back(vvc);
		PI_Left_lane.push_back(vvc);
		PI_My_lane.push_back(vvc);
		PI_Right_lane.push_back(vvc);

		driver_PI_UTC_timestamp.push_back(vvc);
		driver_PI_Annotation_log.push_back(vvc);
	}
	
	int i_PI_N_count[N_SUBJECTS_TO_PROCESS];

	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)	// For now, i=0 indicates sb 13
	{
		i_PI_N_count[i]=0;
	}
	
	char scanned_PI_activity_string[1024]="Ready_to_SCAN";
	char * pch_PI;
	
	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)	// For now, i=0 indicates sb 13
	{
		input_driver_activity_files[i] >> scanned_PI_activity_string;	// Skip the 1st 'title' row				
		
		while(!input_driver_activity_files[i].eof())
		{
			// PARSING
			input_driver_activity_files[i] >> scanned_PI_activity_string; // Realized that it reads only - the first part before the 1st sapce --> 13, Part, "1:22:42
				
			pch_PI = strtok(scanned_PI_activity_string, ",");	
			if(pch_PI != NULL) 	// in order to take care of null data in the last row - Note that this checking must come before 'strcpy'
			{
				printf("i=%d, pch_PI = %s\n", i, pch_PI);

				PI_subject_id[i][i_PI_N_count[i]] = pch_PI;										//strcpy(PI_subject_id[i][i_PI_N_count[i]], pch_PI); // proceed until the 1st delimter

				pch_PI = strtok(NULL, ",");	PI_driving_session[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_driving_session[i][i_PI_N_count[i]], pch_PI);			
				pch_PI = strtok(NULL, ",");	PI_timestamp[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_timestamp[i][i_PI_N_count[i]], pch_PI);

				// Note: there are 'spaces' in the timstamp colum, so we need to run more 'reading' for that column
				input_driver_activity_files[i] >> scanned_PI_activity_string; //Read until the 2nd space in the timestamp column
				input_driver_activity_files[i] >> scanned_PI_activity_string; //Read after the 3rd space in the timestamp column

				strtok(scanned_PI_activity_string, ",");

				pch_PI = strtok(NULL, ",");	PI_year[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_year[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_month[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_month[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_date[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_date[i][i_PI_N_count[i]], pch_PI);		
				pch_PI = strtok(NULL, ",");	PI_hh[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_hh[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_mm[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_mm[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_ss[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_ss[i][i_PI_N_count[i]], pch_PI);
		
				pch_PI = strtok(NULL, ",");	PI_Event_detected[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_Event_detected[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_Specifics[i][i_PI_N_count[i]] = pch_PI;			//strcpy(PI_Specifics[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_Hand_status[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_Hand_status[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_Opposite_traffic[i][i_PI_N_count[i]] = pch_PI;	//strcpy(PI_Opposite_traffic[i][i_PI_N_count[i]], pch_PI);		
				pch_PI = strtok(NULL, ",");	PI_Left_lane[i][i_PI_N_count[i]] = pch_PI;			//strcpy(PI_Left_lane[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, ",");	PI_My_lane[i][i_PI_N_count[i]] = pch_PI;			//strcpy(PI_My_lane[i][i_PI_N_count[i]], pch_PI);
				pch_PI = strtok(NULL, "\n");	PI_Right_lane[i][i_PI_N_count[i]] = pch_PI;		//strcpy(PI_Right_lane[i][i_PI_N_count[i]], pch_PI);

				sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", 
					atoi(PI_year[i][i_PI_N_count[i]].c_str()), 
					atoi(PI_month[i][i_PI_N_count[i]].c_str()), 
					atoi(PI_date[i][i_PI_N_count[i]].c_str()), 
					atoi(PI_hh[i][i_PI_N_count[i]].c_str()), 
					atoi(PI_mm[i][i_PI_N_count[i]].c_str()), 
					atoi(PI_ss[i][i_PI_N_count[i]].c_str())
				);
				driver_PI_UTC_timestamp[i][i_PI_N_count[i]] = sTemp;

				sprintf(sTemp,"%s,%s,%s,%s,%s,%s,%s",PI_Event_detected[i][i_PI_N_count[i]].c_str(), PI_Specifics[i][i_PI_N_count[i]].c_str(), PI_Hand_status[i][i_PI_N_count[i]].c_str(), PI_Opposite_traffic[i][i_PI_N_count[i]].c_str(), PI_Left_lane[i][i_PI_N_count[i]].c_str(), PI_My_lane[i][i_PI_N_count[i]].c_str(), PI_Right_lane[i][i_PI_N_count[i]].c_str());
				driver_PI_Annotation_log[i][i_PI_N_count[i]] = sTemp;

				//Check parsing status
				printf("%s (sb%s) - i_PI_N_count[i]=%d \n", driver_PI_UTC_timestamp[i][i_PI_N_count[i]].c_str(), PI_subject_id[i][i_PI_N_count[i]].c_str(), i_PI_N_count[i]);

				i_PI_N_count[i]++;
			}
		}		

		printf("The PIs of subject %02d are completely scanned (total: %d)\n\n", i_subject_id[i], i_PI_N_count[i]);
		cin.get();
	}
		
	//MAIN - SPSS TABULATING
	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{
		final_output_file[i] << "Timestamp,Subject_ID,Session_ID,";		
		final_output_file[i] << "PI_status,Event_detected,Specifics,Hand_status,Opposite_traffic,Left_lane,My_lane,Right_lane,";
	}

	char scanned_sensor_string[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES][1500];
	char temp_scanned_sensor_string[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES][1500];

	char *pch_sensor[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES];
	bool skip_title[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES]; 
	int printing_status[N_SUBJECTS_TO_PROCESS][N_PARSED_FILES];

	for(int i=0; i<N_PARSED_FILES; i++)
	{
		//Initialize
		for(int j=0; j<N_SUBJECTS_TO_PROCESS; j++)
		{
			skip_title[j][i]=false;
			printing_status[j][i]=PRINT_STANDBY;
		}
	}

	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)	// For now, i=0 indicates sb 13
	{
		//char UTC_timestamp[50]="Null_Timestamp"; // To finally print out
		string UTC_timestamp;
		int pi_count=0;

		printf("Subject %02d started!!\n\n",i_subject_id[i]);
		//cin.get();
				
// Part 1 driving //
		int temp_year=0, temp_month=0, temp_date=0;
		int temp_hour[2]={0,0}, temp_minute[2]={0,0}, temp_second[2]={0,0};
		
		sscanf(p1_date[i].c_str(),"%d/%d/%d", &temp_month, &temp_date, &temp_year); // from session file
		
// # 1/6 SESSION 'PRESHAKING1'
		/*
		sscanf(p1_preshaking_startT[i],"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);		// Session Start Time
		sscanf(p1_preshaking_endT[i],"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);			// Session End Time
		
		//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		sprintf(UTC_timestamp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) 
			//until it reaches to the end-time of the SESSION 'PRESHAKING1'; temp_hour[0], temp_minute[0], and temp_second[0] will be increasing by 1-second at every iteration.
		{
			// 'Title' row of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)		// From OBD, MYEI, BH_GENERAL, BH_BB_RR, and to BH_ECG
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i_subject_count==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// First print out - 'PRESHAKING1'
			final_output_file[i] << UTC_timestamp << "," << STT_subject_id[i] <<",PRESHAKING1,";
			final_output_file[i] << "Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,";

			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp
				}
			
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp)) // Not yet reached.
				{
					//PRINT_SKIP_LOOP --- NO Printing
					//printf("STATUS 1 (sensor timestamp < output_UTC): PRINT_SKIP_LOOP. - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i_subject_count+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// Scan and skip until the same timestamp
					{
						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp

						//printf("SKIP-PRESHAKING1 SESSION - %s, %s\n", pch_sensor[i_subject_count][sensor_id], UTC_timestamp);
						//cin.get();
					}

					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp)) // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Columns 4~ - Main Sensor data streams
						final_output_file[i] << pch_sensor[i][sensor_id];
						printf("[PRINTED- PRESHAKING1 [sb%02d][%d]]: %s\n\n",i_subject_id[i], sensor_id,pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					//printf("STATUS 2 (sensor timestamp == output_UTC): PRINT_SDATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i_subject_count+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Columns 4~ - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];
					printf("[PRINTED]: %s\n\n",pch_sensor[i][sensor_id]);

					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

				}
				
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_NULL_DATA
					//printf("STATUS 3 (sensor timestamp > output_UTC): PRINT_NULL_DATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i_subject_count+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_NULL_DATA;
					
					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}					
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

					printf("[PRINTED]: Null data streams, , , , ,\n\n");
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			sprintf(UTC_timestamp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		}
		//UP TO HERE*/


// # 2/6 SESSION 'BASELINE1'
		sscanf(p1_baseline_startT[i].c_str(),"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);	// Start
		sscanf(p1_baseline_endT[i].c_str(),"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);	// End

		sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		UTC_timestamp = sTemp;

		/// FROM HERE 
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) //until it's not the end of the SESSION 'BASELINE1
		{
			// The following loop mainly concerns 'Title' data in the beginning of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// First print out - 'BASELINE1'
			final_output_file[i] << UTC_timestamp.c_str() << "," << STT_subject_id[i].c_str() <<",BASELINE1,";
			final_output_file[i] << "Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,";

			/*// Only for sb05 which doesn't have Part 1 OBD data					
			for(int missing=0; missing<N_column_to_print[OBD]; missing++)
			{
				final_output_file[i] << "?,";
			}	

			for(int sensor_id=MYEI; sensor_id<BH_ECG+1; sensor_id++)  // up to here (only for sb05)*/
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++) // Original
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
				}
			
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// If the scanned timestamp does not yet reached to the current UTC timestamp to print out.
				{
					//PRINT_SKIP_LOOP 
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))			// Repeatedly scan and skip, until it reaches to the current UTC timestamp to print out.
					{
						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];

						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
					}

					// Print only when the newly-scanned timestamp reached to the current UTC timestamp to print out.
					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))			 // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
						final_output_file[i] << pch_sensor[i][sensor_id];	printf("[Printed after skipping loop -[%d][%d]]: %s\n\n",i, sensor_id,pch_sensor[i][sensor_id]);
						
						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];	printf("[PRINTED]: %s\n\n",pch_sensor[i][sensor_id]);

					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
				}
				
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_NULL_DATA
					printing_status[i][sensor_id]=PRINT_NULL_DATA;
					
					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}	
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

					printf("[Printed the ?(Weka null) data streams] \n\n");
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			UTC_timestamp = sTemp;

		}
		//UP TO HERE
		
// # 3/6 SESSION 'DRIVING1'
		sscanf(p1_driving_startT[i].c_str(),"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);	// Start
		sscanf(p1_driving_endT[i].c_str(),"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);		// End
		
		sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		UTC_timestamp = sTemp;

		char temp_session_info[30]={"DRIVING1"};

		/// FROM HERE 
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) //until it's not the end of the SESSION 'DRIVING1
		{
			// The following loop mainly concerns 'Title' data in the beginning of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// First print out - 'DRIVING1'
			final_output_file[i] << UTC_timestamp.c_str() << "," << STT_subject_id[i].c_str() <<",DRIVING1,";
		
			if(pi_count<i_PI_N_count[i]-1)
			{
				//if(pi_count+1<219) // for the SEPARATE group sb11
				//{
					while((conv_UTC_to_sec(UTC_timestamp)-conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))>1)
					{
							// In case that driving started later than the first Human-annotated PI, skip	
							pi_count++;
					}
				//}
			}

			if(
				(pi_count<i_PI_N_count[i])&&
				(abs(conv_UTC_to_sec(UTC_timestamp)-conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))<=1)  // log for three seconds
				)
			{
				final_output_file[i] << pi_count+1 << "of" << i_PI_N_count[i] << "," << driver_PI_Annotation_log[i][pi_count].c_str() << ",";

				//if(pi_count+1<219) // for the SEPARATE group sb11
				//{
					while(conv_UTC_to_sec(UTC_timestamp)>conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))
					{			
						pi_count++;
					}
				//}
			}else
			{
				final_output_file[i] << "Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,";
			}
			
			
			/*// Only for sb05 which doesn't have Part 1 OBD data		
			for(int missing=0; missing<N_column_to_print[OBD]; missing++)
			{
				final_output_file[i] << "?,";
			}	

			for(int sensor_id=MYEI; sensor_id<BH_ECG+1; sensor_id++)  // up to here (only for sb05)*/
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
					
					//strcpy(temp_session_info,pch_sensor[i][sensor_id]);									// Mainly for the SEPARATE group (sb11 and sb05)

					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
				}
			
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp)) // If the scanned timestamp does not yet reached to the current UTC timestamp to print out.
				{
					//PRINT_SKIP_LOOP --- NO Printing
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// Scan and skip until the same timestamp
					{
						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];

						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
					}

					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp)) // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
						final_output_file[i] << pch_sensor[i][sensor_id];
						printf("[PRINTED-[%d][%d]]: %s\n\n",i, sensor_id,pch_sensor[i][sensor_id]);
						
						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp

					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];		printf("[PRINTED]: %s\n\n",pch_sensor[i][sensor_id]);

					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
				}
				
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_NULL_DATA
					printing_status[i][sensor_id]=PRINT_NULL_DATA;

					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}	
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

					printf("[PRINTED]: Null data streams, , , , ,\n\n");
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			UTC_timestamp = sTemp;
		}
		//UP TO HERE


// Part 2 driving
		sscanf(p2_date[i].c_str(),"%d/%d/%d", &temp_month, &temp_date, &temp_year);
		
		// Only for sb11  ---- From here <----- Remove when processing the other subjects!!!
		//pi_count=219;	// sb11 
		//pi_count=42;	// sb05
		for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
		{
			while(1)	// Scan until the Part2
			{
				input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
				strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]);	// For preventing conflicts with 'strtok' function calls

				pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");		// Column 1 - Subject id 
				pch_sensor[i][sensor_id] = strtok(NULL,","); 											// Column 2 - Session id
				pch_sensor[i][sensor_id] = strtok(NULL,","); 											// Column 3 - Timestamp

				int i_temp_hms[3]; char c_temp_hms[30];
				sscanf(pch_sensor[i][sensor_id],"%d-%d-%dT%s",&i_temp_hms[0],&i_temp_hms[1],&i_temp_hms[2],c_temp_hms);
				if((i_temp_hms[0]==temp_year)&&(i_temp_hms[1]==temp_month)&&(i_temp_hms[2]==temp_date)) break;
			} 
		}// Up to here

// # 4/6 SESSION 'BASELINE2'
		sscanf(p2_baseline_startT[i].c_str(),"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);	// Start
		sscanf(p2_baseline_endT[i].c_str(),"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);	// End
		
		sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		UTC_timestamp = sTemp;
		
		/// FROM HERE 
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) //until it's not the end of the SESSION 'BASELINE2'
		{
			// The following loop mainly concerns 'Title' data in the beginning of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// First print out - 'BASELINE2'
			final_output_file[i] << UTC_timestamp.c_str() << "," << STT_subject_id[i].c_str() <<",BASELINE2,";
			final_output_file[i] << "Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,";

			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
				}

				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp)) // If the scanned timestamp does not yet reached to the current UTC timestamp to print out.
				{
					//PRINT_SKIP_LOOP --- NO Printing
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// Scan and skip until the same timestamp
					{
						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
						printf("[Skipping - Baseline2[sb%02d][SENSOR_%d]]: \npch_sensor[i][sensor_id]=%s, UTC_Timstamp=%s\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					}

					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp)) // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Columns 4~ - Main Sensor data streams
						final_output_file[i] << pch_sensor[i][sensor_id];
						printf("[PRINTED (= after skipping)- Baseline2[sb%02d][SENSOR_%d]]: \n%s\n\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id]);
						//cin.get();
						
						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					//printf("STATUS 2 (sensor timestamp == output_UTC): PRINT_SDATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
					printf("[PRINTED (= right away) - Baseline2[sb%02d][SENSOR_%d]]: \npch_sensor[i][sensor_id]=%s, UTC_Timstamp=%s\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];
					printf("%s\n\n",pch_sensor[i][sensor_id]);
					//cin.get();

					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
				}
				
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_NULL_DATA
					//printf("STATUS 3 (sensor timestamp > output_UTC): PRINT_NULL_DATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_NULL_DATA;
					
					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}	
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					
					printf("[Priting - Baseline2]: Null data streams \n");
					printf("pch_sensor[sb%02d][SENSOR_%d]=%s, UTC_timestamp=%s \n\n", i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					//cin.get();
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			UTC_timestamp = sTemp;
		}
		//UP TO HERE

// # 5/6 SESSION 'DRIVING2'
		sscanf(p2_driving_startT[i].c_str(),"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);	// Start
		sscanf(p2_driving_endT[i].c_str(),"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);		// End
		
		//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		UTC_timestamp = sTemp;

		/// FROM HERE 
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) //until it's not the end of the SESSION 'DRIVING2'
		{
			// The following loop mainly concerns 'Title' data in the beginning of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// Start to print out - 'DRIVING2'
			final_output_file[i] << UTC_timestamp.c_str() << "," << STT_subject_id[i].c_str() <<",DRIVING2,";
							
			if(pi_count<i_PI_N_count[i]-1)
			{
				while((conv_UTC_to_sec(UTC_timestamp)-conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))>1)
				{
						// In case that driving started later than the first Human-annotated PI, skip
						pi_count++;
				}
			}

			if(
				(pi_count<i_PI_N_count[i])&&
				(abs(conv_UTC_to_sec(UTC_timestamp)-conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))<=1)
				)
			{
				final_output_file[i] << pi_count+1 << "of" << i_PI_N_count[i] << "," << driver_PI_Annotation_log[i][pi_count].c_str() << ",";
				printf("Printed PI[pi_count:%d/%d]: %s\n\n", pi_count,i_PI_N_count[i],driver_PI_Annotation_log[i][pi_count]);

				while(conv_UTC_to_sec(UTC_timestamp)>conv_UTC_to_sec(driver_PI_UTC_timestamp[i][pi_count]))
				{   
					// increase after post 1-sec, or skip in case that multiple PIs are annotated with the same timestamps
					if(pi_count==i_PI_N_count[i]) break;
					pi_count++; 
				}
			}else
			{
				final_output_file[i] << "Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,Non_PI,";
			}


			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
				}
			
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp)) // If the scanned timestamp does not yet reached to the current UTC timestamp to print out.
				{
					//PRINT_SKIP_LOOP
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// Scan and skip until the same timestamp
					{
						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
						printf("[Skipping - Driving2[sb%02d][SENSOR_%d]]: \npch_sensor[i][sensor_id]=%s, UTC_Timstamp=%s\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					}

					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp)) // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main sensor data streams

						final_output_file[i] << pch_sensor[i][sensor_id];	
						printf("[PRINTED (= after skipping)- Driving2[sb%02d][SENSOR_%d]]: \n%s\n\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id]);
						//cin.get();
						
						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");										// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 										// Column 3 - Timstamp
					printf("[PRINTED (= right away) - Driving2[sb%02d][SENSOR_%d]]: \npch_sensor[i][sensor_id]=%s, UTC_Timstamp=%s\n",i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 										// Other columns - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];	
					printf("%s\n\n", pch_sensor[i][sensor_id]);
					//cin.get();
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

				}
				
				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_NULL_DATA
					printing_status[i][sensor_id]=PRINT_NULL_DATA;
					
					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}	
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					
					printf("[Priting - Driving2]: Null data streams \n");
					printf("pch_sensor[sb%02d][SENSOR_%d]=%s, UTC_timestamp=%s \n\n", i_subject_id[i], sensor_id, pch_sensor[i][sensor_id], UTC_timestamp.c_str());
					//cin.get();
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			sprintf(sTemp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
			UTC_timestamp = sTemp;
		}
		//UP TO HERE

// SESSION 'POSTSHAKING2'
		/*
		sscanf(p2_postshaking_startT[i],"%d:%d:%d", &temp_hour[0], &temp_minute[0], &temp_second[0]);		// Start
		sscanf(p2_postshaking_endT[i],"%d:%d:%d", &temp_hour[1], &temp_minute[1], &temp_second[1]);		// End
		
		//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		sprintf(UTC_timestamp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		
		/// FROM HERE 
		while(!((temp_hour[0]==temp_hour[1])&&(temp_minute[0]==temp_minute[1])&&(temp_second[0]==temp_second[1]))) //until it's not the end of the SESSION 'POSTSHAKING2'
		{
			// The following loop mainly concerns 'Title' data in the beginning of each sensor file
			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(skip_title[i][sensor_id]==false)
				{
					//Title row
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
										
					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Skip the title 'subject id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Skip the title 'session id'
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Skip the title 'timestamp'
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Reach to the data titles
					// Note: the above four about pch_sensor should be called - not within 'if(i==0)', so as that these four can perform a role of pointer initialzation when moving to the next subjects
					
					//if(i==0) // print only one-time at the first subject
					//{
						final_output_file[i] << pch_sensor[i][sensor_id]; //Print out the 'Title' row of the sensor data streams
						printf("title (sensor_id %d) - %s\n", sensor_id, pch_sensor[i][sensor_id]);

						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}	// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}	// the last sensor file
					//}

					skip_title[i][sensor_id]=true;
				}
			}
			
			// First print out - 'POSTSHAKING2'
			final_output_file[i] << UTC_timestamp << "," << STT_subject_id[i] <<",POSTSHAKING2,";
			final_output_file[i] << "Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,Null_PI,";

			for(int sensor_id=OBD; sensor_id<BH_ECG+1; sensor_id++)
			{
				if(printing_status[i][sensor_id]!=PRINT_NULL_DATA)
				{
					input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];
					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp
				}

				if(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp)) // Not yet reached.
				{
					//PRINT_SKIP_LOOP --- NO Printing
					//printf("STATUS 1 (sensor timestamp < output_UTC): PRINT_SKIP_LOOP. - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_SKIP_LOOP;

					while(conv_UTC_to_sec(pch_sensor[i][sensor_id])<conv_UTC_to_sec(UTC_timestamp))	// Scan and skip until the same timestamp
					{
						//printf("SKIP(begin)-POSTSHAKING2 SESSION - %s, %s\n", pch_sensor[i][sensor_id], UTC_timestamp);

						input_parsed_sensor_data_files[i][sensor_id] >> scanned_sensor_string[i][sensor_id];

						string mystring = scanned_sensor_string[i][sensor_id];
						if(mystring.empty()) break;												// Added 'break' to handle OBD streams ended earlier than post-shaking
						
						strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls
						
						pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],",");	// Column 1 - Subject id 
						pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 2 - Session id
						pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp

						//printf("SKIP(end)-POSTSHAKING2 SESSION - %s, %s\n", pch_sensor[i][sensor_id], UTC_timestamp);
						//cin.get();
					}

					if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp)) // This 'if' condition is necessary since it can exceed (i.e., >)
					{
						pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Columns 4~ - Main Sensor data streams
						final_output_file[i] << pch_sensor[i][sensor_id];
						printf("[PRINTED-[%d][%d]]: %s\n\n",i, sensor_id,pch_sensor[i][sensor_id]);
						
						if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
						if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
					}
				}else if(conv_UTC_to_sec(pch_sensor[i][sensor_id])==conv_UTC_to_sec(UTC_timestamp))
				{
					//PRINT_SDATA
					//printf("STATUS 2 (sensor timestamp == output_UTC): PRINT_SDATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_SDATA;

					strcpy(temp_scanned_sensor_string[i][sensor_id], scanned_sensor_string[i][sensor_id]); // For preventing conflicts with 'strtok' function calls

					pch_sensor[i][sensor_id] = strtok(temp_scanned_sensor_string[i][sensor_id],","); 	// Column 1 - Subject id (Important Note: str in strtok should not be 'NULL' since sensor scanning is changed.
					pch_sensor[i][sensor_id] = strtok(NULL,",");														// Column 2 - Session id
					pch_sensor[i][sensor_id] = strtok(NULL,","); 														// Column 3 - Timstamp
					pch_sensor[i][sensor_id] = strtok(NULL,"\n"); 													// Other columns - Main Sensor data streams
					
					final_output_file[i] << pch_sensor[i][sensor_id];
					printf("[PRINTED]: %s\n\n",pch_sensor[i][sensor_id]);

					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file
				}				
				
				string mystring2 = scanned_sensor_string[i][sensor_id];
				if((mystring2.empty()==true)|(conv_UTC_to_sec(pch_sensor[i][sensor_id])>conv_UTC_to_sec(UTC_timestamp)))  // Added 'empty()' to handle OBD streams ended earlier than post-shaking
				{
					//PRINT_NULL_DATA
					//printf("STATUS 3 (sensor timestamp > output_UTC): PRINT_NULL_DATA - sb%d, sensor %d (0-OBD, 1-MYEI, 2-General, 3-BB_RR, 4-ECG) \n", i+13, sensor_id);
					printing_status[i][sensor_id]=PRINT_NULL_DATA;
					
					for(int missing=0; missing<N_column_to_print[sensor_id]; missing++)
					{
						switch(sensor_id)
						{
						case OBD: final_output_file[i] << "?"; break;
						case MYEI: final_output_file[i] << "?"; break;
						case BH_GENERAL: final_output_file[i] << "?"; break;
						case BH_BB_RR: final_output_file[i] << "?"; break;
						case BH_ECG: final_output_file[i] << "?"; break;
						}

						if(missing!=N_column_to_print[sensor_id]-1)
						{
							final_output_file[i] << ",";
						}
					}	
					
					if(sensor_id!=BH_ECG){final_output_file[i]<<",";}					// Not the last sensor file
					if(sensor_id==BH_ECG){final_output_file[i]<<"\n";}					// the last sensor file

					//check
					//printf("STATUS 2: pch_sensor[%d][MYEI==%d]: %s\n", i, MYEI, pch_sensor[i][MYEI]);
					///printf("STATUS 2: pch_sensor[%d][BH_GENERAL==%d]: %s\n", i, BH_GENERAL, pch_sensor[i][BH_GENERAL]);

					//printf("[PRINTED]: Null data streams, , , , ,\n\n");
				}
			}
			
			// Add 1 second - preparation for the next loop
			temp_second[0]++;			

			if(temp_second[0]==60)
			{
				temp_minute[0]++; temp_second[0]=0;
				if(temp_minute[0]==60)
				{
					temp_hour[0]++; temp_minute[0]=0;
				}
			}

			// Stand by as holding the next UTC to print out
			//sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);			
			sprintf(UTC_timestamp,"%d-%02d-%02dT%02d:%02d:%02d.000-05:00", temp_year, temp_month, temp_date, temp_hour[0], temp_minute[0], temp_second[0]);
		}
		//UP TO HERE*/
		
		printf("Subject %02d ended!!\n......... Sleep for 2 seconds\n\n",i_subject_id[i]);
		Sleep(2000);
		//cin.get();
	}
	
	printf("Enter any key! - all processed, only 'close' and 'clear' left");
	cin.get();

	// Close & Clear files	

	for(int i=0; i<N_SUBJECTS_TO_PROCESS; i++)
	{		
		//sprintf(subject_id_s,"%02d",i+atoi(first_subject_id));

		input_session_timetable_file[i].close(); 
		input_session_timetable_file[i].clear(); printf("\n\n%s is closed \n", fname_session_timetable[i]);			

		input_driver_activity_files[i].close();
		input_driver_activity_files[i].clear(); printf("%s is closed \n", fname_driver_activity[i]);	
		
		for(int j=0; j<N_PARSED_FILES; j++)
		{
			//3 - raw sensor data
			input_parsed_sensor_data_files[i][j].close(); 
			input_parsed_sensor_data_files[i][j].clear(); printf("%s is closed \n", fname_sensor_data_files[i][j]);	
		}

		final_output_file[i].close(); 
		final_output_file[i].clear(); printf("%s is closed \n", fname_SPSS_output_files[i]);
	}

	return 0;
}

int conv_UTC_to_sec(string UTC_format_to_convert)
{
	//2013-12-19T13:16:00.000-05:00
	int utc_hh=0, utc_mm=0, utc_ss=0;
	
	int temp_int=0;
	char temp_string[20]="null";

	sscanf(UTC_format_to_convert.c_str(),"%d-%d-%dT%d:%d:%d%s", &temp_int, &temp_int, &temp_int, &utc_hh, &utc_mm, &utc_ss, temp_string);

	//cin.get();
	//printf("In 'conv_UTC_to_sec' function: %s -> utc_hh (%d), utc_mm(%d), utc_ss(%d)", UTC_format_to_convert, utc_hh, utc_mm, utc_ss);
	//cin.get();

	return utc_ss+utc_mm*60+utc_hh*60*60;
}

