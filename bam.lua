--------------
--We use the build system http://matricks.github.com/bam/[bam]. We have a customized
--build file that resolves module and library dependencies in a simple way for each project.
--------------
CheckVersion("0.4")

Import("configure.lua")

--- Setup Config -------
config = NewConfig()
config:Add(OptCCompiler("compiler"))
-- config:Add(OptTestCompileC("stackprotector", "int main(){return 0;}", "-fstack-protector -fstack-protector-all"))
-- config:Add(OptTestCompileC("minmacosxsdk", "int main(){return 0;}", "-mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk"))
-- config:Add(OptTestCompileC("cvc4.cxx", "#include <cvc4/cvc4.h>\nint main(){return 0;}", "-lcvc4 -lgmp -Wno-deprecated"))
-- config:Add(OptLibrary("zlib", "zlib.h", false))
config:Add(OptLibrary("cvc4", "cvc4/cvc4.h", false))
config:Add(OptLibrary("z3", "z3++.h", false))
config:Add(OptString("solver", "cvc4"))	
config:Finalize("config.lua")


-- Settings
settings = NewSettings()
settings.cc.flags:Add("-O0")
otawa_config = "~/bin/otawa/otawa-config"

-- Wall option
if ScriptArgs["wall"]=="true" or ScriptArgs["Wall"]=="true" then
	settings.cc.flags:Add("-Wall")
end

-- OTAWA
settings.cc.flags:Add("`" .. otawa_config .. " --cflags`")
settings.link.flags:Add("`" .. otawa_config .." --libs --rpath`")
	
-- SMT choice
if config.solver.value == "cvc4" then
	-- CVC4
	settings.link.flags:Add("-lcvc4")
	settings.cc.flags:Add("-Wno-deprecated")
	-- libgmp
	settings.link.flags:Add("-lgmp")
	-- set C++ macro
	settings.cc.flags:Add("-D SMT_SOLVER_CVC4")
	-- set sources
	source = Collect("src/*.cpp", "src/cvc4/*.cpp")
elseif config.solver.value == "z3" then
	-- Z3
	settings.link.flags:Add("-lz3")
	-- set C++ macro
	settings.cc.flags:Add("-D SMT_SOLVER_Z3")
	-- set sources
	source = Collect("src/*.cpp", "src/z3/*.cpp")
else
	error("invalid solver")
end


function Intermediate_Output(settings, input)
	return "objs/" .. string.sub(PathBase(input), string.len("src/")+1) .. settings.config_ext
end

-- Sends all objs to an objs/ directory
settings.cc.Output = Intermediate_Output

-- source = Collect("src/*.cpp", "src/cvc4/*.cpp", "src/z3/*.cpp")
objects = Compile(settings, source)
exe = Link(settings, "pathfinder", objects)
