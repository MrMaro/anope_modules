#include "module.h"

class CommandCSSARegister : public Command
{
 public:
	CommandCSSARegister(Module *creator) : Command(creator, "chanserv/saregister", 3, 3)
	{
		this->SetDesc(_("Registers another channel."));
		this->SetSyntax(_("\037channel\037 \037description\037"));
	}
  
  void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
	{
		User *u = 0;
		Anope::string u_nick = params[0];
		size_t nicklen = u_nick.length();
		Anope::string pass = params[1];
		Anope::string email = params[2];
