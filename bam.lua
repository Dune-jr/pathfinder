
-- Settings
settings = NewSettings()
-- Loading otawa
if true or ScriptArgs["wall"]=="true" or ScriptArgs["Wall"]=="true" then
	settings.cc.flags:Add("-Wall")	
end
settings.cc.flags:Add("`/home/jruiz/Documents/otawa/bin/otawa-config --cflags`")
settings.link.flags:Add("`/home/jruiz/Documents/otawa/bin/otawa-config --libs --rpath`")

function Intermediate_Output(settings, input)
	return "objs/" .. string.sub(PathBase(input), string.len("src/")+1) .. settings.config_ext
end

-- Sends all objs to an objs/ directory
settings.cc.Output = Intermediate_Output

source = Collect("src/*.cpp")
objects = Compile(settings, source)
exe = Link(settings, "pathfinder", objects)
