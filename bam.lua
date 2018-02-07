--------------
--We use the build system http://matricks.github.com/bam/[bam]. We have a customized
--build file that resolves module and library dependencies in a simple way for each project.
--
--Available options:
-- enable all gcc warnings
-- 		bam Wall=[true/false] (default: false)
-- set optimization level
-- 		bam O=[0/1/2/3] (default: 0)
-- 
-- 
--------------
CheckVersion("0.4")

Import("configure.lua")

--- Setup Config -------
config = NewConfig()
config:Add(OptCCompiler("compiler"))
config:Add(OptTestExecute("otawa", "otawa-config", "", "Checks for otawa-config in path"))
config:Add(OptLibrary("cvc4", "cvc4/cvc4.h", "Set the path to CVC4"))
config:Add(OptLibrary("z3", "z3++.h", "Set to path to z3"))
config:Add(OptString("solver", "cvc4", "Select an SMT solver"))
config:Add(OptString("O", "0", "Set optimization level (0/1/2/3)"))
config:Add(OptToggle("Wall", 	true, 	"Enable all GCC warnings"))
config:Add(OptToggle("debug", 	true, 	"Enable OTAWA debugging"))
config:Add(OptToggle("utf8",	true, 	"Enable UTF-8 pretty printing"))
config:Add(OptToggle("slice", 	false, 	"Enable support for slicing"))
config:Add(OptToggle("warnings",false, 	"Enable PathFinder warnings"))
config:Add(OptToggle("v1", 		false, 	"Enable support for v1 analysis"))
config:Finalize("config.lua")

if ScriptArgs["--help"] then
	config.PrintHelp()
end

-- Settings
settings = NewSettings()
otawa_config = "otawa-config"

if config.Wall.value then
	settings.cc.flags:Add("-Wall")
end
if config.slice.value then
	settings.cc.flags:Add("-D OSLICE")
	settings.link.flags:Add("`" .. otawa_config .." --libs --rpath otawa/oslice otawa/clp otawa/graphviz`")
end
if config.debug.value == false then
	settings.cc.flags:Add("-D NDEBUG")
end
if config.utf8.value == false then
	settings.cc.flags:Add("-D NO_UTF8")
end
if config.warnings.value then
	settings.cc.flags:Add("-D DBG_WARNINGS")
end
if config.v1.value then
	settings.cc.flags:Add("-D V1")
end
settings.cc.flags:Add("-O" .. config.O.value)
settings.cc.flags:Add("-std=c++11")
settings.cc.flags:Add("-Wno-maybe-uninitialized")

-- OTAWA
settings.cc.flags:Add("`" .. otawa_config .. " --cflags`")
settings.link.flags:Add("`" .. otawa_config .." --libs --rpath`") -- -Wl,-rpath -Wl,$ORIGIN/home/jruiz/o/linux-x86_64/otawa-core/lib
-- SMT choice
-- CVC4
if config.solver.value == "cvc4" then
	settings.cc.flags:Add("-Wno-deprecated")
	settings.link.flags:Add("-lcvc4")
	settings.link.flags:Add("-lgmp") -- libgmp
	settings.cc.flags:Add("-D SMT_SOLVER_CVC4") -- set C++ macro
	source = Collect("src/*.cpp", "src/struct/*.cpp", "src/dom/*.cpp", "src/v1/*.cpp", "src/v2/*.cpp", "src/cvc4/*.cpp") -- set sources
-- Z3
elseif config.solver.value == "z3" then
	settings.link.flags:Add("-lz3") 
	settings.cc.flags:Add("-D SMT_SOLVER_Z3") -- set C++ macro 
	source = Collect("src/*.cpp", "src/struct/*.cpp", "src/dom/*.cpp", "src/v1/*.cpp", "src/v2/*.cpp", "src/z3/*.cpp") -- set sources
-- Boolector
elseif config.solver.value == "boolector" then
	settings.link.flags:Add("-lboolector")
 	-- lingeling
 	settings.link.flags:Add("-llgl")
 	-- no support yet
	error("boolector not supported")
else
	error("invalid solver")
end


function Intermediate_Output(settings, input)
	return "objs/" .. string.sub(PathBase(input), string.len("src/")+1) .. settings.config_ext
end

-- Sends all objs to an objs/ directory
settings.cc.Output = Intermediate_Output

objects = Compile(settings, source)
exe = Link(settings, "pathfinder", objects)
