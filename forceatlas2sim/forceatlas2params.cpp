#include <iostream>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "forceatlas2params.h"
#include "message.h"
#include "utility.h"

ForceAtlas2Params::ForceAtlas2Params():
	kr(0.01f), krp(100.0f),
	fg(false), fsg(false), kg(1.0f),
	tau(0.5f), ks(0.1f), ksmax(10.0f),
	delta(1),
	debug(false)
{
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
		spdlog::error(fmt::format(msg::ERR_OUT_OF_RANGE_INT, str));
		throw;
	}
	catch (...)
	{
		spdlog::error(fmt::format(msg::ERR_INVALID_INT, str));
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
		spdlog::error(fmt::format(msg::ERR_OUT_OF_RANGE_FLOAT, str));
		throw;
	}
	catch (...)
	{
		spdlog::error(fmt::format(msg::ERR_INVALID_FLOAT, str));
		throw;
	}
}

bool ForceAtlas2Params::isValueArg(std::string argName)
{
	return
		argName == std::string("-i") ||
		argName == std::string("-kr") ||
		argName == std::string("-krp") ||
		argName == std::string("-kg") ||
		argName == std::string("-tau") ||
		argName == std::string("-ks") ||
		argName == std::string("-ksmax") ||
		argName == std::string("-delta");
}

bool ForceAtlas2Params::isFlagArg(std::string argName)
{
	return
		argName == std::string("-fg") ||
		argName == std::string("-fsg") ||
		argName == std::string("-debug");
}

bool ForceAtlas2Params::isNotArg(std::string argName)
{
	return !isValueArg(argName) && !isFlagArg(argName) && argName != std::string("-h");
}

void ForceAtlas2Params::setValueArg(std::string argName, std::string argValue)
{
	if (argName == std::string("-i"))
		setInput(argValue);
	else if (argName == std::string("-kr"))
		setKr(argValue);
	else if (argName == std::string("-krp"))
		setKrp(argValue);
	else if (argName == std::string("-kg"))
		setKg(argValue);
	else if (argName == std::string("-tau"))
		setTau(argValue);
	else if (argName == std::string("-ks"))
		setKs(argValue);
	else if (argName == std::string("-ksmax"))
		setKsmax(argValue);
	else if (argName == std::string("-delta"))
		setDelta(argValue);
	else
		logAndThrow(fmt::format(msg::ERR_INVALID_PROG_ARG, argName));
}

void ForceAtlas2Params::setFlagArg(std::string argName)
{
	if (argName == std::string("-fg"))
		setFg(true);
	else if (argName == std::string("-fsg"))
		setFsg(true);
	else if (argName == std::string("-debug"))
		setDebug(true);
	else
		logAndThrow(fmt::format(msg::ERR_INVALID_PROG_ARG, argName));
}

std::string ForceAtlas2Params::getInput() const
{
	return input;
}

void ForceAtlas2Params::setInput(std::string input_)
{
	input = input_;
}

float ForceAtlas2Params::getKr() const
{
	return kr;
}

void ForceAtlas2Params::setKr(float kr_)
{
	if (kr_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Repulsion force"));

	kr = kr_;
}

void ForceAtlas2Params::setKr(std::string kr_)
{
	setKr(stringToFloat(kr_));
}

float ForceAtlas2Params::getKrp() const
{
	return krp;
}

void ForceAtlas2Params::setKrp(float krp_)
{
	if (krp_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Repulsion force overlap"));

	krp = krp_;
}

void ForceAtlas2Params::setKrp(std::string krp_)
{
	setKrp(stringToFloat(krp_));
}

bool ForceAtlas2Params::getFg() const
{
	return fg;
}

void ForceAtlas2Params::setFg(bool fg_)
{
	if (fsg && fg_)
		logAndThrow(msg::ERR_GRAVITY_FLAGS);

	fg = fg_;
}

bool ForceAtlas2Params::getFsg() const
{
	return fsg;
}

void ForceAtlas2Params::setFsg(bool fsg_)
{
	if (fg && fsg_)
		logAndThrow(msg::ERR_GRAVITY_FLAGS);

	fsg = fsg_;
}

float ForceAtlas2Params::getKg() const
{
	return kg;
}

void ForceAtlas2Params::setKg(float kg_)
{
	if (kg_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Gravitational force"));

	kg = kg_;
}

void ForceAtlas2Params::setKg(std::string kg_)
{
	setKg(stringToFloat(kg_));
}

float ForceAtlas2Params::getTau() const
{
	return tau;
}

void ForceAtlas2Params::setTau(float tau_)
{
	if (tau_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Tolerance to swinging"));

	tau = tau_;
}

void ForceAtlas2Params::setTau(std::string tau_)
{
	setTau(stringToFloat(tau_));
}

float ForceAtlas2Params::getKs() const
{
	return ks;
}

void ForceAtlas2Params::setKs(float ks_)
{
	if (ks_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Global speed"));

	ks = ks_;
}

void ForceAtlas2Params::setKs(std::string ks_)
{
	setKs(stringToFloat(ks_));
}

float ForceAtlas2Params::getKsmax() const
{
	return ksmax;
}

void ForceAtlas2Params::setKsmax(float ksmax_)
{
	if (ksmax_ <= 0.0f)
		logAndThrow(fmt::format(msg::ERR_GT_ZERO, "Max global speed"));

	ksmax = ksmax_;
}

void ForceAtlas2Params::setKsmax(std::string ksmax_)
{
	setKsmax(stringToFloat(ksmax_));
}

int ForceAtlas2Params::getDelta() const
{
	return delta;
}

void ForceAtlas2Params::setDelta(int delta_)
{
	if (delta_ < 0)
		logAndThrow(fmt::format(msg::ERR_EQ_GT_ZERO, "Edge weight influence"));

	delta = delta_;
}

void ForceAtlas2Params::setDelta(std::string delta_)
{
	setDelta(stringToInt(delta_));
}

bool ForceAtlas2Params::getDebug() const
{
	return debug;
}

void ForceAtlas2Params::setDebug(bool debug_)
{
	debug = debug_;
}
