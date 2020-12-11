#include "module.h"

class RootServCore : public Module
{
	Reference<BotInfo> RootServ;

 public:
	RootServCore(const Anope::string& modname, const Anope::string& creator) : Module(modname, creator, PSEUDOCLIENT | THIRD)
	{
		if(!IRCD)
		{
			throw ModuleException("IRCd protocol module not loaded");
		}
	}

	void OnReload(Configuration::Conf* conf) anope_override
	{
		const Anope::string& rsnick = conf->GetModule(this)->Get<const Anope::string>("client");

		if (rsnick.empty())
		{
			throw ConfigException(Module::name + ": <client> must be defined");
		}

		BotInfo* bi = BotInfo::Find(rsnick, true);
		if (!bi)
		{
			throw ConfigException(Module::name + ": no bot named " + rsnick);
		}

		RootServ = bi;
	}

	EventReturn OnPreHelp(CommandSource& source, const std::vector<Anope::string>& params) anope_override
	{
		if (!params.empty() || source.c || source.service != *RootServ)
		{
			return EVENT_CONTINUE;
		}
		source.Reply(_("%s commands:"), RootServ->nick.c_str());
		return EVENT_CONTINUE;
	}
};

MODULE_INIT(RootServCore)
