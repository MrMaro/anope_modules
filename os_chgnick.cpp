/* os_chgnick: a module that implements services-side CHGNICK
 *
 * Copyright 2020 Maro <maro@kampungchat.org.my>
 * 
 * You may use this module under the terms of the GPLv3
 * 
 * Configuration:
 * module { name = "os_chgnick"; }
 * command { name = "CHGNICK"; service = "OperServ"; command = "operserv/chgnick"; permission = "operserv/chgnick"; }
 */

#include "module.h"

class CommandOSChgnick : public Command
{
 public:
	CommandOSChgnick(Module *creator, const Anope::string &sname = "operserv/chgnick") : Command(creator, sname, 2, 2)
	{
		this->SetDesc(_("Tukar Nick User"));
		this->SetSyntax(_("\037nick\037 \037nickbaru\037"));
	}

	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		const Anope::string &nick = params[0];
		Anope::string newnick = params[1];
		User *u2;

		if (!IRCD->CanSVSNick)
		{
			source.Reply("Cannot Changed nick");
			return;
		}

		Log(LOG_ADMIN, source, this) << "to change " << nick << " to " << newnick;
		source.Reply(_("Nick \002%s\002 has changed to \002%s\002"), nick.c_str(), newnick.c_str());

	}

	bool OnHelp (CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Forcefully change a user's nick"));
		return true;
	}
};

class OSChgnick : public Module
{
	CommandOSChgnick commandoschgnick;

 public:
	OSChgnick(const Anope::string &modname, const Anope::string &creator) :
	Module(modname, creator, THIRD), commandoschgnick(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");

		if (!IRCD || !IRCD->CanSVSNick)
			throw ModuleException("Your IRCd does not support command");
	}
};

MODULE_INIT(OSChgnick)

