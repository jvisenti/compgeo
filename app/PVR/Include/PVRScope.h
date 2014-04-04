/*************************************************************************/ /*!
@Title
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description
@License        Strictly Confidential.
*/ /**************************************************************************/

#ifndef _PVRSCOPE_H_
#define _PVRSCOPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Enums
****************************************************************************/

/// PVRScope initialisation return codes.
enum EPVRScopeInitCode
{
	ePVRScopeInitCodeOk,
	ePVRScopeInitCodeOutOfMem,
	ePVRScopeInitCodeDriverSupportNotFound,
	ePVRScopeInitCodeDriverSupportInsufficient,
	ePVRScopeInitCodeDriverSupportInitFailed,
	ePVRScopeInitCodeDriverSupportQueryInfoFailed,
	ePVRScopeInitCodeUnrecognisedHW
};

/****************************************************************************
** Structures
****************************************************************************/

// Internal implementation data
struct SPVRScopeImplData;

/// Definition of a counter that PVRScope calculates.
struct SPVRScopeCounterDef
{
	const char		*pszName;					///< Counter name, null terminated
	bool			bPercentage;				///< true if the counter is a percentage
	unsigned int	nGroup;						///< The counter group that the counter is in.
};

/// A set of return values resulting from querying the counter values.
struct SPVRScopeCounterReading
{
	float			*pfValueBuf;				///< Array of returned values
	unsigned int	nValueCnt;					///< Number of values set in the above array
	unsigned int	nReadingActiveGroup;		///< Group that was active when counters were sampled
};

/// A set of return values holding miscellaneous PVRScope information.
struct SPVRScopeGetInfo
{
	unsigned int	nGroupMax;					///< Highest group number of any counter
};

/****************************************************************************
** Declarations
****************************************************************************/

/// Query the PVRScope library description
const char *PVRScopeGetDescription();

/// Initialise the HW performance monitor section of PVRScope
EPVRScopeInitCode PVRScopeInitialise(SPVRScopeImplData **ppsData);

/*!	\brief
	Shutdown or de-initalise the HW performance monitor section of PVRScope and
	free the allocated memory.
*/
void PVRScopeDeInitialise(
	SPVRScopeImplData		**ppsData,			///< Context data
	SPVRScopeCounterDef		**ppsCounters,		///< Array of counters
	SPVRScopeCounterReading	* const psReading);	///< Results memory area

/*!	\brief
	Query for information relating to the performance monitor section of
	PVRScope. This function need only be called during initialisation.
*/
void PVRScopeGetInfo(
	SPVRScopeImplData	* const psData,			///< Context data
	SPVRScopeGetInfo	* const psInfo);		///< Returned information

/*!	\brief
	Query for the list of HW performance counters support by PVRScope, and
	allocate memory in which the counter values will be received. This function
	need only be called during initialisation.
*/
bool PVRScopeGetCounters(
	SPVRScopeImplData		* const psData,		///< Context data
	unsigned int			* const pnCount,	///< Returned number of counters
	SPVRScopeCounterDef		**ppsCounters,		///< Returned counter array
	SPVRScopeCounterReading	* const psReading);	///< Pass a pointer to the structure to be initialised

/*!	\brief
	As and when desired call this function to fill the counter-value array with
	the current counter values then change the active performance counter
	group. In a 3D application, you might call this once per frame or every N
	frames. Typically the group ID should be 0xffffffff in order to leave the
	active group unchanged; if it is desired to change it then pass the new
	group ID.
*/
bool PVRScopeReadCountersThenSetGroup(
	SPVRScopeImplData		* const psData,		///< Context data
	SPVRScopeCounterReading	* const psReading,	///< Returned data will be filled into the pointed-to structure
	const unsigned int		nTimeUS,			///< Current time, in microseconds
	const unsigned int		nGroup);			///< New group; 0xffffffff to leave it unchanged


/*
	SGSPerfServer and PVRTune communications
*/

/****************************************************************************
** Enums
****************************************************************************/

/// Each editable library item has a data type associated with it
enum ESPSCommsLibType
{
	eSPSCommsLibTypeString,		///< data is string (NOT NULL-terminated, use length parameter)
	eSPSCommsLibTypeFloat,		///< data is SSPSCommsLibraryTypeFloat
	eSPSCommsLibTypeInt,		///< data is SSPSCommsLibraryTypeInt
	eSPSCommsLibTypeEnum,		///< data is string (NOT NULL-terminated, use length parameter). First line is selection number, subsequent lines are available options.
	eSPSCommsLibTypeBool,		///< data is SSPSCommsLibraryTypeBool
};

/****************************************************************************
** Structures
****************************************************************************/

// Internal implementation data
struct SSPSCommsData;

/// Definition of one editable library item
struct SSPSCommsLibraryItem
{
	/// Item name. If dots are used, PVRTune could show these as a foldable tree view.
	const char			*pszName;
	unsigned int		nNameLength;	///< Item name length

	ESPSCommsLibType	eType;			///< Item type

	/// Item data
	const char			*pData;
	unsigned int		nDataLength;	///< Item data length
};

/// Current, minimum and maximum values for an editable library item of type float
struct SSPSCommsLibraryTypeFloat
{
	float fCurrent, fMin, fMax;
};

/// Current, minimum and maximum values for an editable library item of type int
struct SSPSCommsLibraryTypeInt
{
	int nCurrent, nMin, nMax;
};

/// Current value for an editable library item of type bool
struct SSPSCommsLibraryTypeBool
{
	bool bValue;
};

/// Definition of one custom counter
struct SSPSCommsCounterDef
{
	const char			*pszName;		///< Custom counter name
	unsigned int		nNameLength;	///< Custom counter name length
};

/****************************************************************************
** Declarations
****************************************************************************/

/// Initialise the remote control section of PVRScope
SSPSCommsData *pplInitialise(
	const char			* const pszName,	///< String to describe the application
	const unsigned int	nNameLen);			///< String length

/// Shutdown or de-initialise the remote control section of PVRScope
void pplShutdown(SSPSCommsData *psData);

/// Query for the time. Units are microseconds, resolution is undefined.
unsigned int pplGetTimeUS(
	SSPSCommsData		&sData);

/*!	\brief
	Send a time-stamped string marker to be displayed in PVRTune.
	Examples might be:
		switching to outdoor renderer
		starting benchmark test N
*/
bool pplSendMark(
	SSPSCommsData		&sData,
	const char * const	pszString,
	unsigned int		nLen);

/// Create a library of remotely editable items
bool pplLibraryCreate(
	SSPSCommsData				&sData,
	const SSPSCommsLibraryItem	*pItems,
	unsigned int				nItemCount);

/*!	\brief
	Query to see whether a library item has been edited, and also retrieve
	the new data
*/
bool pplLibraryDirtyGetFirst(
	SSPSCommsData		&sData,
	unsigned int		&nItem,
	unsigned int		&nNewDataLen,
	const char			**ppData);

/// Specify the number of custom counters and their definitions
bool pplCountersCreate(
	SSPSCommsData				&sData,
	const SSPSCommsCounterDef	* const psCounterDefs,
	const unsigned int			nCount);

/*!	\brief
	Send an update for all the custom counters. The psCounterReadings array must be
	nCount long.
*/
bool pplCountersUpdate(
	SSPSCommsData				&sData,
	const unsigned int			* const psCounterReadings);

/*!	\brief
	Some implementations cache data sends. If the data rate is low, the real
	send of data can be significantly delayed. If this behaviour causes an
	issue, call this function to force a cache flush.
	If it is necessary to use this function, the best results are likely to be
	achieved by calling with a frequency between once per second up to once per
	frame. If data is sent extremely infrequently, this function could be
	called once at the end of each bout of data send.
*/
bool pplSendFlush(
	SSPSCommsData				&sData);

#ifdef __cplusplus
}
#endif

#endif /* _PVRSCOPE_H_ */

/*****************************************************************************
 End of file (PVRScope.h)
*****************************************************************************/
