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

	std::string getInput() const;
	
	float getKr() const;
	float getKrp() const;

	bool getFg() const;
	bool getFsg() const;
	float getKg() const;

	bool getNw() const;

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
