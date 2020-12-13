#include "module.h"

class CommandNSMyPass : public Command
{
 public:
	CommandNSMyPass(Module *creator) : Command(creator, "nickserv/mypass", 1, 1)
	{
		this->SetDesc(_("Retrieve the password for a nickname"));
		this->SetSyntax(_("\037nickname\037"));
	}

	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		const Anope::string &nick = params[0];
		Anope::string tmp_pass;
		const NickAlias *na;

		if (!(na = NickAlias::Find(nick)))
			source.Reply(NICK_X_NOT_REGISTERED, nick.c_str());
		else if (Config->GetModule("nickserv")->Get<bool>("secureadmins", "yes") && na->nc->IsServicesOper())
			source.Reply(_("You may not get the password of other Services Operators."));
		else
		{
			if (Anope::Decrypt(na->nc->pass, tmp_pass) == 1)
			{
				Log(LOG_ADMIN, source, this) << "for " << nick;
				source.Reply(_("Password for %s is \002%s\002."), nick.c_str(), tmp_pass.c_str());
			}
			else
				source.Reply(_("MYPASS command unavailable because encryption is in use."));
		}
		return;
	}

	bool OnHelp(CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Returns the password for the given nickname.  \002Note\002 that\n"
				"whenever this command is used, a message including the\n"
				"person who issued the command and the nickname it was used\n"
				"on will be logged and sent out as a WALLOPS/GLOBOPS."));
		return true;
	}
};

class NSMyPass : public Module
{
	CommandNSMyPass commandnsmypass;

public:

	NSMyPass(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
		commandnsmypass(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");
	}
};

MODULE_INIT(NSMyPass)
