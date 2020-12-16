#include "module.h"

class CommandRSMassSet : public Command
{
public:
    CommandRSMassSet(Module *creator) : Command(creator, "rootserv/massset", 2, 2)
    {
        this->SetDesc(_("Set options for ALL registered users"));
        this->SetSyntax(_("\037option\037 \037parameter\037"));
    }

    void Execute(CommandSource &source, const std::vector<Anope::string> &params) anope_override
    {
        const Anope::string &option = params[0]; // Our SET option, Kill, AutoOP, etc
        const Anope::string &param = params[1];  // Setting for the above option - on/off
        int count = 0;

        if (Anope::ReadOnly)
        {
            source.Reply(READ_ONLY_MODE);
            return;
        }
        else if (!option[0] || !param[0])
        {
            this->SendSyntax(source);
            source.Reply(_("Type \002%s%s HELP MASSSET\002 for more information."), Config->StrictPrivmsg.c_str(), source.service->nick.c_str());
        }
        else if (option.equals_ci("KILL"))
        {
            if (param.equals_ci("ON"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("KILLPROTECT");
                    nc->Shrink<bool>("KILL_QUICK");
                    nc->Shrink<bool>("KILL_IMMED");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to set kill ON for \002" << count << " users";
                source.Reply(_("Successfully enabled \002kill protection\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("QUICK"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("KILLPROTECT");
                    nc->Extend<bool>("KILL_QUICK");
                    nc->Shrink<bool>("KILL_IMMED");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to set kill QUICK for \002" << count << " users";
                source.Reply(_("Successfully enabled \002quick kill protection\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("IMMED"))
            {
                if (Config->GetModule(this->owner)->Get<bool>("allowkillimmed"))
                {
                    for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                    {
                        NickCore *nc = it->second;
                        count++;

                        EventReturn MOD_RESULT;
                        FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                        if (MOD_RESULT == EVENT_STOP)
                            return;

                        nc->Extend<bool>("KILLPROTECT");
                        nc->Shrink<bool>("KILL_QUICK");
                        nc->Extend<bool>("KILL_IMMED");
                    }
                    Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to set kill IMMED for \002" << count << " users";
                    source.Reply(_("Successfully enabled \002immediate kill protection\002 for \002%d\002 users."), count);
                }
                else
                    source.Reply(_("The \002IMMED\002 option is not available on this network."));
            }
            else if (param.equals_ci("OFF"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Shrink<bool>("KILLPROTECT");
                    nc->Shrink<bool>("KILL_QUICK");
                    nc->Shrink<bool>("KILL_IMMED");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to set kill protection OFF for \002" << count << " users";
                source.Reply(_("Successfully disabled \002kill protection\002 for \002%d\002 users."), count);
            }
            else
                source.Reply(_("Syntax: \002MASSSET KILL \037on|quick|immed|off\037\002"), source.service->nick.c_str());
        }
        else if (option.equals_ci("SECURE"))
        {
            if (param.equals_ci("ON"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("NS_SECURE");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable secure for \002" << count << " users";
                source.Reply(_("Successfully set \002secure\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("OFF"))
                source.Reply(_("For security reasons, mass-setting \002secure\002 to OFF is disabled."));
            else
                source.Reply(_("Syntax: \002MASSSET SECURE \037on|off\037\002"), source.service->nick.c_str());
        }
        else if (option.equals_ci("AUTOOP"))
        {
            if (param.equals_ci("ON"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("AUTOOP");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable autoop for \002" << count << " users";
                source.Reply(_("Successfully enabled \002autoop\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("OFF"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Shrink<bool>("AUTOOP");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to disable autoop for \002" << count << " users";
                source.Reply(_("Successfully disabled \002autoop\002 for \002%d\002 users."), count);
            }
            else
                source.Reply(_("Syntax: \002MASSSET AUTOOP \037on|off\037\002"), source.service->nick.c_str());
        }
        else if (option.equals_ci("MESSAGE"))
        {
            if (!Config->GetBlock("options")->Get<bool>("useprivmsg"))
            {
                source.Reply(_("useprivmsg is disabled on this network."));
                return;
            }

            if (param.equals_ci("ON"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("MSG");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable privmsg replies for \002" << count << " users";
                source.Reply(_("Successfully enabled \002privmsg replies\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("OFF"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Shrink<bool>("MSG");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable notice replies for \002" << count << " users";
                source.Reply(_("Successfully enabled \002notice replies\002 for \002%d\002 users."), count);
            }
            else
                source.Reply(_("Syntax: \002MASSSET MESSAGE \037on|off\037\002"), source.service->nick.c_str());
        }
        else if (option.equals_ci("CHANSTATS"))
        {
            if (!Config->GetModule("m_chanstats"));
            {
                source.Reply(_("m_chanstats is not loaded"));
                return;
            }

            if (param.equals_ci("ON"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Extend<bool>("NS_STATS");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to enable chanstats for \002" << count << " users";
                source.Reply(_("Successfully enabled \002chanstats\002 for \002%d\002 users."), count);
            }
            else if (param.equals_ci("OFF"))
            {
                for (nickcore_map::const_iterator it = NickCoreList->begin(), it_end = NickCoreList->end(); it != it_end; ++it)
                {
                    NickCore *nc = it->second;
                    count++;

                    EventReturn MOD_RESULT;
                    FOREACH_RESULT(OnSetNickOption, MOD_RESULT, (source, this, nc, param));
                    if (MOD_RESULT == EVENT_STOP)
                        return;

                    nc->Shrink<bool>("NS_STATS");
                }
                Log(source.GetAccount() ? LOG_COMMAND : LOG_ADMIN, source, this) << "to disable chanstats for \002" << count << " users";
                source.Reply(_("Successfully disabled \002chanstats\002 for \002%d\002 users."), count);
            }
            else
                source.Reply(_("Syntax: \002MASSSET CHANSTATS \037on|off\037\002"), source.service->nick.c_str());
        }
        else {
            source.Reply(_("Unknown \002MASSSET\002 option \002%s\002\n"
                "\002%s%s HELP MASSSET\002 for more information"), option.c_str(), Config->StrictPrivmsg.c_str(), source.service->nick.c_str());
        }
        return;
    }

    bool OnHelp(CommandSource &source, const Anope::string &subcommand) anope_override
    {
        this->SendSyntax(source);
        source.Reply(" ");
        source.Reply(_("Allows options to be mass-set for ALL registered users\n"
            "	KILL	   Change NickServ's kill setting to on/quick/immed/off\n"
            "              for all registered nicks.\n"
            "	SECURE	   Turns NickServ's security features on/off\n"
            "              for all registered nicks.\n"
            "	AUTOOP	   Turns NickServ's autoop features on/off\n"
            "              for all registered nicks.\n"
            "	MESSAGE	   Set whether NickServ privmsg or notices,\n"
            "              for all registered nicks.\n"
            "	CHANSTATS	Turns Chanstats statistics on/off\n"
            "              for all registered nicks.\n"));
        return true;
    }

};


class RSMassSet : public Module
{
    CommandRSMassSet commandrsmassset;

public:
    RSMassSet(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator, THIRD),
        commandnsmassset(this)
    {
        this->SetAuthor("Maro");
        this->SetVersion("1.0");
    }
};

MODULE_INIT(RSMassSet)
