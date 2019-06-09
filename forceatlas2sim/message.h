#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

namespace msg
{
	// Main messages

	const std::string ERR_GLFW_WINDOW = "Error creating GLFW window";
	const std::string ERR_GLAD_INIT = "Error initializing GLAD";

	const std::string INFO_START_SIMULATION = "Starting simulation";

	const std::string DEBUG_DELTA_RUN_TIME = "Delta run time [ms]: {}";

	// Program parameter messages

	const std::string ERR_INVALID_PROG_ARG = "Invalid program argument '{}'";

	const std::string ERR_OUT_OF_RANGE_INT = "Value '{}' is out of range representable by integer";
	const std::string ERR_INVALID_INT = "'{}' is not a valid integer";

	const std::string ERR_OUT_OF_RANGE_FLOAT = "Value '{}' is out of range representable by float";
	const std::string ERR_INVALID_FLOAT = "'{}' is not a valid float";

	const std::string ERR_GT_ZERO = "{} coefficient must be greater than 0";
	const std::string ERR_EQ_GT_ZERO = "{} coefficient must be equal to or greater than 0";

	const std::string ERR_GRAVITY_FLAGS = "Cannot set both normal and strong gravity flags";

	// Reading input file messages

	const std::string INFO_READING_GRAPH_FILE = "Reading graph file '{}'";

	const std::string INFO_NUM_OF_NODES = "Number of nodes in graph file: {}";
	const std::string INFO_NUM_OF_EDGES = "Number of edges in graph file: {}";

	const std::string ERR_GRAPH_FILE_EXT = "Unsupported file extension '{}'";
	const std::string ERR_READING_GRAPH_FILE = "Error reading graph file: {}";
	const std::string ERR_READING_GEXF_FILE = "Error reading GEXF file";
	const std::string ERR_GEXF_TREE_SUPPORT = "Error compiling GEXF tree support";
	const std::string ERR_READING_GML_FILE = "Error reading GML file at line {} column {}: {}";

	// Parsing graph data messages

	const std::string INFO_POST_PROC_GRAPHICS = "Post-processing graph graphics";
	const std::string INFO_PACK_NODES_IN_CUBE = "Packing nodes in cube structure";
	const std::string INFO_ZERO_FORCE_SUM = "Adjusting node axes to avoid zero force sums";
	const std::string INFO_POST_PROC_EDGES = "Post-processing graph edges";

	// OpenGL/OpenCL device info messages

	const std::string INFO_GL_VENDOR = "OpenGL device vendor: {}";
	const std::string INFO_GL_RENDERER = "OpenGL device renderer: {}";

	const std::string INFO_CL_PLATFORM_NAME = "OpenCL platform name: {}";
	const std::string INFO_CL_PLATFORM_VENDOR = "OpenCL platform vendor: {}";
	const std::string INFO_CL_PLATFORM_VERSION = "OpenCL platform version: {}";

	const std::string INFO_CL_DEVICE_NAME = "OpenCL device name: {}";
	const std::string INFO_CL_DEVICE_VENDOR = "OpenCL device vendor: {}";
	const std::string INFO_CL_DEVICE_VERSION = "OpenCL device version: {}";

	// OpenCL messages

	const std::string ERR_CL_NO_PLATFORM = "Could not find OpenCL platform";
	const std::string ERR_CL_NO_DEVICE = "Could not find CL/GL interoperable device";

	const std::string ERR_CL_CMD_QUEUE_INIT = "Error initializing OpenCL command queue: {}";

	const std::string ERR_CL_ACQ_BUFFER = "Error acquiring shared buffers: {}";
	const std::string ERR_CL_RLS_BUFFER = "Error releasing shared buffers: {}";

	const std::string ERR_CL_KERNEL_BUILD = "Error building OpenCL kernel '{}': {}";
	const std::string ERR_CL_KERNEL_ARG = "Error setting argument '{}' of OpenCL kernel '{}': {}";
	const std::string ERR_CL_KERNEL_RUN = "Error running kernel '{}': {}";

	// OpenGL messages

	const std::string ERR_GL_SHADER_BUILD = "Error building OpenGL shader '{}'";
	const std::string ERR_GL_PROGRAM_BUILD = "Error building OpenGL program";

	// FreeType messages

	const std::string ERR_FT_INIT = "Error initializing the FreeType library";
	const std::string ERR_FT_LOAD_FONT = "Error loading font file '{}'";
	const std::string ERR_FT_LOAD_CHR = "Error loading character '{}' from font file";
}

#endif
