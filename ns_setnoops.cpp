/*
 * Prevents services from setting modes upon you automatically.
 */

#include "module.h"

DECLARE_MODULE_V1
(
    "nickserv/set_noop", false, _modinit, _moddeinit,
    PACKAGE_STRING,
    VENDOR_STRING
);

mowgli_patricia_t **ns_set_cmdtree;

static void ns_cmd_set_noop(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_set_noop = { "NOOP", N_("Prevents services from setting modes upon you automatically."), AC_NONE, 1, ns_cmd_set_noop, { .path = "nickserv/set_noop" } };

static bool has_noop(const mynick_t *mn, const void *arg)
{
    myuser_t *mu = mn->owner;

    return ( mu->flags & MU_NOOP ) == MU_NOOP;
}

void _modinit(module_t *m)
{
    MODULE_TRY_REQUEST_SYMBOL(m, ns_set_cmdtree, "nickserv/set_core", "ns_set_cmdtree");

    command_add(&ns_set_noop, *ns_set_cmdtree);

    use_nslist_main_symbols(m);

    static list_param_t noop;
    noop.opttype = OPT_BOOL;
    noop.is_match = has_noop;

    list_register("noop", &noop);
}



void _moddeinit(module_unload_intent_t intent)
{
    command_delete(&ns_set_noop, *ns_set_cmdtree);

    list_unregister("noop");
}

/* SET NOOP [ON|OFF] */
static void ns_cmd_set_noop(sourceinfo_t *si, int parc, char *parv[])
{
    char *params = parv[0];

    if (!params)
    {
        command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "NOOP");
        return;
    }

    if (!strcasecmp("ON", params))
    {
        if (MU_NOOP & si->smu->flags)
        {
            command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for account \2%s\2."), "NOOP", entity(si->smu)->name);
            return;
        }

        logcommand(si, CMDLOG_SET, "SET:NOOP:ON");

        si->smu->flags |= MU_NOOP;

        command_success_nodata(si, _("The \2%s\2 flag has been set for account \2%s\2."), "NOOP", entity(si->smu)->name);

        return;
    }
    else if (!strcasecmp("OFF", params))
    {
        if (!(MU_NOOP & si->smu->flags))
        {
            command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for account \2%s\2."), "NOOP", entity(si->smu)->name);
            return;
        }

        logcommand(si, CMDLOG_SET, "SET:NOOP:OFF");

        si->smu->flags &= ~MU_NOOP;

        command_success_nodata(si, _("The \2%s\2 flag has been removed for account \2%s\2."), "NOOP", entity(si->smu)->name);

        return;
    }

    else
    {
        command_fail(si, fault_badparams, STR_INVALID_PARAMS, "NOOP");
        return;
    }
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
