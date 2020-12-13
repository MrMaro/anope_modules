#include "module.h"

class CommandNSSetNoOp : public Command
{
 public:
	CommandNSSetNoOp(Module *creator, const Anope::string &sname = "nickserv/set/noop", 2, 2)
	{
		this->SetDesc(_("Sets whether syou can be added in the channel access list."));
		this->SetSyntax(_("SET \037nick\037"));
		this->SetSyntax(_("REVOKE \037nick\037"));
	}

	void Run(CommandSource &source, const Anope::string &user, const Anope::string &param) anope_override
	{
		
		const Anope::string &cmd = params[0];
		const Anope::string &nick = params[1];
		
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
		
		else if (cmd.equals_ci("SET"))
		IRCD->SendSVSNOOP(s, true);
			s->Extend<Anope::string>("noop", source.GetNick());

			Log(LOG_ADMIN, source, this) << "SET on " << s->GetName();
			source.Reply(_("All access from \002%s\002 have been removed."), s->GetName().c_str());

		       }
		}
		else if (cmd.equals_ci("REVOKE"))
		{
			s->Shrink<Anope::string>("noop");
			IRCD->SendSVSNOOP(s, false);
			Log(LOG_ADMIN, source, this) << "REVOKE on " << s->GetName();
			source.Reply(_("All access of \002%s\002 have been reset."), s->GetName().c_str());
		}
		else
			this->OnSyntaxError(source, "");
	}
        

        bool OnHelp(CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("\002SET\002 disable access from the given\n"
				"\002nick\002 and prevents operators from opering\n"
				"\002REVOKE\002 removes this\n"
				"restriction."));
		return true;
	}
};

class OSNOOP : public Module
{
	CommandNSSETNOOP commandnssetnoop;
	PrimitiveExtensibleItem<Anope::string> noop;


 public:
	NSSetNoop(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
	commandnssetnoop(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");
	}
};

MODULE_INIT(NSSetNoop)
