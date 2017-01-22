/**
* @file       FCU_FCTRL_HOVERENGINES_CONTROL.C
* @brief      Hover Engines Control
* @author	  Alessandro Caratelli, Marek Gutt-Mostowy
* @copyright  rLoop Inc.
*/

/////////////////////////////////////////////
///////////  WORK IN PROGRESS  //////////////
/////////////////////////////////////////////

// TO DO:
// - Verification

 //Hoverengines handle
 //   void   vFCU_FLIGHTCTL_HOVERENGINES__Init(void);
 //   void   vFCU_FLIGHTCTL_HOVERENGINES__Process(void);
 //   void   vFCU_FLIGHTCTL_HOVERENGINES__start(void);
 //   void   vFCU_FLIGHTCTL_HOVERENGINES__stop(void);
 //   Lint16 vFCU_FLIGHTCTL_HOVERENGINES__Get_State(Lint8 u8Enable);


#include "../../fcu_core.h"

#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
#if C_LOCALDEF__LCCM655__ENABLE_FLIGHT_CONTROL == 1U
#if C_LOCALDEF__LCCM655__ENABLE_HOVERENGINES_CONTROL == 1U

extern struct _strFCU sFCU;

// TODO: need values for following constants and move to fcu_core_defines.h

/** Hover Engines Parameters */
#define C_FCU__HE_STATIC_HOVER_RPM						(2000U)   // hover engine nominal RPM speed
#define C_FCU__HE_CRUISE_RPM							(2000U)   // hover engine cruise RPM speed
#define C_FCU__HE_MAX_RPM_IN_HOVERING					(2500U)   // hover engine maximum allowed RPM speed douring hovering
#define C_FCU__HE_MIN_RPM_IN_HOVERING					(1500U)   // hover engine minimum allowed RPM speed douring hovering
#define C_FCU__HE_MIN_RPM_IN_STANDBY					(10U)     // hover engine OFF state tolerance
#define C_FCU__HE_MAX_CURRENT							(10U)     // hover engine maximum allowed current TO BE CHECKED IF EXISTS
#define C_FCU__HE_MIN_CURRENT							(1U)      // hover engine minimum allowed current
#define C_FCU__HE_MAX_TEMPERATURE						(95U)     // critical hover engine temperature
/** Pod Dynamics Parameters */
#define C_FCU__NAV_PODSPEED_STANDBY					(5U)      // Pod standby speed
#define C_FCU__NAV_PODSPEED_MAX_SPEED_TO_STABILIZE	(100000U) // max Pod speed to stabilize pod / ATM extra large value so that it's never reached and the hover engines don't throttle down


#if C_LOCALDEF__LCCM655__ENABLE_HOVERENGINES_CONTROL == 1U
struct
{
	/** The hover engines state machine */
	E_FCU_HOVERENGINES__STATES_T eState;

	/** The hover engines input commands from GS */
	E_FCU_HOVERENGINES__COMMANDS_T u16HoverEnginesCommands;

	/** The hover engines RMP values from GS */
	Lint32 u32HoverEnginesRPM_Commands;

	/** Internal parameters */
	struct
	{
		Luint8 u8Enable;
		Luint8 u8RunAuto;
		Luint8 u8SpeedState;
	
	} sIntParams

}sHoverengines
#endif

typedef enum
{
	HOVERENGINES_STATE__IDLE = 0U,
	HOVERENGINES_STATE__ENABLED,
	HOVERENGINES_STATE__START_STATIC_HOVERING,
	HOVERENGINES_STATE__STATIC_HOVERING

} E_FCU__HOVERENGINES__STATES_T;

typedef enum
{
	DO_NOTHING = 0U,
	STATIC_HOVERING,
	RELEASE_STATIC_HOVERING,
	M_SET_SPEED_HE1,
	M_SET_SPEED_HE2,
	M_SET_SPEED_HE3,
	M_SET_SPEED_HE4,
	M_SET_SPEED_HE5,
	M_SET_SPEED_HE6,
	M_SET_SPEED_HE7,
	M_SET_SPEED_HE8

} E_FCU__HOVERENGINES_COMMANDS;


// TODO: need the following functions:
// vFCU_COOLING__Set_Valve(ValveNumber, TimeOn, TimeOff);
// vFCU_COOLING__Set_Valve(ValveNumber, TimeOn, TimeOff);
// vFCU_COOLING__Set_Valve(ValveNumber, TimeOn, TimeOff); 
// vFCU_COOLING__Set_Valve(ValveNumber, TimeOn, TimeOff);
// u32FCU_FLIGHTCTL_NAV__PodSpeed();

void vFCU_FLIGHTCTL_HOVERENGINES__Init(void)
{

	sFCU.sHoverengines.sIntParams.u8Enable = 0U; // temporary variable to be used inside of the functions
	sFCU.sHoverengines.sIntParams.u8SpeedState = 0U; //TO BE LOOKED UP AGAIN
	sFCU.sHoverengines.sIntParams.u8RunAuto = 0U; // Flag to initiate flight mode
	sFCU.sHoverengines.sIntParams.u8TempVar = 0U; // Tempo variable used inside of the functions

	sFCU.sHoverengines.u16HoverenginesCommands = DO_NOTHING; // Set the commands from the ground station to DO_NOTHING at startup

	sFCU.sHoverengines.eState = HOVERENGINES_STATE__IDLE; // set the first state of the hover engines control state machine to IDLE

	vFCU_THROTTLE__Enable_Run(); // Enable the throttles state machine
}


void vFCU_FLIGHTCTL_HOVERENGINES__Process(void)
{
	Luint32 u32PodSpeed; // SHOULD COME FROM THE NAVIGATION FUNCTIONS
	Luint16 u16RPM[8]; // Used in order to avoid fetching from structure


	vFCU_FLIGHTCTL_HOVERENGINES__ManualCommandsHandle(); // Call the function with manual commands handling

	switch(sFCU.sHoverengines.eState)
	{

		case HOVERENGINES_STATE__IDLE:
			// if is received the "Enable hover engines" command or if is active the autonomous behaviour set by the TOD_DRIVE FSM
			if((sFCU.sHoverengines.u16HoverenginesCommands == STATIC_HOVERING) || (sFCU.sHoverengines.sHoverenginesIntParams.u8Enable == 1U && sFCU.sHoverengines.sHoverenginesIntParams.u8RunAuto == 1U))
			{
				// fetch the pod speed
				u32PodSpeed = vFCU__POD_SPEED();
				// if the pod speed is lower than the pod standby speed and the pod is lifted
				if((u32PodSpeed < PODSPEED_STANDBY) && (sFCU.sOpStates.u8Lifted != 0U))
				{
					// go to HOVERENGINES_STATE__ENABLED state
					sFCU.sHoverengines.sHoverenginesIntParams.u8TempVar = 0U;
					sFCU.sHoverengines.eState = HOVERENGINES_STATE__ENABLE_1TH_GROUP;
				}
			}
			break;


		case HOVERENGINES_STATE__ENABLE_1TH_GROUP:
			// this state enable the 2th group of Hover Engines
			Lint8 u8status;
			if(sFCU.sHoverengines.sHoverenginesIntParams.u8TempVar < 1U)
			{
				for(u8Counter = 1U; u8Counter < 8U; u8Counter++)
				{
					if(u8Counter == 1U || u8Counter == 2U || u8Counter == 5U || u8Counter == 6U)
					{
						// activate the cooling system for the 2th group of Hover Engines
						vFCU_COOLING__Set_Valve(u8Counter, 0.5, 1.5); // to be changed (this function is not yet implemented
						// linearly set the 2th group of Hover Engine RPM from 0 to hover engine nominal RPM
						vFCU_THROTTLE__Set_Throttle(u8Counter, HOVER_ENGINE_STATIC_HOVER_RPM, THROTTLE_TYPE__RAMP);
						sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = HOVER_ENGINE_STATIC_HOVER_RPM;
					}
				}
				sFCU.sHoverengines.sHoverenginesIntParams.u8TempVar = 1U;
			}
			// read the RPM value of the 1th group of Hover Engines
			// If all RPM reach the HOVER_ENGINE_STATIC_HOVER_RPM value with
			// a certain tolerance, than set the status flag to 1
			u8status = 1U;
			for(u8Counter = 1U; u8Counter < 8U; u8Counter++)
			{
				if(u8Counter == 1U || u8Counter == 2U || u8Counter == 5U || u8Counter == 6U)
				{

					Luint16 pu16Rpm = 0U;
					s16FCU_ASI__ReadMotorRpm(devIndex, &pu16Rpm);
					if(pu16Rpm < (HOVER_ENGINE_CRUISE_RPM - HOVER_ENGINE_RPM_TOLERANCE))
						u8status = 0U;
				}
			}

			// If all the four THROTTLEs reached the HOVER_ENGINE_STATIC_HOVER_RPM
			if(u8status > 0U)
			{
				// go to state HOVERENGINES_STATE__HOVERING
				sFCU.sHoverengines.eState = HOVERENGINES_STATE__ENABLE_2TH_GROUP;
			}
			break;


		case HOVERENGINES_STATE__ENABLE_2ND_GROUP:
			// this state enable the 2th group of Hover Engines
			Lint16 u8status;
			if(sFCU.sHoverengines.sHoverenginesIntParams.u8TempVar < 2U)
			{
				for(u8Counter = 1U; u8Counter < 8U; u8Counter++)
				{
					if(u8Counter == 3U || u8Counter == 4U || u8Counter == 7U || u8Counter == 8U)
					{
						// activate the cooling system for the 2th group of Hover Engines
						vFCU_COOLING__Set_Valve(u8Counter, 0.5, 1.5); // to be changed (this function is not yet implemented
						// linearly set the 2th group of Hover Engine RPM from 0 to hover engine nominal RPM
						vFCU_THROTTLE__Set_Throttle(u8Counter, HOVER_ENGINE_STATIC_HOVER_RPM, THROTTLE_TYPE__RAMP);
						sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = HOVER_ENGINE_STATIC_HOVER_RPM;
					}
				}
				sFCU.sHoverengines.sHoverenginesIntParams.u8TempVar = 2U;
			}

			// read the RPM value of the 1th group of Hover Engines
			// If all RPM reach the HOVER_ENGINE_STATIC_HOVER_RPM value with
			// a certain tolerance, than set the status flag to 1
			u8status = 1U;
			for(u8Counter = 1U; u8Counter < 8U; u8Counter++)
			{
				if(u8Counter == 3U || u8Counter == 4U || u8Counter == 7U || u8Counter == 8U)
				{
					Luint16 pu16Rpm = 0U;
					s16FCU_ASI__ReadMotorRpm(devIndex, &pu16Rpm);
					if(pu16Rpm < (HOVER_ENGINE_CRUISE_RPM - HOVER_ENGINE_RPM_TOLERANCE))
						u8status = 0U;
				}
			}
			// If all the four THROTTLEs reached the HOVER_ENGINE_STATIC_HOVER_RPM
			if(u8status > 0U)
			{
				// go to state HOVERENGINES_STATE__HOVERING
				sFCU.sHoverengines.eState = HOVERENGINES_STATE__HOVERING;
				sFCU.sHoverengines.sHoverenginesIntParams.u8Enable = 1U;
			}
			break;

		case HOVERENGINES_STATE__HOVERING:
			// In this state all Hover Engines are running at the STATIC_HOVER_RPM.
			Lint16 status1 = 0;
			Lint16 status2 = 0;
			Luint8 u8Counter = 0;

			// If running in Autonomous behavior mode during flight
			if(sFCU.sHoverengines.sHoverenginesIntParams.u8RunAuto == 1U) //Manage HE RPM during flight
			{
				for(u8Counter = 1; u8Counter < 8; u8Counter++) // VERIFY PARAMETERS
				{
					// RPM, Temperature and Current are monitored,
					// a fault is reported if those values goes out of the safety range.
					Luint16 pu16Rpm = 0;
					Luint16 pu16Current = 0;
					Luint16 pu16Voltage = 0;
					Luint16 pu16Temp = 0;
					Luint8  u8ErrorFlag = 0;
					//read hover engine RPM
					s16FCU_ASI_CTRL__ReadMotorRpm(u8Counter, &pu16Rpm);
					//read hover engine Current
					s16FCU_ASI_CTRL__ReadMotorCurrent(u8Counter, &pu16Current));
					//read hover engine Voltage
					s16FCU_ASI_CTRL__ReadMotorVoltage(u8Counter, &pu16Voltage));
					//read hover engine Temperature
					s16FCU_ASI_CTRL__ReadControllerTemperature(u8Counter, &pu16Temp);
					// verify Current range
					u8ErrorFlag = (pu16Current > HOVER_ENGINE_MAX_CURRENT) ? 1U : 0U;
					u8ErrorFlag = (pu16Current < HOVER_ENGINE_MIN_CURRENT) ? 1U : u8ErrorFlag;
					// verify voltage range
					u8ErrorFlag = (pu16Voltage > HOVER_ENGINE_MAX_VOLTAGE) ? 1U : u8ErrorFlag;
					u8ErrorFlag = (pu16Voltage < HOVER_ENGINE_MAX_VOLTAGE) ? 1U : u8ErrorFlag;
					// verify max temperature
					u8ErrorFlag = (pu16Temp > HOVER_ENGINE_MAX_TEMPERATURE) ? 1U : u8ErrorFlag;
					// verify RPM value
					u8ErrorFlag = (pu16Rpm > (sFCU.sHoverengines.HE_RPM_CommandValue + HOVER_ENGINE_RPM_TOLERANCE)) ? 1U : u8ErrorFlag;
					u8ErrorFlag = (pu16Rpm < (sFCU.sHoverengines.HE_RPM_CommandValue - HOVER_ENGINE_RPM_TOLERANCE)) ? 1U : u8ErrorFlag;

					if(u8ErrorFlag == 1U)
					{
						// set hover engine X RPM command and hover engine Y RPM command to 0,
						// where HEY is the hover engine symmetrically opposite to HEX with respect to pod center
						Luint8 u8SimmetricalIndex;
						u8SimmetricalIndex = (u8Counter < 5) ?  (u8Counter + 4) :  (u8Counter - 4);
						vFCU_THROTTLE__Set_Throttle(u8Counter, 0U, THROTTLE_TYPE__STEP);
						vFCU_THROTTLE__Set_Throttle(u8SimmetricalIndex, 0U, THROTTLE_TYPE__STEP);
						sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = 0U;
						sFCU.sHoverengines.HE_RPM_CommandValue[u8SimmetricalIndex] = 0U;
					}
				}
				// get pod speed
				u32PodSpeed = vFCU__POD_SPEED();
				// If the pod speed goes higher than the max speed to stabilize pod
				if((u32PodSpeed > PODSPEED_MAX_SPEED_TO_STABILIZE) && (sFCU.sHoverengines.sHoverenginesIntParams.u8SpeedState == 0U))
				{
					// avoid continusly send the command
					if(sFCU.sHoverengines.sHoverenginesIntParams.u8SpeedState == 0U)
					{
						for(u8Counter = 1; u8Counter < 8; u8Counter++)
						{
							//the HE RPM is reduced to hover engine cruise RPM
							sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = HOVER_ENGINE_CRUISE_RPM;
							vFCU_THROTTLE__Set_Throttle(u8Counter, HOVER_ENGINE_CRUISE_RPM, THROTTLE_TYPE__STEP);
						}
						sFCU.sHoverengines.sHoverenginesIntParams.u8SpeedState = 1U;
					}
				}
				// If the pod speed goes lower than the max speed to stabilize pod
				else
				{
					// avoid continusly send the command
					if(sFCU.sHoverengines.sHoverenginesIntParams.u8SpeedState == 1U)
					{
						for(u8Counter = 1; u8Counter < 8; u8Counter++)
						{
							sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = HOVER_ENGINE_STATIC_HOVER_RPM;
							vFCU_THROTTLE__Set_Throttle(u8Counter, HOVER_ENGINE_STATIC_HOVER_RPM, THROTTLE_TYPE__STEP);
						}
						sFCU.sHoverengines.sHoverenginesIntParams.u8SpeedState = 0U;
					}
				}
			} // end of if(sFCU.sHoverengines.sHoverenginesIntParams.u8RunAuto == 1U) //Manage HE RPM during flight

			// If is received the command to release the static hovering
			// or if the POD_DRIVE FSM disable the HE,
			if((sFCU.sHoverengines.u16HoverenginesCommands == RELEASE_STATIC_HOVERING) || (sFCU.sHoverengines.sHoverenginesIntParams.u8Enable == 0U))
			{
				// get pod speed
				u32PodSpeed = vFCU__POD_SPEED();
				if((sFCU.sOpStates.u8StaticHovering != 0U) && (u32PodSpeed < PODSPEED_STANDBY))
				{
					for(u8Counter = 1; u8Counter < 8; u8Counter++)
					{
						sFCU.sHoverengines.HE_RPM_CommandValue[u8Counter] = 0;
						vFCU_THROTTLE__Set_Throttle(u8Counter, 0, THROTTLE_TYPE__STEP);
						vFCU_COOLING__Set_Valve(u8Counter, 0.0, 2.0);
					}
				}
			}
			// switch to the idle state only when the HE RPM goes to 0 with a certain tolerance
			u8status = 1U;
			for(u8Counter = 1U; u8Counter < 8U; u8Counter++)
			{
					Luint16 pu16Rpm = 0U;
					s16FCU_ASI__ReadMotorRpm(devIndex, &pu16Rpm);
					if(pu16Rpm < (HOVER_ENGINE_CRUISE_RPM - HOVER_ENGINE_RPM_TOLERANCE))
						u8status = 0U;
			}

			// If all the four THROTTLEs reached the HOVER_ENGINE_STATIC_HOVER_RPM
			if(u8status != 0U)
			{
				// go to state IDLE
				sFCU.sHoverengines.eState = HOVERENGINES_STATE__IDLE;
				sFCU.sHoverengines.sHoverenginesIntParams.u8Enable = 0U;
				sFCU.sHoverengines.sHoverenginesIntParams.u8RunAuto = 0U;
			}
			break;
	} //switch(sFCU.sHoverengines.eState)

}

Luint16 u16FCU_FLIGHTCTL_HOVERENGINES__Get_State(void)
{
	return sFCU.sHoverengines.eState;
}

Luint16 u16FCU_FLIGHTCTL_HOVERENGINES__Get_FaultFlag(void){
	// TO DO
}

void vFCU_FLIGHTCTL_HOVERENGINES__enable(void)
{
	sFCU.sHoverengines.sIntParams.u8Enable = 1U;
}

void vFCU_FLIGHTCTL_HOVERENGINES__disable(void)
{
	sFCU.sHoverengines.sIntParams.u8RunAuto = 0U;
	sFCU.sHoverengines.sIntParams.u8Enable  = 0U;
}

void vFCU_FLIGHTCTL_HOVERENGINES__start(void)
{
	if(sFCU.sHoverengines.sIntParams.u8Enable != 0U)
		sFCU.sHoverengines.sIntParams.u8RunAuto = 1U;
}

void vFCU_FLIGHTCTL_HOVERENGINES__stop(void)
{
	sFCU.sHoverengines.sIntParams.u8RunAuto  = 0U;
}


void vFCU_FLIGHTCTL_HOVERENGINES__ManualCommandsHandle(void)
{
	Luint8 u8ManualControlActive;
	Luint32 u32PodSpeed;
	Luint32 u32GS_RPM = sFCU.sHoverengines.u32HoverenginesRPM_Commands;
	switch(sFCU.sHoverengines.u16HoverenginesCommands)
	{
		case M_SET_SPEED_HE1:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(1U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE2:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(2U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE3:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(3U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE4:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(4U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE5:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(5U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE6:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(6U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE7:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(7U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;

		case M_SET_SPEED_HE8:
			u32PodSpeed = u32FCU_FLIGHTCTL_NAV__POD_SPEED();
			if(u32PodSpeed < PODSPEED_STANDBY)
				vFCU_THROTTLE__Set_Throttle(8U, u32GS_RPM, THROTTLE_TYPE__STEP);
			break;
	}
}



#endif //C_LOCALDEF__LCCM655__ENABLE_HOVERENGINES_CONTROL
#ifndef C_LOCALDEF__LCCM655__ENABLE_HOVERENGINES_CONTROL
#error
#endif

#endif //C_LOCALDEF__LCCM655__ENABLE_FLIGHT_CONTROL
#ifndef C_LOCALDEF__LCCM655__ENABLE_FLIGHT_CONTROL
#error
#endif

#endif //#if C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE == 1U
//safetys
#ifndef C_LOCALDEF__LCCM655__ENABLE_THIS_MODULE
#error
#endif
