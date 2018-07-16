// MYEI_parser.cpp : Defines the entry point for the console application.
//
/* Jan 15, 2014 created by SeungJun Kim (HCII, CMU)

(1) Look-up table 1 - one session timetable (i.e., session_time_table.csv) - 16 data in a row / delimiter - ','

	<NOTE 1: Will store all corresponding data in variables first in advance>
	<NOTE 2: Need to search and read only one row containing session time data for the subject to process>

	<STEP 1: Choose Subject_ID to process, and search a corresponding row>
	<STEP 2: Create variables that correspond to each of the columns, and store all data in the variables>

	[0]		Subject_ID							5				9
	[1]		Exp_type							Separate		Both

	[2]		Part1_date							11/1/2013		12/13/2013
	[3]		Preshaking_start					11:27:30		10:08:45
	[4]		Preshaking_end						11:27:50		10:09:05
	[5]		Baseline_start						11:32:40		10:12:40
	[6]		Baseline_end						11:33:40		10:13:40
	[7]		Postshaking_start					12:28:10		None
	[8]		Postshaking_end						12:28:30		None

	[9]		Part2_date							12/23/2013		12/13/2013
	[10]	Preshaking_start					11:12:30		None
	[11]	Preshaking_end						11:12:50		None
	[12]	Baseline_start						11:19:15		11:18:05
	[13]	Baseline_end						11:20:15		11:19:05
	[14]	Postshaking_start					11:54:45		12:01:20
	[15]	Postshaking_end						11:55:05		12:01:40

(2) Look-up table 2 - multiple acvitity records (e.g., sb01_activities.csv, sb02_activities.csv, sb03_activities.csv,,,) - 16 data in a row / delimiter - ','

	<NOTE 3: Will store all corresponding data in variables first in advance>
	<NOTE 4: Need to select only one activity file for a corresponding subject>

	<STEP 3: Create variables that correspond to each of the columns, by assigning MAX of the number of activities during all driving sessions of a subject - 500>

	[0]		Subject_ID			1
	[1]		Driving_session		Part1
	[2]		Datestamps			10/19/2013
	[3]		year				2013
	[4]		month				10
	[5]		date				19
	[6]		hh					12
	[7]		mm					24
	[8]		ss					40
	[9]		Event_detected		SOCIAL
	[10]	Specifics			RESP_PASSENGER
	[11]	Hand_status			BOTH_HANDS_ON_WHEEL
	[12]	Opposite_traffic	TIGHTLY
	[14]	Left_lane			TIGHTLY
	[15]	My_lane				TIGHTLY
	[15]	Right_lane			NONE

(3) MYEI Data Streams (e.g., sb02_MYEI.txt, sb02_MYEI.txt, sb03_MYEI.txt,,,) - 15 data in a row / delimiter - ';'

	<NOTE 5: Will read and process row by row unlike tasks with look-up tables, and generate a labeled output file by also writing/annotating data row by row>

	<STEP 4: Read a row until the end dlimiter '#' of each row>
	<STEP 5: Append 'PRESHAKING' if 'SENSOR date/time stamps' >= 'START date/time stamps' of the Part 1 Preshaking session.>
	<STEP 6: End appending 'PRESHAKING' if 'SENSOR date/time stamps' < 'START date/time stamps' of the Part 1 Preshaking session.>


	, and then append the sesson and activity information, and then >

	Step 2 - check the 1st column; use only if it is 'Y0','Y1','Y2', or 'Y3'(otherwise, skip)

	[0]		Sensor_ID		Y0
	[1]		Body			Left_H
	[2]		Device_ID		00:06:66:09:C6:BA
	[3]		UNIX_time		1382195639472
	[4]		Datestamp		2013/10/19
	[5]		Timestamp		11:13:59.472
	[6]		Gravity_x		-0.115
	[7]		Gravity_y		0.917
	[8]		Gravity_z		-0.382
	[9]		Gyro_x			0.022
	[10]	Gyro_y			0.051
	[11]	Gyro_z			-0.044
	[12]	North_x			0.714
	[13]	North_y			-0.590
	[14]	North_z			-0.376

	Y0;Left_H;00:06:66:09:C6:BA;1382195639472;2013/10/19;11:13:59.472;-0.115;0.917;-0.382;0.022;0.051;-0.044;0.714;-0.590;-0.376#
	Y1;Head;00:06:66:09:C6:AA;1382195639472;2013/10/19;11:13:59.472;-0.874;0.477;-0.089;-0.065;0.079;-0.056;-0.127;-0.873;-0.471#
	Y2;Foot;00:06:66:0A:30:C8;1382195639472;2013/10/19;11:13:59.472;-0.589;0.552;-0.590;-0.348;0.189;-0.014;-0.724;-0.116;-0.680#
	Y3;Right_H;00:06:66:09:46:62;1382195639472;2013/10/19;11:13:59.472;-0.124;0.915;-0.383;0.012;0.075;-0.065;-0.221;-0.817;-0.533#
	Phone-ACC;1382195639654;2013/10/19;11:13:59.662;9.8;1.9;3.0;157.0;-10.5;73.1;23.9;-3.8;10.8;-0.0;0.1;0.0;0.5;0.1;0.2;9.2;1.8;2.8;-0.6;-0.2;-0.8#
	RESTART;2160239;10255;0#
	Y0;Left_H;00:06:66:09:C6:BA;1382195639816;2013/10/19;11:13:59.816;-0.135;0.919;-0.371;-0.007;-0.034;0.075;0.716;-0.590;-0.374#
	Y1;Head;00:06:66:09:C6:AA;1382195639816;2013/10/19;11:13:59.816;-0.882;0.461;-0.093;-0.061;0.023;0.005;-0.110;-0.876;-0.469#
	Y2;Foot;00:06:66:0A:30:C8;1382195639816;2013/10/19;11:13:59.816;-0.443;0.662;-0.605;-0.025;0.105;0.125;-0.636;-0.034;-0.771#
	Y3;Right_H;00:06:66:09:46:62;1382195639816;2013/10/19;11:13:59.816;-0.120;0.918;-0.378;0.022;-0.013;0.006;-0.212;-0.820;-0.532#
	Phone-ACC;1382195640092;2013/10/19;11:14:00.96;8.9;2.1;2.6;166.7;-5.5;67.6;24.2;-4.0;11.6;0.0;0.0;0.0;-0.1;1.1;-1.1;9.0;0.9;3.7;-0.5;-0.1;-0.8#
	RESTART;2160663;424;1#
	:
	:

(4) Labeled Output Files (e.g., [labeled]sb02_MYEI.txt, [labeled]sb02_MYEI.txt, [labeled]sb03_MYEI.txt,,,)

STEP 1: As all data in the 'Session timetable' are stored in corresponding variables,

	[0]		Subject_ID			1								(extract from column [0]SubjectID)
	[1]		Session_ID			PRESHAKING (or BASELINE, POSTSHAKING, OTHERS)	(based on start- and end-times of each of the sessions)

	[2]		Annotated_PI_Event	TRUE or FALSE
	[3]		PI_Datestamp		10/19/2013						(extract from column [2]Datestamps)
	[4]		PI_Timestamp		12:14:40						(compose by using column [6]hh, [7]mm, and [8]ss) 
	[5]		Event_detected		SOCIAL							(extract from column [9]Event_detected)
	[6]		Specifics			RESP_PASSENGER					(extract from column [10]Specifics)
	[7]		Hand_status			BOTH_HANDS_ON_WHEEL				(extract from column [11]Hand_status)
	[8]		Opposite_traffic	TIGHTLY							(extract from column [12]Opposite_traffic)
	[9]		Left_lane			TIGHTLY							(extract from column [13]Left_lane)
	[10]	My_lane				TIGHTLY							(extract from column [14]My_lane)
	[11]	Right_lane			NONE							(extract from column [15]Right_lane)

	[12]	Sensor_Datestamp	2013/10/19
	[13]	Sensor_Timestamp	11:13:59.472

	[14]	Gravity_x_LEFT		-0.115
	[15]	Gravity_y_LEFT		0.917
	[16]	Gravity_z_LEFT		-0.382
	[17]	Gyro_x_LEFT			0.022
	[18]	Gyro_y_LEFT			0.051
	[19]	Gyro_z_LEFT			-0.044
	[20]	North_x_LEFT		0.714
	[21]	North_y_LEFT		-0.590
	[22]	North_z_LEFT		-0.376

	[23]	Gravity_x_RIGHT		-0.115
	[24]	Gravity_y_RIGHT		0.917
	[25]	Gravity_z_RIGHT		-0.382
	[26]	Gyro_x_RIGHT		0.022
	[27]	Gyro_y_RIGHT		0.051
	[28]	Gyro_z_RIGHT		-0.044
	[29]	North_x_RIGHT		0.714
	[30]	North_y_RIGHT		-0.590
	[31]	North_z_RIGHT		-0.376

	[32]	Gravity_x_HEAD		-0.115
	[33]	Gravity_y_HEAD		0.917
	[34]	Gravity_z_HEAD		-0.382
	[35]	Gyro_x_HEAD			0.022
	[36]	Gyro_y_HEAD			0.051
	[37]	Gyro_z_HEAD			-0.044
	[38]	North_x_HEAD		0.714
	[39]	North_y_HEAD		-0.590
	[40]	North_z_HEAD		-0.376

	[41]	Gravity_x_FOOT		-0.115
	[42]	Gravity_y_FOOT		0.917
	[43]	Gravity_z_FOOT		-0.382
	[44]	Gyro_x_FOOT			0.022
	[45]	Gyro_y_FOOT			0.051
	[46]	Gyro_z_FOOT			-0.044
	[47]	North_x_FOOT		0.714
	[48]	North_y_FOOT		-0.590
	[49]	North_z_FOOT		-0.376

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

#define N_SUBJECTS 1 //3
#define N_TIMETABLE_SUBJECTS 1 //3

#define FNAME_LENGTH 50
#define DATA_SIZE 100

#define N_COL_EXP_SESSION_FILE 24
#define N_COL_DRV_ACTIVITY_FILE 16
#define N_COL_RAW_SENSOR_FILE 15

#define NONE_PI 0
#define PI_INTERVAL 1
#define PRE_PI_INTERVAL 2
#define POST_PI_INTERVAL 3

using namespace std; // must exist for file open

ifstream input_session_timetable_file;					// session_time_table.csv (delimeter -> , ) 
ifstream input_driver_activity_files[N_SUBJECTS];		// sb01_activities.csv, sb02_activities.csv, sb03_activities.csv, ... (delimeter -> , ) 

ifstream input_raw_sensor_data_files[N_SUBJECTS];		// sb02_MYEI.txt, sb02_MYEI.txt, sb03_MYEI.txt, ...  (delimiter -> ; )

ofstream output_labeled_sensor_data_files[N_SUBJECTS];	// [labeled]sb02_MYEI.csv, [labeled]sb02_MYEI.csv, [labeled]sb03_MYEI.csv
ofstream output_labeled_body_motion_files[N_SUBJECTS];			// [BM_label]sb02_MYEI.csv, [BM_label]sb02_MYEI.csv, [BM_label]sb03_MYEI.csv

char my_sensor_timestamp[DATA_SIZE]="My_Null";

static bool compare_timestamps(char sensor_datestamp[], char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[]);
static int detect_PI_moments(char sensor_timestamp[], char PI_timestamp[]);

bool compare_timestamps(char sensor_datestamp[], char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[])
{
	char year[2][10], month[2][10], date[2][10], hour[3][10], minute[3][10], second[3][10], millisecond[3][10];
	char zero[10]={"0"};
	
	int sensor=0, session_start=1, session_end=2;
	int sensor_digit=0, session_start_digit=0, session_end_digit=0;
		
	// sensor datestamp - "2013/12/19"
	// sensor timestamp - "13:14:39.584"

	// session exp_type - "Separate" or "Both"
	// session datestamp - "11/1/2013"
	// session timestamp - "11:27:30" or "None"
	
	// fyi - formats in Gimlets "29/09/2011 11:28:30.000000"

	char * pch;
	
	char temp[5][100];

	strcpy(temp[0], sensor_datestamp);
	strcpy(temp[1], sensor_timestamp);

	strcpy(temp[2], session_datestamp);
	strcpy(temp[3], session_start_timestamp);
	strcpy(temp[4], session_end_timestamp);
	
	if((strcmp(session_start_timestamp, "None")==0)&&(strcmp(session_end_timestamp, "None")==0)) // 0 -> equal
	{
		return false;
	}else
	{
		// Decompose: Sensor Time
		pch = strtok (temp[0], "/");	strcpy(year[sensor], pch);
		pch = strtok (NULL, "/");		strcpy(month[sensor], pch);
		pch = strtok (NULL, "\n");		strcpy(date[sensor], pch);

		pch = strtok (temp[1], ":");	strcpy(hour[sensor], pch);
		pch = strtok (NULL, ":");		strcpy(minute[sensor], pch);
		pch = strtok (NULL, ".");		strcpy(second[sensor], pch);
		pch = strtok (NULL, "\n");		strcpy(millisecond[sensor], pch);
		//if(atoi(millisecond[0])<100){strcat(millisecond[sensor],"0");}
		if(atoi(millisecond[sensor])<100){
			strcat(zero, millisecond[sensor]);
			sprintf(my_sensor_timestamp,"%s:%s:%s.%s",hour[sensor], minute[sensor], second[sensor], zero);
			//printf("mymy_s_tstamp (comp_sessiontime): %s \n)", my_sensor_timestamp);
			//cin.get();
		}else{
			strcpy(my_sensor_timestamp, sensor_timestamp);
		}// doing this because I realized Jin's app append 'milisecond' data, just after the period --> it means if the milisec data is 53 (i.e less than 100) it produces xx:xx:xx.53 althought it should be xx.xx:xx.053
		

		// Decompose: Session Time (Start)
		pch = strtok (temp[2], "/");	strcpy(month[session_start], pch);
		pch = strtok (NULL, "/");		strcpy(date[session_start], pch);
		pch = strtok (NULL, "\n");		strcpy(year[session_start], pch);
	
		pch = strtok (temp[3], ":");	strcpy(hour[session_start], pch);
		pch = strtok (NULL, ":");		strcpy(minute[session_start], pch);
		pch = strtok (NULL, "\n");		strcpy(second[session_start], pch);
										strcpy(millisecond[session_start], "000");
							
		// Decompose: Session Time (End)
		pch = strtok (temp[4], ":");	strcpy(hour[session_end], pch);
		pch = strtok (NULL, ":");		strcpy(minute[session_end], pch);
		pch = strtok (NULL, "\n");		strcpy(second[session_end], pch);
										strcpy(millisecond[session_end], "000");

		// the following is milisec level, but we're fine to use sec level in this comparison
		/*
		//sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(millisecond[sensor]);
		sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(zero);
		session_start_digit =((atoi(hour[session_start])*60+atoi(minute[session_start]))*60+atoi(second[session_start]))*1000+atoi(millisecond[session_start]);
		session_end_digit =((atoi(hour[session_end])*60+atoi(minute[session_end]))*60+atoi(second[session_end]))*1000+atoi(millisecond[session_end]);
		*/
										
		sensor_digit = (atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]);
		session_start_digit =(atoi(hour[session_start])*60+atoi(minute[session_start]))*60+atoi(second[session_start]);
		session_end_digit =(atoi(hour[session_end])*60+atoi(minute[session_end]))*60+atoi(second[session_end]);

		// NULL, SHAKING, BASELINE1, SHAKING (or NULL), SHAKING (or NULL), BASELINE2, SHAKING

		//if((sensor_digit >= session_start_digit)&&(sensor_digit <= session_end_digit))  // milisec level, but it looks like having issue with in the end of BASELIE2 of sb14
		if((sensor_digit >= session_start_digit)&&(sensor_digit < session_end_digit))	  // new sec level
		{		
			//printf("TRUE: %d <= %d <= %d \n ", session_start_digit, sensor_digit, session_end_digit);

			//printf("sensor_datestamp=%s, sensor_timestamp=%s \n",sensor_datestamp, sensor_timestamp);
			//printf("session_datestamp=%s, session__start_timestamp=%s \n",session_datestamp, session_start_timestamp);
			//printf("session_datestamp=%s, session_end_timestamp=%s \n",session_datestamp, session_end_timestamp);

			//printf("sensor: %s-%s-%s %s:%s:%s.%s \n", year[sensor], month[sensor], date[sensor], hour[sensor], minute[sensor], second[sensor], millisecond[sensor]);
			//printf("session_start: %s-%s-%s %s:%s:%s.%s \n", year[session_start], month[session_start], date[session_start], hour[session_start], minute[session_start], second[session_start], millisecond[session_start]);
			//printf("session_end: %s-%s-%s %s:%s:%s.%s \n", year[session_start], month[session_start], date[session_start], hour[session_end], minute[session_end], second[session_end], millisecond[session_end]);

			//cin.get();

			return true;
		}else
		{
			return false;
		}	
	}
}

int detect_PI_moments(char sensor_timestamp[], char driver_PI_timestamp[])
{
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

	//printf("in detect_PI_moments 1\n");
	//printf("sensor=%s, driverPI=%s, sensor_digit=%s, driver_pi_digit=%s",sensor_timestamp, driver_PI_timestamp, sensor_digit, driver_pi_digit);
	//cin.get();

	// Decompose: Sensor Time
	pch = strtok (temp[sensor], ":");	strcpy(hour[sensor], pch);
	pch = strtok (NULL, ":");			strcpy(minute[sensor], pch);
	pch = strtok (NULL, ".");			strcpy(second[sensor], pch);
	pch = strtok (NULL, "\n");			strcpy(millisecond[sensor], pch);
	//if(atoi(millisecond[sensor])<100){strcat(millisecond[sensor],"0");}
	if(atoi(millisecond[0])<100){
			strcat(zero, millisecond[sensor]);
			sprintf(my_sensor_timestamp,"%s:%s:%s.%s",hour[sensor], minute[sensor], second[sensor], zero);
			//printf("mymy_s_tstamp (detectPI): %s \n", my_sensor_timestamp);
			//cin.get();
		}else{
			strcpy(my_sensor_timestamp, sensor_timestamp);
		} // doing this because I realized Jin's app append 'milisecond' data, just after the period --> it means if the milisec data is 53 (i.e less than 100) it produces xx:xx:xx.53 althought it should be xx.xx:xx.053
		
	//printf("hour=%s, minute=%s, second=%s, milis=%s \n", hour[sensor], minute[sensor], second[sensor], millisecond[sensor]);

	// Decompose: driver PI Time	
	pch = strtok (temp[driver_pi], ":");	strcpy(hour[driver_pi], pch);
	pch = strtok (NULL, ":");				strcpy(minute[driver_pi], pch);
	pch = strtok (NULL, "\n");				strcpy(second[driver_pi], pch);
											strcpy(millisecond[driver_pi], "000");
											
	//printf("hour=%s, minute=%s, second=%s, milis=%s \n", hour[driver_pi], minute[driver_pi], second[driver_pi], millisecond[driver_pi]);

	//sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(millisecond[sensor]);
	sensor_digit = ((atoi(hour[sensor])*60+atoi(minute[sensor]))*60+atoi(second[sensor]))*1000+atoi(zero);
	driver_pi_digit =((atoi(hour[driver_pi])*60+atoi(minute[driver_pi]))*60+atoi(second[driver_pi]))*1000+atoi(millisecond[driver_pi]);

	//printf("in detect_PI_moments 2\n");
	//printf("sensor=%s, driverPI=%s, sensor_digit=%d, driver_pi_digit=%d",sensor_timestamp, driver_PI_timestamp, sensor_digit, driver_pi_digit);
	//cin.get();

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
	char fname_raw_sensor_data_files[N_SUBJECTS][FNAME_LENGTH];
	char fname_labeled_sensor_data_files[N_SUBJECTS][FNAME_LENGTH];
	char fname_labeled_body_motion_files[N_SUBJECTS][FNAME_LENGTH];

	//sprintf(fname_session_timetable,"session_time_table_new.csv");
	//sprintf(fname_session_timetable,"session_time_table_new_24to26.csv");
	
	//sprintf(fname_session_timetable,"session_time_table_26.csv");

	char subject_id_s[5]="NULL";
	char first_subject_id[5]="11";//"13";

	// FOR SEPARATE GROUP - from here
	/*sb05*///char separate_driving_date[2][50]={"2013/11/01","2013/12/23"}; // sb05 - {part1, part2} in MYEI file date format
	/*sb11*/char separate_driving_date[2][50]={"2013/11/18","2014/01/08"}; // sb11 - {part1, part2} in MYEI file date format
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

		sprintf(fname_driver_activity[i],"sb%s_activities.csv",subject_id_s);		//Input
		sprintf(fname_raw_sensor_data_files[i],"sb%s_MYEI.txt",subject_id_s);		//Input
		//sprintf(fname_labeled_sensor_data_files[i],"[labeled]sb%s_MYEI_1s.csv",subject_id_s);	//Output
		sprintf(fname_labeled_sensor_data_files[i],"[parsed]sb%s_MYEI.csv",subject_id_s);	//Output
		//sprintf(fname_labeled_body_motion_files[i],"[BM_label]sb%s_MYEI.csv",subject_id_s);		//Output

		input_driver_activity_files[i].open(fname_driver_activity[i]);
		if(!input_driver_activity_files[i]) {cerr<< fname_driver_activity[i] << "<-- Unable to open this Driver Activity file. \n";}
		else{printf("%s is opened\n", fname_driver_activity[i]);}

		input_raw_sensor_data_files[i].open(fname_raw_sensor_data_files[i]);
		if(!input_raw_sensor_data_files[i]) {cerr<< fname_raw_sensor_data_files[i] << "<-- Unable to open this Raw Sensor data file. \n";}
		else{printf("%s is opened\n", fname_raw_sensor_data_files[i]);}

		output_labeled_sensor_data_files[i].open(fname_labeled_sensor_data_files[i]);
		//if(!output_labeled_sensor_data_files[i]) {cerr<< fname_labeled_sensor_data_files[i] << "<-- Unable to create the [Labeled] Sensor data file. \n";}
		if(!output_labeled_sensor_data_files[i]) {cerr<< fname_labeled_sensor_data_files[i] << "<-- Unable to create the [Parsed] Sensor data file. \n";}
		else{printf("%s is opened\n", fname_labeled_sensor_data_files[i]);}

		/*output_labeled_body_motion_files[i].open(fname_labeled_body_motion_files[i]);
		if(!output_labeled_body_motion_files[i]) {cerr<< fname_labeled_body_motion_files[i] << "<-- Unable to create the [BM_label] body motion file. \n";}
		else{printf("%s is opened\n", fname_labeled_body_motion_files[i]);}*/

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
		
	// Raw sensor files - Open and read
	char sensor_id[DATA_SIZE]="NULL";	//[0]		Sensor_ID		Y0
	char body_worn[DATA_SIZE]="NULL";	//[1]		Body			Left_H
	char device_id[DATA_SIZE]="NULL";	//[2]		Device_ID		00:06:66:09:C6:BA

	char unix_time[DATA_SIZE]="NULL";	//[3]		UNIX_time		1382195639472
	char datestamp[DATA_SIZE]="NULL";	//[4]		Datestamp		2013/10/19
	char timestamp[DATA_SIZE]="NULL";	//[5]		Timestamp		11:13:59.472
		char pre_timestamp[DATA_SIZE]="NULL";

	char gravity_x[DATA_SIZE]="NULL";	//[6]		Gravity_x		-0.115
	char gravity_y[DATA_SIZE]="NULL";	//[7]		Gravity_y		0.917
	char gravity_z[DATA_SIZE]="NULL";	//[8]		Gravity_z		-0.382

	char gyro_x[DATA_SIZE]="NULL";		//[9]		Gyro_x			0.022
	char gyro_y[DATA_SIZE]="NULL";		//[10]		Gyro_y			0.051
	char gyro_z[DATA_SIZE]="NULL";		//[11]		Gyro_z			-0.044

	char north_x[DATA_SIZE]="NULL";		//[12]		North_x			0.714
	char north_y[DATA_SIZE]="NULL";		//[13]		North_y			-0.590
	char north_z[DATA_SIZE]="NULL";		//[14]		North_z			-0.376

	char scanned_session_string[256];
	char scanned_sensor_stream[256];
	char scanned_PI_activity_string[256];

	//	New Output 1 - Averages in 1-sec unit time-windows
	int sensor_id_scanned=0; // whether Y0, Y1, Y2, or Y3
	bool cycle_completed=false;

	int count_samples=0;

	int hh_timestamp=0;
	int mm_timestamp=0;
	int ss_timestamp=0; int pre_ss_timestamp=0;
	int ls_timestamp=0;

	double total_gravity_x_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_gravity_y_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_gravity_z_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};

	double total_gyro_x_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_gyro_y_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_gyro_z_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};

	double total_north_x_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_north_y_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};
	double total_north_z_per_unit_time[4]={0.0, 0.0, 0.0, 0.0};

	 // feature extractions from New Output 1
	double integrated_gravity_deviation[4]={0.0, 0.0, 0.0, 0.0};
	double baseline_gravity_x[4]={0.0, 0.0, 0.0, 0.0};
	double baseline_gravity_y[4]={0.0, 0.0, 0.0, 0.0};
	double baseline_gravity_z[4]={0.0, 0.0, 0.0, 0.0};
	double temp_baseline_gravity_x[4]={0.0, 0.0, 0.0, 0.0};
	double temp_baseline_gravity_y[4]={0.0, 0.0, 0.0, 0.0};
	double temp_baseline_gravity_z[4]={0.0, 0.0, 0.0, 0.0};
	double integrated_gyro[4]={0.0, 0.0, 0.0, 0.0};
	
	double gravity_level_threshold[2]={0.0, 0.0};
	double gyro_level_threshold[3]={0.0, 0.0, 0.0};

	double pre_integrated_gravity_deviation[4]={0.0, 0.0, 0.0, 0.0};
	double pre_integrated_gyro[4]={0.0, 0.0, 0.0, 0.0};

	char pre_trend_gravity_feature[4][30]={"Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec"};
	char pre_trend_gyro_feature[4][30]={"Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec"};
	int gravity_elapsed_time[4]={0, 0, 0, 0};
	int gyro_elapsed_time[4]={0, 0, 0, 0};

	char last_Y_id[15]="Y_null";

	

	// Output 2 - Body Motion files	
	double pre_unixtime=0.0;

	char UTC_timestamp[30]="NULL"; char next_UTC_timestamp[30]="NULL";
	int UTC_yyyy=0;
	int UTC_mm=0;
	int UTC_dd=0;

	double first_d_gravity[4]={0.0, 0.0, 0.0, 0.0};
	double first_d_gyro[4]={0.0, 0.0, 0.0, 0.0};
	double first_d_north[4]={0.0, 0.0, 0.0, 0.0};

	double SMA1_1d_gravity[4]={0.0, 0.0, 0.0, 0.0};
	double SMA1_1d_gyro[4]={0.0, 0.0, 0.0, 0.0};
	double SMA1_1d_north[4]={0.0, 0.0, 0.0, 0.0};

	double SMA2_1d_gravity[4]={0.0, 0.0, 0.0, 0.0};
	double SMA2_1d_gyro[4]={0.0, 0.0, 0.0, 0.0};
	double SMA2_1d_north[4]={0.0, 0.0, 0.0, 0.0};

	int	motion_changes_gravity[4] = {0, 0, 0, 0};
	int	motion_changes_gyro[4] = {0, 0, 0, 0};
	int	motion_changes_north[4] = {0, 0, 0, 0};

	double threshold=0;

	double near_SMA1_1d_gravity[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};	// [1][k] - two rows before, [0][k] - one row before
	double near_SMA1_1d_gyro[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};
	double near_SMA1_1d_north[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};

	double pre_1d_gravity[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};	// [1][k] - two rows before, [0][k] - one row before
	double pre_1d_gyro[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};
	double pre_1d_north[2][4]={{0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0, 0.0}};

	double pre_grativty_x[4]={0.0, 0.0, 0.0, 0.0};
	double pre_grativty_y[4]={0.0, 0.0, 0.0, 0.0};
	double pre_grativty_z[4]={0.0, 0.0, 0.0, 0.0};

	double pre_gyro_x[4]={0.0, 0.0, 0.0, 0.0};
	double pre_gyro_y[4]={0.0, 0.0, 0.0, 0.0};
	double pre_gyro_z[4]={0.0, 0.0, 0.0, 0.0};

	double pre_north_x[4]={0.0, 0.0, 0.0, 0.0};
	double pre_north_y[4]={0.0, 0.0, 0.0, 0.0};
	double pre_north_z[4]={0.0, 0.0, 0.0, 0.0};

	//Scan Session Time Table file
	input_session_timetable_file >> scanned_session_string;	// read the 1st row - 'title' row (will just skip)
	//printf("1st row - scanned_session_string: %s \n", scanned_session_string);

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

		printf("i=%d : %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", i, STT_subject_id[i], STT_exp_type[i], 
		p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i], p1_baseline_startT[i], p1_baseline_endT[i], p1_driving_startT[i], p1_driving_sigstopT[i], p1_driving_restartT[i], p1_driving_endT[i], p1_postshaking_startT[i], p1_postshaking_endT[i],
		p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i], p2_baseline_startT[i], p2_baseline_endT[i], p2_driving_startT[i], p2_driving_sigstopT[i], p2_driving_restartT[i], p2_driving_endT[i], p2_postshaking_startT[i], p2_postshaking_endT[i]);

		i++;
		//cin.get();
	}
	
	// Printing output files while reading raw sensor data stream files.

	for(i=0; i<N_SUBJECTS; i++)
	{		
		// Write in the output file 1
		/*
		output_labeled_sensor_data_files[i] <<	"subject" << "," << "session" << "," << "timestamp" 
			<<	"," << "gravity_x_LH" << "," << "gravity_y_LH" << "," << "gravity_z_LH" << "," << "gyro_x_LH" << "," << "gyro_y_LH" << "," << "gyro_z_LH" << "," << "north_x_LH" << "," << "north_y_LH" << "," << "north_z_LH"
			<<	"," << "gravity_x_head" << "," << "gravity_y_head" << "," << "gravity_z_head" << "," << "gyro_x_head" << "," << "gyro_y_head" << "," << "gyro_z_head" << "," << "north_x_head" << "," << "north_y_head" << "," << "north_z_head"
			<<	"," << "gravity_x_foot" << "," << "gravity_y_foot" << "," << "gravity_z_foot" << "," << "gyro_x_foot" << "," << "gyro_y_foot" << "," << "gyro_z_foot" << "," << "north_x_foot" << "," << "north_y_foot" << "," << "north_z_foot"
			<<	"," << "gravity_x_RH" << "," << "gravity_y_RH" << "," << "gravity_z_RH" << "," << "gyro_x_RH" << "," << "gyro_y_RH" << "," << "gyro_z_RH" << "," << "north_x_RH" << "," << "north_y_RH" << "," << "north_z_RH" 
			<<	"," << "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
			<< "\n";*/
		
		// Write in the New output file 1
		/*
		output_labeled_sensor_data_files[i] <<	"subject" << "," << "session" << "," << "timestamp" 
			<< "," << "gravity_x_LH" << "," << "gravity_y_LH" << "," << "gravity_z_LH" << "," << "gravity_feature_LH" << "," << "gravity_feature_LH_1std" << "," << "gravity_trend_LH" << "," << "gravity_trend_elapsed_time_LH"
			<< "," << "baseline_gravity_x_LH" << "," << "baseline_gravity_y_LH" << "," << "baseline_gravity_z_LH"
			<< "," << "gyro_x_LH" << "," << "gyro_y_LH" << "," << "gyro_z_LH"  << "," << "gyro_feature_LH" << "," << "gyro_feature_LH_1std"  << "," << "gyro_trend_LH" << "," << "gyro_trend_elapsed_time_LH"
			<< "," << "north_x_LH" << "," << "north_y_LH" << "," << "north_z_LH"
			<<	"," << "gravity_x_head" << "," << "gravity_y_head" << "," << "gravity_z_head" << "," << "gravity_feature_head" << "," << "gravity_feature_head_1std" << "," << "gravity_trend_head" << "," << "gravity_trend_elapsed_time_head"
			<< "," << "baseline_gravity_x_head" << "," << "baseline_gravity_y_head" << "," << "baseline_gravity_z_head" 
			<< "," << "gyro_x_head" << "," << "gyro_y_head" << "," << "gyro_z_head"  << "," << "gyro_feature_head"  << "," << "gyro_feature_head_1std"  << "," << "gyro_trend_head" << "," << "gyro_trend_elapsed_time_head" 
			<< "," << "north_x_head" << "," << "north_y_head" << "," << "north_z_head"
			<<	"," << "gravity_x_foot" << "," << "gravity_y_foot" << "," << "gravity_z_foot" << "," << "gravity_feature_foot" << "," << "gravity_feature_foot_1std"  << "," << "gravity_trend_foot" << "," << "gravity_trend_elapsed_time_foot"
			<< "," << "baseline_gravity_x_foot" << "," << "baseline_gravity_y_foot" << "," << "baseline_gravity_z_foot" 
			<< "," << "gyro_x_foot" << "," << "gyro_y_foot" << "," << "gyro_z_foot"  << "," << "gyro_feature_foot" << "," << "gyro_feature_foot_1std"  << "," << "gyro_trend_foot" << "," << "gyro_trend_elapsed_time_foot" 
			<< "," << "north_x_foot" << "," << "north_y_foot" << "," << "north_z_foot"
			<<	"," << "gravity_x_RH" << "," << "gravity_y_RH" << "," << "gravity_z_RH" << "," << "gravity_feature_RH" << "," << "gravity_feature_RH_1std"  << "," << "gravity_trend_RH" << "," << "gravity_trend_elapsed_time_RH"
			<< "," << "baseline_gravity_x_RH" << "," << "baseline_gravity_y_RH" << "," << "baseline_gravity_z_RH" 
			<< "," << "gyro_x_RH" << "," << "gyro_y_RH" << "," << "gyro_z_RH"  << "," << "gyro_feature_RH" << "," << "gyro_feature_RH_1std"  << "," << "gyro_trend_RH" << "," << "gyro_trend_elapsed_time_RH" 
			<< "," << "north_x_RH" << "," << "north_y_RH" << "," << "north_z_RH" 
			<<	"," << "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane";
			//<< "\n";
			*/
		output_labeled_sensor_data_files[i] <<	"subject" << "," << "session" << "," << "timestamp" 
			<< "," << "gravity_x_LH" << "," << "gravity_y_LH" << "," << "gravity_z_LH" << "," << "gravity_feature_LH" << "," << "gravity_feature_level_LH" << "," << "gravity_feature_LH_1std" << "," << "gravity_trend_LH" << "," << "gravity_trend_elapsed_time_LH"
			<< "," << "baseline_gravity_x_LH" << "," << "baseline_gravity_y_LH" << "," << "baseline_gravity_z_LH"
			<< "," << "gyro_x_LH" << "," << "gyro_y_LH" << "," << "gyro_z_LH"  << "," << "gyro_feature_LH" << "," << "gyro_feature_level_LH" << "," << "gyro_feature_LH_1std"  << "," << "gyro_trend_LH" << "," << "gyro_trend_elapsed_time_LH"
			<< "," << "north_x_LH" << "," << "north_y_LH" << "," << "north_z_LH"
			<<	"," << "gravity_x_head" << "," << "gravity_y_head" << "," << "gravity_z_head" << "," << "gravity_feature_head" << "," << "gravity_feature_level_head" << "," << "gravity_feature_head_1std" << "," << "gravity_trend_head" << "," << "gravity_trend_elapsed_time_head"
			<< "," << "baseline_gravity_x_head" << "," << "baseline_gravity_y_head" << "," << "baseline_gravity_z_head" 
			<< "," << "gyro_x_head" << "," << "gyro_y_head" << "," << "gyro_z_head"  << "," << "gyro_feature_head"  << "," << "gyro_feature_level_head" << "," << "gyro_feature_head_1std"  << "," << "gyro_trend_head" << "," << "gyro_trend_elapsed_time_head" 
			<< "," << "north_x_head" << "," << "north_y_head" << "," << "north_z_head"
			<<	"," << "gravity_x_foot" << "," << "gravity_y_foot" << "," << "gravity_z_foot" << "," << "gravity_feature_foot" << "," << "gravity_feature_level_foot" << "," << "gravity_feature_foot_1std"  << "," << "gravity_trend_foot" << "," << "gravity_trend_elapsed_time_foot"
			<< "," << "baseline_gravity_x_foot" << "," << "baseline_gravity_y_foot" << "," << "baseline_gravity_z_foot" 
			<< "," << "gyro_x_foot" << "," << "gyro_y_foot" << "," << "gyro_z_foot"  << "," << "gyro_feature_foot" << "," << "gyro_feature_level_foot" << "," << "gyro_feature_foot_1std"  << "," << "gyro_trend_foot" << "," << "gyro_trend_elapsed_time_foot" 
			<< "," << "north_x_foot" << "," << "north_y_foot" << "," << "north_z_foot"
			<<	"," << "gravity_x_RH" << "," << "gravity_y_RH" << "," << "gravity_z_RH" << "," << "gravity_feature_RH" << "," << "gravity_feature_level_RH" << "," << "gravity_feature_RH_1std"  << "," << "gravity_trend_RH" << "," << "gravity_trend_elapsed_time_RH"
			<< "," << "baseline_gravity_x_RH" << "," << "baseline_gravity_y_RH" << "," << "baseline_gravity_z_RH" 
			<< "," << "gyro_x_RH" << "," << "gyro_y_RH" << "," << "gyro_z_RH"  << "," << "gyro_feature_RH" << "," << "gyro_feature_level_RH" << "," << "gyro_feature_RH_1std"  << "," << "gyro_trend_RH" << "," << "gyro_trend_elapsed_time_RH" 
			<< "," << "north_x_RH" << "," << "north_y_RH" << "," << "north_z_RH"; 
			//<<	"," << "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane";
			//<< "\n";

		// Write in the output file 2
		/*output_labeled_body_motion_files[i] <<	"subject" << "," << "session" << "," << "UTC_timestamp" << "," << "unix_timestamp"
			<<	"," << "1std_gravity_LH_sqrt_dxyz_dt" << "," << "1std_gyro_LH_sqrt_dxyz_dt" << "," << "1std_north_LH_sqrt_dxyz_dt"
			<<	"," << "1std_gravity_head_sqrt_dxyz_dt" << "," << "1std_gyro_head_sqrt_dxyz_dt" << "," << "1std_north_head_sqrt_dxyz_dt"
			<<	"," << "1std_gravity_foot_sqrt_dxyz_dt" << "," << "1std_gyro_foot_sqrt_dxyz_dt" << "," << "1std_north_foot_sqrt_dxyz_dt"
			<<	"," << "1std_gravity_RH_sqrt_dxyz_dt" << "," << "1std_gyro_RH_sqrt_dxyz_dt" << "," << "1std_north_RH_sqrt_dxyz_dt" 
			<<	"," << "SMI1_gravity_LH" << "," << "SMI1_gyro_LH" << "," << "SMI1_north_LH"
			<<	"," << "SMI1_gravity_head" << "," << "SMI1_gyro_head" << "," << "SMI1_north_head"
			<<	"," << "SMI1_gravity_foot" << "," << "SMI1_gyro_foot" << "," << "SMI1_north_foot"
			<<	"," << "SMI1_gravity_RH" << "," << "SMI1_gyro_RH" << "," << "SMI1_north_RH" 
			<<	"," << "SMI2_gravity_LH" << "," << "SMI2_gyro_LH" << "," << "SMI2_north_LH"
			<<	"," << "SMI2_gravity_head" << "," << "SMI2_gyro_head" << "," << "SMI2_north_head"
			<<	"," << "SMI2_gravity_foot" << "," << "SMI2_gyro_foot" << "," << "SMI2_north_foot"
			<<	"," << "SMI2_gravity_RH" << "," << "SMI2_gyro_RH" << "," << "SMI2_north_RH" 
			<<	"," << "MotionC_gravity_LH" << "," << "MotionC_gyro_LH" << "," << "MotionC_north_LH"
			<<	"," << "MotionC_gravity_head" << "," << "MotionC_gyro_head" << "," << "MotionC_north_head"
			<<	"," << "MotionC_gravity_foot" << "," << "MotionC_gyro_foot" << "," << "MotionC_north_foot"
			<<	"," << "MotionC_gravity_RH" << "," << "MotionC_gyro_RH" << "," << "MotionC_north_RH"
			<< "\n";*/
		
		bool Used=true;
		char driver_PI_timestamp [50] = "Null"; // reformat just for the sub-function 'detect_PI_moments'
		char driver_PI_log[500]="Null,Null,Null,Null,Null,Null,Null";
				
		input_driver_activity_files[i] >> scanned_PI_activity_string; // driver activity skip the 1st row	//printf("PI_string: %s \n", scanned_PI_activity_string);	//cin.get();

		int output_row_num=0; // to determine the denominator for SMA

		char session_id[15] = "NULL_SESSION";
		char pre_session_id[15] = "NULL_SESSION";

		while(!input_raw_sensor_data_files[i].eof()) 
		{			
			// Sensor Data Streams - Start to read a dataset (Note: there is no 'title' row.)
			input_raw_sensor_data_files[i] >> scanned_sensor_stream; 

			pch = strtok (scanned_sensor_stream, ";");	// proceed until the delimter ';'
			if(pch == NULL) break;						// in order to take care of the last 'NULL' row - if there is no ';' to search, 'break' - Note that this checking must come before 'strcpy'

			strcpy(sensor_id, pch);	//printf("check 1st pch (sensor_id) = %s\n", sensor_id);
	
			// Sensor Data Streams - Read and store 
			if((strcmp(pch,"Y0")==0)|(strcmp(pch,"Y1")==0)|(strcmp(pch,"Y2")==0)|(strcmp(pch,"Y3")==0)) // checking the 1st data before the 1st delimiter of the row
			{
				pch = strtok (NULL, ";");	strcpy(body_worn, pch);
				pch = strtok (NULL, ";");	strcpy(device_id, pch);
		
				pch = strtok (NULL, ";");	strcpy(unix_time, pch);
				pch = strtok (NULL, ";");	strcpy(datestamp, pch);
				pch = strtok (NULL, ";");	strcpy(timestamp, pch);
		
				pch = strtok (NULL, ";");	strcpy(gravity_x, pch);
				pch = strtok (NULL, ";");	strcpy(gravity_y, pch);
				pch = strtok (NULL, ";");	strcpy(gravity_z, pch);
		
				pch = strtok (NULL, ";");	strcpy(gyro_x, pch);
				pch = strtok (NULL, ";");	strcpy(gyro_y, pch);
				pch = strtok (NULL, ";");	strcpy(gyro_z, pch);
		
				pch = strtok (NULL, ";");	strcpy(north_x, pch);
				pch = strtok (NULL, ";");	strcpy(north_y, pch);
				pch = strtok (NULL, "#");	strcpy(north_z, pch);

				// Only related to the two subjects in the SEPARATE group
				if(strcmp(datestamp,separate_driving_date[0])==0)
				{
					session_num=1;		// part 1 driving of the separate subject (either sb05 or sb11)
				}else if(strcmp(datestamp,separate_driving_date[1])==0)
				{
					session_num=2;		// part 2 driving of the separate subject (either sb05 or sb11)
				}else
				{
					session_num=0;
				}// up to here
				
			}
						
			// Compare timestamps in 'Sensor' and 'Session', and then get 'labeles' for 'session_id' of the output file 		
			//char session_id[15] = "NULL_SESSION";
			//char pre_session_id[15] = "NULL_SESSION";
			//bool b_session_chagned=false;


			/* Original Codes - from here
			if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i])==true)
			{
				strcpy(session_id, "PRE_SHAKING1");							//Part 1 - pre-shaking
			}else
			{
				if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_baseline_startT[i], p1_baseline_endT[i])==true)
				{
					strcpy(session_id, "BASELINE1");						// Part 1- baseline
				}else
				{		
					if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_driving_startT[i], p1_driving_endT[i])==true)
					{
						strcpy(session_id, "DRIVING1");						// Part 1 - driving
					}else
					{
						if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i])==true)
						{
							strcpy(session_id, "PRE_SHAKING2");				// Part 2 - pre-shaking
						}else
						{
							if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_baseline_startT[i], p2_baseline_endT[i])==true)
							{
								strcpy(session_id, "BASELINE2");			// Part 2 - baseline
							}else
							{							
								if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_driving_startT[i], p2_driving_endT[i])==true)
								{
									strcpy(session_id, "DRIVING2");			// Part 2 - driving
								}else
								{
									if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_postshaking_startT[i], p2_postshaking_endT[i])==true)
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
			} Original code - up to here*/


			// Only for the two subjects in the SEPARATE group
			if(session_num==1) // Part 1 driving of the separate subject (either sb05 or sb11)
			{			
				if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i])==true)
				{
					strcpy(session_id, "PRE_SHAKING1");							//Part 1 - pre-shaking
				}else
				{
					if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_baseline_startT[i], p1_baseline_endT[i])==true)
					{
						strcpy(session_id, "BASELINE1");						// Part 1- baseline
					}else
					{		
						if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_driving_startT[i], p1_driving_endT[i])==true)
						{
							strcpy(session_id, "DRIVING1");						// Part 1 - driving
						}else
						{
							if(compare_timestamps(datestamp, timestamp, p1_date[i], p1_postshaking_startT[i], p1_postshaking_endT[i])==true)
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
				if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i])==true)
				{
					strcpy(session_id, "PRE_SHAKING2");				// Part 2 - pre-shaking
				}else
				{
					if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_baseline_startT[i], p2_baseline_endT[i])==true)
					{
						strcpy(session_id, "BASELINE2");			// Part 2 - baseline
					}else
					{							
						if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_driving_startT[i], p2_driving_endT[i])==true)
						{
							strcpy(session_id, "DRIVING2");			// Part 2 - driving
						}else
						{
							if(compare_timestamps(datestamp, timestamp, p2_date[i], p2_postshaking_startT[i], p2_postshaking_endT[i])==true)
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

				//printf("PI_string1: %s \n", scanned_PI_activity_string); 
				//cin.get();
				
				pch = strtok (scanned_PI_activity_string, ",");	
				if(pch != NULL) 	// in order to take care of null data in the last row - Note that this checking must come before 'strcpy'
				{
					strcpy(PI_subject_id, pch); // proceed until the 1st delimter
					//printf("pch = %s, subject_id[i] = %s \n", pch, PI_subject_id);

					pch = strtok (NULL, ",");	strcpy(PI_driving_session, pch);		
		
					pch = strtok (NULL, ",");	strcpy(PI_timestamp, pch);

					// Note: there are 'spaces' in the timstamp colum, so we need to run more 'reading' for that column
					input_driver_activity_files[i] >> scanned_PI_activity_string; //Read until the 2nd space in the timestamp column	//printf("PI_string2: %s \n", scanned_PI_activity_string);
					input_driver_activity_files[i] >> scanned_PI_activity_string; //Read after the 3rd space in the timestamp column	//printf("PI_string3: %s \n", scanned_PI_activity_string); //cin.get();

					strtok (scanned_PI_activity_string, ",");
					pch = strtok (NULL, ",");	strcpy(PI_year, pch);
					pch = strtok (NULL, ",");	strcpy(PI_month, pch);
					pch = strtok (NULL, ",");	strcpy(PI_date, pch);		
					pch = strtok (NULL, ",");	strcpy(PI_hh, pch);
					pch = strtok (NULL, ",");	strcpy(PI_mm, pch);
					pch = strtok (NULL, ",");	strcpy(PI_ss, pch);
		
					pch = strtok (NULL, ",");	strcpy(PI_Event_detected, pch);
						if(strcmp(PI_Event_detected,"HANDS_SHIFT_ON_WHEEL")==0){strcpy(PI_Event_detected,"HAND_SHIFT_ON_WHEEL");}
					pch = strtok (NULL, ",");	strcpy(PI_Specifics, pch);
					pch = strtok (NULL, ",");	strcpy(PI_Hand_status, pch);
						if(strcmp(PI_Hand_status,"HANDS_OFF_WHEEL")==0){strcpy(PI_Hand_status,"HAND_OFF_WHEEL");}
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
				//printf("after detect_PI_moments\n");
				//cin.get();

				sprintf(driver_PI_log,"%s,%s,%s,%s,%s,%s,%s",PI_Event_detected, PI_Specifics, PI_Hand_status, PI_Opposite_traffic, PI_Left_lane, PI_My_lane, PI_Right_lane);
				Used=false;
			}else if(detect_PI_moments(timestamp, driver_PI_timestamp)==POST_PI_INTERVAL)
			{
				// ---- DO FROM HERE -> Labeling works, but pause in the middle in a particular moment
				strcpy(driver_PI_log, "POST_PI,POST_PI,POST_PI,POST_PI,POST_PI,POST_PI,POST_PI");
				Used=true;
			}else
			{
				strcpy(driver_PI_log, "Null,Null,Null,Null,Null,Null,Null");
				//Used=true;
			}
					
			strcpy(timestamp, my_sensor_timestamp); // if millisecond data in 'timestamp' is fewer than 100, added '0' through detect_PI_moments or compare_timestamp functions, 
			//printf("sensor timestamp = %s \n", timestamp);

			
			/// Calculate sqrt((dx^2+dy^2+dz^2)/dt^2) - for 'output_labeled_body_motion_files'	/// Note - string variables - unixt_time, gravity_x~z, gyro_x~z, north_x~z
			
			//cin.get();
			
			if(pre_unixtime>0.0)
			{
				if(strcmp(sensor_id,"Y0")==0) // Left Hand
				{   
					//first_d_gravity[0]=pow(atof(gravity_x)-pre_grativty_x[0],2.0); 
					first_d_gravity[0]=sqrt((pow(atof(gravity_x)-pre_grativty_x[0],2.0)+pow(atof(gravity_y)-pre_grativty_y[0],2.0)+pow(atof(gravity_z)-pre_grativty_z[0],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_gyro[0]=sqrt((pow(atof(gyro_x)-pre_gyro_x[0],2.0)+pow(atof(gyro_y)-pre_gyro_y[0],2.0)+pow(atof(gyro_z)-pre_gyro_z[0],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_north[0]=sqrt((pow(atof(north_x)-pre_north_x[0],2.0)+pow(atof(north_y)-pre_north_y[0],2.0)+pow(atof(north_z)-pre_north_z[0],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					//printf("LH: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[0],first_d_gyro[0],first_d_north[0]);
				}else if(strcmp(sensor_id,"Y1")==0) // Head
				{				
					first_d_gravity[1]=sqrt((pow(atof(gravity_x)-pre_grativty_x[1],2.0)+pow(atof(gravity_y)-pre_grativty_y[1],2.0)+pow(atof(gravity_z)-pre_grativty_z[1],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_gyro[1]=sqrt((pow(atof(gyro_x)-pre_gyro_x[1],2.0)+pow(atof(gyro_y)-pre_gyro_y[1],2.0)+pow(atof(gyro_z)-pre_gyro_z[1],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_north[1]=sqrt((pow(atof(north_x)-pre_north_x[1],2.0)+pow(atof(north_y)-pre_north_y[1],2.0)+pow(atof(north_z)-pre_north_z[1],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					//printf("Head: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[1],first_d_gyro[1],first_d_north[1]);
				}else if(strcmp(sensor_id,"Y2")==0) // Foot
				{
					first_d_gravity[2]=sqrt((pow(atof(gravity_x)-pre_grativty_x[2],2.0)+pow(atof(gravity_y)-pre_grativty_y[2],2.0)+pow(atof(gravity_z)-pre_grativty_z[2],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));;
					first_d_gyro[2]=sqrt((pow(atof(gyro_x)-pre_gyro_x[2],2.0)+pow(atof(gyro_y)-pre_gyro_y[2],2.0)+pow(atof(gyro_z)-pre_gyro_z[2],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_north[2]=sqrt((pow(atof(north_x)-pre_north_x[2],2.0)+pow(atof(north_y)-pre_north_y[2],2.0)+pow(atof(north_z)-pre_north_z[2],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					//printf("Foot: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[2],first_d_gyro[2],first_d_north[2]);
				}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
				{
					first_d_gravity[3]=sqrt((pow(atof(gravity_x)-pre_grativty_x[3],2.0)+pow(atof(gravity_y)-pre_grativty_y[3],2.0)+pow(atof(gravity_z)-pre_grativty_z[3],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_gyro[3]=sqrt((pow(atof(gyro_x)-pre_gyro_x[3],2.0)+pow(atof(gyro_y)-pre_gyro_y[3],2.0)+pow(atof(gyro_z)-pre_gyro_z[3],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));
					first_d_north[3]=sqrt((pow(atof(north_x)-pre_north_x[3],2.0)+pow(atof(north_y)-pre_north_y[3],2.0)+pow(atof(north_z)-pre_north_z[3],2.0))/pow((atof(unix_time)-pre_unixtime)/1000.0,2.0));	
					//printf("RH: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[3],first_d_gyro[3],first_d_north[3]);
				}
			}else
			{
				if(strcmp(sensor_id,"Y0")==0) // Left Hand
				{   
					first_d_gravity[0]=0;
					first_d_gyro[0]=0;
					first_d_north[0]=0;
					//printf("LH: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[0],first_d_gyro[0],first_d_north[0]);
				}else if(strcmp(sensor_id,"Y1")==0) // Head
				{				
					first_d_gravity[1]=0;
					first_d_gyro[1]=0;
					first_d_north[1]=0;
					//printf("Head: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[1],first_d_gyro[1],first_d_north[1]);
				}else if(strcmp(sensor_id,"Y2")==0) // Foot
				{
					first_d_gravity[2]=0;
					first_d_gyro[2]=0;
					first_d_north[2]=0;
					//printf("Foot: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[2],first_d_gyro[2],first_d_north[2]);
				}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
				{
					first_d_gravity[3]=0;
					first_d_gyro[3]=0;
					first_d_north[3]=0;
					//printf("RH: timediff(%f - %f): %f, first_d_gravity: %f, first_d_gyro: %f, first_d_north: %f   \n",atof(unix_time),pre_unixtime,(atof(unix_time)-pre_unixtime)/1000.0,first_d_gravity[3],first_d_gyro[3],first_d_north[3]);
				}
			}
			//cin.get();

			sscanf(datestamp,"%d/%d/%d",&UTC_yyyy,&UTC_mm,&UTC_dd);
			sprintf(UTC_timestamp,"%d-%02d-%02dT%s-05:00",UTC_yyyy,UTC_mm,UTC_dd,timestamp);

			//printf("UTC_timestamp: %s \n", UTC_timestamp);

			// Printing OUTPUT files 1! ---- Original working version

			/*
			if(strcmp(sensor_id,"Y0")==0) // Left Hand
			{
				output_labeled_sensor_data_files[i] <<	STT_subject_id[i] << "," << session_id << "," << datestamp << " " << timestamp << "," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
			}else if((strcmp(sensor_id,"Y1")==0)|(strcmp(sensor_id,"Y2")==0)) // Head / Foot
			{
				output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
			}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
			{		
				output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z << "," << driver_PI_log << endl;
			}*/

			// Printing OUTPUT files 1! ---- 1-sec sampling version

			

			if((strcmp(sensor_id,"Y0")==0)|(strcmp(sensor_id,"Y1")==0)|(strcmp(sensor_id,"Y2")==0)|(strcmp(sensor_id,"Y3")==0))
			{
				
				if((strcmp(session_id,"PRE_SHAKING1")==0)|(strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"DRIVING1")==0)|(strcmp(session_id,"POST_SHAKING1")==0)|
				   (strcmp(session_id,"PRE_SHAKING2")==0)|(strcmp(session_id,"BASELINE2")==0)|(strcmp(session_id,"DRIVING2")==0)|(strcmp(session_id,"POST_SHAKING2")==0))
				{	
					sscanf(timestamp,"%d:%d:%d.%d",&hh_timestamp,&mm_timestamp,&ss_timestamp,&ls_timestamp);					
					
					//if((strcmp(pre_session_id,"NULL_SESSION")==0)&&((strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"BASELINE2")==0)))
					if((strcmp(pre_session_id,"NULL_SESSION")==0)&&((strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"BASELINE2")==0)|(strcmp(session_id,"PRE_SHAKING1")==0)|(strcmp(session_id,"PRE_SHAKING2")==0)|(strcmp(session_id,"POST_SHAKING1")==0)|(strcmp(session_id,"POST_SHAKING2")==0)))
					{
						//Reset at the moment escaping from the null sessions to a meaningful session
						for(int ii=0; ii<4; ii++)
						{
							temp_baseline_gravity_x[ii]=0.0;
							temp_baseline_gravity_y[ii]=0.0;
							temp_baseline_gravity_z[ii]=0.0;
						}
						
						printf("\nsb%s [Passed RESET loop] - %s -> %s \n", STT_subject_id[i], pre_session_id, session_id);
						
						// WORK HERE -------> baseline x,y,z are incorrectly calculated in DRIVING 2 in the next subject 14 (correctly in DRIVING 1 though)
					}

					if(ss_timestamp==pre_ss_timestamp) // Stack during the same 'sec' unit (i.e., for 1 sec; e.g., hh:mm:01.001 to hh:mm:01.999)
					{	
						if(strcmp(sensor_id,"Y0")==0){
							sensor_id_scanned=0;
							count_samples++;			// single increment during a cycle of Y0~Y3
						}else if(strcmp(sensor_id,"Y1")==0){
							sensor_id_scanned=1;
						}else if(strcmp(sensor_id,"Y2")==0){
							sensor_id_scanned=2;
						}else if(strcmp(sensor_id,"Y3")==0){
							sensor_id_scanned=3;
							pre_ss_timestamp=ss_timestamp; // store only when a cycle from Y0~Y3 is completed
						}
									
						// Averaging w.r.t 1-sec time windows
						total_gravity_x_per_unit_time[sensor_id_scanned]+=atof(gravity_x);
						total_gravity_y_per_unit_time[sensor_id_scanned]+=atof(gravity_y);
						total_gravity_z_per_unit_time[sensor_id_scanned]+=atof(gravity_z);

						//total_gyro_x_per_unit_time[sensor_id_scanned]+=fabs(atof(gyro_x)); // use absolute for feature extraction
						//total_gyro_y_per_unit_time[sensor_id_scanned]+=fabs(atof(gyro_y)); // use absolute for feature extraction
						//total_gyro_z_per_unit_time[sensor_id_scanned]+=fabs(atof(gyro_z)); // use absolute for feature extraction

						total_gyro_x_per_unit_time[sensor_id_scanned]+=atof(gyro_x);
						total_gyro_y_per_unit_time[sensor_id_scanned]+=atof(gyro_y);
						total_gyro_z_per_unit_time[sensor_id_scanned]+=atof(gyro_z);

						total_north_x_per_unit_time[sensor_id_scanned]+=atof(north_x);
						total_north_y_per_unit_time[sensor_id_scanned]+=atof(north_y);
						total_north_z_per_unit_time[sensor_id_scanned]+=atof(north_z);
					}else //Log as soon as the 'sec' unit is changed (e.g., hh:mm:01.999 to hh:mm:02.001)
					{
						sprintf(next_UTC_timestamp,"%d-%02d-%02dT%d:%02d:%02d.000-05:00",UTC_yyyy,UTC_mm,UTC_dd,hh_timestamp,mm_timestamp,ss_timestamp);
						
						if((strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"BASELINE2")==0))
						{
							if(strcmp(sensor_id,"Y0")==0){
								//if((strcmp(STT_subject_id[i],"14")==0)&&(strcmp(session_id,"BASELINE2")==0))
								//{
									//printf("Looping Here - accumulating in temp!\n");
									//printf("baseline_gravity_x[0]=%f, \n count_samples=%d, total_gravity_x_per_unit_time[0]=%f, temp_baseline_gravity_x[0]=%f\n\n",baseline_gravity_x[0],count_samples,total_gravity_x_per_unit_time[0],temp_baseline_gravity_x[0]);
									//printf("Before: baseline_gravity_x[0]=%f, temp_baseline_gravity_x[0]=%f\n\n",baseline_gravity_x[0], temp_baseline_gravity_x[0]);
								//}

								temp_baseline_gravity_x[0]+=total_gravity_x_per_unit_time[0]/(count_samples);
								temp_baseline_gravity_y[0]+=total_gravity_y_per_unit_time[0]/(count_samples);
								temp_baseline_gravity_z[0]+=total_gravity_z_per_unit_time[0]/(count_samples);
								
								baseline_gravity_x[0]=total_gravity_x_per_unit_time[0]/(count_samples);
								baseline_gravity_y[0]=total_gravity_y_per_unit_time[0]/(count_samples);
								baseline_gravity_z[0]=total_gravity_z_per_unit_time[0]/(count_samples);
								//if(strcmp(next_UTC_timestamp,"2013-12-17T11:55:20.000-05:00")==0)
								//if((strcmp(STT_subject_id[i],"14")==0)&&(strcmp(session_id,"BASELINE2")==0))
								//{
									//printf("baseline_gravity_x[0]=%f, \n count_samples=%d, total_gravity_x_per_unit_time[0]=%f, temp_baseline_gravity_x[0]=%f\n\n",baseline_gravity_x[0],count_samples,total_gravity_x_per_unit_time[0],temp_baseline_gravity_x[0]);
									//printf("After: baseline_gravity_x[0]=%f, count_samples=%d, \n total_g_x[0]=%f, temp_baseline_gravity_x[0]=%f\n\n",baseline_gravity_x[0],count_samples,total_gravity_x_per_unit_time[0],temp_baseline_gravity_x[0]);
								//}

							}else if(strcmp(sensor_id,"Y1")==0){
								temp_baseline_gravity_x[1]+=total_gravity_x_per_unit_time[1]/(count_samples);
								temp_baseline_gravity_y[1]+=total_gravity_y_per_unit_time[1]/(count_samples);
								temp_baseline_gravity_z[1]+=total_gravity_z_per_unit_time[1]/(count_samples);
								
								baseline_gravity_x[1]=total_gravity_x_per_unit_time[1]/(count_samples);
								baseline_gravity_y[1]=total_gravity_y_per_unit_time[1]/(count_samples);
								baseline_gravity_z[1]=total_gravity_z_per_unit_time[1]/(count_samples);
							}else if(strcmp(sensor_id,"Y2")==0){
								temp_baseline_gravity_x[2]+=total_gravity_x_per_unit_time[2]/(count_samples);
								temp_baseline_gravity_y[2]+=total_gravity_y_per_unit_time[2]/(count_samples);
								temp_baseline_gravity_z[2]+=total_gravity_z_per_unit_time[2]/(count_samples);
								
								baseline_gravity_x[2]=total_gravity_x_per_unit_time[2]/(count_samples);
								baseline_gravity_y[2]=total_gravity_y_per_unit_time[2]/(count_samples);
								baseline_gravity_z[2]=total_gravity_z_per_unit_time[2]/(count_samples);
							}else if(strcmp(sensor_id,"Y3")==0){
								temp_baseline_gravity_x[3]+=total_gravity_x_per_unit_time[3]/(count_samples);
								temp_baseline_gravity_y[3]+=total_gravity_y_per_unit_time[3]/(count_samples);
								temp_baseline_gravity_z[3]+=total_gravity_z_per_unit_time[3]/(count_samples);
								
								baseline_gravity_x[3]=total_gravity_x_per_unit_time[3]/(count_samples);
								baseline_gravity_y[3]=total_gravity_y_per_unit_time[3]/(count_samples);
								baseline_gravity_z[3]=total_gravity_z_per_unit_time[3]/(count_samples);
							}
						}else
						{
							if(strcmp(sensor_id,"Y0")==0){
								baseline_gravity_x[0]=temp_baseline_gravity_x[0]/60.0;	// 60-sec baseline session
								baseline_gravity_y[0]=temp_baseline_gravity_y[0]/60.0;	// 60-sec baseline session
								baseline_gravity_z[0]=temp_baseline_gravity_z[0]/60.0;	// 60-sec baseline session
							}else if(strcmp(sensor_id,"Y1")==0){
								baseline_gravity_x[1]=temp_baseline_gravity_x[1]/60.0;	// 60-sec baseline session
								baseline_gravity_y[1]=temp_baseline_gravity_y[1]/60.0;	// 60-sec baseline session
								baseline_gravity_z[1]=temp_baseline_gravity_z[1]/60.0;	// 60-sec baseline session
							}else if(strcmp(sensor_id,"Y2")==0){
								baseline_gravity_x[2]=temp_baseline_gravity_x[2]/60.0;	// 60-sec baseline session
								baseline_gravity_y[2]=temp_baseline_gravity_y[2]/60.0;	// 60-sec baseline session
								baseline_gravity_z[2]=temp_baseline_gravity_z[2]/60.0;	// 60-sec baseline session
							}else if(strcmp(sensor_id,"Y3")==0){
								baseline_gravity_x[3]=temp_baseline_gravity_x[3]/60.0;	// 60-sec baseline session
								baseline_gravity_y[3]=temp_baseline_gravity_y[3]/60.0;	// 60-sec baseline session
								baseline_gravity_z[3]=temp_baseline_gravity_z[3]/60.0;	// 60-sec baseline session
							}
						}

						
						char trend_gravity_feature[4][30]={"Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec"};
						char trend_gyro_feature[4][30]={"Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec","Null_Inc_Dec"};

						char level_gravity_feature[4][30]={"Null_Gravity_level","Null_Gravity_level","Null_Gravity_level","Null_Gravity_level"};
						char level_gyro_feature[4][30]={"Null_Gyro_level","Null_Gyro_level","Null_Gyro_level","Null_Gyro_level"};

						//print the results - for pure averaging w.r.t 1-sec time windows

						// if there are missing data in Y0, Y1, Y2, or Y3, fill with null data  (exemplar case: sb14)
						if(
							((strcmp(last_Y_id, "Y0")==0)&&(strcmp(sensor_id,"Y1")!=0))|
							((strcmp(last_Y_id, "Y1")==0)&&(strcmp(sensor_id,"Y2")!=0))|
							((strcmp(last_Y_id, "Y2")==0)&&(strcmp(sensor_id,"Y3")!=0))
							)
						{
							if(strcmp(last_Y_id, "Y0")==0)
							{
								for(int missed_Y=0; missed_Y<3; missed_Y++)
								{
									for(int n_dataset=0; n_dataset<22; n_dataset++) // Important! - if there is any addition or exclusion in the columns, the n_dataset should be changed!!!
									{
										output_labeled_sensor_data_files[i] << "," << "Missing_Y13";
									}
								}
								//output_labeled_sensor_data_files[i] << "," ;//<< "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null";
							}else if(strcmp(last_Y_id, "Y1")==0)
							{
								for(int missed_Y=0; missed_Y<2; missed_Y++)
								{
									for(int n_dataset=0; n_dataset<22; n_dataset++) // Important! - if there is any addition or exclusion in the columns, the n_dataset should be changed!!!
									{
										output_labeled_sensor_data_files[i] << "," << "Missing_Y23";
									}
								}
								//output_labeled_sensor_data_files[i] << "," ;//<< "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null";
							}else if(strcmp(last_Y_id, "Y2")==0)
							{
								for(int n_dataset=0; n_dataset<22; n_dataset++) // Important! - if there is any addition or exclusion in the columns, the n_dataset should be changed!!!
								{
									output_labeled_sensor_data_files[i] << "," << "Missing_Y3";
								}
								//output_labeled_sensor_data_files[i] << "," ;//<< "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null" << "," << "Null";
							}
						}						

						if((strcmp(sensor_id,"Y0")==0)|(strcmp(sensor_id,"Y1")==0)|(strcmp(sensor_id,"Y2")==0)|(strcmp(sensor_id,"Y3")==0))
						{
							strcpy(last_Y_id,sensor_id);
						}

						if(strcmp(sensor_id,"Y0")==0)			// Left Hand
						{
							//integrated_gravity_deviation[0]= (fabs(total_gravity_x_per_unit_time[0]-baseline_gravity_x[0])+fabs(total_gravity_y_per_unit_time[0]-baseline_gravity_y[0])+fabs(total_gravity_z_per_unit_time[0]-baseline_gravity_z[0]))/3.0;
							integrated_gravity_deviation[0]=sqrt((pow(total_gravity_x_per_unit_time[0]/count_samples-baseline_gravity_x[0], 2.0)+pow(total_gravity_y_per_unit_time[0]/count_samples-baseline_gravity_y[0], 2.0)+pow(total_gravity_z_per_unit_time[0]/count_samples-baseline_gravity_z[0], 2.0))/pow(3.0,2.0));
							integrated_gyro[0]=sqrt((pow(total_gyro_x_per_unit_time[0]/count_samples, 2.0)+pow(total_gyro_y_per_unit_time[0]/count_samples, 2.0)+pow(total_gyro_z_per_unit_time[0]/count_samples, 2.0))/pow(3.0,2.0));
		
							if(integrated_gravity_deviation[0]-pre_integrated_gravity_deviation[0]>0)
							{
								strcpy(trend_gravity_feature[0],"Increment");
							}else if(integrated_gravity_deviation[0]-pre_integrated_gravity_deviation[0]<0)
							{
								strcpy(trend_gravity_feature[0],"Decrement");
							}else
							{
								strcpy(trend_gravity_feature[0],"Plain");
							}

							if(strcmp(trend_gravity_feature[0], pre_trend_gravity_feature[0])==0)
							{
								gravity_elapsed_time[0]++;
							}else
							{
								gravity_elapsed_time[0]=1;
							}

							//Levels newly added --- from here
							gravity_level_threshold[0]=0.1;
							gravity_level_threshold[1]=0.25;
							gyro_level_threshold[0]=0.05;
							gyro_level_threshold[1]=0.2;
							gyro_level_threshold[2]=0.4;

							if(integrated_gravity_deviation[0]>=gravity_level_threshold[1])
							{
								strcpy(level_gravity_feature[0],"High_deviation");
							}else if((integrated_gravity_deviation[0]<gravity_level_threshold[1])&&(integrated_gravity_deviation[0]>=gravity_level_threshold[0]))
							{
								strcpy(level_gravity_feature[0],"Medium_deviation");
							}else if(integrated_gravity_deviation[0]<gravity_level_threshold[0])
							{
								strcpy(level_gravity_feature[0],"Low_deviation");
							}

							if(integrated_gyro[0]>=gyro_level_threshold[2])
							{
								strcpy(level_gyro_feature[0],"High_fluctuation");
							}else if((integrated_gyro[0]<gyro_level_threshold[2])&&(integrated_gyro[0]>=gyro_level_threshold[1]))
							{
								strcpy(level_gyro_feature[0],"Medium_fluctuation");
							}else if((integrated_gyro[0]<gyro_level_threshold[1])&&(integrated_gyro[0]>=gyro_level_threshold[0]))
							{
								strcpy(level_gyro_feature[0],"Low_fluctuation");
							}else if(integrated_gyro[0]<gyro_level_threshold[0])
							{
								strcpy(level_gyro_feature[0],"Ignorable_fluctuation");
							}
							// Levels newly added --- up to here

							if(integrated_gyro[0]-pre_integrated_gyro[0]>0)
							{
								strcpy(trend_gyro_feature[0],"Increment");
							}else if(integrated_gyro[0]-pre_integrated_gyro[0]<0)
							{
								strcpy(trend_gyro_feature[0],"Decrement");
							}else
							{
								strcpy(trend_gyro_feature[0],"Plain");
							}

							if(strcmp(trend_gyro_feature[0], pre_trend_gyro_feature[0])==0)
							{
								gyro_elapsed_time[0]++;
							}else
							{
								gyro_elapsed_time[0]=1;
							}

							strcpy(pre_trend_gravity_feature[0], trend_gravity_feature[0]);
							strcpy(pre_trend_gyro_feature[0], trend_gyro_feature[0]);

							output_labeled_sensor_data_files[i] <<	"\n" << STT_subject_id[i] << "," << session_id << "," << next_UTC_timestamp << "," 
								<< total_gravity_x_per_unit_time[0]/(count_samples) << "," << total_gravity_y_per_unit_time[0]/(count_samples) << "," << total_gravity_z_per_unit_time[0]/(count_samples) << ","
								<< integrated_gravity_deviation[0] << "," << level_gravity_feature[0] << ","
								<< integrated_gravity_deviation[0]-pre_integrated_gravity_deviation[0] << "," << trend_gravity_feature[0] << "," << gravity_elapsed_time[0] << ","
								<< baseline_gravity_x[0] << "," << baseline_gravity_y[0] << "," << baseline_gravity_z[0] << ","
								<< total_gyro_x_per_unit_time[0]/(count_samples) << "," << total_gyro_y_per_unit_time[0]/(count_samples) << "," << total_gyro_z_per_unit_time[0]/(count_samples) << "," 
								<< integrated_gyro[0] << "," << level_gyro_feature[0] << ","
								<< integrated_gyro[0]-pre_integrated_gyro[0] << "," << trend_gyro_feature[0] << "," << gyro_elapsed_time[0] << ","
								<< total_north_x_per_unit_time[0]/(count_samples) << "," << total_north_y_per_unit_time[0]/(count_samples) << "," << total_north_z_per_unit_time[0]/(count_samples);

							pre_integrated_gravity_deviation[0]=integrated_gravity_deviation[0];
							pre_integrated_gyro[0]=integrated_gyro[0];
							//if((strcmp(STT_subject_id[i],"14")==0)&&((strcmp(session_id,"BASELINE2")==0)|(strcmp(session_id,"DRIVING2"))))
							//if(((strcmp(STT_subject_id[i],"14")==0)&&(strcmp(session_id,"BASELINE2")==0))|((strcmp(STT_subject_id[i],"14")==0)&&(strcmp(session_id,"DRIVING2")==0)))
							//if((strcmp(session_id,"BASELINE1")==0)|(strcmp(session_id,"BASELINE2")==0))
							//{
							//	printf("sb%s %s (%s %s): count_samples (%d), baseline_gravity_x - %f (temp: %f)\n", STT_subject_id[i], sensor_id, session_id, next_UTC_timestamp, count_samples, baseline_gravity_x[0], temp_baseline_gravity_x[0]);
							//	cin.get();
							//}
						}else if(strcmp(sensor_id,"Y1")==0)		// Head
						{
							//integrated_gravity_deviation[1]= (fabs(total_gravity_x_per_unit_time[1]-baseline_gravity_x[1])+fabs(total_gravity_y_per_unit_time[1]-baseline_gravity_y[1])+fabs(total_gravity_z_per_unit_time[1]-baseline_gravity_z[1]))/3.0;
							integrated_gravity_deviation[1]=sqrt((pow(total_gravity_x_per_unit_time[1]/count_samples-baseline_gravity_x[1], 2.0)+pow(total_gravity_y_per_unit_time[1]/count_samples-baseline_gravity_y[1], 2.0)+pow(total_gravity_z_per_unit_time[1]/count_samples-baseline_gravity_z[1], 2.0))/pow(3.0,2.0));
							integrated_gyro[1]=sqrt((pow(total_gyro_x_per_unit_time[1]/count_samples, 2.0)+pow(total_gyro_y_per_unit_time[1]/count_samples, 2.0)+pow(total_gyro_z_per_unit_time[1]/count_samples, 2.0))/pow(3.0,2.0));

							if(integrated_gravity_deviation[1]-pre_integrated_gravity_deviation[1]>0)
							{
								strcpy(trend_gravity_feature[1],"Increment");
							}else if(integrated_gravity_deviation[1]-pre_integrated_gravity_deviation[1]<0)
							{
								strcpy(trend_gravity_feature[1],"Decrement");
							}else
							{
								strcpy(trend_gravity_feature[1],"Plain");
							}

							if(strcmp(trend_gravity_feature[1], pre_trend_gravity_feature[1])==0)
							{
								gravity_elapsed_time[1]++;
							}else
							{
								gravity_elapsed_time[1]=1;
							}							
							
							//Levels newly added --- from here
							gravity_level_threshold[0]=0.05; // No much meaning
							gravity_level_threshold[1]=0.2;  // No much meaning
							gyro_level_threshold[0]=0.05;
							gyro_level_threshold[1]=0.1;
							gyro_level_threshold[2]=0.2;

							if(integrated_gravity_deviation[1]>=gravity_level_threshold[1])
							{
								strcpy(level_gravity_feature[1],"High_deviation");
							}else if((integrated_gravity_deviation[1]<gravity_level_threshold[1])&&(integrated_gravity_deviation[1]>=gravity_level_threshold[0]))
							{
								strcpy(level_gravity_feature[1],"Medium_deviation");
							}else if(integrated_gravity_deviation[1]<gravity_level_threshold[0])
							{
								strcpy(level_gravity_feature[1],"Low_deviation");
							}

							if(integrated_gyro[1]>=gyro_level_threshold[2])
							{
								strcpy(level_gyro_feature[1],"High_fluctuation");
							}else if((integrated_gyro[1]<gyro_level_threshold[2])&&(integrated_gyro[1]>=gyro_level_threshold[1]))
							{
								strcpy(level_gyro_feature[1],"Medium_fluctuation");
							}else if((integrated_gyro[1]<gyro_level_threshold[1])&&(integrated_gyro[1]>=gyro_level_threshold[0]))
							{
								strcpy(level_gyro_feature[1],"Low_fluctuation");
							}else if(integrated_gyro[1]<gyro_level_threshold[0])
							{
								strcpy(level_gyro_feature[1],"Ignorable_fluctuation");
							}
							// Levels newly added --- up to here
							
							if(integrated_gyro[1]-pre_integrated_gyro[1]>0)
							{
								strcpy(trend_gyro_feature[1],"Increment");
							}else if(integrated_gyro[1]-pre_integrated_gyro[1]<0)
							{
								strcpy(trend_gyro_feature[1],"Decrement");
							}else
							{
								strcpy(trend_gyro_feature[1],"Plain");
							}

							if(strcmp(trend_gyro_feature[1], pre_trend_gyro_feature[1])==0)
							{
								gyro_elapsed_time[1]++;
							}else
							{
								gyro_elapsed_time[1]=1;
							}

							strcpy(pre_trend_gravity_feature[1], trend_gravity_feature[1]);
							strcpy(pre_trend_gyro_feature[1], trend_gyro_feature[1]);

							output_labeled_sensor_data_files[i] <<	"," 
								<< total_gravity_x_per_unit_time[1]/(count_samples) << "," << total_gravity_y_per_unit_time[1]/(count_samples) << "," << total_gravity_z_per_unit_time[1]/(count_samples) << ","
								<< integrated_gravity_deviation[1] << "," << level_gravity_feature[1] << ","
								<< integrated_gravity_deviation[1]-pre_integrated_gravity_deviation[1] << "," << trend_gravity_feature[1] << "," << gravity_elapsed_time[1] << ","
								<< baseline_gravity_x[1] << "," << baseline_gravity_y[1] << "," << baseline_gravity_z[1] << ","
								<< total_gyro_x_per_unit_time[1]/(count_samples) << "," << total_gyro_y_per_unit_time[1]/(count_samples) << "," << total_gyro_z_per_unit_time[1]/(count_samples) << "," 
								<< integrated_gyro[1] << "," << level_gyro_feature[1] << ","
								<< integrated_gyro[1]-pre_integrated_gyro[1] << "," << trend_gyro_feature[1] << "," << gyro_elapsed_time[1] << ","
								<< total_north_x_per_unit_time[1]/(count_samples) << "," << total_north_y_per_unit_time[1]/(count_samples) << "," << total_north_z_per_unit_time[1]/(count_samples);

							pre_integrated_gravity_deviation[1]=integrated_gravity_deviation[1];
							pre_integrated_gyro[1]=integrated_gyro[1];
						}else if(strcmp(sensor_id,"Y2")==0)		// Foot
						{
							//integrated_gravity_deviation[2]= (fabs(total_gravity_x_per_unit_time[2]-baseline_gravity_x[2])+fabs(total_gravity_y_per_unit_time[2]-baseline_gravity_y[2])+fabs(total_gravity_z_per_unit_time[2]-baseline_gravity_z[2]))/3.0;
							integrated_gravity_deviation[2]=sqrt((pow(total_gravity_x_per_unit_time[2]/count_samples-baseline_gravity_x[2], 2.0)+pow(total_gravity_y_per_unit_time[2]/count_samples-baseline_gravity_y[2], 2.0)+pow(total_gravity_z_per_unit_time[2]/count_samples-baseline_gravity_z[2], 2.0))/pow(3.0,2.0));
							integrated_gyro[2]=sqrt((pow(total_gyro_x_per_unit_time[2]/count_samples, 2.0)+pow(total_gyro_y_per_unit_time[2]/count_samples, 2.0)+pow(total_gyro_z_per_unit_time[2]/count_samples, 2.0))/pow(3.0,2.0));
							
							if(integrated_gravity_deviation[2]-pre_integrated_gravity_deviation[2]>0)
							{
								strcpy(trend_gravity_feature[2],"Increment");
							}else if(integrated_gravity_deviation[2]-pre_integrated_gravity_deviation[2]<0)
							{
								strcpy(trend_gravity_feature[2],"Decrement");
							}else
							{
								strcpy(trend_gravity_feature[2],"Plain");
							}

							if(strcmp(trend_gravity_feature[2], pre_trend_gravity_feature[2])==0)
							{
								gravity_elapsed_time[2]++;
							}else
							{
								gravity_elapsed_time[2]=1;
							}						
							
							//Levels newly added --- from here
							gravity_level_threshold[0]=0.05; // No much meaning
							gravity_level_threshold[1]=0.2;  // No much meaning
							gyro_level_threshold[0]=0.05;
							gyro_level_threshold[1]=0.1;
							gyro_level_threshold[2]=0.2;

							if(integrated_gravity_deviation[2]>=gravity_level_threshold[1])
							{
								strcpy(level_gravity_feature[2],"High_deviation");
							}else if((integrated_gravity_deviation[2]<gravity_level_threshold[1])&&(integrated_gravity_deviation[2]>=gravity_level_threshold[0]))
							{
								strcpy(level_gravity_feature[2],"Medium_deviation");
							}else if(integrated_gravity_deviation[2]<gravity_level_threshold[0])
							{
								strcpy(level_gravity_feature[2],"Low_deviation");
							}

							if(integrated_gyro[2]>=gyro_level_threshold[2])
							{
								strcpy(level_gyro_feature[2],"High_fluctuation");
							}else if((integrated_gyro[2]<gyro_level_threshold[2])&&(integrated_gyro[2]>=gyro_level_threshold[1]))
							{
								strcpy(level_gyro_feature[2],"Medium_fluctuation");
							}else if((integrated_gyro[2]<gyro_level_threshold[1])&&(integrated_gyro[2]>=gyro_level_threshold[0]))
							{
								strcpy(level_gyro_feature[2],"Low_fluctuation");
							}else if(integrated_gyro[2]<gyro_level_threshold[0])
							{
								strcpy(level_gyro_feature[2],"Ignorable_fluctuation");
							}
							// Levels newly added --- up to here
							
							if(integrated_gyro[2]-pre_integrated_gyro[2]>0)
							{
								strcpy(trend_gyro_feature[2],"Increment");
							}else if(integrated_gyro[2]-pre_integrated_gyro[2]<0)
							{
								strcpy(trend_gyro_feature[2],"Decrement");
							}else
							{
								strcpy(trend_gyro_feature[2],"Plain");
							}

							if(strcmp(trend_gyro_feature[2], pre_trend_gyro_feature[2])==0)
							{
								gyro_elapsed_time[2]++;
							}else
							{
								gyro_elapsed_time[2]=1;
							}

							strcpy(pre_trend_gravity_feature[2], trend_gravity_feature[2]);
							strcpy(pre_trend_gyro_feature[2], trend_gyro_feature[2]);

							output_labeled_sensor_data_files[i] <<	"," 
								<< total_gravity_x_per_unit_time[2]/(count_samples) << "," << total_gravity_y_per_unit_time[2]/(count_samples) << "," << total_gravity_z_per_unit_time[2]/(count_samples) << ","
								<< integrated_gravity_deviation[2] << "," << level_gravity_feature[2] << ","
								<< integrated_gravity_deviation[2]-pre_integrated_gravity_deviation[2] << "," << trend_gravity_feature[2] << "," << gravity_elapsed_time[2] << ","
								<< baseline_gravity_x[2] << "," << baseline_gravity_y[2] << "," << baseline_gravity_z[2] << ","
								<< total_gyro_x_per_unit_time[2]/(count_samples) << "," << total_gyro_y_per_unit_time[2]/(count_samples) << "," << total_gyro_z_per_unit_time[2]/(count_samples) << "," 
								<< integrated_gyro[2] << "," << level_gyro_feature[2] << ","
								<< integrated_gyro[2]-pre_integrated_gyro[2] << "," << trend_gyro_feature[2] << "," << gyro_elapsed_time[2] << ","
								<< total_north_x_per_unit_time[2]/(count_samples) << "," << total_north_y_per_unit_time[2]/(count_samples) << "," << total_north_z_per_unit_time[2]/(count_samples);

							printf("[parsed] timestamp=%s, trend_gyro_feature[foot]=%s\n\n", timestamp, trend_gyro_feature[2]);

							pre_integrated_gravity_deviation[2]=integrated_gravity_deviation[2];
							pre_integrated_gyro[2]=integrated_gyro[2];
						}else if(strcmp(sensor_id,"Y3")==0)		// Right Hand
						{
							//integrated_gravity_deviation[3]= (fabs(total_gravity_x_per_unit_time[3]-baseline_gravity_x[3])+fabs(total_gravity_y_per_unit_time[3]-baseline_gravity_y[3])+fabs(total_gravity_z_per_unit_time[3]-baseline_gravity_z[3]))/3.0;
							integrated_gravity_deviation[3]=sqrt((pow(total_gravity_x_per_unit_time[3]/count_samples-baseline_gravity_x[3], 2.0)+pow(total_gravity_y_per_unit_time[3]/count_samples-baseline_gravity_y[3], 2.0)+pow(total_gravity_z_per_unit_time[3]/count_samples-baseline_gravity_z[3], 2.0))/pow(3.0,2.0));
							integrated_gyro[3]=sqrt((pow(total_gyro_x_per_unit_time[3]/count_samples, 2.0)+pow(total_gyro_y_per_unit_time[3]/count_samples, 2.0)+pow(total_gyro_z_per_unit_time[3]/count_samples, 2.0))/pow(3.0,2.0));

							if(integrated_gravity_deviation[3]-pre_integrated_gravity_deviation[3]>0)
							{
								strcpy(trend_gravity_feature[3],"Increment");
							}else if(integrated_gravity_deviation[3]-pre_integrated_gravity_deviation[3]<0)
							{
								strcpy(trend_gravity_feature[3],"Decrement");
							}else
							{
								strcpy(trend_gravity_feature[3],"Plain");
							}

							if(strcmp(trend_gravity_feature[3], pre_trend_gravity_feature[3])==0)
							{
								gravity_elapsed_time[3]++;
							}else
							{
								gravity_elapsed_time[3]=1;
							}						
							
							//Levels newly added --- from here
							gravity_level_threshold[0]=0.1;
							gravity_level_threshold[1]=0.25;
							gyro_level_threshold[0]=0.05;
							gyro_level_threshold[1]=0.2;
							gyro_level_threshold[2]=0.4;

							if(integrated_gravity_deviation[3]>=gravity_level_threshold[1])
							{
								strcpy(level_gravity_feature[3],"High_deviation");
							}else if((integrated_gravity_deviation[3]<gravity_level_threshold[1])&&(integrated_gravity_deviation[3]>=gravity_level_threshold[0]))
							{
								strcpy(level_gravity_feature[3],"Medium_deviation");
							}else if(integrated_gravity_deviation[3]<gravity_level_threshold[0])
							{
								strcpy(level_gravity_feature[3],"Low_deviation");
							}

							if(integrated_gyro[3]>=gyro_level_threshold[2])
							{
								strcpy(level_gyro_feature[3],"High_fluctuation");
							}else if((integrated_gyro[3]<gyro_level_threshold[2])&&(integrated_gyro[3]>=gyro_level_threshold[1]))
							{
								strcpy(level_gyro_feature[3],"Medium_fluctuation");
							}else if((integrated_gyro[3]<gyro_level_threshold[1])&&(integrated_gyro[3]>=gyro_level_threshold[0]))
							{
								strcpy(level_gyro_feature[3],"Low_fluctuation");
							}else if(integrated_gyro[3]<gyro_level_threshold[0])
							{
								strcpy(level_gyro_feature[3],"Ignorable_fluctuation");
							}
							// Levels newly added --- up to here
							
							if(integrated_gyro[3]-pre_integrated_gyro[3]>0)
							{
								strcpy(trend_gyro_feature[3],"Increment");
							}else if(integrated_gyro[3]-pre_integrated_gyro[3]<0)
							{
								strcpy(trend_gyro_feature[3],"Decrement");
							}else
							{
								strcpy(trend_gyro_feature[3],"Plain");
							}

							if(strcmp(trend_gyro_feature[3], pre_trend_gyro_feature[3])==0)
							{
								gyro_elapsed_time[3]++;
							}else
							{
								gyro_elapsed_time[3]=1;
							}

							strcpy(pre_trend_gravity_feature[3], trend_gravity_feature[3]);
							strcpy(pre_trend_gyro_feature[3], trend_gyro_feature[3]);

							output_labeled_sensor_data_files[i] <<	"," 
								<< total_gravity_x_per_unit_time[3]/(count_samples) << "," << total_gravity_y_per_unit_time[3]/(count_samples) << "," << total_gravity_z_per_unit_time[3]/(count_samples) << ","
								<< integrated_gravity_deviation[3] << "," << level_gravity_feature[3] << ","
								<< integrated_gravity_deviation[3]-pre_integrated_gravity_deviation[3] << "," << trend_gravity_feature[3] << "," << gravity_elapsed_time[3] << ","
								<< baseline_gravity_x[3] << "," << baseline_gravity_y[3] << "," << baseline_gravity_z[3] << ","
								<< total_gyro_x_per_unit_time[3]/(count_samples) << "," << total_gyro_y_per_unit_time[3]/(count_samples) << "," << total_gyro_z_per_unit_time[3]/(count_samples) << "," 
								<< integrated_gyro[3] << "," << level_gyro_feature[3] << ","
								<< integrated_gyro[3]-pre_integrated_gyro[3] << "," << trend_gyro_feature[3] << "," << gyro_elapsed_time[3] << ","
								<< total_north_x_per_unit_time[3]/(count_samples) << "," << total_north_y_per_unit_time[3]/(count_samples) << "," << total_north_z_per_unit_time[3]/(count_samples); 
								//<< "," 	<< driver_PI_log;// << endl;

							pre_integrated_gravity_deviation[3]=integrated_gravity_deviation[3];
							pre_integrated_gyro[3]=integrated_gyro[3];
						}

						//store the current raw sensor dat for the next iteration
						if(strcmp(sensor_id,"Y0")==0){
							sensor_id_scanned=0;
							cycle_completed=false;
						}else if(strcmp(sensor_id,"Y1")==0){
							sensor_id_scanned=1;
							cycle_completed=false;
						}else if(strcmp(sensor_id,"Y2")==0){
							sensor_id_scanned=2;
							cycle_completed=false;
						}else if(strcmp(sensor_id,"Y3")==0){
							sensor_id_scanned=3;
							pre_ss_timestamp=ss_timestamp;  // store only when a cycle from Y0~Y3 is completed
							count_samples=1;				// reset only when a cycle from Y0~Y3 is completed
						}
									
						// Aeraging w.r.t 1-sec time windows
						total_gravity_x_per_unit_time[sensor_id_scanned]=atof(gravity_x);
						total_gravity_y_per_unit_time[sensor_id_scanned]=atof(gravity_y);
						total_gravity_z_per_unit_time[sensor_id_scanned]=atof(gravity_z);

						//total_gyro_x_per_unit_time[sensor_id_scanned]=fabs(atof(gyro_x)); // use absolute for feature extraction
						//total_gyro_y_per_unit_time[sensor_id_scanned]=fabs(atof(gyro_y)); // use absolute for feature extraction
						//total_gyro_z_per_unit_time[sensor_id_scanned]=fabs(atof(gyro_z)); // use absolute for feature extraction

						total_gyro_x_per_unit_time[sensor_id_scanned]=atof(gyro_x);
						total_gyro_y_per_unit_time[sensor_id_scanned]=atof(gyro_y);
						total_gyro_z_per_unit_time[sensor_id_scanned]=atof(gyro_z);

						total_north_x_per_unit_time[sensor_id_scanned]=atof(north_x);
						total_north_y_per_unit_time[sensor_id_scanned]=atof(north_y);
						total_north_z_per_unit_time[sensor_id_scanned]=atof(north_z);
					}						

				} // end of if Sessions of interest
			} // end of if Y0~Y3

			strcpy(pre_session_id, session_id);

					//strcpy(pre_timestamp,timestamp);
					//strcpy(pre_session_id, session_id);

					/*
					if(strcmp(session_id,pre_session_id)!=0) // if Exp Session is changed
					{
						//b_session_chagned=true;
						printf("[Session_changed]\n");
						if(strcmp(sensor_id,"Y0")==0) // Left Hand
						{
							output_labeled_sensor_data_files[i] <<	STT_subject_id[i] << "," << session_id << "," << next_UTC_timestamp << "," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
							printf("Logged(%s): %s %s %s gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, STT_subject_id[i], session_id, next_UTC_timestamp, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
						}else if((strcmp(sensor_id,"Y1")==0)|(strcmp(sensor_id,"Y2")==0)) // Head / Foot
						{
							output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
							printf("Logged(%s): gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
						}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
						{		
							output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z << "," << driver_PI_log << endl;
							printf("Logged(%s): gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
						}

						// Initialize for the next iteraction 
						count_samples=1;

						if(strcmp(sensor_id,"Y0")==0){
								sensor_id_scanned=0;
								cycle_completed=false;
						}else if(strcmp(sensor_id,"Y1")==0){
								sensor_id_scanned=1;
								cycle_completed=false;
						}else if(strcmp(sensor_id,"Y2")==0){
								sensor_id_scanned=2;
								cycle_completed=false;
						}else if(strcmp(sensor_id,"Y3")==0){
								sensor_id_scanned=3;
								cycle_completed=true;
						}
									
						//WORK HERE #1****** // even when it's Y1, it looks accumulating the value of Y3 (especially, in the first sampling of the session).
						total_gravity_x_per_unit_time[sensor_id_scanned]=atof(gravity_x);
						total_gravity_y_per_unit_time[sensor_id_scanned]=atof(gravity_y);
						total_gravity_z_per_unit_time[sensor_id_scanned]=atof(gravity_z);

						total_gyro_x_per_unit_time[sensor_id_scanned]=atof(gyro_x);
						total_gyro_y_per_unit_time[sensor_id_scanned]=atof(gyro_y);
						total_gyro_z_per_unit_time[sensor_id_scanned]=atof(gyro_z);

						total_north_x_per_unit_time[sensor_id_scanned]=atof(north_x);
						total_north_y_per_unit_time[sensor_id_scanned]=atof(north_y);
						total_north_z_per_unit_time[sensor_id_scanned]=atof(north_z);
					}else  // if Exp Session is the same
					{
						if((ss_timestamp!=pre_ss_timestamp)|(cycle_completed==false))
						{
							printf("[Within the same session]\n");
							if(strcmp(sensor_id,"Y0")==0)			// Left Hand
							{				
								output_labeled_sensor_data_files[i] <<	STT_subject_id[i] << "," << session_id << "," << next_UTC_timestamp << "," 
									<< total_gravity_x_per_unit_time[0]/(count_samples-1) << "," << total_gravity_y_per_unit_time[0]/(count_samples-1) << "," << total_gravity_z_per_unit_time[0]/(count_samples-1) << ","
									<< total_gyro_x_per_unit_time[0]/(count_samples-1) << "," << total_gyro_y_per_unit_time[0]/(count_samples-1) << "," << total_gyro_z_per_unit_time[0]/(count_samples-1) << "," 
									<< total_north_x_per_unit_time[0]/(count_samples-1) << "," << total_north_y_per_unit_time[0]/(count_samples-1) << "," << total_north_z_per_unit_time[0]/(count_samples-1);
							}else if(strcmp(sensor_id,"Y1")==0)		// Head
							{
								output_labeled_sensor_data_files[i] <<	"," 
									<< total_gravity_x_per_unit_time[1]/(count_samples-1) << "," << total_gravity_y_per_unit_time[1]/(count_samples-1) << "," << total_gravity_z_per_unit_time[1]/(count_samples-1) << ","
									<< total_gyro_x_per_unit_time[1]/(count_samples-1) << "," << total_gyro_y_per_unit_time[1]/(count_samples-1) << "," << total_gyro_z_per_unit_time[1]/(count_samples-1) << "," 
									<< total_north_x_per_unit_time[1]/(count_samples-1) << "," << total_north_y_per_unit_time[1]/(count_samples-1) << "," << total_north_z_per_unit_time[1]/(count_samples-1);
							}else if(strcmp(sensor_id,"Y2")==0)		// Foot
							{
								output_labeled_sensor_data_files[i] <<	"," 
									<< total_gravity_x_per_unit_time[2]/(count_samples-1) << "," << total_gravity_y_per_unit_time[2]/(count_samples-1) << "," << total_gravity_z_per_unit_time[2]/(count_samples-1) << ","
									<< total_gyro_x_per_unit_time[2]/(count_samples-1) << "," << total_gyro_y_per_unit_time[2]/(count_samples-1) << "," << total_gyro_z_per_unit_time[2]/(count_samples-1) << "," 
									<< total_north_x_per_unit_time[2]/(count_samples-1) << "," << total_north_y_per_unit_time[2]/(count_samples-1) << "," << total_north_z_per_unit_time[2]/(count_samples-1);
							}else if(strcmp(sensor_id,"Y3")==0)		// Right Hand
							{		
								output_labeled_sensor_data_files[i] <<	"," 
									<< total_gravity_x_per_unit_time[3]/(count_samples-1) << "," << total_gravity_y_per_unit_time[3]/(count_samples-1) << "," << total_gravity_z_per_unit_time[3]/(count_samples-1) << ","
									<< total_gyro_x_per_unit_time[3]/(count_samples-1) << "," << total_gyro_y_per_unit_time[3]/(count_samples-1) << "," << total_gyro_z_per_unit_time[3]/(count_samples-1) << "," 
									<< total_north_x_per_unit_time[3]/(count_samples-1) << "," << total_north_y_per_unit_time[3]/(count_samples-1) << "," << total_north_z_per_unit_time[3]/(count_samples-1) 
									<< "," 	<< driver_PI_log << endl;
							}

							// Initialize for the next iteraction 
							count_samples=1;

							if(strcmp(sensor_id,"Y0")==0){
									sensor_id_scanned=0;
									cycle_completed=false;
							}else if(strcmp(sensor_id,"Y1")==0){
									sensor_id_scanned=1;
									cycle_completed=false;
							}else if(strcmp(sensor_id,"Y2")==0){
									sensor_id_scanned=2;
									cycle_completed=false;
							}else if(strcmp(sensor_id,"Y3")==0){
									sensor_id_scanned=3;
									cycle_completed=true;
							}
									
							//WORK HERE #1****** // even when it's Y1, it looks accumulating the value of Y3 (especially, in the first sampling of the session).
							total_gravity_x_per_unit_time[sensor_id_scanned]=atof(gravity_x);
							total_gravity_y_per_unit_time[sensor_id_scanned]=atof(gravity_y);
							total_gravity_z_per_unit_time[sensor_id_scanned]=atof(gravity_z);

							total_gyro_x_per_unit_time[sensor_id_scanned]=atof(gyro_x);
							total_gyro_y_per_unit_time[sensor_id_scanned]=atof(gyro_y);
							total_gyro_z_per_unit_time[sensor_id_scanned]=atof(gyro_z);

							total_north_x_per_unit_time[sensor_id_scanned]=atof(north_x);
							total_north_y_per_unit_time[sensor_id_scanned]=atof(north_y);
							total_north_z_per_unit_time[sensor_id_scanned]=atof(north_z);
						}else
						{
							count_samples++;

							if(strcmp(sensor_id,"Y0")==0){
								sensor_id_scanned=0;
								cycle_completed=false;
							}else if(strcmp(sensor_id,"Y1")==0){
									sensor_id_scanned=1;
									cycle_completed=false;
							}else if(strcmp(sensor_id,"Y2")==0){
									sensor_id_scanned=2;
									cycle_completed=false;
							}else if(strcmp(sensor_id,"Y3")==0){
									sensor_id_scanned=3;
									cycle_completed=true;
							}
									
							total_gravity_x_per_unit_time[sensor_id_scanned]+=atof(gravity_x);
							total_gravity_y_per_unit_time[sensor_id_scanned]+=atof(gravity_y);
							total_gravity_z_per_unit_time[sensor_id_scanned]+=atof(gravity_z);

							total_gyro_x_per_unit_time[sensor_id_scanned]+=atof(gyro_x);
							total_gyro_y_per_unit_time[sensor_id_scanned]+=atof(gyro_y);
							total_gyro_z_per_unit_time[sensor_id_scanned]+=atof(gyro_z);

							total_north_x_per_unit_time[sensor_id_scanned]+=atof(north_x);
							total_north_y_per_unit_time[sensor_id_scanned]+=atof(north_y);
							total_north_z_per_unit_time[sensor_id_scanned]+=atof(north_z);

							printf("Stacking (count_samples=%d)- %s (%s, timestamp=%s)\n", count_samples, session_id, sensor_id, timestamp);
							printf("total_gravity= (x:%f, y:%f, z:%f) (just accumulated gravity = (x:%s, y:%s, z:%s)\n", 
								total_gravity_x_per_unit_time[sensor_id_scanned], total_gravity_y_per_unit_time[sensor_id_scanned], total_gravity_y_per_unit_time[sensor_id_scanned],
								gravity_x, gravity_y, gravity_z
							);
							printf("total_gyro= (x:%f, y:%f, z:%f) (just accumulated gyro = (x:%s, y:%s, z:%s)\n", 
								total_gyro_x_per_unit_time[sensor_id_scanned], total_gyro_y_per_unit_time[sensor_id_scanned], total_gyro_y_per_unit_time[sensor_id_scanned],
								gyro_x, gyro_y, gyro_z
							);
							printf("total_north= (x:%f, y:%f, z:%f) (just accumulated north = (x:%s, y:%s, z:%s)\n", 
								total_north_x_per_unit_time[sensor_id_scanned], total_north_y_per_unit_time[sensor_id_scanned], total_north_y_per_unit_time[sensor_id_scanned],
								north_x, north_y, north_z
							);
						}
						//b_session_chagned=false;

						
					}

					printf("Previous session: %s, Current session: %s, Count_samples=%d \n", pre_session_id, session_id, count_samples);
					cin.get();


					//if((ss_timestamp!=pre_ss_timestamp)|(cycle_completed==false))
					//{
						//WORK HERE #2****** -  control the 1st log of each session!
					
						if(b_session_chagned==true) // at the moment of session changes
						{
							printf("[Session_changed]\n");
							if(strcmp(sensor_id,"Y0")==0) // Left Hand
							{
								output_labeled_sensor_data_files[i] <<	STT_subject_id[i] << "," << session_id << "," << next_UTC_timestamp << "," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
								printf("Logged(%s): %s %s %s gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, STT_subject_id[i], session_id, next_UTC_timestamp, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
							}else if((strcmp(sensor_id,"Y1")==0)|(strcmp(sensor_id,"Y2")==0)) // Head / Foot
							{
								output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z;
								printf("Logged(%s): gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
							}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
							{		
								output_labeled_sensor_data_files[i] <<	"," << gravity_x << "," << gravity_y << "," << gravity_z << "," << gyro_x << "," << gyro_y << "," << gyro_z << "," << north_x << "," << north_y << "," << north_z << "," << driver_PI_log << endl;
								printf("Logged(%s): gravity(%s,%s,%s) gyro(%s,%s,%s) north(%s,%s,%s)\n", sensor_id, gravity_x, gravity_y, gravity_z, gyro_x, gyro_y, gyro_z, north_x, north_y, north_z);
							}
							b_session_chagned=false;
						}else // if within the same session
						{
							printf("[Within the same session]\n");
						}
						
						
						// reset
						
						
						//count_samples=0;
					
						

					//}else
					//{
						//printf("PASSED: %s: count_samples=%d, timestamp=%s (ss_timestamp %d == pre_ss_timestamp %d)\n", session_id, count_samples, timestamp, ss_timestamp, pre_ss_timestamp);
										
						if(strcmp(sensor_id,"Y0")==0){
								sensor_id_scanned=0;
						}else if(strcmp(sensor_id,"Y1")==0){
								sensor_id_scanned=1;
						}else if(strcmp(sensor_id,"Y2")==0){
								sensor_id_scanned=2;
						}else if(strcmp(sensor_id,"Y3")==0){
								sensor_id_scanned=3;
						}
									
						total_gravity_x_per_unit_time[sensor_id_scanned]+=atof(gravity_x);
						total_gravity_y_per_unit_time[sensor_id_scanned]+=atof(gravity_y);
						total_gravity_z_per_unit_time[sensor_id_scanned]+=atof(gravity_z);

						total_gyro_x_per_unit_time[sensor_id_scanned]+=atof(gyro_x);
						total_gyro_y_per_unit_time[sensor_id_scanned]+=atof(gyro_y);
						total_gyro_z_per_unit_time[sensor_id_scanned]+=atof(gyro_z);

						total_north_x_per_unit_time[sensor_id_scanned]+=atof(north_x);
						total_north_y_per_unit_time[sensor_id_scanned]+=atof(north_y);
						total_north_z_per_unit_time[sensor_id_scanned]+=atof(north_z);

						printf("Stacking (count_samples=%d)- %s (%s, timestamp=%s)\n", count_samples, session_id, sensor_id, timestamp);
						printf("total_gravity= (x:%f, y:%f, z:%f) (just accumulated gravity = (x:%s, y:%s, z:%s)\n", 
							total_gravity_x_per_unit_time[sensor_id_scanned], total_gravity_y_per_unit_time[sensor_id_scanned], total_gravity_y_per_unit_time[sensor_id_scanned],
							gravity_x, gravity_y, gravity_z
						);
						printf("total_gyro= (x:%f, y:%f, z:%f) (just accumulated gyro = (x:%s, y:%s, z:%s)\n", 
							total_gyro_x_per_unit_time[sensor_id_scanned], total_gyro_y_per_unit_time[sensor_id_scanned], total_gyro_y_per_unit_time[sensor_id_scanned],
							gyro_x, gyro_y, gyro_z
						);
						printf("total_north= (x:%f, y:%f, z:%f) (just accumulated north = (x:%s, y:%s, z:%s)\n", 
							total_north_x_per_unit_time[sensor_id_scanned], total_north_y_per_unit_time[sensor_id_scanned], total_north_y_per_unit_time[sensor_id_scanned],
							north_x, north_y, north_z
						);
						//cin.get();	

						//pre_ss_timestamp=ss_timestamp;
						//strcpy(pre_timestamp,timestamp);
						//}
				}			

			}
			
			strcpy(pre_session_id, session_id);
			pre_ss_timestamp=ss_timestamp;
			strcpy(pre_timestamp,timestamp);*/
			
			
			// Printing OUTPUT files 2!
/*
			if(strcmp(sensor_id,"Y0")==0) // Left Hand
			{
				if(pre_unixtime>0.0)
				{
					output_labeled_body_motion_files[i] <<	STT_subject_id[i] << "," << session_id << "," << UTC_timestamp << "," << unix_time << "," << first_d_gravity[0] << "," << first_d_gyro[0] << "," << first_d_north[0];
				}
				
				pre_grativty_x[0]=atof(gravity_x); 
				pre_grativty_y[0]=atof(gravity_y); 
				pre_grativty_z[0]=atof(gravity_z); 

				pre_gyro_x[0]=atof(gyro_x); 
				pre_gyro_y[0]=atof(gyro_y); 
				pre_gyro_z[0]=atof(gyro_z); 

				pre_north_x[0]=atof(north_x); 
				pre_north_y[0]=atof(north_y); 
				pre_north_z[0]=atof(north_z); 


			}else if(strcmp(sensor_id,"Y1")==0) // Head
			{
				if(pre_unixtime>0.0)
				{
					output_labeled_body_motion_files[i] << "," << first_d_gravity[1] << "," << first_d_gyro[1] << "," << first_d_north[1];
				}

				pre_grativty_x[1]=atof(gravity_x); 
				pre_grativty_y[1]=atof(gravity_y); 
				pre_grativty_z[1]=atof(gravity_z); 

				pre_gyro_x[1]=atof(gyro_x); 
				pre_gyro_y[1]=atof(gyro_y); 
				pre_gyro_z[1]=atof(gyro_z); 

				pre_north_x[1]=atof(north_x); 
				pre_north_y[1]=atof(north_y); 
				pre_north_z[1]=atof(north_z); 


			}else if(strcmp(sensor_id,"Y2")==0) // Foot
			{
				if(pre_unixtime>0.0)
				{
					output_labeled_body_motion_files[i] << "," << first_d_gravity[2] << "," << first_d_gyro[2] << "," << first_d_north[2];
				}

				pre_grativty_x[2]=atof(gravity_x); 
				pre_grativty_y[2]=atof(gravity_y); 
				pre_grativty_z[2]=atof(gravity_z); 

				pre_gyro_x[2]=atof(gyro_x); 
				pre_gyro_y[2]=atof(gyro_y); 
				pre_gyro_z[2]=atof(gyro_z); 

				pre_north_x[2]=atof(north_x); 
				pre_north_y[2]=atof(north_y); 
				pre_north_z[2]=atof(north_z); 


			}else if(strcmp(sensor_id,"Y3")==0) // Right Hand
			{	
				if(pre_unixtime>0.0)
				{
					//output_labeled_body_motion_files[i] << "," << first_d_gravity[3] << "," << first_d_gyro[3] << "," << first_d_north[3] << endl;
					output_labeled_body_motion_files[i] << "," << first_d_gravity[3] << "," << first_d_gyro[3] << "," << first_d_north[3];

					// SMA 1
					
					output_row_num++; // to determine the denominator for SMA

					for(int k=0; k<4; k++)
					{
						double denom;
						if(output_row_num==1)
						{
							denom=1.0;
						}else if(output_row_num==2)
						{
							denom=2.0;
						}else if(output_row_num>=3)
						{
							denom=3.0;
						}

						SMA1_1d_gravity[k]=(pre_1d_gravity[1][k]+pre_1d_gravity[0][k]+first_d_gravity[k])/denom;
						SMA1_1d_gyro[k]=(pre_1d_gyro[1][k]+pre_1d_gyro[0][k]+first_d_gyro[k])/denom;
						SMA1_1d_north[k]=(pre_1d_north[1][k]+pre_1d_north[0][k]+first_d_north[k])/denom;
						
						//WORK FROM HERE
						//printf("\noutput_row_num=%d\n",output_row_num);
						//printf("k=%d, SMA1_1d_gravity[%f]=(pre_1d_gravity[1][%f]+pre_1d_gravity[0][%f]+first_d_gravity[%f])/%f;\n",k, SMA1_1d_gravity[k], pre_1d_gravity[1][k], pre_1d_gravity[0][k], first_d_gravity[k], denom);
						//printf("k=%d, SMA1_1d_gyro[%f]=(pre_1d_gyro[1][%f]+pre_1d_gyro[0][%f]+first_d_gyro[%f])/%f;\n",k, SMA1_1d_gyro[k], pre_1d_gyro[1][k], pre_1d_gyro[0][k], first_d_gyro[k], denom);
						//printf("k=%d, SMA1_1d_north[%f]=(pre_1d_north[1][%f]+pre_1d_north[0][%f]+first_d_north[%f])/%f;\n\n",k, SMA1_1d_north[k], pre_1d_north[1][k], pre_1d_north[0][k], first_d_north[k], denom);
						//cin.get();

						SMA2_1d_gravity[k]=(near_SMA1_1d_gravity[1][k]+near_SMA1_1d_gravity[0][k]+SMA1_1d_gravity[k])/denom;
						SMA2_1d_gyro[k]=(near_SMA1_1d_gyro[1][k]+near_SMA1_1d_gyro[0][k]+SMA1_1d_gyro[k])/denom;
						SMA2_1d_north[k]=(near_SMA1_1d_north[1][k]+near_SMA1_1d_north[0][k]+SMA1_1d_north[k])/denom;

						threshold = 2.5;
						motion_changes_gravity[k] = (SMA2_1d_gravity[k]>threshold)? 1:0;
						motion_changes_gyro[k] = (SMA2_1d_gyro[k]>threshold)? 1:0;
						motion_changes_north[k] = (SMA2_1d_north[k]>threshold)? 1:0;
					}
	
					output_labeled_body_motion_files[i] << "," << SMA1_1d_gravity[0]
														<< "," << SMA1_1d_gyro[0]
														<< "," << SMA1_1d_north[0]
														<< "," << SMA1_1d_gravity[1]
														<< "," << SMA1_1d_gyro[1]
														<< "," << SMA1_1d_north[1]
														<< "," << SMA1_1d_gravity[2]
														<< "," << SMA1_1d_gyro[2]
														<< "," << SMA1_1d_north[2]
														<< "," << SMA1_1d_gravity[3]
														<< "," << SMA1_1d_gyro[3]
														<< "," << SMA1_1d_north[3]/////
														<< "," << SMA2_1d_gravity[0]
														<< "," << SMA2_1d_gyro[0]
														<< "," << SMA2_1d_north[0]
														<< "," << SMA2_1d_gravity[1]
														<< "," << SMA2_1d_gyro[1]
														<< "," << SMA2_1d_north[1]
														<< "," << SMA2_1d_gravity[2]
														<< "," << SMA2_1d_gyro[2]
														<< "," << SMA2_1d_north[2]
														<< "," << SMA2_1d_gravity[3]
														<< "," << SMA2_1d_gyro[3]
														<< "," << SMA2_1d_north[3]/////
														<< "," << motion_changes_gravity[0]
														<< "," << motion_changes_gyro[0]
														<< "," << motion_changes_north[0]
														<< "," << motion_changes_gravity[1]
														<< "," << motion_changes_gyro[1]
														<< "," << motion_changes_north[1]
														<< "," << motion_changes_gravity[2]
														<< "," << motion_changes_gyro[2]
														<< "," << motion_changes_north[2]
														<< "," << motion_changes_gravity[3]
														<< "," << motion_changes_gyro[3]
														<< "," << motion_changes_north[3]/////
														<< endl;

					//printf("[BM] timestamp=%s, motion_changes_north[3]=%s\n\n", timestamp, motion_changes_north[3]);

				}

				pre_grativty_x[3]=atof(gravity_x); 
				pre_grativty_y[3]=atof(gravity_y); 
				pre_grativty_z[3]=atof(gravity_z); 

				pre_gyro_x[3]=atof(gyro_x); 
				pre_gyro_y[3]=atof(gyro_y); 
				pre_gyro_z[3]=atof(gyro_z); 

				pre_north_x[3]=atof(north_x); 
				pre_north_y[3]=atof(north_y); 
				pre_north_z[3]=atof(north_z); 	

				for(int k=0; k<4; k++)
				{
					// for 1st SMA (Simple Moving Average - (x_i-2+x_i-1+x_i)/3)  --- [1] - two rows before, [0] - one row before
					pre_1d_gravity[1][k]=pre_1d_gravity[0][k];
					pre_1d_gyro[1][k]=pre_1d_gyro[0][k];
					pre_1d_north[1][k]=pre_1d_north[0][k];

					pre_1d_gravity[0][k]=first_d_gravity[k];
					pre_1d_gyro[0][k]=first_d_gyro[k];
					pre_1d_north[0][k]=first_d_north[k];

					// for 2nd SMA (Simple Moving Average - (x_i-2+x_i-1+x_i)/3) --- [1] - two rows before, [0] - one row before
					near_SMA1_1d_gravity[1][k]=near_SMA1_1d_gravity[0][k]; 
					near_SMA1_1d_gyro[1][k]=near_SMA1_1d_gyro[0][k]; 
					near_SMA1_1d_north[1][k]=near_SMA1_1d_north[0][k];

					near_SMA1_1d_gravity[0][k]=SMA1_1d_gravity[k]; 
					near_SMA1_1d_gyro[0][k]=SMA1_1d_gyro[k]; 
					near_SMA1_1d_north[0][k]=SMA1_1d_north[k];
				}


				pre_unixtime=atof(unix_time);
				//printf("pre_unixtime %f = atof(%f)\n", pre_unixtime, atof(unix_time));
			}	*/		
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

		//2 - driver activity
		input_driver_activity_files[i].close(); input_driver_activity_files[i].clear(); printf("%s is closed \n", fname_driver_activity[i]);

		//3 - raw sensor data
		input_raw_sensor_data_files[i].close(); input_raw_sensor_data_files[i].clear(); printf("%s is closed \n", fname_raw_sensor_data_files[i]);	

		//4 - labelled sensor data
		output_labeled_sensor_data_files[i].close(); output_labeled_sensor_data_files[i].clear(); printf("%s is closed \n", fname_labeled_sensor_data_files[i]); 
		
		//5 - labelled body motion file
		//output_labeled_body_motion_files[i].close(); output_labeled_body_motion_files[i].clear(); printf("%s is closed \n", fname_labeled_body_motion_files[i]); 
	}

	return 0;
}

