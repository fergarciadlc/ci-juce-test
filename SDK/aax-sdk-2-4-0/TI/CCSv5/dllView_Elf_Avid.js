/* 
* dllView_elf_Avid.js
* 
* dllView.js port to support ELF. This script can determine which dll has been 
* loaded by the dynamic loader and will automatically load the appropriate 
* symbols needed for debug. 
*
* Versions  Author
* 0.1       Ki-Soo Lee
* 0.2       Ki-Soo Lee
* 0.3       Ki-Soo Lee
* 0.4       Ki-Soo Lee
* 
* Avid Mods by Paul Vercellotti; Differences from original are:
*  - All debug target CPU's are scanned, and the symbol loading process is
*    performed for all connected targets.
*  - We store the dynamic loader table at a fixed address so we don't need
*    to do symbol lookup to find it (which means we don't need to load the shell's
*    symbols first)
*  - Dynamic loader stores full paths in the table, so we don't need to store
*    symbol paths.  This is a problem debugging a board running on a Mac right now.
*  - The dynamic loader table is in a slightly different arrangement, with all 
*    data in a single contiguous block rather than spread out, and without a 
*    separate pointer structure at the beginning.
* ==============================================================================
*
* Dependencies:
* 1) CCS v4.2.1 or greater
* 2) Application built with a compiler version which supports dynamic linking
* 3) Dynamic loader is being used
* 4) Base image built with DLL debug support enabled
*
*
* This script is meant to be used from the CCS4 Scripting Console. Load this 
* file into the  Scripting Console during an active debug session using the 
* loadJS command: 
* 
* js:> loadJSFile C:/path/to/dllView_elf.js
* 
* To have the script permanently loaded (to avoid having to load it for every
* new debug session):
* 
* js:> loadJSFile C:/path/to/dllView_elf.js true
* 
* Once loaded, the folllowing functons in this script can be called from the
* 'scripts' menu:
* 
* "DLLView-Attach to DSP": Connect to the target
* "DLLView-Load Base Image Symbols": Load the base image symbols
* "DLLView-Load DLL Symbols": Load symbols for currently loaded DLLs
* "DLLView-Delete DLL Symbols": Remove symbols of all loaded DLLs
* "DLLView-Delete All Symbols": Remove all loaded symbols (including base image symbols)
*
* Usage:
* Load the symbols:
*   Scripts -> DLLView-Load Base Image Symbols (if code already loaded)
*   Scripts -> DLLView-Load DLL Symbols
*
* Note: 
* 1) "DLLView-Load Base Image Symbols" would be used if the code is already 
*    loaded (like with an OMAP device using DSP Bridge where the ARM would load 
*    the code on to the DSP).
* 2) The “DLLView-Load DLL Symbols” function has to be re-run whenever a new 
*    DLL is loaded by the dynamic loader.
*
*/
var foo;
//The 'imagepath' and 'symbolFileFolder' variables will need to be defined
//before running the script functions. You can hardcode them here or set them 
//from the Scripting Console:
//Ex:     
//js:> imagepath="C:/path/to/baseimage.out;
//js:> symbolFileFolder=C:/path/to/symbolfiles"

var imagepath = "C:/path/to/baseimage.out";
//var symbolFileFolder="C:/path/to/symbolfiles";
var symbolFileFolder="C:/unified_dyn_loader";

// Use active debug session. This assumes that the user has started a debug
// session in CCS4 and running this script from the Scripting Console. This is
// the expected use case 

var filepath="";
var ldCount=0;
var ldSymbols = new Array();

// This must be kept in sync with the constants in TIShell_MemDefines.h and the TI linker config files.
var kLoaderDebugBlockAddress = 0x80000000;


/**
 * Return active debug session
 * @param none
 */
 function GetActiveDebugSession()
{

    return ds.openSession("*", "*");
}

/**
 * Connect to the target non-intrusively
 * @param none
 */
function dllView_attachToDSP()
{
    var debugSession = GetActiveDebugSession();
    // currently have to configure CCS4 to enable legacy real-time mode for 
    // non-intrusive connect
    debugSession.target.connect();  
}


/**
 * Load the base image symbols
 * @param none
 */
function GetTargetList( oBoards, oCPUs )
{
    var inErrStr = new String();
    try
    {
        var debugSession = ds.openSession();        
    }
    catch( x )
    {
        inErrStr = x.toString();
    }

    var curOffset = inErrStr.indexOf( "\n" );
    if( curOffset == -1 )
        return;
    ++curOffset;
    do {
    	var nextLineOffset = inErrStr.indexOf( "\n", curOffset );
    	if( nextLineOffset == curOffset )
    	    break;
    	if( nextLineOffset == -1 )
    	{
    	    if( inErrStr.length > curOffset )
    	        nextLineOffset = inErrStr.length;
    	    else
    	        break;
    	}
    	    
    	var curLineDividerOffset = inErrStr.lastIndexOf( "/", nextLineOffset );
    	if( !(curLineDividerOffset == -1 || curLineDividerOffset < curOffset) )
    	{
    	    oBoards.push( inErrStr.slice( curOffset, curLineDividerOffset ) );
    	    oCPUs.push( inErrStr.slice( curLineDividerOffset + 1, nextLineOffset ) )
    	}
    	curOffset = nextLineOffset + 1;
    } while( curOffset < inErrStr.length );
}


/**
 * Load the base image symbols
 * @param none
 */
function dllView_loadBaseImageSymbols()
{
    var debugSession = GetActiveDebugSession();
    // Load the base image symbols (but not the code, that's loaded from the 
    // ARM side of DSP/BIOS Bridge) from the ELF base image (imagepath) you have 
    // indicated. This image must of course correspond to the ELF image loaded 
    // by the ARM side of DSP/BIOS Bridge. 
    //
    // (Note: that the DSP is still running after this has been executed.)
    //
    // If the code is manually loaded (with CCS), then the symbols will be 
    // loaded also, eliminating the need to load the symbols for the base image

    // needs 'imagepath' to be properly set        
    print("imagepath: " + imagepath);

    debugSession.symbol.load(imagepath);
}

/**
 * Load symbols for currently loaded DLLs
 * @param none
 */
function dllView_loadDynamicNodeSymbols()
{
    var boardList = new Array();
    var cpuList = new Array();
    
    GetTargetList( boardList, cpuList );

    dllView_deleteAllSymbols( );
    
    var nTargets = cpuList.length;
    print( "Found " + nTargets.toString() + " debug targets:" );
    var nConnectedTargets = 0;
    for( i = 0; i < nTargets; i++ )
    {
        var debugSession = ds.openSession( boardList[i], cpuList[i] );        
        var isConnected = debugSession.target.isConnected();
        print( "Board: " + boardList[i]  + "  CPU: " + cpuList[i] + "   Connected: " + ( isConnected ? "true" : "false" ) );
        if( isConnected )
        {
            dllView_loadDynamicNodeSymbolsForTarget( debugSession );
            nConnectedTargets++;
        }
    }
    if( nConnectedTargets == 0 )
        print( "\n\n\n\n\n**** Debugger not connected to any DSPs!  Connect debugger to dsp target first.  ***** " );
}

/**
 * Load symbols for currently loaded DLLs
 * @param none
 */
function dllView_loadDynamicNodeSymbolsForTarget( debugSession )
{
    /* 
    * "Load dynamic node symbols" will read the dynamic loader entry table on 
    * the target and determine the name of the ELF file it should load to get 
    * the symbols. 
    *
    * The Module Debug List Header data object provides the location and size of 
    * the first Module Debug Record on the list.  A Module Debug List Header 
    * object looks like this:
    *
    * +---------------------------------------------+
    * | target address of first module debug record | 32-bits unsigned
    * +---------------------------------------------+
    * | size (bytes) of first module debug record   | 16-bits unsigned 
    * +---------------------------------------------+
    * | update flag                                 | 16-bits unsigned
    * +---------------------------------------------+
    */
    var foo;
    var dl_modules_addr = kLoaderDebugBlockAddress;

    print("dl_modules_addr: 0x" + dl_modules_addr.toString(16));

    // This line hangs with loadJSFile "myscript.js" true:
    var dll_info_head = debugSession.memory.readData(0, dl_modules_addr, 32);

    print("dll_info_head: 0x" + dll_info_head.toString(16));

    if (dll_info_head==0)
    {
      print("*********************************");
      print("**** No Dynamic Node Loaded *****");
      print("*********************************");
    }

    // keep loading symbols for all records until finished
    while (dll_info_head!=0)
    {
      print("");

      // load symbols for specified debug module
      dllView_dynLoad(debugSession, dll_info_head);

      // get address of next module debug record
      var dll_info_head = debugSession.memory.readData(0, dll_info_head, 32);
      
      ldCount++;
    }
}


/**
 * Remove all loaded symbols (including base image symbols)
 * @param none
 */
function dllView_deleteAllSymbolsForTarget( debugSession )
{
    // remove all symbols
    // need a DSS API. Use GEL for now
    debugSession.expression.evaluate("GEL_UnloadAllSymbols()");
    
}

/**
 * Remove all loaded symbols (including base image symbols)
 * @param none
 */
function dllView_deleteAllSymbols( )
{
    var boardList = new Array();
    var cpuList = new Array();
    
    GetTargetList( boardList, cpuList );

    var nTargets = cpuList.length;
    print( "Found " + nTargets.toString() + " debug targets:" );
    for( i = 0; i < nTargets; i++ )
    {
        var debugSession = ds.openSession( boardList[i], cpuList[i] );        
        var isConnected = debugSession.target.isConnected();
        print( "Board: " + boardList[i]  + "  CPU: " + cpuList[i] + "   Connected: " + ( isConnected ? "true" : "false" ) );
        if( isConnected )
            dllView_deleteAllSymbolsForTarget( debugSession );
    }    
}


/**
 * Remove symbols of all loaded DLLs
 * @param none
 */
function dllView_deleteSymbols()
{
    var debugSession = GetActiveDebugSession();
    // remove previously loaded symbols
    // need a DSS API. Use GEL for now
    var i=0;

    for(i=0;i<ldCount;i++)
    {
        debugSession.expression.evaluate('GEL_SymbolRemove("' + ldSymbols[i] + '")');
    }
    
    ldCount = 0; 
}


/**
 * Load symbols for specified DLL
 * @param dll_info_ptr Address of DLL record
 */
function dllView_dynLoad(debugSession, dll_info_ptr)
{    
    /* 
    * Called by dllView_loadDynamicNodeSymbols(). Gets the load/run addresses 
    * for each segment in the module debug record and calls dllView_ldDll()
    * 
    * The DLL Module Debug List is a singly linked list of 
    * "module debug records".  Each module debug record in the list
    * is associated with an object module that is dynamically linked 
    * and loaded.  
    * 
    * The major difference between the DLL debug support in the DOFF 
    * dynamic loader and the ELF dynamic loader is in the details of
    * the module debug record that is generated for each module.  
    * 
    * Whereas the DOFF dynamic linker provided the name of each section
    * in the module and the section's load and run addresses, the ELF 
    * dynamic loader provides the load and run addresses for each 
    * *segment* that occupies space in target memory.
    * 
    * ELF segments do not have names, so the only string appearing at the
    * end of a given module debug record will be the module name.  The 
    * segment addresses will be listed in the module debug record in the
    * same order that the segments occur in the program header table of 
    * the ELF object file.
    * 
    * Here is the ELF version of the example module debug record 
    * (mr_pink.dll with 3 segments):
    * 
    * +--------------------------------------------+
    * | target address of next module debug record | 32-bits unsigned
    * +--------------------------------------------+
    * | size (bytes) of next module debug record   | 16-bits unsigned 
    * +--------------------------------------------+
    * | tool version identification                | 16-bits unsigned
    * +--------------------------------------------+
    * | verification word                          | 16-bits unsigned
    * +--------------------------------------------+
    * | number of segments                         | 16-bits unsigned
    * +--------------------------------------------+
    * | timestamp                                  | 32-bits unsigned
    * +--------------------------------------------+
    * | load address of 1st segment                | 32-bits unsigned
    * | run address of 1st segment                 | 32-bits unsigned
    * +--------------------------------------------+
    * | load address of 2nd segment                | 32-bits unsigned
    * | run address of 2nd segment                 | 32-bits unsigned
    * +--------------------------------------------+
    * | load address of 3rd segment                | 32-bits unsigned
    * | run address of 3rd segment                 | 32-bits unsigned
    * +--------------------------------------------+
    * | "c:\dev\etc\etc\mr_pink.dll"               | N bytes
    * +--------------------------------------------+
    */

    print("**** Module Infomation ****");
    print("dll_info_ptr: 0x" + dll_info_ptr.toString(16));
    
    // Get number of sections for the module
    var num_sects = debugSession.memory.readData(0, (dll_info_ptr + (5 * 2)), 16);
    
    // load address of first section
    var sect_addr_ptr = dll_info_ptr + (8 * 2);
    
    // start address of module name string
    var module_name_ptr = sect_addr_ptr + ((num_sects * 4) * 2);
    
    // get the a string with the name of the module
    var module_name = dllView_dataString(debugSession, module_name_ptr);
    
    // start address of the first name of the list of modules names
    //var sect_name_ptr = module_name_ptr + module_name.length + 1; 
    
    var reloc_info = "";
    
    print("num_sects: " + num_sects);
    
    if( num_sects > 40 )
    {    
        print("ERROR: Too many segments.  Exiting. ");
        return;
    }

    print("module_name: " + module_name);
    
    print("** Segment Details **");    
    
    var index = 0;
    
    var sect_num_arr = new Array();
    var sect_addr_ld_arr = new Array();
    var sect_addr_run_arr = new Array();   
    
    // get the name and load/run address of each section and add to array
    while (index!=num_sects)
    {
        var sect_num = index+1;
        var sect_addr_ld = debugSession.memory.readData(0, sect_addr_ptr, 32);
        var sect_addr_run = debugSession.memory.readData(0, (sect_addr_ptr + 4), 32);
        
        print("Segment " + sect_num + "--addr ld: 0x" + sect_addr_ld.toString(16));
        print("Segment " + sect_num + "--addr run: 0x" + sect_addr_run.toString(16));        
        
        // increment pointer to start of next load address
        sect_addr_ptr = sect_addr_ptr + (4 * 2);
                
        // put section information in arrays
        sect_num_arr[index] = sect_num;
        sect_addr_ld_arr[index] = sect_addr_ld;
        sect_addr_run_arr[index] = sect_addr_run;        
        
        index = index + 1;        
    }
    
    
    dllView_ldDll(debugSession, module_name, sect_num_arr, sect_addr_ld_arr, sect_addr_run_arr);
}


/**
 * Loads symbols with relocation information
 * @param filename DLL name
 * @param sect_num_arr Array of segment numbers
 * @param sect_addr_ld_arr Array of segment load addresses
 * @param sect_addr_run_arr Array of segment run addresses  
 */
function dllView_ldDll(debugSession, filepath, sect_num_arr, sect_addr_ld_arr, sect_addr_run_arr)
{
    
    print("filepath: " + filepath);
    
    // check to see if the file path + file exists
    var filePath = new java.io.File(filepath);  
    var result = filePath.exists();   
  
    if(!result)
    {
        print(filepath + " does not exist! Please question everything."); 
    } 
    else if( filepath.substring(filepath.length - 3, filepath.length) == "out" )
    {
        // We don't relocate .out files - load symbols directly
        print( "Loading symbols non-relocated for " + filepath );
        debugSession.symbol.load( filepath );
    }
    else
    {
        // load symbols

        // if relocation information in symbol file    
        //debugSession.symbol.load(filepath);  
        
        // else need to specify relocation information
        var i;
        
        var cpu_name = debugSession.getCPUName();
        var board_name = debugSession.getBoardName();
        
        var load_start;
        var run_start;        
        var seg_run_addresses = "";
        
        // get load/run addresses for segments
        for(i=0;i<sect_num_arr.length;i++)
        {
            print("segment_num: "+ sect_num_arr[i]);
        
            load_start = sect_addr_ld_arr[i];
            print("load_start: 0x"+ load_start.toString(16));
              
            run_start = sect_addr_run_arr[i];
            print("run_start: 0x"+ run_start.toString(16));
            
            seg_run_addresses = seg_run_addresses + ',' + '0x' + run_start.toString(16);
            
        }

        // Load symbol information with relocation information
        // Use GEL function until equivalent DSS API available
        
        // A new GEL function for ELF support is added:
        // GEL_SymbolAddELFRel( file_name, cpu_name, board_name, segment_run_address, ...)

        // The function accepts the elf file name, cpu name and the board name 
        // and a number of segment_run_address. Note  that the number of segment_
        // run_address are not fixed        
        print( 'GEL_SymbolAddELFRel("' + filepath + '","' + cpu_name + '","' + board_name + '"' + seg_run_addresses + ')' );
        debugSession.expression.evaluate('GEL_SymbolAddELFRel("' + filepath + '","' + cpu_name + '","' + board_name + '"' + seg_run_addresses + ')');
        
        ldSymbols[ldCount] = filepath;
    }
}


/**
 * Read zero-terminated string from memory, the result is a string.
 * @param stringaddr
 */
function dllView_dataString(debugSession, stringaddr)
{
    var ch="";
    var str="";

    while( (debugSession.memory.readData(0, stringaddr, 8) != 0 ) )
    {    
        ch = String.fromCharCode( debugSession.memory.readData(0, stringaddr, 8) );
        stringaddr++;
        str = str + ch;
    }
    
    return str; 
}


// define scripts hotmenu entries
//hotmenu.addJSFunction("DLLView-Attach to DSP", "dllView_attachToDSP()");
//hotmenu.addJSFunction("DLLView-Load Base Image Symbols", "dllView_loadBaseImageSymbols()");
hotmenu.addJSFunction("DLLView-Load Pro Tools Plug-In Symbols", "dllView_loadDynamicNodeSymbols()");
//hotmenu.addJSFunction("DLLView-Delete DLL Symbols", "dllView_deleteSymbols()");
hotmenu.addJSFunction("DLLView-Delete All Symbols", "dllView_deleteAllSymbols()");
