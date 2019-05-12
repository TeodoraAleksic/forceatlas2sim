#include <iostream>

#include "forceatlas2params.h"

ForceAtlas2Params::ForceAtlas2Params()
{
	kr = 0.01f;
	krp = 100.0f;

	fg = false;
	fsg = false;
	kg = 1.0f;

	tau = 0.5f;
	ks = 0.1f;
	ksmax = 10.0f;

	delta = 1;

	debug = false;
}

ForceAtlas2Params::~ForceAtlas2Params()
{
}

int ForceAtlas2Params::stringToInt(std::string str)
{
	try
	{
		return std::stoi(str);
	}
	catch (std::out_of_range)
	{
		std::cout << "'" << str << "' is out of range of a value representable by integer." << std::endl;
		throw;
	}
	catch (...)
	{
		std::cout << "'" << str << "' is not a valid integer." << std::endl;
		throw;
	}
}

float ForceAtlas2Params::stringToFloat(std::string str)
{
	try
	{
		return std::stof(str);
	}
	catch (std::out_of_range)
	{
		std::cout << "'" << str << "' is out of range of a value representable by float." << std::endl;
		throw;
	}
	catch (...)
	{
		std::cout << "'" << str << "' is not a valid float." << std::endl;
		throw;
	}
}

std::string ForceAtlas2Params::getInput() const
{
	return input;
}

float ForceAtlas2Params::getKr() const
{
	return kr;
}

float ForceAtlas2Params::getKrp() const
{
	return krp;
}

bool ForceAtlas2Params::getFg() const
{
	return fg;
}

bool ForceAtlas2Params::getFsg() const
{
	return fsg;
}

float ForceAtlas2Params::getKg() const
{
	return kg;
}

float ForceAtlas2Params::getTau() const
{
	return tau;
}

float ForceAtlas2Params::getKs() const
{
	return ks;
}

float ForceAtlas2Params::getKsmax() const
{
	return ksmax;
}

int ForceAtlas2Params::getDelta() const
{
	return delta;
}

bool ForceAtlas2Params::getDebug() const
{
	return debug;
}

void ForceAtlas2Params::setInput(std::string input_)
{
	input = input_;
}

void ForceAtlas2Params::setKr(float kr_)
{
	if (kr_ < 0.0f)
	{
		std::string msg = "Repulsion force coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	kr = kr_;
}

void ForceAtlas2Params::setKr(std::string kr_)
{
	setKr(stringToFloat(kr_));
}

void ForceAtlas2Params::setKrp(float krp_)
{
	if (krp_ < 0.0f)
	{
		std::string msg = "Repulsion force overlap coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	krp = krp_;
}

void ForceAtlas2Params::setKrp(std::string krp_)
{
	setKrp(stringToFloat(krp_));
}

void ForceAtlas2Params::setFg(bool fg_)
{
	if (fsg && fg_)
	{
		std::string msg = "Can't set both normal and strong gravity.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	fg = fg_;
}

void ForceAtlas2Params::setFsg(bool fsg_)
{
	if (fg && fsg_)
	{
		std::string msg = "Can't set both normal and strong gravity.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	fsg = fsg_;
}

void ForceAtlas2Params::setKg(float kg_)
{
	if (kg_ < 0.0f)
	{
		std::string msg = "Gravitational force coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	kg = kg_;
}

void ForceAtlas2Params::setKg(std::string kg_)
{
	setKg(stringToFloat(kg_));
}

void ForceAtlas2Params::setTau(float tau_)
{
	if (tau_ < 0.0f)
	{
		std::string msg = "Tolerance to swinging coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	tau = tau_;
}

void ForceAtlas2Params::setTau(std::string tau_)
{
	setTau(stringToFloat(tau_));
}

void ForceAtlas2Params::setKs(float ks_)
{
	if (ks_ < 0.0f)
	{
		std::string msg = "Global speed coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	ks = ks_;
}

void ForceAtlas2Params::setKs(std::string ks_)
{
	setKs(stringToFloat(ks_));
}

void ForceAtlas2Params::setKsmax(float ksmax_)
{
	if (ksmax_ < 0.0f)
	{
		std::string msg = "Max global speed coefficient must be greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	ksmax = ksmax_;
}

void ForceAtlas2Params::setKsmax(std::string ksmax_)
{
	setKsmax(stringToFloat(ksmax_));
}

void ForceAtlas2Params::setDelta(int delta_)
{
	if (delta_ < 0)
	{
		std::string msg = "Edge weight influence must be equal to or greater than 0.";
		std::cout << msg << std::endl;
		throw std::runtime_error(msg);
	}

	delta = delta_;
}

void ForceAtlas2Params::setDelta(std::string delta_)
{
	setDelta(stringToInt(delta_));
}

void ForceAtlas2Params::setDebug(bool debug_)
{
	debug = debug_;
}
