// OBD_parser.cpp : Defines the entry point for the console application.
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

(3) OBD Data Streams (three files per each subject)

	File - sb13_OBD.csv (60 columns, delimiter - ',')

	GPS Time, Device Time, Longitude, Latitude,GPS Speed (Meters/second), Horizontal Dilution of Precision, Altitude, Bearing, G(x), G(y), G(z), G(calibrated),Acceleration Sensor(Total)(g),Acceleration Sensor(Y axis)(g),Acceleration Sensor(X axis)(g),Acceleration Sensor(Z axis)(g),Average trip speed(whilst stopped or moving)(km/h),Air Fuel Ratio(Measured)(:1),Barometer (on Android device)(mb),Distance to empty (Estimated)(km),Distance travelled with MIL/CEL lit(km),Engine Coolant Temperature(°C),Engine Load(%),Fuel Remaining (Calculated from vehicle profile)(%),Engine RPM(rpm),Fuel cost (trip)(cost),Fuel flow rate/hour(gal/hr),Fuel flow rate/minute(gal/min),Fuel used (trip)(gal),GPS Altitude(m),Fuel Trim Bank 1 Long Term(%),Fuel trim bank 1 sensor 2(%),GPS Accuracy(m),Fuel Trim Bank 1 Short Term(%),GPS Bearing(?,GPS vs OBD Speed difference(km/h),Voltage (Control Module)(V),Mass Air Flow Rate(g/s),Intake Air Temperature(°C),Intake Manifold Pressure(psi),O2 Sensor1 Equivalence Ratio(alternate),Trip Distance(km),Trip distance (stored in vehicle profile)(km),Engine kW (At the wheels)(kW),Horsepower (At the wheels)(hp),Kilometers Per Litre(Long Term Average)(kpl),Litres Per 100 Kilometer(Long Term Average)(l/100km),Miles Per Gallon(Long Term Average)(mpg),Torque(Nm),Trip Time(Since journey start)(s),Trip time(whilst moving)(s),Trip time(whilst stationary)(s),Speed (OBD)(km/h),Absolute Throttle Position B(%),Accelerator PedalPosition D(%),Accelerator PedalPosition E(%),Accelerator PedalPosition F(%),CO? in g/km (Average)(g/km),Fuel Level (From Engine ECU)(%),Relative Throttle Position(%),Relative Accelerator Pedal Position(%),Throttle Position(Manifold)(%)
	Thu Dec 19 13:15:44 EST 2013,19-Dec-2013 13:22:48.242,-79.94609866,40.44401488,0,5,323,0,-9.59,-0.18,-0.7,0,-0.02,-0.02,-0.98,-0.17,-,-,985.95,-,-,-,-,-,-,-,-,-,-,-,-,-,5,-,0,-,-,-,-,-,-,-,862.38,-,-,15.14,6.61,35.61,-,-,-,-,-,-,-,-,-,-,-,-,-,-
	Thu Dec 19 13:15:44 EST 2013,19-Dec-2013 13:22:49.630,-79.94609866,40.44401488,0,5,323,0,-9.36,-1.35,-0.08,-0.02,-0.07,-0.09,-0.92,-0.16,14,-,985.91,-,-,76,-,-,-,-,-,-,-,-,-,-,5,-,0,14,-,-,38,-,-,-,862.38,-,-,15.14,6.61,35.61,-,0,0.96,0,14,-,-,-,-,-,-,-,-,-
	Thu Dec 19 13:22:56 EST 2013,19-Dec-2013 13:22:56.862,-79.94592183,40.44419495,2.75,17,320,357.2,-9.79,0.24,-0.36,0.02,0.02,0.02,-1,-0.14,14,-,985.97,-,0,76,13.73,-,1732.5,-,-,-,-,320,-4.69,-,17,0,357.2,14,-,-,38,2.76,-,0.02,862.39,-,-,15.14,6.61,35.61,-,8,8.16,0,14,-,-,-,-,-,-,-,-,17.65
	Thu Dec 19 13:23:02 EST 2013,19-Dec-2013 13:23:03.191,-79.94575069,40.44422409,2.75,15,310,81.1,-9.47,-0.33,-0.82,-0.01,-0.06,-0.02,-0.94,-0.15,12.67,-,985.97,-,0,77,35.69,-,1233.75,-,-,-,-,310,-4.69,-,15,6.25,81.1,0.1,-,-,38,5.8,-,0.03,862.41,0.06,0.08,12.6,7.93,35.61,0.43,14,14.52,0,10,-,-,-,-,-,-,-,-,21.18
		:

(4) Labeled Output Files (e.g., [labeled]sb02_MYEI.txt, [labeled]sb02_MYEI.txt, [labeled]sb03_MYEI.txt,,,)

	[labeled] sb13_OBD.csv

		[0]		Subject_ID
		[1]		Session_ID

		[2]		sensor data 1 - GPS Time
		[3]		sensor data 2 - *Timestamp (Device Time)
		[4]		sensor data 3 - Longitude
		[5]		sensor data 4 - Latitude
		[6]		sensor data 5 - GPS Speed (Meters/second)
		 :			:				:
		[59]	sensor data 58 - Relative Throttle Position(%),
		[60]	sensor data 59 - Relative Accelerator Pedal Position(%)
		[61]	sensor data 60 - Throttle Position(Manifold)(%)

		[62]	Hand_move_detected
		[63]	Specific_interaction
		[64]	Hand_OnOff_Wheel
		[65]	Traffic_Opposite
		[66]	Traffic_Left_lane
		[67]	Traffic_my_lane
		[68]	Traffic_right_lane

*/


// CHECK! - if 'session_num' is not bounded as comments, the code is for sb11 and sb 05 in the SEPARATE group. 
// In this case, you will need to deactivate handling for sb11 and sb05, and activate original parts (if necessary)
// Even you are working with sb11 and sb05, you have to switch the valued in 'separate_driving_date' appropriately.


#include "stdafx.h"

#include "windows.h"
#include <fstream>
#include <iostream>
#include <string>
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

#define N_COL_RAW_SENSOR_DATA 62

#define NONE_PI 0
#define PI_INTERVAL 1
#define PRE_PI_INTERVAL 2
#define POST_PI_INTERVAL 3

using namespace std; // must exist for file open

ifstream input_session_timetable_file;									// session_time_table_new.csv
ifstream input_driver_activity_files[N_SUBJECTS];						// sb01_activities.csv

ifstream input_raw_sensor_data_files[N_SUBJECTS];		

ofstream output_labeled_sensor_data_files[N_SUBJECTS];	

char my_sensor_timestamp[DATA_SIZE]="My_Null";

static bool compare_timestamps(char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[]);
static int detect_PI_moments(char sensor_timestamp[], char PI_timestamp[]);

bool compare_timestamps(char sensor_timestamp[], char session_datestamp[], char session_start_timestamp[], char session_end_timestamp[])
{
	char year[2][10], month[2][10], date[2][10], hour[3][10], minute[3][10], second[3][10], millisecond[3][10];
	char zero[10]={"0"};
	
	int sensor=0, session_start=1, session_end=2;
	int sensor_digit=0, session_start_digit=0, session_end_digit=0;
		
	// OBD sensor timestamp - "13:11:16.444"

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
	char fname_raw_sensor_data_files[N_SUBJECTS][FNAME_LENGTH];
	char fname_labeled_sensor_data_files[N_SUBJECTS][FNAME_LENGTH];

	//sprintf(fname_session_timetable,"session_time_table_new.csv");
	//sprintf(fname_session_timetable,"session_time_table_new_24to26.csv");

	char subject_id_s[5]="NULL";
    char first_subject_id[5]="17";//"24";

	// FOR SEPARATE GROUP - from here
	/*sb05*///char separate_driving_date[2][50]={"2013-11-01","2013-12-23"}; // sb05 - {part1, part2} in MYEI file date format
	/*sb11*///char separate_driving_date[2][50]={"2013-11-18","2014-01-08"};/*should be updated"2014/01/08"*///}; // sb11 - {part1, part2} in MYEI file date format
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
		sprintf(fname_raw_sensor_data_files[i],"sb%s_OBD.csv",subject_id_s);
		//sprintf(fname_labeled_sensor_data_files[i],"[labeled]sb%s_OBD.csv",subject_id_s);
		sprintf(fname_labeled_sensor_data_files[i],"[parsed]sb%s_OBD.csv",subject_id_s);
						
		input_driver_activity_files[i].open(fname_driver_activity[i]);
		if(!input_driver_activity_files[i]) {cerr<< fname_driver_activity[i] << "<-- Unable to open this Driver Activity file. \n";}
		else{printf("%s is opened\n", fname_driver_activity[i]);}

		input_raw_sensor_data_files[i].open(fname_raw_sensor_data_files[i]);
		if(!input_raw_sensor_data_files[i]) {cerr<< fname_raw_sensor_data_files[i] << "<-- Unable to open this Raw Sensor data file. \n";}
		else{printf("%s is opened\n", fname_raw_sensor_data_files[i]);}

		output_labeled_sensor_data_files[i].open(fname_labeled_sensor_data_files[i]);
		if(!output_labeled_sensor_data_files[i]) {cerr<< fname_labeled_sensor_data_files[i] << "<-- Unable to create the [Labeled] Sensor data file. \n";}
		else{printf("%s is opened\n", fname_labeled_sensor_data_files[i]);}		
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
	char data_OBD[N_COL_RAW_SENSOR_DATA][DATA_SIZE];
	char pre_data_OBD[N_COL_RAW_SENSOR_DATA][DATA_SIZE];
	char pre_pre_data_OBD[N_COL_RAW_SENSOR_DATA][DATA_SIZE];

	char data_OBD_1std[N_COL_RAW_SENSOR_DATA][DATA_SIZE];

	char pre_longitude[N_COL_RAW_SENSOR_DATA][DATA_SIZE];
	char pre_pre_longitude[N_COL_RAW_SENSOR_DATA][DATA_SIZE];

	// Scan row by row
	char scanned_session_string[256];
	char scanned_sensor_stream[256];
	char scanned_PI_activity_string[256];

	// Re-tabulating with more features 
	char UTC_timestamp[30]="NULL";

	char UTC_year[15]="Null_UTCyear";
	char UTC_month[15]="Null_UTCmonth";
	char UTC_date[15]="Null_UTCdate";
	char UTC_hh[15]="Null_UTChh";
	char UTC_mm[15]="Null_UTCmm";
	char UTC_ss[15]="Null_UTCss";
	char UTC_mili[15]="Null_UTCmili";
	
	char pre_UTC_hh[15]="Null_preUTChh";
	char pre_UTC_mm[15]="Null_preUTCmm";
	char pre_UTC_ss[15]="Null_preUTCss";

	double time_diff=0.0;

	/* PENDING
	bool b_need_null_filling=false;
	char pre_UTC_ss[15]="Null_preUTCss";*/

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

		printf("i=%d : %s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", i, STT_subject_id[i], STT_exp_type[i], 
		p1_date[i], p1_preshaking_startT[i], p1_preshaking_endT[i], p1_baseline_startT[i], p1_baseline_endT[i], p1_driving_startT[i], p1_driving_sigstopT[i], p1_driving_restartT[i], p1_driving_endT[i], p1_postshaking_startT[i], p1_postshaking_endT[i],
		p2_date[i], p2_preshaking_startT[i], p2_preshaking_endT[i], p2_baseline_startT[i], p2_baseline_endT[i], p2_driving_startT[i], p2_driving_sigstopT[i], p2_driving_restartT[i], p2_driving_endT[i], p2_postshaking_startT[i], p2_postshaking_endT[i]);

		i++;
		//cin.get();
	}
	
	// Printing output files while reading raw sensor data stream files.

	for(i=0; i<N_SUBJECTS; i++)
	{		
		// Write in the output file

		/*output_labeled_sensor_data_files[i] << "subject" << "," <<	"session" << "," 
				<< "GPSTime" << "," << "DeviceTime" << "," << "Longitude" << "," << "Latitude" << ","
				<< "GPSSpeed(Meters/second)" << "," << "HorizontalDilutionofPrecision" << "," << "Altitude" << "," << "Bearing" << ","
				<< "G(x)" << "," << "G(y)" << "," << "G(z)" << "," << "G(calibrated)" << ","
				<< "AccelerationSensor(Total)(g)" << "," << "AccelerationSensor(Yaxis)(g)" << "," << "AccelerationSensor(Xaxis)(g)" << "," << "AccelerationSensor(Zaxis)(g)" << ","			
				<< "AverageTripSpeed(WhilstStoppedorMoving)(km/h)" << "," << "AirFuelRatio(Measured)(:1)" << "," << "Barometer(onAndroiddevice)(mb)" << "," << "Distancetoempty(Estimated)(km)" << ","			
				<< "DistancetravelledwithMIL/CELlit(km)" << "," << "EngineCoolantTemperature" << "," << "EngineLoad" << "," << "FuelRemaining(Calculatedfromvehicleprofile)" << ","			
				<< "EngineRPM(rpm)" << "," << "Fuelcost(trip)(cost)" << "," << "Fuelflowrate/hour(gal/hr)" << "," << "Fuelflowrate/minute(gal/min)" << ","			
				<< "Fuelused(trip)(gal)" << "," << "GPSAltitude(m)" << "," << "FuelTrimBank1LongTerm(%)" << "," << "Fueltrimbank1sensor2(%)" << ","			
				<< "GPSAccuracy(m)" << "," << "FuelTrimBank1ShortTerm(%)" << "," << "GPSBearing" << "," << "GPSvsOBDSpeeddifference(km/h)" << "," << "Voltage(ControlModule)(V)" << ","			
				<< "MassAirFlowRate(g/s)" << "," << "IntakeAirTemperature" << "," << "IntakeManifoldPressure(psi)" << "," << "O2Sensor1EquivalenceRatio(alternate)" << ","			
				<< "TripDistance(km)" << "," << "Tripdistance(storedinvehicleprofile)(km)" << "," << "EnginekW(Atthewheels)(kW)" << "," << "Horsepower(Atthewheels)(hp)" << ","			
				<< "KilometersPerLitre(LongTermAverage)(kpl)" << "," << "LitresPer100Kilometer(LongTermAverage)(l/100km)" << "," << "MilesPerGallon(LongTermAverage)(mpg)" << "," << "Torque(Nm)" << ","			
				<< "TripTime(Sincejourneystart)(s)" << "," << "Triptime(whilst moving)(s)" << "," << "Triptime(whilststationary)(s)" << "," << "Speed(OBD)(km/h)" << ","			
				<< "AbsoluteThrottlePositionB(%)" << "," << "AbsoluteThrottlePositionD(%)" << "," << "AbsoluteThrottlePositionE(%)" << "," << "AbsoluteThrottlePositionF(%)" << ","			
				<< "CO2ing/km(Average)(g/km)" << "," << "FuelLevel(FromEngineECU)(%)" << "," << "RelativeThrottlePosition(%)" << "," << "RelativeAcceleratorPedalPosition(%)" << ","			
				<< "ThrottlePosition(Manifold)(%)" << "," 						
				<< "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
				<< "\n";*/
		output_labeled_sensor_data_files[i] << "subject" << "," <<	"session" << "," 
				<< "New_DeviceTime" << "," <<"Delta_t" << ","
				<< "Longitude" << "," << "Longitude_1std" << ","										//[2]
				<< "Latitude" << "," << "Latitude_1std" << "," << "curvedness_feature" << "," << "centrifugal_feature" << ","			//[3]
				<< "GPSSpeed(Meterspersecond)" << "," << "GPSSpeed(Meterspersecond)_1std" << "," 
				<< "HorizontalDilutionofPrecision" << "," << "HorizontalDilutionofPrecision_1std" << "," 
				<< "Altitude" << "," << "Altitude_1std" << "," << "slope_type" << "," << "slope_level" << "," << "slope_type_N_level" << ","	//[6]
				<< "Bearing" << "," << "Bearing_1std" << ","
				<< "G(x)" << "," << "G(x)_1std" << "," 
				<< "G(y)" << "," << "G(y)_1std" << "," 
				<< "G(z)" << "," << "G(z)_1std" << "," 
				<< "G(calibrated)" << "," << "G(calibrated)_1std" << ","
				<< "AccelerationSensor(Total)(g)" << "," << "AccelerationSensor(Total)(g)_1std" << "," 
				<< "AccelerationSensor(Yaxis)(g)" << "," << "AccelerationSensor(Yaxis)(g)_1std" << "," 
				<< "AccelerationSensor(Xaxis)(g)" << "," << "AccelerationSensor(Xaxis)(g)_1std" << "," 
				<< "AccelerationSensor(Zaxis)(g)" << "," << "AccelerationSensor(Zaxis)(g)_1std" << ","		
				<< "AverageTripSpeed(WhilstStoppedorMoving)(kmperh)" << "," << "AverageTripSpeed(WhilstStoppedorMoving)(kmperh)_1std" << "," 
				<< "AirFuelRatio(Measured)(ratioto1)" << "," << "AirFuelRatio(Measured)(ratioto1)_1std" << "," 
				<< "Barometer(onAndroiddevice)(mb)" << "," << "Barometer(onAndroiddevice)(mb)_1std" << "," 
				<< "Distancetoempty(Estimated)(km)" << "," << "Distancetoempty(Estimated)(km)_1std" << ","				
				<< "DistancetravelledwithMILperCELlit(km)" << "," << "DistancetravelledwithMILperCELlit(km)_1std" << "," 
				<< "EngineCoolantTemperature" << "," << "EngineCoolantTemperature_1std" << "," 
				<< "EngineLoad" << "," << "EngineLoad_1std" << "," 
				<< "FuelRemaining(Calculatedfromvehicleprofile)" << ","	<< "FuelRemaining(Calculatedfromvehicleprofile)_1std" << ","					
				<< "EngineRPM(rpm)" << "," << "EngineRPM(rpm)_1std" << "," 
				<< "Fuelcost(trip)(cost)" << "," << "Fuelcost(trip)(cost)_1std" << "," 
				<< "Fuelflowrateperhour(galperhr)" << "," << "Fuelflowrateperhour(galperhr)_1std" << "," 
				<< "Fuelflowrateperminute(galpermin)" << "," << "Fuelflowrateperminute(galpermin)_1std" << ","				
				<< "Fuelused(trip)(gal)" << "," << "Fuelused(trip)(gal)_1std" << "," 
				<< "GPSAltitude(m)" << "," << "GPSAltitude(m)_1std" << "," 
				<< "FuelTrimBank1LongTerm(percent)" << "," << "FuelTrimBank1LongTerm(percent)_1std" << "," 
				<< "Fueltrimbank1sensor2(percent)" << ","	<< "Fueltrimbank1sensor2(percent)_1std" << ","			
				<< "GPSAccuracy(m)" << "," << "GPSAccuracy(m)_1std" << "," 
				<< "FuelTrimBank1ShortTerm(percent)" << "," << "FuelTrimBank1ShortTerm(percent)_1std" << "," 
				<< "GPSBearing" << "," << "GPSBearing_1std" << "," 
				<< "GPSvsOBDSpeeddifference(kmperh)" << "," << "GPSvsOBDSpeeddifference(kmperh)_1std" << "," 
				<< "Voltage(ControlModule)(V)" << "," << "Voltage(ControlModule)(V)_1std" << ","				
				<< "MassAirFlowRate(gpers)" << "," << "MassAirFlowRate(gpers)_1std" << "," 
				<< "IntakeAirTemperature" << "," << "IntakeAirTemperature_1std" << "," 
				<< "IntakeManifoldPressure(psi)" << "," << "IntakeManifoldPressure(psi)_1std" << "," 
				<< "O2Sensor1EquivalenceRatio(alternate)" << "," << "O2Sensor1EquivalenceRatio(alternate)_1std" << ","			
				<< "TripDistance(km)" << "," << "TripDistance(km)_1std" << "," 
				<< "Tripdistance(storedinvehicleprofile)(km)" << "," << "Tripdistance(storedinvehicleprofile)(km)_1std" << "," 
				<< "EnginekW(Atthewheels)(kW)" << "," << "EnginekW(Atthewheels)(kW)_1std" << "," 
				<< "Horsepower(Atthewheels)(hp)" << ","	<< "Horsepower(Atthewheels)(hp)_1std" << ","			
				<< "KilometersPerLitre(LongTermAverage)(kpl)" << "," << "KilometersPerLitre(LongTermAverage)(kpl)_1std" << "," 
				<< "LitresPer100Kilometer(LongTermAverage)(lper100km)" << "," << "LitresPer100Kilometer(LongTermAverage)(lper100km)_1std" << "," 
				<< "MilesPerGallon(LongTermAverage)(mpg)" << "," << "MilesPerGallon(LongTermAverage)(mpg)_1std" << "," 
				<< "Torque(Nm)" << ","	<< "Torque(Nm)_1std" << ","			
				<< "TripTime(Sincejourneystart)(s)" << "," << "TripTime(Sincejourneystart)(s)_1std" << "," 
				<< "Triptime(whilstmoving)(s)" << "," << "Triptime(whilstmoving)(s)_1std" << "," 
				<< "Triptime(whilststationary)(s)" << "," << "Triptime(whilststationary)(s)_1std" << "," 
				<< "Speed(OBD)(kmperh)" << "," << "Speed(OBD)(mperspow2)_1std" << ","	<< "accel_type" << "," << "accel_level" << "," << "accel_type_N_level" << ","		// [52]
				<< "AbsoluteThrottlePositionB(percent)" << "," << "AbsoluteThrottlePositionB(percent)_1std" << "," 
				<< "AbsoluteThrottlePositionD(percent)" << "," << "AbsoluteThrottlePositionD(percent)_1std" << "," 
				<< "AbsoluteThrottlePositionE(percent)" << "," << "AbsoluteThrottlePositionE(percent)_1std" << "," 
				<< "AbsoluteThrottlePositionF(percent)" << "," << "AbsoluteThrottlePositionF(percent)_1std" << ","			
				<< "CO2ingperkm(Average)(gperkm)" << "," << "CO2ingperkm(Average)(gperkm)_1std" << "," 
				<< "FuelLevel(FromEngineECU)(percent)" << "," << "FuelLevel(FromEngineECU)(percent)_1std" << "," 
				<< "RelativeThrottlePosition(percent)" << "," << "RelativeThrottlePosition(percent)_1std" << "," 
				<< "RelativeAcceleratorPedalPosition(percent)" << ","	<< "RelativeAcceleratorPedalPosition(percent)_1std" << ","			
				<< "ThrottlePosition(Manifold)(percent)" << "," << "ThrottlePosition(Manifold)(percent)_1std" //<< "," 							
				//<< "Hand_move_detected" << "," << "Specific_interaction" << "," << "Hand_OnOff_Wheel" << "," << "Traffic_Opposite" << "," << "Traffic_Left_lane" << "," << "Traffic_my_lane" << "," << "Traffic_right_lane"
				<< "\n";
								
		bool Used=true;
		char driver_PI_timestamp [50] = "Null"; // reformat just for the sub-function 'detect_PI_moments'
		char driver_PI_log[500]="Null,Null,Null,Null,Null,Null,Null";
				
		input_driver_activity_files[i] >> scanned_PI_activity_string;	// Skip the 1st 'title' row
		//printf("PI_string: %s \n", scanned_PI_activity_string);
		//cin.get();

		//input_raw_sensor_data_files[i] >> scanned_sensor_stream;		// Skip the 1st 'title' row

		for(int j=0; j<149; j++)
		{
			input_raw_sensor_data_files[i] >> scanned_sensor_stream;		// Skip the 1st 'title' row
		}
		//printf("OBD title_string: %s \n", scanned_sensor_stream);
		//cin.get();

		bool started_reading=false; // added this in order to prevent an error due to the last null row in the end of the file

		while(!input_raw_sensor_data_files[i].eof()) 
		{			
			// Sensor Data Streams 

			// added this in order to prevent an error due to the last null row in the end of the file
			if(started_reading==true)
			{
				if(pch == NULL) break;
			}
			started_reading=true;/////////////////

			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // Thu
			
			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // Dec ---- month

			if(strcmp(scanned_sensor_stream, "Jan")==0)
			{
				strcpy(UTC_month, "01");
			}else if(strcmp(scanned_sensor_stream, "Feb")==0)
			{
				strcpy(UTC_month, "02");
			}else if(strcmp(scanned_sensor_stream, "Mar")==0)
			{
				strcpy(UTC_month, "03");
			}else if(strcmp(scanned_sensor_stream, "Apr")==0)
			{
				strcpy(UTC_month, "04");
			}else if(strcmp(scanned_sensor_stream, "May")==0)
			{
				strcpy(UTC_month, "05");
			}else if(strcmp(scanned_sensor_stream, "Jun")==0)
			{
				strcpy(UTC_month, "06");
			}else if(strcmp(scanned_sensor_stream, "Jul")==0)
			{
				strcpy(UTC_month, "07");
			}else if(strcmp(scanned_sensor_stream, "Aug")==0)
			{
				strcpy(UTC_month, "08");
			}else if(strcmp(scanned_sensor_stream, "Sep")==0)
			{
				strcpy(UTC_month, "09");
			}else if(strcmp(scanned_sensor_stream, "Oct")==0)
			{
				strcpy(UTC_month, "10");
			}else if(strcmp(scanned_sensor_stream, "Nov")==0)
			{
				strcpy(UTC_month, "11");
			}else if(strcmp(scanned_sensor_stream, "Dec")==0)
			{
				strcpy(UTC_month, "12");
			}

			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // 19 ---- date
			strcpy(UTC_date, scanned_sensor_stream);

			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // 13:15:44
			strcpy(data_OBD[0], scanned_sensor_stream); // GPS time
			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // EST
			
			input_raw_sensor_data_files[i] >> scanned_sensor_stream; // 2013,19-Dec-2013	
			sscanf(scanned_sensor_stream,"%[^,],", UTC_year);
			
			//printf("%s-%s-%s", UTC_year, UTC_month, UTC_date);


			/*// Only related to the two subjects in the SEPARATE group
			char temp_datestamp[50];
			sprintf(temp_datestamp,"%s-%s-%s", UTC_year, UTC_month, UTC_date);

			if(strcmp(temp_datestamp,separate_driving_date[0])==0)
			{
				session_num=1;		// part 1 driving of the separate subject (either sb05 or sb11)
			}else if(strcmp(temp_datestamp,separate_driving_date[1])==0)
			{
				session_num=2;		// part 2 driving of the separate subject (either sb05 or sb11)
			}else
			{
				session_num=0;
			}*/

			/*printf("temp_datestamp=%s, separate_driving_date[0]=%s, separate_driving_date[1]=%s \n\n",temp_datestamp,separate_driving_date[0],separate_driving_date[1]);

			if(strcmp(UTC_year,"2014")==0){cin.get();}*/
			// up to here

			input_raw_sensor_data_files[i] >> scanned_sensor_stream; //	13:22:48.242,-79.94609866,40.44401488,... 	
			
			//printf("scanned_sensor_stream: %s \n", scanned_sensor_stream);
			//cin.get();

			pch = strtok (scanned_sensor_stream, ",");		// proceed until the delimter ','
			//if(pch == NULL) break;						// in order to take care of the last 'NULL' row - if there is no ',' to search, 'break' - Note that this checking must come before 'strcpy'
		
			strcpy(data_OBD[1], pch);	// Device Time
			//sscanf(data_OBD[1],"%s:%s:%s.%s", UTC_hh, UTC_mm, UTC_ss, UTC_mili);

			/* PENDING
			strcpy(pre_UTC_ss, UTC_ss); // store the previous second info to determine whether null_fillning will be needed or not.
			*/					

			strcpy(pre_UTC_hh, UTC_hh);
			strcpy(pre_UTC_mm, UTC_mm);
			strcpy(pre_UTC_ss, UTC_ss);
			sscanf(data_OBD[1],"%[^:]:%[^:]:%[^.].%s", UTC_hh, UTC_mm, UTC_ss, UTC_mili);
			//sprintf(UTC_timestamp,"%s-%s-%sT%s:%s:%s.%s-05:00",UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, UTC_ss, UTC_mili);

			sprintf(UTC_timestamp,"%s-%s-%sT%s:%s:%02d.000-05:00",UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, atoi(UTC_ss)+1);
			printf("Current data_OBD[1]: %s\n", data_OBD[1]);
			printf("Current UTC: %s\n", UTC_timestamp);

			time_diff=(double)((atoi(UTC_hh)*60.0*60.0+atoi(UTC_mm)*60.0+atoi(UTC_ss)+1.0)-(atoi(pre_UTC_hh)*60.0*60.0+atoi(pre_UTC_mm)*60.0+atoi(pre_UTC_ss)+1.0));

			if(time_diff<=1.0)
			{
				time_diff=1.0;
			}else if(time_diff>=999)
			{
				time_diff=999.0;
			}

			/* PENDING
			if(abs(atoi(UTC_ss)-atoi(pre_UTC_ss))!=1)
			{
				b_need_null_filling=true;
			}else
			{
				b_need_null_filling=false;
			}*/

			pch = strtok (NULL, ","); strcpy(data_OBD[2], pch);	// Longitude
			pch = strtok (NULL, ","); strcpy(data_OBD[3], pch);	// Latitude

			pch = strtok (NULL, ","); strcpy(data_OBD[4], pch);	// GPS Speed (Meters/second)
			pch = strtok (NULL, ","); strcpy(data_OBD[5], pch);	// Horizontal Dilution of Precision
			pch = strtok (NULL, ","); strcpy(data_OBD[6], pch);	// Altitude
			pch = strtok (NULL, ","); strcpy(data_OBD[7], pch);	// Bearing

			pch = strtok (NULL, ","); strcpy(data_OBD[8], pch);	// G(x)
			pch = strtok (NULL, ","); strcpy(data_OBD[9], pch);	// G(y)
			pch = strtok (NULL, ","); strcpy(data_OBD[10], pch);// G(z)
			pch = strtok (NULL, ","); strcpy(data_OBD[11], pch);// G(calibrated)

			pch = strtok (NULL, ","); strcpy(data_OBD[12], pch);// Acceleration Sensor(Total)(g)
			pch = strtok (NULL, ","); strcpy(data_OBD[13], pch);// Acceleration Sensor(Y axis)(g)
			pch = strtok (NULL, ","); strcpy(data_OBD[14], pch);// Acceleration Sensor(X axis)(g)
			pch = strtok (NULL, ","); strcpy(data_OBD[15], pch);// Acceleration Sensor(Z axis)(g)

			pch = strtok (NULL, ","); strcpy(data_OBD[16], pch);// Average trip speed(whilst stopped or moving)(km/h)
			pch = strtok (NULL, ","); strcpy(data_OBD[17], pch);// Air Fuel Ratio(Measured)(:1)
			pch = strtok (NULL, ","); strcpy(data_OBD[18], pch);// Barometer (on Android device)(mb)
			pch = strtok (NULL, ","); strcpy(data_OBD[19], pch);// Distance to empty (Estimated)(km)

			pch = strtok (NULL, ","); strcpy(data_OBD[20], pch);// Distance travelled with MIL/CEL lit(km)
			pch = strtok (NULL, ","); strcpy(data_OBD[21], pch);// Engine Coolant Temperature
			pch = strtok (NULL, ","); strcpy(data_OBD[22], pch);// Engine Load(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[23], pch);// Fuel Remaining (Calculated from vehicle profile)(%)
			
			pch = strtok (NULL, ","); strcpy(data_OBD[24], pch);// Engine RPM(rpm)
			pch = strtok (NULL, ","); strcpy(data_OBD[25], pch);// Fuel cost (trip)(cost)
			pch = strtok (NULL, ","); strcpy(data_OBD[26], pch);// Fuel flow rate/hour(gal/hr)
			pch = strtok (NULL, ","); strcpy(data_OBD[27], pch);// Fuel flow rate/minute(gal/min)

			pch = strtok (NULL, ","); strcpy(data_OBD[28], pch);// Fuel used (trip)(gal)
			pch = strtok (NULL, ","); strcpy(data_OBD[29], pch);// GPS Altitude(m)
			pch = strtok (NULL, ","); strcpy(data_OBD[30], pch);// Fuel Trim Bank 1 Long Term(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[31], pch);// Fuel trim bank 1 sensor 2(%)

			pch = strtok (NULL, ","); strcpy(data_OBD[32], pch);// GPS Accuracy(m)
			pch = strtok (NULL, ","); strcpy(data_OBD[33], pch);// Fuel Trim Bank 1 Short Term(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[34], pch);// GPS Bearing(?)
			pch = strtok (NULL, ","); strcpy(data_OBD[35], pch);// GPS vs OBD Speed difference(km/h)

			pch = strtok (NULL, ","); strcpy(data_OBD[36], pch);// Voltage (Control Module)(V)
			pch = strtok (NULL, ","); strcpy(data_OBD[37], pch);// Mass Air Flow Rate(g/s)
			pch = strtok (NULL, ","); strcpy(data_OBD[38], pch);// Intake Air Temperature
			pch = strtok (NULL, ","); strcpy(data_OBD[39], pch);// Intake Manifold Pressure(psi)

			pch = strtok (NULL, ","); strcpy(data_OBD[40], pch);// O2 Sensor1 Equivalence Ratio(alternate)			
			pch = strtok (NULL, ","); strcpy(data_OBD[41], pch);// Trip Distance(km)
			pch = strtok (NULL, ","); strcpy(data_OBD[42], pch);// Trip distance (stored in vehicle profile)(km)
			pch = strtok (NULL, ","); strcpy(data_OBD[43], pch);// Engine kW (At the wheels)(kW)

			pch = strtok (NULL, ","); strcpy(data_OBD[44], pch);// Horsepower (At the wheels)(hp)			 
			pch = strtok (NULL, ","); strcpy(data_OBD[45], pch);// Kilometers Per Litre(Long Term Average)(kpl)
			pch = strtok (NULL, ","); strcpy(data_OBD[46], pch);// Litres Per 100 Kilometer(Long Term Average)(l/100km)
			pch = strtok (NULL, ","); strcpy(data_OBD[47], pch);// Miles Per Gallon(Long Term Average)(mpg)

			pch = strtok (NULL, ","); strcpy(data_OBD[48], pch);// Torque(Nm)
			pch = strtok (NULL, ","); strcpy(data_OBD[49], pch);// Trip Time(Since journey start)(s)
			pch = strtok (NULL, ","); strcpy(data_OBD[50], pch);// Trip time(whilst moving)(s)
			pch = strtok (NULL, ","); strcpy(data_OBD[51], pch);// Trip time(whilst stationary)(s)

			pch = strtok (NULL, ","); strcpy(data_OBD[52], pch);// Speed (OBD)(km/h)
			pch = strtok (NULL, ","); strcpy(data_OBD[53], pch);// Absolute Throttle Position B(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[54], pch);// Accelerator PedalPosition D(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[55], pch);// Accelerator PedalPosition E(%)

			pch = strtok (NULL, ","); strcpy(data_OBD[56], pch);// Accelerator PedalPosition F(%)			
			pch = strtok (NULL, ","); strcpy(data_OBD[57], pch);// CO2 in g/km (Average)(g/km)
			pch = strtok (NULL, ","); strcpy(data_OBD[58], pch);// Fuel Level (From Engine ECU)(%)
			pch = strtok (NULL, ","); strcpy(data_OBD[59], pch);// Relative Throttle Position(%)

			pch = strtok (NULL, ","); strcpy(data_OBD[60], pch);// Relative Accelerator Pedal Position(%)			
			pch = strtok (NULL, "\n"); strcpy(data_OBD[61], pch);// Throttle Position(Manifold)(%)				
			
			//printf("OBD: %s=%s= ...,%s=%s\n", data_OBD[0], data_OBD[1], data_OBD[60], data_OBD[61]);
			//cin.get();

			// Compare timestamps in 'Sensor' and 'Session', and then get 'labeles' for 'session_id' of the output file 		
			char session_id[15] = "NULL_SESSION";
			char timestamp[50]= "NULL_TIMESTAMP";

			strcpy(timestamp, data_OBD[1]);	// Timestamp column ("Device Time")
		
			//original code
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
			}		//original code	*/		
						
			/*// Only for the two subjects in the SEPARATE group
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
			}///////////////////////////////////////// up to here (SEPARATE group)*/

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

			// PRINTING OUTPUTS
			//output_labeled_sensor_data_files[i] << STT_subject_id[i] << "," << session_id << ",";
			
			/* PENDING
			char temp_UTC_time_tofill[30]="Null_UTCtofill";

			if(b_need_null_filling==true)
			{
				for(int ii=0; ii<atoi(UTC_ss)-atoi(pre_UTC_ss)-1; ii++)
				{
					if(atoi(UTC_ss)+1+ii>=60)
					{
						strcpy(UTC_mm, itoa(atoi(UTC_mm)+1));
						strcpy(UTC_ss,

						if(atoi(UTC_mm)>=60)
						{
						}
					}

					sprintf(temp_UTC_time_tofill,"%s-%s-%sT%s:%s:%02d.000-05:00",UTC_year, UTC_month, UTC_date, UTC_hh, UTC_mm, atoi(UTC_ss)+1+ii);
					output_labeled_sensor_data_files[i] << STT_subject_id[i] << "," << pre_session_id << "," << temp_UTC_time_tofill << ",";
				}
			}*/

			output_labeled_sensor_data_files[i] << STT_subject_id[i] << "," << session_id << "," << UTC_timestamp << ",";

			output_labeled_sensor_data_files[i] << time_diff << ",";

			//for(int j=0; j<N_COL_RAW_SENSOR_DATA; j++)
			for(int j=2; j<N_COL_RAW_SENSOR_DATA; j++)
			{	
				if(strcmp(data_OBD[j],"-")==0)
				{
					strcpy(data_OBD[j],"99999999");
				}

				if(strcmp(pre_data_OBD[j],"99999999")==0)
				{
					strcpy(data_OBD_1std[j],data_OBD[j]);
				}else
				{
					sprintf(data_OBD_1std[j],"%f", (atof(data_OBD[j])-atof(pre_data_OBD[j]))/time_diff);
				}
				//output_labeled_sensor_data_files[i] << data_OBD[j] << ","; 		
				if(j==52)// OBD speed (km/h)
				{
					output_labeled_sensor_data_files[i] << data_OBD[j] << "," << atof(data_OBD_1std[j])*1000.0/60.0/60.0 << ","; 	
					
					char accel_type[20]="null_accel_type";
					char accel_level[20]="null_accel_level";

					if(atof(data_OBD_1std[j])*1000.0/60.0/60.0==0)
					{
						strcpy(accel_type,"ConstatndSpeed");						
					}else if(atof(data_OBD_1std[j])*1000.0/60.0/60.0>0)
					{
						strcpy(accel_type,"Acceleration");
					}else if(atof(data_OBD_1std[j])*1000.0/60.0/60.0<0)
					{
						strcpy(accel_type,"Deceleration");
					}

					if(fabs(atof(data_OBD_1std[j])*1000.0/60.0/60.0)<0.28) // less than 1km/h variation per sec
					{
						strcpy(accel_level,"Low");
					}else if(fabs(atof(data_OBD_1std[j])*1000.0/60.0/60.0)>=0.83)  // more than 3km/h variation per sec
					{
						strcpy(accel_level,"High");
					}else
					{
						strcpy(accel_level,"Medium");
					}

					output_labeled_sensor_data_files[i] << accel_type <<","<< accel_level <<"," << accel_type << "_" << accel_level <<",";

				}else
				{
					// Newly handling the last column [61]
					if(j!=61)
					{
						if(strcmp(data_OBD[j],"99999999")!=0)
						{
							output_labeled_sensor_data_files[i] << data_OBD[j] << "," << data_OBD_1std[j] << ","; 	
						}else
						{
							output_labeled_sensor_data_files[i] << "99999999" << "," << "99999999" << ","; 
						}
					}else
					{
						if(strcmp(data_OBD[j],"99999999")!=0)
						{
							output_labeled_sensor_data_files[i] << data_OBD[j] << "," << data_OBD_1std[j]; 
						}else
						{
							output_labeled_sensor_data_files[i] << "99999999" << "," << "99999999"; 
						}
					}
				}
				
				if(j==3) // Latitude column
				{
					//output_labeled_sensor_data_files[i] << data_OBD[j-1]<<"-2*"<<pre_longitude[j-1]<<"+"<<pre_pre_longitude[j-1] << ",";
					//output_labeled_sensor_data_files[i] << data_OBD[j]<<"-2*"<<pre_data_OBD[j]<<"+"<<pre_pre_data_OBD[j] << ",";

					// if the curvedeness is >=10, it might be just the first curvendenss of the record. it can be as large as latitude or longitute, rather than the deviations
					if((sqrt(pow(atof(data_OBD[j-1])-2.0*atof(pre_longitude[j-1])+atof(pre_pre_longitude[j-1]),2.0)+pow(atof(data_OBD[j])-2.0*atof(pre_data_OBD[j])+atof(pre_pre_data_OBD[j]),2.0))/time_diff)<10)
					{
						output_labeled_sensor_data_files[i] << sqrt(pow(atof(data_OBD[j-1])-2.0*atof(pre_longitude[j-1])+atof(pre_pre_longitude[j-1]),2.0)+pow(atof(data_OBD[j])-2.0*atof(pre_data_OBD[j])+atof(pre_pre_data_OBD[j]),2.0))/time_diff << ","; 	
						output_labeled_sensor_data_files[i] << pow(atof(data_OBD[52]),2.0)*(sqrt(pow(atof(data_OBD[j-1])-2.0*atof(pre_longitude[j-1])+atof(pre_pre_longitude[j-1]),2.0)+pow(atof(data_OBD[j])-2.0*atof(pre_data_OBD[j])+atof(pre_pre_data_OBD[j]),2.0))/time_diff) << ","; 	
					}else
					{
						output_labeled_sensor_data_files[i] <<"99999999,";
						output_labeled_sensor_data_files[i] <<"99999999,";
					}
				}

				if(j==6) // Altitude column
				{
					char hill_type[20]="null_hill_type";
					char hill_level[20]="null_hill_level";

					if(atof(data_OBD_1std[j])==0)
					{
						strcpy(hill_type,"Plain");						
					}else if(atof(data_OBD_1std[j])>0)
					{
						strcpy(hill_type,"Uphill");
					}else if(atof(data_OBD_1std[j])<0)
					{
						strcpy(hill_type,"Downhill");
					}

					if(fabs(atof(data_OBD_1std[j]))<0.5)
					{
						strcpy(hill_level,"Low");
					}else if(fabs(atof(data_OBD_1std[j]))>=1.5)
					{
						strcpy(hill_level,"High");
					}else
					{
						strcpy(hill_level,"Medium");
					}

					output_labeled_sensor_data_files[i] << hill_type <<","<< hill_level <<"," << hill_type << "_" << hill_level <<",";
				}

				if(j==2)
				{   // handling an issue relating overwritten pre- prepre- longitude, when calculating curvedness feature
					strcpy(pre_pre_longitude[j],pre_pre_data_OBD[j]);
					strcpy(pre_longitude[j],pre_data_OBD[j]);
				}

				strcpy(pre_pre_data_OBD[j],pre_data_OBD[j]);				
				strcpy(pre_data_OBD[j], data_OBD[j]); // THINK FROM HERE - Curvedness feature should be calculated beforeLongitude 
				
			}
			//output_labeled_sensor_data_files[i] << driver_PI_log << endl;
			//printf("driver_PI_log: %s \n\n",driver_PI_log);
			output_labeled_sensor_data_files[i] << endl;
		}		
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

		//2 - driver activity data
		input_driver_activity_files[i].close(); input_driver_activity_files[i].clear(); printf("%s is closed \n", fname_driver_activity[i]);

		//3 - raw sensor data
		input_raw_sensor_data_files[i].close(); input_raw_sensor_data_files[i].clear(); printf("%s is closed \n", fname_raw_sensor_data_files[i]);	

		//4 - labelled sensor data
		output_labeled_sensor_data_files[i].close(); output_labeled_sensor_data_files[i].clear(); printf("%s is closed \n", fname_labeled_sensor_data_files[i]); 

	}

	return 0;
}