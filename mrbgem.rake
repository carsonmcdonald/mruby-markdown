MRuby::Gem::Specification.new('mruby-markdown') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Carson McDonald'
  
  spec.cc.flags << "-I #{spec.dir}/src/sundown/src/"

  spec.objs += Dir.glob("#{dir}/src/sundown/src/*.{c,cpp,m,asm,S}").map { |f| f.relative_path_from(dir).pathmap("#{build_dir}/%X.o") }
  spec.objs += Dir.glob("#{dir}/src/sundown/html/*.{c,cpp,m,asm,S}").map { |f| f.relative_path_from(dir).pathmap("#{build_dir}/%X.o") }
end
