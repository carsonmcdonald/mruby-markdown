MRuby::Gem::Specification.new('mruby-markdown') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Carson McDonald'
  spec.version = '0.2.0'
  spec.description = 'Markdown for mruby using the sundown markdown library.'
  spec.homepage = 'https://github.com/carsonmcdonald/mruby-markdown'
  
  spec.cc.flags << "-I #{spec.dir}/src/sundown/src/"

  spec.objs += Dir.glob("#{dir}/src/sundown/src/*.{c,cpp,m,asm,S}").map { |f| f.relative_path_from(dir).pathmap("#{build_dir}/%X#{spec.exts.object}") }
  spec.objs += Dir.glob("#{dir}/src/sundown/html/*.{c,cpp,m,asm,S}").map { |f| f.relative_path_from(dir).pathmap("#{build_dir}/%X#{spec.exts.object}") }
end
