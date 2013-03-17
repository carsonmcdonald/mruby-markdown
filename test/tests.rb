assert('Simple markdown test') do
  input = "This is an H1\n============="

  md = Markdown::Markdown.new
  output = md.render(input)

  output == "<h1>This is an H1</h1>\n"
end
