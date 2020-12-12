#include "module.h"

class CommandNSSetNoOp : public Command
{
 public:
	CommandNSSetNoOp(Module *creator, const Anope::string &sname = "nickserv/set/noop", size_t min = 1) : Command(creator, sname, min, min + 1)
	{
		this->SetDesc(_("Sets whether syou can be added in the channel access list."));
		this->SetSyntax("{ON | OFF}");
	}

	void Run(CommandSource &source, const Anope::string &user, const Anope::string &param)
	{
		if (Anope::ReadOnly)
		{
			source.Reply(READ_ONLY_MODE);
			return;
		}

		const NickAlias *na = NickAlias::Find(user);
		if (na == NULL)
		{
			source.Reply(NICK_X_NOT_REGISTERED, user.c_str());
			return;
		}
		NickCore *nc = na->nc;

		EventReturn MOD_RESULT;
		FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
		if (MOD_RESULT == EVENT_STOP)
			return;

		if (param.equals_ci("ON"))
		{
			Log(nc == source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable noop for " << na->nc->display;
			nc->Extend<bool>("NOOP");
			source.Reply(_("Services will from now on set status noop on for %s."), nc->display.c_str());
		}
		else if (param.equals_ci("OFF"))
		{
			Log(nc == source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to disable noop for " << na->nc->display;
			nc->Shrink<bool>("NOOP");
			source.Reply(_("Services will no longer set status noop on %s."), nc->display.c_str());
		}
		else
			this->OnSyntaxError(source, "NOOP");
	}

	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		this->Run(source, source.nc->display, params[0]);
	}

	bool OnHelp(CommandSource &source, const Anope::string &) anope_override
	{
		BotInfo *bi = Config->GetClient("ChanServ");
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Sets whether you can be added in channel acccess list.\n"
				"Set to \002ON\002 to allow %s to set noop modes on you automatically."), bi ? bi->nick.c_str() : "ChanServ");
		return true;
	}
};

class NSSetNoop : public Module
{
	CommandNSSetNoop commandnssetnoop;

 public:
	NSSetNoop(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
	commandnssetnoop(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");
	}
};

MODULE_INIT(NSSetNoop)