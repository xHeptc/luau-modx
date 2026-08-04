#include <extdll.h>
#include <meta_api.h>
#include <cstring>

plugin_info_t Plugin_info = {
    META_INTERFACE_VERSION, 
    "luau-modx",
    "1.0.0.0",
    __DATE__,
    "xHeptc",
    "http://",
    "LUAU",
    PT_STARTUP,
    PT_ANYTIME
};

enginefuncs_t g_engfuncs; 
enginefuncs_t meta_engfuncs; 
enginefuncs_t meta_engfuncs_post; 

globalvars_t *gpGlobals = nullptr; 
meta_globals_t *gpMetaGlobals = nullptr; 
gamedll_funcs_t *gpGamedllFuncs = nullptr; 
mutil_funcs_t *gpMetaUtilFuncs = nullptr;

static META_FUNCTIONS gMetaFunctionTable; 
DLL_FUNCTIONS gFunctionTable; 
DLL_FUNCTIONS gFunctionTable_Post; 

C_DLLEXPORT	int	Meta_Query(const char *ifvers, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs){
    gpMetaUtilFuncs = pMetaUtilFuncs;
    *pPlugInfo = &Plugin_info;

    int	mmajor = 0, mminor = 0,	pmajor = 0, pminor = 0;

	sscanf(ifvers, "%d:%d",	&mmajor, &mminor);
	sscanf(Plugin_info.ifvers, "%d:%d",	&pmajor, &pminor);

	if (strcmp(ifvers, Plugin_info.ifvers))
	{
		LOG_MESSAGE(PLID, "warning: ifvers mismatch (pl \"%s\") (mm \"%s\")", Plugin_info.ifvers, ifvers);
		if (pmajor > mmajor)
		{
			LOG_ERROR(PLID, "metamod version is too old for this plugin; update metamod");
			return (FALSE);
		} else if (pmajor < mmajor) {
			LOG_ERROR(PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin");
			return (FALSE);
		} else if (pmajor == mmajor) {
			if (pminor > mminor)
			{
				LOG_ERROR(PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin");
				return FALSE;
			} else if (pminor < mminor) {
				LOG_MESSAGE(PLID, "warning: there may be a newer version of metamod available");
			}
		}
	}

    return TRUE;
}

C_DLLEXPORT	int	Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs){
    if (now > Plugin_info.loadable){
        LOG_ERROR(PLID, "Can't load plugin right now");
        return FALSE;
    }
    
    gpMetaGlobals = pMGlobals;
    gMetaFunctionTable.pfnGetEntityAPI2 = GetEntityAPI2;
    gMetaFunctionTable.pfnGetEntityAPI2_Post = GetEntityAPI2_Post;
    gMetaFunctionTable.pfnGetEngineFunctions = GetEngineFunctions;
    gMetaFunctionTable.pfnGetEngineFunctions_Post = GetEngineFunctions_Post;
    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
    gpGamedllFuncs = pGamedllFuncs;

    return TRUE;
}

C_DLLEXPORT	int	Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON	reason){
    if (now > Plugin_info.unloadable && reason != PNL_CMD_FORCED){
        LOG_ERROR(PLID, "Can't unload plugin right now");
        return FALSE;
    }
    
    return TRUE;
}

C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals){
    memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
    gpGlobals = pGlobals;
}

C_DLLEXPORT	int	GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion){
	memset(&gFunctionTable, 0, sizeof(DLL_FUNCTIONS));
	//memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT	int	GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion){
	memset(&gFunctionTable_Post, 0, sizeof(DLL_FUNCTIONS));
	//memcpy(pFunctionTable, &gFunctionTable_Post, sizeof(DLL_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT	int	GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion){
	memset(&meta_engfuncs, 0, sizeof(enginefuncs_t));
	//memcpy(pengfuncsFromEngine, &meta_engfuncs, sizeof(enginefuncs_t));
	return TRUE;
}

C_DLLEXPORT	int	GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine,	int	*interfaceVersion){
	memset(&meta_engfuncs_post, 0, sizeof(enginefuncs_t));
	//memcpy(pengfuncsFromEngine, &meta_engfuncs_post, sizeof(enginefuncs_t));
	return TRUE;
}