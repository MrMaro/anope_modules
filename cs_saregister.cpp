#include "module.h"

class CommandCSSARegister : public Command
{
 public:
	CommandCSSARegister(Module *creator) : Command(creator, "chanserv/saregister", 2, 2)
	{
		this->SetDesc(_("Registers another channel."));
		this->SetSyntax(_("\037channel\037 \037description\037"));
	}
  
  void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		
		const Anope::string &chan = params[0];
		const Anope::string &chdesc = params.size() > 1 ? params[1] : "";
		
		const Anope::string &csregister = Config->GetModule(this->owner)->Get<const Anope::string>("registration");
		
		User *u = source.GetUser();
		NickCore *nc = source.nc;
		Channel *c = Channel::Find(params[0]);
		ChannelInfo *ci = ChannelInfo::Find(params[0]);

               if (Anope::ReadOnly)
			source.Reply(_("Sorry, channel registration is temporarily disabled."));
		else if (chan[0] != '#')
			source.Reply(CHAN_SYMBOL_REQUIRED);
		else if (!IRCD->IsChannelValid(chan))
			source.Reply(CHAN_X_INVALID, chan.c_str());
		else if (!c && u)
			source.Reply(CHAN_X_NOT_IN_USE, chan.c_str());
		else if (ci)
			source.Reply(_("Channel \002%s\002 is already registered!"), chan.c_str());
		
		else
		{
			ci = new ChannelInfo(chan);
			ci->SetFounder(nc);
			ci->desc = chdesc;

			if (c && !c->topic.empty())
			{
				ci->last_topic = c->topic;
				ci->last_topic_setter = c->topic_setter;
				ci->last_topic_time = c->topic_time;
			}
			else
				ci->last_topic_setter = source.service->nick;
			
			Log(LOG_COMMAND, source, this, ci);
			source.Reply(_("Channel \002%s\002 registered under your account: %s"), chan.c_str(), nc->display.c_str());
			
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
					"chanserv/saregister privileges register a\n"
					"other channel)."));
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
	}
};

MODULE_INIT(CSSARegister)
			
