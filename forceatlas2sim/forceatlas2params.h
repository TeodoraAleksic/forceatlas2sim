#ifndef _FORCEATLAS2PARAMS_H_
#define _FORCEATLAS2PARAMS_H_

#include <string>

/**
* ForceAtlas2 parameters
*
* Stores ForceAtlas2 program arguments and simulation parameters.
* Checks argument and parameter values.
*/
class ForceAtlas2Params
{
private:

	// Input file name
	std::string input;

	// Repulsion force coefficient
	float kr;
	// Repulsion force overlap coefficient
	float krp;

	// Normal gravity flag
	bool fg;
	// Strong gravity flag
	bool fsg;
	// Gravitational force coefficient
	float kg;

	// Tolerance to swinging coefficient
	float tau;
	// Global speed coefficient
	float ks;
	// Max global speed coefficient
	float ksmax;

	// Edge weight influence coefficient
	int delta;

	// Debug mode flag
	bool debug;

	/**
	* Converts a string to an integer
	*
	* @param str String value
	* @return Integer value
	*/
	int stringToInt(std::string str);

	/**
	* Converts a string to a float
	*
	* @param str String value
	* @return Float value
	*/
	float stringToFloat(std::string str);

public:

	ForceAtlas2Params();
	~ForceAtlas2Params();

	/**
	* Checks if an argument is a program's value argument
	*
	* @param argName Argument name
	* @return True if value argument, else false
	*/
	bool isValueArg(std::string argName);

	/**
	* Checks if an argument is a program's flag argument
	*
	* @param argName Argument name
	* @return True if flag argument, else false
	*/
	bool isFlagArg(std::string argName);

	/**
	* Checks if an argument is not a program's argument
	*
	* @param argName Argument name
	* @return True if not an argument, else false
	*/
	bool isNotArg(std::string argName);

	/**
	* Sets the value of a program's value argument
	*
	* @param argName	Argument name
	* @param argValue	Argument value
	*/
	void setValueArg(std::string argName, std::string argValue);

	/**
	* Sets a program's flag argument
	*
	* @param argName Argument name
	*/
	void setFlagArg(std::string argName);

	// Gets input file name
	std::string getInput() const;
	// Sets input file name
	void setInput(std::string input_);
	
	// Gets repulsion force coefficient
	float getKr() const;
	// Sets repulsion force coefficient
	void setKr(float kr_);
	// Sets repulsion force coefficient
	void setKr(std::string kr_);

	// Gets repulsion force overlap coefficient
	float getKrp() const;
	// Sets repulsion force overlap coefficient
	void setKrp(float krp_);
	// Sets repulsion force overlap coefficient
	void setKrp(std::string krp_);

	// Gets normal gravity flag
	bool getFg() const;
	// Sets normal gravity flag
	void setFg(bool fg_);

	// Gets strong gravity flag
	bool getFsg() const;
	// Sets strong gravity flag
	void setFsg(bool fsg_);

	// Gets gravitational force coefficient
	float getKg() const;
	// Sets gravitational force coefficient
	void setKg(float kg_);
	// Sets gravitational force coefficient
	void setKg(std::string kg_);

	// Gets tolerance to swinging coefficient
	float getTau() const;
	// Sets tolerance to swinging coefficient
	void setTau(float tau_);
	// Sets tolerance to swinging coefficient
	void setTau(std::string tau_);

	// Gets global speed coefficient
	float getKs() const;
	// Sets global speed coefficient
	void setKs(float ks_);
	// Sets global speed coefficient
	void setKs(std::string ks_);

	// Gets max global speed coefficient
	float getKsmax() const;
	// Sets max global speed coefficient
	void setKsmax(float ksmax_);
	// Sets max global speed coefficient
	void setKsmax(std::string ksmax_);

	// Gets edge weight influence coefficient
	int getDelta() const;
	// Sets edge weight influence coefficient
	void setDelta(int delta_);
	// Sets edge weight influence coefficient
	void setDelta(std::string delta_);

	// Gets debug mode flag
	bool getDebug() const;
	// Sets debug mode flag
	void setDebug(bool debug_);

};

#endif
