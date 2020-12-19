class CommandNSSetAutoVoice : public Command
{
 public:
	CommandNSSetAutoVoice(Module *creator, const Anope::string &sname = "nickserv/set/autovoice",1,1)
	{
		this->SetDesc(_("Sets whether services should set channel status modes on you automatically."));
		this->SetSyntax("{ON | OFF}");
	}

   const Anope::string &param = params[0];
   
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
			Log(nc == source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable autovoice for " << na->nc->display;
			nc->Extend<bool>("AUTOVOICE");
			source.Reply(_("Services will from now on set status modes on %s in channels."), nc->display.c_str());
		}
		else if (param.equals_ci("OFF"))
		{
			Log(nc == source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to disable autovoice for " << na->nc->display;
			nc->Shrink<bool>("AUTOVOICE");
			source.Reply(_("Services will no longer set status modes on %s in channels."), nc->display.c_str());
		}
		else
			this->OnSyntaxError(source, "AUTOVOICE");
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
		source.Reply(_("Sets whether you will be given your channel status modes automatically.\n"
				"Set to \002ON\002 to allow %s to set status modes on you automatically\n"
				"when entering channels. Note that depending on channel settings some modes\n"
				"may not get set automatically."), bi ? bi->nick.c_str() : "ChanServ");
		return true;
	}
};

class NSSetAutoVoice : public Module
{
    CommandNSSetAutoVoice commandnssetautovoice;

public:
    NSSetAutoVoice(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
        commandnssetautovoice(this)
    {
        this->SetAuthor("Maro");
        this->SetVersion("1.0");
    }
};

MODULE_INIT(NSSetAutoVoice)
