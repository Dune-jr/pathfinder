--------------
--We use the build system http://matricks.github.com/bam/[bam]. We have a customized
--build file that resolves module and library dependencies in a simple way for each project.
--------------
CheckVersion("0.4")

Import("configure.lua")

--- Setup Config -------
config = NewConfig()
config:Add(OptCCompiler("compiler"))
config:Add(OptTestExecute("otawa", "otawa-config", "", false))
config:Add(OptLibrary("cvc4", "cvc4/cvc4.h", false))
config:Add(OptLibrary("z3", "z3++.h", false))
config:Add(OptString("solver", "cvc4"))
config:Finalize("config.lua")


-- Settings
settings = NewSettings()
-- settings.cc.flags:Add("-O1")
settings.cc.flags:Add("-O1")
-- settings.cc.flags:Add("-std=c++11")

if ScriptArgs["otawa"] and ScriptArgs["otawa"]~="2" then
	otawa_config = "" --v1
else
	otawa_config = "otawa-config" -- v2
end

-- Wall option
if ScriptArgs["wall"]=="true" or ScriptArgs["Wall"]=="true" then
	settings.cc.flags:Add("-Wall -Wno-maybe-uninitialized")
end

-- OTAWA
settings.cc.flags:Add("`" .. otawa_config .. " --cflags`")
settings.link.flags:Add("`" .. otawa_config .." --libs --rpath`") -- -Wl,-rpath -Wl,$ORIGIN/home/jruiz/o/linux-x86_64/otawa-core/lib
-- settings.link.flags:Add("`" .. otawa_config .." --libs --rpath otawa/oslice otawa/clp otawa/graphviz`")
-- settings.cc.flags:Add("-D OSLICE") -- slice
-- settings.cc.flags:Add("-D NDEBUG") -- optimize

-- SMT choice
-- CVC4
if config.solver.value == "cvc4" then
	settings.cc.flags:Add("-Wno-deprecated")
	settings.link.flags:Add("-lcvc4")
	-- libgmp
	settings.link.flags:Add("-lgmp")
	-- set C++ macro
	settings.cc.flags:Add("-D SMT_SOLVER_CVC4")
	-- set sources
	source = Collect("src/*.cpp", "src/dom/*.cpp", "src/cvc4/*.cpp")
-- Z3
elseif config.solver.value == "z3" then
	settings.link.flags:Add("-lz3")
	-- set C++ macro
	settings.cc.flags:Add("-D SMT_SOLVER_Z3")
	-- set sources
	source = Collect("src/*.cpp", "src/dom/*.cpp", "src/z3/*.cpp")
-- Boolector
elseif config.solver.value == "boolector" then
	settings.link.flags:Add("-lboolector")
 	-- lingeling
 	settings.link.flags:Add("-llgl")
 	-- no support yet
	error("boolector not yet supported")
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
