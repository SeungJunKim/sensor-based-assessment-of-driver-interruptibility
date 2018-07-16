// BioHarness_parser.cpp : Defines the entry point for the console application.
//
/* Feb 2, 2014 created by SeungJun Kim (HCII, CMU)

(1) Look-up table 1 - one session timetable (one file across all subjects)

	File - session_time_table_new.csv (24 columns, delimiter - ',')

	Subject_ID,Sessions,P1_date,P1_preshaking_start,P1_preshaking_end,P1_baseline_start,P1_baseline_end,P1_drivnig_start,P1_destination_SigStop,P1_destination_restart,P1_driving_end,P2_postshaking_start,P2_postshaking_end,P2_date,P2_preshaking_start,P2_preshaking_end,P2_baseline_start,P2_baseline_end,P2_drivnig_start,P2_destination_SigStop,P2_destination_restart,P2_driving_end,P2_postshaking_start,P2_postshaking_end
	13,Both,12/19/2013,13:16:00,13:16:20,13:20:25,13:21:25,13:22:41,None,None,14:15:49,None,None,12/19/2013,None,None,14:52:18,14:53:18,14:54:53,None,None,15:32:11,15:36:30,15:36:50
	14,Both,12/17/2013,10:41:00,10:41:20,10:47:15,10:48:15,10:50:04,None,None,11:34:44,None,None,12/17/2013,None,None,11:54:20,11:55:20,11:56:16,12:13:03,12:13:56,12:27:26,12:40:31,12:40:51
	15,Both,12/12/2013,11:03:30,11:03:50,11:07:50,11:08:50,11:09:37,None,None,11:56:25,None,None,12/12/2013,None,None,12:26:30,12:27:30,12:28:10,12:43:52,12:44:26,12:59:06,13:04:00,13:04:20
		:

	[0]		Subject_ID							5				9
	[1]		Exp_type							Separate		Both

	[2]		Part1_date							11/1/2013		12/13/2013
	[3]		P1_Preshaking_start					11:27:30		10:08:45
	[4]		P1_Preshaking_end					11:27:50		10:09:05
	[5]		P1_Baseline_start					11:32:40		10:12:40
	[6]		P1_Baseline_end						11:33:40		10:13:40
	[7]		P1_Driving_start					hh:mm:ss		hh:mm:ss
	[8]		P1_Destination_sigstop								hh:mm:ss or None
	[9]		P1_Destination_restart								hh:mm:ss or None
	[10]	P1_Driving_end						hh:mm:ss		hh:mm:ss
	[11]	P1_Postshaking_start				12:28:10		None
	[12]	P1_Postshaking_end					12:28:30		None

	[13]	Part2_date							12/23/2013		12/13/2013
	[14]	P2_Preshaking_start					11:12:30		None
	[15]	P2_Preshaking_end					11:12:50		None
	[16]	P2_Baseline_start					11:19:15		11:18:05
	[17]	P2_Baseline_end						11:20:15		11:19:05
	[18]	P2_Driving_start					hh:mm:ss		hh:mm:ss
	[19]	P2_Destination_sigstop								hh:mm:ss or None
	[20]	P2_Destination_restart								hh:mm:ss or None
	[21]	P2_Driving_end						hh:mm:ss		hh:mm:ss
	[22]	P2_Postshaking_start				11:54:45		12:01:20
	[23]	P2_Postshaking_end					11:55:05		12:01:40
	

(2) Look-up table 2 - multiple acvitity records (one file per each subject)

	File - sb01_activities.csv (16 columns, delimiter - ',')

	Subject_ID,Driving_session,Timestamps,year,month,date,hh,mm,ss,Event_detected,Specifics,Hand_status,Opposite_traffic,Left_lane,My_lane,Right_lane
	13,Part1,"1:22:42 (6:51, 0:37)",2013,12,19,13,22,42,R_HAND_OFF,OTHERS_SELF,HAND_OFF_WHEEL,NONE,NONE,NONE,NONE
	13,Part1,"1:22:57 (7:06, 0:52)",2013,12,19,13,22,57,R_HAND_OFF,OTHERS_SELF,HAND_OFF_WHEEL,NONE,NONE,NONE,NONE
	13,Part1,"1:23:16 (7:25, 1:11)",2013,12,19,13,23,16,R_HAND_OFF,OTHERS_SELF,HAND_OFF_WHEEL,NONE,NONE,NONE,NONE
	13,Part1,"1:23:17 (7:26, 1:12)",2013,12,19,13,23,17,R_HAND_OFF,GEAR_SHIFT,HAND_OFF_WHEEL,NONE,NONE,NONE,NONE

(3) BioHarness Data Streams (three files per each subject)

	File 1 - sb13_BH_BB_RR.csv (3 columns, delimiter - ',')

	Timestamp,BR,RtoR
	19/12/2013 13:11:16.444,65488,-0.176
	19/12/2013 13:11:16.500,65487,-0.176
	19/12/2013 13:11:16.556,65488,-0.176
		:

	File 2 - sb13_BH_ECG.csv (2 columns, delimiter - ',')

	Timestamp,ECG
	19/12/2013 13:11:16.444,2875
	19/12/2013 13:11:16.448,2859
	19/12/2013 13:11:16.452,2859
		:

	File 3 - sb13_BH_General.csv (17 columns, delimiter - ',')

	Timestamp,HR,BR,Temp,Posture,Activity,Acceleration,Battery,BRAmplitude,ECGAmplitude,ECGNoise,XMin,XPeak,YMin,YPeak,ZMin,ZPeak
	19/12/2013 13:11:16.444,73,10.0,19.5,-56,0.73,0.97,4.188,15.000,0.000577,0.000259,-0.75,-0.24,-0.03,0.52,0.52,1.19
	19/12/2013 13:11:17.444,77,10.0,19.5,-62,0.65,1.94,4.176,15.000,0.002368,0.000245,-1.47,0.57,-0.70,1.80,-0.08,1.80
	19/12/2013 13:11:18.444,87,10.0,19.5,-82,0.53,1.78,4.166,13.000,0.002368,0.000231,-0.70,1.29,-0.39,0.32,0.06,3.08
		:

(4) Labeled Output Files (e.g., [labeled]sb02_MYEI.txt, [labeled]sb02_MYEI.txt, [labeled]sb03_MYEI.txt,,,)

	[labeled] sb13_BH_BB_RR.csv
		[0]		Subject_ID
		[1]		Session_ID
		[2]		sensor data 1 - Timestamp
		[3]		sensor data 2 - BR
		[4]		sensor data 3 - RtoR
		[5]		Hand_move_detected
		[6]		Specific_interaction
		[7]		Hand_OnOff_Wheel
		[8]		Traffic_Opposite
		[9]		Traffic_Left_lane
		[10]	Traffic_my_lane
		[11]	Traffic_right_lane

	[labeled] sb13_BH_ECG.csv
	
		[0]		Subject_ID
		[1]		Session_ID
		[2]		sensor data 1 - Timestamp
		[3]		sensor data 2 - ECG
		[4]		Hand_move_detected
		[5]		Specific_interaction
		[6]		Hand_OnOff_Wheel
		[7]		Traffic_Opposite
		[8]		Traffic_Left_lane
		[9]		Traffic_my_lane
		[10]	Traffic_right_lane

	[labeled] sb13_BH_General.csv
	
		[0]		Subject_ID
		[1]		Session_ID
		[2]		sensor data 1 - Timestamp
		[3]		sensor data 2 - HR
		[4]		sensor data 3 - BR
		[5]		sensor data 4 - Temp
		[6]		sensor data 5 - Posture
		[7]		sensor data 6 - Activity
		[8]		sensor data 7 - Acceleration
		[9]		sensor data 8 - Battery
		[10]	sensor data 9 - BRAmplitude
		[11]	sensor data 10 - ECGAmplitude
		[12]	sensor data 11 - ECGNoise
		[13]	sensor data 12 - XMin
		[14]	sensor data 13 - XPeak
		[15]	sensor data 14 - YMin
		[16]	sensor data 15 - YPeak
		[17]	sensor data 16 - ZMin
		[18]	sensor data 17 - ZPeak
		[19]	Hand_move_detected
		[20]	Specific_interaction
		[21]	Hand_OnOff_Wheel
		[22]	Traffic_Opposite
		[23]	Traffic_Left_lane
		[24]	Traffic_my_lane
		[25]	Traffic_right_lane
*/

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

#define N_SUBJECTS 1
#define N_TIMETABLE_SUBJECTS 1

#define FNAME_LENGTH 50
#define DATA_SIZE 50

#define N_COL_EXP_SESSION_FILE 24
#define N_COL_DRV_ACTIVITY_FILE 16

#define N_SENSOR_FILES 3
#define N_COL_RAW_BB_RR 3
#define N_COL_RAW_ECG 2
#define N_COL_RAW_GENERAL 17

#define BB_RR 0
#define ECG 1
#define GENERAL 2

#define NONE_PI 0
#define PI_INTERVAL 1
#define PRE_PI_INTERVAL 2
#define POST_PI_INTERVAL 3

using namespace std; // must exist for file open

ifstream input_session_timetable_file;									// session_time_table_new.csv
ifstream input_driver_activity_files[N_SUBJECTS];						// sb01_activities.csv

ifstream input_raw_sensor_data_files[N_SENSOR_FILES][N_SUBJECTS];		

ofstream output_labeled_sensor_data_files[N_SENSOR_FILES][N_SUBJECTS];	

char my_sensor_timestamp[DATA_SIZE]="My_Null";

static bool compare_timestamps(char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[]);
static int detect_PI_moments(char sensor_timestamp[], char PI_timestamp[]);

bool compare_timestamps(char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[])
{
	char year[2][10], month[2][10], date[2][10], hour[3][10], minute[3][10], second[3][10], millisecond[3][10];
	char zero[10]={"0"};
	
	int sensor=0, session_start=1, session_end=2;
	int sensor_digit=0, session_start_digit=0, session_end_digit=0;
		
	// sensor timestamp - "19/12/2013 13:11:16.444"

	// session exp_type - "Separate" or "Both"
	// session datestamp - "11/1/2013"
	// session timestamp - "11:27:30" or "None"
	
	// fyi - formats in Gimlets "29/09/2011 11:28:30.000000"

	char * pch;
	
	char temp[4][100];

	strcpy(temp[0], sensor_timestamp);

	strcpy(temp[1], session_datestamp);
	strcpy(temp[2], session_start_timestamp);
	strcpy(temp[3], session_end_timestamp);
	
	if((strcmp(session_start_timestamp, "None")==0)&&(strcmp(session_end_timestamp, "None")==0)) // 0 -> equal
	{
		return false;
	}else
	{
		// Decompose: Sensor Time
		//pch = strtok (temp[0], "/");	strcpy(date[sensor], pch);
		//pch = strtok (NULL, "/");		strcpy(month[sensor], pch);
		//pch = strtok (NULL, "\n");		strcpy(year[sensor], pch);

		pch = strtok (temp[0], ":");	strcpy(hour[sensor], pch);
		pch = strtok (NULL, ":");		strcpy(minute[sensor], pch);
		pch = strtok (NULL, ".");		strcpy(second[sensor], pch);
		pch = strtok (NULL, "\n");		strcpy(millisecond[sensor], pch);

		strcpy(my_sensor_timestamp, sensor_timestamp);

		// Decompose: Session Time (Start)
		pch = strtok (temp[1], "/");	strcpy(month[session_start], pch);
		pch = strtok (NULL, "/");		strcpy(date[session_start], pch);
		pch = strtok (NULL, "\n");		strcpy(year[session_start], pch);
	
		pch = strtok (temp[2], ":");	strcpy(hour[session_start], pch);
		pch = strtok (NULL, ":");		strcpy(minute[session_start], pch);
		pch = strtok (NULL, "\n");		strcpy(second[session_start], pch);
										strcpy(millisecond[session_start], "000");							
		// Decompose: Session Time (End)
		pch = strtok (temp[3], ":");	strcpy(hour[session_end], pch);
		pch = strtok (NULL, ":");		strcpy(minute[session_end], pch);
		pch = strtok (NULL, "\n");		strcpy(second[session_end], pch);
										strcpy(millisecond[session_end], "000");

		sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(millisecond[sensor]);
		//sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(zero);
		session_start_digit =((atoi(hour[session_start])*60+atoi(minute[session_start]))*60+atoi(second[session_start]))*1000+atoi(millisecond[session_start]);
		session_end_digit =((atoi(hour[session_end])*60+atoi(minute[session_end]))*60+atoi(second[session_end]))*1000+atoi(millisecond[session_end]);

		// NULL, SHAKING, BASELINE1, SHAKING (or NULL), SHAKING (or NULL), BASELINE2, SHAKING

		if((sensor_digit >= session_start_digit)&&(sensor_digit <= session_end_digit))
		{		
			return true;
		}else
		{
			return false;
		}	
	}
}

int detect_PI_moments(char sensor_timestamp[], char driver_PI_timestamp[])
{
	char year[2][10], month[2][10], date[2][10];
	char hour[2][10], minute[2][10], second[2][10], millisecond[2][10];
	char zero[10]={"0"};

	int PI_interval = 1500; // we defined as 1.5 sec (= 1500 ms)

	int sensor=0, driver_pi=1;
	int sensor_digit=0, driver_pi_digit=0;

	// sensor timestamp - "13:14:39.584"
	// driver PI timestamp - "hh:mm:ss.000"
	
	// fyi - formats in Gimlets "29/09/2011 11:28:30.000000"

	char * pch;
	
	char temp[2][100];

	strcpy(temp[sensor], sensor_timestamp);
	strcpy(temp[driver_pi], driver_PI_timestamp);

	// Decompose: Sensor Time
	//pch = strtok (temp[sensor], "/");	strcpy(date[sensor], pch);
	//pch = strtok (NULL, "/");		strcpy(month[sensor], pch);
	//pch = strtok (NULL, "\n");		strcpy(year[sensor], pch);

	pch = strtok (temp[sensor], ":");	strcpy(hour[sensor], pch);
	pch = strtok (NULL, ":");		strcpy(minute[sensor], pch);
	pch = strtok (NULL, ".");		strcpy(second[sensor], pch);
	pch = strtok (NULL, "\n");		strcpy(millisecond[sensor], pch);

	strcpy(my_sensor_timestamp, sensor_timestamp);

	// Decompose: driver PI Time	
	pch = strtok (temp[driver_pi], ":");	strcpy(hour[driver_pi], pch);
	pch = strtok (NULL, ":");				strcpy(minute[driver_pi], pch);
	pch = strtok (NULL, "\n");				strcpy(second[driver_pi], pch);
											strcpy(millisecond[driver_pi], "000");
											
	sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(millisecond[sensor]);
	//sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(zero);
	driver_pi_digit =((atoi(hour[driver_pi])*60+atoi(minute[driver_pi]))*60+atoi(second[driver_pi]))*1000+atoi(millisecond[driver_pi]);

	if(abs(sensor_digit-driver_pi_digit)<=PI_interval)
	{
		return PI_INTERVAL;
		//printf("PI_INTERVAL: sensor=%d, driver_pi=%d, diff=%d \n", sensor_digit, driver_pi_digit, sensor_digit-driver_pi_digit);
	}else if((sensor_digit-driver_pi_digit)>PI_interval)
	{
		return POST_PI_INTERVAL;
		//printf("POST_PI_INTERVAL: sensor=%d, driver_pi=%d, diff=%d \n", sensor_digit, driver_pi_digit, sensor_digit-driver_pi_digit);
		//cin.get();
	}else
	{
		//printf("NONE_PI: sensor=%d, driver_pi=%d, diff=%d \n", sensor_digit, driver_pi_digit, sensor_digit-driver_pi_digit);
		return NONE_PI;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	// file names
	char fname_session_timetable[FNAME_LENGTH];
	char fname_driver_activity[N_SUBJECTS][FNAME_LENGTH];
	char fname_raw_sensor_data_files[N_SENSOR_FILES][N_SUBJECTS][FNAME_LENGTH];
	char fname_labeled_sensor_data_files[N_SENSOR_FILES][N_SUBJECTS][FNAME_LENGTH];

	//sprintf(fname_session_timetable,"session_time_table_new.csv");
	//sprintf(fname_session_timetable,"session_time_table_new_24to26.csv");

	char subject_id_s[5]="NULL";
	char first_subject_id[5]="11";//"13";

	// FOR SEPARATE GROUP - from here
	/*sb05*///char separate_driving_date[2][50]={"01/11/2013","23/12/2013"}; // sb05 - {part1, part2} in BH file date format
	/*sb11*/char separate_driving_date[2][50]={"18/11/2013","08/01/2014"}; // sb11 - {part1, part2} in BH file date format
	int session_num=0; /// FOR SEPARTE GROUP - up to here

	// Matching file names
	for(int i=0; i<N_SUBJECTS; i++)
	{
		// Control subject IDs
		if(i+atoi(first_subject_id)<=9) 
		{
			sprintf(subject_id_s,"0%d",i+atoi(first_subject_id));	// from 01 to 09
		}else 
		{
			itoa(i+atoi(first_subject_id),subject_id_s,10);			// from 10
		}

		sprintf(fname_session_timetable,"session_time_table_%s.csv",subject_id_s); // newly added from the Jaemin's files
		sprintf(fname_driver_activity[i],"sb%s_activities.csv",subject_id_s);

		/* Driver activity files should be re-open at every sensro data file
		input_driver_activity_files[i].open(fname_driver_activity[i]);
		if(!input_driver_activity_files[i]) {cerr<< fname_driver_activity[i] << "<-- Unable to open this Driver Activity file. \n";}
		else{printf("%s is opened\n", fname_driver_activity[i]);}
		*/

		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			if(j==BB_RR)
			{				
				sprintf(fname_raw_sensor_data_files[j][i],"sb%s_BH_BB_RR_synced.csv",subject_id_s);
				sprintf(fname_labeled_sensor_data_files[j][i],"[parsed]sb%s_BH_BB_RR.csv",subject_id_s);
			}else if(j==ECG)
			{
				sprintf(fname_raw_sensor_data_files[j][i],"sb%s_BH_ECG_synced.csv",subject_id_s);
				sprintf(fname_labeled_sensor_data_files[j][i],"[parsed]sb%s_BH_ECG.csv",subject_id_s);
			}else if(j==GENERAL)
			{
				sprintf(fname_raw_sensor_data_files[j][i],"sb%s_BH_General_synced.csv",subject_id_s);
				sprintf(fname_labeled_sensor_data_files[j][i],"[parsed]sb%s_BH_General.csv",subject_id_s);
			}

			input_raw_sensor_data_files[j][i].open(fname_raw_sensor_data_files[j][i]);
			if(!input_raw_sensor_data_files[j][i]) {cerr<< fname_raw_sensor_data_files[j][i] << "<-- Unable to open this Raw Sensor data file. \n";}
			else{printf("%s is opened\n", fname_raw_sensor_data_files[j][i]);}

			output_labeled_sensor_data_files[j][i].open(fname_labeled_sensor_data_files[j][i]);
			if(!output_labeled_sensor_data_files[j][i]) {cerr<< fname_labeled_sensor_data_files[j][i] << "<-- Unable to create the [Labeled] Sensor data file. \n";}
			else{printf("%s is opened\n", fname_labeled_sensor_data_files[j][i]);}
		}
		// Note: As shown above, both input and output files are using the same function 'open()' although it's a creation of new file in the aspect of output files.
	}

	// Sessoion Time Table (STT) file - Open and read 
	input_session_timetable_file.open(fname_session_timetable);
	if(!input_session_timetable_file) {cerr<< fname_session_timetable << "<-- Unable to open the Session Timetable file. \n";}
	else{printf("%s is opened\n", fname_session_timetable);}
		
	char STT_subject_id[N_TIMETABLE_SUBJECTS][DATA_SIZE];//={"NULL";					//[0]		Subject_ID							5				9
	char STT_exp_type[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";						//[1]		Sessions ('Both' or 'Separate')		Separate		Both

	char p1_date[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";						//[2]		Part1_date							11/1/2013		12/13/2013
	char p1_preshaking_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[3]		Preshaking_start					11:27:30		10:08:45
	char p1_preshaking_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[4]		Preshaking_end						11:27:50		10:09:05
	char p1_baseline_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[5]		Baseline_start						11:32:40		10:12:40
	char p1_baseline_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[6]		Baseline_end						11:33:40		10:13:40
		
	char p1_driving_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p1_driving_sigstopT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p1_driving_restartT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p1_driving_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];						// Newly added

	char p1_postshaking_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[7]		Postshaking_start					12:28:10		None
	char p1_postshaking_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[8]		Postshaking_end						12:28:30		None

	char p2_date[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";						//[9]		Part2_date							12/23/2013		12/13/2013
	char p2_preshaking_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[10]		Preshaking_start					11:12:30		None
	char p2_preshaking_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[11]		Preshaking_end						11:12:50		None
	char p2_baseline_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[12]		Baseline_start						11:19:15		11:18:05
	char p2_baseline_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";				//[13]		Baseline_end						11:20:15		11:19:05
	
	char p2_driving_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p2_driving_sigstopT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p2_driving_restartT[N_TIMETABLE_SUBJECTS][DATA_SIZE];					// Newly added
	char p2_driving_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];						// Newly added

	char p2_postshaking_startT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[14]		Postshaking_start					11:54:45		12:01:20
	char p2_postshaking_endT[N_TIMETABLE_SUBJECTS][DATA_SIZE];//="NULL";			//[15]		Postshaking_end						11:55:05		12:01:40


	// Driver Activity file (PI - Peripheral Interactions)
	char PI_subject_id[DATA_SIZE]="NULL";				//[0]	Subject_ID			1
	char PI_driving_session[DATA_SIZE]="NULL";			//[1]	Driving_session		Part1

	char PI_timestamp[DATA_SIZE]="NULL";				//[2]	Datestamps			10/19/2013 or 1:22:42 (6:51, 0:37)
	char PI_year[DATA_SIZE]="NULL";						//[3]	year				2013
	char PI_month[DATA_SIZE]="NULL";					//[4]	month				10
	char PI_date[DATA_SIZE]="NULL";						//[5]	date				19
	char PI_hh[DATA_SIZE]="NULL";						//[6]	hh					12
	char PI_mm[DATA_SIZE]="NULL";						//[7]	mm					24
	char PI_ss[DATA_SIZE]="NULL";						//[8]	ss					40
	char PI_Event_detected[DATA_SIZE]="NULL";			//[9]	Event_detected		SOCIAL
	char PI_Specifics[DATA_SIZE]="NULL";				//[10]	Specifics			RESP_PASSENGER
	char PI_Hand_status[DATA_SIZE]="NULL";				//[11]	Hand_status			BOTH_HANDS_ON_WHEEL
	char PI_Opposite_traffic[DATA_SIZE]="NULL";			//[12]	Opposite_traffic	TIGHTLY
	char PI_Left_lane[DATA_SIZE]="NULL";				//[13]	Left_lane			TIGHTLY
	char PI_My_lane[DATA_SIZE]="NULL";					//[14]	My_lane				TIGHTLY
	char PI_Right_lane[DATA_SIZE]="NULL";				//[15]	Right_lane			NONE
		
	// Raw sensor data 
	char data_BB_RR[N_COL_RAW_BB_RR][DATA_SIZE];
	char data_ECG[N_COL_RAW_ECG][DATA_SIZE];
	char data_General[N_COL_RAW_GENERAL][DATA_SIZE];

	char pre_data_BB_RR[N_COL_RAW_BB_RR][DATA_SIZE];
	char pre_data_ECG[N_COL_RAW_ECG][DATA_SIZE];
	char pre_data_General[N_COL_RAW_GENERAL][DATA_SIZE];
	
	// Scan row by row
	char scanned_session_string[256];
	char scanned_sensor_stream[256];
	char scanned_PI_activity_string[256];

	
	// 1-sec tabulating
	char UTC_timestamp[30]="NULL";

	//char UTC_year[15]="Null_UTCyear";
	//char UTC_month[15]="Null_UTCmonth";
	//char UTC_date[15]="Null_UTCdate";
	//char UTC_hh[15]="Null_UTChh";
	//char UTC_mm[15]="Null_UTCmm";
	//char UTC_ss[15]="Null_UTCss";
	//char UTC_mili[15]="Null_UTCmili";
	
	int UTC_year=0;
	int UTC_month=0;
	int UTC_date=0;
	int UTC_hh=0;
	int UTC_mm=0;
	int UTC_ss=0;
	int UTC_mili=0;

	//Scan Session Time Table file
	input_session_timetable_file >> scanned_session_string;	// read the 1st row - 'title' row (will just skip)

	char * pch;										// will be used to check the 1st data of any data stream
	
	int i=0;
	int subject_count=0;

	while(!input_session_timetable_file.eof())
	{
		input_session_timetable_file >> scanned_session_string;	// read from the 2nd row

		pch = strtok (scanned_session_string, ",");	
		if(pch == NULL) break;						// in order to take care of the last 'NULL' row - if there is no ';' to search, 'break'

		strcpy(STT_subject_id[i], pch); // proceed until the 1st delimter

		//printf("pch = %s, subject_id[i] = %s \n", pch, STT_subject_id[i]);

		pch = strtok (NULL, ",");	strcpy(STT_exp_type[i], pch);		
		
		// Part 1 driving
		pch = strtok (NULL, ",");	strcpy(p1_date[i], pch);
		pch = strtok (NULL, ",");	strcpy(p1_preshaking_startT[i], pch);
		pch = strtok (NULL, ",");	strcpy(p1_preshaking_endT[i], pch);
		pch = strtok (NULL, ",");	strcpy(p1_baseline_startT[i], pch);		
		pch = strtok (NULL, ",");	strcpy(p1_baseline_endT[i], pch);

		pch = strtok (NULL, ",");	strcpy(p1_driving_startT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p1_driving_sigstopT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p1_driving_restartT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p1_driving_endT[i], pch);

		pch = strtok (NULL, ",");	strcpy(p1_postshaking_startT[i], pch);
		pch = strtok (NULL, ",");	strcpy(p1_postshaking_endT[i], pch);
		
		// Part 2 driving
		pch = strtok (NULL, ",");	strcpy(p2_date[i], pch);
		pch = strtok (NULL, ",");	strcpy(p2_preshaking_startT[i], pch);
		pch = strtok (NULL, ",");	strcpy(p2_preshaking_endT[i], pch);
		pch = strtok (NULL, ",");	strcpy(p2_baseline_startT[i], pch);		
		pch = strtok (NULL, ",");	strcpy(p2_baseline_endT[i], pch);

		pch = strtok (NULL, ",");	strcpy(p2_driving_startT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p2_driving_sigstopT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p2_driving_restartT[i], pch);	
		pch = strtok (NULL, ",");	strcpy(p2_driving_endT[i], pch);

		pch = strtok (NULL, ",");	strcpy(p2_postshaking_startT[i], pch);
		pch = strtok (NULL, "\n");	strcpy(p2_postshaking_endT[i], pch);

		//printf("i=%d : %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", i, STT_subject_id[i], STT_exp_type[i], 
		//	p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i], p1_baseline_startT[i], p1_baseline_endT[i], p1_postshaking_startT[i], p1_postshaking_endT[i],
		//	p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i], p2_baseline_startT[i], p2_baseline_endT[i], p2_postshaking_startT[i], p2_postshaking_endT[i]);

		//printf("i=%d : %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", i, STT_subject_id[i], STT_exp_type[i], 
		//p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i], p1_baseline_startT[i], p1_baseline_endT[i], p1_driving_startT[i], p1_driving_sigstopT[i], p1_driving_restartT[i], p1_driving_endT[i], p1_postshaking_startT[i], p1_postshaking_endT[i],
		//p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i], p2_baseline_startT[i], p2_baseline_endT[i], p2_driving_startT[i], p2_driving_sigstopT[i], p2_driving_restartT[i], p2_driving_endT[i], p2_postshaking_startT[i], p2_postshaking_endT[i]);

		i++;
		//cin.get();
	}
	
	// Printing output files while reading raw sensor data stream files.

	for(i=0; i<N_SUBJECTS; i++)
	{		
		int count_samples=0;
		int pre_UTC_ss=0;
		int pre_UTC_mm=0;
		int pre_UTC_hh=0;

		double total_BR_per_unit_time=0.0;
		double total_RtoR_per_unit_time=0.0;
		double total_ECG_per_unit_time=0.0;

		double pre_BR_mean=0.0;
		double pre_RtoR_mean=0.0;
		double pre_ECG_mean=0.0;

		double stack_BR_for_std[50];
		double stack_RtoR_for_std[50];
		double stack_ECG_for_std[1000];

		for(int k=0; k<50; k++)
		{
			stack_BR_for_std[k]=0.0;
			stack_RtoR_for_std[k]=0.0;
		}

		for(int k=0; k<1000; k++)
		{
			stack_ECG_for_std[k]=0.0;
		}
		
		char pre_B_fluctuation[15]="Null";
		int BF_elapsed_time=0;

		// Write in the output file
		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			
			input_driver_activity_files[i].open(fname_driver_activity[i]);
			if(!input_driver_activity_files[i]) {cerr<< fname_driver_activity[i] << "<-- Unable to open this Driver Activity file. \n";}
			else{printf("%s is opened\n", fname_driver_activity[i]);}

			if(j==BB_RR)
			{
				output_labeled_sensor_data_files[j][i] << "subject" << "," <<	"session" << "," 
					<< "UTC_Timestamp" << "," 
					<< "BR_mean" << "," << "BR_stdev" << "," << "BR_1std_BB_RR" << ","
					<< "B_level" << "," << "B_fluctuation" << "," << "BF_elapsed_time" << ","
					<< "RtoR_mean" << "," << "RtoR_stdev" << "," << "RtoR_1std" //<< ","
					//<< "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
					<< "\n";
			}else if(j==ECG)
			{
				output_labeled_sensor_data_files[j][i] << "subject" << "," <<	"session" << "," 
					<< "UTC_Timestamp" << "," 
					<< "ECG_mean" << "," << "ECG_stdev" << "," << "ECG_1std" //<< ","
					//<< "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
					<< "\n";
			}else if(j==GENERAL)
			{
				output_labeled_sensor_data_files[j][i] << "subject" << "," << "session" << "," 
					<< "UTC_Timestamp" << ","
					<< "HR" << "," << "HR_1std" << "," 
					<< "BR" << "," << "BR_1std_General" << ","
					<< "Temperature" << "," << "Temperature_1std" << "," 
					<< "Posture" << "," << "Posture_1std" << "," 
					<< "Activity" << "," << "Activity_1std" << "," 
					<< "Acceleration" << "," << "Acceleration_1std" << "," 
					<< "Battery" << "," << "Battery_1std" << "," 
					<< "BR_Amplitude" << "," << "BR_Amplitude_1std" << "," 
					<< "ECG_Amplitude" << "," << "ECG_Amplitude_1std" << "," 
					<< "ECG_Noise" << "," << "ECG_Noise_1std" << "," 
					<< "XMin" << "," << "XMin_1std" << ","
					<< "XPeak" << "," << "XPeak_1std" << "," 
					<< "YMin" << "," << "YMin_1std" << "," 
					<< "YPeak" << "," << "YPeak_1std" << ","
					<< "ZMin" << "," << "ZMin_1std" << "," 
					<< "ZPeak" << "," << "ZPeak_1std" //<< "," 
					//<< "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
					<< "\n";
			}
					
			bool Used=true;
			char driver_PI_timestamp [50] = "Null"; // reformat just for the sub-function 'detect_PI_moments'
			char driver_PI_log[500]="Null,Null,Null,Null,Null,Null,Null";
				
			input_driver_activity_files[i] >> scanned_PI_activity_string;	// Skip the 1st 'title' row
			//printf("PI_string: %s \n", scanned_PI_activity_string);
			//cin.get();

			input_raw_sensor_data_files[j][i] >> scanned_sensor_stream;		// Skip the 1st 'title' row
			//printf("sensor_string: %s \n", scanned_sensor_stream);
			//cin.get();

			while(!input_raw_sensor_data_files[j][i].eof()) 
			{			
				// Sensor Data Streams 
				input_raw_sensor_data_files[j][i] >> scanned_sensor_stream; // >> reads spaces, so should skip the date data (i.e., front part of the 2nd 'Timestamp' row
				sscanf(scanned_sensor_stream,"%d/%d/%d ", &UTC_date, &UTC_month, &UTC_year);

				// Only related to the two subjects in the SEPARATE group
				if(strcmp(scanned_sensor_stream,separate_driving_date[0])==0)
				{
					session_num=1;		// part 1 driving of the separate subject (either sb05 or sb11)
				}else if(strcmp(scanned_sensor_stream,separate_driving_date[1])==0)
				{
					session_num=2;		// part 2 driving of the separate subject (either sb05 or sb11)
				}else
				{
					session_num=0;
				}// up to here


				input_raw_sensor_data_files[j][i] >> scanned_sensor_stream; // this is back part of the 2nd 'Timestamp' row
				
				//printf("sensor_string: %s \n", scanned_sensor_stream);
				//cin.get();

				pch = strtok (scanned_sensor_stream, ",");	// proceed until the delimter ','
				if(pch == NULL) break;						// in order to take care of the last 'NULL' row - if there is no ',' to search, 'break' - Note that this checking must come before 'strcpy'
				
				if(j==BB_RR)
				{
					strcpy(data_BB_RR[0], pch);								// Timestamp
					pch = strtok (NULL, ","); strcpy(data_BB_RR[1], pch);	// BR
					pch = strtok (NULL, "\n"); strcpy(data_BB_RR[2], pch);	// RtoR

					//printf("BB_RR: %s, %s, %s \n", data_BB_RR[0], data_BB_RR[1], data_BB_RR[2]);
				}else if(j==ECG)
				{
					strcpy(data_ECG[0], pch);								// Timestamp
					pch = strtok (NULL, "\n"); strcpy(data_ECG[1], pch);	// ECG		

					//printf("ECG: %s, %s\n", data_ECG[0], data_ECG[1]);
				}else if(j==GENERAL)
				{
					strcpy(data_General[0], pch);							// Timestamp
					pch = strtok (NULL, ","); strcpy(data_General[1], pch);	// HR
					pch = strtok (NULL, ","); strcpy(data_General[2], pch);	// BR
					pch = strtok (NULL, ","); strcpy(data_General[3], pch);	// Temp
					pch = strtok (NULL, ","); strcpy(data_General[4], pch);	// Posture
					pch = strtok (NULL, ","); strcpy(data_General[5], pch);	// Activity
					pch = strtok (NULL, ","); strcpy(data_General[6], pch);	// Acceleration
					pch = strtok (NULL, ","); strcpy(data_General[7], pch);	// Battery
					pch = strtok (NULL, ","); strcpy(data_General[8], pch);	// BRAmplitude
					pch = strtok (NULL, ","); strcpy(data_General[9], pch);	// ECGAmplitude
					pch = strtok (NULL, ","); strcpy(data_General[10], pch);// ECGNoise
					pch = strtok (NULL, ","); strcpy(data_General[11], pch);// XMin
					pch = strtok (NULL, ","); strcpy(data_General[12], pch);// XPeak
					pch = strtok (NULL, ","); strcpy(data_General[13], pch);// YMin
					pch = strtok (NULL, ","); strcpy(data_General[14], pch);// YPeak
					pch = strtok (NULL, ","); strcpy(data_General[15], pch);// ZMin
					pch = strtok (NULL, "\n"); strcpy(data_General[16], pch);//ZPeak 
										
					//printf("General: %s, %s, ..., %s\n", data_General[0], data_General[1], data_General[16]);
				}
						
			
				//cin.get();
				// Compare timestamps in 'Sensor' and 'Session', and then get 'labeles' for 'session_id' of the output file 		
				char session_id[15] = "NULL_SESSION";
				char timestamp[50]= "NULL_TIMESTAMP";

				if(j==BB_RR)
				{
					strcpy(timestamp, data_BB_RR[0]);	//Timestamp column in BB_RR file
				}else if(j==ECG)
				{
					strcpy(timestamp, data_ECG[0]);		//Timestamp column in ECG file
				}else if(j==GENERAL)
				{
					strcpy(timestamp, data_General[0]);	//Timestamp column in General file
				}

				/* Original code
				if(compare_timestamps(timestamp, p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i])==true)
				{
					strcpy(session_id, "PRE_SHAKING1");							//Part 1 - pre-shaking
				}else
				{
					if(compare_timestamps(timestamp, p1_date[i], p1_baseline_startT[i], p1_baseline_endT[i])==true)
					{
						strcpy(session_id, "BASELINE1");						// Part 1- baseline
					}else
					{		
						if(compare_timestamps(timestamp, p1_date[i], p1_driving_startT[i], p1_driving_endT[i])==true)
						{
							strcpy(session_id, "DRIVING1");						// Part 1 - driving
						}else
						{
							if(compare_timestamps(timestamp, p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i])==true)
							{
								strcpy(session_id, "PRE_SHAKING2");				// Part 2 - pre-shaking
							}else
							{
								if(compare_timestamps(timestamp, p2_date[i], p2_baseline_startT[i], p2_baseline_endT[i])==true)
								{
									strcpy(session_id, "BASELINE2");			// Part 2 - baseline
								}else
								{							
									if(compare_timestamps(timestamp, p2_date[i], p2_driving_startT[i], p2_driving_endT[i])==true)
									{
										strcpy(session_id, "DRIVING2");			// Part 2 - driving
									}else
									{
										if(compare_timestamps(timestamp, p2_date[i], p2_postshaking_startT[i], p2_postshaking_endT[i])==true)
										{
											strcpy(session_id, "POST_SHAKING2");// Part 2 - post-shaking
										}else
										{
											strcpy(session_id, "NULL_SESSION");
										}
									}
								}									
							}						
						}					
					}
				}	Original code - up to here	*/			
				
			// Only for the two subjects in the SEPARATE group
			if(session_num==1) // Part 1 driving of the separate subject (either sb05 or sb11)
			{			
				if(compare_timestamps(timestamp, p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i])==true)
				{
					strcpy(session_id, "PRE_SHAKING1");							//Part 1 - pre-shaking
				}else
				{
					if(compare_timestamps(timestamp, p1_date[i], p1_baseline_startT[i], p1_baseline_endT[i])==true)
					{
						strcpy(session_id, "BASELINE1");						// Part 1- baseline
					}else
					{		
						if(compare_timestamps(timestamp, p1_date[i], p1_driving_startT[i], p1_driving_endT[i])==true)
						{
							strcpy(session_id, "DRIVING1");						// Part 1 - driving
						}else
						{
							if(compare_timestamps(timestamp, p1_date[i], p1_postshaking_startT[i], p1_postshaking_endT[i])==true)
							{
								strcpy(session_id, "POST_SHAKING1");	// Part 1 - post-shaking
							}else
							{
								strcpy(session_id, "NULL_SESSION");
							}						
						}					
					}
				} 
			}else if(session_num==2)// part 2 driving of the separate subject (either sb05 or sb11)
			{
				if(compare_timestamps(timestamp, p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i])==true)
				{
					strcpy(session_id, "PRE_SHAKING2");				// Part 2 - pre-shaking
				}else
				{
					if(compare_timestamps(timestamp, p2_date[i], p2_baseline_startT[i], p2_baseline_endT[i])==true)
					{
						strcpy(session_id, "BASELINE2");			// Part 2 - baseline
					}else
					{							
						if(compare_timestamps(timestamp, p2_date[i], p2_driving_startT[i], p2_driving_endT[i])==true)
						{
							strcpy(session_id, "DRIVING2");			// Part 2 - driving
						}else
						{
							if(compare_timestamps(timestamp, p2_date[i], p2_postshaking_startT[i], p2_postshaking_endT[i])==true)
							{
								strcpy(session_id, "POST_SHAKING2");// Part 2 - post-shaking
							}else
							{
								strcpy(session_id, "NULL_SESSION");
							}
						}
					}									
				}
			}///////////////////////////////////////// up to here (SEPARATE group)

				if(Used==true)
				{
					//get new one if it's used
					input_driver_activity_files[i] >> scanned_PI_activity_string; // Realized that it reads only - the first part before the 1st sapce --> 13, Part, "1:22:42
				
					pch = strtok (scanned_PI_activity_string, ",");	
					if(pch != NULL) 	// in order to take care of null data in the last row - Note that this checking must come before 'strcpy'
					{
						strcpy(PI_subject_id, pch); // proceed until the 1st delimter
						//printf("pch = %s, subject_id[i] = %s \n", pch, PI_subject_id);

						pch = strtok (NULL, ",");	strcpy(PI_driving_session, pch);		
		
						pch = strtok (NULL, ",");	strcpy(PI_timestamp, pch);

						// Note: there are 'spaces' in the timstamp colum, so we need to run more 'reading' for that column
						input_driver_activity_files[i] >> scanned_PI_activity_string; //Read until the 2nd space in the timestamp column

						input_driver_activity_files[i] >> scanned_PI_activity_string; //Read after the 3rd space in the timestamp column

						strtok (scanned_PI_activity_string, ",");
						pch = strtok (NULL, ",");	strcpy(PI_year, pch);
						pch = strtok (NULL, ",");	strcpy(PI_month, pch);
						pch = strtok (NULL, ",");	strcpy(PI_date, pch);		
						pch = strtok (NULL, ",");	strcpy(PI_hh, pch);
						pch = strtok (NULL, ",");	strcpy(PI_mm, pch);
						pch = strtok (NULL, ",");	strcpy(PI_ss, pch);
		
						pch = strtok (NULL, ",");	strcpy(PI_Event_detected, pch);
						pch = strtok (NULL, ",");	strcpy(PI_Specifics, pch);
						pch = strtok (NULL, ",");	strcpy(PI_Hand_status, pch);
						pch = strtok (NULL, ",");	strcpy(PI_Opposite_traffic, pch);		
						pch = strtok (NULL, ",");	strcpy(PI_Left_lane, pch);
						pch = strtok (NULL, ",");	strcpy(PI_My_lane, pch);
						pch = strtok (NULL, "\n");	strcpy(PI_Right_lane, pch);

						sprintf(driver_PI_timestamp,"%s:%s:%s", PI_hh, PI_mm, PI_ss);

						//printf("Driver Activity: %s(%s) - %s; %s; %s; %s; %s; %s; %s \n", driver_PI_timestamp, PI_subject_id, PI_Event_detected, PI_Specifics, PI_Hand_status, PI_Opposite_traffic, PI_Left_lane, PI_My_lane, PI_Right_lane);
						//cin.get();
						Used=false;
					}
				}						

				if(detect_PI_moments(timestamp, driver_PI_timestamp)==PI_INTERVAL) // (timestamps in sensor data steam, timestamps of the driver PI moment)
				{
					sprintf(driver_PI_log,"%s,%s,%s,%s,%s,%s,%s",PI_Event_detected, PI_Specifics, PI_Hand_status, PI_Opposite_traffic, PI_Left_lane, PI_My_lane, PI_Right_lane);
					Used=false;
				}else if(detect_PI_moments(timestamp, driver_PI_timestamp)==POST_PI_INTERVAL)
				{
					strcpy(driver_PI_log, "POST_PI,POST_PI,POST_PI,POST_PI,POST_PI,POST_PI,POST_PI");
					Used=true;
				}else
				{
					strcpy(driver_PI_log, "Null,Null,Null,Null,Null,Null,Null");
					//Used=true;
				}
					
				strcpy(timestamp, my_sensor_timestamp); // if millisecond data in 'timestamp' is fewer than 100, added '0' through detect_PI_moments or compare_timestamp functions, 
				//printf("sensor timestamp = %s \n", timestamp);

				// Printing output files
				if((strcmp(session_id,"PRE_SHAKING1")==0)|(strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"DRIVING1")==0)|(strcmp(session_id,"POST_SHAKING1")==0)|
				   (strcmp(session_id,"PRE_SHAKING2")==0)|(strcmp(session_id,"BASELINE2")==0)|(strcmp(session_id,"DRIVING2")==0)|(strcmp(session_id,"POST_SHAKING2")==0))
				{
					if(j==BB_RR)
					{
						printf("start to read data_BB_RR[0] \n");
						sscanf(data_BB_RR[0],"%d:%d:%d.%d", &UTC_hh, &UTC_mm, &UTC_ss, &UTC_mili); //easier to load as intergers, rather than strings, when using sscanf
						sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, UTC_ss);	
						printf("end reading the data_BB_RR[0] \n");
						printf("BB_RR: Current UTC (%s): %s (%s)\n", STT_subject_id[i], UTC_timestamp, data_BB_RR[0]);
						//cin.get();

						if((pre_UTC_ss!=UTC_ss)|(pre_UTC_mm!=UTC_mm)|(pre_UTC_hh!=UTC_hh)) // checked mm and hh as well just in case that ss can be the same when trasiting sessions
						{
							printf("loop_printing...\\nn");
							double temp_BR_pow_sum=0;
							double temp_RtoR_pow_sum=0;

							if(count_samples==0){count_samples=1;}; // just to avoid divided by zero in the first row

							for(int ii=0; ii < count_samples; ii++)
							{
								temp_BR_pow_sum+=pow(stack_BR_for_std[ii+1]-total_BR_per_unit_time/count_samples, 2.0);
								temp_RtoR_pow_sum+=pow(stack_RtoR_for_std[ii+1]-total_RtoR_per_unit_time/count_samples, 2.0);
							}

							double BR_1std=total_BR_per_unit_time/count_samples-pre_BR_mean;
							double RtoR_1st=total_RtoR_per_unit_time/count_samples-pre_RtoR_mean;

							char B_level[15]="Null";
							char B_fluctuation[15]="Null";

							if(total_BR_per_unit_time/count_samples>=50000)
							{
								strcpy(B_level, "High");
							}else if(total_BR_per_unit_time/count_samples<20000)
							{
								strcpy(B_level, "Low");
							}else
							{
								strcpy(B_level, "Medium");
							}

							if(BR_1std>50)
							{
								strcpy(B_fluctuation, "Breathing_in");
							}else if(BR_1std<-50)
							{
								strcpy(B_fluctuation, "Breathing_out");
							}else
							{
								strcpy(B_fluctuation, "Breath_holding");
							}

							if(strcmp(B_fluctuation, pre_B_fluctuation)==0)
							{
								BF_elapsed_time++;  //+1sec
							}else
							{
								BF_elapsed_time=1;
							}

							strcpy(pre_B_fluctuation, B_fluctuation);

							output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," 
								<< UTC_timestamp << "," 
								<< total_BR_per_unit_time/count_samples << "," << sqrt(temp_BR_pow_sum/count_samples) << "," << BR_1std << ","
								<< B_level << "," << B_fluctuation << "," << BF_elapsed_time << ","
								<< total_RtoR_per_unit_time/count_samples << "," << sqrt(temp_RtoR_pow_sum/count_samples) << "," << RtoR_1st //<< ","
								//<< driver_PI_log << endl;
								<< endl;
							
							printf("%s BB_RR (sb%s, session - %s)\n",UTC_timestamp, STT_subject_id[i], session_id); 
							//output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," << UTC_timestamp << "," << data_BB_RR[1] << "," << data_BB_RR[2] << "," << driver_PI_log << endl;
							
							//printf("ss: pre %d != %d\n", pre_UTC_ss, UTC_ss);
							//printf("mm: pre %d != %d\n", pre_UTC_mm, UTC_mm);
							//printf("hh: pre %d != %d\n", pre_UTC_hh, UTC_hh);

							//printf("%s (count_samples= %d): BR_mean %f\n", UTC_timestamp, count_samples, total_BR_per_unit_time/count_samples);
							//printf("%s (count_samples= %d): RtoR_mean %f\n", UTC_timestamp, count_samples, total_RtoR_per_unit_time/count_samples);

							//printf("%s (count_samples= %d): BR_std %f\n", UTC_timestamp, count_samples, sqrt(temp_BR_pow_sum/(count_samples-1)));
							//printf("%s (count_samples= %d): RtoR_std %f\n", UTC_timestamp, count_samples, sqrt(temp_RtoR_pow_sum/(count_samples-1)));

							//cin.get();

							pre_BR_mean=total_BR_per_unit_time/count_samples;
							pre_RtoR_mean=total_RtoR_per_unit_time/count_samples;

							count_samples=1;
							total_BR_per_unit_time=atof(data_BB_RR[1]);
							total_RtoR_per_unit_time=atof(data_BB_RR[2]);

							stack_BR_for_std[count_samples]=atof(data_BB_RR[1]);
							stack_RtoR_for_std[count_samples]=atof(data_BB_RR[2]);

						}else
						{
							printf("loop_stacking...\\nn");
							count_samples++; // WORK FROM HERE

							total_BR_per_unit_time+=atof(data_BB_RR[1]);
							total_RtoR_per_unit_time+=atof(data_BB_RR[2]);

							stack_BR_for_std[count_samples]=atof(data_BB_RR[1]);
							stack_RtoR_for_std[count_samples]=atof(data_BB_RR[2]);
						}
						
						pre_UTC_ss=UTC_ss;
						pre_UTC_mm=UTC_mm;
						pre_UTC_hh=UTC_hh;

						printf("Scan the Next BB_RR data\n\n");

						//output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," << data_BB_RR[0] << "," << data_BB_RR[1] << "," << data_BB_RR[2] << "," << driver_PI_log << endl;
					}else if(j==ECG)
					{
						sscanf(data_ECG[0],"%d:%d:%d.%d", &UTC_hh, &UTC_mm, &UTC_ss, &UTC_mili);
						sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, UTC_ss);	
						printf("ECG: Current UTC (%s): %s (%s)\n", STT_subject_id[i], UTC_timestamp, data_ECG[0]);
						
						if((pre_UTC_ss!=UTC_ss)|(pre_UTC_mm!=UTC_mm)|(pre_UTC_hh!=UTC_hh)) // checked mm and hh as well just in case that ss can be the same when trasiting sessions
						{		

							double temp_ECG_pow_sum=0;

							if(count_samples==0){count_samples=1;}; // just to avoid divided by zero in the first row

							for(int ii=0; ii < count_samples; ii++)
							{
								temp_ECG_pow_sum+=pow(stack_ECG_for_std[ii+1]-total_ECG_per_unit_time/count_samples, 2.0);
							}

							double ECG_1std=total_ECG_per_unit_time/count_samples-pre_ECG_mean;

							output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," 
									<< UTC_timestamp << "," 
									<< total_ECG_per_unit_time/count_samples << "," << sqrt(temp_ECG_pow_sum/count_samples) << "," << ECG_1std //<< ","
									//<< driver_PI_log << endl;
									<< endl;				

							pre_ECG_mean=total_ECG_per_unit_time/count_samples;

							count_samples=1;

							total_ECG_per_unit_time=atof(data_ECG[1]);
							stack_ECG_for_std[count_samples]=atof(data_ECG[1]);

							//printf("PRINT_OUT \n Current UTC (%s): %s, data_ECG[0]=%s\n", STT_subject_id[i], UTC_timestamp, data_ECG[0]);
							//printf("temp_ECG_pow_sum=%f, total_ECG_per_unit_time=%f / count_samples=%d \n",temp_ECG_pow_sum, total_ECG_per_unit_time, count_samples);
							//cin.get();
						}else
						{
							count_samples++; 

							//printf("PASSED \n Current UTC (%s): %s count_samples=%03d \n", STT_subject_id[i], data_ECG[0], count_samples);

							total_ECG_per_unit_time+=atof(data_ECG[1]); //printf("total_ECG_per_unit_time=%f / data_ECG[1]=%s /count_samples=%d \n", total_ECG_per_unit_time, data_ECG[1], count_samples);
							stack_ECG_for_std[count_samples]=atof(data_ECG[1]); //printf("stack_ECG_for_std=%f / data_ECG[1]=%s /count_samples=%d \n", stack_ECG_for_std[count_samples], data_ECG[1], count_samples);							
							
							//cin.get();
						}
						
						pre_UTC_ss=UTC_ss;
						pre_UTC_mm=UTC_mm;
						pre_UTC_hh=UTC_hh;

						//output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," << data_ECG[0] << "," << data_ECG[1] << "," << driver_PI_log << endl;
					}else if(j==GENERAL)
					{
						sscanf(data_General[0],"%d:%d:%d.%d", &UTC_hh, &UTC_mm, &UTC_ss, &UTC_mili);

						UTC_ss=UTC_ss+1;
						sprintf(UTC_timestamp,"%d-%d-%dT%02d:%02d:%02d.000-05:00", UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, UTC_ss);	
						
						printf("General: Current UTC (%s): %s (%s)\n", STT_subject_id[i], UTC_timestamp, data_General[0]);
						//printf("Current UTC (%s): %s\n", STT_subject_id[i], UTC_timestamp);
						//cin.get();

						output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," 
							<< UTC_timestamp << "," 
							<< data_General[1] << "," << atof(data_General[1])-atof(pre_data_General[1]) << ","
							<< data_General[2] << "," << atof(data_General[2])-atof(pre_data_General[2]) << "," 
							<< data_General[3] << "," << atof(data_General[3])-atof(pre_data_General[3]) << "," 
							<< data_General[4] << "," << atof(data_General[4])-atof(pre_data_General[4]) << "," 
							<< data_General[5] << "," << atof(data_General[5])-atof(pre_data_General[5]) << "," 
							<< data_General[6] << "," << atof(data_General[6])-atof(pre_data_General[6]) << "," 
							<< data_General[7] << "," << atof(data_General[7])-atof(pre_data_General[7]) << "," 
							<< data_General[8] << "," << atof(data_General[8])-atof(pre_data_General[8]) << "," 
							<< data_General[9] << "," << atof(data_General[9])-atof(pre_data_General[9]) << "," 
							<< data_General[10] << "," << atof(data_General[10])-atof(pre_data_General[10]) << "," 
							<< data_General[11] << "," << atof(data_General[11])-atof(pre_data_General[11]) << "," 
							<< data_General[12] << "," << atof(data_General[12])-atof(pre_data_General[12]) << "," 
							<< data_General[13] << "," << atof(data_General[13])-atof(pre_data_General[13]) << "," 
							<< data_General[14] << "," << atof(data_General[14])-atof(pre_data_General[14]) << "," 
							<< data_General[15] << "," << atof(data_General[15])-atof(pre_data_General[15]) << "," 
							<< data_General[16] << "," << atof(data_General[16])-atof(pre_data_General[16]) //<< "," 
							//<< driver_PI_log << endl;
							<< endl;

						//printf("%s General (sb%s, session - %s)\n",UTC_timestamp, STT_subject_id[i], session_id); 

						for (int ii=0; ii<16; ii++)
						{
							strcpy(pre_data_General[ii+1], data_General[ii+1]);
						}
						/*output_labeled_sensor_data_files[j][i] << STT_subject_id[i] << "," << session_id << "," 
							<< data_General[0] << "," 
							<< data_General[1] << "," 
							<< data_General[2] << "," 
							<< data_General[3] << "," 
							<< data_General[4] << "," 
							<< data_General[5] << "," 
							<< data_General[6] << "," 
							<< data_General[7] << "," 
							<< data_General[8] << "," 
							<< data_General[9] << "," 
							<< data_General[10] << "," 
							<< data_General[11] << "," 
							<< data_General[12] << "," 
							<< data_General[13] << "," 
							<< data_General[14] << "," 
							<< data_General[15] << "," 
							<< data_General[16] << "," << driver_PI_log << endl;*/
					}
				}

				
			}	

		// Close & Clear driver activity files
		input_driver_activity_files[i].close(); input_driver_activity_files[i].clear(); printf("%s is closed \n", fname_driver_activity[i]);

		}
		//cin.get(); // wait until a keystroke in the command window	
	}
	
	// Close & Clear files
	
	//1 - session timetable
	input_session_timetable_file.close(); input_session_timetable_file.clear(); printf("%s is closed \n", fname_session_timetable);			

	for(int i=0; i<N_SUBJECTS; i++)
	{		
		// Control subject IDs
		if(i+atoi(first_subject_id)<=9) 
		{
			sprintf(subject_id_s,"0%d",i+atoi(first_subject_id));	// from 01 to 09
		}else 
		{
			itoa(i+atoi(first_subject_id),subject_id_s,10);			// from 10
		}
		
		for(int j=0; j<N_SENSOR_FILES; j++)
		{
			//3 - raw sensor data
			input_raw_sensor_data_files[j][i].close(); input_raw_sensor_data_files[j][i].clear(); printf("%s is closed \n", fname_raw_sensor_data_files[j][i]);	

			//4 - labelled sensor data
			output_labeled_sensor_data_files[j][i].close(); output_labeled_sensor_data_files[j][i].clear(); printf("%s is closed \n", fname_labeled_sensor_data_files[j][i]); 
		}
	}

	return 0;
}


