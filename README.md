mruby-markdown
==============

Markdown for mruby using the [Sundown library](https://github.com/vmg/sundown).

## Installation

In the mruby build_config.rb file add:

conf.gem :git => 'git://github.com/carsonmcdonald/mruby-markdown.git', :branch => 'master', :options => '--recursive'

In the mruby directory:

```bash
mkdir mrbgems
cd mrbgems
git clone --recursive git://github.com/carsonmcdonald/mruby-markdown.git
```

Then in your build config add:

```ruby
MRuby::Build.new do |conf|
  
  conf.gem 'mrbgems/mruby-markdown'

end
```

## Example Use

```ruby
md = Markdown::Markdown.new
puts md.render("Working\n========")
```

or

```ruby
md = Markdown::Markdown.new(:tables => true)
puts md.render("Working\n========")
```

## Options

You can pass the following options when creating a new Markdown instance:

  - :no_intra_emphassis - Disable emphasis parsing inside of words.
  - :autolink - Enable link parsing when the link is not enclosed in <> characters.
  - :space_headers - Enable strict checking of a space between a name and the hash at the beginning of a header.
  - :superscript - Enable the superscripts ^ character.
  - :lax_spacing - Disable strict checking of empty line checking around HTML blocks.
  - :tables - Enable PHP-Markdown style table parsing.
  - :fenced_code - Enable PHP-Markdown style fenced code blocks.
  - :strikethrough - Enable PHP-Markdown style strikethrough parsing.

## License

MIT - See LICENSE
