

#ifndef _RM57L843_LAUNCHPAD__BSP_H_
#define _RM57L843_LAUNCHPAD__BSP_H_

/*******************************************************************************
RM4 MAIN SYSTEM SETUP
Place this at the top of your Localdef file
Change the build settings:
1. Enable GCC extensions under advanced options -> language options
2. Enable unaligned loads and stores to on
3. Make sure designate enum type is packed.

*******************************************************************************/
	/*lint -e950 */
	/*lint -e621 */
	
	#include "sil3_generic__cpu.h"
	
	//data types
	#include <RM4/LCCM105__RM4__BASIC_TYPES/basic_types.h>


	//set this to 1 to disable all common startups as would be used by a bootloader
	#define C_LOCALDEF__SYSTEM__DISABLE_ALL_FOR_BOOTLOADER					(0U)

	//change osc freq to match the osc on your board
	//valid values are 10 and 16MHZ
	#define C_LOCALDEF__SYSTEM__OSCILLATOR_FREQ     						(16000000U)

	//This is the HCLK frequency, it is not defined here, but is a result of the
	//PLL1 setup.  Standard range is 200MHZ to 220MHZ (952 part)
	//Valid Is:
	//220
	//200 (Standard)
	//150 (Low Power) + RM57
	//100 (Low Power)
	#define C_LOCALDEF__SYSTEM__HCLK_FREQ_MHZ								(150U)

	//normal bus division is either 1 or 2
	//In 1x mode, VCLK1 is 100MHZ, in 2x mode VCLK is 50MHZ
	//you may only need 2x mode if you need ultra lo SPI comms, etc.
	#define C_LOCALDEF__SYSTEM__VCLK1_DIVISOR								(1U)


	//just here because of no EMIF
	//if ou are using EMIF, then remove this line
	#define C_LOCALDEF__LCCM107__EMIF_CLOCK_DIVISOR							(1U)


	//we have the ability to use the ECLK pin to generate a high frequency divided
	//clock signal, possibly faster than we can do with PWM
	#define C_LOCALDEF__SYSTEM__ENABLE_ECLK									(1U)

	//The divisor is VCLK1 /x so a value of 40x = 2.5MHZ ECLK
	#define C_LOCALDEF__SYSTEM__ECLK_DIVISOR								(2U)

	//other includes
	#include <RM4/LCCM219__RM4__SYSTEM/rm4_system.h>

	#if C_LOCALDEF__SIL3_GENERIC__CPU_TYPE__RM57L843 == 1U
		#include <RM57/LCCM704__RM57__COMMON/HL_system.h>
	#endif


	#include <RM4/LCCM364__RM4__FIRMWARE_VERSION/rm4_firmware_version.h>

/*******************************************************************************
RM4 CRC MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM226__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM226__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM226__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM226__RM4__CRC/rm4_crc.h>

	#endif //C_LOCALDEF__LCCM226__ENABLE_THIS_MODULE


/*******************************************************************************
BUILT IN SELF TEST MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM227__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM227__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM227__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM227__RM4__BIST/rm4_bist.h>

	#endif //C_LOCALDEF__LCCM227__ENABLE_THIS_MODULE



/*******************************************************************************
FLASH MODULE
Even if you don't want in system programming or firmware update features
include this to get access to the flash registers and setup features
*******************************************************************************/
	#define C_LOCALDEF__LCCM135__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM135__ENABLE_THIS_MODULE == 1U

		//enable F021
		#define C_LOCALDEF__LCCM135__ENABLE_F021							(1U)

		//testing options
		#define C_LOCALDEF__LCCM135__ENABLE_TEST_SPEC						(0U)

		#include <RM4/LCCM135__RM4__FLASH/rm4_flash.h>

	#endif //#if C_LOCALDEF__LCCM135__ENABLE_THIS_MODULE == 1U


/*******************************************************************************
RM4 DELAY ROUTINES
*******************************************************************************/
	#define C_LOCALDEF__LCCM228__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM228__ENABLE_THIS_MODULE == 1U

		//Testing options
		#define C_LOCALDEF__LCCM228__ENABLE_TEST_SPEC						(0U)

		//enable the PMU if needed
		#define C_LOCALDEF__LCCM288__ENABLE_PMU_FOR_DEBUG					(0U)

		//main include file
		#include <RM4/LCCM228__RM4__DELAYS/rm4_delays.h>

	#endif //C_LOCALDEF__LCCM228__ENABLE_THIS_MODULE
	

/*******************************************************************************
ERROR SIGNALLING MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM220__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM220__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM220__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM220__RM4__ERROR_SIGNAL_MODULE/rm4_esm.h>

	#endif //C_LOCALDEF__LCCM220__ENABLE_THIS_MODULE

/*******************************************************************************
RM57 - PERIPH CENTRAL RESOURCE
*******************************************************************************/
	#define C_LOCALDEF__LCCM708__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM708__ENABLE_THIS_MODULE == 1U

		/** Testing options */
		#define C_LOCALDEF__LCCM708__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM57/LCCM708__RM57__PERIPH_CENTRAL_RESOURCE/rm57_pcr.h>

	#endif //C_LOCALDEF__LCCM708__ENABLE_THIS_MODULE

/*******************************************************************************
RM4 DMA Module
*******************************************************************************/
	#define C_LOCALDEF__LCCM229__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM229__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM229__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM229__RM4__DMA/rm4_dma.h>

	#endif //C_LOCALDEF__LCCM229__ENABLE_THIS_MODULE

/*******************************************************************************
RM57 - PIN MUX SYSTEM
*******************************************************************************/
	#define C_LOCALDEF__LCCM709__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM709__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM709__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM57/LCCM709__RM57__PIN_MUX/rm57_pinmux.h>

	#endif //C_LOCALDEF__LCCM709__ENABLE_THIS_MODULE
	

/*******************************************************************************
VECTORED INTERRUPT MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM225__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM225__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM225__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM225__RM4__VECTORED_ISR_MGR/rm4_vim.h>

	#endif //C_LOCALDEF__LCCM225__ENABLE_THIS_MODULE


/*******************************************************************************
SCI / LIN Module
*******************************************************************************/
	#define C_LOCALDEF__LCCM282__ENABLE_THIS_MODULE							(0U)
	#if C_LOCALDEF__LCCM282__ENABLE_THIS_MODULE == 1U

		//if you have to do debugging
		#define DEBUG_PRINT(x)												vRM4_SCI_HELPERS__DisplayText(SCI_CHANNEL__2, x, 100)

		//enable interrupts, else use polling Mode
		#define C_LOCALDEF__LCCM282__ENABLE_INTERRUPTS						(0U)

		/** Switch on DMA functions */
		#define C_LOCALDEF__LCCM282__ENABLE_DMA								(0U)

		//determine which SCI module to enable
		//SCI1 shares pins with EMAC
		#define C_LOCALDEF__LCCM282__ENABLE_SCI_1							(0U)

		//SCI 2 is also LIN and uses the LIN pins on ZWT Package
		//SCI2 is USB debug on HDK
		#define C_LOCALDEF__LCCM282__ENABLE_SCI_2							(1U)


		//main include file
		#include <RM4/LCCM282__RM4__SCI/rm4_sci.h>

	#else
		#define DEBUG_PRINT(x)
	#endif //C_LOCALDEF__LCCM282__ENABLE_THIS_MODULE


/*******************************************************************************
SafeTool Branch Analysis Support
*******************************************************************************/
#ifdef PCLINT_ENABLED
	#define C_LOCALDEF__LCCM517__ENABLE_THIS_MODULE							(0U)
#else
	#define C_LOCALDEF__LCCM517__ENABLE_THIS_MODULE							(0U)
#endif	
	#if C_LOCALDEF__LCCM517__ENABLE_THIS_MODULE == 1U

		/** The maximum number of branch calls permissible, adjust as necessary
		 * depening on function size and test environment.
		 */
		#define C_LOCALDEF__LCCM517__MAX_BRANCH_CALLS						(16U)


		/** Testing Options */
		#define C_LOCALDEF__LCCM517__ENABLE_TEST_SPEC						(0U)


		/** Main include file */
		#include <MULTICORE/LCCM517__MULTICORE__SAFETOOL_BRANCH/safetool_branch.h>
	#else
		//placeholder
		#define SAFETOOL_BRANCH(stFuncID)
	#endif //#if C_LOCALDEF__LCCM517__ENABLE_THIS_MODULE == 1U

	
/*******************************************************************************
SOFTWARE MULTIPURPOSE FAULT TREE HANDLER
*******************************************************************************/
	#define C_LOCALDEF__LCCM284__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM284__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM284__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <MULTICORE/LCCM284__MULTICORE__FAULT_TREE/fault_tree.h>

	#endif //C_LOCALDEF__LCCM284__ENABLE_THIS_MODULE

/*******************************************************************************
SELFTEST MODULE
*******************************************************************************/
	#define C_LOCALDEF__LCCM221__ENABLE_THIS_MODULE							(1U)
	#if C_LOCALDEF__LCCM221__ENABLE_THIS_MODULE == 1U

		//testing options
		#define C_LOCALDEF__LCCM221__ENABLE_TEST_SPEC						(0U)

		//main include file
		#include <RM4/LCCM221__RM4__SELFTEST/rm4_selftest.h>

	#endif //C_LOCALDEF__LCCM221__ENABLE_THIS_MODULE

#endif //_RM57L843_LAUNCHPAD__BSP_H_

