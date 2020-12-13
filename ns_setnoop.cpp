#include "module.h"

class CommandNSSetNoOp : public Command
{
 public:
	CommandNSSetNoOp(Module *creator, const Anope::string &sname = "nickserv/set/noop", 2)
	{
		this->SetDesc(_("Sets whether syou can be added in the channel access list."));
		this->SetSyntax(_("SET \037nick\037"));
		this->SetSyntax(_("REVOKE \037nick\037"));
	}

	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		
		const Anope::string &cmd = params[0];
		const Anope::string &user = params[1];
		
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
		IRCD->SendSVSNOOP(true);
			Extend<Anope::string>("noop", source.GetNick());

			Log(LOG_ADMIN, source, this) << "SET on " << GetName();
			source.Reply(_("All access from \002%s\002 have been removed."), GetName().c_str());

		}
		if (cmd.equals_ci("REVOKE"))
		{
			Shrink<Anope::string>("noop");
			IRCD->SendSVSNOOP(false);
			Log(LOG_ADMIN, source, this) << "REVOKE on " << GetName();
			source.Reply(_("All access of \002%s\002 have been reset."), GetName().c_str());
		}
		else
			this->OnSyntaxError(source, "");
	};
        

        bool OnHelp(CommandSource &source, const Anope::string &subcommand) anope_override
	{
		source.Reply(" ");
		source.Reply(_("\002SET\002 disable access from the given\n"
				"\002nick\002 and prevents operators from opering\n"
				"\002REVOKE\002 removes this\n"
				"restriction."));
		return true;
	};

class NSSETNOOP : public Module
{
	CommandNSSETNOOP commandnssetnoop;
	PrimitiveExtensibleItem<Anope::string> noop;


 public:
	NSSetNoOp(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
	commandnssetnoop(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");
	}
};

MODULE_INIT(NSSetNoOp)
