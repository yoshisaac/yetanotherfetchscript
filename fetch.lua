function parse_shell(shell)
   if string.find(shell, "bash") ~= nil and string.find(shell, "bash") > 0 then
      return "bash"
   end

   if string.find(shell, "zsh") ~= nil and string.find(shell, "zsh") > 0 then
      return "zsh"
   end   
   
   return shell
end

function parse_term(term)
   if string.find(term, "kitty") ~= nil and string.find(term, "kitty") > 0 then
      return "kitty"
   end
   
   return term
end

function parse_cpuinfo(cpuinfo)

   --parse /proc/cpuinfo file
   local current_line   = ""
   local past_colon     = false
   local cpu_model      = ""
   local read_cpu_model = false
   
   for char in cpuinfo:lines(1) do
      if char == '\n' then
	 current_line = ""
	 past_colon = false
	 read_cpu_model = false
	 goto continue
      end
      if char == ':'  then
	 past_colon = true
	 goto continue
      end

      current_line = current_line .. char

      if current_line == "model name" then read_cpu_model = true end
      
      if read_cpu_model and past_colon then
	 cpu_model = cpu_model .. char
      end

      if current_line == "power management" then break end
      
      ::continue::
   end

   local cpu_model_rebuild = ""
   local cpu_i = string.find(cpu_model, "CPU")
   local i = 1
   for _=0, string.len(cpu_model) do
      if i == cpu_i then i = i + string.len("CPU") + 1 end
      
      cpu_model_rebuild = cpu_model_rebuild .. cpu_model:sub(i, i)
      i = i + 1
   end

   
   return cpu_model_rebuild
end

function sys_info()
   local kernel   = io.popen("uname -r", "r"):read()
   local arch     = io.popen("uname -m", "r"):read()
   local hostname = io.popen("uname -n", "r"):read()
   local whoami   = io.popen("whoami",   "r"):read()
   local shell    = parse_shell(os.getenv("SHELL"))
   local term     = parse_term(os.getenv("TERM"))
   local cpuinfo  = parse_cpuinfo(io.open("/proc/cpuinfo", "r"))
   
   return kernel, arch, whoami, hostname, shell, term, cpuinfo
end

function main()
   local kernel, arch, whoami, hostname, shell, term, cpuinfo = sys_info()
   
   print("Kernel: " .. kernel)
   print("Shell: "  .. shell)
   print("CPU: "    .. arch .. cpuinfo)
   print("Host: "   .. hostname)
   print("User: "   .. whoami)
   print("Term: "   .. term)

end

main()
