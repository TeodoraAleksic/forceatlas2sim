#ifndef _FORCEATLAS2PARAMS_H_
#define _FORCEATLAS2PARAMS_H_

#include <string>

class ForceAtlas2Params
{
private:

	std::string input;

	float kr, krp;

	bool fg, fsg;
	float kg;

	bool nw;

	float stringToFloat(std::string str);

public:

	ForceAtlas2Params();
	~ForceAtlas2Params();

	// Getters

	std::string getInput();
	
	float getKr();
	float getKrp();

	bool getFg();
	bool getFsg();
	float getKg();

	bool getNw();

	// Setters

	void setInput(std::string input_);

	void setKr(float kr_);
	void setKr(std::string kr_);

	void setKrp(float krp_);
	void setKrp(std::string krp_);

	void setFg(bool fg_);
	void setFsg(bool fsg_);
	
	void setKg(float kg_);
	void setKg(std::string kg_);

	void setNw(bool nw_);

};

#endif
