#include "forceatlas2params.h"

ForceAtlas2Params::ForceAtlas2Params()
{
	kr = 0.01f;
	krp = 100.0f;

	fg = false;
	fsg = false;
	kg = 9.81f;

	nw = false;
}

ForceAtlas2Params::~ForceAtlas2Params()
{
}

float ForceAtlas2Params::stringToFloat(std::string str)
{
	try
	{
		return std::stof(str);
	}
	catch (std::out_of_range)
	{
		throw str + " is out of range of a value representable by float";
	}
	catch (...)
	{
		throw str + " is not a valid float";
	}
}

std::string ForceAtlas2Params::getInput()
{
	return input;
}

float ForceAtlas2Params::getKr()
{
	return kr;
}

float ForceAtlas2Params::getKrp()
{
	return krp;
}

bool ForceAtlas2Params::getFg()
{
	return fg;
}

bool ForceAtlas2Params::getFsg()
{
	return fsg;
}

float ForceAtlas2Params::getKg()
{
	return kg;
}

bool ForceAtlas2Params::getNw()
{
	return nw;
}

void ForceAtlas2Params::setInput(std::string input_)
{
	input = input_;
}

void ForceAtlas2Params::setKr(float kr_)
{
	if (kr_ < 0.0f)
		throw "Repulsion force coefficient must be greater than 0.0";

	kr = kr_;
}

void ForceAtlas2Params::setKr(std::string kr_)
{
	kr = stringToFloat(kr_);

	if (kr < 0.0f)
		throw "Repulsion force coefficient must be greater than 0.0";
}

void ForceAtlas2Params::setKrp(float krp_)
{
	if (krp_ < 0.0f)
		throw "Repulsion force overlap coefficient must be greater than 0.0";

	krp = krp_;
}

void ForceAtlas2Params::setKrp(std::string krp_)
{
	krp = stringToFloat(krp_);

	if (krp < 0.0f)
		throw "Repulsion force overlap coefficient must be greater than 0.0";
}

void ForceAtlas2Params::setFg(bool fg_)
{
	if (fsg && fg_)
		throw "Can't set both normal and strong gravity";

	fg = fg_;
}

void ForceAtlas2Params::setFsg(bool fsg_)
{
	if (fg && fsg_)
		throw "Can't set both normal and strong gravity";

	fsg = fsg_;
}

void ForceAtlas2Params::setKg(float kg_)
{
	if (kg_ < 0.0f)
		throw "Gravitational force coefficient must be greater than 0.0";

	kg = kg_;
}

void ForceAtlas2Params::setKg(std::string kg_)
{
	kg = stringToFloat(kg_);

	if (kg < 0.0f)
		throw "Gravitational force coefficient must be greater than 0.0";
}

void ForceAtlas2Params::setNw(bool nw_)
{
	nw = nw_;
}
