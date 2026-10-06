/**
 * @file
 * @brief Header file to define the useful structures for NDSnisync
 * @authors Alberto de Gracia - UPM
 * @authors Miguel Astrain - UPM
 *
 */

#ifndef NISYNCDEFINITIONS_H_
#define NISYNCDEFINITIONS_H_

#include <nisync-lib.h>

/**
 * @brief Struct for managing basic device parameters (devfd, devnum, devtype)
 */
typedef struct devData {
	nisync_device_type devtype;
	int devfd;
	int devnum;
	bool softReset;
}devData_t;

/**
 * @brief Struct for global device error management
 */
typedef struct ndsError {
    std::string source;
    std::string grade;
    std::string message;
} ndsError_t;

/**
 * @brief Available modes for setting actions in FTE node
 */
enum class setMode_t
{
    single,      	///< Set one FTE
    pulse,          ///< Set one pulse
    clk, 			///< Set a clock in a terminal
    inmLVL,			///< Set the level in a terminal
    MAX_MODE_NUM 	///< Indicate the number of modes in the enumeration
};

/**
 * @brief Available modes for suppressing actions in FTE node
 */
enum class suppressMode_t
{
    single,     	///< Suppress one FTE
	clk, 			///< Suppress a clock in a terminal
    MAX_MODE_NUM 	///< Indicate the number of modes in the enumeration
};

/**
 * @brief Struct for FTE configuration
 */
typedef struct fte{
    uint64_t time;
    nisync_level_t level;
}fte_t;

/**
 * @brief Struct for clock configuration
 */
typedef struct clock{
    uint64_t startTime;
    uint64_t endTime;
    uint64_t period;
    uint64_t dutyCycle;
} clk_t;

/**
 * @brief Struct for Timestamp configuration
 */
typedef struct timestamp {
    nisync_edge_t edge;
    uint32_t decCount;
} nisyncTimestamp_t;


/**
 * @brief Struct for Routing Terminal configuration
 */
typedef struct connection {
    nisync_terminal_t srcTerminal;
    nisync_terminal_t dstTerminal;
    nisync_synchronous_t syncStatus;
    nisync_invert_t invertStatus;
} connection_t;


#endif /* NISYNDEFINITIONS_H_ */
