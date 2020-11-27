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
		this->SetDesc(_("Forcibly change a user's nick"));
		this->SetSyntax(_("\037nick\037 \037nick\037"));
	}
	
	void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		User *u = User::Find(params[0], true);
		
		if (!u)
		{
			source.Reply(_("User %s is not online"), params[0].c_str());
			return;
		}
		
		Anope::string nick = params[1];
		
		if (!IRCD->IsHostValid(nick))
		{
			source.Reply("Invalid nick");
			return;
		}
		
		IRCD->SendVhost(u, ident, nick);
		u->UpdateHost();
		
		Log(LOG_ADMIN, source, this) << "to change the nick of " << u->nick.c_str() << " to " << nick.c_str();
		source.Reply(_("Nick of \002%s\002 changed to \002%s\002"), u->nick.c_str(), nick.c_str());
		
	}
	
	bool OnHelp (CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
		source.Reply(" ");
		source.Reply(_("Allows a Services Operator to forcibly change a user's nick"));
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
		
		if (!IRCD || !IRCD->CanChangeNick)
			throw ModuleException("Your IRCd does not support command");
	}
};

MODULE_INIT(OSChgnick)
