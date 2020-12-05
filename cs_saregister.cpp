/* Inspired by the original cs_saregister module by Viper.
 * Date of Original: 15/01/2011 (by Viper)
 * Date of This Version: 06/12/2020
 * Rewritten for 2.0.x by Maro 
 **********************************************
 * Module used in: ChanServ
 * Syntax: /msg ChanServ SAREGISTER 'Channel' 'Nick' 'Desc'
 **********************************************
 * Completely rewritten for Anope 2.0.
 * Confirmed to work on 2.0.9.
 * Did this stop working in a future version? Let me know on irc.kampungchat.org
 ***********************************************
 * To use, include the following in your configuration:
 * module { name = "cs_saregister" }
 * command { service = "ChanServ"; name = "SAREGISTER"; command = "chanserv/saregister"; permission = "chanserv/saregister"; }
 *
 * Don't forget to give your service opers the "chanserv/saregister" permission!
 ************
*/

#include "module.h"


class CommandCSSARegister : public Command
{
 public:
	CommandCSSARegister(Module *creator) : Command(creator, "chanserv/saregister", 3, 3)
	{
		this->SetDesc(_("Registers another channel for user."));
		this->SetSyntax(_("\037channel\037 \037nick\037 \037desc\037"));
	}
  
  void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		const Anope::string &chan = params[0];
                const Anope::string &nick = params[1];
                const Anope::string &chdesc = params.size() > 2 ? params[2] : "";
		
		const Anope::string &csregister = Config->GetModule(this->owner)->Get<const Anope::string>("registration");
                	

                User *u = source.GetUser();
                NickCore *nc = new NickCore(nick);
                NickAlias *na = NickAlias::Find(nick);
		Channel *c = Channel::Find(params[0]);
		ChannelInfo *ci = ChannelInfo::Find(params[0]);

               if (Anope::ReadOnly)
			source.Reply(_("Sorry, channel registration is temporarily disabled."));
                else if (nc->HasExt("UNCONFIRMED"))
			source.Reply(_("You must confirm your account before you can register a channel."));
		else if (chan[0] == '&')
			source.Reply(_("Local channels cannot be registered."));
		else if (chan[0] != '#')
			source.Reply(CHAN_SYMBOL_REQUIRED);
		else if (!IRCD->IsChannelValid(chan))
			source.Reply(CHAN_X_INVALID, chan.c_str());
		else if (!c && u)
			source.Reply(CHAN_X_NOT_IN_USE, chan.c_str());
		else if (ci)
			source.Reply(_("Channel \002%s\002 is already registered!"), chan.c_str());
                else if (!na)
                        source.Reply(NICK_X_NOT_REGISTERED, nick.c_str());
                else 
		{
			ci = new ChannelInfo(chan);
			ci->SetFounder(nc);
                        ci->desc = chdesc;
			
			Log(LOG_COMMAND, source, this, ci) << "to register on behalf of " << nick.c_str(); 
			source.Reply(_("Channel \002%s\002 has been registered to: %s"), chan.c_str(), nc->display.c_str());
			
			FOREACH_MOD(OnChanRegistered, (ci));
			
			if (c)
			{
				c->CheckModes();
				if (u)
					c->SetCorrectModes(u, true);
	        	}

		}
	}
			
			
		bool OnHelp(CommandSource &source, const Anope::string &subcommand) anope_override
	{
		this->SendSyntax(source);
			source.Reply(" ");
			source.Reply(_("This module lets a services operator with the\n"
					"chanserv/saregister privileges register a new\n"
					"channel for user)."));

		return true;
	}
};
    
class CSSARegister : public Module
{
	CommandCSSARegister commandcssaregister;

 public:
	CSSARegister(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
		commandcssaregister(this)
	{
		this->SetAuthor("Maro");
		this->SetVersion("1.0");
	}
};

MODULE_INIT(CSSARegister)
			
