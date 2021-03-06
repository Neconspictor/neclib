
const int NEC_SILENT = FALSE;
const int _NEC_FLAGS = 0;

// **********************************************************************************
// Loads and inits the NEC library. If the loaded library version
// doesn't conform to the expected one, no initialization will be performed and a
// error message will be appear to the user in form of a message box.
// NOTE: This function has to be called in InitPerceptions() !
// **********************************************************************************

func void NEC_InitPerceptions(var int flags)
{
    var int expectedLibVersion;
    var int libVersion;
    var string msg;
    expectedLibVersion = _NEC_GetExpectedLibVersion();
    libVersion = NEC_GetLibVersion();

    //Library couldn't be loaded?
    if (!libVersion) {
        NEC_Init_Modules = false;
        if (!NEC_SILENT) {
            MEMINT_HandleError(zERR_TYPE_FATAL, ConcatStrings(NEC_relativeLibraryPath, " couldn't be loaded!"));
        } else {
            msg = ConcatStrings("neclib: ", NEC_relativeLibraryPath);
            msg = ConcatStrings(msg, " couldn't be loaded!");
            MEM_Error(msg);
        };
        return;
    };
    if (libVersion != expectedLibVersion) {
        msg = ConcatStrings("Expected lib version: ", toStringf(expectedLibVersion));
        msg = ConcatStrings(msg, " , loadded lib version: ");
        msg = ConcatStrings(msg, toStringf(libVersion));
        msg = ConcatStrings(msg, "; Library version doesn't conform to expected one! No initialization will be performed and neclib won't work as expected!");
        NEC_Init_Modules = false;
        if (!NEC_SILENT) {
            MEMINT_HandleError(zERR_TYPE_FATAL, msg);
        } else {
            MEM_Error(msg);
        };
        return;
    } else {
		MEM_Info(ConcatStrings("neclib: NEC_InitPerceptions: neclib version = ", toStringf(libVersion)));
	};

    var int adr;
    adr = GetProcAddress (LoadLibrary (NEC_relativeLibraryPath), "Hook");
	CALL_IntParam(flags);
	CALL_PutRetValTo(_@(_NEC_FLAGS));
    CALL__cdecl(adr);
	
	
	if (flags != _NEC_FLAGS) {
		MEM_Error("neclib: Not all modules have been correctly hooked!");
	} else {
		MEM_Info("neclib: All modules have been hooked.");
	};
	
	// NOTE: the other flags are set in NEC_INIT_GLOBAL
	NEC_Init_Modules = _NEC_FLAGS & NEC_DII;
	
};


func int _NEC_INIT_Modul(var string initName, var int moduleFlag) {
	
	var int symb;
	
	if (_NEC_FLAGS & moduleFlag) {
		
		symb = MEM_FindParserSymbol(initName);
		
		if (symb <= 0) {
			MEM_SendToSpy(zERR_TYPE_FATAL, ConcatStrings(ConcatStrings("neclib: Function ", initName), " not found"));
			return FALSE;
		};
		
		NEC_Init_Modules = NEC_Init_Modules | moduleFlag;
		MEM_CallByID(symb);
	};
	
	return TRUE;
	
};

// **********************************************************************************
// Initialization function for NEC that has to be called in INIT_GLOBAL.
// **********************************************************************************
func void NEC_INIT_GLOBAL() {

	const string LEVITATION_INIT_STR = "LEVITATION_Init";
	const string TELEKINESIS_INIT_STR = "TELEKINESIS_Init";	
	
	var int success;
	
	success = _NEC_INIT_Modul(LEVITATION_INIT_STR, NEC_LEVITATION);
	success = success && _NEC_INIT_Modul(TELEKINESIS_INIT_STR, NEC_TELEKINESIS);
	
	if (success) {
		MEM_Info("neclib: NEC_INIT_GLOBAL: initialized.");
	};
};